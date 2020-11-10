#include "Model.h"

#include <iostream>
#include <fstream>

// Update function
void CModel::Update(Uint32 GameTime)
{
	if (SpriteTime == 0) { SpriteTime = GameTime; return; }
	
	if (childNode!=NULL) CNodeUpdate();

	int deltaTime = GameTime - SpriteTime;
	if ( deltaTime < 0) deltaTime = 0;
	
	if (dying > 0) dying=dying-deltaTime;
	
	// linear motion update
    float dx = float(Speed*Direction.x*deltaTime/1000.0f);
	float dy = float(Speed*Direction.y*deltaTime/1000.0f);
	float dz = float(Speed*Direction.z*deltaTime/1000.0f);

    Position.x += dx; Position.y += dy; Position.z += dz;
    
    // auto movement
    if (automove)
    {
	  DistanceToTarget=DistanceToTarget-(float)sqrt(dx*dx+dy*dy+dz*dz);
	  if (DistanceToTarget <= 0) Stop();
	}
	
	// rotation update
	Rotate( Omega.x*deltaTime/1000.0f, Omega.y*deltaTime/1000.0f, Omega.z*deltaTime/1000.0f);
	
	// animation update forward
	if (period > 0)
	{
	  currentFrame=currentFrame+deltaTime*period/1000.0f;
	  if (currentFrame >= stopFrame+1) 
	  { 
		if (looping) currentFrame=(float)startFrame;
		else SetFrame( stopFrame); 
	  }
    }
    
	
    SpriteTime = GameTime;
    
    if (childNode!=NULL) 
    {
		CNodeUpdate();
		childNode->Update( GameTime); 
		
	}
		 		
}

void CModel::PlayAnimation( int start, int stop, float speed, bool loop)
{
	  if (currentFrame <= start || currentFrame >= stop+1 || period == 0)
	  {
	    startFrame=start; stopFrame=stop; period=speed; looping=loop;
	    currentFrame = (float)start; 
	  }
	
}

void CModel::CNodeUpdate()
{
	 // todo: check all parameters
	 if (childNode!=NULL)
	 {
		CVector v=childNode->localPosition;
		v=GetLocalPositionV(v);
	    childNode->SetRotationV(GetRotationV());
		childNode->SetPositionV( v);
		childNode->SetDirectionV( GetDirectionV());
		childNode->Speed=Speed;
		childNode->Omega.x=Omega.x; childNode->Omega.y=Omega.y; childNode->Omega.z=Omega.z;
		
		// animation handling
		childNode->currentFrame=currentFrame;
		childNode->startFrame=startFrame;
		childNode->stopFrame=stopFrame;
		childNode->period=period;
		childNode->looping=looping;
		
	 }
}

bool CModel::LoadModel(string obj_filename)
{
	ifstream inFile;
	string str;   // string containing a line of text
	int numV,numT,numN,numTris;
	
	Clear();
	
	numVertices=0;
	string filename = "models/"+obj_filename;
	cout << "loading... " << filename; 
	
	
	
	inFile.open(filename.c_str(), ios::in);
	
	// ---- determining array sizes ------
	if (inFile.is_open())
	{
	  while (!inFile.eof())
	  {
		  getline( inFile, str); // read a line of text into string str
		  if (str.length() < 1) continue;
		  if (str[0]=='f' && str[1]==' ') numVertices=numVertices+3; 	  
	  }
	  inFile.close();	
	  
	}
	else 
	{
		cout << "FAIL: " << filename << " can not be opened" << endl;
		return false;
	}
	
	if (numVertices >= 99999)
	{
		cout << " FAIL: The model exceeds vertex limit" << endl;
		numVertices=0;
		inFile.close();	
		return false;
	}
	
	
	
	// ---- allocating the necessary memory ------
	vertexArray = new vector3f[numVertices];
	normalArray = new vector3f[numVertices];
	texArray = new vector2f[numVertices];
	vector3f* tmpArrayV=new vector3f[numVertices];
	vector2f* tmpArrayT=new vector2f[numVertices];
	vector3f* tmpArrayN=new vector3f[numVertices];


	numVertices=0;
	numT=numN=numV=numTris=0;
	
	minx = 100000; maxx= -100000;
	miny = 100000; maxy= -100000;
	minz = 100000; maxz= -100000;
	
	
	inFile.open(filename.c_str(), ios::in);
	
	// ---- reading in vertex and texture data ------
	if (inFile.is_open())
	{
	  while (!inFile.eof())
	  {
		  getline( inFile, str); // read a line of text into string str
		  if (str.length() < 1) continue;
		  if (str[0]=='v' && str[1]==' ')
		  {
			 str[0]=' ';
			 sscanf_s(str.c_str(),"%f %f %f", &tmpArrayV[numV].x, &tmpArrayV[numV].y, &tmpArrayV[numV].z);
			 
			 minx = (tmpArrayV[numV].x<minx)?tmpArrayV[numV].x:minx;
			 miny = (tmpArrayV[numV].y<miny)?tmpArrayV[numV].y:miny;
		     minz = (tmpArrayV[numV].z<minz)?tmpArrayV[numV].z:minz;
		     maxx = (tmpArrayV[numV].x>maxx)?tmpArrayV[numV].x:maxx;
		     maxy = (tmpArrayV[numV].y>maxy)?tmpArrayV[numV].y:maxy;
		     maxz = (tmpArrayV[numV].z>maxz)?tmpArrayV[numV].z:maxz;
			 
			 numV++; 
			 //cout << str<< endl;
		  }	
		  else if (str[0]=='v' && str[1]=='t')
		  {
			 str[0]=' '; str[1]=' ';
			 sscanf_s(str.c_str(),"%f %f", &tmpArrayT[numT].x, &tmpArrayT[numT].y); 
			 numT++;
		     //cout << str<< endl;
		  }
		  else if (str[0]=='v' && str[1]=='n')
		  {
			 str[0]=' '; str[1]=' ';
			 sscanf_s(str.c_str(),"%f %f %f", &tmpArrayN[numN].x, &tmpArrayN[numN].y, &tmpArrayN[numN].z); 
			 // normalize vector
			 normalizeVector3f(  &tmpArrayN[numN]);
			 numN++; 
			 //cout << str<< endl;
		  }	
		  else if (str[0]=='f' && str[1]==' ')
		  {
			  
			  //cout << str<< endl;
			  if (!readFace( str, tmpArrayV, tmpArrayN, tmpArrayT))
			  {
				 cout << " FAIL: This model is not triangulated. To triangulate, load it into Misfit 3D and save it as an obj file." << endl;
				 inFile.close();
				 // todo: clear up
	               delete [] tmpArrayV;
				   delete [] tmpArrayN;
	               delete [] tmpArrayT;
	               numVertices=0;
				 return false;	  
			  }
			  numTris++;
		  }
	  } 
	  cout << "..ok";
	  cout << " Tris:" << numTris << " W:" << int(maxx-minx) << " H:" << int(maxy-miny) << " D:" << int(maxz-minz)  << endl;
	  cout << "Vert: " << numV << " Tex: " << numT << " Norm: " << numN << endl;
	  inFile.close();	
	}
	
	// set y position
	Position.y=(maxy-miny)/2.0f;
	
	// calculate normals from vertex array
	if (numN == 0) CalculateNormals();
	
	Color.Set(CColor::LightGray());

	delete [] tmpArrayV;
	delete [] tmpArrayN;
	delete [] tmpArrayT;
	
	return true;
}


void CModel::LoadTexture(string bmp_filename)
{
	string filename = "models/" + bmp_filename;
	Texture.LoadTexture(filename); 
	
	Color.Set( 1.0, 1.0, 1.0, 1.0);
}

void CModel::CalculateNormals()
{
   vector3f a, b, c;
   float length;
   
   
   for (int n=0; n < numVertices; n=n+3)
   {
	   
     a.x = vertexArray[n].x - vertexArray[n+1].x;
     a.y = vertexArray[n].y - vertexArray[n+1].y;
     a.z = vertexArray[n].z - vertexArray[n+1].z;

     b.x = vertexArray[n].x - vertexArray[n+2].x;
     b.y = vertexArray[n].y - vertexArray[n+2].y;
     b.z = vertexArray[n].z - vertexArray[n+2].z;

     c.x = a.y * b.z - b.y * a.z;
     c.y = b.x * a.z - a.x * b.z;
     c.z = a.x * b.y - b.x * a.y;

     // calculate the length of the normal
     length = (float)sqrt(c.x*c.x + c.y*c.y + c.z*c.z);
     
     normalArray[n].x=c.x/length;  normalArray[n].y=c.y/length;   normalArray[n].z=c.z/length; 
     normalArray[n+1].x=c.x/length;  normalArray[n+1].y=c.y/length;   normalArray[n+1].z=c.z/length; 
     normalArray[n+2].x=c.x/length;  normalArray[n+2].y=c.y/length;   normalArray[n+2].z=c.z/length; 
     
   }
}


void CModel::Clear()
{
	  if (!isCloned)
	  {
	    if (vertexArray) delete [] vertexArray;
	    if (normalArray) delete [] normalArray;
	    if (texArray) delete [] texArray;
	  }	
	  
	  texArray=NULL; vertexArray=NULL; texArray=NULL;
	  numVertices=0;
	  
}


bool CModel::readFace(string& str, vector3f* tmpArrayV, vector3f* tmpArrayN, vector2f* tmpArrayT)
{
	int num[5];
	int values[9]; // up to 9 values to be read in
	int numNum=0;
	unsigned int n;
	int numVertex=0;
	int value=0;
	
	// clear values
	memset(values,0,9*sizeof(int));
	
	
	for (n=1; n < str.length(); n++)
	{
		
		if (str[n]==' ' && numVertex==0) { numVertex++; continue; } // start first vertex
		
		if (str[n] >=48 && str[n] <= 57)  // read numbers as characters
		{
		  num[numNum++]=str[n]-48; 
		  if  (n < str.length()-1) continue;
		}
		
		if (str[n] == '/' || str[n]==' ' || n == str.length()-1) 
		{
		  // todo: optimise string to int conversion
		  if (numNum==1) { value=num[0];  }
		  else if (numNum==2) { value=num[1]+10*num[0];}
		  else if (numNum==3) { value=num[2]+10*num[1]+100*num[0];}
		  else if (numNum==4) { value=num[3]+10*num[2]+100*num[1]+1000*num[0];}
		  else if (numNum==5) { value=num[4]+10*num[3]+100*num[2]+1000*num[1]+10000*num[0];}
		  else value=0; // no value
		  values[numVertex-1]=value; // assign value
		  numNum=0; numVertex++; 
		 
		}
	}
	
	// this model is not triangulated
	if (numVertex > 10) return false;
	
	if (numVertex > 7)  // v/t/n 
	{
     memcpy( &vertexArray[ numVertices], &tmpArrayV[values[0]-1],sizeof( vector3f));
	 if (values[2]>0) memcpy( &normalArray[ numVertices], &tmpArrayN[values[2]-1],sizeof( vector3f));
	 if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[1]-1],sizeof( vector2f));	
	 numVertices++;
	
	 memcpy( &vertexArray[ numVertices], &tmpArrayV[values[3]-1],sizeof( vector3f));
	 if (values[2]>0) memcpy( &normalArray[ numVertices], &tmpArrayN[values[5]-1],sizeof( vector3f));
	 if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[4]-1],sizeof( vector2f)); 	
     numVertices++;
    
     memcpy( &vertexArray[ numVertices], &tmpArrayV[values[6]-1],sizeof( vector3f));
     if (values[2]>0) memcpy( &normalArray[ numVertices], &tmpArrayN[values[8]-1],sizeof( vector3f));
     if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[7]-1],sizeof( vector2f));
	 numVertices++;
    }
    else  // v/t
    {
	 memcpy( &vertexArray[ numVertices], &tmpArrayV[values[0]-1],sizeof( vector3f));
	 if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[1]-1],sizeof( vector2f));	
	 numVertices++;
	
	 memcpy( &vertexArray[ numVertices], &tmpArrayV[values[2]-1],sizeof( vector3f));
	 if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[3]-1],sizeof( vector2f)); 	
     numVertices++;
    
     memcpy( &vertexArray[ numVertices], &tmpArrayV[values[4]-1],sizeof( vector3f));
     if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[5]-1],sizeof( vector2f));
	 numVertices++;
	}
	return true;
}


void CModel::Draw(CGraphics* g)
{
	// geometry data in vertex array?
	if (numVertices > 0 && vertexArray != NULL)
	{
	  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	  glColor3f( Color.R, Color.G, Color.B);
	  // glColor3f(1,1,1);
	  // binding texture to handle
	 
      glBindTexture( GL_TEXTURE_2D, Texture.GetTextureID());
      
      glPushMatrix();
	   glTranslatef( Position.x, Position.y, Position.z);  	// transformation to world coordinates
	   glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
	   glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
	   glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
	   //glTranslatef(localPosition.x, localPosition.y, localPosition.z);
       glScalef( Scale, Scale, Scale);
		
		// draw vertex array data
		glEnableClientState(GL_VERTEX_ARRAY);	 
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 		
		glTexCoordPointer(2, GL_FLOAT, 0, texArray);			
		glVertexPointer(3,GL_FLOAT,	0, vertexArray);				
		glNormalPointer(GL_FLOAT, 0, normalArray);
	
		glDrawArrays(GL_TRIANGLES, 0, numVertices);		
		glDisableClientState(GL_VERTEX_ARRAY);						
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	  
	  glPopMatrix();
	  
	  glBindTexture( GL_TEXTURE_2D, 0);
	  	
	}
	else // just draw a box
	{
	  glLineWidth(2.0);
	  glColor4f( Color.R, Color.G, Color.B, Color.A);
	  if (!filled) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	  else glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	  DrawBox(false);
	  
	  if (border) 
	  { 
		  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		  glColor4f( Color2.R, Color2.G, Color2.B, Color2.A);
		  DrawBox(false);
	  }
    }
	
	if (g->IsDebug()) // if debug draw red bounding box
	{
		 glLineWidth(2.0);
		 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		 glColor3f( 1.0, 0, 0);
		 DrawBox(true);
	}
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	
	
	if (childNode!=NULL && visible) 
	{
		childNode->Draw( g);
	}
		
}

void CModel::DrawBox(bool bbox)
{
    	glPushMatrix();
		  glTranslatef( Position.x, Position.y, Position.z);  	// transformation to world coordinates
		  glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
		  glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
		  glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
		  //glTranslatef(localPosition.x, localPosition.y, localPosition.z);
          
          if (bbox) glScalef( ScaleBB*Scale, ScaleBB*Scale, ScaleBB*Scale);
          else glScalef( Scale, Scale, Scale);
          
		  glBegin(GL_QUADS);
		  
		   // front
		   glNormal3f(0,0,1);
		   glVertex3f(minx, miny,  maxz);
		   glVertex3f(maxx, miny,  maxz);
		   glVertex3f(maxx, maxy,  maxz);
		   glVertex3f(minx, maxy,  maxz);
		   // back
		   glNormal3f(0,0,-1);
		   glVertex3f(minx, miny,  minz);
		   glVertex3f(maxx, miny,  minz);
		   glVertex3f(maxx, maxy,  minz);
		   glVertex3f(minx, maxy,  minz);
		   // top
		   glNormal3f(0,1,0);
		   glVertex3f(minx, maxy,  maxz);
		   glVertex3f(maxx, maxy,  maxz);
		   glVertex3f(maxx, maxy,  minz);
		   glVertex3f(minx, maxy,  minz);
		   // bottom
		   glNormal3f(0,-1,0);
		   glVertex3f(minx, miny,  maxz);
		   glVertex3f(maxx, miny,  maxz);
		   glVertex3f(maxx, miny,  minz);
		   glVertex3f(minx, miny,  minz);
		   // left
		   glNormal3f(-1,0,0);
		   glVertex3f(minx, maxy,  maxz);
		   glVertex3f(minx, miny,  maxz);
		   glVertex3f(minx, miny,  minz);
		   glVertex3f(minx, maxy,  minz);
		   // right
		    glNormal3f(1,0,0);
		   glVertex3f(maxx, maxy,  maxz);
		   glVertex3f(maxx, miny,  maxz);
		   glVertex3f(maxx, miny,  minz);
		   glVertex3f(maxx, maxy,  minz);
		   
		  glEnd();
		glPopMatrix();	
}


// move distance in xz plane in direction of rotation angle
void CModel::MoveXZRotation( float distance) 
{  
  Position.x += float(distance*cos( DEG2RAD (Rotation.y))); Position.z -= float(distance*sin( DEG2RAD (Rotation.y))); 
}

// move in xyz plane according to specified direction
void CModel::Move( float distance) 
{  
	Position.x += distance*Direction.x;
	Position.y += distance*Direction.y;
	Position.z += distance*Direction.z; 
}

void CModel::MoveTo(float x, float z, float speed) 
{
	DistanceToTarget=CVector::Distance( CVector(x,Position.y, z), Position); 
	SetDirectionAndRotationToPoint(x,z);
	SetSpeed( speed);
	automove=true;
}

void CModel::SetToFloor( float y) 
{
 //Position.y = y+1+Scale*abs(int(miny));
 Position.y = y+1+(Scale*(maxy-miny))/2.0f;
 
} 

// facing towards the vector specified through dx,dy,dz
void CModel::SetRotationV(float dX, float dY, float dZ)
{ 
	float angle_y = (float)RAD2DEG (atan2(-dZ, dX));
	Rotation.y = angle_y;
	float hyp_y=hypot(-dZ,dX);
	
	float angle_z=(float)RAD2DEG (atan2(dY, hyp_y));
	
	Rotation.z=angle_z;
}

CVector CModel::GetRotationV()
{
	float x=1.0f,y=0,z=0;  // default along x-axis
	CVector v; 
	
	GLfloat view[16];
	
	glPushMatrix();
	 glLoadIdentity();
	 glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
	 glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
	 glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
	 glGetFloatv(GL_MODELVIEW_MATRIX, view);
	 
    glPopMatrix();
    
    v.x=x*view[0]+y*view[4]+z*view[8]+view[12];
    v.y=x*view[1]+y*view[5]+z*view[9]+view[13];
    v.z=x*view[2]+y*view[6]+z*view[10]+view[14];
	
	return v;
}




void CModel::DrawVertex( float x, float y, float z)
{
	glColor3f(1.0,1.0,0);
	glPointSize( 7); 
	
	glBegin(GL_POINTS);
	 glVertex3f( x, y, z);
	glEnd();
		
}
// attached model shares the same origin at (0,0,0)
void CModel::AttachModel(CModel* model) { childNode = model; }

void CModel::AttachModel(CModel* model, CVector localPosition)
{
	childNode = model; model->localPosition=localPosition;
	
}

CVector CModel::GetAttachedModelPositionV()
{
	CVector v;
	
	if (childNode!=NULL)
	{
		v=GetLocalPositionV(childNode->localPosition);
	}
	
	return v;
}

CModel* CModel::Clone()
{
	// create new model
	CModel* m = new CModel();
	m->isCloned=true;
	
	m->minx=minx; m->maxx=maxx; m->miny=miny; m->maxy=maxy; m->minz=minz; m->maxz=maxz;
	m->localPosition=localPosition;
	
	m->Scale=Scale; 
	m->ScaleBB=ScaleBB;
	m->Color.Set( Color);
	m->Color2.Set(Color2);
	m->filled=filled;
	m->border=border;
	
	// assign texture ID
	m->Texture.SetTextureID( Texture.GetTextureID());
	
	if (childNode != NULL) m->childNode=childNode->Clone();
	
	// copy vertex data pointers
	if (numVertices > 0)
	{
		m->numVertices=numVertices;
		m->vertexArray = vertexArray;
	    m->normalArray = normalArray;
	    m->texArray = texArray;
	}
	
	return m;
}

float CModel::GetRadius()
{
	float xmax, ymax, zmax;
	
	if (abs(minx) > maxx) xmax=abs(minx);
	else xmax=maxx;
	
	if (abs(miny) > maxy) ymax=abs(miny);
	else ymax=maxy;
	
	if (abs(minz) > maxz) zmax=abs(minz);
	else zmax=maxz;
	
	return Scale*(float)sqrt( xmax*xmax+ymax*ymax+zmax*zmax);
}

CVector CModel::GetLocalPositionV(CVector pos)
{
	CVector p;
	float x,y,z;
	
	GLfloat view[16];
	
	x=pos.x; y=pos.y;  z=pos.z; 
	
	glPushMatrix();
	 glLoadIdentity();
	 glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
	 glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
	 glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
	 glGetFloatv(GL_MODELVIEW_MATRIX, view);
	 
    glPopMatrix();
    
    p.x=x*view[0]+y*view[4]+z*view[8]+view[12];
    p.y=x*view[1]+y*view[5]+z*view[9]+view[13];
    p.z=x*view[2]+y*view[6]+z*view[10]+view[14];
    

	pos.x=p.x+Position.x; pos.y=p.y+Position.y; pos.z=p.z+Position.z; 
	
	return pos;
	
}

bool CModel::HitTest(CVector pos, float distance)
{
	float dist;
	
	dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
	
	if (dist < distance) return true;
	
	return false;
}

bool CModel::HitTest(CVector pos)
{
	float xp,yp,zp;
	float x,y,z;
	
	GLfloat view[16];
	
	x=xp=pos.x-Position.x; y=yp=pos.y-Position.y;  z=zp=pos.z-Position.z; 
  
    glPushMatrix();
    
	 glLoadIdentity();
	 glRotatef( -Rotation.z, 0, 0, 1 );       // rotation around z-axis
	 glRotatef( -Rotation.y, 0, 1, 0 );       // rotation around y-axis
	 glRotatef( -Rotation.x, 1, 0, 0 );       // rotation around x-axis
	 glGetFloatv(GL_MODELVIEW_MATRIX, view); 
	 
    glPopMatrix();
    
    xp=x*view[0]+y*view[4]+z*view[8]+view[12];
    yp=x*view[1]+y*view[5]+z*view[9]+view[13];
    zp=x*view[2]+y*view[6]+z*view[10]+view[14];
  
  
    if (xp >= Scale*ScaleBB*minx && xp <= Scale*ScaleBB*maxx && yp >= Scale*ScaleBB*miny && yp <= Scale*ScaleBB*maxy && zp >= Scale*ScaleBB*minz && zp <= Scale*ScaleBB*maxz) 
    {
		return true;
    }
	return false;
}

void CModel::DoRotation( CVector& pos, float yoffset)
{
	float xp,yp,zp;
	float x,y,z;
	
	GLfloat view[16];
	
	x=xp=pos.x-Position.x; y=yp=pos.y-Position.y;  z=zp=pos.z-Position.z; 
	
	glPushMatrix();
	 glLoadIdentity();
	 glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
	 glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
	 glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
	 glGetFloatv(GL_MODELVIEW_MATRIX, view);
	 
    glPopMatrix();
    
    xp=x*view[0]+y*view[4]+z*view[8]+view[12];
    yp=x*view[1]+y*view[5]+z*view[9]+view[13];
    zp=x*view[2]+y*view[6]+z*view[10]+view[14];
	
	pos.x=float(xp+Position.x); pos.y=float(yp+Position.y);  pos.z=float(zp+Position.z); 
	
	// ---------------------------------------------------------------------------------
	//DrawVertex(pos.x, pos.y, pos.z);
}

bool CModel::HitTest(CModel *pModel)
{
   CVector pos;
   
   pos = pModel->GetPositionV();
   
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
  
  
   // --- inside ? -----
   if (pModel->HitTest( Position)) return true;
   
   // --- inside ? -----
   if (HitTest( pModel->GetPositionV())) return true;
  
   // ----  test 8 vertex positions from this model----  
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos); if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*minz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*minz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*minz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   
   
   // ----  test 8 vertex positions from pModel----  
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   
   return false;
}

bool CModel::HitTestFront(CModel *pModel)
{
   CVector pos;
     
   pos = pModel->GetPositionV();
    
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
   
   // ----  test front 9 vertex positions from this model---- 
   
   // --- front middle  
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   // --- front right
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos); 
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;

   //---- front left
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   return false;
	
}

bool CModel::HitTestBack(CModel *pModel)
{
   CVector pos;
     
   pos = pModel->GetPositionV();
    
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
   
   // ----  test back 9 vertex positions from this model---- 
   
   // --- back middle  
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   // --- back right
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos); 
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;

   //---- back left
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   return false;
	
	
}

bool CModel::HitTestLeft(CModel *pModel)
{
   CVector pos;
     
   pos = pModel->GetPositionV();
    
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
   
   // ----  test left 9 vertex positions from this model---- 
   
   // --- left middle  
   pos.x=Position.x; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   // --- left front
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos); 
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;

   //---- left back
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   return false;
	
}

bool CModel::HitTestRight(CModel *pModel)
{
   CVector pos;
     
   pos = pModel->GetPositionV();
    
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
   
   // ----  test left 9 vertex positions from this model---- 
   
   // --- right middle  
   pos.x=Position.x; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   // --- right front
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos); 
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;

   //---- right back
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   return false;
	
}









