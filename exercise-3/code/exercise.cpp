#include "gauss_seidel.h"
#include "fluid2d.h"

#define GETF(x,y) field[(x) + (y) * xRes]
#define GETB(x,y) b[(x) + (y) * xRes]
#define GET_X_V(x,y) xVelocity[(x) + (y) * xRes]
#define GET_Y_V(x,y) yVelocity[(x) + (y) * xRes]

// clamp if necessary
#define GET_2D_array(arr,x,y) (x>xRes || y>yRes)? 0 : arr[(x) + (y) * xRes]

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
void ExCorrectVelocities(int xRes, int yRes, double dt, const double* field, double* xVelocity, double* yVelocity) {
	// note: velocity u_{i+1/2} is practically stored at i+1
	for (int y = 1; y < yRes - 1; y++) {
		for (int x = 1; x < xRes - 1; x++) {
			GET_X_V(x, y) = GET_X_V(x, y) - dt*(GETF(x + 1, y) - GETF(x, y));
			GET_Y_V(x, y) = GET_Y_V(x, y) - dt*(GETF(x, y + 1) - GETF(x, y));
		}
	}
}

// Problem 3
void ExAdvectWithSemiLagrange(int xRes, int yRes, double dt,double* xVelocity, double* yVelocity, double *density, double *densityTemp,double *xVelocityTemp,double *yVelocityTemp)
{
	// note: velocity u_{i+1/2} is practically stored at i+1
	for (int y = 1; y < yRes - 1; y++) {
		for (int x = 1; x < xRes - 1; x++) {
			double p_x = x - dt*GET_X_V(x, y);
			double p_y = y - dt*GET_Y_V(x, y);

			// calc pressure at p
			// find interpolating pts
			
			int floor_p_x = floor(p_x); int floor_p_y = floor(p_y);

			int p_pres_1_x = floor_p_x - 1; int p_pres_1_y = floor_p_y - 1;
			double w_pres_1 = abs((p_x - p_pres_1_x)*(p_y - p_pres_1_y));

			int p_pres_2_x = floor_p_x - 1; int p_pres_2_y = floor_p_y;
			double w_pres_2 = abs((p_x - p_pres_2_x)*(p_y - p_pres_2_y));

			int p_pres_3_x = floor_p_x; int p_pres_3_y = floor_p_y;
			double w_pres_3 = abs((p_x - p_pres_3_x)*(p_y - p_pres_3_y));

			int p_pres_4_x = floor_p_x; int p_pres_4_y = floor_p_y - 1;
			double w_pres_4 = abs((p_x - p_pres_4_x)*(p_y - p_pres_4_y));

			// bilinear interpolation with clamping if necesssary
			GET_2D_array(densityTemp, x, y) = w_pres_1* GET_2D_array(density, p_pres_1_x, p_pres_1_y) + w_pres_2* GET_2D_array(density, p_pres_2_x, p_pres_2_y)
				+ w_pres_3 * GET_2D_array(density, p_pres_3_x, p_pres_3_y) + w_pres_4 * GET_2D_array(density, p_pres_4_x, p_pres_4_y);

			// calc speed at p, the index calculation is same, the weights are different
			floor_p_x += 1; floor_p_y += 1;

			double p_vel_1_x = floor_p_x; int p_vel_1_y = floor_p_y;
			double w_vel_1 = abs((floor_p_x - p_vel_1_x) * (floor_p_y - p_vel_1_y));

			double p_vel_2_x = floor_p_x + 1; int p_vel_2_y = floor_p_y;
			double w_vel_2 = abs((floor_p_x - p_vel_2_x) * (floor_p_y - p_vel_2_y));

			double p_vel_3_x = floor_p_x + 1; int p_vel_3_y = floor_p_y + 1;
			double w_vel_3 = abs((floor_p_x - p_vel_3_x) * (floor_p_y - p_vel_3_y));

			double p_vel_4_x = floor_p_x; int p_vel_4_y = floor_p_y + 1;
			double w_vel_4 = abs((floor_p_x - p_vel_4_x) * (floor_p_y - p_vel_4_y));

			// bilinear interpolation with clamping if necesssary
			GET_2D_array(xVelocity, x, y) = w_vel_1* GET_2D_array(xVelocity, p_pres_1_x, p_pres_1_y) + w_vel_2* GET_2D_array(xVelocity, p_pres_2_x, p_pres_2_y)
				+ w_vel_3 * GET_2D_array(xVelocity, p_pres_3_x, p_pres_3_y) + w_vel_4 * GET_2D_array(xVelocity, p_pres_4_x, p_pres_4_y);

			GET_2D_array(yVelocity, x, y) = w_vel_1* GET_2D_array(yVelocity, p_pres_1_x, p_pres_1_y) + w_vel_2* GET_2D_array(yVelocity, p_pres_2_x, p_pres_2_y)
				+ w_vel_3 * GET_2D_array(yVelocity, p_pres_3_x, p_pres_3_y) + w_vel_4 * GET_2D_array(yVelocity, p_pres_4_x, p_pres_4_y);
		}
	}
	
	// copy results from tmp buffers
	memcpy(density, densityTemp, xRes*yRes);
	memcpy(xVelocity, xVelocityTemp, xRes*yRes);
	memcpy(yVelocity, yVelocityTemp, xRes*yRes);
}
