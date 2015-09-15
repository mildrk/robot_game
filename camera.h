#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// Needed on MsWindows
#include <windows.h>
#endif // Win32 platform
 
#include <GLUT/glut.h>

#include "float2.h"
#include "float3.h"
#include "Mesh.h"
#include <vector>
#include <map>

// Skeletal Camera class. Feel free to add custom initialization, set aspect ratio to fit viewport dimensions, or animation.
class Camera
{
	float3 eye;
	float2 lastMousePos;
	float2 mouseDelta;

	float3 ahead;
	float3 right;	
	float3 up;

	//instead of right and up vectors to set up the camera
	float fov; //field of view angle
	float aspect;	//aspect ratio
	friend class Avatar;
public:
	float3 getEye()
	{
		return eye;
	}
	Camera()
	{
		eye = float3(0, 0, 0);
		ahead = float3(0, 0, 1);
		right = float3(1, 0, 0);
		up = float3(0, 1, 0);
		fov = 1.5;
		aspect = 2;
	}
	void setAspectRatio(float ar)  { 
	  aspect = ar;
	}

	void startDrag(int x, int y) {
        lastMousePos = float2(x, y);
    }
    void drag(int x, int y){
        float2 mousePos(x, y);
        mouseDelta = mousePos - lastMousePos;
        lastMousePos = mousePos;
    }
    void endDrag(){
        mouseDelta = float2(0, 0);
    }
    void move(float dt, std::vector<bool>& keysPressed) {
	  if(keysPressed.at('w'))
	    eye += ahead * dt * 20;
	  if(keysPressed.at('s'))
	    eye -= ahead * dt * 20;
	  if(keysPressed.at('a'))
	    eye -= right * dt * 20;
	  if(keysPressed.at('d'))
	    eye += right * dt * 20;
	  if(keysPressed.at('q'))
	    eye -= float3(0,1,0) * dt * 20;
	  if(keysPressed.at('e'))
	    eye += float3(0,1,0) * dt * 20;
	  //to be continued here
		float yaw = atan2f( ahead.x, ahead.z );
		float pitch = -atan2f( ahead.y, 
		sqrtf(ahead.x * ahead.x + ahead.z * ahead.z) );

		yaw -= mouseDelta.x * 0.02f;
		pitch += mouseDelta.y * 0.02f;
		if(pitch > 3.14/2) pitch = 3.14/2;
		if(pitch < -3.14/2) pitch = -3.14/2;

		mouseDelta = float2(0, 0);

		ahead = float3(sin(yaw)*cos(pitch), -sin(pitch),
		                             cos(yaw)*cos(pitch) );
		right = ahead.cross(float3(0, 1, 0)).normalize();
		up = right.cross(ahead);
		float3 lookAt = eye + ahead;
	}

	void apply()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov / 3.14 * 180, aspect, 0.1, 200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x, eye.y, eye.z, eye.x+ahead.x, eye.y+ahead.y, eye.z+ahead.z, 0.0, 1.0, 0.0);
	}
};
