#include "../include/grid.h"

Grid::Grid(int const x, int const y, int s , int r)
{

    size = s;
    rank = r;

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
    fill_resultFxy();
    grid.resize(nx*ny);
    for ( int i = 0; i < nx * ny; ++i)
    {
        grid[i] = 0.0;
    }

    //initialize last row
    for ( int i = 0; i < x+1; ++i)
    {
        double initialValue = sin(2.0*M_PI*(double)i*(2.0/(double)x))*sinh(2.0*M_PI);
        setValue(grid ,y,i,initialValue);
    }

    vector_g.resize(nx*ny);
    vector_d.resize(nx*ny);
    vector_h.resize(nx*ny);

}

Grid::~Grid()
{

}

int Grid::computeConjugateGradients(int iterations, double epsilon)
{
    //initialize vector g
    getResidualVector(vector_g);
    //initialize vector d
    for ( int i = 0; i < nx * ny; ++i)
    {
        vector_d[i] = -(vector_g[i]);
    }
    delta = computeDotProduct(vector_g,vector_g);

    if(delta <= epsilon)
    {
        return 0;
    }

	for(int iter = 0; iter < iterations; ++iter)
    {
        //h = Ad
        for(int i = 1; i < ny-1; ++i)
        {
            for(int j = 1; j < nx-1; ++j)
            {
               double tmpAx	= (2.0/(hx*hx) + 2.0/(hy*hy) + 4.0 * M_PI * M_PI)*getValue(vector_d,i,j)
                        + (-1.0/(hy*hy))*getValue(vector_d,i-1,j)
                        + (-1.0/(hy*hy))*getValue(vector_d,i+1,j)
                        + (-1.0/(hx*hx))*getValue(vector_d,i,j-1)
                        + (-1.0/(hx*hx))*getValue(vector_d,i,j+1);
               setValue(vector_h,i,j,tmpAx);
            }
        }

        //alpha = delta/<d,h>
        alpha = delta / computeDotProduct(vector_d,vector_h);

        //x = x +  alpha * d
        for ( int i = 0; i < nx * ny; ++i)
        {
            grid[i] = grid[i] + alpha * vector_d[i];
        }

        //g = g + alpha * g
        for ( int i = 0; i < nx * ny; ++i)
        {
            vector_g[i] = vector_g[i] + alpha * vector_h[i];
        }

        //delta1 = <g,g>
        double delta_new = computeDotProduct(vector_g,vector_g);

        if(delta_new <= epsilon)
        {
            return 0;
        }


        beta = delta_new/delta;
        //d = -g + beta * d
        for ( int i = 0; i < nx * ny; ++i)
        {
            vector_d[i] = -vector_g[i] + beta * vector_d[i];
        }
        delta = delta_new;
    }
		
    return 0;
}

void Grid::setValue(std::vector<double> & vector,int i, int j, double value)
{
    if(i >= nx || j >= ny)
    {
        std::cout << "Matrix::setValue: Position is out of range" << std::endl;
    }
    vector[i*ny + j] = value;
}

double Grid::getValue(std::vector<double> & vector, int i, int j)
{
    if(i >= nx || j >= ny)
    {
        std::cout << "Matrix::setValue: Position is out of range" << std::endl;
    }
    return vector[i*ny + j];
}

double Grid::computeDotProduct(std::vector<double> & vector_A,std::vector<double> & vector_B)
{
    double result = 0.0;
    for ( int i = 0; i < nx * ny; ++i)
    {
        result += vector_A[i]*vector_B[i];
    }
    return result;
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
                    resultVector_Fxy.push_back(tempValueFxy);
		}
	}
}

void Grid::print(std::string filename)
{
	//print to file named filename
	std::ofstream outputFile(filename.c_str(), std::ios::out);
    for(int x_out = 0; x_out < nx ; ++x_out)
	{	
        for(int y_out = 0; y_out < ny ; ++y_out)
		{
			outputFile <<	(x_out)*hx <<	"\t"	<<	(y_out)*hy 
                   <<	"\t"	<<	getValue(grid,x_out, y_out) << std::endl;
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
            tmpAx 	= (2.0/(hx*hx) + 2.0/(hy*hy) + 4.0 * M_PI * M_PI)*getValue(grid,i,j)
                    + (-1.0/(hy*hy))*getValue(grid,i-1,j)
                    + (-1.0/(hy*hy))*getValue(grid,i+1,j)
                    + (-1.0/(hx*hx))*getValue(grid,i,j-1)
                    + (-1.0/(hx*hx))*getValue(grid,i,j+1);
            tmp_diff = tmpAx - resultVector_Fxy[ i * nx + j ];
			l2_sum += (tmp_diff * tmp_diff);
		}
	}
	double residual =  sqrt( l2_sum * hx * hy  ); 
	std::cout << "residual: " << residual  << std::endl;	
}

void Grid::getResidualVector(std::vector<double> & vector)
{
    double tmpAx = 0.0;
    double tmp_diff = 0.0;

    //compute and output residual
    for(int i = 1; i < ny-1; ++i){
        for(int j = 1; j < nx-1; ++j){
            tmpAx 	= (2.0/(hx*hx) + 2.0/(hy*hy) + 4.0 * M_PI * M_PI)*getValue(grid,i,j)
                    + (-1.0/(hy*hy))*getValue(grid,i-1,j)
                    + (-1.0/(hy*hy))*getValue(grid,i+1,j)
                    + (-1.0/(hx*hx))*getValue(grid,i,j-1)
                    + (-1.0/(hx*hx))*getValue(grid,i,j+1);
            tmp_diff = tmpAx - resultVector_Fxy[ i * nx + j ];
            setValue(vector,i,j,tmp_diff);
        }
    }
}
