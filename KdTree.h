#ifndef KDTREE_H_
#define KDTREE_H_
#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPBPriorityQueue.h"
#define POINTSNUMBER 3

/*
 * STRUCTS:
 */
typedef struct SPKDArray {
	SPPoint **pointsArr;
	int **sortedMatrix;
	int dim;

} SPKDArray;


typedef struct KDTreeNode{
	int Dim;
	double Val;
	struct KDTreeNode *Left;
	struct KDTreeNode *Right;
	SPPoint *Data;
} KDTreeNode;

typedef struct tempRowForSort {
	double Coor;
	int indexOfOrig;
} tempRowForSort;


/*
 * enums
 */

typedef enum spKDTreeSplitMethod  {RANDOM, MAX_SPREAD, INCREMENTAL } spKDTreeSplitMethod;

/*
 * Founctions
 */


/**
 *
 * SPKDArray* Init(SPPoint** arr, int size)
 *
 * Creates a KDArray object which contains an array of SPPoints and a
 * matrix that presents their order by the indexes.
 *
 * @param SPPoint** arr - An array of SPPoints
 * @param int size - Number of points in the array mentioned above.
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct of KDArray
 *
 */
SPKDArray* Init(SPPoint** arr, int size);


/**
 *
 * int SortByPointsIndex(const void* a,const void* b)
 *
 * tempRowForSort is a struct that helps to sort the matrix of the KDArray. It contains the the value of a
 * point in a specific and the index itself.
 * This function get two items of tempRowForSort and checks which one has bigger value.
 *
 * @param const void* a - tempRowForSort #1
 * @param const void* a - tempRowForSort #2
 *
 * @return the diffrence between a.value and b.value
 *
 */

int SortByPointsIndex(const void* a,const void* b);

/**
 *
 * void DestroyKdTree (KDTreeNode* tree)
 *
 * Free all data that has been alocated for KDtree.
 *
 * @param KDTreeNode* tree - tree to be freed. *
 */

void DestroyKdTree (KDTreeNode* tree);

/**
 *
 * kNearestNeighbors(KDTreeNode *curr, SPBPQueue *bpq, SPPoint *point)
 *
 * Gets a point, KDTree and a SPBPQueue in length k, and inserts to the queue the points
 * stored in the KDTree, where the index  stored in the KDTree, whose l2distance with the given point is the lowest
 *
 * @param KDTreeNode *curr - KDTree that stored the points to be searched
 * @param SPBPQueue *bpq - Queue that has been initialized. The function will insert to the queue the points in the KDTree.
 * @param SPPoint *point - The Query point.
 *
 */

void kNearestNeighbors(KDTreeNode *curr, SPBPQueue *bpq, SPPoint *point);

/**
 *
 * KDTreeNode *InitKdTreeFromKdArray(SPKDArray *kdArray, spKDTreeSplitMethod SpCriteria, int forINCREMENTAL, int PcadDimension)
 *
 * Initialize a KDTree based on a given KDArray.
 *
 * @param SPKDArray *kdArray - a given KDArray
 * @param spKDTreeSplitMethod SpCriteria - while creates the tree, the function splits the kdtree by a specific index. The index is chosen
 * 		by the value of the SpCriteria.
 * @param int forINCREMENTAL - if SpCriteria == INCREMENTAL, this parameter will cary the vaule of the index used in the last recursive call.
 * @param int PcadDimension - the dim of each point in tree.
 *
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct of KDTreeNode
 *
 */

KDTreeNode *InitKdTreeFromKdArray(SPKDArray *kdArray, spKDTreeSplitMethod SpCriteria, int forINCREMENTAL, int PcadDimension);


/**
 * int Split(SPKDArray *kdArr, SPKDArray **left, SPKDArray **right, int coor)
 *
 * Returns two kd-arrays (kdLeft, kdRight) such that the first
 *  ⌈𝒏/𝟐⌉  points with respect to the coordinate coor are in kdLeft 
 *  ,and the rest of the points are in kdRight.
 *
 * @param SPKDArray *kdArray - KDArray to be splited
 * @param SPKDArray **left - a pointer that will be update to the left kdarray.
 * @param SPKDArray **right - a pointer that will be ypdate to the right kdarray.
 * @param int coor - as mentioned above
 *
 * @return -1 if fail, Otherwise 0
 *
 */

int Split(SPKDArray *kdArr, SPKDArray **left, SPKDArray **right, int coor);

/**
 * void DestroyKDArray (SPKDArray *arr, int numOfPoints)
 *
 * Free all data that has been allocated for SPKDArray.
 *
 * @param KDTreeNode* arr - arr of SSPoints to be freed.
 * @param int numOfPoints - number of points in arr.
 * 
 */


void DestroyKDArray(SPKDArray *arr, int numOfPoints);

/**
 * DestroySppointArray(SPPoint **pointArr, int howmanypoints)
 *
 * Free all data that has been allocated for pointArr.
 *
 * @param SPPoint **pointArr - arr of SSPoints to be freed.
 * @param int howmanypoints - number of points in arr.
 * 
 */

void DestroySppointArray(SPPoint **pointArr, int howmanypoints);

#endif /* KDTREE_H_ */