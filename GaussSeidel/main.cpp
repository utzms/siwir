#include "grid.h"
#include <iostream>
#include <sys/time.h>
#include <omp.h>
#include <string.h>
int main(int argc, char ** argv)
{

	//input handling
	if(argc < 4 || argc > 5)
	{
		std::cout <<"Usage: ./rbgs gridDimensionX gridDimensionY Iterations" << std::endl;
		return -1;
	}

	int dim_x = atoi(argv[1]);
	int dim_y = atoi(argv[2]);
	int iterations = atoi(argv[3]);
	std::string log("-log");
	
	int cpu = 0;
	//omp_set_num_threads(32);
	#pragma omp parallel

	{
		
		

		if (omp_get_thread_num()== 0)
		{
			cpu=omp_get_num_threads();
			
		}
		
	}
	 
		
	
	//create Grid and initialize result vector (Fxy)
	Grid Grid(dim_x, dim_y);
	Grid.fill_resultFxy();
	
	//initialize last row
	for ( int x = 0; x < dim_x+1; ++x)
	{		
		double initialValue = sin(2.0*M_PI*(double)x*(2.0/(double)dim_x))*sinh(2.0*M_PI);
	 	Grid.setValue(dim_y,x,initialValue);
	}
	
	
	
	//compute Gauss Seidel + time meassurement 
	timeval start,end;	
	gettimeofday(&start,0);
	Grid.computeGaussSeidel(iterations);	
	gettimeofday(&end,0);
	double time = ((double)(end.tv_sec*1000000 + end.tv_usec)-(double)(start.tv_sec*1000000 + start.tv_usec))/1000000;
	
	//print wall clock time and residual
	std::cout << "wall clock time: " << time << std::endl;
	Grid.getResidual();		
	
	//log time if -log option set	
	if( argc >= 5 && (log.compare(std::string(argv[4]))) == 0){
		std::ofstream logFile("log.csv",std::ios::out|std::ios::app);
		logFile << argv[1] <<"," << cpu  << ","<< time << std::endl;
	}

	// write solution to file
	Grid.print("solution.txt");

        return 0;
}
