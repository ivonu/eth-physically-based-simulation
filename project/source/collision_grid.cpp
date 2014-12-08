#include "collision_grid.h"
#include <set>
#include <algorithm>

CollisionGrid::CollisionGrid(Vector3d size, Vector3d lower_bound, double h)
{
	this->size = size / h;
	this->lower_bound = lower_bound;
	this->h = h;

	this->size.x() = ceil(this->size.x());
	this->size.y() = ceil(this->size.y());
	this->size.z() = ceil(this->size.z());

	this->grid = vector< vector<CollisionTriangle*> >((this->size.x()+1) * (this->size.y()+1) * (this->size.z()+1));
}

void CollisionGrid::removeCollisionObjects() {
	for (int i = 0; i < grid.size(); i++) {
		grid[i].clear();
	}
}

void CollisionGrid::addCollisionObject(CollisionTriangle* object) {

	double min_x = min(object->v1.x(), min(object->v2.x(), object->v3.x()));
	double min_y = min(object->v1.y(), min(object->v2.y(), object->v3.y()));
	double min_z = min(object->v1.z(), min(object->v2.z(), object->v3.z()));
	double max_x = max(object->v1.x(), max(object->v2.x(), object->v3.x()));
	double max_y = max(object->v1.y(), max(object->v2.y(), object->v3.y()));
	double max_z = max(object->v1.z(), max(object->v2.z(), object->v3.z()));

	min_x = floor((min_x - lower_bound.x()) / h);
	min_y = floor((min_y - lower_bound.y()) / h);
	min_z = floor((min_z - lower_bound.z()) / h);
	max_x = ceil((max_x - lower_bound.x()) / h);
	max_y = ceil((max_y - lower_bound.y()) / h);
	max_z = ceil((max_z - lower_bound.z()) / h);

	for (int x = min_x; x <= max_x; x++) {
		for (int y = min_y; y <= max_y; y++) {
			for (int z = min_z; z <= max_z; z++) {
				int array_pos = x*size.y()*size.z() + y*size.z() + z;
				grid[array_pos].push_back(object);
			}
		}
	}
}

vector<CollisionTriangle*> CollisionGrid::getCollisionObjects(Particle* particle, double dt) {

	// Vector3d old_pos = particle->position - (particle->speed*dt);
	// old_pos = (old_pos - lower_bound) / h;

	Vector3d old_pos = (particle->old_position - lower_bound) / h;
	Vector3d pos = (particle->position - lower_bound) / h;

	int min_x = floor(min(old_pos.x(), pos.x())); int max_x = ceil(max(old_pos.x(), pos.x()));
	int min_y = floor(min(old_pos.y(), pos.y())); int max_y = ceil(max(old_pos.y(), pos.y()));
	int min_z = floor(min(old_pos.z(), pos.z())); int max_z = ceil(max(old_pos.z(), pos.z()));

	set<CollisionTriangle*> objects;
	for (int x = max(0, min_x); x <= min((int)size.x(), max_x); x++) {
		for (int y = max(0, min_y); y <= min((int)size.y(), max_y); y++) {
			for (int z = max(0, min_z); z <= min((int)size.z(), max_z); z++) {
				int array_pos = x*size.y()*size.z() + y*size.z() + z;
				std::copy(grid[array_pos].begin(), grid[array_pos].end(), std::inserter(objects, objects.end()));			
			}
		}
	}
			
	return vector<CollisionTriangle*> (objects.begin(), objects.end());
}
