#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <omp.h>
#include <math.h>
#include "matrix.h"

class Grid
{

protected:
int nx;
int ny;
double * blackValues;
double * redValues;
Matrix * resultFxy;
std::vector<double> resultVectorX;

double stencil_up;
double stencil_down;
double stencil_right;
double stencil_left;
double stencil_center;


public:

Grid(int const x, int const y);
void writeToFile(std::string filename);

int computeGaussSeidel(size_t iterations);
int computeResidual();
int getIndexRed( size_t row, size_t column );
int getIndexBlack( size_t row, size_t column );
void setValue(int x, int y, double value);
double getValue(int x, int y);
double computeStencilBlack(int i, int j);
};
