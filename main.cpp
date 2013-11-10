#include "matrix.h"

int main(int argc, char *argv[]){

	Matrix testmatrix(3,3);
	testmatrix.setIdentity();
	testmatrix.print();
	int i = 0;
	while(std::cin >> i);
return 0;
}
