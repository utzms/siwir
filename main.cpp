#include "matrix.h"
#include <sys/time.h>

extern "C" {
	#include <likwid.h>
}



static void matmul(const int M, const int N, const int K, double * A, const int lda, double * B, int ldb, double * C, int ldc)
{
	Matrix MatrixA(M,K,A,lda);
	Matrix MatrixB(K,N,B,ldb);
	Matrix MatrixC(K,N,C,ldc);
		
	Matrix::matmult(MatrixA,MatrixB,MatrixC);
	for(size_t i = 0; i< MatrixC.getMdim() ; ++i)
	{	
		for(size_t j = 0; j<MatrixC.getNdim() ; ++j)
		{
			C[i*ldc+j] = MatrixC.getValueAt(i,j);
		}
	}
}

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
	timeval timeStart, timeEnd;
	gettimeofday(&timeStart, 0);

	likwid_markerInit();
	likwid_markerStartRegion("_matmult");

	Matrix::matmult(inputMatrix,inputMatrix1,testmatrix3);
	//testmatrix3 = inputMatrix * inputMatrix1;

	likwid_markerStopRegion("_matmult");
	likwid_markerClose();

	gettimeofday(&timeEnd, 0);
	//output 
	std::cout << "Laufzeit: " << timeEnd.tv_sec - timeStart.tv_sec << "," << timeEnd.tv_usec - timeStart.tv_usec << std::endl;	     testmatrix3.print(argv[3]);
	//testmatrix3.print();
	//int i = 0;
	//while(std::cin >> i);
return 0;
}


