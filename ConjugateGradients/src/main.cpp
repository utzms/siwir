#include "../include/grid.h"
#include <iostream>
#include <sys/time.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char ** argv)
{

	//input handling
    if(argc < 5 || argc > 6)
	{
        std::cout <<"Usage: ./rbgs gridDimensionX gridDimensionY Iterations epsilon" << std::endl;
		return -1;
	}

	int dim_x = atoi(argv[1]);
	int dim_y = atoi(argv[2]);
	int iterations = atoi(argv[3]);
    double epsilon = atoi(argv[4]);
	std::string log("-log");
    MPI_Init( int* argc, char*** argv );
	int cpu = 0;
	
	//create Grid and initialize result vector (Fxy)
    Grid Grid(dim_x, dim_y);
	
	
	
	//compute Gauss Seidel + time meassurement 
	timeval start,end;	
	gettimeofday(&start,0);
    Grid.computeConjugateGradients(iterations,epsilon);
	gettimeofday(&end,0);
	double time = ((double)(end.tv_sec*1000000 + end.tv_usec)-(double)(start.tv_sec*1000000 + start.tv_usec))/1000000;
	
	//print wall clock time and residual
	std::cout << "wall clock time: " << time << std::endl;
    //Grid.getResidual();
	
	//log time if -log option set	
	if( argc >= 5 && (log.compare(std::string(argv[4]))) == 0){
		std::ofstream logFile("log.csv",std::ios::out|std::ios::app);
		logFile << argv[1] <<"," << cpu  << ","<< time << std::endl;
	}

	// write solution to file
    Grid.print("solution.txt");

    return 0;
}
