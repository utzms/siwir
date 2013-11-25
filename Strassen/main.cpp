#include "matrix.h"
#include <sys/time.h>

extern "C" {
	#include <likwid.h>
}

bool blocking = true;
bool _sse_ON = true;

//claimed function for calling the program, only due to the assignment tasks
static void matrixmult(const int M, const int N, const int K, double * A, const int lda, double * B, int ldb, double * C, int ldc)
{
		
	Matrix MatrixA(M,K,A,lda);
	Matrix MatrixB(K,N,B,ldb);
	Matrix MatrixC(M,N);
	
	//for matrices with dimensions to the power of two strassen is used  		
	if(Matrix::checkIfPowerOfTwo(M,K))
	{	
		if(Matrix::checkIfPowerOfTwo(K,N))
		{
			Matrix::matmult(MatrixA,MatrixB,MatrixC);
		}
	}
	else
	{
	//otherwise native matrix multiplication
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
	
	if(argc < 4){
		std::cerr << "4 arguments required" << std::endl;
		return -1;
	}
	
	//setting optimizations by commmand line
	if(argc>5){
		std::string arg4(argv[4]);	
		if(arg4 == "-q")
		{
			_sse_ON = true;
			blocking = false;
		}	
		else if(arg4 == "-w")
		{
			_sse_ON = false;
			blocking = true;
		}
		else if(arg4 == "-e")
		{
			_sse_ON = false;
			blocking = false;
		}
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
	
	timeval start,end;	
		
	gettimeofday(&start,0);
	matrixmult(mdimA,ndimB,ndimA,dataA,ndimA,dataB,ndimB,dataC,ndimB);
	gettimeofday(&end,0);
	std::cout << "wall clock time: " << ((double)(end.tv_sec*1000000 + end.tv_usec)-(double)(start.tv_sec*1000000 + start.tv_usec))/1000000 << std::endl;
	//writing C	
	std::ofstream outputFile(argv[3], std::ios::out);
	outputFile << mdimA <<  " " << ndimB << std::endl;
        for(int i = 0; i < size; ++i)
        {
                outputFile << dataC[i] << std::endl;
        }	
	
	return 0;
}

