#pragma once

#include "Sprite.h"

class CSpritePoint : public CSprite
{
public:
	// Constructor
	CSpritePoint() : CSprite() { Radius = 0.0f; }
	
	float Radius;
	
	CSpritePoint(float x, float y, float r=3.0f) : CSprite()
    {
	  Xpos=x; Ypos=y;
	  Radius = r;
      Color.Set( CColor::Yellow());    
    }
	virtual ~CSpritePoint() {}

	virtual void Draw(CGraphics* g)
    {
       glColor4f( Color.R, Color.G, Color.B, Color.A);
	   glPointSize( float(2*Radius)); 

		glBegin(GL_POINTS);
	  		glVertex3f( Xpos, Ypos, 0);
		glEnd();
  	}
	
	
};
