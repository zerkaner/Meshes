#ifndef LOADER_H
#define LOADER_H

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <GL\glew.h>
#include <SDL.h>
#include <Texture.h>
#include <SDL_opengl.h>
#include <vector>
#include "stb_image.h"

//#include <MDX.h>


/* Utility for geometry and texture loading. */
class Loader {
  public:
    Loader ();
    ~Loader ();

    
    void loadMDX (const char* filename);

    Texture* LoadTexture (const char* filename);
};

#endif


