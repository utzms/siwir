#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
class Matrix
{
	private:
    size_t ndim;
    size_t mdim;
    size_t size;
    double *dataPointer;

public:
    Matrix  (const int n,const int m){
        ndim=n;
        mdim=m;
        size=n*m;
        dataPointer = createLinArray();
    }


    Matrix (std::string filename)
    {

		std::ifstream inputFile(filename, std::ios::in);
        std::string dimensions;
        int ntmp;
        int mtmp;

		inputFile  >>  mtmp;
        inputFile >> ntmp;

		ndim = static_cast<size_t>(ntmp);
		mdim = static_cast<size_t>(mtmp);

        /* extract matrix dimensions
        int space = dimensions.find(" ");
        std::string nDimString = dimensions.substr(0,space);
        std::string mDimString =  dimensions.substr(space+1,dimensions.length());
        ndim = atoi(nDimString.c_str());
        mdim = atoi(mDimString.c_str());
        */
        dataPointer = createLinArray();

        for(size_t i = 0;i < size; i++)
        {
            inputFile >> dataPointer[i++];
        }

    }

    double* createLinArray()
    {

        double* linArray = new double[size];
        return linArray;

    }

    void setIdentity()
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

    void print()
    {
        std::cout << "Dimensions: " << ndim << " x " << mdim << std::endl;
		for(size_t i = 0; i < size; ++i)
        {
			if((i % (ndim)) == 0 && i != 0){ std::endl (std::cout); }
            std::cout << dataPointer[i] << ' ';
        }
		std::endl (std::cout);
    }
};
#endif // MATRIX_H
