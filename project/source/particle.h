#pragma once

#include "utilities.h"

class Particle {
	public:
		Vector3d position;
		Vector3d speed;
		double density;
		double pressure;
		Vector3d force;
		double mass;
		double r, g, b;

		Particle() {};
		Particle (Vector3d position);
		void draw();
};