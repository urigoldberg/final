/*
 * AllocateManager.c
 *
 *  Created on: Mar 9, 2017
 *      Author: uri
 */


#include "AllocateManager.h"



void* MyMalloc (size_t size, char* kindOfPty, AllocateManager manager) {

	void* resultptr = malloc(size);

	//in case of memory leak - free all and return null
	if (resultptr == NULL) {
		DestroyAll (manager);
		return NULL;
	}

	if (strcmp(kindOfPty, "int*") == 0)
	{
		manager.IntArraySize++;
		manager.IntArray = (int**)realloc(manager.IntArray,manager.IntArraySize*sizeof(int*));
		if (manager.IntArray == NULL) {
			DestroyAll(manager);
			return NULL;
		}
		manager.IntArray[manager.IntArraySize-1] = (int*)resultptr;
	}

	else if (strcmp(kindOfPty, "char*") == 0)
		{
		manager.CharArraySize++;
		manager.CharArray = (int**)realloc(manager.CharArray,manager.CharArraySize*sizeof(int*));
		if (manager.CharArray == NULL)
			{
			DestroyAll(manager);
			return NULL;
			}
		manager.CharArray[manager.CharArraySize-1] = (char*)resultptr;
		}

	else if (strcmp(kindOfPty, "double*") == 0)
		{
		manager.DoubleArraySize++;
		manager.DoubleArray = (int**)realloc(manager.DoubleArray,manager.DoubleArraySize*sizeof(int*));
		if (manager.DoubleArray == NULL)
			{
			DestroyAll(manager);
			return NULL;
			}
		manager.DoubleArray[manager.DoubleArraySize-1] = (double*)resultptr;
		}

	else if (strcmp(kindOfPty, "double**") == 0)
		{
		manager.DoubleDoubleArraySize++;
		manager.DoubleDoubleArray = (int**)realloc(manager.DoubleDoubleArray,manager.DoubleDoubleArraySize*sizeof(int*));
		if (manager.DoubleDoubleArray == NULL)
			{
			DestroyAll(manager);
			return NULL;
			}
		manager.DoubleDoubleArray[manager.DoubleDoubleArraySize-1] = (double*)resultptr;
		}


	else if (strcmp(kindOfPty, "KDArray*") == 0)
		{
		manager.KdArray++;
		manager.KdArray = (KDArray*)realloc(manager.KdArray,manager.KdArraySize*sizeof(KDArray*));
		if (manager.KdArray == NULL)
		{
			DestroyAll(manager);
			return NULL;
				}
//		manager.KdArray[manager.KdArraySize-1] = (double*)resultptr;
		}

	else if (strcmp(kindOfPty, "SPPoint**") == 0)
		{
		manager.SPPointArrayArraySize++;
		manager.SPPointArrayArray = (SPPoint**)realloc(manager.SPPointArrayArray,manager.SPPointArrayArraySize*sizeof(SPPoint*));
		if (manager.SPPointArrayArray == NULL)
			{
			DestroyAll(manager);
			return NULL;
			}
		}


	else if (strcmp(kindOfPty, "SPPoint**") == 0)
	{
		manager.SPPointArraySize++;
//		manager.SPPointArray = (SPPoint*)realloc(manager.SPPointArray,manager.SPPointArraySize*sizeof(SPPoint));
		if (manager.SPPointArray == NULL) {
			DestroyAll(manager);
			return NULL;


		}

	}
	return resultptr;
}

void DestroyAll (AllocateManager manager) {

	//destroy int arrays
	for (int i = 0; i < manager.IntArraySize; i++)
		free(manager.IntArray[i]);
	free(manager.IntArray);

	//destroy char arrays
	for (int i = 0; i < manager.CharArraySize; i++)
		free(manager.CharArray[i]);
	free(manager.CharArray);

	//destroy double arrays
	for (int i = 0; i < manager.DoubleArraySize; i++)
		free(manager.DoubleArray[i]);
	free(manager.DoubleArray);

	//destroy arrays of double arrays - to be checked
	for (int i = 0; i < manager.DoubleDoubleArraySize; i++)
		free(manager.DoubleDoubleArray[i]);
	free(manager.DoubleDoubleArray);

	//destroy arrays of double arrays - to be checked
	for (int i = 0; i < manager.KdArraySize; i++)
//		free(manager.KdArray[i]);
	free(manager.KdArray);

	//destroy arrays of double arrays - to be checked
	for (int i = 0; i < manager.SPPointArrayArraySize; i++)
		free(manager.SPPointArrayArray[i]);
	free(manager.SPPointArrayArray);



}

void DestroyManager(AllocateManager manager) {

		free(manager.IntArray);
		free(manager.CharArray);
		free(manager.DoubleArray);
		free(manager.DoubleDoubleArray);
		free(manager.KdArray);
		free(manager.SPPointArrayArray);
		free(manager.SPPointArray);
}


