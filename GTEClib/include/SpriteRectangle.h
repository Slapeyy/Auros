#pragma once

#include "Sprite.h"

class CSpriteRectangle : public CSprite
{
public:
	
	// Constructors & Destructor
	CSpriteRectangle() : CSprite() {}
	
	
	CSpriteRectangle(float x, float y, float width, float height, bool fill=true) : CSprite()
    {
        Xpos=x; Ypos=y; Width=width; Height=height; Color.Set( CColor::Gray()); filled=fill;
    }


	CSpriteRectangle(float x, float y, float width, float height, const CColor& color, bool fill=true) : CSprite()
    {
        Xpos=x; Ypos=y; Width=width; Height=height; Color.Set( color); filled=fill;
    }
	
	virtual ~CSpriteRectangle() {}




    // draw the rectangle as an OpenGL Quad 
	virtual void Draw(CGraphics* g)
    {
		
        if (!filled) glPolygonMode( GL_FRONT, GL_LINE);
        else glPolygonMode( GL_FRONT, GL_FILL);
		
    	glColor4f( Color.R, Color.G, Color.B, Color.A);
	
    	glPushMatrix();
		  glTranslatef( Xpos, Ypos, 0);  	// transformation to world coordinates
		  glRotatef( Rotation, 0, 0, 1 );   // rotation in degrees

		  glBegin(GL_QUADS);
		   glVertex3f( -Width/2.0f, -Height/2.0f,   0);
		   glVertex3f(  Width/2.0f, -Height/2.0f,   0);
		   glVertex3f(  Width/2.0f,  Height/2.0f,   0);
		   glVertex3f( -Width/2.0f,  Height/2.0f,   0);
		  glEnd();
		  
		glPopMatrix();
		glPolygonMode( GL_FRONT, GL_FILL);
		
		
		if (debug)
		{
		  glColor3f(1.0f,0,0); // red
		
		  glPolygonMode( GL_FRONT, GL_LINE);
	
    	  glPushMatrix();
		   glTranslatef( Xpos, Ypos, 0);  	// transformation to world coordinates
		   glRotatef( Rotation, 0, 0, 1 );   // rotation in degrees

		  glBegin(GL_QUADS);
		   glVertex3f( ScaleBB*-Width/2.0f, ScaleBB*-Height/2.0f,   0);
		   glVertex3f( ScaleBB*Width/2.0f,  ScaleBB*-Height/2.0f,   0);
		   glVertex3f( ScaleBB*Width/2.0f,  ScaleBB*Height/2.0f,   0);
		   glVertex3f( ScaleBB*-Width/2.0f, ScaleBB*Height/2.0f,   0);
		  glEnd();
		  
		 glPopMatrix();
		 glPolygonMode( GL_FRONT, GL_FILL);
		}
	}
	
};
