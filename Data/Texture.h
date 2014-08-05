#pragma once

#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>    // Always put GLEW before normal OpenGL include!
#include <SDL_opengl.h>




/* Base class, defines common functions needed for a texture. */
class Texture {

  public:
    virtual ~Texture() {}
    virtual GLuint id()             const = 0;
    virtual int width()             const = 0;
    virtual int height()            const = 0;
    virtual int stored_width()      const = 0;
    virtual int stored_height()     const = 0;
    virtual int offset_x()          const = 0;
    virtual int offset_y()          const = 0;
    virtual const Texture *parent() const = 0;

    Texture* slice (int x, int y, int width, int height);
};


/* A basic texture class, extends the abstract one. */
class SimpleTexture : public Texture {
  
  private:
    GLuint _id;          // OpenGL ID of texture.
    int _width;          // Texture width.
    int _height;         // Texture height.
    int _stored_width;   // Actual width in memory.
    int _stored_height;  // Actual height in memory.
  
  public:
    SimpleTexture(int width, int height);
    ~SimpleTexture();
    bool initFromSurface(SDL_Surface *surface);

    // Getter functions.
    GLuint id()             const { return _id; }
    int width()             const { return _width; }
    int height()            const { return _height; }
    int stored_width()      const { return _stored_width; }
    int stored_height()     const { return _stored_height; }
    int offset_x()          const { return 0; }
    int offset_y()          const { return 0; }
    const Texture* parent() const { return 0; }   
};


/* Represents a slice of an texture. */
class TextureSlice : public Texture {
  
  private:
    Texture* _parent; // Parent (=major) texture.
    int _offset_x;    // X offset of this slice.
    int _offset_y;    // Y offset of slice.
    int _width;       // The width this slice has.
    int _height;      // The height.
  
  public:
    TextureSlice(Texture* parent, int x, int y, int width, int height);

    // Getter functions.
    GLuint id()             const { return _parent->id(); }
    int width()             const { return _width; }
    int height()            const { return _height; }
    int stored_width()      const { return _parent->stored_width(); }
    int stored_height()     const { return _parent->stored_height(); }
    int offset_x()          const { return _offset_x; }
    int offset_y()          const { return _offset_y; }
    const Texture* parent() const { return _parent; }
};


// Create a new texture from a SDL_Surface. 
Texture* textureFromSurface (SDL_Surface* surface);
