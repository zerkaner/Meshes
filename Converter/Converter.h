#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <Datatypes.h>
#include <Model3D.h>


class Converter {

  private:
    enum FORMATS {MDX, MS3D, RAW, MDL, OBJ};
    struct LONG4 { long v = 0, n = 0, t = 0, g = 0; };
    struct WORD3 { WORD indices [3]; };
    int _nrFormats = 5;
    int _nrBinary  = 3;

    FILE* _fp = NULL;
    Model3D* _model = NULL;

    /* Parser functions for various formats. */
    void readOBJ  ();  //TODO
    void readMDL  ();  //TODO
    void readMDX  ();  //TODO
    void readMS3D ();  //TODO
    void readRAW  ();  // Texture format (heightmap).

    /* Method to check the beginning of a string. */
    int startsWith (const char* string, const char* prefix);


  public:
    Converter ();
    ~Converter ();

    /* Load file, redirect to format interpreter. */
    void LoadFile (const char* filename, const char* savename);
};
