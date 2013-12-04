#include "grid.h"

Grid::Grid(int const x, int const y)
{
	
	nx = x + 1;
	ny = y + 1;

	// SET CONSTANTS
	double hx_sq = 4.0 /static_cast<double>(x*x);
	double hy_sq = 1.0 /static_cast<double>(y*y);
	
	hx = 2.0/static_cast<double>(x);
	hy = 1.0/static_cast<double>(y);
	 	
	stencil_up = 1.0/hy_sq;
	stencil_down = stencil_up;
	stencil_right = 1.0/(hx_sq);
	stencil_left = stencil_right;
	stencil_center = 1.0/(2.0/hx_sq + 2.0/hy_sq + 4.0*M_PI*M_PI);
		
	int n = (int)((nx*ny)/2);
	
	if( ((x*y)&1) != 0 )
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

	for(int i = 0; i < ny; ++i)
	{
		for(int j = 0; j < nx; ++j)
		{
			setValue(i,j,0.0);
		}
	}

}

Grid::~Grid()
{
delete[] blackValues;
delete[] redValues;

}

int Grid::computeGaussSeidel(int iterations)
{
	

	for(int iter = 0; iter < iterations; ++iter)
	{	
		#pragma omp parallel for schedule (dynamic)
		for(int i = 1; i < ny-1; ++i)
		{
			for(int j = 1; j < nx-1; ++j)
			{
				int redIndex = getIndexRed(i,j);
				if(redIndex != -1)
				{
					
					int indBlackDown = getIndexBlack(i-1,j); 
					int indBlackUp   = getIndexBlack(i+1,j);
					int indBlackRight= getIndexBlack(i,j+1);
					int indBlackLeft = getIndexBlack(i,j-1);
					
					redValues[redIndex] = stencil_center*(resultVectorFxy[i*nx + j] 
							    + stencil_right*(blackValues[indBlackRight] 
							    + blackValues[indBlackLeft]) 
							    + stencil_up*(blackValues[indBlackUp] 
							    + blackValues[indBlackDown]));
				
				}	
			}
		}
		
		#pragma omp parallel for schedule (dynamic)
		for(int i = 1; i < ny-1; ++i)
		{
			for(int j = 1; j < nx-1; ++j)
			{
				int blackIndex = getIndexBlack(i,j);
				if(blackIndex != -1)
				{
					int indRedDown = getIndexRed(i-1,j); 
					int indRedUp   = getIndexRed(i+1,j);
					int indRedRight= getIndexRed(i,j+1);
					int indRedLeft = getIndexRed(i,j-1);
					
					blackValues[blackIndex] = stencil_center*(resultVectorFxy[i*nx + j] 
								+ stencil_right*(redValues[indRedRight] 
								+ redValues[indRedLeft]) 
								+ stencil_up*(redValues[indRedUp] 
								+ redValues[indRedDown]));					
				}	
			}		
		}
		
	}
	return 0;
}


int Grid::getIndexRed( size_t row, size_t column )
{
	//wie viele Rote sind in Zeilen bis Indexzeile (ausgeschlossen)
	int idx;
	if ( (nx&1) == 0)
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

	if ((row&1) == 0 && (column&1) == 0)
	{
		idx = idx + (int)(0.5*column)+1;
	}
	else if ( (row&1) != 0 && (column&1) !=0)
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
	if ( (nx&1) == 0)
	{
		idx = 0.5*nx*(row)-1;
	}
	else
	{
		idx = (int) (0.5*nx+0.5)*row;
		idx = idx - (int)(0.5*row+0.5);
		--idx;
	}

	if (((row&1) == 0 && (column&1) != 0))
	{
		idx = idx + (int) (0.5*column+0.5);
	}
	else if ( (row&1) != 0 && (column&1) == 0)
	{
		idx = idx + (int) (0.5*column)+1;
	}
	else
	{
		return -1;
	}
	return idx;
}

void Grid::setValue(int i, int j, double value)
{
	int realIndex = getIndexRed(i,j);
	if( realIndex == -1)
	{
		realIndex = getIndexBlack(i,j);
		blackValues[realIndex] = value;
	}	
	else
	{
		redValues[realIndex] = value;
	}
}

double Grid::getValue(int i, int j)
{
	int realIndex = getIndexRed(i,j);
	if( realIndex == -1)
	{	
		realIndex = getIndexBlack(i,j);
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
			tempValueFxy 	= 4.0 * M_PI * M_PI 
					* sin(2.0*M_PI*static_cast<double>(x)*hx) 
					* sinh(2.0*M_PI*static_cast<double>(y)*hy);
			resultVectorFxy.push_back(tempValueFxy);
			//std::cout << resultVectorFxy[(y-1)*(nx-2) + (x-1)] << std::endl;
		}
	}
}

void Grid::print(std::string filename)
{
	//print to file named filename
	std::ofstream outputFile(filename.c_str(), std::ios::out);
	for(int x_out = 0; x_out < nx ; ++x_out)
	{	
		for(int y_out = 0; y_out < ny; ++y_out)
		{
			outputFile <<	(x_out)*hx <<	"\t"	<<	(y_out)*hy 
				   <<	"\t"	<<	getValue(x_out, y_out) << std::endl;
		}
		outputFile << std::endl;
		
	}	
	std::endl (outputFile);
}

void Grid::getResidual()
{
	
	double l2_sum = 0.0;

	double tmpAx = 0.0;
	double tmp_diff = 0.0;

	for( int i = 1; i < ny-1; ++i) {
		for(int j = 1; j < nx-1; ++j){
			std::cout << "i,j: " << i << " " << j << std::endl;
			tmpAx 	= (2.0/((hx*hx) + (2.0/hy*hy) + 4.0 * M_PI * M_PI))*getValue(i,j) 
					+ (-1.0/(hy*hy))*getValue(i-1,j) 
					+ (-1.0/(hy*hy))*getValue(i+1,j) 
					+ (-1.0/(hx*hx))*getValue(i,j-1) 
					+ (-1.0/(hx*hx))*getValue(i,j+1); 
			tmp_diff = tmpAx - resultVectorFxy[ i * nx + j ];  
			l2_sum += (tmp_diff * tmp_diff);
			
		}
	}

	double residual =  sqrt( l2_sum *hx *hy  ); 

	std::cout << "residual: " << residual  << std::endl;	
}
