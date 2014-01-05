#include "../include/grid.h"

Grid::Grid(int const x, int const y)
{
	
	//set gridsize
	nx = x + 1;
	ny = y + 1;

	//set stepsize hx,hy
	hx = 2.0/static_cast<double>(x);
	hy = 1.0/static_cast<double>(y);
	
	//compute and set stencils for rbgs
	double hx_sq = 4.0 /static_cast<double>(x*x);
	double hy_sq = 1.0 /static_cast<double>(y*y);
	 	
	stencil_up = 1.0/hy_sq;
	stencil_down = stencil_up;
	stencil_right = 1.0/(hx_sq);
	stencil_left = stencil_right;
	stencil_center = 1.0/(2.0/hx_sq + 2.0/hy_sq + 4.0*M_PI*M_PI);

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
		//compute red values
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

		//compute black values
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

void Grid::setValue(int i, int j, double value)
{

}

double Grid::getValue(int i, int j)
{

}

void Grid::fill_resultFxy()
{
	//initialize result vector(right hand side) by given function
	double tempValueFxy = 0;
	for( int y = 0; y < ny; ++y) {
		for(int x = 0; x < nx; ++x){
			tempValueFxy 	= 4.0 * M_PI * M_PI 
					* sin(2.0*M_PI*static_cast<double>(x)*hx) 
					* sinh(2.0*M_PI*static_cast<double>(y)*hy);
                    resultVectorFxy.push_back(tempValueFxy);
		}
	}
}

void Grid::print(std::string filename)
{
	//print to file named filename
	std::ofstream outputFile(filename.c_str(), std::ios::out);
	for(int x_out = 0; x_out < nx-1 ; ++x_out)
	{	
		for(int y_out = 0; y_out < ny-1 ; ++y_out)
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
	
	//compute and output residual
	for(int i = 1; i < ny-1; ++i){
		for(int j = 1; j < nx-1; ++j){
			tmpAx 	= (2.0/(hx*hx) + 2.0/(hy*hy) + 4.0 * M_PI * M_PI)*getValue(i,j) 
					+ (-1.0/(hy*hy))*getValue(i-1,j) 
					+ (-1.0/(hy*hy))*getValue(i+1,j) 
					+ (-1.0/(hx*hx))*getValue(i,j-1) 
					+ (-1.0/(hx*hx))*getValue(i,j+1); 
			tmp_diff = tmpAx - resultVectorFxy[ i * nx + j ];  
			l2_sum += (tmp_diff * tmp_diff);
		}
	}
	double residual =  sqrt( l2_sum * hx * hy  ); 
	std::cout << "residual: " << residual  << std::endl;	
}
