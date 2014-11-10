#include "utilities.h"

class Particle {
	public:
		Vector3d position;
		Vector3d speed;

		Particle() {};
		Particle (Vector3d position);
		void draw();
};