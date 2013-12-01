#include "grid.h"

Grid::Grid(int const x, int const y)
{
	
	nx = x + 1;
	ny = y + 1;

	// SET CONSTANTS
	double hx_sq = 4 /(x*x);
	double hy_sq = 1 /(y*y);

	stencil_up = -1/hy_sq;
	stencil_down = stencil_up;
	stencil_right = -1/(hx_sq);
	stencil_left = stencil_right;
	stencil_center = 1/(2/hx_sq + 2/hy_sq + 2*M_PI);
	
	solutionVectorX.reserve(nx*ny);
	resultVectorFxy.resize(nx*ny);
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

int Grid::computeGaussSeidel(size_t iterations)
{
	
	
	for(int i = 1; i < ny-1; i++)
	{
		for(int j = 1; j < nx-1; j++)
		{
			int redIndex = getIndexRed(i,j);
			if(redIndex != -1)
			{
				int indBlackDown = getIndexBlack(i-1,j); 
				int indBlackUp   = getIndexBlack(i+1,j);
				int indBlackRight= getIndexBlack(i,j-1);
				int indBlackLeft = getIndexBlack(i,j+1);
				redValues[redIndex] = stencil_center*(resultVectorFxy[i*(nx-1) + j] + stencil_right*(blackValues[indBlackRight] 
				+ blackValues[indBlackLeft]) + stencil_up*(blackValues[indBlackUp] + blackValues[indBlackDown]));
			}	
		}
	}

	
	for(int i = 1; i < ny-1; i++)
	{
		for(int j = 1; j < nx-1; j++)
		{
			int blackIndex = getIndexBlack(i,j);
			if(blackIndex != -1)
			{
				int indRedDown = getIndexRed(i-1,j); 
				int indRedUp   = getIndexRed(i+1,j);
				int indRedRight= getIndexRed(i,j-1);
				int indRedLeft = getIndexRed(i,j+1);
				blackValues[blackIndex] = stencil_center*(resultVectorFxy[i*nx + j] + stencil_right*(redValues[indRedRight] 
				+ redValues[indRedLeft]) + stencil_up*(redValues[indRedUp] + redValues[indRedDown]));
			}	
		}		
	}
	
	return 0;
}

void Grid::writeToFile(std::string filename)
{ 

}

int Grid::computeResidual()
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

void Grid::fill_resultFxy()
{
	double tempValueFxy = 0;
	for( int y = 0; y < ny; ++y) {
		for(int x = 0; x < nx; ++x){
			tempValueFxy = 4 * M_PI * M_PI * sin(2*M_PI*x) * sinh(2*M_PI*y);
			resultVectorFxy.push_back(tempValueFxy);
		}
	}
}




