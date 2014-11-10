#include "Scene.h"
#include "utilities.h"
#include "GLUT/glut.h"

Scene::Scene(void)
{
   Init();
}

Scene::Scene(int argc, char* argv[])
{
   Init();
}

Scene::~Scene(void)
{
}


void Scene::Init(void)
{
   //Animation settings
   pause=false;
   particle.position = Vector3d(0,0,-10);
}


void Scene::Update(void)
{
   if(pause)
   {
      return;
   }
	
	particle.position.x() += 0.01;
	if (particle.position.x() > 2.0)
		particle.position.x() = -2.0;
}

void Scene::Render(void)
{
	particle.draw();
}
