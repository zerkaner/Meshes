#include "Loader.h"

using namespace std;


Loader::Loader () {}
Loader::~Loader () {}


/* Load a texture from a file into a SDL_Surface object. */
Texture* Loader::LoadTexture (const char* filename) { 
  FILE* fp = fopen (filename, "rb");
  if (fp != NULL) {
    int x, y, cmp;  // Stores x, y resolution and components.
    unsigned char* data = stbi_load_from_file (fp, &x, &y, &cmp, 0);
    fclose (fp);

    // Take care of the byteorder for RGB-masks.
    uint32_t rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif

    // Create the surface.
    SDL_Surface* sf;
    switch (cmp) {
      case 3 : sf = SDL_CreateRGBSurface (0, x, y, 24, rmask, gmask, bmask, 0);     break;
      case 4 : sf = SDL_CreateRGBSurface (0, x, y, 32, rmask, gmask, bmask, amask); break;
      default: stbi_image_free(data); return NULL;
    }
    // Copy pixels to surface object. 
    memcpy (sf->pixels, data, cmp * x * y);
    stbi_image_free (data);

    // Create an OpenGL texture from the SDL_Surface.
    Texture* texture = textureFromSurface (sf);
    //delete sf;
    return texture;
  }

  // Print error message.
  else {
    printf ("[ERROR]: Could not open file \"%s\"!\n", filename);
    return NULL;
  }
}



/* Method to check the beginning of a string. [internal] */
int startsWith (const char* string, const char* prefix) {
  size_t lenpre = strlen (prefix), lenstr = strlen (string);
  return lenstr < lenpre ? 0 : strncmp (prefix, string, lenpre) == 0;
}



/* Load OBJ material library. */
void loadMTL (const char* filename) {
  FILE* fp = fopen (filename, "r");
  if (fp != NULL) {
    /*
    float v1, v2, v3;  // Input variables.
    float lSpec [3];   // Light specifications.
    char texStr [256]; // Texture file names.
    char buffer [256]; // Buffer for reader.
    int ret;           // Sscanf return value.

    while (fgets (buffer, 256, fp) != NULL) {

      if ((ret = sscanf(buffer, "Ka %f %f %f", &lSpec[0],&lSpec[1],&lSpec[2])) != EOF && ret == 3) {
        printf ("Ka: %6.4f, %6.4f, %6.4f\n", lSpec[0], lSpec[1], lSpec[2]);
      }
      else if ((ret = sscanf(buffer, "Kd %f %f %f", &lSpec[0],&lSpec[1],&lSpec[2])) != EOF && ret == 3) {
        printf ("Kd: %6.4f, %6.4f, %6.4f\n", lSpec[0], lSpec[1], lSpec[2]);
      }
      else if ((ret = sscanf(buffer, "Ks %f %f %f", &lSpec[0],&lSpec[1],&lSpec[2])) != EOF && ret == 3) {
        printf ("Ks: %6.4f, %6.4f, %6.4f\n", lSpec[0], lSpec[1], lSpec[2]);
      }

      // Schreiben, wenn
      // - newmtl kommt
      // - Datei zu ende ist

      // Bessere Idee: Sammelstruktur.
      // Bei "new" erstellen, dann einlesen, bis neues neu oder ende!

      bool newInput;

/*
newmtl plane
Ka 0.117647 0.117647 0.117647
Kd 1 1 1
Ks 0.752941 0.752941 0.752941

illum 2
Ns 8

map_kA 
map_Kd Saab-35_Draken_P01.png
map_kS 
map_Ns

map_bump Saab-35_Draken_N.png
bump Saab-35_Draken_N.png
map_opacity 
map_d 
refl 



    }
*/
  
    fclose (fp);
  }
}


void Loader::loadMDX (const char* filename) {
  FILE* fp = fopen (filename, "rb");
  if (fp != NULL) {
    
    
    ModelChunk chunk;
    DWORD dbuf = 0x00;
    char buffer [260] = "";

    while (dbuf != 'MODL' && dbuf != 'LDOM') fread (&dbuf, sizeof (DWORD), 1, fp);   
    fread (&chunk.ChunkSize,         sizeof (DWORD),  1, fp);     
    fread ( chunk.Name,              sizeof (CHAR),  80, fp);
    fread ( chunk.AnimationFileName, sizeof (CHAR), 260, fp);
    fread (&chunk.BoundsRadius,      sizeof (FLOAT),  1, fp);
    fread ( chunk.MinimumExtent,     sizeof (FLOAT),  3, fp);
    fread ( chunk.MaximumExtent,     sizeof (FLOAT),  3, fp);
    fread (&chunk.BlendTime,         sizeof (DWORD),  1, fp);

    printf ("Chunk size: %d bytes. \n", chunk.ChunkSize);
    printf ("Model name: %s        \n", chunk.Name);
    printf ("Anim. file: %s        \n", chunk.AnimationFileName);
    printf ("Bounds rad: %f        \n", chunk.BoundsRadius);
    printf ("Min extent: %f, %f, %f\n", chunk.MinimumExtent[0], chunk.MinimumExtent[1], chunk.MinimumExtent[2]);
    printf ("Max extent: %f, %f, %f\n", chunk.MaximumExtent[0], chunk.MaximumExtent[1], chunk.MaximumExtent[2]);
    printf ("Blend time: %d      \n\n", chunk.BlendTime);
    

    SequenceChunk seqs;
    fread (&dbuf, sizeof (DWORD), 1, fp);  // Read position identifier 'SEQS'.
    fread (&seqs.ChunkSize, sizeof (DWORD), 1, fp);
    int iterations = seqs.ChunkSize / 132;
    seqs.Sequences = (Sequence*) malloc (sizeof (Sequence) * iterations); 

    printf ("Sequence chunk size: %d bytes.\n\n", seqs.ChunkSize);

    for (int i = 0; i < iterations; i ++) {
      fread ( seqs.Sequences[i].Name,          sizeof (CHAR), 80, fp);
      fread ( seqs.Sequences[i].Interval,      sizeof (DWORD), 2, fp);    
      fread (&seqs.Sequences[i].MoveSpeed,     sizeof (FLOAT), 1, fp);
      fread (&seqs.Sequences[i].Flags,         sizeof (DWORD), 1, fp);
      fread (&seqs.Sequences[i].Rarity,        sizeof (FLOAT), 1, fp);
      fread (&seqs.Sequences[i].SyncPoint,     sizeof (DWORD), 1, fp);
      fread (&seqs.Sequences[i].BoundsRadius,  sizeof (FLOAT), 1, fp);
      fread ( seqs.Sequences[i].MinimumExtent, sizeof (FLOAT), 3, fp);
      fread ( seqs.Sequences[i].MaximumExtent, sizeof (FLOAT), 3, fp);

      printf ("[%d] Anim ID: %s    \n", i, seqs.Sequences[i].Name);
      printf ("[%d] Inv S/E: %d, %d\n", i, seqs.Sequences[i].Interval[0], seqs.Sequences[i].Interval[1]);
      printf ("[%d] Movespd: %f    \n", i, seqs.Sequences[i].MoveSpeed);
      printf ("[%d] Flags  : %x    \n", i, seqs.Sequences[i].Flags);
      printf ("[%d] Rarity : %f    \n", i, seqs.Sequences[i].Rarity);
      printf ("[%d] Sync pt: %d    \n", i, seqs.Sequences[i].SyncPoint);
      printf ("[%d] Bds rad: %f    \n", i, seqs.Sequences[i].BoundsRadius);
      printf ("[%d] Min ext: %f, %f, %f  \n", i, seqs.Sequences[i].MinimumExtent[0], seqs.Sequences[i].MinimumExtent[1], seqs.Sequences[i].MinimumExtent[2]);
      printf ("[%d] Max ext: %f, %f, %f\n\n", i, seqs.Sequences[i].MaximumExtent[0], seqs.Sequences[i].MaximumExtent[1], seqs.Sequences[i].MaximumExtent[2]);
    }

    MaterialChunk mats = MaterialChunk ();
    Material* mptr = NULL;
    fread (&dbuf, sizeof (DWORD), 1, fp);  // Read material identifier 'MTLS'.
    fread (&mats.ChunkSize, sizeof (DWORD), 1, fp);

    printf ("Material chunk size: %d bytes.\n\n", mats.ChunkSize);
    
    for (int i = 0, remaining = mats.ChunkSize; remaining > 0; i ++) {    
      mats.Materials = (Material*) realloc (mats.Materials, sizeof (Material) * (i+1));
      mats.Materials[i] = Material ();
      fread (&mats.Materials[i].InclusiveSize, sizeof (DWORD), 1, fp);
      fread (&mats.Materials[i].PriorityPlane, sizeof (DWORD), 1, fp);
      fread (&mats.Materials[i].Flags,         sizeof (DWORD), 1, fp);
      remaining -= mats.Materials[i].InclusiveSize;

      printf ("[%d] Size    : %d \n", i, mats.Materials[i].InclusiveSize);
      printf ("[%d] Priority: %d \n", i, mats.Materials[i].PriorityPlane);
      printf ("[%d] Flags   : %d \n", i, mats.Materials[i].Flags);

      fread (&dbuf, sizeof(DWORD), 1, fp);
      
      if (dbuf == 'SYAL') {  // Load the Layer chunks [LAYS].
        mats.Materials[i].LayerChunk = (LayerChunk*) malloc (sizeof (LayerChunk));
        *mats.Materials[i].LayerChunk = LayerChunk ();
        fread (&(*mats.Materials[i].LayerChunk).NrOfLayers, sizeof (DWORD), 1, fp);
        int counter = (*mats.Materials[i].LayerChunk).NrOfLayers;
        (*mats.Materials[i].LayerChunk).Layers = (Layer*) calloc (counter, sizeof (Layer));
        for (int j = 0; j < counter; j ++) {
          (*mats.Materials[i].LayerChunk).Layers[j] = Layer ();
          fread (&(*mats.Materials[i].LayerChunk).Layers[j].InclusiveSize, sizeof (DWORD), 1, fp);
          fread (&(*mats.Materials[i].LayerChunk).Layers[j].FilterMode,    sizeof (DWORD), 1, fp);
          fread (&(*mats.Materials[i].LayerChunk).Layers[j].ShadingFlags,  sizeof (DWORD), 1, fp);
          fread (&(*mats.Materials[i].LayerChunk).Layers[j].TextureID,     sizeof (DWORD), 1, fp);
          fread (&(*mats.Materials[i].LayerChunk).Layers[j].TextureAnimID, sizeof (DWORD), 1, fp);
          fread (&(*mats.Materials[i].LayerChunk).Layers[j].CoordID,       sizeof (DWORD), 1, fp);
          fread (&(*mats.Materials[i].LayerChunk).Layers[j].Alpha,         sizeof (FLOAT), 1, fp);
          //TODO Read in KMTA and KMTF, if available.

          printf ("[%d] [%d] Size    : %d \n", i, j, (*mats.Materials[i].LayerChunk).Layers[j].InclusiveSize);
          printf ("[%d] [%d] Filter  : %d \n", i, j, (*mats.Materials[i].LayerChunk).Layers[j].FilterMode);
          printf ("[%d] [%d] Sh-Flags: %d \n", i, j, (*mats.Materials[i].LayerChunk).Layers[j].ShadingFlags);
          printf ("[%d] [%d] Text. ID: %d \n", i, j, (*mats.Materials[i].LayerChunk).Layers[j].TextureID);
          printf ("[%d] [%d] Anim. ID: %d \n", i, j, (*mats.Materials[i].LayerChunk).Layers[j].TextureAnimID);
          printf ("[%d] [%d] Coord ID: %d \n", i, j, (*mats.Materials[i].LayerChunk).Layers[j].CoordID);
          printf ("[%d] [%d] Alpha   : %f \n\n", i, j, (*mats.Materials[i].LayerChunk).Layers[j].Alpha);
        }
      }
    }


    // Reading textures ['TEXS'].
    fread (&dbuf, sizeof (DWORD), 1, fp);
    TextureChunk textures = TextureChunk ();
    fread (&textures.ChunkSize, sizeof (DWORD), 1, fp);
    printf ("Texture Chunk Size: %d\n\n", textures.ChunkSize);

    // Allocate cells for the textures and read them in.  
    int counter = textures.ChunkSize / 268;
    textures.Textures = (Texture_Str*) calloc (counter, sizeof (Texture_Str));
    for (int i = 0; i < counter; i ++) {
      textures.Textures[i] = Texture_Str ();
      fread (&textures.Textures[i].ReplaceableID, sizeof (DWORD),   1, fp);
      fread ( textures.Textures[i].FileName,      sizeof (CHAR),  260, fp);
      fread (&textures.Textures[i].Flags,         sizeof (DWORD),   1, fp);
      printf ("[%d] Texture ID: %d    \n", i, textures.Textures[i].ReplaceableID);
      printf ("[%d] File name : %s    \n", i, textures.Textures[i].FileName);
      printf ("[%d] Flags     : %x  \n\n", i, textures.Textures[i].Flags);
    }



    // Reading Geosets ['GEOS'].
    fread (&dbuf, sizeof (DWORD), 1, fp);
    GeosetChunk geosets = GeosetChunk ();
    fread (&geosets.ChunkSize, sizeof (DWORD), 1, fp);
    printf ("Geoset Chunk Size: %d\n\n", geosets.ChunkSize);

    // Main geoset loop over all entries in chunk.
    for (int i = 0, remaining = geosets.ChunkSize; remaining > 0; i ++) {
      geosets.Geosets = (Geoset2*) realloc (geosets.Geosets, sizeof (Geoset2) * (i+1));
      geosets.Geosets[i] = Geoset2 ();
      fread (&geosets.Geosets[i].InclusiveSize, sizeof (DWORD), 1, fp);
      remaining -= geosets.Geosets[i].InclusiveSize;

      // Read vertex positions.
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'VRTX'.
      fread (&geosets.Geosets[i].NrOfVertexPositions, sizeof (DWORD), 1, fp);
      int nrVP = geosets.Geosets[i].NrOfVertexPositions;
      geosets.Geosets[i].VertexPositions = (FLOAT3*) calloc (nrVP, sizeof (FLOAT3));
      for (int j = 0; j < nrVP; j ++) {
        fread (&geosets.Geosets[i].VertexPositions[j].x, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].VertexPositions[j].y, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].VertexPositions[j].z, sizeof (float), 1, fp);
      }

      // Read vertex normals.
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'NRMS'.
      fread (&geosets.Geosets[i].NrOfVertexNormals, sizeof (DWORD), 1, fp);
      int nrVN = geosets.Geosets[i].NrOfVertexNormals; 
      geosets.Geosets[i].VertexNormals = (FLOAT3*) calloc (nrVN, sizeof (FLOAT3));
      for (int j = 0; j < nrVN; j ++) {
        fread (&geosets.Geosets[i].VertexNormals[j].x, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].VertexNormals[j].y, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].VertexNormals[j].z, sizeof (float), 1, fp);
      }

      // Read face types.
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'PTYP'.
      fread (&geosets.Geosets[i].NrOfFaceTypeGroups, sizeof (DWORD), 1, fp);
      int nrFT = geosets.Geosets[i].NrOfFaceTypeGroups;
      geosets.Geosets[i].FaceTypes = (DWORD*) calloc (nrFT, sizeof (DWORD));
      for (int j = 0; j < nrFT; j ++) {
        fread (&geosets.Geosets[i].FaceTypes[j], sizeof (DWORD), 1, fp);
      }

      // Read face groups.
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'PCNT'.
      fread (&geosets.Geosets[i].NrOfFaceGroups, sizeof (DWORD), 1, fp);
      int nrFG = geosets.Geosets[i].NrOfFaceGroups;
      geosets.Geosets[i].FaceGroups = (DWORD*) calloc (nrFG, sizeof (DWORD));
      for (int j = 0; j < nrFG; j ++) {
        fread (&geosets.Geosets[i].FaceGroups[j], sizeof (DWORD), 1, fp);
      }

      // Read faces.
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'PVTX'.
      fread (&geosets.Geosets[i].TotalNrOfIndexes, sizeof (DWORD), 1, fp);
      int nrFA = geosets.Geosets[i].TotalNrOfIndexes / 3;
      geosets.Geosets[i].Faces = (Face*) calloc (nrFA, sizeof (Face));
      for (int j = 0; j < nrFA; j ++) {
        fread (&geosets.Geosets[i].Faces[j].Index1, sizeof (WORD), 1, fp);
        fread (&geosets.Geosets[i].Faces[j].Index2, sizeof (WORD), 1, fp);
        fread (&geosets.Geosets[i].Faces[j].Index3, sizeof (WORD), 1, fp);
      }

      // Read vertex groups.
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'GNDX'.
      fread (&geosets.Geosets[i].NrOfVertexGroups, sizeof (DWORD), 1, fp);
      int nrVG = geosets.Geosets[i].NrOfVertexGroups;
      geosets.Geosets[i].VertexGroups = (BYTE*) calloc (nrVG, sizeof (BYTE));
      for (int j = 0; j < nrVG; j ++) {
        fread (&geosets.Geosets[i].VertexGroups[j], sizeof (BYTE), 1, fp);
      }

      // Read matrix groups.
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'MTGC'.
      fread (&geosets.Geosets[i].NrOfMatrixGroups, sizeof (DWORD), 1, fp);
      int nrMG = geosets.Geosets[i].NrOfMatrixGroups;
      geosets.Geosets[i].MatrixGroups = (DWORD*) calloc (nrMG, sizeof (DWORD));
      for (int j = 0; j < nrMG; j ++) {
        fread (&geosets.Geosets[i].MatrixGroups[j], sizeof (DWORD), 1, fp);
      }

      // Read matrix indexes.
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'MATS'.
      fread (&geosets.Geosets[i].NrOfMatrixIndexes, sizeof (DWORD), 1, fp);
      int nrMI = geosets.Geosets[i].NrOfMatrixIndexes;
      geosets.Geosets[i].MatrixIndexes = (DWORD*) calloc (nrMI, sizeof (DWORD));
      for (int j = 0; j < nrMI; j ++) {
        fread (&geosets.Geosets[i].MatrixIndexes[j], sizeof (DWORD), 1, fp);
      }

      // Read diverse single values.
      fread (&geosets.Geosets[i].MaterialID,     sizeof (DWORD), 1, fp);
      fread (&geosets.Geosets[i].SelectionGroup, sizeof (DWORD), 1, fp);
      fread (&geosets.Geosets[i].SelectionFlags, sizeof (DWORD), 1, fp);
      fread (&geosets.Geosets[i].BoundsRadius,   sizeof (float), 1, fp);
      fread (&geosets.Geosets[i].MinimumExtent,  sizeof (float), 3, fp);
      fread (&geosets.Geosets[i].MaximumExtent,  sizeof (float), 3, fp);

      // Read the extents.
      fread (&geosets.Geosets[i].NrOfExtents, sizeof (DWORD), 1, fp);
      int nrEX = geosets.Geosets[i].NrOfExtents;
      geosets.Geosets[i].Extents = (Extent*) calloc (nrEX, sizeof (Extent));
      for (int j = 0; j < nrEX; j ++) {
        fread (&geosets.Geosets[i].Extents[j].BoundsRadius,    sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].Extents[j].MinimumExtent.x, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].Extents[j].MinimumExtent.y, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].Extents[j].MinimumExtent.z, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].Extents[j].MaximumExtent.x, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].Extents[j].MaximumExtent.y, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].Extents[j].MaximumExtent.z, sizeof (float), 1, fp);
      }

      // Read texture vertex groups and positions.
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'UVAS'.
      fread (&geosets.Geosets[i].NrOfTextureVertexGroups, sizeof (DWORD), 1, fp);
      fread (&dbuf, sizeof (DWORD), 1, fp); // Skip 'UVBS'.
      fread (&geosets.Geosets[i].NrOfTextureVertexPositions, sizeof (DWORD), 1, fp);
      int nrTP = geosets.Geosets[i].NrOfTextureVertexPositions;
      geosets.Geosets[i].TexturePositions = (FLOAT2*) calloc (nrTP, sizeof (FLOAT2));
      for (int j = 0; j < nrTP; j ++) {
        fread (&geosets.Geosets[i].TexturePositions[j].x, sizeof (float), 1, fp);
        fread (&geosets.Geosets[i].TexturePositions[j].y, sizeof (float), 1, fp);
      }

      printf ("Geoset [%03d]:\n", i);
      printf ("- Inclusive Size: %d  (remaining: %d)\n", geosets.Geosets[i].InclusiveSize, remaining);
      printf ("- Vertexes      : %d\n", nrVP);
      printf ("- Normal Vectors: %d\n", nrVN);
      printf ("- F. Type Groups: %d\n", nrFT);
      printf ("- Face Groups   : %d\n", nrFG);
      printf ("- Faces total   : %d\n", nrFA);
      printf ("- Vertex Groups : %d\n", nrVG);
      printf ("- Matrix Groups : %d\n", nrMG);
      printf ("- Matrix Indexes: %d\n", nrMI);
      printf ("- Material ID   : %d\n", geosets.Geosets[i].MaterialID);
      printf ("- Select. Group : %d\n", geosets.Geosets[i].SelectionGroup);
      printf ("- Select. Flags : %d\n", geosets.Geosets[i].SelectionFlags);
      printf ("- Bounds Radius : %f\n", geosets.Geosets[i].BoundsRadius);
      printf ("- Extents       : %d\n", nrEX);
      printf ("- Minimum Extent:%10.6f %10.6f %10.6f\n", geosets.Geosets[i].MinimumExtent[0], geosets.Geosets[i].MinimumExtent[1], geosets.Geosets[i].MinimumExtent[2]);
      printf ("- Maximum Extent:%10.6f %10.6f %10.6f\n", geosets.Geosets[i].MaximumExtent[0], geosets.Geosets[i].MaximumExtent[1], geosets.Geosets[i].MaximumExtent[2]);
      printf ("- Texture Pos.  : %d\n\n", nrTP);
    }


    printf ("Geosets finished, file pointer now at address %x.\n", ftell (fp));

    fclose (fp);
  }
  else printf ("File not found or access denied!\n");
}
