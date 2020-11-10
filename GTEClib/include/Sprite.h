#pragma once

#include "GTEC.h"
#include "Color.h"
#include "Texture.h"
#include "Graphics.h"
#include <list>
#include <vector>


using namespace std;

class CSprite
{
public:
	
	// ----- Constructors & Destructor -----
	
	CSprite();
	CSprite( float x, float y, CSprite &sprite, Uint32 time=0);

	virtual ~CSprite() {}

	// ----- Attributes ------

    static bool debug;
    
   
protected:

    // Appearance
	CColor Color;			// sprite colour
	CTexture Texture;  		// texture, zero if no texture assigned
	bool filled;
	bool looping;		 
	
	// Animation variables
	int numFrames;	    		// number of frames
	int startFrame, stopFrame;
	float currentFrame, period;
	
	// Position & Size
	float Xpos, Ypos; // position is the center point of the sprite 
	float Width, Height;	// width and height of sprite
	float Scale;			// scale of the sprite
	float ScaleBB;			// scale of the bounding box

	// Sprite Time
	Uint32 SpriteTime;		// reference time
	
	// Sprite states
	int Status;				// user defined status
	int Health;				// health value
	bool marked_for_removal;
	
	int dying;
	
	
	// Dynamics - Linear
	float Speed;		// movement speed in pixel/second
	float Direction;      // movement direction in degrees
	
	// Dynamics - Rotational
	float Rotation;		// rotation (orientation) in degrees (zero pointing right)
	float Omega;			// rotational velocity in degrees per second


public:
	
	// Position functions
	float GetX()								{ return Xpos; }
	float GetY()								{ return Ypos; }
	void SetX( float x)                      { Xpos = x; }
	void SetY( float y)                      { Ypos = y; }
	void SetPosition(float x, float y)	{ Xpos = x; Ypos = y; }
	
	void Move(float dx, float dy)			{ Xpos += dx; Ypos += dy; }
	void Move( float distance);

	// Size functions
	float GetWidth()							{ return Width; }
	float GetHeight()							{ return Height; }
	void SetSize(float w, float h);
	void SetScale(float scale);
	float GetScale()						{ return Scale; }
	void SetBoxScale(float scale)		{ ScaleBB = scale;}
	float GetRadius();							 
	
	// -------  Hit Tests-------				
	bool HitTest(float x, float y, float minDistance);
	virtual bool HitTest(float x, float y);
	virtual bool HitTest(CSprite *pSprite);
	
	bool HitTestFront(CSprite* pSprite);
	
	// Sprite Time
	void ResetTime()						{ SpriteTime = 0; }

	
	// Sprite State
	int GetHealth()							{ return Health; }
	void SetHealth(int health)			    { Health = health; }
	int GetStatus()							{ return Status; }
	void SetStatus(int status)			    { Status = status; }
	
	
	//  ------- Animation Functions --------------
	void SetFrame( int frame)				{ currentFrame=(float)frame; period=0.0; startFrame=frame; stopFrame=frame;}
	void NextFrame()					    { currentFrame++; if (currentFrame > numFrames) SetFrame( 1); }
	int GetFrame()							{ return (int)currentFrame; }
	int GetMaxFrames()						{ return numFrames; }	
	
	void PlayAnimation( int start, int stop, float speed, bool loop=false);
	
	bool IsAnimationFinished()				{ return stopFrame==currentFrame;}
	
	
    // ---------- Motion Functions ----------------
    
	
	// retrieve direction of the sprite motion in degrees
	float GetDirection()						{ return Direction; }
	
	// sets the direction
	void SetDirection(float dir)				{ Direction = dir; } 
	void SetDirection(float dX, float dY)		
	{ 
		Direction = (float)RAD2DEG (atan2(dY, dX)); // tan dir = dX/dY
	}
	
	// get the sprite speed in pixels per second
	float GetSpeed()						{ return Speed; }

	// set sprite speed
	void SetSpeed(float vel)				{ Speed = vel; }

	// set motion from horizontal and vertical speed.
	void SetMotion(float hSpeed, float vSpeed)	
	{
		SetSpeed( (float)sqrt( (hSpeed*hSpeed)+(vSpeed*vSpeed)));
		SetDirection( hSpeed, vSpeed);
	}
	
	// set sprite velocity	
	void SetVelocity(float hSpeed, float vSpeed)	{ SetMotion( hSpeed, vSpeed); }

	void SetXVelocity(float vH)				{ SetMotion( vH, GetYVelocity()); }
	void SetYVelocity(float vV)				{ SetMotion( GetXVelocity(), vV); }
		
	// retrieve horizontal & vertical component of the velocity vector
	float GetXVelocity()						{ return float(Speed*cos( DEG2RAD (Direction))); }
	float GetYVelocity()						{ return float(Speed*sin( DEG2RAD (Direction))); }
	
	// rotation angle of the sprite
    void Rotate(float rot)					{ Rotation += rot; if (Rotation >= 360) Rotation -= 360;}
    void SetRotation(float dX, float dY)	{ Rotation = (float)RAD2DEG( atan2(dY, dX)); }
    void SetRotation( float rot)				{ Rotation=rot; }
	float GetRotation()						{ return Rotation; }

	// rotation speed omega in degrees/second
    float GetOmega()							{ return Omega; }
	void SetOmega(float omg) 				    { Omega = omg; }

	bool IsDeleted()							{ if (dying > 0) return false;
												  else return marked_for_removal; }
    void Delete()								{ dying = 0; marked_for_removal = true; }
    void UnDelete()								{ dying = 0; marked_for_removal = false; }
    void Die(int delay=0)					    { dying = delay; marked_for_removal = true; }
    void UnDie()								{ dying = 0; }

	// Update & Draw
	void SetColor( Uint8 r, Uint8 g, Uint8 b, Uint8 a=100) { Color.Set( (float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)a/100.0f);}
	void SetColor( const CColor& color) 		{ Color.Set( color); }
	void SetAlpha( int alpha) 			{ Color.SetAlpha( (float)alpha/100.0f);}
	void SetFilled( bool fill)			{ filled=fill; }

    // Images
    void LoadImage(string filename);
    void LoadImage(string filename, CColor colorKey, int frames=1);
    
	// Update and Draw function
	void Update(Uint32 GameTime);
	virtual void Draw(CGraphics* g);

};


typedef std::vector<CSprite*> CSpriteVector;
typedef std::list<CSprite*> CSpriteListBase;
typedef std::list<CSprite*>::iterator CSpriteIter;

inline bool deletedSprite(CSprite *p) 
{ 
	bool isDeleted = p->IsDeleted();
	if (isDeleted) delete p;
	return isDeleted;
}

class CSpriteList : public CSpriteListBase
{
  public:
  void remove_if(bool b)
  {
	  CSpriteListBase::remove_if(deletedSprite);
  }
  
  void delete_if(bool b)
  {
	  CSpriteListBase::remove_if(deletedSprite);
  }
  
  void Update(long t)
  {
	for (CSpriteIter i = begin(); i != end(); i++)
	{
	  (*i)->Update(t);
    }  
  }
  
  void Draw(CGraphics* g)
  {
	for (CSpriteIter i = begin(); i != end(); i++)
	{
	  (*i)->Draw(g);
    }  
  }
  
  virtual void clear()
  {
	for (CSpriteIter i = begin(); i != end(); i++)
	{
	  delete (*i);
    }  
    list::clear();
	  
  }
  
};



