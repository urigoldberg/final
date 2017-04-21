#include <cstdlib>

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

int main(int argc, char **argv) {
    // Members
	sp::ImageProc* imageProc = NULL;
	char dir[MAXBUF];
	strcpy(dir, "../spcbir.config"); // todo change
	bool usingDefaultConfigFile = true;
	SP_CONFIG_MSG msg;
	SPPoint** arrOfAllPoints = NULL;
	SPPoint*** spPointMatrix = NULL;
	int* pointsExtractInPic = NULL;
	int pointIndex;
	int howManyPoints;
	SPKDArray* kdArray;
	KDTreeNode* kdTree;

	if (argc > 1) {
		if (argc != 3) {
				printf("Invalid command line : use -c <config_filename>\n");
				return -1;
			} else {
				strcpy(dir, argv[2]);
				usingDefaultConfigFile = false;
				if (strcmp(argv[1], "-c") != 0) {
					printf("Invalid command line : use -c <config_filename>\n");
					return -1;
				}
			}
	}

	SPConfig config = spConfigCreate(dir, &msg);
	if (!config) {
		if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {
			printf("The configuration file %s couldn't be open\n", usingDefaultConfigFile ? "spcbir.config" : dir);
		}
        return -1;
	}

	// config successfull
    imageProc = new sp::ImageProc(config);
    // todo check if needed to handle error

    //
    howManyPoints = 0;
    spPointMatrix = (SPPoint***)malloc(sizeof(SPPoint**)*config->spNumOfImages);
    pointsExtractInPic = (int*)calloc(config->spNumOfImages,sizeof(int));
    if (spPointMatrix == NULL || pointsExtractInPic == NULL) {
    	//free all
    	return -1;
    }

    for (int i = 0; i < config->spNumOfImages; i++)
    {
    	spPointMatrix[i] = getImageFeatures(config->spImagesDirectory,i,&pointsExtractInPic[i]);
    	if (spPointMatrix[i] == NULL)
    	{
    		//free all
    		return -1;
    	}
    	howManyPoints += pointsExtractInPic[i];
    }


    arrOfAllPoints = (SPPoint**)malloc(sizeof(SPPoint*)*howManyPoints);
    if (arrOfAllPoints == NULL) {
    	//free all
    	return -1;
    }

    pointIndex = 0;
    //for each row in matrix
    for (int i = 0; i < config->spNumOfImages; i++) {

    	//copy each point
    	for (int j = 0; j < pointsExtractInPic[i]; j++) {
    		arrOfAllPoints[i] = spPointCopy(spPointMatrix[i][j]);
    		if (arrOfAllPoints[i] == NULL) {
    			//OMG free all...
    			return -1;
    		}
    		spPointDestroy(spPointMatrix[i][j]);
    	}

    	free(spPointMatrix[i]);
    }

    free (spPointMatrix);

    kdArray = Init(arrOfAllPoints, howManyPoints);

    if (kdArray == NULL) {
    		DestroySppointArray(pointsArr,5);
    		return 1;
    	}
    //which one? incremental? random? max? pass the correct one
    kdTree = InitKdTreeFromKdArray(kdArray, INCREMENTAL, 1);

    //query, search , finish project








}
