#include "../include/grid.h"

Grid::Grid(int const x, int const y, int s , int r)
{

    size = s;
    rank = r;

	//set gridsize
    nx = x + 1;
	ny = y + 1;



	vectorSize = ((ny-2)/size + 1) * nx;
	if(rank == 0)
	{
		vectorSize_node0 = (ny-2)*nx - (size-1) * vectorSize;
		vectorSize_withGhosts_node0 = vectorSize_node0 + 2*nx;
		dim_y = vectorSize_node0/nx;
	}else{
		dim_y = vectorSize/nx;
	}

	vectorSize_withGhosts = vectorSize + 2*nx;
	dim_x = x;


	std::cout << rank << " vectorSize: " << vectorSize << " with dim_y " << dim_y << std::endl;
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

    if(rank == 0)
    {
        grid.resize(nx*ny);
        vector_g.resize(nx*ny);
		//init grid
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
    }
	if(rank == 0)
	{
		localVector_g.resize(vectorSize_withGhosts_node0);
		localVector_d.resize(vectorSize_withGhosts_node0);
		localVector_h.resize(vectorSize_withGhosts_node0);
		local_grid.resize(vectorSize_withGhosts_node0);
		for ( int i = 0; i < vectorSize_withGhosts_node0; ++i)
		{
			localVector_g[i] = 0;
			localVector_d[i] = 0;
			localVector_h[i] = 0;
			local_grid[i] = 0;
		}
	}
	else
	{
		localVector_g.resize(vectorSize_withGhosts);
		localVector_d.resize(vectorSize_withGhosts);
		localVector_h.resize(vectorSize_withGhosts);
		local_grid.resize(vectorSize_withGhosts);
		for ( int i = 0; i < vectorSize_withGhosts; ++i)
		{
			localVector_g[i] = 0;
			localVector_d[i] = 0;
			localVector_h[i] = 0;
			local_grid[i] = 0;
		}
	}
}

Grid::~Grid()
{

}

int Grid::computeConjugateGradients(int iterations, double epsilon)
{

    initAndSplit(epsilon);
    //initialize vector d

	if(rank == 0)
	{
		for ( int i = nx; i < vectorSize_node0+nx; ++i)
		{
			localVector_d[i] = -(localVector_g[i]);
		}
	}
	else
	{
		for ( int i = nx; i < vectorSize+nx; ++i)
		{
			localVector_d[i] = -(localVector_g[i]);
		}
	}

	for(int iter = 0; iter < iterations; ++iter)
    {

		  sendAndReceive_ghosts();

		  //h = Ad
		  for(int i = 1; i <= dim_y; ++i)
		  {
			  for(int j = 1; j < dim_x; ++j)
			  {
				 double tmpAx	= (2.0/(hx*hx) + 2.0/(hy*hy) + 4.0 * M_PI * M_PI)*getValue(localVector_d,i,j)
						  + (-1.0/(hy*hy))*getValue(localVector_d,i-1,j)
						  + (-1.0/(hy*hy))*getValue(localVector_d,i+1,j)
						  + (-1.0/(hx*hx))*getValue(localVector_d,i,j-1)
						  + (-1.0/(hx*hx))*getValue(localVector_d,i,j+1);
				 setValue(localVector_h,i,j,tmpAx);
			  }
		  }

		  //alpha = delta/<d,h>
		  double alpha_part = computeDotProduct(localVector_d,localVector_h);

		  double alpha_complete = 0.0;
		  MPI_Allreduce( &alpha_part, &alpha_complete, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );

		  alpha = delta / alpha_complete;

		  //x = x +  alpha * d
		  if(rank == 0)
		  {
			  for ( int i = nx; i < vectorSize_node0+nx; ++i)
			  {
				  local_grid[i] = local_grid[i] + alpha * localVector_d[i];
			  }
		  }
		  else
		  {
			  for ( int i = nx; i < vectorSize+nx; ++i)
			  {
				  local_grid[i] = local_grid[i] + alpha * localVector_d[i];
			  }
		  }


		  //g = g + alpha * g
		  if(rank == 0)
		  {
			  for ( int i = nx; i < vectorSize_node0+nx; ++i)
			  {
				  localVector_g[i] = localVector_g[i] + alpha * localVector_h[i];
			  }
		  }
		  else
		  {
			  for ( int i = nx; i < vectorSize+nx; ++i)
			  {
				  localVector_g[i] = localVector_g[i] + alpha * localVector_h[i];
			  }
		  }


		  //delta1 = <g,g>
		  double delta_part = computeDotProduct(localVector_g,localVector_g);
		  double delta_new  = 0.0;
		  MPI_Allreduce( &delta_part, &delta_new, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );

		  if(delta_new <= epsilon)
		  {
			  return 0;
		  }

		  beta = delta_new/delta;
		  //d = -g + beta * d
		  if(rank == 0)
		  {
			  for ( int i = nx; i < vectorSize_node0+nx; ++i)
			  {
				  localVector_d[i] = -localVector_g[i] + beta * localVector_d[i];
			  }
		  }
		  else
		  {
			  for ( int i = nx; i < vectorSize+nx; ++i)
			  {
				  localVector_d[i] = -localVector_g[i] + beta * localVector_d[i];
			  }
		  }

		  delta = delta_new;
    }

	 MPI_Gather((double *)&local_grid[nx], vectorSize, MPI_DOUBLE, (double *)&grid[0] , vectorSize,
	         MPI_DOUBLE, 0, MPI_COMM_WORLD );
    return 0;
}

//MPI FUNCTIONS----->
void Grid::initAndSplit(double epsilon)
{
    //initialize vector g
    if(rank == 0)
    {
		  //init resultVector_Fxy
		  fill_resultFxy();
		  //init g
		  for ( int i = 0; i < nx * ny; ++i)
		  {
			   //vector_g[i] = 0.0;
		  }
     	  getResidualVector(vector_g);

		   //set partial vector for node 0
		   for ( int i = 0; i < vectorSize_withGhosts_node0; ++i)
		   {
			   localVector_g[i] = vector_g[i];
			   local_grid[i] = grid[i];
		   }

		   //send partial vectors
           for(int currentNode = 1; currentNode < size ; currentNode++)
		   {
			   MPI_Send((double *)(&vector_g[vectorSize_node0 + (currentNode-1) * vectorSize]), vectorSize_withGhosts, MPI_DOUBLE
                          ,currentNode, 0, MPI_COMM_WORLD);
           }

		   for(int currentNode = 1; currentNode < size ; currentNode++)
           {
			   MPI_Send((double *)(&grid[vectorSize_node0 + (currentNode-1) * vectorSize]), vectorSize_withGhosts, MPI_DOUBLE
                          ,currentNode, 0, MPI_COMM_WORLD);
           }

           double result = 0.0;
           for ( int i = 0; i < nx * ny; ++i)
           {
           	 result += vector_g[i]*vector_g[i];
           }
           delta = result;

           if(delta <= epsilon)
           {
               return;
           }
			std::cout << rank << " delta: " << delta<< std::endl;
		   MPI_Bcast(&delta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status status;
		MPI_Recv( (double *)&localVector_g[0],vectorSize_withGhosts ,MPI_DOUBLE,
                  0, 0 , MPI_COMM_WORLD, &status );
		MPI_Recv( (double *)&local_grid[0] , vectorSize_withGhosts ,MPI_DOUBLE,
                  0, 0 , MPI_COMM_WORLD, &status );
		MPI_Bcast(&delta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }


}

void Grid::sendAndReceive_ghosts()
{
	MPI_Request request[2];
	MPI_Status status[2];
	MPI_Status send_status;
	
	if(rank != 0)
	{
		MPI_Issend((double *)(&localVector_d[nx]), nx, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD , &request[0]);
		std::cerr << rank << " sended " << nx/nx << std::endl;
	}
	
	if(rank != size-1)
	{
		if(rank == 0)
		{
			MPI_Issend((double *)(&localVector_d[vectorSize_withGhosts_node0-2*nx]), nx, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD , &request[1]);
			std::cerr << rank << " sended " << (vectorSize_withGhosts_node0-2*nx) /nx << std::endl;
		}
		else
		{
			MPI_Issend((double *)(&localVector_d[vectorSize_withGhosts-2*nx]), nx, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD , &request[1]);
			std::cerr << rank << " sended " << (vectorSize_withGhosts-2*nx) /nx << std::endl;
		}
	}

	std::cerr << rank << " sended" << std::endl;

	if(rank != size-1)
	{
		if(rank == 0)
		{
			MPI_Recv( (double *)&localVector_d[vectorSize_withGhosts_node0-nx] , nx ,MPI_DOUBLE, rank+1, 0 , MPI_COMM_WORLD, &send_status );
		}
		else
		{
			MPI_Recv( (double *)&localVector_d[vectorSize_withGhosts-nx] , nx ,MPI_DOUBLE, rank+1, 0 , MPI_COMM_WORLD, &send_status );
		}
		std::cerr << rank << " received " << "from " << rank +1 << std::endl;

	}

	if(rank != 0)
	{
		MPI_Recv( (double *)&localVector_d[0] , nx ,MPI_DOUBLE, rank-1, 0 , MPI_COMM_WORLD, &send_status );
		std::cerr << rank << " received from " << rank-1 << std::endl;
	}
	
	if(rank != 0)
		MPI_Wait(&request[0], &status[0]);
	
	if(rank != size-1)
		MPI_Wait(&request[1], &status[1]);
	std::cerr << rank << " continues" << std::endl;

}


//<------------MPI FUNCTIONS
void Grid::setValue(std::vector<double> & vector,int i, int j, double value)
{
    if(i >= nx || j >= ny)
    {
        std::cout << "Matrix::setValue: Position is out of range" << std::endl;
    }
    vector[i*nx + j] = value;
}

double Grid::getValue(std::vector<double> & vector, int i, int j)
{
    if(i >= nx || j >= ny)
    {
        std::cout << "Matrix::setValue: Position is out of range" << std::endl;
    }
    return vector[i*nx + j];
}

double Grid::computeDotProduct(std::vector<double> & vector_A,std::vector<double> & vector_B)
{
	double result = 0.0;
	if(rank == 0){
		for ( int i = nx; i < vectorSize_node0+nx; ++i)
		{
			result += vector_A[i]*vector_B[i];
		}
	}
	else
	{
		for ( int i = nx; i < vectorSize+nx; ++i)
		{
			result += vector_A[i]*vector_B[i];
		}
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
