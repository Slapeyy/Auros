

#include "Sprite.h"


CSprite::CSprite()
{
	Xpos=Ypos=0.0f;		
	Width=Height=0.0f;
	Scale=ScaleBB=1.0f;
	
	numFrames=1; currentFrame=1.0f; period=0.0f; looping=false;
	startFrame=stopFrame=1;

	Color.Set( 1.0, 0.0, 0.0, 1.0); // red
	filled=true;
	
	// Time
	SpriteTime=0;
	
	Health=100;	Status=0;		
	
	// Linear Motion
	Speed=0;		
	Direction=0;    
	
	// Rotation
	Rotation=0;		
	Omega=0;

	marked_for_removal=false; 
	dying = 0;
	
}


CSprite::CSprite( float x, float y, CSprite &sprite, Uint32 time)
{
	
	Xpos = x; Ypos = y;	
	
	// cloning appearance of existing sprite
	Width = sprite.Width;
	Height = sprite.Height;	
    Scale = sprite.Scale;
    ScaleBB = sprite.ScaleBB;

    Texture.SetTextureID( sprite.Texture.GetTextureID());

	filled=sprite.filled;
	
	numFrames = sprite.numFrames;
	currentFrame = sprite.currentFrame;
	period = sprite.currentFrame; 
	
	looping = sprite.looping;
	startFrame = sprite.startFrame;
	stopFrame = sprite.stopFrame;

	SetColor( sprite.Color);
	
	// Time
	SpriteTime=time;
	
	Health=100;	Status=0;		
	
	// Linear Motion
	Speed=0;		
	Direction=0;         
	
	// Rotation
	Rotation=0;		
	Omega=0;	
	
	marked_for_removal=false;
	dying = 0;

}

void CSprite::SetSize(float w, float h)
{
	Width = w; Height = h;
	// scale remains the same
}

void CSprite::SetScale( float scale)
{
	if (scale > 0)
	{
	 // size changes
	  Width= float(Width*scale/Scale);
	  Height=float(Height*scale/Scale);
		
	  Scale=scale;
	}	
} 

void CSprite::LoadImage(string filename)
{
	string fname = "images/" + filename;
	Texture.LoadTexture(fname);

	Width = (float)Texture.GetWidth();
	Height = (float)Texture.GetHeight();
   
	numFrames = Texture.GetFrames();
	currentFrame=1.0f; period=0.0f; looping=false;
	startFrame=stopFrame=1;

	Color.Set( 1.0, 1.0, 1.0, 1.0); // White
	
	filled=true;
	
}

void CSprite::LoadImage(string filename, CColor colorkey, int frames)
{
	string fname = "images/" + filename;
	Texture.LoadTexture(fname, colorkey, frames);

	Width = (float)Texture.GetWidth();
	Height = (float)Texture.GetHeight();
   
	filled=true;
	
	numFrames = Texture.GetFrames();
	currentFrame=1.0f; period=0.0f; looping=false;
	startFrame=stopFrame=1;

	Color.Set( 1.0, 1.0, 1.0, 1.0); // White
}



float CSprite::GetRadius()
{
	
   return (float)sqrt( (Width/2.0)*(Width/2.0)+(Height/2.0)*(Height/2.0));

}


// ----------  Collision detection -----------


// true if the distance between this sprite and the position (x,y) is less than the specified minDistance 
bool CSprite::HitTest(float x, float y, float minDistance)
{
	float distance=(float)sqrt((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos));
	
	if (distance < minDistance) return true;
	return false;
}


// true if the position (x,y) is inside the bounding box of this sprite
bool CSprite::HitTest(float x, float y)
{
    float x1=-ScaleBB*Width/2.0f;
    float x2=ScaleBB*Width/2.0f;
	float y1=-ScaleBB*Height/2.0f;
	float y2=ScaleBB*Height/2.0f;

	float xp,yp;
	   
	// correcting for bounding box rotation and position
    xp = (x-Xpos)*(float)cos(DEG2RAD (-Rotation)) - (y-Ypos)*(float)sin(DEG2RAD (-Rotation));
	yp = (x-Xpos)*(float)sin(DEG2RAD (-Rotation)) + (y-Ypos)*(float)cos(DEG2RAD (-Rotation));

    if (xp >= x1 && xp <= x2 && yp >= y1 && yp <= y2) return true;
	return false;
}


// are bounding rectangles overlapping?
	
	bool CSprite::HitTest(CSprite *pSprite) 
    { 
       float Xpos2=pSprite->GetX();
	   float Ypos2=pSprite->GetY();
		
       float distance=(float)sqrt((Xpos2-Xpos)*(Xpos2-Xpos)+(Ypos2-Ypos)*(Ypos2-Ypos));
	   float radius1=ScaleBB*GetRadius(); 
	   float radius2=pSprite->ScaleBB*pSprite->GetRadius();

       // sprites are too far apart and could not possibly overlap
	   if (distance > radius1+radius2) return false;

	   // centre of pSprite inside this sprite?
       if ( HitTest( Xpos2, Ypos2)) return true;
       
       // centre of pSprite inside this sprite?
       if ( pSprite->HitTest( Xpos, Ypos)) return true;
       
       // ----- 4 bounding points of this sprite inside pSprite? ----
         
       float x1=-ScaleBB*Width/2.0f;
	   float x2=ScaleBB*Width/2.0f;
	   float y1=-ScaleBB*Height/2.0f;
	   float y2=ScaleBB*Height/2.0f;
     
       // top left hand corner    
       Xpos2 = Xpos + x1*(float)cos(DEG2RAD (Rotation)) - y2*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x1*(float)sin(DEG2RAD (Rotation)) + y2*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // top right hand corner 
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y2*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y2*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // bottom right hand corner
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y1*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y1*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // bottom left hand corner   
       Xpos2 = Xpos + x1*(float)cos(DEG2RAD (Rotation)) - y1*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x1*(float)sin(DEG2RAD (Rotation)) + y1*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;

       
       // ----- 4 bounding points of pSprite inside this sprite? ----
         
       x1=-pSprite->ScaleBB*pSprite->Width/2.0f;
	   x2=pSprite->ScaleBB*pSprite->Width/2.0f;
	   y1=-pSprite->ScaleBB*pSprite->Height/2.0f;
	   y2=pSprite->ScaleBB*pSprite->Height/2.0f;
         
         
       // top left hand corner    
       Xpos2 = pSprite->Xpos + x1*(float)cos(DEG2RAD (pSprite->Rotation)) - y2*(float)sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + x1*(float)sin(DEG2RAD (pSprite->Rotation)) + y2*(float)cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( Xpos2, Ypos2)) return true;
       // top right hand corner 
       Xpos2 = pSprite->Xpos + x2*(float)cos(DEG2RAD (pSprite->Rotation)) - y2*(float)sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + x2*(float)sin(DEG2RAD (pSprite->Rotation)) + y2*(float)cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( Xpos2, Ypos2)) return true;
       // bottom right hand corner
       Xpos2 = pSprite->Xpos + x2*(float)cos(DEG2RAD (pSprite->Rotation)) - y1*(float)sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + x2*(float)sin(DEG2RAD (pSprite->Rotation)) + y1*(float)cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( Xpos2, Ypos2)) return true;
       // bottom left hand corner   
       Xpos2 = pSprite->Xpos + x1*(float)cos(DEG2RAD (pSprite->Rotation)) - y1*(float)sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + x1*(float)sin(DEG2RAD (pSprite->Rotation)) + y1*(float)cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( Xpos2, Ypos2)) return true;
      
       return false;
    }

	bool CSprite::HitTestFront(CSprite* pSprite)
	{
	   float Xpos2=pSprite->GetX();
	   float Ypos2=pSprite->GetY();
		
       float distance=(float)sqrt((Xpos2-Xpos)*(Xpos2-Xpos)+(Ypos2-Ypos)*(Ypos2-Ypos));
	   float radius1=ScaleBB*GetRadius(); 
	   float radius2=pSprite->ScaleBB*pSprite->GetRadius();

       // sprites are too far apart and could not possibly overlap
	   if (distance > radius1+radius2) return false;

	   // centre of pSprite inside this sprite?
       if ( HitTest( Xpos2, Ypos2)) return true;
       
       // centre of pSprite inside this sprite?
       if ( pSprite->HitTest( Xpos, Ypos)) return true;
       
       // ----- 3 forward in x-direction (right) bounding points of this sprite inside pSprite? ----
         
	   float x2=ScaleBB*Width/2.0f;
	   
	   float y1=-ScaleBB*Height/2.0f;
	   float y2=ScaleBB*Height/2.0f;
	   float y3=0.0f;
	  
	   
	   // top right hand corner 
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y2*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y2*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // bottom right hand corner
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y1*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y1*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // middle right had side
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y3*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y3*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       
       return false;

	}



// -----------    Update -----------------

void CSprite::Update(Uint32 GameTime)
{
	if (SpriteTime == 0) { SpriteTime = GameTime; return; }

	int deltaTime = GameTime - SpriteTime;
	if ( deltaTime < 0) deltaTime = 0;
	
	if (dying > 0) dying=dying-deltaTime;

	// linear motion update
    float dx = Speed*(float)cos( DEG2RAD (Direction))*deltaTime/1000.0f;
	float dy = Speed*(float)sin( DEG2RAD (Direction))*deltaTime/1000.0f;

    Xpos += dx; Ypos += dy;
	
	// rotation update
	Rotate( Omega*deltaTime/1000.0f);
	
	// animation update
	if (period > 0)
	{
	  currentFrame=currentFrame+deltaTime*period/1000.0f;
	  if (currentFrame >= stopFrame+1) 
	  { 
		if (looping) currentFrame=(float)startFrame;
		else SetFrame( stopFrame); 
	  }
    }
    
    if (dying)
    {
		if (IsAnimationFinished()) Delete();
	}
    
    SpriteTime = GameTime;
}


void CSprite::Move( float distance)
{
      Xpos += distance*(float)cos( DEG2RAD (Direction));
	  Ypos += distance*(float)sin( DEG2RAD (Direction));
}

void CSprite::PlayAnimation( int start, int stop, float speed, bool loop)
{
	  if (currentFrame <= start || currentFrame >= stop+1 || period == 0)
	  {
	    startFrame=start; stopFrame=stop; period=speed;
	    currentFrame = (float)start; looping=loop;
	  }
	
}


void CSprite::Draw(CGraphics* g)  
{	
    if (Texture.GetTextureID()  <  1)
	{
    // ----------  If no texture assigned draw a red bounding box with a green cross --------
    glPolygonMode( GL_FRONT, GL_LINE);
     glColor3f( 0 , 1.0f, 0);
     glPushMatrix();
		glTranslatef( Xpos, Ypos, 0);  // transformation to world coordinates
		glRotatef( Rotation, 0, 0, 1 );  // rotation
	
		glBegin(GL_QUADS);
	  		glVertex3f(float(-10), float(-10),   0);
	  		glVertex3f( float(10), float(-10),   0);
	  		glVertex3f( float(10),  float(10),   0);
	  		glVertex3f(float(-10),  float(10),   0);
		glEnd();
		glColor3f(1.0f , 0, 0);
		glBegin(GL_LINES);
	  	  glVertex3f( -10.0, 10.0, 0);
	      glVertex3f( 10.0, -10.0, 0);
	      glVertex3f( -10.0, -10.0, 0);
	      glVertex3f( 10.0, 10.0, 0);
		glEnd();
		
	 glPopMatrix();
	 glPolygonMode( GL_FRONT, GL_FILL);
	 
	}
	else  // draw texture as a rectangular sprite
	{
        glColor4f( Color.R, Color.G, Color.B, Color.A);
		// binding texture
   		glBindTexture( GL_TEXTURE_2D, Texture.GetTextureID());  

   		glPushMatrix();
   		glTranslatef( Xpos, Ypos, 0);    // transformation to world coordinates
		glRotatef( Rotation, 0, 0, 1 );   // rotation in degrees
	
	   if (numFrames > 1)  // animated texture: select the correct frame
	   {
		double dx=1.0/numFrames;
		glBegin(GL_QUADS);
			glTexCoord2f(float(((int)currentFrame-1)*dx), 0.0); glVertex3f( -Width/2.0f, -Height/2.0f,   0);
			glTexCoord2f(float((int)currentFrame*dx), 0.0);     glVertex3f(  Width/2.0f, -Height/2.0f,   0);
			glTexCoord2f(float((int)currentFrame*dx), 1.0);     glVertex3f(  Width/2.0f,  Height/2.0f,   0);
			glTexCoord2f(float(((int)currentFrame-1)*dx), 1.0); glVertex3f( -Width/2.0f,  Height/2.0f,   0);
		glEnd();	
	   }
	   else  // no anmimated texture
	   {
		glBegin(GL_QUADS);
	  		glTexCoord2f(0.0f, 0.0f); glVertex3f( -Width/2.0f, -Height/2.0f,   0);
	  		glTexCoord2f(1.0f, 0.0f); glVertex3f(  Width/2.0f, -Height/2.0f,   0);
	  		glTexCoord2f(1.0f, 1.0f); glVertex3f(  Width/2.0f,  Height/2.0f,   0);
	  		glTexCoord2f(0.0f, 1.0f); glVertex3f( -Width/2.0f,  Height/2.0f,   0);
		glEnd();
		
	   }
    	glPopMatrix();	

  		glBindTexture( GL_TEXTURE_2D, 0);
	}
	
	// ---- draw red bounding box -----
	if (g->IsDebug())
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
