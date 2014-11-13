#pragma once

#include <vector>
#include "utilities.h"
#include "particle.h"

class Grid {
	public:
		vector< vector<Particle*> > grid;
		Vector3d size;
		Vector3d lower_bound;
		double h;

		Grid(Vector3d size, Vector3d lower_bound, double h);

		void removeParticles();
		void addParticle(Particle* particle);
		vector<Particle*> getNeighboors(Particle* particle);

};