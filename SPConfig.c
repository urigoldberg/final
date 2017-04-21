#include "SPconfig.h"


int parseIntFromString(char *str, int *ptr) {
    char *endptr;
    errno = 0;
    long result = strtol(str, &endptr, 10);

    if (endptr == str) {
        return -1;
    }
    if ((result == LONG_MAX || result == LONG_MIN) && errno == ERANGE) {
        return -1;
    }
    *ptr = result;
    return 0;
}

void printError(int line, const char *file, char *msg) {
    printf("File: %d\nLine: %s\nMessage: %s\n", line, file, msg);
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
    }
    sprintf(formattedString, "Parameter %s is not set", paramName);
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
        if (parseIntFromString(value, &numOfImages) != -1 && numOfImages > 0) {
            configObj->spNumOfImages = numOfImages;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_INTEGER;
        }
    }
    if (strcmp(key, "spPCADimension") == 0) {
        int spPCADimansion;
        if (parseIntFromString(value, &spPCADimansion) != -1
            && spPCADimansion > 9 && spPCADimansion < 29) {
            configObj->spPCADimension = strtol(value, NULL, 10);
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
        if (parseIntFromString(value, &spNumOfFeatures) != -1
            && spNumOfFeatures > 0) {
            configObj->spNumOfFeatures = strtol(value, NULL, 10);
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
        if (parseIntFromString(value, &spNumOfSimilarImages) != -1
            && spNumOfSimilarImages > 0) {
            configObj->spNumOfSimilarImages = strtol(value, NULL, 10);
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_INTEGER;
        }
    }
    if (strcmp(key, "spKDTreeSplitMethod") == 0) {
        if (strcmp(value, "RANDOM") == 0) {
            configObj->spKDTreeSplitMethod = RANDOM;
            return SP_CONFIG_SUCCESS;
        }
        if (strcmp(value, "MAX_SPREAD") == 0) {
            configObj->spKDTreeSplitMethod = MAX_SPREAD;
            return SP_CONFIG_SUCCESS;
        }
        if (strcmp(value, "INCREMENTAL") == 0) {
            configObj->spKDTreeSplitMethod = INCREMENTAL;
            return SP_CONFIG_SUCCESS;
        } else {
            printConstraintError(lineIndex, filename);
            return SP_CONFIG_INVALID_STRING;
        }
    }
    if (strcmp(key, "spKNN") == 0) {
        int spKNN;
        if (parseIntFromString(value, &spKNN) != -1 && spKNN > 0) {
            configObj->spKNN = strtol(value, NULL, 10);
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
        if (parseIntFromString(value, &spLoggerLevel) != -1
            && (spLoggerLevel == 1 || spLoggerLevel == 2
                || spLoggerLevel == 3 || spLoggerLevel == 4)) {
            configObj->spKNN = strtol(value, NULL, 10);
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
    while (tmpWord != NULL) {
        wordCounter++;
        strcat(lineNoSpaces, tmpWord);
        tmpWord = strtok(NULL, " ");
        if (wordCounter > 3) {
            printInvalidLineError(lineIndex, filename);
            return SP_CONFIG_INVALID_STRING;
        }
    }

    wordCounter = 0;
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
    SPConfig res = (SPConfig) malloc(sizeof(SPConfig));
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
    res->spKDTreeSplitMethod = MAX_SPREAD;
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
        char indexString[MAXBUF];
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







