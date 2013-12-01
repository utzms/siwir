#include "matrix.h"
extern bool _sse_ON;
extern bool blocking;
double Matrix::getValueAt(size_t i, size_t j)
{
	return dataPointer[i*ndim + j];
}

size_t Matrix::getNdim()
{
	return ndim;
}

size_t Matrix::getMdim()
{ 
	return mdim;
}

void Matrix::setValue(int m, int n, double value)
{
	if(m >= mdim || n >= ndim)
	{
		std::cout << "Matrix::setValue: Position is out of range" << std::endl;
	}
	dataPointer[m*ndim + n] = value;
}

bool Matrix::checkIfPowerOfTwo(size_t M, size_t N)
{
	//check if matrix is quadratic
	size_t i = 2;
	while(i <= M || i <= N)
	{
		if(M == i && N == i)
			return true;
		i*=2;
	}	
	return false;
}

void Matrix::setIdentity()
{
	for(size_t i = 0; i < size; ++i)
	{
		dataPointer[i] = 0;
	}

	int count = 0;
	for(size_t i = 0; i < size; i+=ndim)
	{
		dataPointer[i + count] = 1;
		count++;
	}
}

void Matrix::print()
{
	std::cout << "Dim: " << mdim << " " << ndim << std::endl;
	for(size_t i = 0; i < size; ++i)
	{
		if((i % (ndim)) == 0 && i != 0){ std::endl (std::cout); }
		std::cout << dataPointer[i] << ' ';
	}
	std::endl (std::cout);
}

Matrix::Matrix(const int m,const int n)
{
	ndim = static_cast<size_t>(n);
	mdim = static_cast<size_t>(m);
	size=n*m; 
	dataPointer = new double[size];
}

Matrix::Matrix(const int m, const int n, double * datapointer,const int  ld)
{
	ndim = static_cast<size_t>(n);
	mdim = static_cast<size_t>(m);
	size=n*m; 
	dataPointer = new double[size]; 
	for(int i = 0;i < m; i++)
	{
		for(int j = 0 ;j < n; j++)
		{
			dataPointer[i*ndim + j] = datapointer[i*ld + j];	
		}
	}
}


Matrix::Matrix(std::string filename)
{
	//read in matrix from file
	std::ifstream inputFile(filename.c_str(), std::ios::in);
	int ntmp;
	int mtmp;

	inputFile >> mtmp;
	inputFile >> ntmp;
	ndim = static_cast<size_t>(ntmp);
	mdim = static_cast<size_t>(mtmp);
	size=ndim*mdim;

	dataPointer = new double[size];
	for(size_t i = 0;i < size;)
	{
		inputFile >> dataPointer[i++];
	}

}



double* Matrix::createLinArray()
{
	double* linArray = new double[size];
	return linArray;
}


void Matrix::print(std::string filename)
{
	//print to file named filename
	std::ofstream outputFile(filename.c_str(), std::ios::out);
	outputFile <<  mdim << " " << ndim << std::endl;
	for(size_t i = 0; i < size; ++i)
	{
		outputFile << dataPointer[i] << std::endl;
	}
	std::endl (outputFile);
}

Matrix& Matrix::operator* (Matrix MatrixB)
{
	//multiplication operator without optimizations
	Matrix * MatrixC = new Matrix(mdim,MatrixB.ndim); 

	if(ndim != MatrixB.mdim)
	{
		std::cout << "Matrix::operator*: Inner Dimensions don't match" << std::endl;
	}

	for(size_t i = 0; i < mdim ; ++i)
	{
		for(size_t j = 0; j < MatrixB.ndim; ++j)
		{
			double rowSum = 0;
			for(size_t rowi = 0; rowi < ndim; ++rowi)
			{
				rowSum += (dataPointer[i*ndim + rowi] * MatrixB.dataPointer[j + rowi * MatrixB.ndim]);
			}
			MatrixC->dataPointer[i*MatrixC->ndim + j] = rowSum;
		}
	}
	return *MatrixC;
}

void Matrix::split(std::vector<Matrix> subMat)
{
	size_t newDim = ndim*0.5;
	for(size_t i = 0; i < newDim ; ++i)
	{
		for(size_t j = 0; j < newDim ;++j)
		{
			subMat[0].dataPointer[i * newDim + j] = dataPointer[i * ndim + j];
			subMat[1].dataPointer[i * newDim + j] = dataPointer[i * ndim + j + (newDim)];
			subMat[2].dataPointer[i * newDim + j] = dataPointer[i * ndim + j + (ndim*newDim)];
			subMat[3].dataPointer[i * newDim + j] = dataPointer[i * ndim + j + (ndim*newDim+newDim)];
		}
	}	
}

void Matrix::sumOf(Matrix & A, Matrix & B)
{
	for(size_t i = 0; i < A.size; ++i)
	{
		dataPointer[i] = A.dataPointer[i] + B.dataPointer[i];
	}
}

void Matrix::diffOf(Matrix & A, Matrix & B)
{
	for(size_t i = 0; i < A.size; ++i)
	{
		dataPointer[i] = A.dataPointer[i] - B.dataPointer[i];
	}
}




void Matrix::matmult(Matrix& A, Matrix& B, Matrix& C)
{
	//bool _sse_ON = false;// TURN SSE ON/OFF

	__m128d _vec_1, _vec_2, _vec_3 ;
	double _vec_store[2] = {0.0, 0.0};

	//optimal threshold for using native multiplication:128
	if( A.ndim < 128  && B.ndim < 128  )
	{
		int blocksize=8;
		//bool blocking = false;
		if (blocking) {
			Matrix Btrans(B.ndim,B.mdim);
			//transpone Matrix B
			for (size_t i = 0; i < B.ndim ; ++i)
			{
				for (size_t j = 0; j < B.mdim ; ++j)
				{
					Btrans.dataPointer[i*B.ndim+j] = B.dataPointer[j*B.ndim+i];	
				}
			}
			//perform multiplication with optimizations
			for(size_t i = 0; i < A.mdim ; ++i)
			{       
				for(size_t j = 0; j < B.ndim; j++)
				{
					double rowSum = 0;
					for(size_t rowi = 0; rowi < A.ndim; rowi+=blocksize)
					{
						if(_sse_ON){	
							//Opimization 1: Cache Blocking 
							for(int k = 0; k < blocksize;k += 2)
							{	

								//Opimization 2: SSE Intrinsics
								_vec_1 = _mm_loadu_pd( &A.dataPointer[i*A.ndim + rowi + k] );
								_vec_2 = _mm_loadu_pd( &Btrans.dataPointer[(j)*Btrans.ndim + rowi + k] );
								_vec_3 = _mm_mul_pd( _vec_1, _vec_2);
								_mm_store_pd(_vec_store, _vec_3);
								rowSum += _vec_store[0] + _vec_store[1];
							}
						}else{
							for(int k = 0; k < blocksize;k += 1)
							{	
								rowSum += (A.dataPointer[i*A.ndim + rowi + k] * Btrans.dataPointer[(j)*Btrans.ndim + rowi + k]);
							}

						}
						C.dataPointer[i*C.ndim + j]  = rowSum;
					}
				}
			}
			return;

		}else{
			//perform multiplication without optimization
			for(size_t i = 0; i < A.mdim ; ++i)
			{	
				for(size_t j = 0; j < B.ndim; ++j)
				{
					double rowSum = 0;
					for(size_t rowi = 0; rowi < B.ndim; ++rowi)
					{
						rowSum += (A.dataPointer[i*A.ndim + rowi] * B.dataPointer[j + rowi * B.ndim]);
					}
					C.dataPointer[i*C.ndim + j] = rowSum;
				}
			}
			return;
		}
	}
	size_t newDim = A.ndim*0.5;
	
	//create submatrices
	std::vector<Matrix> subMatA;
	std::vector<Matrix> subMatB;
	subMatA.push_back(Matrix(newDim,newDim));
	subMatA.push_back(Matrix(newDim,newDim));
	subMatA.push_back(Matrix(newDim,newDim));
	subMatA.push_back(Matrix(newDim,newDim));
	subMatB.push_back(Matrix(newDim,newDim));
	subMatB.push_back(Matrix(newDim,newDim));
	subMatB.push_back(Matrix(newDim,newDim));
	subMatB.push_back(Matrix(newDim,newDim));
	
	//fill submatrices with values from A,B
	A.split(subMatA);
	B.split(subMatB);
	
	//create strassen Matrices M1-M7 and compute sums/differences of submatrices
	Matrix M1(newDim, newDim);	
	Matrix sumA1A4(newDim, newDim);	
	sumA1A4.sumOf(subMatA[0] , subMatA[3]);
	Matrix sumB1B4(newDim, newDim);	
	sumB1B4.sumOf(subMatB[0] , subMatB[3]);

	Matrix M2(newDim, newDim);
	Matrix sumA3A4(newDim, newDim);
	sumA3A4.sumOf(subMatA[2],subMatA[3]);

	Matrix M3(newDim, newDim);
	Matrix diffB2B4(newDim, newDim);
	diffB2B4.diffOf(subMatB[1],subMatB[3]);

	Matrix M4(newDim, newDim);
	Matrix diffB3B1(newDim, newDim);
	diffB3B1.diffOf(subMatB[2],subMatB[0]);

	Matrix M5(newDim, newDim);
	Matrix sumA1A2(newDim, newDim);
	sumA1A2.sumOf(subMatA[0],subMatA[1]);

	Matrix M6(newDim, newDim);
	Matrix diffA3A1(newDim, newDim);
	diffA3A1.diffOf(subMatA[2],subMatA[0]);
	Matrix sumB1B2(newDim, newDim);
	sumB1B2.sumOf(subMatB[0],subMatB[1]);

	Matrix M7(newDim, newDim);
	Matrix diffA2A4(newDim, newDim);
	diffA2A4.diffOf(subMatA[1],subMatA[3]);
	Matrix sumB3B4(newDim, newDim);
	sumB3B4.sumOf(subMatB[2],subMatB[3]);
	
	//recursive call of strassen algorithm
	matmult(sumA1A4, sumB1B4, M1);
	matmult(sumA3A4, subMatB[0], M2);
	matmult(subMatA[0], diffB2B4, M3);
	matmult(subMatA[3], diffB3B1, M4);
	matmult(sumA1A2, subMatB[3], M5 );
	matmult(diffA3A1, sumB1B2, M6);
	matmult(diffA2A4,sumB3B4, M7);
	
	for(size_t i = 0; i < newDim ; ++i){
		for(size_t j = 0; j < newDim ;++j)
		{
			C.dataPointer[i * C.ndim + j] = M1.dataPointer[i * newDim + j] + M4.dataPointer[i * newDim + j] - M5.dataPointer[i * newDim + j] + M7.dataPointer[i * newDim + j];
			C.dataPointer[i * C.ndim + j + newDim] = M3.dataPointer[i * newDim + j] + M5.dataPointer[i * newDim + j];
			C.dataPointer[i * C.ndim + j + (C.ndim*newDim)] = M2.dataPointer[i * newDim + j] + M4.dataPointer[i * newDim + j];
			C.dataPointer[i * C.ndim + j + (C.ndim*newDim+newDim)] = M1.dataPointer[i * newDim + j] - M2.dataPointer[i * newDim + j] + M3.dataPointer[i * newDim + j] + M6.dataPointer[i * newDim + j];
		}
	}
}

