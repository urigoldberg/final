#include <cstdlib>
#include <ctype.h>
#include <conio.h>
#include "main_aux.cpp"

//include c library
extern "C" {

//include your own C source files
#include "SPPoint.h"
#include "SPLogger.h"
#include "SPConfig.h"
#include "kdTree.h"
#include "AllocateManager.h"
}

//include C++ source files
#include "SPImageProc.h"


void removeNewline(char s1[]) {
    s1[strlen(s1) - 1] = '\0';
}

int cmpfunc(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

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

    // Check Args
    if (argc > 1) {
        if (argc != 3) {
            printf("Invalid command line : use -c <config_filename>\n");
            return -1;
        } else {
            strcpy(dir, argv[2]);
            usingDefaultConfigFile = false;
            if (strcmp(argv[1], "-c") != 0) {
            	// todo change <config_filename> to something else?
                printf("Invalid command line : use -c <config_filename>\n");
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
    sp::ImageProc imageProc(config);

    // todo check if needed to handle error


    // Allocate points matrix for points will be extracted from images
    howManyPoints = 0;
    spPointMatrix = (SPPoint ***) malloc(sizeof(SPPoint **) * config->spNumOfImages);
    pointsExtractInPic = (int *) malloc(config->spNumOfImages * sizeof(int));
    if (spPointMatrix == NULL || pointsExtractInPic == NULL) {
        //free all
        return -1;
    }

    // Extract points
    for (int i = 0; i < config->spNumOfImages; i++) {
    	char path[1024];

    	spConfigGetImagePath(path,config,i);
    	if (path == NULL) {
    		//path
    		return -1;
    	}
        spPointMatrix[i] = imageProc.getImageFeatures(path, i, &pointsExtractInPic[i]);
        if (spPointMatrix[i] == NULL) {
            //todo free all
            return -1;
        }
        howManyPoints += pointsExtractInPic[i];
    }


    // Put all points in one array for KDtree
    arrOfAllPoints = (SPPoint **) malloc(sizeof(SPPoint *) * howManyPoints);
    if (arrOfAllPoints == NULL) {
        //free all
        return -1;
    }

    pointIndex = 0;
    //for each row in matrix
    for (int i = 0; i < config->spNumOfImages; i++) {

        //copy each point
        for (int j = 0; j < pointsExtractInPic[i]; j++) {
            arrOfAllPoints[pointIndex] = spPointCopy(spPointMatrix[i][j]);
            if (arrOfAllPoints[pointIndex] == NULL) {
                //todo OMG free all...
                return -1;
            }
            int check  = spPointGetDimension(arrOfAllPoints[pointIndex]);
            pointIndex++;
            spPointDestroy(spPointMatrix[i][j]);
        }

        free(spPointMatrix[i]);
    }
    free(spPointMatrix);

    // Create KDArray + KDTree
    kdArray = Init(arrOfAllPoints, howManyPoints);
    if (kdArray == NULL) {
        // todo destroy all
        return -1;
    }
    kdTree = InitKdTreeFromKdArray(kdArray, config->spKDTreeSplitMethod, 0);
    if (kdTree == NULL) {
		// todo destroy all
		return -1;
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
        int queryNumOfFeatures;
        int i;
        char str[15];
        SPPoint **queryFeatures = imageProc.getImageFeatures(query, 0, &queryNumOfFeatures);
        if (queryFeatures == NULL) {
        	//todo shoaa
        	printf("tofet");
        	return -1;
        }

        // Count similar pictures
        for (i = 0; i < queryNumOfFeatures; i++) {

        	if (queryFeatures[i] == NULL) {
        		//todo fuck
        		return -1;
        	}

		SPBPQueue *queue = spBPQueueCreate(config->spKNN);
		kNearestNeighbors(kdTree, queue, queryFeatures[i]);
		while (!spBPQueueIsEmpty(queue)) {
			BPQueueElement element;
			spBPQueuePeek(queue, &element);
			spBPQueueDequeue(queue);
			imgAppearanceCounterArr[element.index]++;
		}
		spBPQueueDestroy(queue);
	}

        DestroySppointArray(queryFeatures, queryNumOfFeatures);

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
            	getch(); //wait for user
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
//todo free tibet
    printf(EXIT);
    return 0;

}

