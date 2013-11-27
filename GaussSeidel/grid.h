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
int getIndexRed( size_t row, size_t column );
int getIndexBlack( size_t row, size_t column );
};
