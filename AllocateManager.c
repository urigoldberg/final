/*
 * AllocateManager.c
 *
 *  Created on: Mar 9, 2017
 *      Author: uri
 */


#include "AllocateManager.h"


void* MyMalloc (size_t size, AllocateManager* manager, void (*comp)(void*)) {

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
		manager->founcArray = (void**)realloc(manager->founcArray,manager->arraySize*sizeof(void*));
	}

	if (manager->voidArray == NULL || manager->founcArray == NULL) {
		DestroyAll(manager);
		return NULL;
	}

	manager->voidArray[manager->numOfElements-1] = (void*)resultptr;
	manager->founcArray[manager->numOfElements-1] = (void*)comp;
	return resultptr;
}



int AddParamWithoutAllocation (void *elem, void (*comp)(void*), AllocateManager* manager) {

	manager->numOfElements++;

	if (manager->numOfElements >= manager->arraySize/2) {
		manager->arraySize = (manager->arraySize + 1)*2;
		manager->voidArray = (void**)realloc(manager->voidArray,manager->arraySize*sizeof(void*));
		manager->founcArray = (void**)realloc(manager->founcArray,manager->arraySize*sizeof(void*));
	}

	if (manager->voidArray == NULL || manager->founcArray == NULL) {
		DestroyAll(manager);
		(comp)(elem);
		return 1;
	}

	manager->voidArray[manager->numOfElements-1] = elem;
	manager->founcArray[manager->numOfElements-1] = (void*)comp;
	return 0;
	}

/*
 * destroiers
 */

void DestroyAll (AllocateManager * manager) {

	for (int i = 0; i < manager->numOfElements; i++)
		((void (*)(void*))manager->founcArray[i])(manager->voidArray[i]);
	free(manager->voidArray);
}

void DestroySPPoint (void * point) {
	spPointDestroy((SPPoint*)point);
	return;
}


void DestroyManager(AllocateManager* manager) {
	free(manager->voidArray);
	free(manager->founcArray);
	free(manager);
}
