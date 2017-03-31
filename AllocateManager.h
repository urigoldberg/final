

#ifndef ALLOCATEMANAGER_H_
#define ALLOCATEMANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPPoint.h"

/*
 * Documentation about this struct *
 */

typedef struct allocate_manager {

	int isInit;
	int arraySize;
	int numOfElements;
	void **voidArray;
	void **founcArray;


} AllocateManager;

/*
 * instad of malloc
 */

void* MyMalloc (size_t size, AllocateManager* manager, void (*comp)(void*));

int AddParamWithoutAllocation (void *elem, void (*comp)(void*), AllocateManager* manager);

/*
 * destroiers
 */
void DestroyAll (AllocateManager* manager);

void DestroyManager(AllocateManager* manager);

void DestroyFree (void * ptr);

void DestroySPPoint (void * point);

void DestroySppointArray(SPPoint** pointArr, int howmanypoints);

#endif /* ALLOCATEMANAGER_H_ */
