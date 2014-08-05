#include "ModelGL.h"


/* Render this model. Depending on mode, different render functions are called. */
void ModelGL::Render () {
  if (_renderMode == OFF) return;

  for (int g = 0; g < _nrGeosets; g ++) {
    if (_geosets[g].enabled) {
      switch (_renderMode) {
        case DIRECT  : renderDirect (_geosets[g]);  break;
        case POINTS  : renderPoints (_geosets[g]);  break;
        case MESH    : renderMesh   (_geosets[g]);  break;
        case VBO     : renderVBO    (_VBOs[g]);     break;
      } 
    }
  }
}



unsigned char lightEnabled = 2;  // 0: disabled, 1: enabled, 2: not assigned

/* Change the rendering mode. */
void ModelGL::ChangeMode (int mode) {
  if (_renderMode == mode) return;
  printf ("Changing rendering mode: [%d] ==> [%d]\n", _renderMode, mode);
  
  // Create VBOs on fist activation.
  if (mode == VBO && _VBOs == NULL) buildVBOs (); 
  
  // Disable lighting for simple output.
  if (mode == MESH || mode == POINTS) {
    if (lightEnabled == 2) glGetBooleanv (GL_LIGHTING, &lightEnabled);
    glDisable (GL_LIGHTING);
  } else if (lightEnabled == 1) glEnable (GL_LIGHTING);
  
  _renderMode = mode;
}



/* Direct model rendering an the CPU. */
void ModelGL::renderDirect (Geoset geoset) {
  glEnable (GL_TEXTURE_2D);
  if (_texture != NULL) glBindTexture (GL_TEXTURE_2D, _texture->id());      
  glBegin (GL_TRIANGLES);
  for (int i = 0, c; i < geoset.nrGeometries; i ++) {
    for (c = 0; c < 3; c ++) {
      glTexCoord2f(geoset.geometries[i].texcoords[c].u, 
                   geoset.geometries[i].texcoords[c].v);
      glNormal3f  (geoset.geometries[i].normals[c].x, 
                   geoset.geometries[i].normals[c].y, 
                   geoset.geometries[i].normals[c].z);
      glVertex3f  (geoset.geometries[i].vertices[c].x, 
                   geoset.geometries[i].vertices[c].y, 
                   geoset.geometries[i].vertices[c].z);
    }   
  }  
  glEnd();
  glDisable (GL_TEXTURE_2D);
}



/* Just vertex point output. */
void ModelGL::renderPoints (Geoset geoset) {
  glBegin (GL_POINTS);      
  for (int i = 0, c; i < geoset.nrGeometries; i ++) {
    for (c = 0; c < 3; c ++) {
      glVertex3f  (geoset.geometries[i].vertices[c].x, 
                   geoset.geometries[i].vertices[c].y, 
                   geoset.geometries[i].vertices[c].z);    
    }   
  }
  glEnd ();
}



/* Render a non-textured mesh of the model. */
void ModelGL::renderMesh (Geoset geoset) {  
  for (int i = 0, c; i < geoset.nrGeometries; i ++) {
    glBegin (GL_LINE_LOOP);
    for (c = 0; c < 3; c ++) {
      glVertex3f  (geoset.geometries[i].vertices[c].x, 
                   geoset.geometries[i].vertices[c].y, 
                   geoset.geometries[i].vertices[c].z);  
    } 
    glEnd();
  }    
}



/* Build VBOs for all geosets. */
void ModelGL::buildVBOs () {

  // Reserve memory and loop over all geosets.
  _VBOs = (GeosetVBO*) calloc (_nrGeosets, sizeof (GeosetVBO));
  for (int g = 0; g < _nrGeosets; g ++) {
    _VBOs[g].length = _geosets[g].nrGeometries * 3;            // Set vertex count.
    if (_texture != NULL) _VBOs[g].textureID = _texture->id(); // Set texture ID.

    // Build temporary arrays based on geometry definitions.
    int vsize = _geosets[g].nrGeometries * 3 * 3; //| 3 vertices per geometry
    int nsize = _geosets[g].nrGeometries * 3 * 3; //| with 3 points, 3 nv-points 
    int tsize = _geosets[g].nrGeometries * 3 * 2; //| and 2 texture coords each.
    float* vdata = new float [vsize];
    float* ndata = new float [nsize];
    float* tdata = new float [tsize];

    // Extract data for each geoset.
    for (int i = 0; i < _geosets[g].nrGeometries; i ++) { // i: Loop over geometries.
      for (int c = 0; c < 3; c ++) {                      // c: Loop over geom indices.

        vdata [i*9 + c*3 + 0] = _geosets[g].geometries[i].vertices[c].x;  // Outer offset: 9 (3*inner).
        vdata [i*9 + c*3 + 1] = _geosets[g].geometries[i].vertices[c].y;  // Inner offset: 3 (3 runs each).
        vdata [i*9 + c*3 + 2] = _geosets[g].geometries[i].vertices[c].z;
        
        ndata [i*9 + c*3 + 0] = _geosets[g].geometries[i].normals[c].x;
        ndata [i*9 + c*3 + 1] = _geosets[g].geometries[i].normals[c].y;
        ndata [i*9 + c*3 + 2] = _geosets[g].geometries[i].normals[c].z;
        
        tdata [i*6 + c*2 + 0] = _geosets[g].geometries[i].texcoords[c].u;  // OO: 6 (2*3)
        tdata [i*6 + c*2 + 1] = _geosets[g].geometries[i].texcoords[c].v;  
      }
    }

    // Create VBOs.
    _VBOs[g].vVBO = createVBO (vdata, vsize * sizeof(float), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    _VBOs[g].nVBO = createVBO (ndata, nsize * sizeof(float), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    _VBOs[g].tVBO = createVBO (tdata, tsize * sizeof(float), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    // Free temporary data.
    delete [] vdata;
    delete [] ndata;
    delete [] tdata;
  }
}



/* Create a vertex buffer object (VBO) to store data in video memory. */
uint ModelGL::createVBO (const void* data, int dataSize, uint target, uint usage) {
  uint id = 0;
  glGenBuffersARB (1, &id);  // Create a vertex buffer object.
  if (id == 0) {             // Check for success.
    printf ("[ERROR] VBO could not be created!\n");
    return 0;
  }
  
  // Select new VBO, update data to video card and return ID.
  glBindBufferARB (target, id);                    
  glBufferDataARB (target, dataSize, data, usage);
  return id;
}



/* Render a VBO on screen. */
void ModelGL::renderVBO (GeosetVBO geosetVBO) {   

  // If textures available, bind their buffer array.
  if (geosetVBO.textureID != -1 && geosetVBO.tVBO != 0) { 
    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, geosetVBO.textureID);  
    glBindBufferARB (GL_ARRAY_BUFFER, geosetVBO.tVBO);
    glTexCoordPointer (2, GL_FLOAT, 0, 0);
  }

  // Enable vertex array. Draw normals (if exist) and the vertices.
  glEnableClientState (GL_VERTEX_ARRAY);
  if (geosetVBO.nVBO != 0) {
    glBindBufferARB (GL_ARRAY_BUFFER, geosetVBO.nVBO);
    glNormalPointer (GL_FLOAT, 0, 0);    
  }

  // Draw the vertices!
  glBindBufferARB (GL_ARRAY_BUFFER_ARB, geosetVBO.vVBO);
  glVertexPointer (3, GL_FLOAT, 0, 0); 
  glDrawArrays (GL_TRIANGLES, 0, geosetVBO.length);
  
  // Disable client states.
  glDisableClientState (GL_VERTEX_ARRAY);  
  glDisable (GL_TEXTURE_2D);
  glDisableClientState (GL_TEXTURE_COORD_ARRAY);
}
