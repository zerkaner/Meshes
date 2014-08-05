#pragma once

class GL_Material {

  public:
    GL_Material ();
    ~GL_Material ();

    enum PTR {AMB, DIFF, SPEC, HIGHL, BUMP, OPAC, ALPHA};

    char* Name;          // Texture description.
    float LightDiff [3]; // Diffuse light.
    float LightAmbi [3]; // Ambient (environmental) light.
    float LightSpec [4]; // Specular (reflected) light.  ([3] Ns = weight)
    int Illum;           // Illumination model (enumeration code).

    char* TexPath [6];

};

