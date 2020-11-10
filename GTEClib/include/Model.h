#pragma once

#include "GTEC.h"
#include "Color.h"
#include "Graphics.h"
#include "Texture.h"
#include "Sprite.h"
#include "Vector3.h"
#include <list>
#include <vector>

using namespace std;


class CModel
{
public:
	
	// ----- Constructors & Destructor -----
	
CModel() 
	{
    Position.x=Position.y=Position.z=0.0f;
    localPosition.x=localPosition.y=localPosition.z=0.0f;
    
    minx=miny=minz=-0.5f;
    maxx=maxy=maxz=0.5f;
    
	Scale=1.0f;   // model scale
	ScaleBB=1.0f; // bounding box scale 
	
	Color.Set( 0.75f, 0.75f, 0.75f, 1.0f); // grey
	Color2.Set(0.8f, 0.8f, 0.8f, 1.0f); // grey
	filled=true; border=false;
	visible=true;
	
	// Time
	SpriteTime=0;
	
	// Properties
	Health=100;	Status=0; Id=0;	
	
	// Linear Motion { Veclocity and Direction }
	Direction.x=1.0f; // facing in the direction of the positive x-axis
	Direction.y=Direction.z=0.0f; 
	Speed=0.0f;		 	 // speed
	   
	// Rotation
	Rotation.x=Rotation.y=Rotation.z=0.0f;	// rotation angle
	Omega.x=Omega.y=Omega.z=0.0f; // rotation speed

	marked_for_removal=false; 
	dying=0;
	automove=false; 
	DistanceToTarget=0.0f;
	 
	childNode=NULL;
	
	// geometry data
	numVertices=0;
	
	vertexArray=NULL;			
	normalArray=NULL;			
	texArray=NULL;	
	
	isCloned=false;		
	
	// setting up animation variables
	numFrames=1; currentFrame=1.0f; period=0.0f; looping=false;
	startFrame=stopFrame=1;	
	
	
}
	
	
CModel(float x, float y, float z, float width, float height, float depth, const CColor& color=CColor::Blue())
{
	Position.x=x; Position.y=y; Position.z=z;
	localPosition.x=localPosition.y=localPosition.z=0.0f;
	minx=-width/2; maxx=width/2; miny=-height/2; maxy=height/2; minz=-depth/2; maxz=depth/2; Scale=1;
		
	ScaleBB=1.0f;
	
	Color.Set(color);
	Color2.Set(0.8f, 0.8f, 0.8f, 1.0f); // grey
	filled=true; border=false;
	visible=true;
	
	
	// Time
	SpriteTime=0;
	
	// Properties
	Health=100;	Status=0; Id=0;			
	
	// Linear Motion { Veclocity and Direction }
	Direction.x=1.0f; Direction.y=Direction.z=0.0f; 
	Speed=0.0f;		 			// speed
	   
	// Rotation
	Rotation.x=Rotation.y=Rotation.z=0;	// rotation angles	
	Omega.x=Omega.y=Omega.z=0; // rotation speed

	marked_for_removal=false;
	dying=0;
	
	//  automove functionality
	automove=false; 
	DistanceToTarget=0;
	 
	childNode=NULL;
	
	vertexArray=NULL;			
	normalArray=NULL;			
	texArray=NULL;	
		
	isCloned=false;		
	
	numVertices=0;	
	
	//currentFrame=0;	maxFrames=0;
	
	numFrames=1; currentFrame=1.0f; period=0.0f; looping=false;
	startFrame=stopFrame=1;	
	
}
	
	
	virtual ~CModel() 
	{
	  
	 Clear();
		
	}

	// ----- Attributes ------ 
           
protected:

    // Appearance
	CColor Color;			// model colour 1
	CColor Color2;			// model colour 2
	CTexture Texture;  		// texture, zero if no texture assigned
	bool looping;
	bool filled;
	bool border;
	bool visible;		 
	
	// Position & Size
	CVector Position;		// position (x,y,z) is the center point of the model in the game world
	CVector localPosition;  // position relative to the center point
	 
	float minx, maxx, miny, maxy, minz, maxz;
	
	float Scale;			// scale 1=100%
	float ScaleBB;			// scale bounding box

	// Model Time
	Uint32 SpriteTime;		// reference time
	
	// Model states
	int Status;				// user defined status
	int Health;				// health
	int Id;					// model id
	bool marked_for_removal; // indicate model can be removed
	int dying;
	
	// Auto movement 
	bool automove;				// are we auto moving?
	float DistanceToTarget;    // used for automatic movement towards target
	
	// Child node
	CModel* childNode;          // child node model reference
	bool isCloned;				// is this model cloned
	
	
	// Dynamics - Linear
	CVector Direction;			// movement direction vector
	float Speed;			// movement speed in pixel/second
	
	// Dynamics - Rotational
	CVector Rotation;			// rotation (orientation) in degrees around x,y,z axis
	CVector Omega;				// rotational velocity in degrees per second per x,y,z axis
	
	// Vertex data (vertices,normals,texture coordinates)
	vector3f* vertexArray;			// array containing vertex positions
	vector3f* normalArray;			// array containing vertex normals
	vector2f* texArray;				// array containing vertex texture coordinates
	
	int numVertices;				// number of vertices in the model
	
	
	// current and max frame in animation
	// Animation variables
	int numFrames;	    		// number of frames
	int startFrame, stopFrame;
	float currentFrame, period;


public:
	
	// Position functions
	float GetX()								{ return Position.x; }
	float GetY()								{ return Position.y; }
	float GetZ()								{ return Position.z; }
	void SetX( float x)                      { Position.x = x; }
	void SetY( float y)                      { Position.y = y; }
	void SetZ( float z)                      { Position.z = z; }
	void SetToFloor( float y);
	void SetPosition(float x, float y,float z)	{ Position.x=x; Position.y=y; Position.z=z; CNodeUpdate();}
	void SetPosition(float x, float z) { Position.x=x; Position.z=z; CNodeUpdate();}
	void SetPositionV(CVector pos) { Position=pos; CNodeUpdate();}
	
	CVector GetPositionV() { return Position;}
	CVector GetLocalPositionV(CVector pos);
	
	void Move(float dx, float dy, float dz)		{ Position.x += dx; Position.y += dy; Position.z += dz;}
	void MoveV(CVector v) { Position.x += v.x; Position.y += v.y; Position.z += v.z;}
	
    void Move( float distance);
    
    // todo: rename function: move distance in xz plane in direction of rotation angle
    void MoveXZRotation( float distance);
    
    
    // Automatic movement functions
    void MoveTo(float x, float z, float speed);
    bool IsAutoMoving() { return automove;}
    
    //void MoveToV(CVector& pos);
    void Stop() { automove=false; DistanceToTarget=0; SetSpeed(0);}
    
    
	// Size functions
	void SetSize(float width, float height, float depth) { minx=-width/2; maxx=width/2; miny=-height/2; maxy=height/2; minz=-depth/2; maxz=depth/2; Scale=1; }
	float GetTop() { return Position.y+Scale*maxy;}
	float GetBottom() { return Position.y+Scale*miny; }
	float GetFront() { return Position.x+Scale*maxx;}
	float GetBack() { return Position.x+Scale*minx; }
	float GetRight() { return Position.z+Scale*maxz;}
	float GetLeft() { return Position.z+Scale*minz; }
	void SetScale(float scale) 		{  Scale=scale; }
	float GetScale()					{ return Scale; }
	void SetBoxScale( float scale) { ScaleBB=scale; }
	
	float GetRadius();	
	
	
	void PrintP() { cout << "x: " << minx << " " << maxx << " y: " << miny << " " << maxy << " z: " << minz << " " << maxz << endl; }
	void PrintD() { cout << "dx: " << Direction.x << " dy: " << Direction.y << " dz: " << Direction.z << endl; }
	void PrintR() { cout << "rx: " << Rotation.x << " ry: " << Rotation.y << " rz: " << Rotation.z << endl; }								 
	
	// ----------  Bounding Box Hit Tests  --------				
	bool HitTest(CVector pos, float distance); 
	bool HitTest(CVector pos);
	virtual bool HitTest(CModel *pModel);
	
	// ----------- Partial Hit Tests -------
	bool HitTestFront(CModel *pModel);
	bool HitTestBack(CModel *pModel);
	bool HitTestLeft(CModel *pModel);
	bool HitTestRight(CModel *pModel);
	
	// Sprite Time
	void ResetTime()						{ SpriteTime = 0; }

	// ----------  Model States -----------
	int GetHealth()							{ return Health; }
	void SetHealth(int health)			    { Health = health; }
	int GetStatus()							{ return Status; }
	void SetStatus(int status)			    { Status = status; }
	void SetID( int id) 					{ Id = id; }
	int GetID()								{ return Id;}
	
	virtual void Action( int type) {}
	
    // ---------- Motion Functions ----------------
    
	// Direction = (float)RAD2DEG (atan2(dZ, dX)); // tan dir = dX/dZ
	
	// returns the direction in form of an angle in the XZ plane
	float GetDirection() { return (float)RAD2DEG (atan2(-Direction.z, Direction.x)); }
	//float GetDirection() { return (float)RAD2DEG (atan2(Direction.x, Direction.z)); }
	
	// set direction for the XZ-plane
	void SetDirection(float y_angle) 
	{  
		// rotation around y-axis
		float x1=float(cos( DEG2RAD (y_angle)));
		float z1=float(-sin( DEG2RAD (y_angle)));
		float y1=0;
		
		SetDirection(x1,y1,z1);
	}

    void SetDirectionAndRotationToPoint( float x, float z) 
    {
		float y_angle = (float)RAD2DEG (atan2(Position.z-z, x-Position.x));
		SetDirection( y_angle); SetRotation(y_angle);
	} 
	
	void SetDirectionToPoint( float x, float z) 
    {
		float y_angle = (float)RAD2DEG (atan2(Position.z-z, x-Position.x));
		SetDirection( y_angle);
	}
	
	void SetRotationToPoint( float x, float z) 
    {
		float y_angle = (float)RAD2DEG (atan2(Position.z-z, x-Position.x));
		SetRotation(y_angle);
	}
	

	// retrieve direction of the model motion in degrees
	CVector GetDirectionV()						{ return Direction; }
	
	void SetDirectionV( CVector dir) { SetDirection( dir.x, dir.y, dir.z); }

  
	// sets the direction
	void SetDirection(float dX, float dY, float dZ)		
	{ 
		float mag=(float)sqrt(dX*dX+dY*dY+dZ*dZ); // normalize direction vector
		if (mag != 0.0)
		{		
		   Direction.x=dX/mag; Direction.y=dY/mag; Direction.z=dZ/mag;
	    }
	    else { Direction.x=Direction.y=Direction.z=0; }    
	}
	
	 void SetDirection( float dX, float dZ) 
    { 
		float y_angle = (float)RAD2DEG (atan2(-dZ, dX));
		SetDirection( y_angle);
    }
	
	
	// get the model speed in units per second
	float GetSpeed()   { return Speed; }

	// set model speed
	void SetSpeed(float s)  { Speed = s; }

	// set motion
	void SetMotion(float xSpeed, float ySpeed, float zSpeed)	
	{
		Speed = (float)sqrt(xSpeed*xSpeed+ySpeed*ySpeed+zSpeed*zSpeed);
		
		if (xSpeed != 0.0f || ySpeed != 0.0f || zSpeed != 0.0f)
		{		
		   Direction.x=xSpeed/Speed; Direction.y=ySpeed/Speed; Direction.z=zSpeed/Speed;
	    }
	    else { Direction.x=Direction.y=Direction.z=0; }
	}
	void SetVelocity(float xSpeed, float ySpeed, float zSpeed)	{ SetMotion( xSpeed, ySpeed, zSpeed); }
	void SetVelocityV(CVector v)	{ SetMotion( v.x, v.y, v.z); }

	void SetXVelocity(float vX)				{ SetMotion( vX, GetYVelocity(), GetZVelocity()); }
	void SetYVelocity(float vY)				{ SetMotion( GetXVelocity(), vY, GetZVelocity()); }
	void SetZVelocity(float vZ)				{ SetMotion( GetXVelocity(), GetYVelocity(), vZ); }
	
		
	// retrieve x,y,z component of the velocity vector
	float GetXVelocity()						{ return Speed*Direction.x; }
	float GetYVelocity()						{ return Speed*Direction.y; }
	float GetZVelocity()						{ return Speed*Direction.z; }
	
	// rotation angles of the model
    void Rotate(float aX, float aY, float aZ)		{ Rotation.x += aX;  Rotation.y += aY; Rotation.z += aZ; }
    // rotation angles. Use just y and z angles
    void SetRotation(float aX, float aY, float aZ) { Rotation.x = aX;  Rotation.y = aY; Rotation.z = aZ; }
    // rotating towards the vector specified through dx,dy,dz
    void SetRotationV(float dX, float dY, float dZ);
    // rotating towards the vector specified as rot
    void SetRotationV(CVector rot)	{ SetRotationV(rot.x,rot.y,rot.z); }
    
    CVector GetRotationA()							{ return Rotation; }
    CVector GetRotationV();						
    
    void Rotate( float rY)							{ Rotation.y += rY; }
    void SetRotation( float y_angle)				{ Rotation.y=y_angle; }
    
    float GetRotation() 							{ return Rotation.y;}
	
	
	// rotational speed omega in degrees/second
    CVector GetOmegaV()									{ return Omega; }
    float GetOmega() { return Omega.y; }
	void SetOmega(float omgX, float omgY, float omgZ) 	{ Omega.x = omgX; Omega.y = omgY; Omega.z = omgZ; }
	void SetOmega( float omgY) { Omega.y = omgY; }
	void SetOmegaV( CVector omg) { Omega.x = omg.x; Omega.y = omg.y; Omega.z = omg.z;}

	
	// Colors
	void SetColor( Uint8 r, Uint8 g, Uint8 b, Uint8 a=100) { Color.Set( (float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)a/100.0f); border=false;}
	void SetColor( const CColor& color) 		{ Color.Set( color); border=false; }
	void SetColors( const CColor& fillColor, const CColor& borderColor ) { Color.Set( fillColor); Color2.Set( borderColor); border=true; }
	void SetAlpha( int alpha) 			{ Color.SetAlpha( (float)alpha/100.0f);}
	void SetFilled( bool fill)			{ filled=fill; }
	void ShowBorder(bool b=true) { border=b; }

    
    // ---------- Loading Model Geometry and Textures------

    virtual bool LoadModel(string obj_filename);
    
    virtual void LoadTexture(string bmp_filename);
    
    virtual bool LoadModel(string obj_filename, string bmp_filename)
    {
		if (LoadModel(obj_filename)) { LoadTexture( bmp_filename); return true; }
		return false;
	}
         
	// --------   Update and Draw functions ----------------------
	void Update(Uint32 GameTime);
	virtual void Draw(CGraphics* g);
	
	void SetVisible(bool show=true) { visible=true; }
	bool IsVisible() { return visible; }
	
	
	virtual int AnimateModel() { return 0; }
	
	//  ------- Animation Functions --------------
	void SetFrame( int frame)				{ currentFrame=(float)frame; period=0.0; startFrame=frame; stopFrame=frame;}
	void NextFrame()					    { currentFrame++; if (currentFrame > numFrames) SetFrame( 1); }
	int GetFrame()							{ return (int)currentFrame; }
	int GetMaxFrames()						{ return numFrames; }	
	
	void PlayAnimation( int start, int stop, float speed, bool loop=false);
	
	bool AnimationFinished() {   return stopFrame==currentFrame; }
	
	// bool IsPlaying() {}
	
	bool IsDeleted()							{ if (dying > 0) return false;
												  else return marked_for_removal; }
    void Delete()								{ dying = 0; marked_for_removal = true; }
    void UnDelete()								{ dying = 0; marked_for_removal = false; }
    void Die(int delay=0)					    { dying = delay; marked_for_removal = true; }
    void UnDie()								{ dying = 0; }
	
	// ------ Child Node functions -------------
	
	void AttachModel(CModel* model);
	void AttachModel(CModel* model, CVector localPosition);
	void DetachModel() { childNode=NULL; }
	void DeleteAttachedModel() { if (childNode!=NULL) delete childNode; childNode=NULL;}
	CModel* GetAttachedModel() { return childNode; }
	CVector GetAttachedModelPositionV();
	
	// clone the current model
	virtual CModel* Clone();
	
	// clear vertex array data
	virtual void Clear();
	
	
	protected:
	
	void DrawBox(bool bbox);
	
	void CNodeUpdate();
	
	void DrawVertex( float x, float y, float z);
	
	void SetMaxFrames(int frame) { numFrames=frame; }
	
	void CalculateNormals();
	
	void DoRotation( CVector& pos, float yoffset=0);
	
	bool readFace(string& str, vector3f* tmpArrayV, vector3f* tmpArrayN, vector2f* tmpArrayT);
	

};

typedef std::vector<CModel*> CModelVector;
typedef std::list<CModel*> CModelListBase;
typedef std::list<CModel*>::iterator CModelIter;


inline bool deletedModel(CModel *p)
{
	 bool isDeleted = p->IsDeleted();
	 if (isDeleted) delete p;
	 return isDeleted;
}

class CModelList : public CModelListBase
{
  public:
  void remove_if(bool b)
  {
	  CModelListBase::remove_if(deletedModel);
  }
  
  void delete_if(bool b)
  {
	  CModelListBase::remove_if(deletedModel);
  }
  
  void Update(long t)
  {
	for (CModelIter i = begin(); i != end(); i++)
	{
	  (*i)->Update(t);
    }  
  }
  
  void Draw(CGraphics* g)
  {
	for (CModelIter i = begin(); i != end(); i++)
	{
	  (*i)->Draw(g);
    }  
  }

  virtual void clear()
  {
	for (CModelIter i = begin(); i != end(); i++)
	{
	  delete (*i);
    }  
    list::clear();
  }
	
};


