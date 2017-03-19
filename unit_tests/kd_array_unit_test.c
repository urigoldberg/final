//#include <stdio.h>
//#include <stdlib.h>
//#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
//#include "../kdTree.h"
//#include "../SPPoint.h"
//#include <time.h>
//#include <stdlib.h>
//#include <limits.h>
//#include <float.h>
//
//#define numOfPoints 20
//#define dimOfEachPoint 2
//
//typedef enum Errors {memleak, logicproblem,succeeded } Errors;
//
//void printDoubleMatrix (int** m, int col, int row) {
//	for (int i = 0; i < row; i ++) {
//		printf("row num %d:",i);
//		for (int j = 0; j < col; j++)
//					printf(" %d ", m[i][j]);
//		printf("\n");
//	}
//
//}
//
//void printSppointArray(SPPoint** pointArr, int howmanypoints) {
//
//	if (pointArr == NULL)
//	{
//		printf("its null you syco!\n");
//		return;
//	}
//
//	for (int i = 0 ; i < howmanypoints ; i ++) {
//		printf("\nPoint num %d. has index %d and dim %d\n",i,spPointGetIndex(pointArr[i]),spPointGetDimension(pointArr[i]));
//		printf("its data is :");
//		for (int j = 0; j < spPointGetDimension(pointArr[i]); j ++)
//			printf (" %f ",spPointGetAxisCoor(pointArr[i], j));
//	}
//}
//
//void printKDarray (SPKDArray* firstKDDrray, int howmanypoints) {
//	if (firstKDDrray == NULL)
//		{
//			printf("its null you syco!\n");
//			return;
//		}
//	printf("\nthis KDarray is built like that: dim = %d\n points arr = \n",firstKDDrray->dim);
//	printSppointArray(firstKDDrray->pointsArr,firstKDDrray->dim);
//	printf("matrix arr:\n");
//	printDoubleMatrix(firstKDDrray->sortedMatrix,howmanypoints , spPointGetDimension(firstKDDrray->pointsArr[0]));
//}
///*
////Errors ManagerCheck(){
////
////	//allocate array of points
////	AllocateManager manager;
////
////    SPPoint** pointArr = (SPPoint**)MyMalloc(sizeof(SPPoint*)*numOfPoints, &manager, free);
////    if (pointArr == NULL) {
////        return memleak;
////    }
////
////    //create a random data array and use it for filling the points array
////    double dataArr[numOfPoints][dimOfEachPoint];
////    for (int i = 0; i < numOfPoints; i++)
////    {
////    	for (int j = 0; j < dimOfEachPoint; j++)
////    	    	{
////    	    		dataArr[i][j] = rand();
////    	    	}
////    	//try to create point
////    	pointArr[i] = spPointCreate(dataArr[i], dimOfEachPoint, i);
////    	if (pointArr[i] == NULL)
////    	{
////			DestroyAll(&manager);
////			return memleak;
////    	}
////    	if (AddParamWithoutAllocation(pointArr[i], DestroySPPoint, &manager))
////    	{
////			DestroyAll(&manager);
////			return memleak;
////    	}
////
////    }
////
////
////
////    printf("%f",spPointGetAxisCoor(pointArr[3], 1));
////    printf("%f",dataArr[3][1]);
////
////    AllocateManager* t = &manager;
////
////    printf("array size: %d\n",t->arraySize);
////    printf("array numberofelements: %d\n",t->numOfElements);
////
////    //destroy each value that has been saved in the manager
////    for (int i = 0; i < t->numOfElements; i++){
////    	if (t->founcArray[i] == free) {
////    		printf("free\n");
////    	}
////    	else if (t->founcArray[i]  == DestroySPPoint){
////    		printf("DestroySPPoint\n");
////    	}
////
////    	else {printf("i dont know what is going on\n");}
////    	((void (*)(void*))t->founcArray[i])(t->voidArray[i]);
////    }
////
////
////    printf("%f",spPointGetAxisCoor(pointArr[3], 1));
////    return succeeded;
////}
//*/
//Errors SPKDArray_general_Assert(bool isInit, bool isSplit, bool toPrint){
//
//	//for random
//	srand(time(NULL));
//
//    SPPoint** pointArr = (SPPoint**)calloc(numOfPoints,sizeof(SPPoint*));
//    if (pointArr == NULL) {
//        return memleak;
//    }
//
//    //create a random data array and use it for filling the points array
//    double dataArr[numOfPoints][dimOfEachPoint];
//    for (int i = 0; i < numOfPoints; i++)
//    {
//    	for (int j = 0; j < dimOfEachPoint; j++)
//    	    	{
//    	    		dataArr[i][j] = rand();
//    	    	}
//    	//try to create point
//    	pointArr[i] = spPointCreate(dataArr[i], dimOfEachPoint, i);
//    	if (pointArr[i] == NULL)
//    	{
//			DestroySppointArray(pointArr,numOfPoints );
//			return memleak;
//    	}
//
//    }
//
//    SPKDArray* firstKDDrray = Init( pointArr, numOfPoints);
//
//    if (firstKDDrray == NULL) {
//    	DestroySppointArray(pointArr,numOfPoints );
//    	return memleak;
//
//    }
//
//    if (toPrint){
//    	printSppointArray(pointArr, numOfPoints);
//		printKDarray(firstKDDrray, numOfPoints);
//    }
//
//    //if called from isInit
//    if (isInit) {
//
//
//
//		//Assert KDARRAY contains all the points at the same order:
//		for (int i = 0; i < numOfPoints; i++) {
//			if (spPointL2SquaredDistance(firstKDDrray->pointsArr[i],pointArr[i]) != 0) {
//				printf("failed beacuse of diffrent point\n");
//				return logicproblem;
//			}
//		}
//
//
//		/* Assert KDARRAY matrix:
//		 * verify for each column separately
//		 */
//
//
//		 for (int i = 0; i < dimOfEachPoint; i++)
//			{
//			 int indexFormatrix = 0;
//			for (int j = 0; j < numOfPoints; j++)
//				{
//				double minValue = DBL_MAX;
//				int minIndex = -1;
//				for (int k = 0; k < numOfPoints; k++)
//					{
//						if (dataArr[k][i] < minValue) {
//							minValue = dataArr[k][i];
//							minIndex = k;
//						}
//					}
//				if (firstKDDrray->sortedMatrix[i][indexFormatrix] != minIndex) {
//					printf("sm = %d, k = %d, i = %d, index = %d",firstKDDrray->sortedMatrix[i][indexFormatrix],minIndex,i,indexFormatrix);
//					DestroyKDArray(firstKDDrray, numOfPoints);
//					DestroySppointArray(pointArr,numOfPoints );
//					return logicproblem;
//				}
//				indexFormatrix++;
//				dataArr[minIndex][i] = DBL_MAX;
//				}
//			}
//    	}
//
//    DestroyKDArray(firstKDDrray, numOfPoints);
//    DestroySppointArray(pointArr,numOfPoints);
//    return succeeded;
//}
//
//
////Errors SPKDArray_general_Assert01(bool isInit, bool isSplit){
////
////
////	//allocate array of points
////	AllocateManager manager;
////
////    SPPoint** pointArr = (SPPoint**)MyMalloc(sizeof(SPPoint*)*numOfPoints, &manager, free);
////    if (pointArr == NULL) {
////        return memleak;
////    }
////
////    //create a data array and use it for filling the points array
////    double dataArr[numOfPoints][dimOfEachPoint];
////    for (int i = 0; i < numOfPoints; i++)
////    {
////    	for (int j = 0; j < dimOfEachPoint; j++)
////    	    	{
////    	    		dataArr[i][j] = rand();
////    	    	}
////    	//try to create point
////    	pointArr[i] = spPointCreate(dataArr[i], dimOfEachPoint, i);
////    	if (pointArr[i] == NULL)
////    	{
////			DestroyAll(&manager);
////			return memleak;
////    	}
////    	if (AddParamWithoutAllocation(pointArr[i], DestroySPPoint, &manager))
////    	{
////			DestroyAll(&manager);
////			return memleak;
////    	}
////
////    }
////
////    SPKDArray* firstKDDrray = Init( pointArr, numOfPoints);
////
////    if (firstKDDrray == NULL) {
////    	DestroyAll(&manager);
////    	return memleak;
////
////    }
////
////    //if called from isInit
////    if (isInit) {
////
////		//Assert KDARRAY contains all the points at the same order:
////		for (int i = 0; i < numOfPoints; i++) {
////			if (spPointL2SquaredDistance(firstKDDrray->pointsArr[i],pointArr[i]) != 0) {
////				//printf("shit");
////				return logicproblem;
////			}
////		}
////
////
////		/* Assert KDARRAY matrix:
////		 * verify for each column separately
////		 */
////
////		int indexFormatrix = 0;
////		 for (int i = 0; i < dimOfEachPoint; i++)
////			{
////			for (int j = 0; j < numOfPoints; j++)
////				{
////				double minValue = DBL_MAX;
////				int minIndex = -1;
////				int k;
////				for (k = 0; j < numOfPoints; j++)
////					{
////						if (dataArr[k][i] < minValue) {
////							minValue = dataArr[k][i];
////							minIndex = k;
////						}
////					}
////				if (firstKDDrray->sortedMatrix[i][indexFormatrix] != k) {
////					DestroyAll(firstKDDrray->manager);
////					DestroyAll(&manager);
////					return logicproblem;
////				}
////				indexFormatrix++;
////				dataArr[k][i] = DBL_MAX;
////				}
////			}
////    	}
////
////
////    DestroyAll(firstKDDrray->manager);
////	DestroyAll(&manager);
////    return succeeded;
////}
////
//Errors SPKDArray_Init_Assert(bool toPrint) {
//	return SPKDArray_general_Assert(true,false,toPrint);
//}
////
////void FreeArrayOfSppoints(SPPoint* pointsArr[],int size) {
////	for (int i = 0; i < size; i++) {
////		spPointDestroy(pointsArr[i]);
////	}
////}
////
////Errors SPKDArray_Split_Assert(bool toPrint) {
////
////	SPPoint* pointsArr[5] = {NULL,NULL,NULL,NULL,NULL};
////
////	double data0[] = {1.0,2.0};
////	pointsArr[0] = spPointCreate(data0, 2, 0);
////
////	double data1[] = {123.0,70.0};
////	pointsArr[1] = spPointCreate(data1, 2, 1);
////
////	double data2[] = {2.0,7.0};
////	pointsArr[2] = spPointCreate(data2, 2, 2);
////
////	double data3[] = {9.0,11.0};
////	pointsArr[3] = spPointCreate(data3, 2, 3);
////
////	double data4[] = {3.0,4.0};
////	pointsArr[4] = spPointCreate(data4, 2, 4);
////
////	//Asserts
////	for (int i = 0; i < 5; i++) {
////		if (pointsArr[i] == NULL) {
////			FreeArrayOfSppoints(pointsArr,5);
////			return memleak;
////		}
////	}
////
////	SPKDArray* firstKDDrray = Init( pointsArr, 5);
////
////	if (firstKDDrray == NULL) {
////		FreeArrayOfSppoints(pointsArr,5);
////		return memleak;
////	}
////
////
////	if (toPrint) {
////		printSppointArray(pointsArr, 5);
////		printKDarray(firstKDDrray, 5);
////	}
////
////	//Split Part
////	SPKDArray* left = NULL;
////	SPKDArray* right = NULL;
////
////	////////////////////////////////////////////////////////split it
////	//Asserts
////		if (firstKDDrray == NULL) {
////			return memleak;
////		}
////
////		AllocateManager founcManager;
////
////		//get dimention
////		int dim = firstKDDrray->dim;
////		if (dim <= 0) {
////			return logicproblem;
////		}
////
////		if (firstKDDrray->pointsArr == NULL || firstKDDrray->pointsArr[0] == NULL || firstKDDrray->sortedMatrix == NULL) {
////			return logicproblem;
////		}
////
////		int pointDim = spPointGetDimension(firstKDDrray->pointsArr[0]);
////
////		//create left and right
////		int leftSize = dim % 2 == 0 ? dim / 2 : dim / 2 + 1;
////		int rightSize = dim - leftSize;
////
////		right = (SPKDArray*) malloc(sizeof(SPKDArray));
////		if (right == NULL) {
////			printf("we dead");
////			goto freeAll;
////		}
////
////		left = (SPKDArray*) malloc(sizeof(SPKDArray));
////		if (left == NULL) {
////			printf("we dead");
////			goto freeAll;
////		}
////
////		left->dim = leftSize;
////		right->dim = rightSize;
////
////		left->pointsArr = (SPPoint**) calloc(leftSize,sizeof(SPPoint*));
////
////		if (left->pointsArr == NULL) {
////			printf("we dead");
////			goto freeAll;
////		}
////
////
////		/*//TODO change mem check
////		right->pointsArr = (SPPoint**) MyMalloc(sizeof(SPPoint*) * rightSize,
////					right->manager,free);
////
////		//in case of memory leak
////		if (right->manager == NULL || left->manager == NULL) {
////			DestroyAll(&founcManager);
////			return 1;
////			}
////
////
////		//create map array - each index has
////		//if (map[i] > 0) - map[i] = point i in kdArr->pointsArr is in left kdarr with index  map[i]-1
////		//else - map[i] = point i in kdArr->pointsArr is in right kdarr with index  -(map[i]-1)
////		int *map = (int*) MyMalloc(sizeof(int) * pointDim,&founcManager,free);
////		if (map == NULL) {
////			DestroyAll(right->manager);
////			DestroyAll(left->manager);
////			return 1;
////			}
////
////
////		//create pointArr of left side + fill map with left element
////		for (int i = 0; i < leftSize; i++) {
////			int index = ((kdArr->sortedMatrix[coor][i]));
////			left->pointsArr[i] = spPointCopy(kdArr->pointsArr[index]);
////
////			map[index] = i+1;
////
////			//In case of memory leak
////			if (AddParamWithoutAllocation((void*)(left->pointsArr[i]),DestroySPPoint,left->manager) != 0) {
////				DestroyAll(&founcManager);
////				return 1;
////				}
////		}
////
////		//create pointArr of right side + fill map with left element
////		for (int i = 0; i < rightSize; i++) {
////			int index = ((kdArr->sortedMatrix[coor][leftSize - 1 + i]));
////			right->pointsArr[i] = spPointCopy(kdArr->pointsArr[index]);
////
////			map[index] = -1*(i+1);
////
////			//In case of memory leak
////			if (AddParamWithoutAllocation((void*)(right->pointsArr[i]),DestroySPPoint,right->manager) != 0) {
////				DestroyAll(&founcManager);
////				return 1;
////			}
////		}
////
////		//create matrix left
////		left->sortedMatrix = (int**) MyMalloc(sizeof(int*)*pointDim,left->manager,free);
////		if (left->sortedMatrix == NULL) {
////			DestroyAll(&founcManager);
////			DestroyAll(right->manager);
////			return 1;
////		}
////
////		//create matrix right
////		right->sortedMatrix = (int**) MyMalloc(sizeof(int*)*pointDim,right->manager,free);
////		if (right->sortedMatrix == NULL) {
////			DestroyAll(&founcManager);
////			DestroyAll(left->manager);
////			return 1;
////			}
////
////		//fill dXn matrix
////		for (int i = 0; i < pointDim; i++) {
////
////			//allocate the i-th row
////			left->sortedMatrix[i] = (int*) MyMalloc(sizeof(int) * leftSize,
////					left->manager,free);
////			right->sortedMatrix[i] = (int*) MyMalloc(sizeof(int) * rightSize,
////					right->manager,free);
////
////			//in case of memory leak or i-th row doesnt exsists in the original kdArr
////			if (left->sortedMatrix[i] == NULL || right->sortedMatrix[i] == NULL || kdArr->sortedMatrix[i] == NULL) {
////				DestroyAll(&founcManager);
////				DestroyAll(left->manager);
////				DestroyAll(right->manager);
////				return 1;
////			}
////
////			int whereInLeft = 0;
////			int whereInRight = 0;
////
////			//fill the i-th row
////			for (int j = 0; j < dim; j++) {
////				int forMapCheck = kdArr->sortedMatrix[i][j];
////				int afterMapCheck = map[forMapCheck];
////
////				//point in left array
////				if (afterMapCheck > 0) {
////					afterMapCheck = afterMapCheck - 1;
////					left->sortedMatrix[i][whereInLeft] = afterMapCheck;
////					whereInLeft++;
////				}
////
////				//point in right array
////				else {
////					afterMapCheck = (-1*afterMapCheck) - 1;
////					right->sortedMatrix[i][whereInRight] = afterMapCheck;
////					whereInRight++;
////				}
////			}
////		}
////
////		free(map);
////		DestroyManager(&founcManager);
////		return 0;
////	}*/
////	////////////////////////////////////////////////////////
/////*	if (Split(firstKDDrray, left, right, 0) == 1){
////		goto freeAll;
////	}*/
////
////
////
////	/*if (toPrint) {
////		printf("left:\n");
////		printKDarray(left, 5);
////		printf("right:\n");
////		printKDarray(right, 5);
////	}
////
////	//Assert left array splitted arrays
////	for (int i = 0; i < 3; i++) {
////		if (spPointL2SquaredDistance(firstKDDrray->pointsArr[i],left->pointsArr[i]) != 0)
////			printf("dead left\n");
////			goto freeAll;
////	}
////
////	for (int i = 3; i < 5; i++) {
////		if (spPointL2SquaredDistance(firstKDDrray->pointsArr[i],right->pointsArr[i]) != 0)
////			printf("dead right\n");
////			goto freeAll;
////	}
////
////
////	goto freeAll;
////	freeAll:
////	DestroyAll(right->manager);*/
////	//DestroyAll(left->manager);
////	freeAll:
////	free(right);
////	free(left);
////	FreeArrayOfSppoints(left->pointsArr,3);
////	FreeArrayOfSppoints(pointsArr,5);
////	DestroyAll(firstKDDrray->manager);
////
////	return succeeded;
////
////}
////
////
//int main(int argc, char **argv) {
//	printf("test started\n");
//
//	//init
//	if (SPKDArray_Init_Assert(false) == succeeded){
//		printf("Init TIL BALISTI\n");
//	}
//	else {
//		printf("Init failed\n");
//	}
//
////	if (SPKDArray_Split_Assert(true) == succeeded){
////		printf("Split BALISTI\n");
////	}
////	else {
////		printf("Split failed\n");
////	}
//
//	return 0;
//}
