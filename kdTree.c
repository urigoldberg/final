#include "kdTree.h"
#include "SPPoint.h"
#include "AllocateManager.h"
#define POINTSNUMBER 3

SPKDArray* Init(SPPoint** arr, int size) {

	//Asserts
	if ((arr == NULL) || (size == 0)) {
		return NULL;
	}

	SPKDArray *res = (SPKDArray*) malloc(sizeof(SPKDArray));
	if (res == NULL) {
		return NULL;
	}

	res->manager = (AllocateManager*) malloc(sizeof(AllocateManager));
	if (res->manager == NULL) {
		free(res);
		return NULL;
	}

	res->dim = size;

	//Copy the array that received in the init function
	res->pointsArr = (SPPoint**) MyMalloc(sizeof(SPPoint*) * size,
			res->manager,free);
	if (res->pointsArr == NULL) {
		free(res);
		return NULL;
	}

	for (int i = 0; i < size; i++) {
		res->pointsArr[i] = spPointCopy(arr[i]); //To Be Checked Later
		if (AddParamWithoutAllocation((void*)(res->pointsArr[i]),DestroySPPoint,res->manager) != 0) {
			free(res);
			return NULL;
		}
	}

	// Create a dXn
	int pointDim = spPointGetDimension(arr);
	res->sortedMatrix = (double**) MyMalloc(pointDim * sizeof(double*),
			res->manager,free);
	if (res->sortedMatrix == NULL) {
		free(res);
		return NULL;
	}

	tempRowForSort* tempRow = (tempRowForSort*) malloc(
			size * sizeof(tempRowForSort));
	if (tempRow == NULL) {
		DestroyAll(res->manager);
		free(res);
		return NULL;
	}

	for (int i = 0; i < pointDim; i++) {

		res->sortedMatrix[i] = (double*) MyMalloc(sizeof(double) * pointDim,
				res->manager,free);
		if (res->sortedMatrix[i] == NULL) {
			free(tempRow);
			return NULL;
		}

		for (int j = 0; j < size; j++) {
			tempRow[j].Coor = spPointGetAxisCoor(arr[j], i); //to be checked later...
			tempRow[j].indexOfOrig = j;
		}

		//sort tempRow by index
		qsort(tempRow, size, sizeof(tempRowForSort), SortByPointsIndex);

		//update array with indexes after sorting
		for (int k = 0; k < size; k++) {
			res->sortedMatrix[i][k] = tempRow[k].indexOfOrig;
		}
	}

	//free resources
	free(tempRow);

	return res;
}

int SortByPointsIndex(const void* a, const void* b) {
	tempRowForSort First = (*(tempRowForSort*) a);
	tempRowForSort Second = (*(tempRowForSort*) b);
	return (First.Coor - Second.Coor);
}

int Split(SPKDArray* kdArr, SPKDArray * left, SPKDArray * right, int coor) {

	//Asserts
	if (kdArr == NULL || left == NULL || right == NULL) {
		return 1;
	}

	AllocateManager founcManager;

	//get dimention
	int dim = kdArr->dim;
	if (dim <= 0) {
		return 1;
	}

	if (kdArr->pointsArr == NULL || kdArr->pointsArr[0] == NULL) {
		return 1;
	}

	int pointDim = spPointGetDimension(kdArr->pointsArr[0]);

	//create left and right
	int leftSize = dim % 2 == 0 ? dim / 2 : dim / 2 + 1;
	int rightSize = dim - leftSize;

	right = (SPKDArray*)MyMalloc(sizeof(SPKDArray),&founcManager,free);
	left = (SPKDArray*)MyMalloc(sizeof(SPKDArray),&founcManager,free);

	//in case of memory leak
	if (right == NULL || left == NULL) {
		return 1;
	}

	right->manager = (AllocateManager*) MyMalloc(sizeof(AllocateManager),&founcManager,free);
	left->manager = (AllocateManager*) MyMalloc(sizeof(AllocateManager),&founcManager,free);

	//in case of memory leak
	if (right->manager == NULL || left->manager == NULL) {
		return 1;
	}

	left->pointsArr = (SPPoint**) MyMalloc(sizeof(SPPoint*) * leftSize,
			left->manager,free);
	right->pointsArr = (SPPoint**) MyMalloc(sizeof(SPPoint*) * rightSize,
				right->manager,free);

	//in case of memory leak
	if (right->manager == NULL || left->manager == NULL) {
		DestroyAll(&founcManager);
		return 1;
		}


	for (int i = 0; i < leftSize; i++) {
		left->pointsArr[i] = spPointCopy(kdArr->pointsArr[((int)(kdArr->sortedMatrix[coor][i]))]);

		//In case of memory leak
		if (AddParamWithoutAllocation((void*)(left->pointsArr[i]),DestroySPPoint,left->manager) != 0) {
			DestroyAll(&founcManager);
			return 1;
			}
	}
	for (int i = 0; i < rightSize; i++) {
		right->pointsArr[i] = spPointCopy(kdArr->pointsArr[((int)(kdArr->sortedMatrix[coor][leftSize - 1 + i]))]);
		//In case of memory leak
		if (AddParamWithoutAllocation((void*)(right->pointsArr[i]),DestroySPPoint,right->manager) != 0) {
			DestroyAll(&founcManager);
			return 1;
		}
	}

	left->sortedMatrix = (double**) MyMalloc(sizeof(double*)*pointDim,left->manager,free);
	if (left->sortedMatrix == NULL) {
		DestroyAll(&founcManager);
		DestroyAll(right->manager);
		return 1;
	}

	right->sortedMatrix = (double**) MyMalloc(sizeof(double*)*pointDim,right->manager,free);
	if (right->sortedMatrix == NULL) {
		DestroyAll(&founcManager);
		DestroyAll(left->manager);
		return 1;
		}
}

