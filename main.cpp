#include "matrix.h"

int main(int argc, char *argv[]){

	Matrix testmatrix("matrix.txt");
	std::vector<Matrix> matrixVector;
	matrixVector.push_back(Matrix(2,2));
	testmatrix.split(matrixVector);
	testmatrix.print();
	matrixVector[0].print();
	int i = 0;
	while(std::cin >> i);
return 0;
}
