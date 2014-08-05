#pragma once

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned int   uint;

// Base types for vector and joint representation.
struct Vertex   { float x, y, z; };
struct TexCoord { float u, v;    };

// Fixed-size triangular geometry ("face").
struct Geometry {
  Vertex   vertices [3];
  Vertex   normals  [3];
  TexCoord texcoords[3];
};

// Partial structure. A model consists of one or more geosets.
struct Geoset {
  int  id;
  bool enabled;
  long nrGeometries;
  Geometry* geometries;
};


/*
05.08.2014  09:59            94.977 Bear.mod
02.08.2014  20:14            30.171 Cat.mod
02.08.2014  13:02           293.809 draken.mod
05.08.2014  15:30         3.604.505 hf.mod
02.08.2014  12:31           293.809 td.mod
               5 Datei(en),      4.317.271 Bytes
               2 Verzeichnis(se), 50.563.219.456 Bytes frei
*/
