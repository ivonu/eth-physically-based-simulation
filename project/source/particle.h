#pragma once

#include "utilities.h"

class Particle {
	public:
		Vector3d old_position;
		Vector3d position;
		Vector3d speed;
		double density;
		double pressure;
		Vector3d force;
		double mass;
		double rho0;
		Vector3d color;

		bool ice;

		Particle() {};
		Particle (Vector3d position);
		void draw();
};