#pragma once

#include <vector>
#include "particle.h"
#include "grid.h"
#include "collision_object.h"
#include "glm.h"

using namespace std;

class Scene
{

public:
  	GLMmodel *objmodel_ptr;

	Grid grid;
	vector<Particle*> particles;
   vector<CollisionObject*> collision_objects;

	const static Vector3d initial_pos;
	const static int NUM_PARTICLES_X;
	const static int NUM_PARTICLES_Y;
	const static int NUM_PARTICLES_Z;
	const static int NUM_PARTICLES;
	const static double BARRIER;
	const static double LEFT_WALL;
	const static double RIGHT_WALL;
	const static double BACK_WALL;
	const static double FRONT_WALL;
	const static double BOTTOM_WALL;
	const static double TOP_WALL;
	const static double d;
	const static double h;
	const static double volume;
	const static double rho0;
	const static double k;
	const static double mu;

	const static int timestep;
	const static double collision_damping;
	
protected:
	//Animation
	bool pause;

public:
	Scene(void);
	~Scene(void);

	//Initialization
	void Init(void);
	void Render();
	void Update();
	vector<Particle*> findNeighboors (const vector<Particle*>& potential_neighboors, Particle* particle);

	double poly6_kernel(double r);
	double spiky_kernel_gradient(double r);
	double viscosity_laplacian(double r);
};
