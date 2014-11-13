#include "grid.h"

Grid::Grid(Vector3d size, Vector3d lower_bound, double h)
{
	this->size = size / h;
	this->lower_bound = lower_bound;
	this->h = h;

	this->size.x() = ceil(this->size.x());
	this->size.y() = ceil(this->size.y());
	this->size.z() = ceil(this->size.z());

	this->grid = vector< vector<Particle*> >((this->size.x()+1) * (this->size.y()+1) * (this->size.z()+1));
}

void Grid::removeParticles() {
	for (int i = 0; i < grid.size(); i++) {
		grid[i].clear();
	}
}

void Grid::addParticle(Particle* particle) {
	Vector3d pos = (particle->position - lower_bound) / h;
	pos.x() = floor(pos.x());
	pos.y() = floor(pos.y());
	pos.z() = floor(-pos.z()+0.0001);

	int array_pos = pos.x()*size.y()*size.z() + pos.y()*size.z() + pos.z();

	grid[array_pos].push_back(particle);
}

vector<Particle*> Grid::getNeighboors(Particle* particle) {
	Vector3d pos = (particle->position - lower_bound) / h;
	pos.x() = floor(pos.x());
	pos.y() = floor(pos.y());
	pos.z() = floor(-pos.z()+0.0001);
	
	vector<Particle*> neighboors;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				if (pos.x()+x < 0 || pos.y()+y < 0 || pos.z()+z < 0 ||
					pos.x()+x > size.x() || pos.y()+y > size.y() || pos.z()+z > size.z())
					continue;
				int array_pos = (pos.x()+x)*size.y()*size.z() + (pos.y()+y)*size.z() + (pos.z()+z);
				std::copy(grid[array_pos].begin(), grid[array_pos].end(), std::back_inserter(neighboors));
			}
		}
	}

	return neighboors;
}
