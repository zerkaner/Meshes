#include "Model3D.h"


/* Reads a model from a given file. */
void Model3D::LoadFile (const char* filename) {
      
  // Try to open the file.
  printf ("Loading file \"%s\" ", filename);
  FILE* fp = fopen (filename, "rb");
  int g, i, j;
  if (fp == NULL) {
    printf ("[ERROR]\n");
    return;
  }
      
  // Echo file size.
  fseek (fp, 0L, SEEK_END);
  long bytes = ftell (fp);
  fseek (fp, 0L, SEEK_SET);
  printf ("[%d bytes] ", bytes);
     
  // Read number of geosets, reserve memory and loop over them.
  fread (&_nrGeosets, sizeof (int), 1, fp);
  _geosets = (Geoset*) calloc (_nrGeosets, sizeof (Geoset));
  for (g = 0; g < _nrGeosets; g ++) {
  
    // Read identifier, rendering status and geometry count.
    fread (&_geosets[g].id,           sizeof (int),  1, fp);
    fread (&_geosets[g].enabled,      sizeof (bool), 1, fp);
    fread (&_geosets[g].nrGeometries, sizeof (long), 1, fp);

    // Read triangular geometries.   
    _geosets[g].geometries = (Geometry*) calloc (_geosets[g].nrGeometries, sizeof (Geometry));
    for (i = 0; i < _geosets[g].nrGeometries; i ++) {
      for (j = 0; j < 3; j ++) {   // Vertices.
        fread (&_geosets[g].geometries[i].vertices[j].x, sizeof (float), 1, fp);
        fread (&_geosets[g].geometries[i].vertices[j].y, sizeof (float), 1, fp);
        fread (&_geosets[g].geometries[i].vertices[j].z, sizeof (float), 1, fp);
      }
      for (j = 0; j < 3; j ++) {   // Normal vectors.
        fread (&_geosets[g].geometries[i].normals[j].x, sizeof (float), 1, fp);
        fread (&_geosets[g].geometries[i].normals[j].y, sizeof (float), 1, fp);
        fread (&_geosets[g].geometries[i].normals[j].z, sizeof (float), 1, fp);
      }
      for (j = 0; j < 3; j ++) {   // Texture coordinates.
        fread (&_geosets[g].geometries[i].texcoords[j].u, sizeof (float), 1, fp);
        fread (&_geosets[g].geometries[i].texcoords[j].v, sizeof (float), 1, fp);
      }
    }
  }
  // Close file stream and quit.
  fclose (fp);
  printf ("[OK]\n");
}



/* Writes the model data to a .mod file. */
void Model3D::WriteFile (const char* filename) {
  FILE* fp = fopen (filename, "wb");
  int g, i, j;

  // Write number of geosets and loop over them.
  fwrite (&_nrGeosets, sizeof (int), 1, fp);
  for (g = 0; g < _nrGeosets; g ++) {
  
    // Write identifier and rendering status.
    fwrite (&_geosets[g].id,           sizeof (int),  1, fp);
    fwrite (&_geosets[g].enabled,      sizeof (bool), 1, fp);
    fwrite (&_geosets[g].nrGeometries, sizeof (long), 1, fp);


    // Write triangular geometries.
    for (i = 0; i < _geosets[g].nrGeometries; i ++) {
      for (j = 0; j < 3; j ++) {   // Vertices.
        fwrite (&_geosets[g].geometries[i].vertices[j].x, sizeof (float), 1, fp);
        fwrite (&_geosets[g].geometries[i].vertices[j].y, sizeof (float), 1, fp);
        fwrite (&_geosets[g].geometries[i].vertices[j].z, sizeof (float), 1, fp);
      }
      for (j = 0; j < 3; j ++) {   // Normal vectors.
        fwrite (&_geosets[g].geometries[i].normals[j].x, sizeof (float), 1, fp);
        fwrite (&_geosets[g].geometries[i].normals[j].y, sizeof (float), 1, fp);
        fwrite (&_geosets[g].geometries[i].normals[j].z, sizeof (float), 1, fp);
      }
      for (j = 0; j < 3; j ++) {   // Texture coordinates.
        fwrite (&_geosets[g].geometries[i].texcoords[j].u, sizeof (float), 1, fp);
        fwrite (&_geosets[g].geometries[i].texcoords[j].v, sizeof (float), 1, fp);
      }
    }
  }

  // Close file stream and quit.
  printf ("%d bytes out.\n", ftell (fp));
  fclose (fp);
}



/* Outputs the model properties to a console format. */
void Model3D::Echo (bool details) {
  if (details) {
    FILE* fp = fopen ("output.txt", "w");
    fprintf (fp, "Detailed output for Model3D [%d]:\n", _id);
    for (int i = 0; i < _nrGeosets; i ++) {
      fprintf (fp, " Geoset [%02d] (%s): %d geometries.\n", _geosets[i].id, 
              (_geosets[i].enabled? "enabled" : "disabled"), _geosets[i].nrGeometries);

      for (int j = 0; j < _geosets[i].nrGeometries; j ++) {
        fprintf (fp,   " - [%04d] Vrt: (%8.4f, %8.4f, %8.4f) (%8.4f, %8.4f, %8.4f) (%8.4f, %8.4f, %8.4f)\n"
                       "          Nml: (%8.4f, %8.4f, %8.4f) (%8.4f, %8.4f, %8.4f) (%8.4f, %8.4f, %8.4f)\n"
                       "          Tex: (%8.4f, %8.4f, %8.4f) (%8.4f, %8.4f, %8.4f)\n", j,
                     _geosets[i].geometries[j].vertices[0].x, 
                     _geosets[i].geometries[j].vertices[0].y,
                     _geosets[i].geometries[j].vertices[0].z, 
                     _geosets[i].geometries[j].vertices[1].x,
                     _geosets[i].geometries[j].vertices[1].y,
                     _geosets[i].geometries[j].vertices[1].z,
                     _geosets[i].geometries[j].vertices[2].x,
                     _geosets[i].geometries[j].vertices[2].y,
                     _geosets[i].geometries[j].vertices[2].z,
                     _geosets[i].geometries[j].normals[0].x,
                     _geosets[i].geometries[j].normals[0].y,
                     _geosets[i].geometries[j].normals[0].z,
                     _geosets[i].geometries[j].normals[1].x,
                     _geosets[i].geometries[j].normals[1].y,
                     _geosets[i].geometries[j].normals[1].z,
                     _geosets[i].geometries[j].normals[2].x,
                     _geosets[i].geometries[j].normals[2].y,
                     _geosets[i].geometries[j].normals[2].z,
                     _geosets[i].geometries[j].texcoords[0].u,
                     _geosets[i].geometries[j].texcoords[0].v,
                     _geosets[i].geometries[j].texcoords[1].u,
                     _geosets[i].geometries[j].texcoords[1].v);
      }
    }
    fclose (fp);
  }

  else {
    printf ("Overview for model [%d]:\n", _id);
    for (int i = 0; i < _nrGeosets; i ++) {
      printf (" - Geoset [%02d]\n"
              "   - State     : %s\n"
              "   - Geometries: %d\n"
              "   - Vtx/Nm/Tex: %d\n",
              _geosets[i].id, (_geosets[i].enabled? "enabled" : "disabled"),
              _geosets[i].nrGeometries, _geosets[i].nrGeometries*3);
    }
  }
  printf ("\n");
}



/* Create a new geoset with the given content and add it to the model. */
void Model3D::CreateGeoset (int id, bool enabled, long nrGeometries, Geometry* geom) {

  // Allocate memory.
  _geosets = (Geoset*) realloc (_geosets, (_nrGeosets + 1) * sizeof (Geoset));
  
  // Enter information in the newly appended geoset.
  _geosets[_nrGeosets].id = id;
  _geosets[_nrGeosets].enabled = enabled;
  _geosets[_nrGeosets].nrGeometries = nrGeometries;
  _geosets[_nrGeosets].geometries = geom;

  _nrGeosets ++;  // Increase counter.
}



/* Remove a geoset from the model. */
void Model3D::RemoveGeoset (int id) {
  for (int i = 0; i < _nrGeosets; i ++) {
    if (_geosets[i].id == id) {
      
      // Geoset found, now remove it!
      free (_geosets[i].geometries);

      // If there is a gap, left-shift the remaining elements.
      int elementsToCopy = _nrGeosets - (i+1);
      if (elementsToCopy > 0) {
        memcpy (&_geosets[i], &_geosets[i+1], sizeof(Geoset) * elementsToCopy); 
      }
     
      _nrGeosets --;  // Decrease counter.
      break;
    }
  }
}



/* Set a geoset's rendering flag to enabled or disabled. */
void Model3D::EnableGeoset (int id, bool enabled) {
  for (int i = 0; i < _nrGeosets; i ++) {
    if (_geosets[i].id == id) {
      _geosets[i].enabled = enabled;
      break;
    }
  }
}
