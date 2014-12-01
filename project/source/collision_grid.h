#pragma once

#include <vector>
#include "utilities.h"
#include "collision_object.h"

class CollisionGrid {
	public:
		vector< vector<CollisionTriangle*> > grid;
		Vector3d size;
		Vector3d lower_bound;
		double h;

		CollisionGrid(Vector3d size, Vector3d lower_bound, double h);

		void removeCollisionObjects();
		void addCollisionObject(CollisionTriangle* object);
		vector<CollisionTriangle*> getCollisionObjects(Particle* particle, double dt);

};