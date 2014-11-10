#include "Scene.h"
#include "GLUT/glut.h"

Scene::Scene(void)
{
   Init();
}

//some default call: -testcase hanging -method Euler -stiff 10 -mass 0.1 -step 0.003 -damp 0.01

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
}


void Scene::Update(void)
{
   if(pause)
   {
      return;
   }
	
	x += 0.1;
	if (x > 2.0)
		x = -2.0;
}

void Scene::Render(void)
{
	glColor3d(1,0,0);
	glPushMatrix();
        glTranslated(x, 0, -10);
        glutSolidSphere(1,36,36);
    glPopMatrix(); 
}
