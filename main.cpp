#include "matrix.h"

int main(int argc, char *argv[]){

	if(argc != 4){
		std::cerr << "4 arguments required" << std::endl;
		return -1;
	}
	
	Matrix testmatrix(argv[1]);
	Matrix testmatrix1(argv[2]);
	Matrix testmatrix2(32,32);
	//testmatrix2.setIdentity();
	Matrix testmatrix3(32,32);
	//testmatrix.print();
	//testmatrix1.print();
	/*
	std::vector<Matrix> matrixVector;
	matrixVector.push_back(Matrix(4,4));
	matrixVector.push_back(Matrix(4,4));
	matrixVector.push_back(Matrix(4,4));
	matrixVector.push_back(Matrix(4,4));
	testmatrix.split(matrixVector);
	*/
	//testmatrix.print();
	//Matrix::matmult(testmatrix,testmatrix1,testmatrix2);
	testmatrix3 = testmatrix*testmatrix1;
	testmatrix3.print(argv[3]);
	//testmatrix3.print();
	int i = 0;
	//while(std::cin >> i);
return 0;
}
