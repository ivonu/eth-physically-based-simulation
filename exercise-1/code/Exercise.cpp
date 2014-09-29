#include <iostream>
#include "Vec2.h"
#include "LinSys.h"
#include "Scene.h"

using namespace std;

// gravitational acceleration (9.81)
static const double g = 9.81f;
double CalculateForce(double p1, double p2, double length, double stiffness);

// Exercise 1
// hanging mass point
void AdvanceTimeStep1(double stiffness, double mass, double damping, double length, double time, int method, double p1, double v1, double& p2, double& v2)
{
	double force = CalculateForce(p1, p2, length, stiffness);
	double initial_a = (force - damping*v2) / mass;

	switch (method) {
		case Scene::EULER:
		p2 = p2 + time*v2;
		v2 = v2 + time* initial_a;
		break;

		case Scene::MIDPOINT:
		{
		// location
			double mid_p = p2 + (time / 2) * v2;
			double mid_p_v = v2 + (time / 2) * initial_a;
			p2 = p2 + time*mid_p_v;
		// speed
			double mid_p_a = (CalculateForce(p1, mid_p, length, stiffness) - damping*mid_p_v) / mass;
			v2 = v2 + time * mid_p_a;
			break;
		}
		
		case Scene::ANALYTIC: {
			double alpha = -damping / (2 * mass); double beta = sqrt(4 * stiffness*mass - damping*damping) / (2 * mass);
			double c1 = (mass*g) / stiffness;
			double c2 = (-alpha / beta) * c1;
			p2 = c1*exp(alpha*time)*cos(beta*time) + c2*exp(alpha*time)*sin(beta*time) - length - (mass*g) / stiffness;
			break;
		}
		
		default:
		break;
	}
}

double CalculateForce(double p1, double p2, double length, double stiffness) {
	return stiffness*((p1-p2)-length) - g;
}

// Exercise 3
// falling triangle
void AdvanceTimeStep3(double k, double m, double d, double L, double dt,
	Vec2& p1, Vec2& v1, Vec2& p2, Vec2& v2, Vec2& p3, Vec2& v3)
{
	p1 += Vec2(1,1);
}
