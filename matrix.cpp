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
		for(size_t i = 0; i < ndim/2 ; ++i)
		{
			for(size_t j = 0; j < ndim/2 ;++j)
			{
				subMat[0].dataPointer[i * ndim/2 + j] = dataPointer[i * ndim + j];
				subMat[1].dataPointer[i * ndim/2 + j] = dataPointer[i * ndim + j + (int)(ndim*0.5)];
				subMat[2].dataPointer[i * ndim/2 + j] = dataPointer[i * ndim + j + (ndim*2)];
				subMat[3].dataPointer[i * ndim/2 + j] = dataPointer[i * ndim + j + (int)(ndim*2.5)];
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
		if( A.ndim <= 4 && B.ndim <= 4  )
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
		size_t dimA = A.ndim/2;
		std::vector<Matrix> subMatA;
		std::vector<Matrix> subMatB;
		subMatA.push_back(Matrix(dimA,dimA));
		subMatA.push_back(Matrix(dimA,dimA));
		subMatA.push_back(Matrix(dimA,dimA));
		subMatA.push_back(Matrix(dimA,dimA));
		subMatB.push_back(Matrix(dimA,dimA));
		subMatB.push_back(Matrix(dimA,dimA));
		subMatB.push_back(Matrix(dimA,dimA));
		subMatB.push_back(Matrix(dimA,dimA));
		
		A.split(subMatA);
		B.split(subMatB);

		Matrix M1(dimA, dimA);	
		Matrix sumA1A4(dimA, dimA);	
		sumA1A4.sumOf(subMatA[0] , subMatA[3]);
		Matrix sumB1B4(dimA, dimA);	
		sumB1B4.sumOf(subMatB[0] , subMatB[3]);

		Matrix M2(dimA, dimA);
		Matrix sumA3A4(dimA, dimA);
		sumA3A4.sumOf(subMatA[2],subMatA[3]);

		Matrix M3(dimA, dimA);
		Matrix diffB2B4(dimA, dimA);
		diffB2B4.diffOf(subMatB[1],subMatB[3]);

		Matrix M4(dimA, dimA);
		Matrix diffB3B1(dimA, dimA);
		diffB3B1.diffOf(subMatB[2],subMatB[0]);

		Matrix M5(dimA, dimA);
		Matrix sumA1A2(dimA, dimA);
		sumA1A2.sumOf(subMatA[0],subMatA[1]);

		Matrix M6(dimA, dimA);
		Matrix diffA3A1(dimA, dimA);
		diffA3A1.diffOf(subMatA[2],subMatA[0]);
		Matrix sumB1B2(dimA, dimA);
		sumB1B2.sumOf(subMatB[0],subMatB[1]);

		Matrix M7(dimA, dimA);
		Matrix diffA2A4(dimA, dimA);
		diffA2A4.diffOf(subMatA[1],subMatA[3]);
		Matrix sumB3B4(dimA, dimA);
		sumB3B4.sumOf(subMatB[2],subMatB[3]);

		matmult(sumA1A4, sumB1B4, M1);
		matmult(sumA3A4, subMatB[0], M2);
		matmult(subMatA[0], diffB2B4, M3);
		matmult(subMatA[3], diffB3B1, M4);
		matmult(sumA1A2, subMatB[3], M5 );
		matmult(diffA3A1, sumB1B2, M6);
		matmult(diffA2A4,sumB3B4, M7);
		for(size_t i = 0; i < dimA ; ++i)
		{
			for(size_t j = 0; j < dimA ;++j)
			{
				C.dataPointer[i * C.ndim + j] = M1.dataPointer[i * dimA + j] + M4.dataPointer[i * dimA + j] - M5.dataPointer[i * dimA + j] + M7.dataPointer[i * dimA + j];
				C.dataPointer[i * C.ndim + j + static_cast<size_t>(C.ndim*0.5)] = M3.dataPointer[i * dimA + j] + M5.dataPointer[i * dimA + j];
				C.dataPointer[i * C.ndim + j + (C.ndim+C.ndim)] = M2.dataPointer[i * dimA + j] + M4.dataPointer[i * dimA + j];
				C.dataPointer[i * C.ndim + j + static_cast<size_t>(C.ndim*2.5)] = M1.dataPointer[i * dimA + j] - M2.dataPointer[i * dimA + j] + M3.dataPointer[i * dimA + j] + M6.dataPointer[i * dimA + j];
			}
		}
		C.print();
	}

