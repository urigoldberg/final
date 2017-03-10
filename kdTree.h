#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AllocateManager.h"
#ifndef KDTREE_H_
#define KDTREE_H_

#define POINTSNUMBER 3

/*
 * STRUCTS:
 */
typedef struct SPKDArray {
	SPPoint **pointsArr;
	double **sortedMatrix;
	int dim;
	AllocateManager* manager;

} SPKDArray;

/*
typedef struct KDTreeNode {
	int Dim;
	int Val;
	KDTree Left;
	KDTree Right;
	SPPoint Data;
} KDTreeNode;
*\

/*
 * Founctions
 */

typedef struct tempRowForSort {
	double Coor;
	int indexOfOrig;
} tempRowForSort;


SPKDArray* Init(SPPoint** arr, int size);

int SortByPointsIndex(const void* a,const void* b);

#endif /* KDTREE_H_ */


