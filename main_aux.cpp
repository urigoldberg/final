
#include<string.h>
#include <stdlib.h>
#include "main_aux.h"

void removeNewline(char s1[]) {
	if (s1[strlen(s1) - 1] == '\n')
	{
		s1[strlen(s1) - 1] = '\0';
	}
}

int cmpfunc(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

// Free all points in matrix
// Free the rows of the matrix & matrix itself
// Free
//void DestroyspPointMatrix(SPPoint ***spPointMatrix,int* pointsExtractInPic,int whereTofree) {
//	for (int i = 0; i < whereTofree; i++) {
//		for (int j = 0; j < pointsExtractInPic[i]; j++) {
//			if (spPointMatrix[i][j] != NULL) {
//				spPointDestroy(spPointMatrix[i][j]);
//			}
//		}
//		if (spPointMatrix[i] != NULL) {
//			free(spPointMatrix[i]);
//		}
//	}
//
//	free(spPointMatrix);
//	free (pointsExtractInPic);
//}

