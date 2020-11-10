#pragma once

#include "Sprite.h"

class CHealthBar : public CSprite
{
public:
	// Constructor
	CHealthBar() : CSprite() 
	{ 
	  Color.Set( CColor::Green());
	  Border.Set(CColor::Black());
	  Background.Set( CColor::Red());
	  
	}
	
	CHealthBar(int x, int y, int width, int height) : CSprite()
    {
        Xpos=(float)x; Ypos=(float)y; Width=(float)width; Height=(float)height; Color.Set( CColor::Green()); Border.Set(CColor::Black());
        Background.Set( CColor::Red());
    }
	
	virtual ~CHealthBar() {}
	
    CColor Border;
    CColor Background;
	
	void Init(int x, int y, int width, int height) 
	{
		Xpos=(float)x; Ypos=(float)y;  Width=(float)width; Height=(float)height;
	}
	
	
	void SetColors( const CColor& bar, const CColor& background, const CColor& border) 		
	{ 
		Color.Set( bar); 
		Border.Set( border);
		Background.Set(background);
	}

	virtual void Draw(CGraphics* g)
    {
       
    	glPushMatrix();
		  glTranslatef( (float)Xpos, (float)Ypos, 0);  	// transformation to world coordinates
		  glRotatef( (float)Rotation, 0, 0, 1 );   // rotation in degrees

          glColor4f( Border.R, Border.G, Border.B, Border.A);
 
		  glBegin(GL_QUADS);
		   glVertex3f(float(-Width/2-1), float(-Height/2-1),   0);
		   glVertex3f(float(Width/2+1), float(-Height/2-1),   0);
		   glVertex3f(float(Width/2+1),  float(Height/2+1),   0);
		   glVertex3f(float(-Width/2-1),  float(Height/2+1),   0);
		  glEnd();


          glColor4f( Background.R, Background.G, Background.B, Background.A);

		  glBegin(GL_QUADS);
		   glVertex3f(float(-Width/2), float(-Height/2),   0);
		   glVertex3f(float(Width/2), float(-Height/2),   0);
		   glVertex3f(float(Width/2),  float(Height/2),   0);
		   glVertex3f(float(-Width/2),  float(Height/2),   0);
		  glEnd();
		  
		  
		glColor4f( Color.R, Color.G, Color.B, Color.A);
		
		
		if (Health < 0) Health=0;
		if (Health > 100) Health=100;
		  
		float w=float(Health*Width/100);
		
		  
		  
		  glBegin(GL_QUADS);
		   glVertex3f(float(-Width/2), float(-Height/2),   0);
		   glVertex3f(float(-Width/2+w), float(-Height/2),   0);
		   glVertex3f(float(-Width/2+w),  float(Height/2),   0);
		   glVertex3f(float(-Width/2),  float(Height/2),   0);
		  glEnd();  
		  
		  
		glPopMatrix();
		
  	}
	
	
};
