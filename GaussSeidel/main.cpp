#include "grid.h"
#include <iostream>

int main(int argc, char ** argv)
{
        //	cout << "!!!Version 0.01!!!" << endl;
	int row = 4;
	int column = 4;
	Grid Grid(row, column);
	 
	//int size = row*column;
	double counter = 1;
	for ( int i = 0; i < column; ++i)
	{
		for ( int j =0; j < row; ++j)
		{		
			Grid.setValue(i,j,counter);
			
		       // std::cout << Grid.getValue(i,j) << std::endl;
			++counter;	
		}
	}

	for ( int i = 0;i < column; ++i)
	{
		for (int j = 0; j < row; ++j)
		{
		if ( Grid.getIndexBlack(i,j) != -1)
			std::cout << Grid.getValue(i,j) << std::endl;
		}
	}

		
        return 0;
}
