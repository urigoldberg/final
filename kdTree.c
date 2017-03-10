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
			res->manager);
	if (res->pointsArr == NULL) {
		free(res);
		return NULL;
	}

	for (int i = 0; i < size; i++) {
		res->pointsArr[i] = spPointCopy(arr[i]); //To Be Checked Later
	}

	// Create a dXn
	int pointDim = spPointGetDimension(arr);
	res->sortedMatrix = (double**) MyMalloc(pointDim * sizeof(double*),
			res->manager);
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
				res->manager);
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

int Split(SPKDArray kdArr, SPKDArray * left, SPKDArray * right, int coor) {
	if (kdArr == NULL || left == NULL || right == NULL) {
		return 1;
	}

	int dim = kdArr->dim;
	if (dim <= 0) {
		return 1;
	}

	if (kdArr->pointsArr == NULL || kdArr->pointsArr[0] == NULL) {
		return 1;
	}
	int pointDim = kdArr->pointsArr[0]->dim;

	int leftSize = dim % 2 == 0 ? dim / 2 : dim / 2 + 1;
	int rightSize = dim - leftSize;
	right = (SPKDArray*) malloc(sizeof(SPKDArray));
	left = (SPKDArray*) malloc(sizeof(SPKDArray));
	// TODO malloc check

	right->manager = (AllocateManager*) malloc(sizeof(AllocateManager));
	left->manager = (AllocateManager*) malloc(sizeof(AllocateManager));

	left->pointsArr = (SPPoint*) malloc(sizeof(SPPoint) * leftSize);
	right->pointsArr = (SPPoint*) malloc(sizeof(SPPoint) * rightSize);

	for (int i = 0; i < leftSize; i++) {
		left->pointsArr[i] = kdArr->pointsArr[kdArr->sortedMatrix[coor][i]];
	};
	for (int i = 0; i < rightSize; i++) {
		right->pointsArr[i] = kdArr->pointsArr[kdArr->sortedMatrix[coor][leftSize - 1 + i]];
	};

	left->sortedMatrix = (double**) malloc(sizeof(double*)*pointDim);
	right->sortedMatrix = (double**) malloc(sizeof(double*)*pointDim)

}

