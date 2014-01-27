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
	_hx = 300/_dimx;
	_hy = 200/_dimy;

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
        spawnedRays[i]._posY = -0.1 + 0.2 * drand48();
		spawnedRays[i]._b = spawnedRays[i]._posY;
        //set angles
		double angle = M_PI * (-0.5 + drand48());
		//if()
        spawnedRays[i]._angle = angle;
		spawnedRays[i]._m = tan(angle);
		//set ray power
        spawnedRays[i]._power = _initialPower;
    }

    _activeRays = _rayCount;
	while( _activeRays != 0)
	{
		for(int i = 0; i < _activeRays; i++)
		{
			traceRay(spawnedRays[i], i);
		}

		for(std::vector<int>::iterator completedRayIterator = completedRayIndices.begin();
			completedRayIterator != completedRayIndices.end();
			completedRayIterator++
			)
		{
			spawnedRays.erase(spawnedRays.begin() + *completedRayIterator);
		}
	}

}

void Grid::alterAngle(double refractionIndex)
{
	return;
}

void Grid::traceRay(Ray currentRay, int index)
{

	// Cell Layout( 5 propagation cases )
	//	 _ _
	//	|_|_|
	//->|o|_|
	//	|_|_|


	double xincrement = 0.2;
	//Perform RayTracing the next stepo: y = m(x-xa) +ya;
	currentRay._posX += xincrement;
	currentRay._posY =  currentRay._m*currentRay._posX + currentRay._b;

	if(currentRay ._posX >= _dimx*_hx)
	{
		_activeRays--;
		completedRayIndices.push_back(index);
		return;
	}
	else if(currentRay ._posY >= _dimx*_hx)
	{
		_activeRays--;
		completedRayIndices.push_back(index);
		return;return;
	}

	//decide wether the next Cell is entered by the Ray
	//Propagation cases:

	double cellRefractionIndex = 0.0;
	double currentCellX = currentRay._currentCellX;
	double currentCellY = currentRay._currentCellY;


	if(currentRay._posX >= currentCellX + _hx/2
			&& currentRay._posY >= currentCellY + _hy/2)
	{
		currentRay._currentCellX++;
		currentRay._currentCellY++;
		cellRefractionIndex =_refractionIndex[currentRay._currentCellY * 60 + currentRay._currentCellX];
	}
	else if(currentRay._posX < currentCellX + _hx/2
			&& currentRay._posY >= currentCellY + _hy/2)
	{
		currentRay._currentCellY++;
		cellRefractionIndex =_refractionIndex[currentRay._currentCellY * 60 + currentRay._currentCellX];
	}
	else if(currentRay._posX >= currentCellX + _hx/2
			&& currentRay._posY <= currentCellY + _hy/2
			&& currentRay._posY > currentCellY - _hy/2)
	{
		currentRay._currentCellX++;
		cellRefractionIndex =_refractionIndex[currentRay._currentCellY * 60 + currentRay._currentCellX];
	}
	else if(currentRay._posX >= currentCellX + _hx/2 && currentRay._posY <= currentCellY - _hy/2)
	{
		currentRay._currentCellX++;
		currentRay._currentCellY--;
		cellRefractionIndex =_refractionIndex[currentRay._currentCellY * 60 + currentRay._currentCellX];
	}
	else if(currentRay._posX <= currentCellX + _hx/2
			&& currentRay._posY <= currentCellY - _hy/2)
	{
		currentRay._currentCellY--;
		cellRefractionIndex =_refractionIndex[currentRay._currentCellY * 60 + currentRay._currentCellX];
	}
	else
	{
		cellRefractionIndex = currentRay._refractionIndex;
	}

	if(currentRay._refractionIndex != cellRefractionIndex)
	{
		alterAngle(cellRefractionIndex);
	}



	//compute Lambert Beer
	double Pout = currentRay._power
			* exp( -_absorptionCoefficient[currentRay._currentCellY * 60 + currentRay._currentCellX]
			* xincrement);

	// compute power difference
	double	Pdiff  =  Pout - currentRay._power;
	currentRay._power -= Pdiff;

	// add power difference to array
	_absorbedPower[currentRay._currentCellY * 60 + currentRay._currentCellX] += Pdiff;

	if(currentRay._power < 0.001 *_initialPower)
	{
		_activeRays--;
		completedRayIndices.push_back(index);
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
        outputFile << _absorptionCoefficient[i] << " ";
        if(i%60 == 0 || i%16 == 0)
             outputFile << std::endl;
	}
    outputFile.close();
}
