#include "Scene.h"
#include "utilities.h"
#include "GLUT/glut.h"
#include <cmath>

const int Scene::NUM_PARTICLES = 5;
const double Scene::d = 0.1;
const double Scene::h = 2 * d;
const double Scene::volume = d*d*d;
const double Scene::rho0 = 1000;
const double Scene::k = 1000;
const double Scene::mass = volume * rho0;
const double Scene::mu = 0.00089;

const int Scene::timestep = 1;

Scene::Scene(void)
{
   Init();
}

Scene::Scene(int argc, char* argv[])
{
   Init();
}

Scene::~Scene(void)
{
}


void Scene::Init(void)
{
   //Animation settings
   pause=false;

   Vector3d initial_pos (-0.5, 1.0, -11.0);

   for (int x = 0; x < NUM_PARTICLES; x++) {
	   for (int y = 0; y < NUM_PARTICLES; y++) {
	   		for (int z = 0; z < NUM_PARTICLES; z++) {
   				particles.push_back(Particle(initial_pos + Vector3d(x*d, y*d, z*d)));
   			}
   		}
  	}
}

double Scene::poly6_kernel(double r) {
	if (r < 0 || r > h) 
		return 0;

	return (315.0 / (64.0 * M_PI * pow (h, 9))) * pow(h*h - r*r, 3);
}

double Scene::spiky_kernel_gradient(double r) {
	if (r < 0 || r > h) 
		return 0.0;

	return -3.0 * (15.0 / (M_PI * pow(h, 6))) * pow(h-r, 2);
}

double Scene::viscosity_laplacian(double r) {
	if (r < 0 || r > h) 
		return 0;

	return (45.0 / (M_PI * pow(h, 6))) * (h-r);
}


vector<Particle*> Scene::findNeighboors (int j) {
	vector<Particle*> neighboors;
	for (int i = 0; i < particles.size(); i++) {
		if ((particles[j].position - particles[i].position).length() <= h) {
			neighboors.push_back(&particles[i]);
		}
	}
	return neighboors;
}


void Scene::Update(void)
{
	if (pause) {
	  return;
	}

	vector< vector<Particle*> > neighboors (particles.size());
	for (int i = 0; i < particles.size(); i++) {
		// find neighborhoods Ni(t)
		neighboors[i] = findNeighboors(i);

		// compute density
		particles[i].density = 0.0;
		for (int j = 0; j < neighboors[i].size(); j++) {
			particles[i].density += mass * poly6_kernel((particles[i].position - neighboors[i][j]->position).length());
		}

		// compute pressure
		particles[i].pressure = k * (particles[i].density - rho0);
		particles[i].pressure = max(particles[i].pressure, 0.0);
	}

	// compute forces
	for (int i = 0; i < particles.size(); i++) {

		Vector3d pressure_force (0.0,0.0,0.0);
		Vector3d viscosity_force (0.0,0.0,0.0);
		for (int j = 0; j < neighboors[i].size(); j++) {
			Vector3d x_ij = (particles[i].position - neighboors[i][j]->position);

			double pressure_force_scalar = (mass / neighboors[i][j]->density) *
				((particles[i].pressure + neighboors[i][j]->pressure) / 2.0) * 
				spiky_kernel_gradient(x_ij.length());

			if (x_ij.length() != 0.0)
				pressure_force -= x_ij.normalized() * pressure_force_scalar;
			
			viscosity_force += mu * 
				(mass / neighboors[i][j]->density) * 
				(neighboors[i][j]->speed - particles[i].speed) * 
				viscosity_laplacian(x_ij.length());
		}
		
		Vector3d gravitation_force = particles[i].density * Vector3d(0,-9.81,0);

		particles[i].force = Vector3d(0,0,0);
		particles[i].force += pressure_force;
		particles[i].force += viscosity_force;
		particles[i].force += gravitation_force;
	}

	double dt = timestep / 1000.0;
	for (int i = 0; i < particles.size(); i++) {
		// update velocities and positions
		particles[i].speed += particles[i].force * (dt / particles[i].density);
		particles[i].position += (particles[i].speed * dt);

		// handle collisions
		if (particles[i].position.y() < -1) {
			particles[i].position.y() = -1;
			particles[i].speed.y() = -particles[i].speed.y();
		}
		if (particles[i].position.x() < -2) {
			particles[i].position.x() = -2;
			particles[i].speed.x() = -particles[i].speed.x();
		}
		if (particles[i].position.x() > 2) {
			particles[i].position.x() = 2;
			particles[i].speed.x() = -particles[i].speed.x();
		}
		if (particles[i].position.z() < -14) {
			particles[i].position.z() = -14;
			particles[i].speed.z() = -particles[i].speed.z();
		}
		if (particles[i].position.z() > -10) {
			particles[i].position.z() = -10;
			particles[i].speed.z() = -particles[i].speed.z();
		}
	}
}

void Scene::Render(void)
{
	for (int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}
}
