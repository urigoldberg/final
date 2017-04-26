#include <cstdlib>
#include <ctype.h>
//#include <conio.h>
#include "main_aux.h"

//include c library
extern "C" {

//include your own C source files
#include "SPPoint.h"
#include "SPLogger.h"
#include "SPConfig.h"
#include "kdTree.h"
}

//include C++ source files
#include "SPImageProc.h"



int main(int argc, char **argv) {

    // variables
    char query[1024];
    char dir[MAXBUF];
    strcpy(dir, "./spcbir.config");
    bool usingDefaultConfigFile = true;
    SP_CONFIG_MSG msg;
    SPPoint **arrOfAllPoints = NULL;
    SPPoint ***spPointMatrix = NULL;
    int *pointsExtractInPic = NULL;
    int pointIndex;
    int howManyPoints;
    SPKDArray *kdArray;
    KDTreeNode *kdTree;
    SPBPQueue *queue;

    // Check Args
    if (argc > 1) {
        if (argc != 3) {
            printf(INVALID_ARGS);
            return -1;
        } else {
            strcpy(dir, argv[2]);
            usingDefaultConfigFile = false;
            if (strcmp(argv[1], "-c") != 0) {
                printf(INVALID_ARGS);
                return -1;
            }
        }
    }

    // Create Config & image proc
    SPConfig config = spConfigCreate(dir, &msg);
    if (!config) {
        if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {
            printf(CANNTOPENCONFIG, usingDefaultConfigFile ? "spcbir.config" : dir);
        }
        return -1;
    }

    //logger Init
    SP_LOGGER_MSG loggerMsg;
    loggerMsg =  spLoggerCreate(config->spLoggerFilename, (SP_LOGGER_LEVEL)config->spLoggerLevel);\
    if (loggerMsg != SP_LOGGER_SUCCESS) {
        spConfigDestroy(config);
        return -1;
    }

    sp::ImageProc imageProc(config);

    // todo check if needed to handle error


    // Allocate points matrix for points will be extracted from images
    howManyPoints = 0;
    spPointMatrix = (SPPoint ***) malloc(sizeof(SPPoint **) * config->spNumOfImages);

    // Allocation problem
    if (spPointMatrix == NULL) {
        spLoggerPrintError(ALLOCATION_FAILURE_ERROR,__FILE__, __func__, __LINE__);
        spLoggerDestroy();
        free(config);
        return -1;
    }

    pointsExtractInPic = (int *) malloc(config->spNumOfImages * sizeof(int));
    // Allocation problem
    if (pointsExtractInPic == NULL) {
        spLoggerPrintError(ALLOCATION_FAILURE_ERROR,__FILE__, __func__, __LINE__);
        free(config);
        spLoggerDestroy();
        free(spPointMatrix);
        return -1;
    }

    // Extract points
    for (int i = 0; i < config->spNumOfImages; i++) {
        char path[1024];

        spConfigGetImagePath(path,config,i);

        spPointMatrix[i] = imageProc.getImageFeatures(path, i, &pointsExtractInPic[i]);

        // Allocation problem
        if (spPointMatrix[i] == NULL) {
            DestroyspPointMatrix(spPointMatrix,pointsExtractInPic,i);
            free(config);
            return -1;
        }
        howManyPoints += pointsExtractInPic[i];
    }


    // Put all points in one array for KDtree
    arrOfAllPoints = (SPPoint **) calloc(howManyPoints,sizeof(SPPoint *));

    // Allocation problem
    if (arrOfAllPoints == NULL) {
        DestroyspPointMatrix(spPointMatrix,pointsExtractInPic,config->spNumOfImages);
        free(config);
        return -1;
    }

    pointIndex = 0;
    //for each row in matrix
    for (int i = 0; i < config->spNumOfImages; i++) {

        //copy each point
        for (int j = 0; j < pointsExtractInPic[i]; j++) {
            arrOfAllPoints[pointIndex] = spPointCopy(spPointMatrix[i][j]);

            // Allocation problem
            if (arrOfAllPoints[pointIndex] == NULL) {
                DestroyspPointMatrix(spPointMatrix,pointsExtractInPic,config->spNumOfImages);
                DestroySppointArray(arrOfAllPoints, howManyPoints);
                free(config);
                return -1;
            }

            pointIndex++;
            spPointDestroy(spPointMatrix[i][j]);
            spPointMatrix[i][j] = NULL; // in case of memory problem try to free all in DestroyspPointMatrix
        }
        free(spPointMatrix[i]);
        spPointMatrix[i] = NULL; // in case of memory problem try to free all in DestroyspPointMatrix
    }
    free(spPointMatrix);
    free(pointsExtractInPic);

    // Create KDArray + KDTree
    kdArray = Init(arrOfAllPoints, howManyPoints);
    if (kdArray == NULL) {
        DestroySppointArray(arrOfAllPoints, howManyPoints);
        free(config);
        return -1;
    }



    kdTree = InitKdTreeFromKdArray(kdArray, config->spKDTreeSplitMethod, 0);
    if (kdTree == NULL) {
        DestroyKDArray(kdArray,howManyPoints);
        free(config);
        return -1;
    }

    DestroyKDArray(kdArray,howManyPoints);

    // Create SPBPQueue for query
    queue = spBPQueueCreate(config->spKNN);

    if (queue == NULL) {
        DestroyKDArray(kdArray,howManyPoints);
        DestroyKdTree(kdTree);
        free(config);
    }


    // Ask for query
    printf(EnterQuery);
    fgets(query, 1024, stdin);
    removeNewline(query);

    //###################will be deleted#####################
    query[0] = '\0';
    strcpy(query,"./images/img7.png");
    //###################will be deleted#####################

    // Execute query
    while (strcmp(query, EXITSIGN) != 0) {

        int *imgAppearanceCounterArr = (int *) calloc((size_t) config->spNumOfImages, sizeof(int));

        if (imgAppearanceCounterArr == NULL) {
            DestroyKDArray(kdArray,howManyPoints);
            DestroyKdTree(kdTree);
            free(config);
        }

        int queryNumOfFeatures;
        int i;

        SPPoint **queryFeatures = imageProc.getImageFeatures(query, 0, &queryNumOfFeatures);
        if (queryFeatures == NULL) {
            DestroyKDArray(kdArray,howManyPoints);
            DestroyKdTree(kdTree);
            free(imgAppearanceCounterArr);
            free(config);
            return -1;
        }

        // Count similar pictures
        for (i = 0; i < queryNumOfFeatures; i++) {

            if (queryFeatures[i] == NULL) {
                DestroyKDArray(kdArray,howManyPoints);
                DestroyKdTree(kdTree);
                DestroySppointArray(queryFeatures, queryNumOfFeatures);
                free(imgAppearanceCounterArr);
                free(config);
                return -1;
            }

            kNearestNeighbors(kdTree, queue, queryFeatures[i]);
            while (!spBPQueueIsEmpty(queue)) {
                BPQueueElement element;
                spBPQueuePeek(queue, &element);
                spBPQueueDequeue(queue);
                imgAppearanceCounterArr[element.index]++;
            }
        }

        DestroySppointArray(queryFeatures, queryNumOfFeatures);
        free(imgAppearanceCounterArr);

        // Find and print spNumOfSimilarImages
        for (i = 0; i < config->spNumOfSimilarImages; i++) {

            if (i == 0 && !config->spMinimalGUI)
            {
                printf(BESTCANDID,query);
            }

            // Find closest image
            int maxIndex = -1;
            int maxValue = -1;
            for (int j = 0; j < config->spNumOfImages; j++) {
                if (imgAppearanceCounterArr[j] > maxValue) {
                    maxValue = imgAppearanceCounterArr[j];
                    maxIndex = j;
                }
            }
            if (maxIndex > -1) {
                imgAppearanceCounterArr[maxIndex] = -1;
            }

            // Create path of closest image
            char path[1024];
            spConfigGetImagePath(path,config,maxIndex);

            // Show results (non) minimal gui
            if (config->spMinimalGUI)
            {
                imageProc.showImage(path);
//                getch(); //wait for user
            }

            else
            {
                printf("%s\n",path);
            }

        }
        printf(EnterQuery);
        fgets(query, 1024, stdin);
        removeNewline(query);

        //###################will be deleted#####################
        query[0] = '\0';
        strcpy(query,EXITSIGN);
        //###################will be deleted#####################
    }

    // Free All
    spBPQueueDestroy(queue);
    DestroyKdTree(kdTree);
    free(config);
    printf(EXIT);
    return 0;

}

