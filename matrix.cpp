#include "matrix.h"

Matrix::Matrix(const int n,const int m)
	{
        ndim=n;
        mdim=m;
        size=n*m; 
        dataPointer = createLinArray();
    }

Matrix::Matrix(const int n, const int m, double * datapointer)
	{
        ndim=n;
        mdim=m;
        size=n*m; 
        dataPointer = datapointer;
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
        std::cout << "Dim: " <<mdim << " " << ndim << std::endl;
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
			if((i % (ndim)) == 0 && i != 0){ std::endl (outputFile); }
            outputFile << dataPointer[i] << ' ';
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
		subMat.push_back(Matrix(ndim/2,mdim/2));
		subMat.push_back(Matrix(ndim/2,mdim/2));
		subMat.push_back(Matrix(ndim/2,mdim/2));
		subMat.push_back(Matrix(ndim/2,mdim/2));
		for(int i = 0; i < ndim/2 ; ++i)
		{
			for(int j = 0; j < ndim/2 ;++j)
			{
			subMat[0].dataPointer[i] = dataPointer[i];
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

	/*
	void matmul(const int M, const int N, const int K, double * A, const int lda, double * B, int ldb, double * C, int c)
	{
		
		vector<Matrix &> subMatB;
		vector<Matrix &> subMatB;
		subMatA = split(A);
		subMatB = split(B);
	
		if( M < 9 && N < 9  )
		{
			{
				for(size_t i = 0; i < M ; ++i)
	               		 {
	               		         for(size_t j = 0; j < N; ++j)
	               		         {
	                	                double rowSum = 0;
	             	  	                for(size_t rowi = 0; rowi < ndim; ++rowi)
	                               	 {
	                                        rowSum += (dataPointer[i*ndim + rowi] * MatrixB.dataPointer[j + rowi * MatrixB.ndim]);
	                                }
	                                MatrixC->dataPointer[i*MatrixC->ndim + j] = rowSum;
	                        }
	                }
		}
	
	}
	*/
	void Matrix::matmult(Matrix& A, Matrix& B, Matrix& C)
	{
		if( A.ndim < 9 && B.ndim < 9  )
		{
			 for(size_t i = 0; i < A.ndim ; ++i)
	               	 {
	               	         for(size_t j = 0; j < B.ndim; ++j)
	               	         {
	                                double rowSum = 0;
	                                for(size_t rowi = 0; rowi < ndim; ++rowi)
	                                {
	                                        rowSum += (dataPointer[i*ndim + rowi] * B.dataPointer[j + rowi * B.ndim]);
	                                }
									C.dataPointer[i*C.ndim + j] = rowSum;
	                        }
	                }
			return;
		}
		std::vector<Matrix> subMatA;
		std::vector<Matrix> subMatB;
		A.split(subMatA);
		B.split(subMatB);
		
		Matrix M1(A.mdim, B.ndim);	
		Matrix sumA1A4(A.mdim, B.ndim);	
		sumA1A4.sumOf(subMatA[0] , subMatA[3]);
		Matrix sumB1B4(A.mdim, B.ndim);	
		sumB1B4.sumOf(subMatB[0] , subMatB[3]);

		Matrix M2(A.mdim, B.ndim);
		Matrix sumA3A4(A.mdim, B.ndim);
		sumA3A4.sumOf(subMatA[2],subMatA[3]);

		Matrix M3(A.mdim, B.ndim);
		Matrix diffB2B4(A.mdim, B.ndim);
		diffB2B4.diffOf(subMatB[1],subMatB[3]);

		Matrix M4(A.mdim, B.ndim);
		Matrix diffB3B1(A.mdim, B.ndim);
		diffB3B1.diffOf(subMatB[1],subMatB[0]);

		Matrix M5(A.mdim, B.ndim);	
		Matrix sumA1A2(A.mdim, B.ndim);
		sumA1A2.sumOf(subMatA[0],subMatA[1]);

		Matrix M6(A.mdim, B.ndim);	
		Matrix diffA3A1(A.mdim, B.ndim);
		diffA3A1.diffOf(subMatA[2],subMatA[0]);
		Matrix sumB1B2(A.mdim, B.ndim);
		sumB1B2.sumOf(subMatB[0],subMatB[1]);

		Matrix M7(A.mdim, B.ndim);	
		Matrix diffA2A4(A.mdim, B.ndim);
		diffA2A4.diffOf(subMatA[0],subMatA[1]);
		Matrix sumB3B4(A.mdim, B.ndim);
		sumB3B4.sumOf(subMatB[2],subMatB[3]);

		matmult(sumA1A4, sumB1B4, M1);
		matmult(sumA3A4, subMatB[0], M2);
		matmult(subMatA[0], diffB2B4, M3);
		matmult(subMatA[4], diffB3B1, M4);
		matmult(sumA1A2, subMatB[4], M5 );
		matmult(diffA3A1, sumB1B2, M6);
		matmult(diffA2A4,sumB3B4,M7);
	}

