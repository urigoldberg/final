
#include<string.h>
#include <stdlib.h>
#include "main_aux.h"

void removeNewline(char s1[]) {
	if (s1[strlen(s1) - 1] == '\n')
	{
		s1[strlen(s1) - 1] = '\0';
	}
}


void destroySPPointMatrix(SPPoint ***spPointMatrix, int *pointsExtractInPic, int untilIndex) {
	for (int i = 0; i < untilIndex; i++) {
		for (int j = 0; j < pointsExtractInPic[i]; j++) {
			if (spPointMatrix[i][j] != NULL) {
				spPointDestroy(spPointMatrix[i][j]);
			}
		}
		if (spPointMatrix[i] != NULL) {
			free(spPointMatrix[i]);
		}
	}

	free(spPointMatrix);
	free(pointsExtractInPic);
}


