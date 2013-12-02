#include "grid.h"

Grid::Grid(int const x, int const y)
{
	
	nx = x + 2;
	ny = y + 2;

	// SET CONSTANTS
	double hx_sq = 4 /static_cast<double>(x*x);
	double hy_sq = 1 /static_cast<double>(y*y);
	std::cout<< hx_sq <<" "<<  hy_sq << std::endl;	

	stepSizeY = 1/(double)y;
	stepSizeX = 1/(double)x; 
		
	stencil_up = 1/hy_sq;
	stencil_down = stencil_up;
	stencil_right = 1/(hx_sq);
	stencil_left = stencil_right;
	stencil_center = 1/(2/hx_sq + 2/hy_sq + 4*M_PI*M_PI);
		
	resultVectorFxy.reserve(x*y);
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

	for(int i = 0; i < ny; ++i)
	{
		for(int j = 0; j < nx; ++j)
		{
			setValue(i,j,0.0);
		}
	}

}

int Grid::computeGaussSeidel(int iterations)
{
	

	for(int iter = 0; iter < iterations; ++iter)
	{	

		for(int i = 1; i < ny-1; ++i)
		{
			for(int j = 1; j < nx-1; ++j)
			{
				int redIndex = getIndexRed(i,j);
				if(redIndex != -1)
				{
					//std::cout<< "red: " <<i<< " " <<j << std::endl;
					
					int indBlackDown = getIndexBlack(i-1,j); 
					int indBlackUp   = getIndexBlack(i+1,j);
					int indBlackRight= getIndexBlack(i,j+1);
					int indBlackLeft = getIndexBlack(i,j-1);
					//std::cout <<"    "<< indBlackUp << std::endl;
					//std::cout << indBlackLeft << "\t"; 
					//std::cout << indBlackRight << std::endl;
					//std::cout <<"    " << indBlackDown << std::endl;
					redValues[redIndex] = stencil_center*(resultVectorFxy[(i-1)*(nx-2) + (j-1)] + stencil_right*(blackValues[indBlackRight] 
					+ blackValues[indBlackLeft]) + stencil_up*(blackValues[indBlackUp] + blackValues[indBlackDown]));
				
					std::cout <<"rsv: "<< (i-1)*(nx-2) + (j-1) << std::endl;
				/*std::cout <<  blackValues[indBlackDown] << std::endl;
				std::cout <<  blackValues[indBlackUp] << std::endl;
				std::cout <<  blackValues[indBlackRight] << std::endl;
				std::cout <<  blackValues[indBlackLeft] << std::endl;
				*/
					//std::cout << redValues[redIndex] << std::endl;
				}	
			}
		}
		
		for(int i = 1; i < ny-1; ++i)
		{
			for(int j = 1; j < nx-1; ++j)
			{
				int blackIndex = getIndexBlack(i,j);
				if(blackIndex != -1)
				{
					//std::cout << "black: " <<i<< " " <<j << std::endl;
					int indRedDown = getIndexRed(i-1,j); 
					int indRedUp   = getIndexRed(i+1,j);
					int indRedRight= getIndexRed(i,j+1);
					int indRedLeft = getIndexRed(i,j-1);
					//std::cout << "    "<< indRedUp << std::endl;
					//std::cout << indRedLeft << "\t";
					//std::cout << indRedRight << std::endl;
					//std::cout <<"    "<< indRedDown << std::endl;
					blackValues[blackIndex] = stencil_center*(resultVectorFxy[(i-1)*(nx-2) + (j-1)] + stencil_right*(redValues[indRedRight] 
					+ redValues[indRedLeft]) + stencil_up*(redValues[indRedUp] + redValues[indRedDown]));
					
					std::cout <<"rsv: "<< (i-1)*(nx-2) + (j-1) << std::endl;
					/*std::cout <<  redValues[indRedDown] << std::endl;
					std::cout <<  redValues[indRedUp] << std::endl;
					std::cout <<  redValues[indRedRight] << std::endl;
					std::cout <<  redValues[indRedLeft] << std::endl;
					*/
					//std::cout << blackValues[blackIndex] << std::endl;
				}	
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
	for( int y = 1; y < ny-1; ++y) {
		for(int x = 1; x < nx-1; ++x){
			tempValueFxy = 4.0 * M_PI * M_PI * sin(2.0*M_PI*static_cast<double>(x)*stepSizeX) * sinh(2.0*M_PI*static_cast<double>(y)*stepSizeY);
			resultVectorFxy.push_back(tempValueFxy);
			std::cout << resultVectorFxy[(y-1)*(nx-2) + (x-1)] << std::endl;
		}
	}
}

void Grid::print(std::string filename)
{
	//print to file named filename
	std::ofstream outputFile(filename.c_str(), std::ios::out);
	for(int x_out = 0; x_out < nx ; ++x_out)
	{	
		for(int y_out = 0; y_out < ny-1; ++y_out)
		{
			outputFile <<	(x_out) <<	"\t"	<<	(y_out) 
				   <<	"\t"	<<	getValue(x_out, y_out) << std::endl;
		}
		outputFile << std::endl;
		
	}	
	std::endl (outputFile);
}





