#include "../include/grid.h"

Grid::Grid( double P, int rays, char* absFile, char* refrFile )
{
	//read in matrix from file
    std::ifstream absCoeffFile(absFile, std::ios::in);
    std::ifstream refrIndFile(refrFile, std::ios::in);

	int xtmp;
	int ytmp;
	int maxtmp;

    _initialPower = 10.0;
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
    _hx = 3.0/(double)_dimx;
    _hy = 2.0/(double)_dimy;


	_absorptionCoefficient	=	new double[size];
	_refractionIndex		=	new double[size];
	_absorbedPower			=	new double[size];
    for(int i = 0;i < size;)
	{
		absCoeffFile >> _absorptionCoefficient[i];
        _absorptionCoefficient[i] /= 255.0;
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
    srand48(5);
    for( int i = 0; i < _rayCount ; ++i)
    {
        //set initial values
        spawnedRays[i]._posX = 0.0;
        spawnedRays[i]._posY = -0.1 + 0.2 * drand48() + 1.0;
        //set angles
        double angle = M_PI/4 -drand48() * M_PI/2;
        //double angle = drand48() * M_PI/6;
        //double angle = -drand48() * M_PI/2;
        //if(M_PI/6  + )
        spawnedRays[i]._angle = angle;
		//set ray power
        spawnedRays[i]._power = _initialPower/_rayCount;
        spawnedRays[i]._currentCellX = _hx*0.5;
        spawnedRays[i]._currentCellY = (int)(spawnedRays[i]._posY / _hy) * _hy + _hy*0.5;
        spawnedRays[i]._refractionIndex = _refractionIndex[(int)(spawnedRays[i]._currentCellY/_hy) * _dimx + (int)(spawnedRays[i]._currentCellX/_hx)];
    }

    _activeRays = _rayCount;
    while( _activeRays > 0)
	{
        int raysToDecrement = 0;
        #pragma omp parallel for reduction (+: raysToDecrement)
        for(int i = 0; i < _rayCount; ++i)
		{
            traceRay(spawnedRays[i], i, raysToDecrement);
		}
        _activeRays -= raysToDecrement;
	}

    for(int i = 0; i < _rayCount; ++i)
    {
        for(std::vector<Cell>::iterator cellsIterator = spawnedRays[i].cells.begin()
            ; cellsIterator < spawnedRays[i].cells.end()
            ; cellsIterator++
            )
        {
             _absorbedPower[(int)(cellsIterator->indexY/_hy) * _dimx + (int)(cellsIterator->indexX/_hx)] += cellsIterator->Pvalue;
        }
    }

    double max = 0.0;
    double sum = 0.0;
    for(int i = 0;  i < _dimx*_dimy; ++i)
    {
        double powerAtCell = _absorbedPower[i];
        if(powerAtCell > max)
        {
            max = powerAtCell;
        }
        sum += powerAtCell;
    }
    std::cout <<"maximally absorbed power: "<<  max << std::endl;
    std::cout <<"overall absorbed power: "<<  sum << std::endl;

    for(int i = 0;  i < _dimx*_dimy; ++i)
    {
        _absorbedPower[i] = _absorbedPower[i] / max * 255.0;
    }


}

void Grid::alterAngle(Ray& currentRay, double refractionIndex )
{
    double newAngle = asin((refractionIndex / currentRay._refractionIndex ) * sin(currentRay._angle));
	currentRay._refractionIndex = refractionIndex;  
    currentRay._angle = newAngle;

	return;
}

void Grid::traceRay(Ray& currentRay, int index, int& raysToDecrement)
{

	// Cell Layout( 5 propagation cases )
	//	 _ _
	//	|_|_|
	//->|o|_|
	//	|_|_|

    double rayOriginX = currentRay._posX;
    double rayOriginY = currentRay._posY;
    if(currentRay._currentCellX > _dimx * _hx || currentRay._currentCellY > _dimy * _hy || currentRay._currentCellY < 0.0 )
    {
        return;
    }


    //decide wether the next Cell is entered by the Ray_absorbedPower[i]
	//Propagation cases:
    //compute corner vectors

    //-- checked
    double deltaX = ( (currentRay._currentCellX + _hx*0.5) -  currentRay._posX);
    double deltaYup = ((currentRay._currentCellY + _hy*0.5) - currentRay._posY);
    double deltaYdown = (currentRay._posY - (currentRay._currentCellY - _hy*0.5));
    //--


    double upperAngle = atan2( deltaYup, deltaX );
    double downerAngle= atan2( deltaYdown, deltaX);

	double cellRefractionIndex = 0.0;
    double lastCellX = currentRay._currentCellX;
    double lastCellY = currentRay._currentCellY;

    //Ray from side
    if(currentRay._posX <= currentRay._currentCellX - _hx*.5)
    {
        double testOpposite =   tan(fabs(currentRay._angle)) * _hx;
        //ray hits top
        if(  currentRay._angle > 0.0 && currentRay._angle > upperAngle)
        {std::vector< Cell > cells;
            double adjacent = deltaYup / tan(currentRay._angle);
            currentRay._posX += adjacent;
            currentRay._posY =  currentRay._currentCellY + _hy*0.5;
            currentRay._currentCellY += _hy;
        }
        //ray hits bottom
        else if( currentRay._angle < 0.0 && fabs(currentRay._angle) > downerAngle)
        {
            double adjacent = deltaYdown / tan(fabs(currentRay._angle));
            currentRay._posX += adjacent;
            currentRay._posY =  currentRay._currentCellY - _hy*0.5;
            currentRay._currentCellY -= _hy;
        }
        //ray hits side
        else
        {
            currentRay._posX = currentRay._currentCellX + _hx*0.5;
            if(currentRay._angle < 0.0)
            {
                currentRay._posY -= testOpposite;
            }
            else if(currentRay._angle > 0.0)
            {
                currentRay._posY += testOpposite;
            }
            currentRay._currentCellX += _hx;
        }
    }
    //Ray from Top
    else if( deltaX < _hx && currentRay._angle < 0.0 )
    {
        double adjacent = deltaX;
        double testOpposite =  tan(fabs(currentRay._angle)) * adjacent;

        //ray hits bottom
        if( fabs(currentRay._angle) > downerAngle )
        {
            double adjacentNew = _hy / tan(fabs(currentRay._angle));
            currentRay._posX += adjacentNew;
            currentRay._posY = currentRay._currentCellY - _hy*0.5;
            currentRay._currentCellY -= _hy;
        }
        //ray hits side
        else
        {
            currentRay._posX = currentRay._currentCellX + _hx*0.5;
            currentRay._posY -= testOpposite;
            currentRay._currentCellX += _hx;
        }

    }
    //Ray from bottom
    else if( deltaX < _hx && currentRay._angle > 0.0 )
    {
        double adjacent = deltaX;
        double testOpposite =  tan(currentRay._angle) * adjacent;
        //ray hits top
        if( currentRay._angle >upperAngle )
        {
            double adjacentNew = _hy / tan(currentRay._angle);
            currentRay._posX += adjacentNew;
            currentRay._posY = currentRay._currentCellY + _hy*0.5;
            currentRay._currentCellY += _hy;
        }
        //ray hits side
        else
        {
            currentRay._posX = currentRay._currentCellX + _hx*0.5;
            currentRay._posY += testOpposite;
            currentRay._currentCellX += _hx;
        }
    }

    if(currentRay._currentCellX > _dimx * _hx || currentRay._currentCellY > _dimy * _hy || currentRay._currentCellY < 0.0 )
    {
        raysToDecrement += 1;
        return;
    }


    cellRefractionIndex = _refractionIndex[(int)(currentRay._currentCellY/_hy) * _dimx + (int)(currentRay._currentCellX/_hx)];
    if(currentRay._refractionIndex > cellRefractionIndex || currentRay._refractionIndex < cellRefractionIndex)
    {
        alterAngle(currentRay ,cellRefractionIndex);
    }

    double rayLengthX = currentRay._posX - rayOriginX;
    double rayLengthY = currentRay._posY - rayOriginY;
    double length = sqrt(rayLengthX*rayLengthX + rayLengthY*rayLengthY);

	//compute Lambert Beer
	double Pout = currentRay._power
            * exp( -1.0*_absorptionCoefficient[(int)(lastCellY/_hy) * _dimx + (int)(lastCellX/_hx)]
            * length);

	//compute power difference
    double	Pdiff  =  currentRay._power - Pout;
    currentRay._power -= Pdiff;

	// add power difference to array

    Cell newCell;
    newCell.indexX = lastCellX;
    newCell.indexY = lastCellY;
    newCell.Pvalue = Pdiff;
    currentRay.cells.push_back(newCell);
    if(currentRay._power < 0.001 *_initialPower/_rayCount)
    {
        raysToDecrement += 1;
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
        outputFile << (int)_absorbedPower[i] << " ";
        if(i%_dimx == 0 || i%16 == 0)
             outputFile << std::endl;
	}

    outputFile.close();
}
