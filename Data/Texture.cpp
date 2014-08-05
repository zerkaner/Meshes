#include "Texture.h"


/* Function for examining the next "power-of-two". */
template <class T>
T nextPowerOfTwo (T value) {
  if ((value & (value - 1)) == 0) return value;
  value -= 1;
  for (size_t i = 1; i < sizeof(T)*8; i <<= 1) value = value|value>>i;
  return value + 1;
}



/* Slice a texture, return pointer on that slice. */
Texture* Texture::slice (int x, int y, int width, int height) {
  return new TextureSlice (this, x, y, width, height);
}



/* Create an empty texture. Loading must be performed by calling "initFromSurface(...)"! */
SimpleTexture::SimpleTexture (int width, int height) {
  _id = 0;
  _width = width;
  _height = height;
  if (SDL_GL_ExtensionSupported ("GL_ARB_texture_non_power_of_two")) {
    _stored_width  = _width;
    _stored_height = _height;
  } 
  else {  // Extend storage size, if it must be a power of two.
    _stored_width  = nextPowerOfTwo (_width);
    _stored_height = nextPowerOfTwo (_height);
  }
}



/* Delete the texture and free the memory. */
SimpleTexture::~SimpleTexture () {
  if (_id) glDeleteTextures (1, &_id);
}



/* Upload the SDL_Surface content to the GPU memory. */
bool SimpleTexture::initFromSurface (SDL_Surface* surface) {
  if (surface->w == _width && surface->h == _height) {
    glEnable (GL_TEXTURE_2D);
    glGenTextures (1, &_id);
    glBindTexture (GL_TEXTURE_2D, _id);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
  
    GLenum format;
    bool isLE = (surface->format->Rmask == 0x000000ff);
    switch (surface->format->BytesPerPixel) {     
      case 3 : format = isLE? GL_RGB  : GL_BGR;  break;
      case 4 : format = isLE? GL_RGBA : GL_BGRA; break;
      default: return false;
    }
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, _stored_width, _stored_height,
                  0, format, GL_UNSIGNED_BYTE, surface->pixels);
    return true;
  }
  return false;
}



/* Create a new texture slice. */
TextureSlice::TextureSlice (Texture* parent, int x, int y, int width, int height) {
  _parent = parent;
  _offset_x = x;
  _offset_y = y;
  _width = width;
  _height = height;
}



/* Create a new texture, given a SDL_Surface object (classless method). */
Texture* textureFromSurface (SDL_Surface* surface) {
  SimpleTexture* rv = new SimpleTexture (surface->w, surface->h);
  rv->initFromSurface (surface);
  return rv;
}
