#include "particle.h"
#include "GLUT/glut.h"
#include "scene.h"

Particle::Particle(Vector3d position) {
	this->position = position;
	this->rho0 = Scene::rho0;
	this->mass = Scene::volume * rho0;
	this->color = Vector3d(1,0,0);
	this->ice = false;
}

void Particle::draw() {
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		glColor3d(color.x(), color.y(), color.z());
        glTranslated(position.x(), position.y(), position.z());
        if (this->ice) 
        	glutSolidSphere(Scene::d*0.8, 6, 3);
        else 
        	glutSolidSphere(Scene::d*0.4, 6, 3);
    glPopMatrix(); 
}