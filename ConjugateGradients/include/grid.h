#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <mpi.h>
#include <math.h>

class Grid
{

protected:
int nx;
int ny;
std::vector<double> grid;
std::vector<double> resultVector_Fxy;


std::vector<double> vector_g;
std::vector<double> vector_d;
std::vector<double> vector_h;

double delta;
double alpha;
double beta;

int  rank;
int  size;


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

int computeConjugateGradients(int iterations, double epsilon);
void setValue(std::vector<double> & vector, int i, int j, double value);
double getValue(std::vector<double> & vector, int i, int j);
void fill_resultFxy();
void print(std::string filename);
void getResidual();
void getResidualVector(std::vector<double> & vector);

double computeDotProduct(std::vector<double> & vector_A,std::vector<double> & vector_B);

};
