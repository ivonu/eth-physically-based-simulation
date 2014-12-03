#pragma once

#include "utilities.h"
#include "particle.h"

class CollisionObject {
	
	public:
		CollisionObject() {};
		virtual ~CollisionObject() {};
		virtual bool handleCollision(Particle* particle, double dt) { return false; }; 
};

class CollisionPlane : public CollisionObject {
	public:
		Vector3d normal;
		Vector3d anchor;

		static const double forceDistance;
		static const double forceStrength;

		CollisionPlane(Vector3d anchor, Vector3d normal);
		~CollisionPlane() {};

		bool handleCollision(Particle* particle, double dt);
		bool handleBoundaryForce(Particle* particle, double dt);
};

class CollisionTriangle : public CollisionObject {
	public:
		Vector3d v1, v2, v3;
		Vector3d normal;

		CollisionTriangle (Vector3d v1, Vector3d v2, Vector3d v3);
		~CollisionTriangle() {};

		bool handleCollision(Particle* particle, double dt);
};