#include "matrix.h"

int main(int argc, char *argv[]){

	Matrix testmatrix(8,8);
	Matrix testmatrix1(8,8);
	testmatrix.setIdentity();
	testmatrix1.setIdentity();
	Matrix testmatrix2(8,8);
	testmatrix2.setIdentity();
	Matrix testmatrix3(8,8);
	std::vector<Matrix> matrixVector;
	testmatrix.print();
	Matrix::matmult(testmatrix,testmatrix1,testmatrix2);
	testmatrix3 = testmatrix*testmatrix1;
	testmatrix2.print();
	testmatrix3.print();
	int i = 0;
	while(std::cin >> i);
return 0;
}
