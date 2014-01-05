#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <math.h>

class Grid
{

protected:
int nx;
int ny;
double * blackValues;
double * redValues;
std::vector<double> resultVectorFxy;

double hx;
double hy;

double stencil_up;
double stencil_down;
double stencil_right;
double stencil_left;
double stencil_center;

public:
Grid(int const x, int const y);
~Grid();

int computeGaussSeidel(int iterations);
int getIndexRed( size_t row, size_t column );
int getIndexBlack( size_t row, size_t column );
void setValue(int i, int j, double value);
double getValue(int i, int j);
double computeStencilBlack(int i, int j); 
void fill_resultFxy();
void print(std::string filename);
void getResidual();
};
