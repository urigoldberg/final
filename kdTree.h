

#ifndef _SPPointh__
#define _SPPointh__
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define POINTSNUMBER 3

/*
 * STRUCTS:
 */
typedef struct KDArray {
	SPPoint **pointsArr;
	double **sortedMatrix;
	int dim;
} KDArray;

/*
typedef struct KDTreeNode {
	int Dim;
	int Val;
	KDTree Left;
	KDTree Right;
	SPPoint Data;
} KDTreeNode;*\

typedef struct tempRowForSort {
	double Coor;
	int indexOfOrig;
} tempRowForSort;

/*
 * Founctions
 */

KDArray Init(SPPoint** arr, int size);

int SortByPointsIndex(const void* a,const void* b);




