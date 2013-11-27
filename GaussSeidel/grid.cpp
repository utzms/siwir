#include "grid.h"

Grid::Grid(int const x, int const y)
{
	
	nx = x;
	ny = y;
	
	int n = (int)((nx*ny)/2);
	
	if( ((x*y)%2) != 0 )
	{
		blackValues = new double[n];
		n++;		
		redValues = new double[n];		
	}
	else
	{
		blackValues = new double[n];		
		redValues = new double[n];		
	}

}

inline int Grid::computeGaussSeidel(size_t iterations)
{
	/*
	for(int i = 0; i < nx; i++)
	{
		for(int j = j; j < ny; j++)
		{
			int redIndex = getIndexRed(i,j);
			if(redIndex != -1)
			{
						
			}
		}
	}
	*/
	return 0;
}

inline void Grid::writeToFile(std::string filename)
{ 

}

inline int Grid::computeResidual()
{
	return 0;
}

int Grid::getIndexRed( size_t row, size_t column )
{
	//wie viele Rote sind in Zeilen bis Indexzeile (ausgeschlossen)
	int idx;
	if ( nx % 2 == 0)
	{
		idx = 0.5*nx*(row)-1;
	}
	else
	{
		idx  = (int) (0.5*nx+0.5)*row;
		idx -= (int) (0.5*(row));
		--idx;
	}
	//0 x o x o
	//1 o x o x
	//2 x o x o
	//3 o x o x

	if ((row%2) == 0 && column%2 == 0)
	{
		idx = idx + (int)(0.5*column)+1;
	}
	else if ( row%2 != 0 && column%2 !=0)
	{
		idx = idx + (int)(0.5*column+0.5);
	}
	else
	{
		return -1;
	}

	return idx;
}

int Grid::getIndexBlack( size_t row, size_t column )
{
	int idx;
	//wie viele Rote sind in Zeilen bis Indexzeile (ausgeschlossen)
	if ( nx % 2 == 0)
	{
		idx = 0.5*nx*(row)-1;
	}
	else
	{
		idx = (int) (0.5*nx+0.5)*row;
		idx = idx - (int)(0.5*row+0.5);
		--idx;
	}

	if ((row%2 == 0 && column%2 != 0))
	{
		idx = idx + (int) (0.5*column+0.5);
	}
	else if ( row%2 != 0 && column%2 == 0)
	{
		idx = idx + (int) (0.5*column)+1;
	}
	else
	{
		return -1;
	}
	return idx;
}

void Grid::setValue(int x, int y, double value)
{
	int realIndex = getIndexRed(x,y);
	if( realIndex == -1)
	{
		realIndex = getIndexBlack(x,y);
		blackValues[realIndex] = value;
	}	
	else
	{
		redValues[realIndex] = value;
	}
}
double Grid::getValue(int x, int y)
{
	int realIndex = getIndexRed(x,y);
	if( realIndex == -1)
	{
		realIndex = getIndexBlack(x,y);
		return blackValues[realIndex];
	}	
	else
	{
		return redValues[realIndex];
	}	
}

