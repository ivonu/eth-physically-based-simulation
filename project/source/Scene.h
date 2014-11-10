#pragma once

#include <vector>
#include "particle.h"
using namespace std;

class Scene
{

public:
	vector<Particle> particles;

   	const static int NUM_PARTICLES = 5;
   	const static double d = 0.1;
   	const static double h = 2 * d;
   	const static double volume = d*d*d;
   	const static int rho0 = 1000;
   	const static int k = 1000;
   	const static double mass = volume * rho0;
   	const static double mu = 0.00089;

	const static double timestep = 0.01;
	
protected:
	//Animation
	bool pause;

public:
	Scene(void);
	Scene(int argc, char* argv[]);
	~Scene(void);

	//Initialization
	void Init(void);
	void Render();
	void Update();
	vector<Particle> findNeighboors (int j);

	double poly6_kernel(double r);
	double spiky_kernel_gradient(double r);
	double viscosity_laplacian(double r);
};
