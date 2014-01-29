#ifndef RAY_H
#define RAY_H

#include "cell.h"

class Ray{
public:
    double _posX;
    double _posY;

    double _currentCellX;
    double _currentCellY;

    double _angle;
    double _power;
    double _refractionIndex;
    std::vector< Cell > cells;
};
#endif // RAY_H
