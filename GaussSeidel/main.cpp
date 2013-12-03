#include "grid.h"
#include <iostream>
#include <sys/time.h>
#include <omp.h>
int main(int argc, char ** argv)
{
        //cout << "!!!Version 0.01!!!" << endl;
	int dim_x = 2048;
	int dim_y = 2048;
	omp_set_num_threads(1);	

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
	Grid.computeGaussSeidel(500);	
	gettimeofday(&end,0);
	std::cout << "wall clock time: " << ((double)(end.tv_sec*1000000 + end.tv_usec)-(double)(start.tv_sec*1000000 + start.tv_usec))/1000000 << std::endl;
	
	Grid.print("BeispielTest.txt");

        return 0;
}
