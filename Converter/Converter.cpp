#include "Converter.h"


Converter::Converter () {}
Converter::~Converter () {}


void Converter::LoadFile (const char* filename, const char* savename) {
  
  // Detect file format.
  int fileformat = -1;
  const char* binary [] = {".mdx", ".ms3d",".raw", ".mdl", ".obj"};
  for (int i = 0; i < _nrFormats; i ++) {
    if (strstr (filename, binary[i]) != NULL) {
      fileformat = i;
      break;
    }
  } 
  // Print error and return on unrecognized file format.
  if (fileformat == -1) {
    printf ("\nError reading file \"%s\"! Please make sure it has a proper ending.\n"
            "Formats currently supported: .mdl, .mdx, .obj, .raw, .ms3d\n", filename);
    return;
  }

  // Try to open filestream.
  _fp = fopen (filename, (fileformat<_nrBinary)? "rb" : "r");
  if (_fp == NULL) {
    printf ("\nError opening file \"%s\"!\n", filename);
    return;
  }

  // Get the length of the file and print status output.
  fseek (_fp, 0L, SEEK_END);
  long bytes = ftell (_fp);
  fseek (_fp, 0L, SEEK_SET);
  printf ("\nOpening file \"%s\" [%d bytes].\n", filename, bytes);

  // Execute corresponding converter function.
  switch (fileformat) {
    case MDL :  readMDL();  break;
    case MDX :  readMDX();  break;
    case OBJ :  readOBJ();  break;
    case RAW :  readRAW();  break;
    case MS3D:  readMS3D(); break;
  }

  // Write the model to file, print output and exit.
  if (_model != NULL) {
    printf ("Conversion finished!\n"
            "Saving to \"%s\": ", savename);
    _model->WriteFile (savename);
    delete _model;
    _model = NULL;
  }
  fclose (_fp);
}


//TODO None of these is implemented!
void Converter::readMDL  () {
  printf ("MDL: Not yet implemented!\n");
}
void Converter::readMS3D () {
  printf ("MS3D: Not yet implemented!\n");
}



/* Read a heightmap file. */
void Converter::readRAW () {
  printf ("RAW converter (terrain heightmap interpreter) loaded.\n");
  int LOD = 8;

  // Read in dimensions (width, height) and height data.
  long width, height;  
  fread (&width,  sizeof (long), 1, _fp);
  fread (&height, sizeof (long), 1, _fp);
  BYTE* heights = (BYTE*) calloc (width*height, sizeof (BYTE));
  fread (heights, sizeof (BYTE), width*height, _fp);

  // Create counter and structures for storage.
  int nrVNT = (int) (width*height*6) / (LOD*LOD);  // *6: two triangles based on these points.
  int nrG   = nrVNT / 3;
  Vertex*   vertices   = (Vertex*)   calloc (nrVNT, sizeof (Vertex));
  Vertex*   normals    = (Vertex*)   calloc (nrVNT, sizeof (Vertex));
  TexCoord* textures   = (TexCoord*) calloc (nrVNT, sizeof (TexCoord));
  Geometry* geometries = (Geometry*) calloc (nrG,   sizeof (Geometry));


  // Iterate over all vertices and set up the values.
  for (int x = 0, vI=0, gI=0, tmpX, tmpZ; x < width; x += LOD) {  // X values.
    for (int z = 0; z < height; z += LOD) {                       // Z-values.
      for (int nTri = 0; nTri < 6; nTri ++, vI ++) {              // Triangle comp.
        tmpX = x + ((nTri == 1 || nTri == 2 || nTri == 5) ? LOD : 0);
        tmpZ = z + ((nTri == 1 || nTri == 4 || nTri == 5) ? LOD : 0);   
        vertices[vI].x = (float) tmpX;
        vertices[vI].y = heights[tmpX*width + tmpZ];
        vertices[vI].z = (float) tmpZ;          
        textures[vI].u = (float) tmpX / width;
        textures[vI].v = (float) tmpZ / height;
      }
      
      // Set up geometries.
      for (int geom = 2; geom > 0; geom --, gI ++) {
        geometries[gI].symIndices = true;
        geometries[gI].vIdx[0] = vI - (geom*3);      // No need to copy to n and t. The
        geometries[gI].vIdx[1] = vI - (geom*3 - 1);  // next Load() will do it for us!
        geometries[gI].vIdx[2] = vI - (geom*3 - 2);
      }
    }
  }

  // Remove temporary data and create new model with terrain geoset.
  free (heights);
  _model = new Model3D (00);
  _model->CreateGeoset (0, true, nrVNT, nrVNT, nrVNT, nrG, vertices, normals, textures, geometries); 


  // Print results and return.
  printf ("Terrain interpreter results:\n"
          " - X extent  : %d\n"
          " - Z extent  : %d\n" 
          " - Details   : %d\n"
          " - Vtx/Nm/Tex: %d\n"
          " - Geometries: %d\n", 
          width, height, LOD, nrVNT, nrG);
}



/* Load a Blizzard MDX file. */
void Converter::readMDX  () {
  printf ("MDX converter loaded.\n");
 
  // Iteration variables.
  int g, i;
  DWORD dbuf = 0x00;

  // 3D model, counters and memory pointers.
  _model = new Model3D (00);
  int nrV = 0, nrN = 0, nrT = 0, nrG = 0;
  int totalV = 0, totalN = 0, totalT = 0, totalG = 0; 
  Vertex* vertices     = NULL;
  Vertex* normals      = NULL;
  TexCoord* textures   = NULL;
  Geometry* geometries = NULL;
  

  // Skip file to geoset definitions.
  while (dbuf != 'SOEG') fread (&dbuf, sizeof (DWORD), 1, _fp);

  DWORD remaining;
  fread (&remaining, sizeof (DWORD), 1, _fp);
  for (g = 0; remaining > 0; g ++) {

    // Read and reduce remaining size.
    fread (&dbuf, sizeof (DWORD), 1, _fp);
    remaining -= dbuf;

    // Read vertexes.
    fread (&dbuf, sizeof (DWORD), 1, _fp); // Skip 'VRTX'.
    fread (&nrV, sizeof (DWORD), 1, _fp);
    totalV += nrV;
    vertices = (Vertex*) calloc (nrV, sizeof (Vertex));
    for (i = 0; i < nrV; i ++) {
      fread (&vertices[i].x, sizeof (float), 1, _fp);
      fread (&vertices[i].z, sizeof (float), 1, _fp);  //| Blizzard has y- and
      fread (&vertices[i].y, sizeof (float), 1, _fp);  //| z-axis swapped!
    }

    // Read normals.
    fread (&dbuf, sizeof (DWORD), 1, _fp); // Skip 'NRMS'.
    fread (&nrN,  sizeof (DWORD), 1, _fp);
    totalN += nrN;
    normals = (Vertex*) calloc (nrN, sizeof (Vertex));
    for (i = 0; i < nrN; i ++) {
      fread (&normals[i].x, sizeof (float), 1, _fp);
      fread (&normals[i].z, sizeof (float), 1, _fp);
      fread (&normals[i].y, sizeof (float), 1, _fp);
    }

    // Scan to faces definition.
    while (dbuf != 'XTVP') fread (&dbuf, sizeof (DWORD), 1, _fp);  // Skip 'PVTX'.
    fread (&nrG, sizeof (DWORD), 1, _fp);
    nrG /= 3;  // Only triangles used! 
    totalG += nrG;
    geometries = (Geometry*) calloc (nrG, sizeof (Geometry));
    for (i = 0; i < nrG; i ++) {
      geometries[i].symIndices = true;
      fread (&geometries[i].vIdx[0], sizeof (WORD), 1, _fp);
      fread (&geometries[i].vIdx[1], sizeof (WORD), 1, _fp);
      fread (&geometries[i].vIdx[2], sizeof (WORD), 1, _fp);
    }
    fread (&dbuf, sizeof (DWORD), 1, _fp);  // Skip 'GNDX'.
    fread (&dbuf, sizeof (DWORD), 1, _fp);  // Read vertex group numbers.
    for (i = dbuf; i > 0; i --) fread (&dbuf, sizeof (BYTE), 1, _fp);

    // Skip to texture coordinates [UVBS].
    while (dbuf != 'SBVU') fread (&dbuf, sizeof (DWORD), 1, _fp);
    fread (&nrT, sizeof (DWORD), 1, _fp);
    totalT += nrT;
    textures = (TexCoord*) calloc (nrT, sizeof (TexCoord));
    for (i = 0; i < nrT; i ++) {
      fread (&textures[i].u, sizeof (float), 1, _fp);
      fread (&textures[i].v, sizeof (float), 1, _fp);
    }

    // Add new geoset.
    _model->CreateGeoset (g, true, nrV, nrN, nrT, nrG, vertices, normals, textures, geometries);


    // Scan to 'VRTX' (to skip the rubbish afterwards), then reset to proper start.
    if (remaining > 0) {
      while (dbuf != 'XTRV') fread (&dbuf, sizeof (DWORD), 1, _fp);
      fseek (_fp, -8, SEEK_CUR);
    }
  }

  // Print parser results.
  printf ("Parser results:  \n"
          " - Geosets   : %d\n"
          " - Vertexes  : %d\n"
          " - Normals   : %d\n"
          " - Textures  : %d\n"
          " - Geometries: %d\n", 
            g, totalV, totalN, totalT, totalG);
}



/* Load an OBJ file (Blender). */
void Converter::readOBJ  () {
  printf ("OBJ converter loaded.\n");

  int v, vn, vt;       // Index variables for faces match.
  char buffer [256];   // Input buffer for file read-in.
  char mtlfile [80];   // Material library file name.


  // First run: Determine geosets and array sizes.
  int geos = 1;
  LONG4* counters;    // Group member counters (zero is default).
  counters = (LONG4*) malloc (sizeof (LONG4)); 
  counters[0] = LONG4();
  while (fgets (buffer, 256, _fp) != NULL) {
  
    // A new group was found. Add counters for it.
    if (startsWith (buffer, "g ")) {
      if (counters[geos-1].v != 0) { // Skip empty definitions.
        geos ++;
        counters = (LONG4*) realloc (counters, sizeof (LONG4) * geos);
        counters[geos-1] = LONG4 ();
      }
    }

    // Increase counter values.
    else if (startsWith (buffer, "v "))  counters[geos-1].v ++;   
    else if (startsWith (buffer, "vn ")) counters[geos-1].n ++;
    else if (startsWith (buffer, "vt ")) counters[geos-1].t ++;
    else if (startsWith (buffer, "f "))  counters[geos-1].g ++;
  }

  // Add for total numbers.
  long tV = 0, tN = 0, tT = 0, tG = 0;
  for (int i = 0; i < geos; i ++) {
    tV += counters[i].v;
    tN += counters[i].n;
    tT += counters[i].t;
    tG += counters[i].g;
  }

  // Allocate heap memory for first run, also create model.
  int curGeo = 0;
  _model = new Model3D (00);  
  Vertex*   vertices   = (Vertex*)   calloc (counters[curGeo].v, sizeof (Vertex));
  Vertex*   normals    = (Vertex*)   calloc (counters[curGeo].n, sizeof (Vertex));
  TexCoord* textures   = (TexCoord*) calloc (counters[curGeo].t, sizeof (TexCoord));
  Geometry* geometries = (Geometry*) calloc (counters[curGeo].g, sizeof (Geometry));
  std::vector<Material*> materials = std::vector<Material*>();


  // Second run: Gather data and load them into structures.
  fseek (_fp, 0L, SEEK_SET);
  int curV = 0, curN = 0, curT = 0, curG = 0;   // Current index (loop counter).
  int foV = 1, foN = 1, foT = 1;                // Faces offsets (index dereferencing).
  while (fgets (buffer, 256, _fp) != NULL) {

    if (startsWith (buffer, "v ")) {
      sscanf (buffer, "v %f %f %f", &vertices[curV].x, &vertices[curV].z, &vertices[curV].y);
      curV ++;
    }

    else if (startsWith (buffer, "vn ")) {
      sscanf (buffer, "vn %f %f %f", &normals[curN].x, &normals[curN].z, &normals[curN].y);
      curN ++;
    }

    else if (startsWith (buffer, "vt ")) {
      sscanf (buffer, "vt %f %f", &textures[curT].u, &textures[curT].v);
      curT ++;
    }

    else if (startsWith (buffer, "f ")) {    
      int count = 0;
      long* indices = NULL;

      // Loop over all entries and aggregate geometry components.
      char* splitPtr = strtok (buffer, " ");  // Split of first segment.      
      while (splitPtr != NULL) {  
        count ++;
        //if (count == 4) break;  // Break on invalid mesh.
        indices = (long*) realloc (indices, count * sizeof (long));

        // Dereference the vertices, normals and texture crap.
        if (sscanf (splitPtr, "%d/%d/%d", &v, &vt, &vn) == 3) {
          geometries[curG].vIdx [count-1] = v  - foV;
          geometries[curG].nIdx [count-1] = vn - foN;
          geometries[curG].tIdx [count-1] = vt - foT;      //TODO What if there are no n or t vrtx?
        }
        else if (sscanf (splitPtr, "%d//%d", &v, &vn) == 2) {
          geometries[curG].vIdx[count-1] = v  - foV;
          geometries[curG].nIdx[count-1] = vn - foN;
        }
        else if (sscanf (splitPtr, "%d", &v) == 1) {
          geometries[curG].vIdx[count-1] =  v - foV;
        }
        else count --;

        splitPtr = strtok (NULL, " ");
      } 
      curG ++;
    }

    // Load material (mtllib).
    else if (startsWith (buffer, "mtllib ")) {
      sscanf (buffer, "mtllib %s", &mtlfile);
      readMTL (mtlfile, &materials);
    }


    // If geoset group is finished, write it to model!
    if (curV == counters[curGeo].v && curN == counters[curGeo].n && 
        curT == counters[curGeo].t && curG == counters[curGeo].g) {

      _model->CreateGeoset (curGeo, true, counters[curGeo].v, counters[curGeo].n, 
                            counters[curGeo].t, counters[curGeo].g, 
                            vertices, normals, textures, geometries);
      curGeo ++;
      foV += curV; foN += curN; foT += curT;  // Increase faces offset
      curV = 0; curN = 0; curT = 0; curG = 0; // Reset counters.

      // Still geosets left? Re-allocate memory then!
      if (curGeo < geos) {
        vertices   = (Vertex*)   calloc (counters[curGeo].v, sizeof (Vertex));
        normals    = (Vertex*)   calloc (counters[curGeo].n, sizeof (Vertex));
        textures   = (TexCoord*) calloc (counters[curGeo].t, sizeof (TexCoord));
        geometries = (Geometry*) calloc (counters[curGeo].g, sizeof (Geometry));    
      }
    }
  }

  // Print parser results.
  printf ("Parser results:  \n"
          " - Geosets   : %d\n"
          " - Vertexes  : %d\n"
          " - Normals   : %d\n"
          " - Materials : %d\n"
          " - Tex Coords: %d\n"
          " - Geometries: %d\n", 
            geos, tV, tN, materials.size(), tT, tG);
}



void Converter::readMTL (const char* filename, std::vector<Material*>* materials) {
  FILE* fp = fopen (filename, "r");
  
  // Print error, if file is not existent.
  if (fp == NULL) { 
    printf ("Error: Material library \"%s\" could not be opened!\n", filename);
    return;
  }

  // File successfully opened. Start parsing.
  Material* mtl = NULL; // Current material to set. 
  char buffer [256];    // Buffer for reader. 
  
  printf ("Loading material library \"%s\".\n", filename); 
  while (fgets (buffer, 256, fp) != NULL) {
  
    // A new material. Save last one to vector.
    if (startsWith (buffer, "newmtl ")) {
      if (mtl != NULL) materials->push_back (mtl);
      mtl = new Material ();
      sscanf(buffer, "newmtl %s", &(mtl->identifier));
    }

    // Read lighting/reflection values and texture file name.
    else if (startsWith (buffer, "Ka ")) sscanf(buffer, "Ka %f %f %f", 
      &mtl->ambientLight[0], &mtl->ambientLight[1], &mtl->ambientLight[2]);
    else if (startsWith (buffer, "Kd ")) sscanf(buffer, "Kd %f %f %f", 
      &mtl->diffuseLight[0], &mtl->diffuseLight[1], &mtl->diffuseLight[2]);
    else if (startsWith (buffer, "Ks ")) sscanf(buffer, "Ks %f %f %f", 
      &mtl->specularLight[0], &mtl->specularLight[1], &mtl->specularLight[2]);
    else if (startsWith (buffer, "map_Kd ")) sscanf(buffer, "map_Kd %s", 
      &mtl->textureName);
  }

  // Write last entry and close file stream.
  if (mtl != NULL) materials->push_back (mtl);
  fclose (fp);

  /*
  printf ("Materials read: %d\n", materials->size());
  for (int i = 0; i < materials->size(); i ++) {
    printf ("(%d)  Name: %s    \n"
            "     Text: %s     \n"
            "     Ka: %f %f %f \n"
            "     Kd: %f %f %f \n"
            "     Ks: %f %f %f \n", i,
            materials->at(i)->identifier,
            materials->at(i)->textureName,
            materials->at(i)->ambientLight[0], materials->at(i)->ambientLight[1], materials->at(i)->ambientLight[2], 
            materials->at(i)->diffuseLight[0], materials->at(i)->diffuseLight[1], materials->at(i)->diffuseLight[2],
            materials->at(i)->specularLight[0],materials->at(i)->specularLight[1],materials->at(i)->specularLight[2] 
    );   
  } */
}





/* Method to check the beginning of a string. [internal] */
int Converter::startsWith (const char* string, const char* prefix) {
  size_t lenpre = strlen (prefix), lenstr = strlen (string);
  return lenstr < lenpre ? 0 : strncmp (prefix, string, lenpre) == 0;
}