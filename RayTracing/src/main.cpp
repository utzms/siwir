#include "../include/grid.h"
#include <stdexcept>


int main( int argc, char** argv)
{

	if(argc != 5)
	{
		throw std::invalid_argument("usage: ./raytr P nRays refrIdx absCoeff");
	}

	double P = atof(argv[1]);
	int nRays = atoi(argv[2]);


    Grid grid(P, nRays, argv[3], argv[4]);
	//grid.castRays();
	grid.print("absorption.pgm");

	return 0;
}
