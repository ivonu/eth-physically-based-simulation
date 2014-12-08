#include "collision_grid.h"
#include <set>

CollisionGrid::CollisionGrid(Vector3d size, Vector3d lower_bound, double h)
{
	this->size = size / h;
	this->lower_bound = lower_bound;
	this->h = h;

	this->size.x() = ceil(this->size.x());
	this->size.y() = ceil(this->size.y());
	this->size.z() = ceil(this->size.z());

	this->grid = vector< vector<CollisionTriangle*> >((this->size.x()) * (this->size.y()) * (this->size.z()));
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

	min_x = max(0.0, floor((min_x - lower_bound.x()) / h));
	min_y = max(0.0, floor((min_y - lower_bound.y()) / h));
	min_z = max(0.0, floor((min_z - lower_bound.z()) / h));
	max_x = min(size.x()-1, floor((max_x - lower_bound.x()) / h));
	max_y = min(size.y()-1, floor((max_y - lower_bound.y()) / h));
	max_z = min(size.z()-1, floor((max_z - lower_bound.z()) / h));

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

	Vector3d old_pos = (particle->old_position - lower_bound) / h;
	Vector3d pos = (particle->position - lower_bound) / h;

	int min_x = max(0.0, floor(min(old_pos.x(), pos.x()))); 
	int min_y = max(0.0, floor(min(old_pos.y(), pos.y()))); 
	int min_z = max(0.0, floor(min(old_pos.z(), pos.z()))); 
	int max_x = min(size.x()-1, floor(max(old_pos.x(), pos.x())));
	int max_y = min(size.y()-1, floor(max(old_pos.y(), pos.y())));
	int max_z = min(size.z()-1, floor(max(old_pos.z(), pos.z())));

	set<CollisionTriangle*> objects;
	for (int x = min_x; x <= max_x; x++) {
		for (int y = min_y; y <= max_y; y++) {
			for (int z = min_z; z <= max_z; z++) {
				int array_pos = x*size.y()*size.z() + y*size.z() + z;
				std::copy(grid[array_pos].begin(), grid[array_pos].end(), std::inserter(objects, objects.end()));			
			}
		}
	}
			
	return vector<CollisionTriangle*> (objects.begin(), objects.end());
}
