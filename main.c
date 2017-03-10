#include "kdTree.h"
#include "SPPoint.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int dim = 6;
	SPPoint** pointArr = (SPPoint**)malloc(sizeof(SPPoint*)*dim);
	srand(time(NULL));
	for (int i = 0; i < dim; i++) {
		double data[2] = {rand(), rand()};
		printf("point %d: (%d,%d)", i, data[0], data[1] );
		pointArr[i] = spPointCreate(data, 2, i);
	}
	SPKDArray* kdArr = Init(pointArr, dim);

}
