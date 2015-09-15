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
#include <math.h>
#include <map>
// Object abstract base class.
#define SKY_SIZE 100
class Object
{
protected:
	Material* material; 
	float3 scaleFactor;
	float3 position;
	float3 xAxis;
	float3 yAxis;
	float xAngle;
	float yAngle;
public:
	Object(Material* material):material(material),position(0, 0, 0),xAngle(0.0f),yAngle(0.0f),scaleFactor(1.0,1.0,1.0),xAxis(-1.0,0.0,0.0),yAxis(0,1,0){}
    virtual ~Object(){}
    Object* translate(float3 offset){
        position += offset; return this;
    }
    Object* scale(float3 factor){
        scaleFactor *= factor; return this;
    }
    Object* rotateX(float angle){
        xAngle += angle; return this;
    }
    Object* rotateY(float angle){
        yAngle += angle; return this;
    }
    virtual void draw()
    {
		material->apply();
        // apply scaling, translation and orientation
        //transformations happen in reverse order
		glMatrixMode(GL_MODELVIEW); //view transformation
		glPushMatrix();	//save matrix - model transformation
			glTranslatef(position.x, position.y, position.z);	//transform matrix - happens in reverse order
			glRotatef(yAngle, yAxis.x, yAxis.y, yAxis.z);
			glRotatef(xAngle, xAxis.x, xAxis.y, xAxis.z);
			glScalef(scaleFactor.x, scaleFactor.y, scaleFactor.z);
			drawModel();
		glPopMatrix();
		//GL Model-View matrix 
		//Projection Matrix
		glDisable(GL_TEXTURE_2D);
    }
    virtual void drawModel()=0;
    virtual void move(double t, double dt, Camera* c, bool HELICAM){}
    virtual bool control(std::vector<bool>& keysPressed, std::vector<Object*>& objects){return false;}
};

class MeshInstance : public Object
{
protected:
	Mesh* mesh;
public:
	MeshInstance(Material* material, Mesh* mesh):Object(material),mesh(mesh){}
	void drawModel() {
		mesh->draw();
	}
};
class Avatar;
class Beam : public MeshInstance {
protected:
	float3 velocity;
public:
	Beam(Material* material, Mesh* mesh, float3 velocity):MeshInstance(material,mesh),velocity(velocity) {
	}
	void move(double t, double dt, Camera* c, bool HELICAM) {
		position += velocity * dt;
	}
	void draw()
	{
		material->apply();
		glMatrixMode(GL_MODELVIEW); //view transformation
			glPushMatrix();	//save matrix - model transformation
				glTranslatef(position.x, position.y, position.z);	//transform matrix - happens in reverse order
				glRotatef(yAngle, yAxis.x, yAxis.y, yAxis.z);
				glTranslatef(-2.3,3.7,0);
				glRotatef(xAngle, xAxis.x, xAxis.y, xAxis.z);
				glScalef(scaleFactor.x, scaleFactor.y, scaleFactor.z);
				drawModel();
			glPopMatrix();
		glDisable(GL_TEXTURE_2D);

	}
	void setup(Avatar *arm);
};

class Avatar : public MeshInstance
{
protected:
	float3 velocity = float3(0,0,0);
	float3 acceleration = float3(0,0,0);
	float xAngVelocity = 0;
	float xAngAcceleration = 0;
	float yAngVelocity = 0;
	float yAngAcceleration = 0;
	bool arm;
	Material * b;
	friend class Beam;
public:
	Avatar(Material* material, Mesh* mesh, bool arm):MeshInstance(material,mesh),arm(arm){
		b = new Material;
		b->kd = float3(0.9,0.3,1);
	}
	void move(double t, double dt, Camera* c, bool HELICAM) {
		if(HELICAM && !(arm)) {
			c->eye = position - float3(sin(yAngle * M_PI/180),-1,cos(yAngle * M_PI/180)) * 8;
			c->ahead += float3(sin(yAngle * M_PI/180),-0.5,cos(yAngle * M_PI/180));
			c->right += float3(sin(yAngle * M_PI/180),-0.5,cos(yAngle * M_PI/180));
			c->up += float3(sin(yAngle * M_PI/180),-0.5,cos(yAngle * M_PI/180));
		}
		velocity += acceleration * dt;
		velocity *= pow(0.5,dt);
		position += velocity * dt;
		xAngVelocity += xAngAcceleration * dt;
		xAngVelocity *= pow(0.2,dt);
		xAngle += xAngVelocity * dt;
		if (xAngle > 360) {
			xAngle -= 360;
		}
		if (xAngle < 0) {
			xAngle += 360;
		}
		yAngVelocity += yAngAcceleration * dt;
		yAngVelocity *= pow(0.2,dt);
		yAngle += yAngVelocity * dt;
		if (yAngle > 360) {
			yAngle -= 360;
		}
		if (yAngle < 0) {
			yAngle += 360;
		}
	}
	void draw()
    {
		material->apply();
    	glMatrixMode(GL_MODELVIEW); //view transformation
		glPushMatrix();	//save matrix - model transformation
			glTranslatef(position.x, position.y, position.z);	//transform matrix - happens in reverse order
			glRotatef(yAngle, yAxis.x, yAxis.y, yAxis.z);
			if (arm) {
				glTranslatef(-2.3,3.7,0);
			}
			glRotatef(xAngle, xAxis.x, xAxis.y, xAxis.z);
			glScalef(scaleFactor.x, scaleFactor.y, scaleFactor.z);
			drawModel();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
    }
	bool control(std::vector<bool>& keysPressed, std::vector<Object*>& objects)
	{
		acceleration = float3(0,0,0);
		if(keysPressed.at('p')) {
			acceleration = float3(sin(yAngle * M_PI/180),0,cos(yAngle * M_PI/180)) * 8;
		}
		if(keysPressed.at(';')) {
			acceleration = float3(sin(yAngle * M_PI/180),0,cos(yAngle * M_PI/180)) * -8;
		}
		if(keysPressed.at('l')) {
			yAngVelocity = 80;
		}
		if(keysPressed.at('\'')) {
			yAngVelocity = -80;
		}
		if (arm) 
		{
			int a = -1;
			if (xAngle < 180) {
				a = 1;
			}
			xAngAcceleration = 0;
			if(keysPressed.at('r')) {
				xAngVelocity = 70;
			}
			if(keysPressed.at('f')) {
				xAngVelocity = -70;
			}
			if(keysPressed.at(' ')) {
				Beam * beam = new Beam(b, new Mesh("beam2.obj"),
					float3(sin(xAngle * M_PI/180)*sin(yAngle * M_PI/180),-cos(xAngle * M_PI/180),(cos(yAngle * M_PI/180)*sin(xAngle * M_PI/180)))*10);
				// printf("xANGLE:%f, yAngle%f\n", xAngle, yAngle);
				// printf("x:%f,y:%f,z%f\n", position.x,position.y,position.z);
				beam->setup(this);
				objects.push_back(beam);
			}
		}
		return true;
	}
};

void Beam::setup(Avatar *arm) {
	position = arm->position;
	xAngle = arm->xAngle;
	yAngle = arm->yAngle;
}

class Ground : public Object
{
public:
	Ground(Material* material):Object(material){}
	void drawModel()
	{
		material->apply();
		glBegin(GL_QUADS);
		glNormal3d(0,1,0);
		glVertex4d(1,0,0,0.000001);	glTexCoord2f(0,10000);
		glVertex4d(0,0,1,0.000001);	glTexCoord2f(10000,10000);
		glVertex4d(-1,0,0,0.000001);	glTexCoord2f(10000,0);
		glVertex4d(0,0,-1,0.000001);	glTexCoord2f(0,0);
		
		glEnd();
		glDisable(GL_TEXTURE_2D);

	}
};
