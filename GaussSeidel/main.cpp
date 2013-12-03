#include "grid.h"
#include <iostream>
#include <sys/time.h>
#include <omp.h>
int main(int argc, char ** argv)
{

	if(argc != 4)
	{
		std::cout <<"Usage: ./rbgs gridDimensionX gridDimensionY Iterations" << std::endl;
		return -1;
	}
        //cout << "!!!Version 0.01!!!" << endl;
	int dim_x = atoi(argv[1]);
	int dim_y = atoi(argv[2]);
	int iterations = atoi(argv[3]);
	omp_set_num_threads(32);	

	Grid Grid(dim_y, dim_x);
	 
	for ( int x = 1; x < dim_x+1; ++x)
	{		
		double initialValue = sin(2.0*M_PI*(double)x*(2.0/(double)dim_x))*sinh(2.0*M_PI);
		//std::cout << initialValue << std::endl;
	 	Grid.setValue(dim_y+1,x,initialValue);
	}
//	Grid.print("BeispielTest.txt");
	
	Grid.fill_resultFxy();
	timeval start,end;	
		
	gettimeofday(&start,0);
	Grid.computeGaussSeidel(iterations);	
	gettimeofday(&end,0);
	std::cout << "wall clock time: " << ((double)(end.tv_sec*1000000 + end.tv_usec)-(double)(start.tv_sec*1000000 + start.tv_usec))/1000000 << std::endl;
	
	Grid.print("solution.txt");

	Grid.getResidual();		

        return 0;
}
