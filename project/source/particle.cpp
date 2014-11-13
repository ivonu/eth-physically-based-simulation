#include "particle.h"
#include "GLUT/glut.h"
#include "Scene.h"

Particle::Particle(Vector3d position) {
	this->position = position;
	this->mass = Scene::volume * Scene::rho0;
	r = 1.0;
	g = 0.0;
	b = 0.0;
}

void Particle::draw() {
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		glColor3d(r,g,b);
        glTranslated(position.x(), position.y(), position.z());
        glutSolidSphere(Scene::d*0.4,4,4);
    glPopMatrix(); 
}