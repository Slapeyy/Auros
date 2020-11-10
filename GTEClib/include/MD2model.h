#ifndef __MD2_H
#define __MD2_H

/*
	MD2.H

	The CMD2Model class

	Author: Kevin Hawkins
	Date: 3/29/2001
	Description: The CMD2Model class loads, animates, and displays
			   Quake 2 (TM id Software) .MD2 model files. 

    Modifications and additions made.

*/

#include "GTEC.h"
#include "Color.h"
#include "Texture.h"
#include "Model.h"

#define MAX_FRAMES 512


/*
	Vector Functionality
*/


// a single vertex
typedef struct
{
   float point[3];
} vector_t;


/* 
	MD2 Model Functionality
*/

// texture coordinate
typedef struct
{
   float s;
   float t;
} texCoord_t;

// texture coordinate index
typedef struct
{
   short s;
   short t;
} stIndex_t;

// info for a single frame point
typedef struct
{
   unsigned char v[3];
   unsigned char normalIndex;	// not used
} framePoint_t;

// information for a single frame
typedef struct
{
   float scale[3];
   float translate[3];
   char name[16];
   framePoint_t fp[1];
} frame_t;

// data for a single triangle
typedef struct
{
   unsigned short meshIndex[3];	// vertex indices
   unsigned short stIndex[3];		// texture coordinate indices
} mesh_t;

typedef struct
{
   int ident;		 // identifies as MD2 file "IDP2"
   int version;	     // mine is 8
   int skinwidth;    // width of texture
   int skinheight;   // height of texture
   int framesize;    // number of bytes per frame
   int numSkins;     // number of textures
   int numXYZ;       // number of points
   int numST;        // number of texture
   int numTris;      // number of triangles
   int numGLcmds;
   int numFrames;    // total number of frames
   int offsetSkins;  // offset to skin names (64 bytes each)
   int offsetST;     // offset of texture s-t values
   int offsetTris;   // offset of triangle mesh
   int offsetFrames; // offset of frame data (points)
   int offsetGLcmds; // type of OpenGL commands to use
   int offsetEnd;    // end of file
} modelHeader_t;


class CModelMd2 : public CModel
{
	
private:


     int numFrames;			   // number of model frames
     int numVertices;         // number of vertices
     int numTriangles;        // number of triangles
     int numST;               // number of skins
     int frameSize;           // size of each frame in bytes
     mesh_t *triIndex;        // triangle list
     texCoord_t *st;          // texture coordinate list
     vector_t *vertexList;    // vertex list
    

public:

	float bbsize;

     CModelMd2();        // constructor
     ~CModelMd2();       // destructor

     // load model only
     virtual bool LoadModel(string md2_filename);

     virtual void LoadTexture(string bmp_filename);
     
     virtual bool LoadModel(string md2_filename, string bmp_filename)
     {
		if (LoadModel(md2_filename)) { LoadTexture( bmp_filename); return true; }
		return false;
	 }

     // render model with interpolation to get animation
     virtual int AnimateModel(CGraphics* g);

     // render a single frame
     virtual void Draw(CGraphics* g);
     
   	  // free memory of model
     void Clear();
     
     virtual CModel* Clone();
     
	 private:
	 
	 void CalculateNormal( float *p1, float *p2, float *p3 );
	 
	
	 
	 
};

#endif
