#pragma once
#include "Model.h"


class CPoint3 : public CModel
{
	public:
	float Size;
	
	CPoint3(float x, float y, float z, float size=3) : CModel()
	{
		Position.x=x; Position.y=y; Position.z=z;
		Size=size; Color.Set( 1.0, 1.0, 0.0, 1.0); // yellow	
	}


	
	CPoint3(vector3f& pos, float size=3) : CModel()
	{
		Position.x=pos.x; Position.y=pos.y; Position.z=pos.z;
		Size=size; Color.Set( 1.0, 1.0, 0.0, 1.0); // yellow	
		
		//cout << pos.x << " " << pos.y << " " << pos.z << endl;
	}

	virtual void Draw(CGraphics* g)
	{
	   glColor4f( Color.R, Color.G, Color.B, Color.A);
	   glPointSize( Size); 

		glBegin(GL_POINTS);
	  		glVertex3fv( (float*)&Position );
		glEnd();
  	}
};
