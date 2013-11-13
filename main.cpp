#include "matrix.h"
#include <sys/time.h>
int main(int argc, char *argv[]){

	if(argc != 4){
		std::cerr << "4 arguments required" << std::endl;
		return -1;
	}

	//read input matrices	
	Matrix inputMatrix(argv[1]);
	Matrix inputMatrix1(argv[2]);
	Matrix testmatrix2(inputMatrix.getMdim(),inputMatrix1.getNdim());
	Matrix testmatrix3(inputMatrix.getMdim(),inputMatrix1.getNdim());
	
	//call strassen + time check
	timeval timeStart, timeEnd ;
	gettimeofday(&timeStart, 0);
	Matrix::matmult(inputMatrix,inputMatrix1,testmatrix3);
	gettimeofday(&timeEnd, 0);
	
	//output 
	std::cout << "Laufzeit: " << timeEnd.tv_sec - timeStart.tv_sec << "," << timeEnd.tv_usec - timeStart.tv_usec << std::endl;	
	testmatrix3.print(argv[3]);
	//testmatrix3.print();
	//int i = 0;
	//while(std::cin >> i);
return 0;
}
