#include "gauss_seidel.h"
#include "fluid2d.h"

#define GETF(x,y) field[(x) + (y) * xRes]
#define GETF_OLD(x,y) field_old[(x) + (y) * xRes]
#define GETB(x,y) b[(x) + (y) * xRes]
#define GET_X_V(x,y) xVelocity[(x) + (y) * xRes]
#define GET_Y_V(x,y) yVelocity[(x) + (y) * xRes]

// clamp if necessary
#define GET_2D_array(arr,x,y) (x>xRes || y>yRes)? 0 : arr[(x) + (y) * xRes]

double bilinear_inter(double x, double y, double* arr, int xSize, int ySize);

// Problem 1
void ExSolvePoisson(int xRes, int yRes, int _iterations, double _accuracy, double* field, double* b)
{
	// note that the boundaries are handles by the framework, so you iterations should be similar to:
	double* field_old = new double[xRes*yRes];
	double residual = 0;
	for (int i = 1; i <= _iterations; i++) {
		memcpy(field_old, field, xRes*yRes*sizeof(double));
		for (int y = 1; y < yRes - 1; y++) {
			for (int x = 1; x < xRes - 1; x++) {
				//GETF(x, y) = (GETB(x, y) + GETF(x + 1, y) + GETF(x - 1, y) + GETF(x, y + 1) + GETF(x, y - 1)) / 4;
				GETF(x, y) = (GETB(x, y) + GETF_OLD(x + 1, y) + GETF_OLD(x - 1, y) + GETF_OLD(x, y + 1) + GETF_OLD(x, y - 1)) / 4;

				// calc residual
				residual += fabs(GETF(x, y) - (4 * GETF(x, y) - (GETF(x + 1, y) + GETF(x - 1, y) + GETF(x, y + 1) + GETF(x, y - 1))));
			}
			
		}
		
		
		residual /= (double)(xRes*yRes);
		if (i == _iterations - 1)
			printf("Pressure solver: iter=%d , res=%f \n", i, residual);
		if (residual<_accuracy) {
			printf("Pressure solver: iter=%d , converged \n", i, residual);
			//break; // optional
		}
		
		
	}
}

// Probelm 2
void ExCorrectVelocities(int xRes, int yRes, double dt, const double* field, double* xVelocity, double* yVelocity) {
	// note: velocity u_{i+1/2} is practically stored at i+1
	double dx, dy; dx = 1.0 / double(xRes);  dy = 1.0 / double(yRes);
	for (int y = 0; y < yRes - 1; y++) {
		for (int x = 0; x < xRes - 1; x++) {
			GET_X_V(x + 1, y) = GET_X_V(x + 1, y) - (dt/dx)* (GETF(x + 1, y) - GETF(x, y));
			GET_Y_V(x, y + 1) = GET_Y_V(x, y + 1) - (dt/dy)* (GETF(x, y + 1) - GETF(x, y));
		}
	}
}

// Problem 3

void ExAdvectWithSemiLagrange(int xRes, int yRes, double dt,double* xVelocity, double* yVelocity, double *density, double *densityTemp,double *xVelocityTemp,double *yVelocityTemp)
{
	// note: velocity u_{i+1/2} is practically stored at i+1
	for (int y = 0; y < yRes - 1; y++) {
		for (int x = 0; x < xRes - 1; x++) {
			double adv_x = (GET_X_V(x, y) + GET_X_V(x + 1, y)) / 2.0;
			double adv_y = (GET_Y_V(x, y) + GET_Y_V(x, y+1)) / 2.0;

			double xpos = double(x) / xRes; double ypos = double(y) / yRes;

			double p_x = xpos - dt* adv_x;
			double p_y = ypos - dt* adv_y;

			GET_2D_array(densityTemp, x, y) = bilinear_inter(p_x, p_y, density, xRes, yRes);
			GET_2D_array(xVelocityTemp, x, y) = bilinear_inter(p_x, p_y, xVelocity, xRes, yRes);
			GET_2D_array(yVelocityTemp, x, y) = bilinear_inter(p_x, p_y, yVelocity, xRes, yRes);
			
		}
	}
	
	// copy results from tmp buffers
	
	memcpy(density, densityTemp, xRes*yRes*sizeof(double));
	memcpy(xVelocity, xVelocityTemp, xRes*yRes*sizeof(double));
	memcpy(yVelocity, yVelocityTemp, xRes*yRes*sizeof(double));
}

double get_safe_2d_Array(double* arr, int x, int y, int xSize, int ySize) {
	if (x >= xSize || y >= ySize || x < 0 || y < 0) {
		return 0; // clamp
	} else {
		return arr[(x)+(y)* xSize];
	}
}
double bilinear_inter(double x, double y, double* arr, int xRes, int yRes) {
	int floor_idx_x = floor(x*xRes); int floor_idx_y = floor(y*yRes);
	
	double floor_pos_x = double(floor_idx_x) / xRes; double floor_pos_y = double(floor_idx_y) / yRes;

	double w_x = (x - floor_pos_x) * xRes;
	double w_y = (y - floor_pos_y) * yRes;

	int x0y0 = (floor_idx_x)+(floor_idx_y)*xRes; // lower left
	int x1y0 = (floor_idx_x + 1) + (floor_idx_y)*xRes; // lower right
	int x0y1 = (floor_idx_x)+(floor_idx_y + 1)*xRes; // upper left
	int x1y1 = (floor_idx_x + 1) + (floor_idx_y + 1)*xRes; // upper right
	
	
	int x0 = floor_idx_x; int x1 = floor_idx_x + 1;
	int y0 = floor_idx_y; int y1 = floor_idx_y + 1;

	// clamp if necessary
	double arr_x0_y0 = GET_2D_array(arr, x0, y0); double arr_x0_y1 = GET_2D_array(arr, x0, y1);
	double arr_x1_y0 = GET_2D_array(arr, x1, y0); double arr_x1_y1 = GET_2D_array(arr, x1, y1);

	return (1 - w_y)* ((1 - w_x)*arr_x0_y0 + w_x*arr_x1_y0) + w_y * ((1 - w_x) * arr_x0_y1 + w_x*arr_x1_y1);
}