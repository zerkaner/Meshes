#include "Camera.h"

/* Axis definition in OpenGL:
  x-Axis: (-) ← left  |  right → (+)
  y-Axis: (-) ↓ down  |     up ↑ (+)
  z-Axis: (-)  farer  |  nearer  (+)
  [right-handed coordinate system]
  // not to be confused with pixels on screen [(0,0) = (top,left)]
*/

Camera::Camera (SDL_Window* win) : Camera (win, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f) {}

Camera::Camera (SDL_Window* win, float x, float y, float z, float pitch, float yaw) {
  _window = win;
  SetPosition (x, y, z, pitch, yaw);
  UpdateCentre ();
}

Camera::~Camera () {}


void Camera::UpdateCentre () {
  SDL_GetWindowSize (_window, &_midX, &_midY);
  _midX /= 2;
  _midY /= 2;
}


/* Set the camera position. */
void Camera::SetPosition (float x, float y, float z, float pitch, float yaw) {
  _pos.x = x; 
  _pos.y = y;
  _pos.z = z;
  _pitch = pitch;
  _yaw = yaw;
 
  // Force update.
  glRotatef (_pitch, -1.0f,  0.0f, 0.0f);   // Rotate on x-Axis (set height).
  glRotatef (_yaw,    0.0f, -1.0f, 0.0f);   // Rotate on y-Axis (set spin).
  glTranslatef (-_pos.x, -_pos.y, -_pos.z); // Set the camera position.  
}



FLOAT3 UpdatePosition (FLOAT3 pos, float dist, float pitch, float yaw) {
  yaw += 180.0f; // Standard coordinate system -> OpenGL shift.
  pitch *= 0.0174532925f;
  yaw   *= 0.0174532925f;
  pos.x += dist * cos(pitch) * sin(yaw);
  pos.y += dist * sin(pitch);
  pos.z += dist * cos(pitch) * cos(yaw);
  return pos;
}



void Camera::Update () {

  if (!MouseIn) return;  // No focus, ignore mouse and keyboard.

  // Get mouse movement and calculate pitch and yaw. 
  int tmpX, tmpY;
  SDL_GetMouseState (&tmpX, &tmpY);       // Mouse coords: [0,0] = top,left.
  _pitch += MouseSpeed * (_midY - tmpY);  // mouse down:  -offset → reduce (add -).
  _yaw   += MouseSpeed * (_midX - tmpX);  // mouse right: -offset → reduce (add -).
  SDL_WarpMouseInWindow (NULL, _midX, _midY);
    
  // Check calculated values and adjust, if necessary.
  if (_pitch >  90.0f) _pitch =  90.0f;
  if (_pitch < -90.0f) _pitch = -90.0f;
  if (_yaw  < -180.0f) _yaw  += 360.0f;
  if (_yaw  >  180.0f) _yaw  -= 360.0f;
  
  // Calculate the movement speed and the travelled distance.
  float dist = 0.80f;
  //TODO Real stuff comes here ...
  
  // Read pressed keys for movement impulse.
  const Uint8* state = SDL_GetKeyboardState(NULL);
  if      (state[SDL_SCANCODE_W]) _pos = UpdatePosition (_pos,  dist, _pitch, _yaw);
  else if (state[SDL_SCANCODE_S]) _pos = UpdatePosition (_pos, -dist, _pitch, _yaw);
  else if (state[SDL_SCANCODE_A]) _pos = UpdatePosition (_pos,  dist,  0,  _yaw+90);
  else if (state[SDL_SCANCODE_D]) _pos = UpdatePosition (_pos,  dist,  0,  _yaw-90);

  // Update GL model with the calculated values. 
  glRotatef (_pitch, -1.0f,  0.0f, 0.0f);   // Rotate on x-Axis (set height).
  glRotatef (_yaw,    0.0f, -1.0f, 0.0f);   // Rotate on y-Axis (set spin).
  glTranslatef (-_pos.x, -_pos.y, -_pos.z); // Set the camera position.

  //printf ("U2: (%6.2f |%6.2f |%6.2f), Yaw:%4d, Pitch %d\n", _pos.x,_pos.y,_pos.z,(int)_yaw,(int)_pitch);
}
