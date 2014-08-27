#include <cstdio>
#include <iostream>
#include <cmath>
#include <approx.h>
#include <Eigen/Core>
using namespace Eigen;
using namespace std;

int main(int argc, char const *argv[])
{
	Matrix<int,3,7> testMatrix = Matrix<int,3,7>();
	int *zeroZero = &testMatrix(0,0);
	int *zeroOne = &testMatrix(0,1);
	int *oneZero = &testMatrix(1,0);
	cout << zeroZero << endl;
	cout << zeroOne << endl;
	cout << oneZero << endl;
	int *start = testMatrix.data();
	cout << start << endl;
	cout << exp(2);
	// scanf float -> int rounds down
	int blah;
	scanf("%d", &blah);
	printf("%d\n", blah);
	for (int i = 0; i < 7; i++) {
		int n = 1 << i;
		float normal = exp(n);
		float fast = fastexp(n);
		float faster = fasterexp(n);
		printf("testing: %d\n", n);
		printf("fast accuracy: %f\n", 1 - abs(normal - fast) / normal);
		printf("faster accuracy: %f\n", 1 - abs(normal - faster) / normal);
	}
	return 0;
}