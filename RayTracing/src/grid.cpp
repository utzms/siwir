#include "../include/grid.h"

Grid::Grid( double P, int rays, std::string absFile, std::string refrFile )
{
	//read in matrix from file
	std::ifstream absCoeffFile(absFile.c_str(), std::ios::in);
	std::ifstream refrIndFile(absFile.c_str(), std::ios::in);

	int xtmp;
	int ytmp;
	int maxtmp;

	int del ;
	absCoeffFile >> del;

	absCoeffFile >> xtmp;
	absCoeffFile >> ytmp;
	_dimx = static_cast<size_t>(xtmp);
	_dimy = static_cast<size_t>(ytmp);

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
	outputFile <<  _dimx << " " << _dimy << std::endl;
	int size= _dimx*_dimy;

	for(size_t i = 0; i < size; ++i)
	{
		outputFile << _refractionIndex[i] << std::endl;
	}
	std::endl (outputFile);
}
