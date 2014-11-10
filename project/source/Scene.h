#pragma once

#include <vector>
using namespace std;

class Scene
{

public:
	double x = 0.0;
	

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
