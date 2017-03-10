

#include <stdio.h>
#include <stdlib.h>
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
#include "AllocateManager.h"
#include "kd_tree.h"

#define POINTSNUMBER 3




KDArray Init(SPPoint** arr, int size)  {

	AllocateManager manager;

	KDArray res;

	//Asserts
	if ((arr == NULL) || (size == 0)) {
		return res;
	}

	//create KDArray object
	res = MyMalloc(sizeof(KDArray),5,manager);
	if (res == NULL) {
		return NULL;
	}

	res.size = size;

	//Copy the array that received in the init function
	res.P = (SPPoint**)MyMalloc(sizeof(SPPoint)*size,?,manager);
	if (res == NULL) {
			return NULL;
		}
	for (int i = 0; i < size; i++) {
		res.P[i] = spPointCopy(arr[i]); //To Be Checked Later
	}


	// Create a dXn
	int d = spPointGetDimension(arr);
	res.A = (double**)MyMalloc(d*sizeof(double*),?,manager);


	ForMatrixSort* tempMatrix  = (ForMatrixSort*)realloc(tempMatrix, size*sizeof(ForMatrixSort));
	if (tempMatrix == NULL) {
		DestroyAll(manager);
		return NULL;
	}


	for (int i = 0; i < d; i++) {

		res.A[i] = (double*)MyMalloc(sizeof(double)*d,9,manager);
		if (res.A[i] == NULL) {
			free(tempMatrix);
			return NULL;
		}

		for (int j = 0; j < size; j++) {
			tempMatrix[j].Coor = spPointGetAxisCoor(arr[j],i); //to be checked later...
			tempMatrix[j].indexOfOrig = j;
		}

		//sort array by index
		qsort(tempMatrix,size,sizeof(ForMatrixSort),SortByPointsIndex);


		//update array with indexes after sorting
		for (int k = 0; k < size; k++) {
			res.A[i][k] =  tempMatrix[k].indexOfOrig;
		}

	}

	//free resources
	DestroyManager(manager);
	free(tempMatrix);

	return res;
}

int SortByPointsIndex(const void* a,const void* b)
	{

	ForMatrixSort First = (*(ForMatrixSort*)a);
	ForMatrixSort Second = (*(ForMatrixSort*)b);


	return ( First.Coor - Second.Coor );
	}




