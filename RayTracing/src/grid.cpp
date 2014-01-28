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

	//determine max value
	absCoeffFile >> maxtmp;
	_maxValue = maxtmp;

	//determine gridsize
	int size= _dimx*_dimy;

	//set stepsizes
    _hx = 3/(double)_dimx;
    _hy = 2/(double)_dimy;


	_absorptionCoefficient	=	new double[size];
	_refractionIndex		=	new double[size];
	_absorbedPower			=	new double[size];
    for(int i = 0;i < size;)
	{
		absCoeffFile >> _absorptionCoefficient[i];
		refrIndFile  >> _refractionIndex[i];
		_absorbedPower[i++] = 0.0;
	}
    if(rays != 0)
    {
        _rayCount = rays;
    }
    else
    {
		_rayCount = 100;
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
        spawnedRays[i]._posY = -0.1 + 0.2 * drand48() + 1;
		spawnedRays[i]._b = spawnedRays[i]._posY;
        //set angles
        double angle = -M_PI/6  + drand48() * M_PI/3;
		//if()
        spawnedRays[i]._angle = angle;
		spawnedRays[i]._m = tan(angle);
		//set ray power
        spawnedRays[i]._power = _initialPower;
        spawnedRays[i]._currentCellX = _hx/2;
        spawnedRays[i]._currentCellY = (int)(spawnedRays[i]._posY / _hy) * _hy + _hy/2;
    }

    _activeRays = _rayCount;
	int spawnedRayIndex = 0;
	while( _activeRays != 0)
	{
		spawnedRayIndex = 0;
		for(std::vector<Ray>::iterator spawnedRaysIterator = spawnedRays.begin();
			spawnedRaysIterator != spawnedRays.end();
			spawnedRaysIterator++)
		{
            traceRay(*spawnedRaysIterator, spawnedRayIndex);
            ++spawnedRayIndex;
		}
	}

}

void Grid::alterAngle(Ray& currentRay, double refractionIndex)
{
	currentRay._refractionIndex = refractionIndex;
	return;
}

void Grid::traceRay(Ray& currentRay, int index)
{

	// Cell Layout( 5 propagation cases )
	//	 _ _
	//	|_|_|
	//->|o|_|
	//	|_|_|

    double rayOriginX = currentRay._posX;
    double rayOriginY = currentRay._posY;
    if(currentRay._currentCellX > _dimx * _hx || currentRay._currentCellY > _dimx * _hy || currentRay._currentCellY < 0 )
    {
        return;
    }


    double xincrement = 0.01;
	//decide wether the next Cell is entered by the Ray
	//Propagation cases:

	double cellRefractionIndex = 0.0;
	double currentCellX = currentRay._currentCellX;
	double currentCellY = currentRay._currentCellY;


    if(currentRay._posY > currentRay._currentCellY - _hy/2 && currentRay._posY < currentRay._currentCellY + _hy/2 )
    {
        double testOpposite =  atan(currentRay._angle) * _hx;
        //ray hits top
        if( currentRay._posY + testOpposite > currentRay._currentCellY + _hy/2)
        {
            double adjacent = (currentRay._currentCellY + _hy/2 - currentRay._posY ) / tan(currentRay._angle);
            currentRay._posX += adjacent;
            currentRay._posY =  currentRay._currentCellY + _hy/2;
            currentRay._currentCellY += _hy;
        }
        //ray hits bottom
        else if(currentRay._posY + testOpposite < currentRay._currentCellY - _hy/2)
        {
            double adjacent = (currentRay._posY - currentRay._currentCellY - _hy/2  ) / tan(currentRay._angle);
            currentRay._posX += adjacent;
            currentRay._posY =  currentRay._currentCellY - _hy/2;
            currentRay._currentCellY -= _hy;
        }
        //ray hits side
        else
        {
            currentRay._posX += _hx;
            currentRay._posY += testOpposite;
            currentRay._currentCellX += _hx;
        }
    }
    else if(currentRay._posX > currentRay._currentCellX - _hx/2
            && currentRay._posY < currentRay._currentCellY)
    {
        double adjacent = currentRay._currentCellX + hx/2 - currentRay._posX;
        double opposite =  atan(currentRay) * adjacent;
    }
    else if(currentRay._posX > currentRay._currentCellX - _hx/2
            && currentRay._posY > currentRay._currentCellY)
    {
        double adjacent = currentRay._currentCellX + hx/2 - currentRay._posX;
        double opposite =  atan(currentRay) * adjacent;
    }

    if(currentRay._currentCellX > _dimx * _hx || currentRay._currentCellY > _dimx * _hy || currentRay._currentCellY < 0 )
    {
        _activeRays--;
        completedRaysIndices.push_back(index);
        return;
    }

//	if(currentRay._refractionIndex != cellRefractionIndex)
//	{
//		alterAngle(currentRay ,cellRefractionIndex);
//	}

	//compute Lambert Beer
	double Pout = currentRay._power
            * exp( -_absorptionCoefficient[(int)currentRay._currentCellY * 60 + (int)currentRay._currentCellX]
            * _hx);

	//compute power difference
	double	Pdiff  =  Pout - currentRay._power;
    //currentRay._power -= Pdiff;

	// add power difference to array
    _absorbedPower[(int)currentCellY * 60 + (int)currentCellY] += 255;

	if(currentRay._power < 0.001 *_initialPower)
	{
		_activeRays--;
		completedRaysIndices.push_back(index);
		return;
	}
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
		outputFile << _absorbedPower[i] << " ";
        if(i%60 == 0 || i%16 == 0)
             outputFile << std::endl;
	}
    outputFile.close();
}
