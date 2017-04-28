
#ifndef KDTREE_H_
#define KDTREE_H_
#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPBPriorityQueue.h"
#define POINTSNUMBER 3

/*
 * STRUCTS:
 */
typedef struct SPKDArray {
	SPPoint **pointsArr;
	int **sortedMatrix;
	int dim;

} SPKDArray;


 typedef struct KDTreeNode{
	int Dim;
	int Val;
	struct KDTreeNode *Left;
	struct KDTreeNode *Right;
	SPPoint *Data;
} KDTreeNode;

typedef struct tempRowForSort {
	double Coor;
	int indexOfOrig;
} tempRowForSort;


/*
 * enums
 */

typedef enum spKDTreeSplitMethod  {RANDOM, MAX_SPREAD, INCREMENTAL } spKDTreeSplitMethod;

/*
 * Founctions
 */



SPKDArray* Init(SPPoint** arr, int size);

int SortByPointsIndex(const void* a,const void* b);

void DestroyKdTree (KDTreeNode* tree);

void kNearestNeighbors(KDTreeNode *curr, SPBPQueue *bpq, SPPoint *point);

KDTreeNode *InitKdTreeFromKdArray(SPKDArray *kdArray, spKDTreeSplitMethod SpCriteria, int forINCREMENTAL, int PcadDimension);

void DestroyKDArray(SPKDArray *arr, int numOfPoints);

void DestroySppointArray(SPPoint **pointArr, int howmanypoints);

#endif /* KDTREE_H_ */


