#pragma once

#include "GTEC.h"
#include "Color.h"
#include "Vector3.h"


// ---- Implementation of a OpenGL graphics related functions ------
class CGraphics
{
public:
	CGraphics() 
	{ 
		Height=Width=0; 
		centerX=centerY=centerZ=0.0f; 
		eyeX=0.0f; eyeY=0.0f; eyeZ=15.0f;
		viewAngle=40.0f; nearPlane=1.0f; farPlane=1000.0f; 
		debug=false; Color.Set(CColor::Black());
	}
	~CGraphics() {}


//private:

Uint16 Height,Width;    	// width and height of the game viewport
float viewAngle;           // viewing angle
float nearPlane;			// position of the near plane of the viewing frustum
float farPlane;				// position of the far plane of the viewing frustum
float eyeX,eyeY,eyeZ;
float centerX,centerY,centerZ;

bool debug;

// modelview matrix
GLfloat view[16];

// fill colour
CColor Color;

public:

void SetViewPort(Uint16 w, Uint16 h) { Width=w; Height=h;}

void SetViewAngle(float angle) { viewAngle=angle;}

void SetClearColor( const CColor& color)  { Color.Set( color); }

void InitializeGL();

void SetProjection2D();

void SetProjection3D();

void SetDebugGraphics( bool db) { debug=db;}

bool IsDebug() { return debug; }

void UpdateViewMatrix() { glGetFloatv(GL_MODELVIEW_MATRIX, view); }

// where is a point on the screen
CVector WorldToScreenCoordinate(CVector pos);
// where is a point on the floor
CVector ScreenToFloorCoordinate( int x, int y);

private:

void PrintMatrix( float* m, int size);

bool gluInvertMatrix(const float m[16], float invOut[16]);







		
};

