#include <stdio.h>
#include <stdlib.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../KdTree.h"
#include "../SPPoint.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#define numOfPoints 3
#define dimOfEachPoint 2

typedef enum Errors {memleak, logicproblem,succeeded } Errors;


Errors SPKDTree_Init_Test() {

	//create kdarray for kdtree
	SPPoint** pointsArr = malloc(sizeof(SPPoint*)*5);

	double* data0 = malloc(sizeof(double)*2);
	data0[0] = 1.0; data0[1] = 2.0;
	pointsArr[0] = spPointCreate(data0, 2, 0);

	double* data1 = malloc(sizeof(double)*2);
	data1[0] = 123.0; data1[1] = 70.0;

	pointsArr[1] = spPointCreate(data1, 2, 1);

	double* data2 = malloc(sizeof(double)*2);
	data2[0] = 2.0; data2[1] = 7.0;
	pointsArr[2] = spPointCreate(data2, 2, 2);

	double* data3 = malloc(sizeof(double)*2);
	data3[0] = 9.0; data3[1] = 11.0;

	pointsArr[3] = spPointCreate(data3, 2, 3);

	double* data4 = malloc(sizeof(double)*2);
	data4[0] = 3.0; data4[1] = 4.0;

	pointsArr[4] = spPointCreate(data4, 2, 4);

	//Asserts
	for (int i = 0; i < 5; i++) {
		if (pointsArr[i] == NULL) {
			DestroySppointArray(pointsArr,5);
			return 1;
		}
	}

	SPKDArray* firstKDDrray = Init(pointsArr, 5);

	if (firstKDDrray == NULL) {
		DestroySppointArray(pointsArr,5);
		return 1;
	}

	//create tree
	KDTreeNode* tree = InitKdTreeFromKdArray(firstKDDrray, INCREMENTAL, 1);

	DestroySppointArray(firstKDDrray,5);
	DestroyKdTree(tree);
}


/*
int main(int argc, char **argv) {
	printf("test started\n");

	//init
	if (SPKDTree_Init_Test() == succeeded){
		printf("Init TIL BALISTI\n");
	}


	return 0;
}*/
