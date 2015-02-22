#pragma once

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned int   uint;

// Base types for vector and joint representation.
struct Vertex   { float x, y, z; };
struct TexCoord { float u, v;    };

// Triangular size geometry ("face").
struct Geometry {
  bool symIndices;
  long vIdx [3];
  long nIdx [3];
  long tIdx [3];
};

// Partial structure. A model consists of one or more geosets.
struct Geoset {
  int  id;
  bool enabled;
  long nrV, nrN, nrT, nrG;
  Vertex*   vertices;
  Vertex*   normals;
  TexCoord* textures;
  Geometry* geometries;
  int materialID;
};

// Material structure, containing a texture and various reflection settings.
struct Material {
  char  identifier  [20];
  float ambientLight [3];
  float diffuseLight [3];
  float specularLight[3];
  long  textureSize;
  char* textureData;
  char  textureName [80];  // Later direct storage.
};



/*
05.08.2014  09:59            94.977 Bear.mod
02.08.2014  20:14            30.171 Cat.mod
02.08.2014  13:02           293.809 draken.mod
05.08.2014  15:30         3.604.505 hf.mod

05.08.2014  20:51           238.165 Bear.mod
05.08.2014  21:31            69.535 Cat.mod
05.08.2014  21:29           454.003 draken.mod
05.08.2014  21:39         3.145.741 hf.mod
*/