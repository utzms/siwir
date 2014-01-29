#include "../include/grid.h"
#include <stdexcept>
#include <sys/time.h>


int main( int argc, char** argv)
{

	if(argc != 5)
	{
		throw std::invalid_argument("usage: ./raytr P nRays refrIdx absCoeff");
	}

	double P = atof(argv[1]);
	int nRays = atoi(argv[2]);

    omp_set_num_threads(32);
    Grid grid(P, nRays, argv[4], argv[3]);



    //compute RayTracing
    timeval start,end;
    gettimeofday(&start,0);
    grid.castRays();
    gettimeofday(&end,0);
    double time = ((double)(end.tv_sec*1000000 + end.tv_usec)-(double)(start.tv_sec*1000000 + start.tv_usec))/1000000;

    //print wall clock time and residual
    std::cout << "wall clock time: " << time << std::endl;


	grid.print("absorption.pgm");

	return 0;
}
