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
    
    // Read identifier and rendering status.
    fread (&_geosets[g].id,      sizeof (int),  1, fp);
    fread (&_geosets[g].enabled, sizeof (bool), 1, fp);

    // Read vertices.
    fread (&_geosets[g].nrV, sizeof (long), 1, fp);
    _geosets[g].vertices = (Vertex*) calloc (_geosets[g].nrV, sizeof (Vertex));
    for (i = 0; i < _geosets[g].nrV; i ++) {
      fread (&_geosets[g].vertices[i].x, sizeof (float), 1, fp);
      fread (&_geosets[g].vertices[i].y, sizeof (float), 1, fp);
      fread (&_geosets[g].vertices[i].z, sizeof (float), 1, fp);
    }

    // Read normal vectors.
    fread (&_geosets[g].nrN, sizeof (long), 1, fp);
    _geosets[g].normals = (Vertex*) calloc (_geosets[g].nrN, sizeof (Vertex));
    for (i = 0; i < _geosets[g].nrN; i ++) {
      fread (&_geosets[g].normals[i].x, sizeof (float), 1, fp);
      fread (&_geosets[g].normals[i].y, sizeof (float), 1, fp);
      fread (&_geosets[g].normals[i].z, sizeof (float), 1, fp);
    }

    // Read texture coordinates.
    fread (&_geosets[g].nrT, sizeof (long), 1, fp);
    _geosets[g].textures = (TexCoord*) calloc (_geosets[g].nrT, sizeof (TexCoord));
    for (i = 0; i < _geosets[g].nrT; i ++) {
      fread (&_geosets[g].textures[i].u, sizeof (float), 1, fp);
      fread (&_geosets[g].textures[i].v, sizeof (float), 1, fp);
    }

    // Read geometries.
    fread (&_geosets[g].nrG, sizeof (long), 1, fp);
    _geosets[g].geometries = (Geometry*) calloc (_geosets[g].nrG, sizeof (Geometry));
    for (i = 0; i < _geosets[g].nrG; i ++) {
      
      // If indices are symmetric, read one and apply to each. Otherwise read in sequential.
      fread (&_geosets[g].geometries[i].symIndices, sizeof (bool), 1, fp);
      for (j = 0; j < 3; j ++) {  
        fread (&_geosets[g].geometries[i].vIdx[j], sizeof (long), 1, fp); 
        if (_geosets[g].geometries[i].symIndices) {
          _geosets[g].geometries[i].nIdx[j] = _geosets[g].geometries[i].vIdx[j];
          _geosets[g].geometries[i].tIdx[j] = _geosets[g].geometries[i].vIdx[j];
        }
        else {
          fread (&_geosets[g].geometries[i].nIdx[j], sizeof (long), 1, fp); 
          fread (&_geosets[g].geometries[i].tIdx[j], sizeof (long), 1, fp); 
        }        
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
    fwrite (&_geosets[g].id,      sizeof (int),  1, fp);
    fwrite (&_geosets[g].enabled, sizeof (bool), 1, fp);

    // Write vertices.
    fwrite (&_geosets[g].nrV, sizeof(long), 1, fp);
    for (i = 0; i < _geosets[g].nrV; i ++) {
      fwrite (&_geosets[g].vertices[i].x, sizeof (float), 1, fp);
      fwrite (&_geosets[g].vertices[i].y, sizeof (float), 1, fp);
      fwrite (&_geosets[g].vertices[i].z, sizeof (float), 1, fp);
    }

    // Write normal vectors.
    fwrite (&_geosets[g].nrN, sizeof(long), 1, fp);
    for (i = 0; i < _geosets[g].nrN; i ++) {
      fwrite (&_geosets[g].normals[i].x, sizeof (float), 1, fp);
      fwrite (&_geosets[g].normals[i].y, sizeof (float), 1, fp);
      fwrite (&_geosets[g].normals[i].z, sizeof (float), 1, fp);
    }

    // Write texture coordinates.
    fwrite (&_geosets[g].nrT, sizeof (long), 1, fp);
    for (i = 0; i < _geosets[g].nrT; i ++) {
      fwrite (&_geosets[g].textures[i].u, sizeof (float), 1, fp);
      fwrite (&_geosets[g].textures[i].v, sizeof (float), 1, fp);
    }

    // Write geometries.
    fwrite (&_geosets[g].nrG, sizeof (long), 1, fp);
    for (i = 0; i < _geosets[g].nrG; i ++) {
      fwrite (&_geosets[g].geometries[i].symIndices, sizeof (bool), 1, fp);     
      for (j = 0; j < 3; j ++) {  
        fwrite (&_geosets[g].geometries[i].vIdx[j], sizeof (long), 1, fp); 
        if (!_geosets[g].geometries[i].symIndices) {
          fwrite (&_geosets[g].geometries[i].nIdx[j], sizeof (long), 1, fp); 
          fwrite (&_geosets[g].geometries[i].tIdx[j], sizeof (long), 1, fp); 
        }        
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
      fprintf (fp, " - Geoset [%02d] (%s)\n", _geosets[i].id, 
              (_geosets[i].enabled? "enabled" : "disabled"));
      
      fprintf (fp, "   - Vertices: %d\n", _geosets[i].nrV);
      for (int j = 0; j < _geosets[i].nrV; j ++) {
        fprintf (fp, "     [%02d] %8.4f, %8.4f, %8.4f\n", j, 
          _geosets[i].vertices[j].x, _geosets[i].vertices[j].y, _geosets[i].vertices[j].z);
      }

      fprintf (fp, "   - Normals: %d\n", _geosets[i].nrN);
      for (int j = 0; j < _geosets[i].nrN; j ++) {
        fprintf (fp, "     [%02d] %8.4f, %8.4f, %8.4f\n", j, 
          _geosets[i].normals[j].x, _geosets[i].normals[j].y, _geosets[i].normals[j].z);
      }

      fprintf (fp, "   - Textures: %d\n", _geosets[i].nrT);
      for (int j = 0; j < _geosets[i].nrT; j ++) {
        fprintf (fp, "     [%02d] %8.4f, %8.4f\n", j, 
          _geosets[i].textures[j].u, _geosets[i].textures[j].v);
      }

      fprintf (fp, "   - Geometries: %d\n", _geosets[i].nrG);
      for (int j = 0; j < _geosets[i].nrG; j ++) {
        fprintf (fp, "     [%02d]", j);
        for (int k = 0; k < 3; k ++) {
          fprintf (fp, "  %d/%d/%d",  _geosets[i].geometries[j].vIdx[k], 
                                      _geosets[i].geometries[j].nIdx[k], 
                                      _geosets[i].geometries[j].tIdx[k]);
        }       
        fprintf (fp, "\n");
      }
    }
  }

  else {
    printf ("Overview for model [%d]:\n", _id);
    for (int i = 0; i < _nrGeosets; i ++) {
      printf (" - Geoset [%02d] (%s)\n"
              "   - Vertices  : %d\n"
              "   - Normals   : %d\n"
              "   - Textures  : %d\n"
              "   - Geometries: %d\n",
              _geosets[i].id, (_geosets[i].enabled? "enabled" : "disabled"),
              _geosets[i].nrV, _geosets[i].nrN, _geosets[i].nrT, _geosets[i].nrG);
    }
  }
  printf ("\n");
}



/* Create a new geoset with the given content and add it to the model. */
void Model3D::CreateGeoset (int id, bool enabled, long nrV, long nrN, long nrT, long nrG,
                            Vertex* vert,  Vertex* norm, TexCoord* text, Geometry* geom) { 
  // Allocate memory.
  _geosets = (Geoset*) realloc (_geosets, (_nrGeosets + 1) * sizeof (Geoset));
  
  // Enter information in the newly appended geoset.
  _geosets[_nrGeosets].id = id;
  _geosets[_nrGeosets].enabled = enabled;
  _geosets[_nrGeosets].nrV = nrV;
  _geosets[_nrGeosets].nrN = nrN;
  _geosets[_nrGeosets].nrT = nrT;
  _geosets[_nrGeosets].nrG = nrG;
  _geosets[_nrGeosets].vertices = vert;
  _geosets[_nrGeosets].normals = norm;
  _geosets[_nrGeosets].textures = text;
  _geosets[_nrGeosets].geometries = geom;

  _nrGeosets ++;  // Increase counter.
}



/* Remove a geoset from the model. */
void Model3D::RemoveGeoset (int id) {
  for (int i = 0; i < _nrGeosets; i ++) {
    if (_geosets[i].id == id) {
      
      // Geoset found, now remove it!
      free (_geosets[i].vertices);
      free (_geosets[i].normals);
      free (_geosets[i].textures);
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
