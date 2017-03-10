/*
 * AllocateManager.c
 *
 *  Created on: Mar 9, 2017
 *      Author: uri
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kd_tree.h"

/*
 * Documentation about this struct *
 */

typedef struct AllocateManager {

	int IntArraySize;
	int** IntArray;

	int CharArraySize;
	char** CharArray;

	int DoubleArraySize;
	double** DoubleArray;

	int DoubleDoubleArraySize;
	double*** DoubleDoubleArray;

	int KdArraySize;
	KDArray *KdArray;

	int SPPointArrayArraySize;
	SPPoint** SPPointArrayArray;

	int SPPointArraySize;
	SPPoint* SPPointArray;


} AllocateManager;

/*
 * instad of malloc
 */

void* MyMalloc (size_t size, char* kindOfPty, AllocateManager manager);


/*
 * 1 - array of integers
 * 2 - array of chars
 * 3 - array of double
 * 4 - array of array of double
 * 5 - KDArray
 * 6 - SPPointArrayArray
 * 7 - SPPointArray
 */

void DestroyAll (AllocateManager manager);

void DestroyManager(AllocateManager manager);

