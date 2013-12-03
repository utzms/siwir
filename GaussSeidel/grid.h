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
double stepSizeX;
double stepSizeY;
double * blackValues;
double * redValues;
std::vector<double> resultVectorFxy;
std::vector<double> solutionVectorX;
std::vector<double> y1Vector;

double stencil_up;
double stencil_down;
double stencil_right;
double stencil_left;
double stencil_center;

public:

Grid(int const x, int const y);
~Grid();
void writeToFile(std::string filename);

int computeGaussSeidel(int iterations);
int computeResidual();
int getIndexRed( size_t row, size_t column );
int getIndexBlack( size_t row, size_t column );
void setValue(int x, int y, double value);
double getValue(int x, int y);
double computeStencilBlack(int i, int j); 
void fill_resultFxy();
void print(std::string filename);
};
