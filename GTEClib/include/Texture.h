#pragma once

#include "GTEC.h"
#include "Color.h"

using namespace std;



class CTexture
{
public:
	
	// ----- Constructors & Destructor -----
	
	CTexture() { Width=Height=Xpos=Ypos=0; TextureID=0; Frames=frame=1; iscloned=false;}
	
	~CTexture() { if (!iscloned) glDeleteTextures( 1, &TextureID); }
	
	static bool smooth;

	// ----- Attributes ------

private:
	
	// Position & Size
	int Xpos, Ypos;			// position of the texture
	int Width, Height;		// width and height of texture in pixels
    GLuint TextureID;		// OpenGL texture ID
	int frame, Frames;		// current frame, total number of frames
	CColor TranspColor;		// transparent colour, only used when creating texture
	
	bool iscloned;

	
public:
	

	// Size of the Texture
	int GetWidth()			    	    { return Width; }
	int GetHeight()			    		{ return Height; }
	void SetSize(int w, int h)         { Width=w; Height=h; }
	void SetPosition(int x, int y)		{ Xpos=x; Ypos=y; }

		
	int GetFrames()						{ return Frames; }
	void SetFrame (int f) 				{ frame=f; }
	void NextFrame()					{ frame++; if (frame > Frames) frame=1; }

	GLuint GetTextureID()				{ return TextureID; }
	void SetTextureID( GLuint tid)		{ TextureID=tid; iscloned=true;} 
	
    bool LoadTexture( string filename, const CColor& color, int frames=1);
    bool LoadTexture( string filename, int frames=1) { return LoadTexture( filename, CColor::None(), frames); } 
    
	void Draw(int x, int y);
	void Draw() { Draw( Xpos, Ypos); }
	
	bool HitTest(int x, int y);

	

private:

	unsigned char* LoadFromBMP(const char* filename);
	unsigned char* LoadFromTGA(const char* filename);
	
    int readWord(FILE* in, int len);

        

	
};



