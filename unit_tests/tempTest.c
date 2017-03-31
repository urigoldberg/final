#include <stdio.h>
#include <stdlib.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../kdTree.h"
#include "../SPPoint.h"
#include "../AllocateManager.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#define numOfPoints 3
#define dimOfEachPoint 2


void printDoubleMatrix (int** m, int col, int row) {
	for (int i = 0; i < row; i ++) {
		printf("row num %d:",i);
		for (int j = 0; j < col; j++)
					printf(" %d ", m[i][j]);
		printf("\n");
	}

}

void printSppointArray(SPPoint** pointArr, int howmanypoints) {

	if (pointArr == NULL)
	{
		printf("its null you syco!\n");
		return;
	}

	for (int i = 0 ; i < howmanypoints ; i ++) {
		printf("\nPoint num %d. has index %d and dim %d\n",i,spPointGetIndex(pointArr[i]),spPointGetDimension(pointArr[i]));
		printf("its data is :");
		for (int j = 0; j < spPointGetDimension(pointArr[i]); j ++)
			printf (" %f ",spPointGetAxisCoor(pointArr[i], j));
	}
}

void printKDarray (SPKDArray* firstKDDrray, int howmanypoints) {
	if (firstKDDrray == NULL)
		{
			printf("its null you syco!\n");
			return;
		}
	printf("\nthis KDarray is built like that: dim = %d\n points arr = \n",firstKDDrray->dim);
	printSppointArray(firstKDDrray->pointsArr,firstKDDrray->dim);
	printf("matrix arr:\n");
	printDoubleMatrix(firstKDDrray->sortedMatrix,howmanypoints , spPointGetDimension(firstKDDrray->pointsArr[0]));
}

int Init_test () {

	//for random
		srand(time(NULL));

		printf("strat\n");

	    SPPoint** pointArr = (SPPoint**)calloc(numOfPoints,sizeof(SPPoint*));
	    if (pointArr == NULL) {
	        return 1;
	    }

	    //create a random data array and use it for filling the points array
	    double dataArr[numOfPoints][dimOfEachPoint];
	    for (int i = 0; i < numOfPoints; i++)
	    {
	    	for (int j = 0; j < dimOfEachPoint; j++)
	    	    	{
	    	    		dataArr[i][j] = rand();
	    	    	}
	    	//try to create point
	    	pointArr[i] = spPointCreate(dataArr[i], dimOfEachPoint, i);
	    	if (pointArr[i] == NULL)
	    	{
				DestroySppointArray(pointArr,numOfPoints );
				return 1;
	    	}

	    }

	    SPKDArray* firstKDDrray = Init( pointArr, numOfPoints);

	    if (firstKDDrray == NULL) {
	    	DestroySppointArray(pointArr,numOfPoints );
	    	return 1;

	    }

	    if (true){
	    	printSppointArray(pointArr, numOfPoints);
			printKDarray(firstKDDrray, numOfPoints);
	    }

	    //if called from isInit
	    if (true) {



			//Assert KDARRAY contains all the points at the same order:
			for (int i = 0; i < numOfPoints; i++) {
				if (spPointL2SquaredDistance(firstKDDrray->pointsArr[i],pointArr[i]) != 0) {
					printf("failed beacuse of diffrent point\n");
					return 1;
				}
			}



			 for (int i = 0; i < dimOfEachPoint; i++)
				{
				int indexFormatrix = 0;
				for (int j = 0; j < numOfPoints; j++)
					{
					double minValue = DBL_MAX;
					int minIndex = -1;
					for (int k = 0; k < numOfPoints; k++)
						{
							if (dataArr[k][i] < minValue) {
								minValue = dataArr[k][i];
								minIndex = k;
							}
						}
					if (firstKDDrray->sortedMatrix[i][indexFormatrix] != minIndex) {
						printf("sm = %d, k = %d, i = %d, index = %d",firstKDDrray->sortedMatrix[i][indexFormatrix],minIndex,i,indexFormatrix);
						DestroyKDArray(firstKDDrray, numOfPoints);
						DestroySppointArray(pointArr,numOfPoints );
						return 1;
					}
					indexFormatrix++;
					dataArr[minIndex][i] = DBL_MAX;
					}
				}
	    	}


	    DestroyKDArray(firstKDDrray, numOfPoints);
	    DestroySppointArray(pointArr,numOfPoints);
	    return 0;
}

int Split_test() {


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


		if (true) {
			printSppointArray(pointsArr, 5);
			printKDarray(firstKDDrray, 5);
		}

		//Split Part
		SPKDArray* left = NULL;
		SPKDArray* right = NULL;

		if (Split(firstKDDrray, &left, &right, 0)==0){
			printf("ok ok ok\n");
		}

		DestroyKDArray(right, right->dim);
		DestroyKDArray(left, left->dim);
		DestroyKDArray(firstKDDrray, firstKDDrray->dim);
		return 0;
}

int main() {
	printf("start\n");
	if (Split_test()==0){
		printf("good");
	}
	else {
		printf("bad");
	}
	printf("end\n");
}
