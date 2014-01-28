#ifndef RAY_H
#define RAY_H

#endif // RAY_H

class Ray{
public:
    double _posX;
    double _posY;
	double _b;

    double _currentCellX;
    double _currentCellY;

	double _m;
    double _angle;
    double _power;
    double _refractionIndex;
	void alterAngle(double _refractionIndex);
};
