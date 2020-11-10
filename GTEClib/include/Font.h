#pragma once

#include "GTEC.h"
#include "Color.h"

using namespace std;

class CFont
{
public:
	
	// ----- Constructors & Destructor -----
	
	CFont() { Width=Height=0; TextureID=0; Size=32; Color.Set( 1.0, 0.0, 0.0, 1.0);}
	
	~CFont() { glDeleteTextures( 1, &TextureID); }

	// ----- Attributes ------

private:
	
	// Position & Size
	int Width, Height;		// width and height of texture in pixels
    GLuint TextureID;
    int Size;
    
    CColor Color;			// font colour
	
	
    bool LoadFontFromBitmapFile( string filename);
	unsigned char* LoadFromBMP(const char* filename);
    int readWord(FILE* in, int len);
    
    

public:
    bool LoadDefault() { return LoadFontFromBitmapFile("font.bmp");}
    
    bool Load( string filename) { return LoadFontFromBitmapFile( filename.c_str());}
    
    void SetColor( Uint8 r, Uint8 g, Uint8 b, Uint8 a=100) { Color.Set( r,g,b); Color.SetAlpha(float(a/100.0));}
	void SetColor( const CColor& color) 		{ Color.Set( color); }
	
    void SetSize( int size)		{ Size=size; }
    
	void Draw(int x, int y);
	
	void DrawText(int x, int y, string Text);
	
	void DrawText(int x, int y, string Text, const CColor& color, int size)
	{
		SetColor( color); SetSize( size); DrawText(x,y,Text);
	}
	
	void DrawNumber(int x, int y, int number);
	
	void DrawNumber(int x, int y, int number, const CColor& color, int size)
	{
		SetColor( color); SetSize( size); DrawNumber(x,y,number);
	}
	
	int DrawChar(int x, int y, char c);
	
     

	
};



