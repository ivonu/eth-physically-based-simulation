#pragma once

#include <vector>
#include "utilities.h"
#include "particle.h"
#include "Eigen/Core"

typedef float ScalarType;
typedef unsigned IndexType;
class Grid {
	public:
		vector< vector<Particle*> > grid;

		int num_of_points;
		Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> values;
		Eigen::Matrix<ScalarType, Eigen::Dynamic, 3> points;
		
		vector< double > colors;
		Vector3d size;
		Vector3d lower_bound;
		double h;

		Grid(Vector3d size, Vector3d lower_bound, double h);

		void removeParticles();
		void addParticle(Particle* particle);
		vector<Particle*> getNeighboors(Particle* particle);
		
		double poly6_kernel(double r);
		void calculateColors();

};