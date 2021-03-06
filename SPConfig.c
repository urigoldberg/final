#include "SPConfig.h"

void printError(int line, const char *file, char *msg) {
    printf("File: %s\nLine: %d\nMessage: %s\n", file, line, msg);
}

void printInvalidLineError(int line, const char *file) {
    printError(line, file, "Invalid configuration line");
}

void printConstraintError(int line, const char *file) {
    printError(line, file, "Invalid value - constraint not met");
}

void printDefaultValueError(int line, const char *file, SP_CONFIG_MSG msg) {
    char formattedString[1024];
    char *paramName;
    if (msg == SP_CONFIG_MISSING_DIR) {
        paramName = "spImagesDirectory";
    } else if (msg == SP_CONFIG_MISSING_PREFIX) {
        paramName = "spImagesPrefix";
    } else if (msg == SP_CONFIG_MISSING_SUFFIX) {
        paramName = "spImagesSuffix";
    } else if (msg == SP_CONFIG_MISSING_NUM_IMAGES) {
        paramName = "spNumOfImages";
    } else {
        return;
    }
    sprintf(formattedString, R_PARAMETER_NOT_SET_ERROR, paramName);
    printError(line, file, formattedString);
}

SP_CONFIG_MSG insertToConfig(char key[MAXBUF], char value[MAXBUF],
                             SPConfig configObj, int lineIndex, const char *filename) {
    if (strcmp(key, "spImagesDirectory") == 0) {
        strcpy(configObj->spImagesDirectory, value);
        return SP_CONFIG_SUCCESS;
    }
    if (strcmp(key, "spImagesPrefix") == 0) {
        strcpy(configObj->spImagesPrefix, value);
        return SP_CONFIG_SUCCESS;
    }
    if (strcmp(key, "spImagesSuffix") == 0) {
        if (strcmp(value, ".jpg") == 0 || strcmp(value, ".png") == 0
            || strcmp(value, ".bmp") == 0 || strcmp(value, ".gif") == 0) {
            strcpy(configObj->spImagesSuffix, value);
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_STRING;
        }

    }
    if (strcmp(key, "spNumOfImages") == 0) {
        int numOfImages;
        numOfImages = atoi(value);
        if (numOfImages > 0) {
            configObj->spNumOfImages = numOfImages;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_INTEGER;
        }
    }
    if (strcmp(key, "spPCADimension") == 0) {
        int spPCADimansion;
        spPCADimansion = atoi(value);
        if (spPCADimansion > 9 && spPCADimansion < 29) {
            configObj->spPCADimension = spPCADimansion;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_INTEGER;
        }
    }
    if (strcmp(key, "spPCAFilename") == 0) {
        strcpy(configObj->spPCAFilename, value);
        return SP_CONFIG_SUCCESS;
    }
    if (strcmp(key, "spNumOfFeatures") == 0) {
        int spNumOfFeatures;
        spNumOfFeatures = atoi(value);
        if (spNumOfFeatures > 0) {
            configObj->spNumOfFeatures = spNumOfFeatures;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_INTEGER;
        }
    }
    if (strcmp(key, "spExtractionMode") == 0) {
        if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
            configObj->spExtractionMode = strcmp(value, "true") == 0;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_STRING;
        }
    }
    if (strcmp(key, "spNumOfSimilarImages") == 0) {
        int spNumOfSimilarImages;
        spNumOfSimilarImages = atoi(value);
        if (spNumOfSimilarImages > 0) {
            configObj->spNumOfSimilarImages = spNumOfSimilarImages;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_INTEGER;
        }
    }
    if (strcmp(key, "spKDTreeSplitMethod") == 0) {
        if (strcmp(value, "RANDOM") == 0) {
            configObj->kdTreeSplitMethod = RANDOM;
            return SP_CONFIG_SUCCESS;
        }
        if (strcmp(value, "MAX_SPREAD") == 0) {
            configObj->kdTreeSplitMethod = MAX_SPREAD;
            return SP_CONFIG_SUCCESS;
        }
        if (strcmp(value, "INCREMENTAL") == 0) {
            configObj->kdTreeSplitMethod = INCREMENTAL;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_STRING;
        }
    }
    if (strcmp(key, "spKNN") == 0) {
        int spKNN;
        spKNN = atoi(value);
        if (spKNN > 0) {
            configObj->spKNN = spKNN;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_INTEGER;
        }
    }
    if (strcmp(key, "spMinimalGUI") == 0) {
        if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
            configObj->spMinimalGUI = strcmp(value, "true") == 0;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_STRING;
        }
    }
    if (strcmp(key, "spLoggerLevel") == 0) {
        int spLoggerLevel;
        spLoggerLevel = atoi(value);
        if ((spLoggerLevel == 1 || spLoggerLevel == 2 || spLoggerLevel == 3 || spLoggerLevel == 4)) {
            configObj->spLoggerLevel = spLoggerLevel;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_INTEGER;
        }
    }
    if (strcmp(key, "spLoggerFilename") == 0) {
        strcpy(configObj->spLoggerFilename, value);
        return SP_CONFIG_SUCCESS;
    }
    return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG handleLine(SPConfig config, char *line, int lineIndex,
                         const char *filename) {
    char lineNoSpaces[MAXBUF];
    char key[MAXBUF];
    char tmpLine[MAXBUF];
    char value[MAXBUF];
    char *tmpWord;
    int wordCounter;
    if (line[0] == '#') {
        return SP_CONFIG_SUCCESS;
    }
    tmpWord = strtok(line, "#");
    strcpy(tmpLine, tmpWord);
    tmpWord = strtok(tmpLine, " ");
    strcpy(lineNoSpaces, "");
    wordCounter = 0;
    while (tmpWord != NULL && strcmp(tmpWord, "\n") != 0) {
        wordCounter++;
        strcat(lineNoSpaces, tmpWord);
        tmpWord = strtok(NULL, " ");
        if (wordCounter > 3) {
            printInvalidLineError(lineIndex, filename);
            return SP_CONFIG_INVALID_STRING;
        }
    }

    tmpWord = strtok(lineNoSpaces, "=");
    if (!tmpWord) {
        printInvalidLineError(lineIndex, filename);
        return SP_CONFIG_INVALID_STRING;
    }
    strcpy(key, tmpWord);
    tmpWord = strtok(NULL, "=");
    if (!tmpWord) {
        printInvalidLineError(lineIndex, filename);
        return SP_CONFIG_INVALID_STRING;
    }
    strcpy(tmpLine, tmpWord);

    tmpWord = strtok(NULL, "=");
    if (tmpWord) {
        printInvalidLineError(lineIndex, filename);
        return SP_CONFIG_INVALID_STRING;
    }

    tmpWord = strtok(tmpLine, "\n");
    strcpy(value, tmpWord);
    return insertToConfig(key, value, config, lineIndex, filename);
}

SP_CONFIG_MSG getNotInitParam(SPConfig config) {
    if (strlen(config->spImagesDirectory) == 0) {
        return SP_CONFIG_MISSING_DIR;
    }
    if (strlen(config->spImagesPrefix) == 0) {
        return SP_CONFIG_MISSING_PREFIX;
    }
    if (strlen(config->spImagesSuffix) == 0) {
        return SP_CONFIG_MISSING_SUFFIX;
    }
    if (config->spNumOfImages == 0) {
        return SP_CONFIG_MISSING_NUM_IMAGES;
    }
    return SP_CONFIG_SUCCESS;
}

SPConfig spConfigCreate(const char *filename, SP_CONFIG_MSG *msg) {
    if (filename == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NULL;
    }
    SPConfig res = (SPConfig) malloc(sizeof(*res));
    if (res == NULL) {
        *msg = SP_CONFIG_ALLOC_FAIL;
        return NULL;
    }
    char line[MAXBUF];
    int lineIndex = 0;
    FILE *configFile;
    SP_CONFIG_MSG tmpMsg;
    res->spPCADimension = 20;
    strcpy(res->spPCAFilename, "pca.yml");
    res->spNumOfFeatures = 100;
    res->spExtractionMode = true;
    res->spMinimalGUI = false;
    res->spNumOfSimilarImages = 1;
    res->spKNN = 1;
    res->kdTreeSplitMethod = MAX_SPREAD;
    res->spLoggerLevel = 3;
    res->spImagesDirectory[0] = 0;
    res->spImagesPrefix[0] = 0;
    res->spImagesSuffix[0] = 0;
    strcpy(res->spLoggerFilename, "stdout");

    configFile = fopen(filename, "r");

    if (configFile == NULL) {
        *msg = SP_CONFIG_CANNOT_OPEN_FILE;
        free(res);
        return NULL;
    }
    while (fgets(line, sizeof(line), configFile)) {
        lineIndex++;
        tmpMsg = handleLine(res, line, lineIndex, filename);
        if (tmpMsg != SP_CONFIG_SUCCESS) {
            *msg = tmpMsg;
            free(res);
            return NULL;
        }
    }

    tmpMsg = getNotInitParam(res);
    if (tmpMsg == SP_CONFIG_SUCCESS) {
        *msg = SP_CONFIG_SUCCESS;
        return res;
    } else {
        printDefaultValueError(lineIndex, filename, tmpMsg);
        *msg = tmpMsg;
        free(res);
        return NULL;
    }

}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG *msg) {
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return false;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spExtractionMode;
    }

}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG *msg) {
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return false;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spMinimalGUI;
    }
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG *msg) {
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfImages;
    }
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG *msg) {
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfFeatures;
    }
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG *msg) {
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spPCADimension;
    }
}

SP_CONFIG_MSG spConfigGetImagePath(char *imagePath, const SPConfig config,
                                   int index) {
    if (imagePath == NULL || config == NULL) {
        return SP_CONFIG_INVALID_ARGUMENT;
    } else if (index >= config->spNumOfImages) {
        return SP_CONFIG_INDEX_OUT_OF_RANGE;
    } else {
        char res[MAXBUF];
        res[0] = '\0';
        char indexString[MAXBUF];
        indexString[0] = '\0';
        strcat(res, config->spImagesDirectory);
        strcat(res, config->spImagesPrefix);
        sprintf(indexString, "%d", index);
        strcat(res, indexString);
        strcat(res, config->spImagesSuffix);
        strcpy(imagePath, res);
        return SP_CONFIG_SUCCESS;
    }
}

SP_CONFIG_MSG spConfigGetPCAPath(char *pcaPath, const SPConfig config) {
    if (pcaPath == NULL || config == NULL) {
        return SP_CONFIG_INVALID_ARGUMENT;
    } else {
        char res[MAXBUF];
        strcat(res, config->spImagesDirectory);
        strcat(res, config->spPCAFilename);
        strcpy(pcaPath, res);
        return SP_CONFIG_SUCCESS;
    }
}

void spConfigDestroy(SPConfig config) {
    free(config);
}