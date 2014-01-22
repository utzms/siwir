#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

class Grid
{
	private:
		double * _absorbedPower;
		double * _refractionIndex;
		double * _absorptionCoefficient;
		int _NX;
		int _NY;
		double _hx;
		double _hy;
		double _offsetX;
		double _offsetY;

	public:
		Grid();

};
