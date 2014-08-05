Meshes
======

An OpenGL engine project.

This project serves no special purpose, it is just for learning OpenGL, 3D modeling and C++ language. It comes along with an own 3D model format (.mod) that may be rendered on CPU and/or GPU. 

A seperate converter is included to convert .mdx, .obj and .raw (heightmap) files to the .mod format. This utility is found in Debug\ directory, it is run on command line with "`conv.exe <inputfile> <outputfile>`". 

Though basic movement and input options are already implemented, there is still no user interface available. For now, all loading and configuration take place in Core\Program.cpp.
