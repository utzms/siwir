#include "grid.h"
#include <iostream>

int main(int argc, char ** argv)
{
        //	cout << "!!!Version 0.01!!!" << endl;
	int dim_x = 16;
	int dim_y = 16;
	Grid Grid(dim_y, dim_x);
	 
	for ( int x = 1; x < dim_x+1; ++x)
	{		
		double initialValue = sin(2.0*M_PI*(double)x*(1.0/(double)dim_x))*sinh(2.0*M_PI);
		//std::cout << initialValue << std::endl;
	 	Grid.setValue(dim_y+1,x,initialValue);
	}

	for ( int i = 0;i < dim_y + 2; ++i)
	{
		for (int j = 0; j < dim_x + 2; ++j)
		{
		//if ( Grid.getIndexBlack(i,j) != -1)
			std::cout << Grid.getValue(i,j) << std::endl;
		}
	}

		
        return 0;
}
