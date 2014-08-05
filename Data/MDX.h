/*  This file contains the structures for representation of Blizzard's MDL/MDX format.  */

#ifndef MDX_H
#define MDX_H

//TODO This basic stuff shall be moved elsewhere!
struct FLOAT2 {
  float x;
  float y;
};

struct FLOAT3 {
  float x;
  float y;
  float z;
};



// Chunk information.
typedef struct {
  DWORD ChunkSize;
  CHAR  Name [80];
  CHAR  AnimationFileName [260];
  FLOAT BoundsRadius; 
  FLOAT MinimumExtent [3];
  FLOAT MaximumExtent [3];
  DWORD BlendTime;
} ModelChunk;


// Animation sequence. 
typedef struct {  
  CHAR  Name [80];
  DWORD Interval [2];
  FLOAT MoveSpeed;
  DWORD Flags;   //0 - Looping, 1 - NonLooping
  FLOAT Rarity;
  DWORD SyncPoint; 
  FLOAT MinimumExtent [3];
  FLOAT MaximumExtent [3];
  FLOAT BoundsRadius;
} Sequence;


// Sequence chunk.
typedef struct {
  DWORD ChunkSize;
  Sequence* Sequences;  //NrOfSequences = ChunkSize / 132 
} SequenceChunk;


// Scaling tracks for material texture id.
typedef struct {
  DWORD Time;
  DWORD TextureId;
  DWORD InTan;   // Used, if InterpolationType > 1
  DWORD OutTan;  // -same- 
} ScalingTrack_TF;


// Scaling tracks for transparency.
typedef struct {
  DWORD Time;
  FLOAT Alpha;
  FLOAT InTan;   // Used, if InterpolationType > 1
  FLOAT OutTan;  // -same-
} ScalingTrack_TA;


// Animated material texture ID.  [KMTF]
typedef struct {
  DWORD NrOfTracks;
  DWORD InterpolationType; // 0 - None, 1 - Linear, 2 - Hermite, 3 - Bezier
  DWORD GlobalSequenceId;
  ScalingTrack_TF* ScalingTracks;  // [NrOfTracks]
} MaterialTextureID;


// Animated material transparency.  [KMTA]
typedef struct {
  DWORD NrOfTracks;
  DWORD InterpolationType; // 0 - None, 1 - Linear, 2 - Hermite, 3 - Bezier
  DWORD GlobalSequenceID;
  ScalingTrack_TA* ScalingTracks;  // [NrOfTracks]
} MaterialAlpha;


// Layer structure.
typedef struct {
  DWORD InclusiveSize;
  DWORD FilterMode;   // 0 - None, 1 - Transparent, 2 - Blend, 3 - Additive, 4 - AddAlpha, 5 - Modulate, 6 - Modulate2x
  DWORD ShadingFlags; // 1 - Unshaded, 2 - SphereEnvironmentMap, 16 - TwoSided, 32 - Unfogged, 64 - NoDepthTest, 128 - NoDepthSet
  DWORD TextureID;
  DWORD TextureAnimID;
  DWORD CoordID;
  FLOAT Alpha;
  MaterialAlpha* MaterialAlpha;         // Optional structure.
  MaterialTextureID* MaterialTextureID; // -same-
} Layer;


// A layer chunk.
typedef struct {
  DWORD NrOfLayers;
  Layer* Layers;
} LayerChunk;


// Single material definition.
typedef struct {
  DWORD InclusiveSize;
  DWORD PriorityPlane;
  DWORD Flags;  // Flags: 1 - ConstantColor, 8 - SortPrimNearZ, 16 - SortPrimFarZ, 32 - FullResolution
  LayerChunk* LayerChunk;
} Material;


// Material chunk.
typedef struct {
  DWORD ChunkSize;
  Material* Materials;
} MaterialChunk;


// A texture.
typedef struct {
  DWORD ReplaceableID;
  CHAR FileName [260];
  DWORD Flags;   //1 - WrapWidth, 2 - WrapHeight
} Texture_Str;


// Texture chunk.
typedef struct {
  DWORD ChunkSize;
  Texture_Str* Textures;  //NrOfTextures = ChunkSize / 268
} TextureChunk;







struct Face {
  WORD Index1;
  WORD Index2;
  WORD Index3;
};


struct Extent {
  FLOAT BoundsRadius;
  FLOAT3 MinimumExtent;
  FLOAT3 MaximumExtent;
};


struct Geoset2 {

  DWORD InclusiveSize;
  DWORD NrOfVertexPositions;        // 'VRTX' before.
  DWORD NrOfVertexNormals;          // 'NRMS' before.
  DWORD NrOfFaceTypeGroups;         // 'PTYP' before.
  DWORD NrOfFaceGroups;             // 'PCNT' before.
  DWORD TotalNrOfIndexes;           // 'PVTX' before.
  DWORD NrOfVertexGroups;           // 'GNDX' before.
  DWORD NrOfMatrixGroups;           // 'MTGC' before.
  DWORD NrOfMatrixIndexes;          // 'MATS' before.
  DWORD NrOfExtents;                // no prefix?
  DWORD NrOfTextureVertexGroups;    // 'UVAS' before.
  DWORD NrOfTextureVertexPositions; // 'UVBS' before.

  // Substructures and array pointers.
  FLOAT3* VertexPositions;
  FLOAT3* VertexNormals;
  DWORD*  FaceTypes;         // 4 - Triangles
  DWORD*  FaceGroups;
  Face*   Faces;             // [TotalNrOfIndexes / 3]
  BYTE*   VertexGroups;
  DWORD*  MatrixGroups;
  DWORD*  MatrixIndexes;
  Extent* Extents;
  FLOAT2* TexturePositions;

  // Single values.
  DWORD MaterialID;
  DWORD SelectionGroup;
  DWORD SelectionFlags;  // 0 - None, 4 - Unselectable
  FLOAT BoundsRadius;
  FLOAT MinimumExtent [3];
  FLOAT MaximumExtent [3];
};


struct GeosetChunk {
  DWORD ChunkSize;   // 'GEOS' before.
  Geoset2* Geosets;   // Unknown, ChunkSize minus the InclusiveSizes till zero.
};

#endif