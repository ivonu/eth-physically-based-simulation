#include <iostream>
using namespace std;
#include "Vec2.h"
#include "LinSys.h"
#include "Scene.h"

// gravitational acceleration (9.81)
static const double g = 9.81f;
int CalculateForce(double p1, double p2, double length, double stiffness);

// Exercise 1
// hanging mass point
void AdvanceTimeStep1(double stiffness, double mass, double damping, double length, double time_step, int method, double p1, double v1, double& p2, double& v2)
{
	int force = CalculateForce(p1, p2, length, stiffness);
	switch (method) {
	case Scene::Method::EULER:
		p2 = p2 + time_step*v2;
		v2 = v2 + time_step* ((force - damping*v2) / mass);
		break;
	default:
		break;
	}
}

int CalculateForce(double p1, double p2, double length, double stiffness) {
	return stiffness*((p1-p2)-length) - g;
}

// Exercise 3
// falling triangle
void AdvanceTimeStep3(double k, double m, double d, double L, double dt,
                      Vec2& p1, Vec2& v1, Vec2& p2, Vec2& v2, Vec2& p3, Vec2& v3)
{
   p1 += Vec2(1,1);
}
