#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Datatypes.h"

/* Data plane of a 3D model. Contains also loading and save functions. */
class Model3D {

  protected:
    int   _id;               // Model ID, maybe useful later.
    int   _nrGeosets = 0;    // Number of geosets. 
    Geoset* _geosets = NULL; // Pointer to the geosets.


  public:

    /* Empty constructor. */
    Model3D (long id) : _id(id) {}

    /* Destructor, frees all allocated memory. */
    ~Model3D () {
      for (int i = 0; i < _nrGeosets; i ++) free (_geosets[i].geometries);
      free (_geosets);
    }

    /* Create a new geoset with the given content and add it to the model. */
    void CreateGeoset (int id, bool enabled, long nrGeometries, Geometry* geom);
    
    /* Remove a geoset from the model. */
    void RemoveGeoset (int id);

    /* Set a geoset's rendering flag to enabled or disabled. */
    void EnableGeoset (int id, bool enabled);



    /* Reads a model from a given file. */
    void LoadFile (const char* filename);

    /* Writes the model data to a .mod file. */
    void WriteFile (const char* filename);

    /* Outputs the model properties to a console format. */
    void Echo (bool details);
};
