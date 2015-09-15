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
#include "camera.h"
#include "light.h"
#include "material.h"
#include "object.h"

class Scene
{
	Camera camera;
	std::vector<LightSource*> lightSources;
	std::vector<Object*> objects; //instead of 'intersectables'
	std::vector<Material*> materials;
	std::vector<MeshInstance*> meshes;
public:
	Scene() {}
	void initialize() {
		lightSources.push_back(
			new DirectionalLight(
				float3(-0.5, 0, -0.5), 
				float3(0.2, 0.2, 0.2)));
		lightSources.push_back(
			new DirectionalLight(
				float3(0.5, 0, 0.5), 
				float3(0.1, 0.1, 0.1)));
		lightSources.push_back(
			new DirectionalLight(
				float3(0, 1, 0), 
				float3(0.5, 0.5, 0.5)));

		Material* greenDiffuseMaterial = new Material();
		greenDiffuseMaterial->kd = float3(0, 1, 0);
		materials.push_back(greenDiffuseMaterial);

		TexturedMaterial * f = new TexturedMaterial("star.jpg", GL_LINEAR);
		f->kd = float3(1,1,1);
		materials.push_back(f);
		
		Avatar * arm = new Avatar(greenDiffuseMaterial,new Mesh("androidarm2.obj"), true);
		Ground* g = new Ground(f);
		objects.push_back(g);
		Avatar * m = new Avatar(greenDiffuseMaterial,new Mesh("androidbody.obj"), false);
		objects.push_back(m);
		objects.push_back(arm);
		
	}
	~Scene()
	{
		for (std::vector<LightSource*>::iterator iLightSource = lightSources.begin(); iLightSource != lightSources.end(); ++iLightSource)
			delete *iLightSource;
		for (std::vector<Material*>::iterator iMaterial = materials.begin(); iMaterial != materials.end(); ++iMaterial)
			delete *iMaterial;
		for (std::vector<Object*>::iterator iObject = objects.begin(); iObject != objects.end(); ++iObject)
			delete *iObject;
	}
	void move(double t, double dt, bool HELICAM) 
	{
		for (unsigned int iObject=0; iObject<objects.size(); iObject++)
			objects.at(iObject)->move(t, dt, &camera, HELICAM);
	}
	Camera& getCamera()
	{
		return camera;
	}
	void control(std::vector<bool>& keysPressed)
	{
	  std::vector<Object*> spawn;
	  for (unsigned int iObject=0;
	                 iObject<objects.size(); iObject++)
	      objects.at(iObject)->control(
	             keysPressed, objects);
	}

	void draw()
	{
		camera.apply();
		unsigned int iLightSource=0;
		for (; iLightSource<lightSources.size(); iLightSource++)
		{
			glEnable(GL_LIGHT0 + iLightSource);
			lightSources.at(iLightSource)->apply(GL_LIGHT0 + iLightSource);
		}
		for (; iLightSource<GL_MAX_LIGHTS; iLightSource++)
			glDisable(GL_LIGHT0 + iLightSource);

		for (unsigned int iObject=0; iObject<objects.size(); iObject++)
			objects.at(iObject)->draw();
		for (unsigned int iMesh=0; iMesh<meshes.size(); iMesh++)
			meshes.at(iMesh)->draw();		
	}
};