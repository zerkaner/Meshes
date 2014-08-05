#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <Model3D.h>
#include "Definitions.h"
#include <Texture.h>


/* An OpenGL model specialization that implements various rendering methods. */
class ModelGL : public Model3D, public IObjectGL  {

  /* This structure aggregates geoset information for VBO rendering. */
  struct GeosetVBO {
    uint vVBO = 0, tVBO = 0, nVBO = 0;  //  0 = not assigned.
    uint textureID = -1;                // -1 = no texture.
    long length = 0;
  };

  /* An enumeration of available rendering modes. */
  enum RMODE {OFF, DIRECT, VBO, POINTS, MESH};


  private:

    Texture* _texture = NULL;   // Pointer to texture.  |TODO What about multiple textures?!
    GeosetVBO* _VBOs  = NULL;   // Vertex buffer object holder.  
    uint _renderMode  = DIRECT; // Rendering mode. Per default on "direct" (CPU) rendering.


    /* Build VBOs for all geosets. Uses the below function for each. */
    void buildVBOs ();

    /* Create a vertex buffer object (VBO) to store data in video memory. */
    uint createVBO (const void* data, int dataSize, uint target, uint usage);

    /* Rendering methods. These are called by Render() function, depending on mode. */
    void renderDirect (Geoset geoset);    
    void renderPoints (Geoset geoset);
    void renderMesh   (Geoset geoset);
    void renderVBO    (GeosetVBO geosetVBO);


  public:

    ModelGL (long id, Texture* texture) : Model3D(id) {
      _texture = texture;
    }

    /* Change the rendering mode. */
    void ChangeMode (int mode);
    
    /* Render this model. Depending on mode, different render functions are called. */
    void Render ();
};
