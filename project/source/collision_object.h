#pragma once

#include "utilities.h"
#include "particle.h"

class CollisionObject {
	
	public:
		CollisionObject() {};
		virtual ~CollisionObject() {};
		virtual Vector3d getNormal(Vector3d point) {return Vector3d(0,0,0);};
		virtual void handleCollision(Particle* particle) {}; 
};

class CollisionPlane : public CollisionObject {
	public:
		Vector3d normal;
		Vector3d anchor;

		CollisionPlane(Vector3d anchor, Vector3d normal);
		~CollisionPlane() {};

		Vector3d getNormal(Vector3d point);
		void handleCollision(Particle* particle);
};