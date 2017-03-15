#include "SPconfig.h"

#define DELIM "="

void insertToConfig(char key[MAXBUF], char value[MAXBUF], SPConfig configObj) {
	if (strcmp(key, "spImagesDirectory") == 0) {
		strcpy(configObj->spImagesDirectory, value);
		return;
	}
	if (strcmp(key, "spImagesPrefix") == 0) {
		strcpy(configObj->spImagesPrefix, value);
		return;
	}
	if (strcmp(key, "spImagesSuffix") == 0) {
		strcpy(configObj->spImagesSuffix, value);
		return;
	}
	if (strcmp(key, "spNumOfImages") == 0) {
		configObj->spNumOfImages = value - '0';
		return;
	}
	if (strcmp(key, "spPCADimension") == 0) {
		configObj->spPCADimension = value - '0';
		return;
	}
	if (strcmp(key, "spPCAFilename") == 0) {
		strcpy(configObj->spPCAFilename, value);
		return;
	}
	if (strcmp(key, "spNumOfFeatures") == 0) {
		configObj->spNumOfFeatures = value - '0';
		return;
	}
	if (strcmp(key, "spExtractionMode") == 0) {
		configObj->spExtractionMode = strcmp(value, "true") == 0;
		return;
	}
	if (strcmp(key, "spKDTreeSplitMethod") == 0) {
		if (strcmp(value, "RANDOM") == 0) {
			configObj->spKDTreeSplitMethod = RANDOM;
			return;
		}
		if (strcmp(value, "MAX_SPREAD") == 0) {
			configObj->spKDTreeSplitMethod = MAX_SPREAD;
			return;
		}
		if (strcmp(value, "INCREMENTAL") == 0) {
			configObj->spKDTreeSplitMethod = INCREMENTAL;
			return;
		}
	}
	if (strcmp(key, "spKNN") == 0) {
		configObj->spKNN = value - '0';
		return;
	}
	if (strcmp(key, "spMinimalGUI") == 0) {
		configObj->spMinimalGUI = strcmp(value, "true") == 0;
		return;
	}
	if (strcmp(key, "spLoggerLevel") == 0) {
		configObj->spLoggerLevel = value - '0';
		return;
	}
	if (strcmp(key, "spLoggerFilename") == 0) {
		strcpy(configObj->spLoggerFilename, value);
		return;
	}
}

void printError(char* line, char* file, char* msg) {
	printf("File: %s\nLine: %s\nMessage: %s\n", line, file, msg);
}

void printInvalidLineError(char* line, char* file) {
	printError(line, file, "Invalid configuration line");
}

void printConstraintError(char* line, char* file) {
	printError(line, file, "Invalid value - constraint not met");
}

void printDefaultValueError(char* line, char* file, char* paramName) {
	char formattedString[1024];
	sprintf(formattedString, "Parameter %s is not set", paramName);
	printError(line, file, formattedString);
}


int handleLine(SPConfig config, char* line) {
	char lineNoSpaces[MAXBUF];
	char key[MAXBUF];
	char tmpLine[MAXBUF];
	char value[MAXBUF];
	char* tmpWord;
	int wordCounter;
	if (line[0] == '#') {
		return -1;
	}
	tmpWord = strtok(line, "#");
	strcpy(tmpLine, tmpWord);
	tmpWord = strtok(tmpLine, " ");
	strcpy(lineNoSpaces, "");
	wordCounter = 0;
	while (tmpWord != NULL) {
		wordCounter++;
		strcat(lineNoSpaces, tmpWord);
		tmpWord = strtok(NULL, " ");
		if (wordCounter > 3) {
			return (-1);
		}
	}

	wordCounter = 0;
	tmpWord = strtok(lineNoSpaces, "=");
	if (!tmpWord) {
		return (-1);
	}
	strcpy(key, tmpWord);
	tmpWord = strtok(NULL, "=");
	if (!tmpWord) {
		return (-1);
	}
	strcpy(tmpLine, tmpWord);

	tmpWord = strtok(NULL, "=");
	if (tmpWord) {
		return (-1);
	}

	tmpWord = strtok(tmpLine, "\n");
	strcpy(value, tmpWord);
	insertToConfig(key, value, config);
	return (0);
}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	SPConfig res = (SPConfig) malloc(sizeof(SPConfig));
	char line[MAXBUF];
	FILE* configFile;
	if (!filename) {
		configFile = fopen("spcbir.config", "r");
	} else {
		configFile = fopen(filename, "r");
	}

	if (configFile == NULL) {
		// TODO return error
		return NULL;
	}
	while (fgets(line, sizeof(line), configFile)) {
		handleLine(res, line);
	}
	return res;
}
/*
int main(int argc, char **argv) {
	spConfigCreate(NULL, NULL);
}*/

