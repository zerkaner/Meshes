#pragma once
#include <stdio.h>
#include <math.h>
#include <SDL.h>       
#include <SDL_opengl.h> 
#include <MDX.h>

class Camera {

  private:
    SDL_Window* _window;   // Pointer to the window.
    FLOAT3 _pos;           // Coordinates of the camera position.
    float _pitch, _yaw;    // Pitch (up/down) and yaw (turn around).
    int _midX, _midY;      // Screen centre coordinates.

    const float MouseSpeed = 0.15f;  // Factor for mouse acceleration.
    const float MoveSpeed  = 0.15f;  // Factor for movement speed.
    
    void MoveHorizontal (float dir);
    void MoveVertical (float dir);


  public:
    bool MouseIn = false;

    Camera (SDL_Window* win);
    Camera (SDL_Window* win, float x, float y, float z, float pitch, float yaw);
    ~Camera ();

    void SetPosition (float x, float y, float z, float pitch, float yaw);

    void UpdateCentre();   // Set the centre coordinates (on init and/or change).
    void Update();
};

