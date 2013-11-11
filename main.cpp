#include "matrix.h"

int main(int argc, char *argv[]){

	Matrix testmatrix("matrix.txt");
	testmatrix.setIdentity();
	Matrix testmatrix2(3,3);
	testmatrix2.setIdentity();
	testmatrix.print();
	testmatrix2.print();
	testmatrix2 = testmatrix*testmatrix2;
	testmatrix2.print();
	int i = 0;
	while(std::cin >> i);
return 0;
}
