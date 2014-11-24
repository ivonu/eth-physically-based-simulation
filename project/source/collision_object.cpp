#include "collision_object.h"

CollisionPlane::CollisionPlane(Vector3d anchor, Vector3d normal) {
	this->anchor = anchor;
	this->normal = normal;
}

Vector3d CollisionPlane::getNormal(Vector3d point) {
	return normal;
}

bool CollisionPlane::handleCollision(Particle* particle, double dt) {
	double distToPlane = (normal | (particle->position - anchor));

	if (distToPlane >= 0) 
		return false;

	particle->position -= normal * distToPlane;
	particle->speed = particle->speed.reflectionAt(normal);
	return true;
}

CollisionTriangle::CollisionTriangle (Vector3d v1, Vector3d v2, Vector3d v3) {
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->normal = (v2-v1).cross(v3-v1).normalized();
}

Vector3d CollisionTriangle::getNormal(Vector3d point) {
	return normal;
}

bool CollisionTriangle::handleCollision(Particle* particle, double dt) {
	
	Vector3d o = particle->position - (particle->speed*dt);

	if ((particle->position-o).length() < 0.001)
		return false;

	Vector3d d = (particle->position - o).normalized();


	double t = -(((o-v1) | normal) / (d | normal));
	Vector3d x = o + t*d;
	
	if ((particle->speed*dt).length() <= t)
		return false;

	Vector3d v1_v2 = v2 - v1;
	Vector3d v2_v3 = v3 - v2;
	Vector3d v3_v1 = v1 - v3;

	if ((normal | v1_v2.cross(x-v1)) < 0)
		return false;

	if ((normal | v2_v3.cross(x-v2)) < 0)
		return false;

	if ((normal | v3_v1.cross(x-v3)) < 0)
		return false;

	Vector3d p = particle->position;
	cout << "o: " << o.x() << " / " << o.y() << " / " << o.z() << endl;
	cout << "p: " << p.x() << " / " << p.y() << " / " << p.z() << endl;
	cout << "d: " << d.x() << " / " << d.y() << " / " << d.z() << endl;
	cout << "t: " << t << endl;
	cout << "x: " << x.x() << " / " << x.y() << " / " << x.z() << endl;
	cout << "old speed: " << particle->speed.x() << " / " << particle->speed.y() << " / " << particle->speed.z() << endl;


	double distToPlane = (normal | (particle->position - v1));
	particle->position -= normal * distToPlane;
	particle->speed = particle->speed.reflectionAt(normal);

	//particle->speed = particle->speed.reflectionAt(normal);
	//particle->position = x + particle->speed * (dt-t);
	//particle->speed = Vector3d(0,0,0);

	cout << "new speed: " << particle->speed.x() << " / " << particle->speed.y() << " / " << particle->speed.z() << endl;
	
	return true;
}