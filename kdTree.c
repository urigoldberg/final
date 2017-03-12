#include "kdTree.h"
#include "SPPoint.h"
#include "AllocateManager.h"
#include <time.h>
#define POINTSNUMBER 3

/***************
 *             *
 *    PART I   *
 *    UTILS    *
 *             *
 ***************
 */

void DestroyKdTreeNode (KDTreeNode* treeNode) {
	free(treeNode->Data);
	free(treeNode->Left);
	free(treeNode->Right);
	free(treeNode);
}

void DestroyKdTree (KDTreeNode* tree) {
	while (tree->Left != NULL) {
		DestroyKdTree(tree->Left);
	}

	while (tree->Right != NULL) {
			DestroyKdTree(tree->Right);
		}
	DestroyKdTreeNode(tree);

}

int GetRandomIndex (int dim) {
	srand(time(NULL));
	return rand()%dim;
}

int FindMaxSpread (int columns, int rows, SPKDArray* kdArray) {
	double max = -1;
	int indexOfMax = -1;

	//search max spread
	for (int i = 0; i < rows; i++){
		int indexOfMinPoint = (int)kdArray->sortedMatrix[i][0];
		int indexOfMaxPoint = (int)kdArray->sortedMatrix[i][columns-1];

		int minIndex = spPointGetAxisCoor(kdArray->pointsArr[indexOfMinPoint],i);
		int maxIndex = spPointGetAxisCoor(kdArray->pointsArr[indexOfMaxPoint],i);

		//if there are several candidates choose the lowest dimension
		if ((maxIndex - minIndex) > max) {
			max = (maxIndex - minIndex);
			indexOfMax = i;
		}
	}

	return indexOfMax;

}

/***************
 *             *
 *    PART II  *
 *  KD - ARRAY *
 *             *
 ***************
 */

int SortByPointsIndex(const void* a, const void* b) {
	tempRowForSort First = (*(tempRowForSort*) a);
	tempRowForSort Second = (*(tempRowForSort*) b);
	return (First.Coor - Second.Coor);
}


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
	int pointDim = spPointGetDimension(arr[0]);
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
			free(res);
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

int Split(SPKDArray* kdArr, SPKDArray * left, SPKDArray * right, int coor) {

	//Asserts
	if (kdArr == NULL) {
		return 1;
	}

	AllocateManager founcManager;

	//get dimention
	int dim = kdArr->dim;
	if (dim <= 0) {
		return 1;
	}

	if (kdArr->pointsArr == NULL || kdArr->pointsArr[0] == NULL || kdArr->sortedMatrix == NULL) {
		return 1;
	}

	int pointDim = spPointGetDimension(kdArr->pointsArr[0]);

	//create left and right
	int leftSize = dim % 2 == 0 ? dim / 2 : dim / 2 + 1;
	int rightSize = dim - leftSize;

	right = (SPKDArray*)MyMalloc(sizeof(SPKDArray),&founcManager,free);
	left = (SPKDArray*)MyMalloc(sizeof(SPKDArray),&founcManager,free);

	left->dim = leftSize;
	right->dim = rightSize;

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


	//create map array - each index has
	//if (map[i] > 0) - map[i] = point i in kdArr->pointsArr is in left kdarr with index  map[i]-1
	//else - map[i] = point i in kdArr->pointsArr is in right kdarr with index  -(map[i]-1)
	double *map = (double*) MyMalloc(sizeof(double) * pointDim,&founcManager,free);
	if (map == NULL) {
		DestroyAll(right->manager);
		DestroyAll(left->manager);
		return 1;
		}


	//create pointArr of left side + fill map with left element
	for (int i = 0; i < leftSize; i++) {
		int index = ((int)(kdArr->sortedMatrix[coor][i]));
		left->pointsArr[i] = spPointCopy(kdArr->pointsArr[index]);

		map[index] = i+1;

		//In case of memory leak
		if (AddParamWithoutAllocation((void*)(left->pointsArr[i]),DestroySPPoint,left->manager) != 0) {
			DestroyAll(&founcManager);
			return 1;
			}
	}

	//create pointArr of right side + fill map with left element
	for (int i = 0; i < rightSize; i++) {
		int index = ((int)(kdArr->sortedMatrix[coor][leftSize - 1 + i]));
		right->pointsArr[i] = spPointCopy(kdArr->pointsArr[index]);

		map[index] = -1*(i+1);

		//In case of memory leak
		if (AddParamWithoutAllocation((void*)(right->pointsArr[i]),DestroySPPoint,right->manager) != 0) {
			DestroyAll(&founcManager);
			return 1;
		}
	}

	//create matrix left
	left->sortedMatrix = (double**) MyMalloc(sizeof(double*)*pointDim,left->manager,free);
	if (left->sortedMatrix == NULL) {
		DestroyAll(&founcManager);
		DestroyAll(right->manager);
		return 1;
	}

	//create matrix right
	right->sortedMatrix = (double**) MyMalloc(sizeof(double*)*pointDim,right->manager,free);
	if (right->sortedMatrix == NULL) {
		DestroyAll(&founcManager);
		DestroyAll(left->manager);
		return 1;
		}

	//fill dXn matrix
	for (int i = 0; i < pointDim; i++) {

		//allocate the i-th row
		left->sortedMatrix[i] = (double*) MyMalloc(sizeof(double) * leftSize,
				left->manager,free);
		right->sortedMatrix[i] = (double*) MyMalloc(sizeof(double) * rightSize,
				right->manager,free);

		//in case of memory leak or i-th row doesnt exsists in the original kdArr
		if (left->sortedMatrix[i] == NULL || right->sortedMatrix[i] == NULL || kdArr->sortedMatrix[i] == NULL) {
			DestroyAll(&founcManager);
			DestroyAll(left->manager);
			DestroyAll(right->manager);
			return 1;
		}

		int whereInLeft = 0;
		int whereInRight = 0;

		//fill the i-th row
		for (int j = 0; j < dim; j++) {
			int forMapCheck = kdArr->sortedMatrix[i][j];
			int afterMapCheck = map[forMapCheck];

			//point in left array
			if (afterMapCheck > 0) {
				afterMapCheck = afterMapCheck - 1;
				left->sortedMatrix[i][whereInLeft] = afterMapCheck;
				whereInLeft++;
			}

			//point in right array
			else {
				afterMapCheck = (-1*afterMapCheck) - 1;
				right->sortedMatrix[i][whereInRight] = afterMapCheck;
				whereInRight++;
			}
		}
	}

	free(map);
	DestroyManager(&founcManager);
	return 0;
}

/***************
 *             *
 *  PART III   *
 *  KD - Tree  *
 *             *
 ***************
 */

KDTreeNode* InitKdTreeNode(int Dim,int Val, KDTreeNode *Left,KDTreeNode *Right,SPPoint* Data){

	KDTreeNode *res = (KDTreeNode*) malloc(sizeof(KDTreeNode));
	if (res == NULL) {
		return NULL;
	}

	res->Data = spPointCopy(Data);

	if (res->Data == NULL) {
		free(res);
	}

	res->Dim = Dim;
	res->Left = Left;
	res->Right = Right;
	res->Val = Val;

	return res;
}

KDTreeNode* InitKdTreeFromKdArray (SPKDArray* kdArray, spKDTreeSplitMethod SpCriteria,int forINCREMENTAL) {

	//Asserts
	if (kdArray == NULL || kdArray->pointsArr == NULL || kdArray->sortedMatrix == NULL) {
		return NULL;
	}

	//stop condition
	if (kdArray->dim == 1) {
		return InitKdTreeNode(-1,-1,NULL,NULL,kdArray->pointsArr[0]);
	}

	//find split index
	int dimOfPoints = spPointGetDimension(kdArray->pointsArr[0]);
	int howManyPoints = kdArray->dim;

	int i_thRow;
	if (SpCriteria == MAX_SPREAD) {
		i_thRow = FindMaxSpread(howManyPoints,dimOfPoints,kdArray);
	}
	else if (SpCriteria == RANDOM) {
		i_thRow = GetRandomIndex(dimOfPoints);
	}

	else {
		i_thRow = (forINCREMENTAL+1)%dimOfPoints;
	}

	//create tree
	KDTreeNode* res = (KDTreeNode*)malloc(sizeof(KDTreeNode));
	if (res == NULL) {
		return NULL;
	}

	//split KDArray
	SPKDArray * leftArr = NULL;
	SPKDArray * rightArr = NULL;

	//split did not succeeded
	if (Split(kdArray, leftArr, rightArr, i_thRow)) {
		free(res);
		return NULL;
	}


	//create left tree
	KDTreeNode* leftTree = InitKdTreeFromKdArray(leftArr,SpCriteria,forINCREMENTAL);
	if (leftTree == NULL) {
		free(res);
		DestroyAll(leftArr->manager);
		DestroyAll(rightArr->manager);
		return NULL;
	}

	KDTreeNode* rightTree = InitKdTreeFromKdArray(rightArr,SpCriteria,forINCREMENTAL);
	if (rightTree == NULL) {
		free(res);
		DestroyAll(leftArr->manager);
		DestroyAll(rightArr->manager);
		DestroyTree(leftArr);
		return NULL;
	}

	DestroyAll(leftArr->manager);
	DestroyAll(rightArr->manager);

	return InitKdTreeNode(i_thRow,???,leftTree,rightTree,NULL);

}


