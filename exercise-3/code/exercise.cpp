#include "gauss_seidel.h"
#include "fluid2d.h"

#define GETF(x,y) field[(x) + (y) * xRes]
#define GETB(x,y) b[(x) + (y) * xRes]

// Problem 1
void ExSolvePoisson(int xRes, int yRes, int _iterations, double _accuracy, double* field, double* b)
{
	// note that the boundaries are handles by the framework, so you iterations should be similar to:
	for (int i = 1; i <= _iterations; i++) {
		for (int y = 1; y < yRes - 1; y++) {
			for (int x = 1; x < xRes - 1; x++) {
				GETF(x, y) = (GETB(x, y) + GETF(x + 1, y) + GETF(x - 1, y) + GETF(x, y + 1) + GETF(x, y - 1)) / 4;

				// calc residual
				double residual = 4 * GETF(x, y) - ( GETF(x + 1, y) + GETF(x - 1, y) + GETF(x, y + 1) + GETF(x, y - 1) );
				residual += GETB(x,y);
				if (i == _iterations - 1)
					printf("Pressure solver: iter=%d , res=%f \n", i, residual);
				if (residual<_accuracy) {
					//printf("Pressure solver: iter=%d , converged \n", i, residual);
					break; // optional
				}
			}
		}
	
		
	}
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
