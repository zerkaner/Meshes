#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <vector>
#include "Camera.h"
#include "Definitions.h"
using namespace std;


class Engine {

  private:
    SDL_Window* _window;
    SDL_GLContext _glcontext;    
    vector <IObjectGL*> _objects;

    bool _running = false;
    void _initExtensions ();


  public:
    Camera* camera;

    Engine (char* winname, int width, int height, bool fullscreen);
    ~Engine ();

    void SetResolution (int width, int height, bool fullscreen);
    void StartThread ();
    void Render ();

    /* Primitive functions to add terrain and models or to stop execution. */
    void AddModel   (IObjectGL* model) { _objects.push_back (model); }
    void StopThread ()                 { _running = false;           }
};
