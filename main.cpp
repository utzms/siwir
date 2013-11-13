#include "matrix.h"
#include <sys/time.h>

extern "C" {
	#include <likwid.h>
}


//claimed function for calling the program, only due to the assignment tasks
static void matrixmult(const int M, const int N, const int K, double * A, const int lda, double * B, int ldb, double * C, int ldc)
{
		
	Matrix MatrixA(M,K,A,lda);
	Matrix MatrixB(K,N,B,ldb);
	Matrix MatrixC(M,N);
	
		
	if(Matrix::checkIfPowerOfTwo(M,K))
	{	
		if(Matrix::checkIfPowerOfTwo(K,N))
		{
			Matrix::matmult(MatrixA,MatrixB,MatrixC);
		}
	}
	else
	{
		MatrixC = MatrixA*MatrixB;
	}
	for(size_t i = 0; i< MatrixC.getMdim() ; ++i)
	{	
		for(size_t j = 0; j<MatrixC.getNdim() ; ++j)
		{
			C[i*ldc + j] = MatrixC.getValueAt(i,j);
		}
	}
}

int main(int argc, char *argv[]){
	
	if(argc != 4){
		std::cerr << "4 arguments required" << std::endl;
		return -1;
	}

	//reading in A
	std::ifstream inputFileA(argv[1], std::ios::in);

	int ndimA;
	int mdimA;
        inputFileA >> mdimA;
	inputFileA >> ndimA;
	int size = ndimA*mdimA;
        double * dataA = new double[size];
        for(int i = 0;i < size;)
        {
            inputFileA >> dataA[i++];
        }
	
	//reading in B
	std::ifstream inputFileB(argv[2], std::ios::in);
        int ndimB;
        int mdimB;
        inputFileB >> mdimB;
	inputFileB >> ndimB;
	size = ndimB*mdimB;

        double * dataB = new double[size];
        for(int i = 0;i < size;)
        {
            inputFileB >> dataB[i++];
        }
	size = mdimA*ndimB;
	double * dataC = new double[size];	

	matrixmult(mdimA,ndimB,ndimA,dataA,ndimA,dataB,ndimB,dataC,ndimB);
	//writing C	
	std::ofstream outputFile(argv[3], std::ios::out);
	outputFile << mdimA <<  " " << ndimB << std::endl;
        for(int i = 0; i < size; ++i)
        {
                outputFile << dataC[i] << std::endl;
        }	
	
	return 0;
}

