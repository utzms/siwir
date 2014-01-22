#include "../include/grid.h"

Grid::Grid( double P, int rays, char* absFile, char* refrFile )
{
	//read in matrix from file
    std::ifstream absCoeffFile(absFile, std::ios::in);
    std::ifstream refrIndFile(refrFile, std::ios::in);

	int xtmp;
	int ytmp;
	int maxtmp;


    char del;
    int  delInt;
    absCoeffFile >> del;
    absCoeffFile >> del;
    refrIndFile >> del;
    refrIndFile >> del;
    refrIndFile >> delInt;
    refrIndFile >> delInt;

	absCoeffFile >> xtmp;
	absCoeffFile >> ytmp;
    _dimx = static_cast<int>(xtmp);
    _dimy = static_cast<int>(ytmp);

	absCoeffFile >> maxtmp;
	_maxValue = maxtmp;

	int size= _dimx*_dimy;

	_absorptionCoefficient	=	new double[size];
	_refractionIndex		=	new double[size];
	_absorbedPower			=	new double[size];
	for(size_t i = 0;i < size;)
	{
		absCoeffFile >> _absorptionCoefficient[i];
		refrIndFile  >> _refractionIndex[i++];
	}

}



void Grid::print(std::string filename)
{
	std::ofstream outputFile(filename.c_str(), std::ios::out);
    outputFile << "P2" << std::endl;
	outputFile <<  _dimx << " " << _dimy << std::endl;
    outputFile << _maxValue << std::endl;
    int size= _dimx*_dimy;

	for(size_t i = 0; i < size; ++i)
	{
        outputFile << _absorptionCoefficient[i] << " ";
        if(i%60 == 0 || i%16 == 0)
             outputFile << std::endl;
	}
    outputFile.close();
}
