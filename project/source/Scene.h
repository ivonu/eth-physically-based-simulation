#pragma once

#include <vector>
#include "particle.h"
using namespace std;

class Scene
{

public:
	Particle particle;
	
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
};
