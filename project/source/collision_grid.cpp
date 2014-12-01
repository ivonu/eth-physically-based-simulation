#include "collision_grid.h"

CollisionGrid::CollisionGrid(Vector3d size, Vector3d lower_bound, double h)
{
	cout << "init start" << endl;
	this->size = size / h;
	this->lower_bound = lower_bound;
	this->h = h;

	this->size.x() = ceil(this->size.x());
	this->size.y() = ceil(this->size.y());
	this->size.z() = ceil(this->size.z());

	this->grid = vector< vector<CollisionTriangle*> >((this->size.x()+1) * (this->size.y()+1) * (this->size.z()+1));
	cout << "init end" << endl;
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
	max_x = floor((max_x - lower_bound.x()) / h);
	max_y = floor((max_y - lower_bound.y()) / h);
	max_z = floor((max_z - lower_bound.z()) / h);

	cout << "min: " << min_x << " / " << min_y << " / " << min_z << endl;
	cout << "max: " << max_x << " / " << max_y << " / " << max_z << endl;

	for (int x = min_x; x < max_x; x++) {
		for (int y = min_y; y < max_y; y++) {
			for (int z = min_z; z < max_z; z++) {
				int array_pos = x*size.y()*size.z() + y*size.z() + z;
				grid[array_pos].push_back(object);
			}
		}
	}
}

vector<CollisionTriangle*> CollisionGrid::getCollisionObjects(Particle* particle) {
	Vector3d pos = (particle->position - lower_bound) / h;
	pos.x() = floor(pos.x());
	pos.y() = floor(pos.y());
	pos.z() = floor(-pos.z()+0.0001);
	
	int array_pos = pos.x()*size.y()*size.z() + pos.y()*size.z() + pos.z();
	return grid[array_pos];				
}
