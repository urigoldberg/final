
#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#define R_ENTER_QUERY "Please enter an image path:\n"
#define R_EXIT_MSG "Exiting...\n"
#define EXIT_SIGN "<>"
#define R_CANT_OPEN_CONFIG "The configuration file %s couldn't be open\n"
#define R_BEST_CAND_ID "Best candidates for - %s - are:\n"
#define R_INVALID_ARGS "Invalid command line : use -c <config_filename>\n"
#define R_PARAMETER_NOT_SET_ERROR "Parameter %s is not set\n"
#define R_LOGGER_OUT_OF_MEMORY_ERROR "Logger out of memory\n"
#define R_LOGGER_CANNOT_OPEN_FILE_ERROR "Logger cannot open file\n"
#define R_ALLOCATION_FAILURE_ERROR "Allocation failure\n"

#define LOGGER_INIT_MSG "Logger initialized"
#define POINT_MATRIX_INIT_MSG "Point matrix initialized"
#define POINT_ARRAY_INIT_MSG "Point array initialized"
#define KD_ARRAY_INIT_MSG "KDArray initialized"
#define KD_TREE_INIT_MSG "KDTree initialized"
#define EXTRAT_FATURES_ERROR_MSG "Error when trying to extract image features"
#define INVALID_FEATURES_ERROR_MSG "Invalid feature"

#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include "SPLogger.h"
#include "SPConfig.h"
#include "KdTree.h"

/**
 * removes the '\n' char in the end of the line
 *
 * @param s1 - the string
 * @assert s1 != NULL
 *
 */
void removeNewline(char s1[]);


/**
 * destroys the SPPoint matrix and frees all of the resources
 *
 * @param spPointMatrix - the matrix to be freed
 * @assert spPointMatrix != NULL
 * @param pointsExtractInPic - array of points that extracted from each picture
 * @param untilIndex - until what index to destroy the matrix
 */
void destroySPPointMatrix(SPPoint ***spPointMatrix, int *pointsExtractInPic, int untilIndex);

#endif /* MAIN_AUX_H_ */

#ifdef __cplusplus
}
#endif