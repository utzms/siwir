#ifndef RAY_H
#define RAY_H

#endif // RAY_H

class Ray{
public:
    double _posX;
    double _posY;
	double _b;

	int _currentCellX;
	int _currentCellY;

	double _m;
    double _angle;
    double _power;
    double _refractionIndex;
	void alterAngle(double _refractionIndex);
};
