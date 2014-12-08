#include "collision_object.h"
#include "scene.h"

const double CollisionPlane::forceDistance = Scene::d;
const double CollisionPlane::forceStrength = 0.1;

CollisionPlane::CollisionPlane(Vector3d anchor, Vector3d normal) {
	this->anchor = anchor;
	this->normal = normal;
}

bool CollisionPlane::handleBoundaryForce(Particle* particle, double dt) {
	double distToPlane = (normal | (particle->position - anchor));

	// no collision
	if (distToPlane >= forceDistance) 
		return false;

	// collision
	if (distToPlane < 0)
		return false;

	// boundary force
	float factor = 1.0 - (distToPlane / forceDistance); // linear
	particle->force += normal * factor * forceStrength; // add the force
	return true;
}

bool CollisionPlane::handleCollision(Particle* particle, double dt) {
    double distToPlane = (normal | (particle->position - anchor));

    if (distToPlane >= 0) 
        return false;

    particle->position -= normal * distToPlane;
    particle->speed = particle->speed.reflectionAt(normal) * Scene::collision_damping;
    
    return true;
}

CollisionTriangle::CollisionTriangle (Vector3d v1, Vector3d v2, Vector3d v3) {
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->normal = (v2-v1).cross(v3-v1).normalized();
}

bool CollisionTriangle::handleCollision(Particle* particle, double dt) {
	
	// no collision
	if ((normal | (particle->position - v1)) >= 0)
		return false;

	// distance from old position to position
	double dist = (particle->position - particle->old_position).length();

	// normalized direction
	Vector3d d = (particle->position - particle->old_position).normalized();

	// parallel to plane?
	if ((d | normal) == 0) 
		return false;

	// distance from old position to hit point
	double t = -((particle->old_position - v1) | normal) / (d | normal);

	// o also below plane?
	if (t < 0) 
		return false;

	// dist from o to p smaller than dist from o to x?
	if (dist <= t)
		return false;

	// hit point
	Vector3d x = particle->old_position + t*d;

	Vector3d v1_v2 = v2 - v1;
	Vector3d v2_v3 = v3 - v2;
	Vector3d v3_v1 = v1 - v3;

	if ((normal | v1_v2.cross(x-v1)) < 0)
		return false;

	if ((normal | v2_v3.cross(x-v2)) < 0)
		return false;

	if ((normal | v3_v1.cross(x-v3)) < 0)
		return false;

	particle->speed = particle->speed.reflectionAt(normal) * Scene::collision_damping;
	particle->position = x + particle->speed.normalized() * (dist-t) * Scene::collision_damping;
	particle->old_position = x;

	return true;
}