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

		// the parameter's name is symplectic euler so we'll chose that one
		case Scene::LEAP_FROG: {
			v2 = v2 + time* initial_a;
			p2 = p2 + time*v2;
			break;
		}
		case Scene::BACK_EULER: {
			// see bottom of page for eq doc
			v2 = (v2 + (time / mass) * (force - damping*v2)) / (1 - (time / mass) *(time*force + damping ) );
			p2 = p2 + time*v2;
			break;
		}
		
		default:
		break;
	}
}

double CalculateForce(double p1, double p2, double length, double stiffness) {
	return stiffness*((p1-p2)-length) - g;
}

Vec2 CalculateForce2D(Vec2& p1, Vec2& p2, double L, double stiffness) {
	Vec2 p = p1-p2;
	return -stiffness * (p.length() - L) * ((1/p.length()) * p);
}

Vec2 CalculateGroundForce(Vec2& p, double stiffness) {
	Vec2 n = Vec2(0, 1);
	double d = p.y + 1;
	return -stiffness * d * n;
}

Vec2 Calculate2dCollForce(Vec2& p, Vec2& v, Vec2& p1, Vec2& p2, double L, double k, double d) {
	Vec2 force_1 = CalculateForce2D(p, p1, L, k);
	force_1 += CalculateForce2D(p, p2, L, k);
	force_1 -= Vec2(0, g);
	force_1 -= d*v;

	// add ground force if necessary
	if (p.y <= -1) {
		force_1 += CalculateGroundForce(p, k);
	}
	return force_1;
}

// Exercise 3
// falling triangle
void AdvanceTimeStep3(double k, double m, double d, double L, double dt,
	Vec2& p1, Vec2& v1, Vec2& p2, Vec2& v2, Vec2& p3, Vec2& v3)
{
	Vec2 force_1 = Calculate2dCollForce(p1, v1, p2, p3, L, k ,d);
	Vec2 force_2 = Calculate2dCollForce(p2, v2, p1, p3, L, k, d);
	Vec2 force_3 = Calculate2dCollForce(p3, v3, p1, p2, L, k, d);

	Vec2 initial_a1 = (1/m) * (force_1);
	Vec2 initial_a2 = (1/m) * (force_2);
	Vec2 initial_a3 = (1 / m) * (force_3);

	// Symplectic Euler
	v1 = v1 + dt*initial_a1;  p1 = p1 + dt*v1;
	v2 = v2 + dt*initial_a2;  p2 = p2 + dt*v2;
	v3 = v3 + dt*initial_a3;  p3 = p3 + dt*v3;
}


/*
backward euler 1-d
deltaX = X_new - X;
deltaV = V_new - V;
F_n = F(t) + SpringF*(deltaX)+dampingF*(deltaV)
V_new = V + (t*F_n) / M;
X_new = X + t*V_n;


v_n = v + (t / M) * (F + t*SpringF*V_n + dampingF*(V_n - V));
v_n *(1 - ((t / M) *[t*Spring + dampingF])) = v + (t / M) *(F - dampinfF*V);
v_n = (v + (t / M) * (F - dampingF*V)) / (1 - ((t / M) *[t*Spring + dampingF]))
*/