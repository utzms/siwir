#include "matrix.h"

Matrix::Matrix  (const int n,const int m){
        ndim=n;
        mdim=m;
        size=n*m;
        dataPointer = createLinArray();
    }


Matrix::Matrix (std::string filename)
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
        std::cout << ndim << " " << mdim << std::endl;
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
			for(size_t j = 0; j < ndim; ++j)
			{
				double rowSum = 0;
				for(size_t rowi = 0; rowi < MatrixC->ndim; ++rowi)
				{
					rowSum += (dataPointer[i*ndim + rowi] * MatrixB.dataPointer[j + rowi*mdim]);
				}
				MatrixC->dataPointer[i*MatrixC->ndim + j] = rowSum;
			}
		}
		return *MatrixC;
	}
