
#include "Floor.h"

void CFloor::LoadTexture(string filename)
{
	string fname = "images/" + filename;
	Texture.LoadTexture(fname); 
	
	Color.Set( 1.0, 1.0, 1.0, 1.0);
}


void CFloor::Draw(CGraphics* g)
{
  float texXR=1.0f, texYR=1.0f;
    
    glPushMatrix();
      glTranslatef( Position.x, Position.y-1, Position.z);  	// transformation to world coordinates
      
    if (Texture.GetTextureID())
    {
	 glColor4f(1.0,1.0,1.0, 1.0);
	
	 if (tiling)
	 {
	  texXR=float(Width)/Texture.GetWidth();
	  texYR=float(Depth)/Texture.GetHeight();
     }
     
	 glBindTexture( GL_TEXTURE_2D, Texture.GetTextureID());  // binding texture
     glBegin(GL_QUADS);
      glNormal3f(0,0,0);
	  glTexCoord2f(0.0, texYR); glVertex3f(-Width/2, Position.y, -Depth/2);
	  glTexCoord2f(texXR, texYR); glVertex3f( Width/2, Position.y, -Depth/2);
	  glTexCoord2f(texXR, 0.0); glVertex3f( Width/2, Position.y,  Depth/2);
	  glTexCoord2f(0.0, 0.0); glVertex3f(-Width/2, Position.y,  Depth/2);
     glEnd();
	 glBindTexture( GL_TEXTURE_2D, 0);
	}
	else
	{

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
     glColor3f(Color.R, Color.G, Color.B);
     //cout << Color.G << endl;
     //glColor3f(0, 0.3f, 0);
     glBegin(GL_QUADS);
     glNormal3f(0,0,0);
	  glVertex3f(-Width/2, Position.y, -Depth/2);
	  glVertex3f( Width/2, Position.y, -Depth/2);
	  glVertex3f( Width/2, Position.y,  Depth/2);
	  glVertex3f(-Width/2, Position.y,  Depth/2);
     glEnd();
     
    }
  glPopMatrix();
  
  if (drawgrid)
  {
  glColor3f( 0.7f, 0.7f, 0.7f);  // the grid is grey
  glLineWidth(1.0f);

  glPushMatrix();
    glTranslatef( Position.x, Position.y+3, Position.z);  	// transformation to world coordinates	
	
	 glBegin(GL_LINES);
	 glNormal3f(0,0,0);
     for (float n=-Width/2; n <= Width/2; n=n+100)
     {
	   glVertex3f( n, Position.y, -Depth/2); 
	   glVertex3f( n, Position.y, Depth/2); 
	 }
	 
	 for (float n=-Depth/2; n <= Depth/2; n=n+100)
     {
	   glVertex3f( -Width/2, Position.y, n); 
	   glVertex3f( Width/2, Position.y, n); 
	 }
	glEnd();
	glPopMatrix();
   }
}



