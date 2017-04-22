#include <stdio.h>
#include <stdlib.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../kdTree.h"
#include "../SPPoint.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#define numOfPoints 3
#define dimOfEachPoint 2

typedef enum Errors {memleak, logicproblem,succeeded } Errors;


Errors SPKDTree_Test(bool init) {

	bool good = true;

	//create kdarray for kdtree
	SPPoint* pointsArr[5] = {NULL,NULL,NULL,NULL,NULL};

	double data0[] = {1.0,2.0};
	pointsArr[0] = spPointCreate(data0, 2, 0);

	double data1[] = {123.0,70.0};
	pointsArr[1] = spPointCreate(data1, 2, 1);

	double data2[] = {2.0,7.0};
	pointsArr[2] = spPointCreate(data2, 2, 2);

	double data3[] = {9.0,11.0};
	pointsArr[3] = spPointCreate(data3, 2, 3);

	double data4[] = {3.0,4.0};
	pointsArr[4] = spPointCreate(data4, 2, 4);

	//Asserts
	for (int i = 0; i < 5; i++) {
		if (pointsArr[i] == NULL) {
			DestroySppointArray(pointsArr,5);
			return 1;
		}
	}

	SPKDArray* firstKDDrray = Init( pointsArr, 5);


	if (firstKDDrray == NULL) {
		DestroySppointArray(pointsArr,5);
		return 1;
	}

	if (init)
	{
		//create tree incremental
		KDTreeNode* treeINCR = InitKdTreeFromKdArray(firstKDDrray, INCREMENTAL, 1);
		DestroyKdTree(treeINCR);

		KDTreeNode* treeRAND = InitKdTreeFromKdArray(firstKDDrray, RANDOM, 1);
		DestroyKdTree(treeRAND);

		KDTreeNode* treeMAX = InitKdTreeFromKdArray(firstKDDrray,MAX_SPREAD , 1);
		DestroyKdTree(treeMAX);
	}

	else {
	//create tree incremental
	KDTreeNode* treeINCR = InitKdTreeFromKdArray(firstKDDrray, INCREMENTAL, 1);


	SPBPQueue *bqp = spBPQueueCreate(3);
	kNearestNeighbors(treeINCR, bqp, pointsArr[3]);
	BPQueueElement element;

	short i = 0;
	while (!spBPQueueIsEmpty(bqp)) {
		spBPQueuePeek(bqp, &element);
		spBPQueueDequeue(bqp);
		if (i == 0) {
			good = (element.index == 3);
			if (!good) break;
		}
		else if (i == 1) {
			good = (element.index == 2);
			if (!good) break;
		}
		else {
			good = (element.index == 4);
			if (!good) break;
		}
		i++;
	}


	DestroyKdTree(treeINCR);
	spBPQueueDestroy(bqp);
	}



	DestroySppointArray(pointsArr,5);
	DestroyKDArray(firstKDDrray, 5);

	if (!good) return logicproblem;
	return succeeded;
}



//int main(int argc, char **argv) {
//	printf("test started\n");
//
//	//init
//	if (SPKDTree_Test(true) == succeeded){
//		printf("Init TIL BALISTI\n");
//	}
//
//	//search
//	if (SPKDTree_Test(false) == succeeded){
//		printf("Search TIL BALISTI\n");
//	}
//
//
//	return 0;
//}
