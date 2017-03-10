

#ifndef ALLOCATEMANAGER_H_
#define ALLOCATEMANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Documentation about this struct *
 */

typedef struct allocate_manager {

	int arraySize;
	int numOfElements;
	void **voidArray;

} AllocateManager;

/*
 * instad of malloc
 */

void* MyMalloc (size_t size, AllocateManager* manager);

void DestroyAll (AllocateManager* manager);

void DestroyManager(AllocateManager * manager);

#endif /* ALLOCATEMANAGER_H_ */
