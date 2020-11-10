#pragma once
#include "Model.h"


typedef struct
{
   float x;
   float y;
   float z;
   float w;
} vector4f;


class CLight
{
	private:

        vector4f Ambient, Diffuse, Direction;
        int Light;

	public:

	CLight()
	{
       Light=0;
	   Ambient.x=Ambient.y=Ambient.z=0.7f; Ambient.w=1.0f;
       Diffuse.x=Diffuse.y=Diffuse.z=1.0f; Diffuse.w=1.0f;
	   Direction.x=2.0f; Direction.y=5.0f; Direction.z=1.0f; Direction.w=0;	
	}

        void SetAmbientLight( float r, float g, float b) {  Ambient.x=r; Ambient.y=g; Ambient.z=b; Ambient.w=1.0f; }

        void SetDiffuseLight( float r, float g, float b) {  Diffuse.x=r; Diffuse.y=g; Diffuse.z=b; Diffuse.w=1.0f; }

        void SetLightDirection( float x, float y, float z) { Direction.x=x; Direction.y=y; Direction.z=z; Direction.w=0; }

        void SelectLight( int light=0)
		{
            if (light > 2 || light < 0) { cout << "Three lights max" << endl; return; }
            Light=light;
		}

        void Enable()
		{
            glEnable(GL_LIGHTING);
            
            // normalize normal vectors
			glEnable(GL_NORMALIZE);
    
			// enable color tracking
			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

			// smooth shading
            glShadeModel(GL_SMOOTH); 

			// Apply the lights
            Apply();

		}
		
		// apply the light
		void Apply()
		{
	     if (Light==0)
	    {
 	      glEnable(GL_LIGHT0); 
             
          glLightfv(GL_LIGHT0, GL_AMBIENT, (float*)&Ambient);  // add lighting. (ambient)
	      glLightfv(GL_LIGHT0, GL_DIFFUSE, (float*)&Diffuse);  // add lighting. (diffuse).
	      glLightfv(GL_LIGHT0, GL_POSITION,(float*)&Direction); // set light position.
            }
 	    else if (Light==1)
	    {
 	      glEnable(GL_LIGHT1); 
          glLightfv(GL_LIGHT1, GL_AMBIENT, (float*)&Ambient);  // add lighting. (ambient)
	      glLightfv(GL_LIGHT1, GL_DIFFUSE, (float*)&Diffuse);  // add lighting. (diffuse).
	      glLightfv(GL_LIGHT1, GL_POSITION,(float*)&Direction); // set light position.
            }
	    else if (Light==2)
	    {
 	      glEnable(GL_LIGHT2); 
          glLightfv(GL_LIGHT2, GL_AMBIENT, (float*)&Ambient);  // add lighting. (ambient)
	      glLightfv(GL_LIGHT2, GL_DIFFUSE, (float*)&Diffuse);  // add lighting. (diffuse).
	      glLightfv(GL_LIGHT2, GL_POSITION,(float*)&Direction); // set light position.
            }
			
			
		}

        void Disable() 
        {
            if (Light==0) glDisable(GL_LIGHT0); 
            if (Light==1) glDisable(GL_LIGHT1); 
			if (Light==2) glDisable(GL_LIGHT2); 

	} 



};
