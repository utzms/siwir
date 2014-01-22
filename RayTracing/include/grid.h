#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <omp.h>
#include <vector>
#include <string>
//#include <pam.h>

#include "ray.h"


class Grid
{
	private:
		double* _absorbedPower;
		double* _refractionIndex;
		double* _absorptionCoefficient;
		int		_dimx;
		int		_dimy;
		double	_hx;
		double	_hy;
		double	_offsetX;
		double	_offsetY;
		int		_maxValue;

		int		_rayCount;
		std::vector<Ray> spawnedRays;

	public:
		Grid( double P, int rays, std::string absFile, std::string refrFile );
		void castRays();
		void traceRay(Ray currentRay);

		void print(std::string filename);
};
