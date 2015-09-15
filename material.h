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
extern "C" unsigned char* stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);
class Material
{
public:
	float3 kd;			// diffuse reflection coefficient
	float3 ks;			// specular reflection coefficient
	float shininess;	// specular exponent
	Material()
	{
		kd = float3(0.5, 0.5, 0.5) + kd * 0.5;
		ks = float3(1, 1, 1);
		shininess = 15;
	}
	virtual void apply()
	{
		//openGL can differentiate between front face and back face
		//here set both front and back as the material
		float aglDiffuse[] = {kd.x, kd.y, kd.z, 1.0f};	
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, aglDiffuse);
		float aglSpecular[] = {kd.x, kd.y, kd.z, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, aglSpecular);
		if(shininess <= 128)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		else
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
		//can add things here for texturing
	}
};
class TexturedMaterial : public Material
{
protected:
	GLuint textureName;
	GLint filtering;
public:
	TexturedMaterial(const char* filename, 
	GLint filtering = GL_LINEAR)
	{
		unsigned char* data;
		int width;
		int height;
		int nComponents = 4;
		

		data = stbi_load(filename, &width, &height, &nComponents, 0);
		if(data == NULL) return;
		// opengl texture creation comes here
		glGenTextures(1, &textureName);  // id generation
		glBindTexture(GL_TEXTURE_2D, textureName);      // binding
		if(nComponents == 4)

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //uploading

		else if(nComponents == 3)

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //uploading

		delete data;
	}
	virtual void apply() {
		Material::apply();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
	}

};