#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
class Matrix
{
    size_t ndim;
    size_t mdim;
    size_t size;
    double *dataPointer;

    Matrix  (const int n,const int m){
        ndim=n;
        mdim=m;
        size=n*m;
        dataPointer = createLinArray();
    }


    Matrix (std::string filename)
    {

        std::fstream inputFile(filename, "r");
        std::string dimensions;
        int ntmp;
        int mtmp;

        mtmp << inputFile;
        ntmp << inputFile;

        ndim = static_cast(ntmp);
        mdim = static_cast(mtmp);

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
            dataPointer[i++] << vinputFile;
        }

    }

    double* createLinArray()
    {

        double* linArray = new double[size];
        return linArray;

    }

    void setIdentity()
    {
        for(int i = 0; i< size; ++i)
        {
            dataPointer[i] = 0;
        }

        int count = 0;
        for(int i = 0; i < size; i+=ndim)
        {
            dataPointer[i + count] = 1;
            count++;
        }
    }

    void print()
    {
        std::cout << "Dimensions: " << ndim << " x " << mdim<< std::endl;
        for(int i = 0; i < size; i++)
        {
            std::cout << dataPointer[i] << " ";
            if(i % mdim) std::cout << std::endl;
        }
    }
};
#endif // MATRIX_H
