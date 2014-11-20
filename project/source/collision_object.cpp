#include "collision_object.h"

CollisionPlane::CollisionPlane(Vector3d anchor, Vector3d normal) {
	this->anchor = anchor;
	this->normal = normal;
}

Vector3d CollisionPlane::getNormal(Vector3d point) {
	return normal;
}

void CollisionPlane::handleCollision(Particle* particle) {

	double distToPlane = (normal | (particle->position - anchor));

	

	if (distToPlane < 0) {
		particle->position -= normal * distToPlane;
		particle->speed = particle->speed.reflectionAt(normal);
	}
}