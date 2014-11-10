#include "particle.h"
#include "GLUT/glut.h"
#include "Scene.h"

Particle::Particle(Vector3d position) {
	this->position = position;
}

void Particle::draw() {
	glColor3d(1,0,0);
	glPushMatrix();
        glTranslated(position.x(), position.y(), position.z());
        glutSolidSphere(Scene::d*0.5,36,36);
    glPopMatrix(); 
}