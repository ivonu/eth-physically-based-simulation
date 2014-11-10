#pragma once

#include <vector>
#include "particle.h"
using namespace std;

class Scene
{

public:
	vector<Particle> particles;

   	const static int NUM_PARTICLES;
   	const static double d;
   	const static double h;
   	const static double volume;
   	const static int rho0;
   	const static int k;
   	const static double mass;
   	const static double mu;

	const static int timestep;
	
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
