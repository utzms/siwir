#include "matrix.h"

Matrix::Matrix(const int n,const int m)
	{
	//static int count = 0;
        ndim=n;
        mdim=m;
        size=n*m; 
        dataPointer = createLinArray();
	//std::cout << count << std::endl;
	//count++;
    }

Matrix::Matrix(const int n, const int m, double * datapointer)
	{
        ndim=n;
        mdim=m;
        size=n*m; 
        ;
    }


Matrix::Matrix(std::string filename)
    {

		std::ifstream inputFile(filename.c_str(), std::ios::in);
        std::string dimensions;
        int ntmp;
        int mtmp;

		inputFile >> mtmp;
        inputFile >> ntmp;
		ndim = static_cast<size_t>(ntmp);
		mdim = static_cast<size_t>(mtmp);
		size=ndim*mdim;
        /* extract matrix dimensions
        int space = dimensions.find(" ");
        std::string nDimString = dimensions.substr(0,space);
        std::string mDimString =  dimensions.substr(space+1,dimensions.length());
        ndim = atoi(nDimString.c_str());
        mdim = atoi(mDimString.c_str());
        */
        dataPointer = createLinArray();

        for(size_t i = 0;i < size;)
        {
            inputFile >> dataPointer[i++];
        }

    }

	void Matrix::setValue(size_t m, size_t n, double value)
	{
		if(m >= mdim || n >= ndim)
		{
			std::cout << "Matrix::setValue: Position is out of range" << std::endl;
		}
		dataPointer[m*ndim + n] = value;
	}

	double* Matrix::createLinArray()
    {

        double* linArray = new double[size];
        return linArray;

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

	 void Matrix::print(std::string filename)
    {
		std::ofstream outputFile(filename.c_str(), std::ios::out);
        outputFile <<  mdim << " " << ndim << std::endl;
		for(size_t i = 0; i < size; ++i)
        {
			//if((i % (ndim)) == 0 && i != 0){ std::endl (outputFile); }
            outputFile << dataPointer[i] << std::endl;
        }
		std::endl (outputFile);
	}

	Matrix& Matrix::operator* (Matrix MatrixB)
	{
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

	
	void matmul(const int M, const int N, const int K, double * A, const int lda, double * B, int ldb, double * C, int c)
	{
		


	
	}
	
	void Matrix::matmult(Matrix& A, Matrix& B, Matrix& C)
	{
		if( A.ndim < 16  && B.ndim < 16  )
		{
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
		size_t newDim = A.ndim*0.5;
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
		
		A.split(subMatA);
		B.split(subMatB);

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
		//C.print();
	}

