

#include <stdio.h>
#include <stdlib.h>
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
#include "AllocateManager.h"

#define POINTSNUMBER 3

/*
 * STRUCTS:
 */
typedef struct KDArray {
	SPPoint **P;
	double **A;
	int size;

} KDArray;


typedef struct KDTreeNode {
	int Dim;
	int Val;
	KDTree Left;
	KDTree Right;
	SPPoint Data;
} KDTreeNode;

typedef struct ForMatrixSort {
	double Coor;
	int indexOfOrig;
} ForMatrixSort;

/*
 * Founctions
 */

KDArray Init(SPPoint** arr, int size);

int SortByPointsIndex(const void* a,const void* b);




