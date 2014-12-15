#include "grid.h"

Grid::Grid(Vector3d size, Vector3d lower_bound, double h) : 
	num_of_points ((size.x()/h)*(size.y()/h)*(size.z()/h)),
	values (num_of_points,1),
	points (num_of_points,3)
{
	this->size = size / h;
	this->lower_bound = lower_bound;
	this->h = h;

	this->size.x() = ceil(this->size.x());
	this->size.y() = ceil(this->size.y());
	this->size.z() = ceil(this->size.z());

	this->grid = vector< vector<Particle*> >((this->size.x()+1) * (this->size.y()+1) * (this->size.z()+1));


	for (int x = 0; x < size.x(); x++) {
		for (int y = 0; y < size.y(); y++) {
			for (int z = 0; z < size.z(); z++) {
				int array_pos = x*size.y()*size.z() + y*size.z() + z;
				Vector3d real_world_pos ((Vector3d(x,y,z) + lower_bound) * h);
				points(array_pos,0) = real_world_pos.x();
				points(array_pos,1) = real_world_pos.y();
				points(array_pos,2) = real_world_pos.z();
			}
		}
	}
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


double Grid::poly6_kernel(double r) {
	if (r < 0 || r > h) 
		return 0;

	return (315.0 / (64.0 * M_PI * pow (h, 9))) * pow(h*h - r*r, 3);
}

void Grid::calculateColors() {
	for (int x = 0; x < size.x(); x++) {
		for (int y = 0; y < size.y(); y++) {
			for (int z = 0; z < size.z(); z++) {
				int array_pos = x*size.y()*size.z() + y*size.z() + z;
				Particle part(Vector3d(x,y,z));
				vector<Particle *> particles = getNeighboors(&part);

				for (int i = 0; i < particles.size(); i++) {
					if ((part.position - particles[i]->position).length() <= h) {
						values(array_pos) += (particles[i]->mass / particles[i]->density) * poly6_kernel((Vector3d(x,y,z)-particles[i]->position).length());
					}
				}

			}
		}
	}
}