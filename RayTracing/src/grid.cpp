#include "../include/grid.h"

Grid::Grid( double P, int rays, char* absFile, char* refrFile )
{
	//read in matrix from file
    std::ifstream absCoeffFile(absFile, std::ios::in);
    std::ifstream refrIndFile(refrFile, std::ios::in);

	int xtmp;
	int ytmp;
	int maxtmp;

    _initialPower = P;
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
    for(int i = 0;i < size;)
	{
		absCoeffFile >> _absorptionCoefficient[i];
		refrIndFile  >> _refractionIndex[i++];
	}
    if(rays != 0)
    {
        _rayCount = rays;
    }
    else
    {
        _rayCount = 1000000;
    }
    spawnedRays.resize(rays);

}

void Grid::castRays()
{
    //Ray Initialization
    srand48(1.0);
    for( int i = 0; i < _rayCount ; ++i)
    {
        //set initial values
        spawnedRays[i]._posX = 0.0;
        spawnedRays[i]._posY = -0.1 + 0.2 * drand48();
        //set angles
        double angle = 3/2 * M_PI + M_PI  * drand48();
        spawnedRays[i]._angle = angle;
        spawnedRays[i]._mX = cos(angle);
        spawnedRays[i]._mY = sin(angle);
        //set ray power
        spawnedRays[i]._power = _initialPower;
    }

    _activeRays = _rayCount;
//    while( _activeRays != 0)
//    {

//    }

}

void Grid::traceRay(Ray currentRay)
{

}

void Grid::print(std::string filename)
{
	std::ofstream outputFile(filename.c_str(), std::ios::out);
    outputFile << "P2" << std::endl;
	outputFile <<  _dimx << " " << _dimy << std::endl;
    outputFile << _maxValue << std::endl;
    int size= _dimx*_dimy;

    for(int i = 0; i < size; ++i)
	{
        outputFile << _absorptionCoefficient[i] << " ";
        if(i%60 == 0 || i%16 == 0)
             outputFile << std::endl;
	}
    outputFile.close();
}
