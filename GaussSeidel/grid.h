#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>


class Grid
{

protected:
size_t nx;
size_t ny;
size_t iterations;
double * blackValues;
double * redValues;

public:

void Grid(size_t x, size_t y);
void writeToFile(std::string filename);

int computeGaussSeidel(size_t iterations);
int computeResidual();

};


