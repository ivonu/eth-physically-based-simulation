#include "gauss_seidel.h"
#include "fluid2d.h"

#define GETF(x,y) field[(x) + (y) * xRes]
#define GETB(x,y) b[(x) + (y) * xRes]

// Problem 1
void ExSolvePoisson(int xRes, int yRes, int _iterations, double _accuracy, double* field, double* b)
{
	// note that the boundaries are handles by the framework, so you iterations should be similar to:
	for (int y = 1; y < yRes - 1; y++) {
		for (int x = 1; x < xRes - 1; x++)
		{
			//GETF(x,y) ... GETB(x,y)
		}
	}

	// for your debugging, and ours, please add these prints after every iteration
	/*if(iter==_iterations-1) 
		printf("Pressure solver: iter=%d , res=%f \n",iter, residual);
	if(residual<_accuracy) {
		printf("Pressure solver: iter=%d , converged \n",iter,residual);
		break; // optional
	} */

}

// Probelm 2
void ExCorrectVelocities(int _xRes, int _yRes, double _dt, const double* _pressure, double* _xVelocity, double* _yVelocity)
{
	// note: velocity u_{i+1/2} is practically stored at i+1
}

// Problem 3
void ExAdvectWithSemiLagrange(int xRes, int yRes, double dt,double* xVelocity, double* yVelocity, double *density, double *densityTemp,double *xVelocityTemp,double *yVelocityTemp)
{
	// note: velocity u_{i+1/2} is practically stored at i+1
}
