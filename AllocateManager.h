#include "kdTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ALLOCATEMANAGER_H_
#define ALLOCATEMANAGER_H_



/*
 * Documentation about this struct *
 */

typedef struct AllocateManager {

	int arraySize;
	int numOfElements;
	void **voidArray;

} AllocateManager;

/*
 * instad of malloc
 */

void* MyMalloc (size_t size, AllocateManager manager);

void DestroyAll (AllocateManager manager);

void DestroyManager(AllocateManager manager);

#endif /* ALLOCATEMANAGER_H_ */
