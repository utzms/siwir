#include "grid.h"

Grid::Grid(size_t x, size_t y)
{

}

inline int Grid::computeGaussSeidel(size_t iterations)
{
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


