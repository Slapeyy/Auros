#pragma once

#include "GTEC.h"
#include "Model.h"


using namespace std;

class CFloor
{
public:
	
	// ----- Constructors & Destructor -----
	
	CFloor() 
	{ 
		
		Width=Depth=0; 
		Position.x=Position.y=Position.z=0; 
		tiling=false; 
		drawgrid=false; 
		Color.Set(0, 0.5f, 0, 1.0); 
		heightmap=NULL;
	
	}
	
	~CFloor() 
	{ 
		
		
	}

	// ----- Attributes ------

private:
	
	// Position & Size
	float  Width, Depth; // width, depth
	CVector Position; // centre position
    CTexture Texture;			   // OpenGL texture
	CColor Color;				   // colour
	bool tiling;				   // should the texture be tiled?
	bool drawgrid;
	float** heightmap;
	
public:
	

	// Size of the Texture
	float GetWidth()			    { return Width; }
	float GetDepth()		        { return Depth; }
    float GetHeight( float x, float z)	    { return 0; }
	// multiple of hundreds
	void SetSize(float x, float z)         { Width=x; Depth=z; }
	void SetPosition( float x, float z)    { Position.x=x; Position.z=z; }
	void SetPosition( float x, float y, float z)    { Position.x=x; Position.y=y; Position.z=z; }
	void SetPositionV(CVector pos) { Position=pos; }
	void Move( float dx, float dy, float dz) { Position.x+=dx; Position.y+=dy; Position.z+=dz; }
	
	
	
	void LoadTexture(string filename);
	
	void SetColor( const CColor& c) { Color.Set( c);  }
	void SetTiling( bool tile) { tiling=tile;}
	void ShowGrid(bool grid=true) { drawgrid=grid; }
	
    void Draw(CGraphics* g);
    


};



