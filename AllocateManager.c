/*
 * AllocateManager.c
 *
 *  Created on: Mar 9, 2017
 *      Author: uri
 */


#include "AllocateManager.h"


void* MyMalloc (size_t size, AllocateManager* manager) {

	void* resultptr = malloc(size);

	//in case of memory leak - free all and return null
	if (resultptr == NULL) {
		DestroyAll (manager);
		return NULL;
	}

	manager->numOfElements++;

	if (manager->numOfElements >= manager->arraySize/2) {
		manager->arraySize = (manager->arraySize + 1)*2;
		manager->voidArray = (void**)realloc(manager->voidArray,manager->arraySize*sizeof(void*));
	}

	if (manager->voidArray == NULL) {
		DestroyAll(manager);
		return NULL;
	}

	manager->voidArray[manager->numOfElements-1] = (void*)resultptr;
	return resultptr;
}

void DestroyAll (AllocateManager * manager) {

	//destroy int arrays
	for (int i = 0; i < manager->numOfElements; i++)
		free(manager->voidArray[i]);
	free(manager->voidArray);
}

void DestroyManager(AllocateManager* manager) {
	free(manager->voidArray);
	free(manager);
}


