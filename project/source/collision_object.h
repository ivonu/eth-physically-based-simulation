#pragma once

#include "utilities.h"
#include "particle.h"

class CollisionObject {
	
	public:
		CollisionObject() {};
		virtual ~CollisionObject() {};
		virtual Vector3d getNormal(Vector3d point) {return Vector3d(0,0,0);};
		virtual bool handleCollision(Particle* particle, double dt) { return false; }; 
};

class CollisionPlane : public CollisionObject {
	public:
		Vector3d normal;
		Vector3d anchor;

		CollisionPlane(Vector3d anchor, Vector3d normal);
		~CollisionPlane() {};

		Vector3d getNormal(Vector3d point);
		bool handleCollision(Particle* particle, double dt);
};

class CollisionTriangle : public CollisionObject {
	public:
		Vector3d v1, v2, v3;
		Vector3d normal;

		CollisionTriangle (Vector3d v1, Vector3d v2, Vector3d v3);
		~CollisionTriangle() {};

		Vector3d getNormal(Vector3d point);
		bool handleCollision(Particle* particle, double dt);
};