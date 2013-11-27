#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <omp.h>

class Grid
{

protected:
int nx;
int ny;
double * blackValues;
double * redValues;

public:

Grid(int const x, int const y);
void writeToFile(std::string filename);

int computeGaussSeidel(size_t iterations);
int computeResidual();
int getIndexRed( size_t row, size_t column );
int getIndexBlack( size_t row, size_t column );
void setValue(int x, int y, double value);
double getValue(int x, int y);
};
