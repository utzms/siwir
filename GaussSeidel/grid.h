#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <omp.h>

class Grid
{

protected:
size_t nx;
size_t ny;
double * blackValues;
double * redValues;

public:

Grid(size_t x, size_t y);
void writeToFile(std::string filename);

int computeGaussSeidel(size_t iterations);
int computeResidual();

};


