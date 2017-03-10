
#include "kdTree.h"
#include "SPPoint.h"
#include "AllocateManager.h"
#define POINTSNUMBER 3



KDArray* Init(SPPoint** arr, int size)  {

	//Asserts
	if ((arr == NULL) || (size == 0)) {
		return NULL;
	}


	KDArray *res = (KDArray*)malloc(sizeof(KDArray));
	if (res == NULL) {
		return NULL;
		}

	res->manager = (AllocateManager*)malloc(sizeof(AllocateManager));
	if (res->manager == NULL) {
		free(res);
		return NULL;
		}

	res->dim = size;

	//Copy the array that received in the init function
	res->pointsArr = (SPPoint**)MyMalloc(sizeof(SPPoint*)*size,res->manager);
	if (res->pointsArr == NULL) {
		free(res);
		return NULL;
		}


	for (int i = 0; i < size; i++) {
		res->pointsArr[i] = spPointCopy(arr[i]); //To Be Checked Later
	}


	// Create a dXn
	int pointDim = spPointGetDimension(arr);
	res->sortedMatrix = (double**)MyMalloc(pointDim*sizeof(double*),res->manager);
	if (res->sortedMatrix == NULL) {
		free(res);
		return NULL;
	}


	tempRowForSort* tempRow  = (tempRowForSort*)malloc(size*sizeof(tempRowForSort));
	if (tempRow == NULL) {
		DestroyAll(res->manager);
		free(res);
		return NULL;
	}

	for (int i = 0; i < pointDim; i++) {

		res->sortedMatrix[i] = (double*)MyMalloc(sizeof(double)*pointDim,res->manager);
		if (res->sortedMatrix[i] == NULL) {
			free(tempRow);
			return NULL;
		}

		for (int j = 0; j < size; j++) {
			tempRow[j].Coor = spPointGetAxisCoor(arr[j],i); //to be checked later...
			tempRow[j].indexOfOrig = j;
		}

		//sort tempRow by index
		qsort(tempRow,size,sizeof(tempRowForSort),SortByPointsIndex);


		//update array with indexes after sorting
		for (int k = 0; k < size; k++) {
			res->sortedMatrix[i][k] =  tempRow[k].indexOfOrig;
		}
	}

	//free resources
	DestroyAll(res->manager);
	free(tempRow);

	return res;
}

int SortByPointsIndex(const void* a,const void* b)
	{
	tempRowForSort First = (*(tempRowForSort*)a);
	tempRowForSort Second = (*(tempRowForSort*)b);
	return ( First.Coor - Second.Coor );
	}




