#include <stdio.h>
#include <stdlib.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../kdTree.h"
#include "../AllocateManager.h"
#include "../SPPoint.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#define numOfPoints 6
#define dimOfEachPoint 2

typedef enum Errors {memleak, logicproblem,succeeded } Errors;

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

Errors ManagerCheck(){

	//allocate array of points
	AllocateManager manager;

    SPPoint** pointArr = (SPPoint**)MyMalloc(sizeof(SPPoint*)*numOfPoints, &manager, free);
    if (pointArr == NULL) {
        return memleak;
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
			DestroyAll(&manager);
			return memleak;
    	}
    	if (AddParamWithoutAllocation(pointArr[i], DestroySPPoint, &manager))
    	{
			DestroyAll(&manager);
			return memleak;
    	}

    }



    printf("%f",spPointGetAxisCoor(pointArr[3], 1));
    printf("%f",dataArr[3][1]);

    AllocateManager* t = &manager;

    printf("array size: %d\n",t->arraySize);
    printf("array numberofelements: %d\n",t->numOfElements);

    //destroy each value that has been saved in the manager
    for (int i = 0; i < t->numOfElements; i++){
    	if (t->founcArray[i] == free) {
    		printf("free\n");
    	}
    	else if (t->founcArray[i]  == DestroySPPoint){
    		printf("DestroySPPoint\n");
    	}

    	else {printf("i dont know what is going on\n");}
    	((void (*)(void*))t->founcArray[i])(t->voidArray[i]);
    }


    printf("%f",spPointGetAxisCoor(pointArr[3], 1));
    return succeeded;
}

Errors SPKDArray_general_Assert(bool isInit, bool isSplit){

	//for random
	srand(time(NULL));

	//allocate array of points
	AllocateManager manager;

    SPPoint** pointArr = (SPPoint**)MyMalloc(sizeof(SPPoint*)*numOfPoints, &manager, free);
    if (pointArr == NULL) {
        return memleak;
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
			DestroyAll(&manager);
			return memleak;
    	}
    	if (AddParamWithoutAllocation(pointArr[i], DestroySPPoint, &manager))
    	{
			DestroyAll(&manager);
			return memleak;
    	}

    }
    printf("\n");

    SPKDArray* firstKDDrray = Init( pointArr, numOfPoints);

    if (firstKDDrray == NULL) {
    	DestroyAll(&manager);
    	return memleak;

    }

    //if called from isInit
    if (isInit) {

    	printSppointArray(pointArr, numOfPoints);
    	printKDarray(firstKDDrray, numOfPoints);

		//Assert KDARRAY contains all the points at the same order:
		for (int i = 0; i < numOfPoints; i++) {
			if (spPointL2SquaredDistance(firstKDDrray->pointsArr[i],pointArr[i]) != 0) {
				printf("failed beacuse of diffrent point\n");
				return logicproblem;
			}
		}


		/* Assert KDARRAY matrix:
		 * verify for each column separately
		 */


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
					DestroyAll(firstKDDrray->manager);
					DestroyAll(&manager);
					return logicproblem;
				}
				indexFormatrix++;
				dataArr[minIndex][i] = DBL_MAX;
				}
			}
    	}


    DestroyAll(firstKDDrray->manager);
	DestroyAll(&manager);
    return succeeded;
}




Errors SPKDArray_general_Assert01(bool isInit, bool isSplit){


	//allocate array of points
	AllocateManager manager;

    SPPoint** pointArr = (SPPoint**)MyMalloc(sizeof(SPPoint*)*numOfPoints, &manager, free);
    if (pointArr == NULL) {
        return memleak;
    }

    //create a data array and use it for filling the points array
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
			DestroyAll(&manager);
			return memleak;
    	}
    	if (AddParamWithoutAllocation(pointArr[i], DestroySPPoint, &manager))
    	{
			DestroyAll(&manager);
			return memleak;
    	}

    }

    SPKDArray* firstKDDrray = Init( pointArr, numOfPoints);

    if (firstKDDrray == NULL) {
    	DestroyAll(&manager);
    	return memleak;

    }

    //if called from isInit
    if (isInit) {

		//Assert KDARRAY contains all the points at the same order:
		for (int i = 0; i < numOfPoints; i++) {
			if (spPointL2SquaredDistance(firstKDDrray->pointsArr[i],pointArr[i]) != 0) {
				//printf("shit");
				return logicproblem;
			}
		}


		/* Assert KDARRAY matrix:
		 * verify for each column separately
		 */

		int indexFormatrix = 0;
		 for (int i = 0; i < dimOfEachPoint; i++)
			{
			for (int j = 0; j < numOfPoints; j++)
				{
				double minValue = DBL_MAX;
				int minIndex = -1;
				int k;
				for (k = 0; j < numOfPoints; j++)
					{
						if (dataArr[k][i] < minValue) {
							minValue = dataArr[k][i];
							minIndex = k;
						}
					}
				if (firstKDDrray->sortedMatrix[i][indexFormatrix] != k) {
					DestroyAll(firstKDDrray->manager);
					DestroyAll(&manager);
					return logicproblem;
				}
				indexFormatrix++;
				dataArr[k][i] = DBL_MAX;
				}
			}
    	}


    DestroyAll(firstKDDrray->manager);
	DestroyAll(&manager);
    return succeeded;
}

Errors SPKDArray_Init_Assert() {
	return SPKDArray_general_Assert(true,false);
}


Errors SPKDArray_Split_Assert() {
	return SPKDArray_general_Assert(false,true);
}


int main(int argc, char **argv) {
	printf("test started\n");

	if (SPKDArray_Init_Assert() == succeeded){
		printf("Init TIL BALISTI\n");
	}

	else {
		printf("Init failed\n");
	}



	printf("test finished\n");
	return 0;
}
