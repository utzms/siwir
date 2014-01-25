#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <omp.h>
#include <vector>
#include <string>
#include <math.h>
//#include <pam.h>

#include "ray.h"


class Grid
{
	private:

        //grids
		double* _absorbedPower;
		double* _refractionIndex;
		double* _absorptionCoefficient;

        //grid variables
		int		_dimx;
		int		_dimy;
		double	_hx;
		double	_hy;
		double	_offsetX;
		double	_offsetY;
		int		_maxValue;

        //ray variables
		int		_rayCount;
        double  _initialPower;
        int		_activeRays;

		std::vector<Ray> spawnedRays;

	public:
        Grid( double P, int rays, char* absFile, char* refrFile );
		void castRays();
		void traceRay(Ray currentRay);

		void print(std::string filename);
};
