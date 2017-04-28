#include "KdTree.h"
#include "SPPoint.h"
#include <time.h>

#define POINTSNUMBER 3

/***************
 *             *
 *    PART I   *
 *    UTILS    *
 *             *
 ***************
 */

void DestroyKdTreeNode(KDTreeNode *treeNode) {
    if (treeNode == NULL)
        return;
    spPointDestroy(treeNode->Data);
    free(treeNode->Left);
    free(treeNode->Right);
    free(treeNode);
}

void DestroyKdTree(KDTreeNode *tree) {
    if (tree->Left != NULL) {
        DestroyKdTree(tree->Left);
        tree->Left = NULL;
    }

    if (tree->Right != NULL) {
        DestroyKdTree(tree->Right);
        tree->Right = NULL;
    }
    DestroyKdTreeNode(tree);

}

void DestroyKDArray(SPKDArray *arr, int numOfPoints) {
    if (arr == NULL || numOfPoints == 0) {
        return;
    }

    int dim = 0;

    if (arr->pointsArr == NULL) {
        free(arr);
        return;
    }

    if (arr->pointsArr[0] == NULL) {
        free(arr->pointsArr);
        free(arr);
        return;
    }
    dim = spPointGetDimension(arr->pointsArr[0]);
    DestroySppointArray(arr->pointsArr, numOfPoints);

    if (arr->sortedMatrix != NULL) {
        for (int i = 0; i < dim; i++) {
            free(arr->sortedMatrix[i]); //TODO check this kind of free. regular dosent work
        }

        free(arr->sortedMatrix);
    }

    free(arr);
}

void DestroySppointArray(SPPoint **pointArr, int howmanypoints) {

    if (pointArr == NULL) {
        return;
    }

    for (int i = 0; i < howmanypoints; i++) {
        spPointDestroy(pointArr[i]);
    }

    free(pointArr);
}


int GetRandomIndex(int dim) {
    srand(time(NULL));
    return rand() % dim;
}

int FindMaxSpread(int columns, int rows, SPKDArray *kdArray) {
    if (kdArray == NULL) {
        return -1;
    }

    double max = -1;
    int indexOfMax = -1;

    //search max spread
    for (int i = 0; i < rows; i++) {
        int indexOfMinPoint = (int) kdArray->sortedMatrix[i][0];
        int indexOfMaxPoint = (int) kdArray->sortedMatrix[i][columns - 1];

        int minIndex = spPointGetAxisCoor(kdArray->pointsArr[indexOfMinPoint], i);
        int maxIndex = spPointGetAxisCoor(kdArray->pointsArr[indexOfMaxPoint], i);

        //if there are several candidates choose the lowest dimension
        if ((maxIndex - minIndex) > max) {
            max = (maxIndex - minIndex);
            indexOfMax = i;
        }
    }

    return indexOfMax;

}

/***************
 *             *
 *    PART II  *
 *  KD - ARRAY *
 *             *
 ***************
 */

int SortByPointsIndex(const void *a, const void *b) {
    tempRowForSort First = (*(tempRowForSort *) a);
    tempRowForSort Second = (*(tempRowForSort *) b);
    return (First.Coor - Second.Coor);
}


SPKDArray *Init(SPPoint **arr, int size) {

    //Asserts
    if ((arr == NULL) || (size == 0)) {
        return NULL;
    }


    SPKDArray *res = (SPKDArray *) calloc(1, sizeof(SPKDArray));
    if (res == NULL) {
        return NULL;
    }

    res->dim = size;

    //Copy the array that received in the init function
    res->pointsArr = (SPPoint **) calloc(size, sizeof(SPPoint *));
    if (res->pointsArr == NULL) {
        DestroyKDArray(res, size);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        res->pointsArr[i] = spPointCopy(arr[i]);
        if (res->pointsArr[i] == NULL) {
            DestroyKDArray(res, size);
            return NULL;
        }
    }

    // Create a dXn
    int pointDim = spPointGetDimension(arr[0]);
    res->sortedMatrix = (int **) calloc(pointDim, sizeof(int *));
    if (res->sortedMatrix == NULL) {
        DestroyKDArray(res, size);
        return NULL;
    }

    tempRowForSort *tempRow = (tempRowForSort *) calloc(size, sizeof(tempRowForSort));
    if (tempRow == NULL) {
        DestroyKDArray(res, size);
        return NULL;
    }

    for (int i = 0; i < pointDim; i++) {

        res->sortedMatrix[i] = (int *) calloc(size, sizeof(int));
        if (res->sortedMatrix[i] == NULL) {
            DestroyKDArray(res, size);
            free(tempRow);
            return NULL;
        }

        for (int j = 0; j < size; j++) {
            tempRow[j].Coor = spPointGetAxisCoor(arr[j], i);
            tempRow[j].indexOfOrig = j;
        }

        //sort tempRow by index
        qsort(tempRow, size, sizeof(tempRowForSort), SortByPointsIndex);

        //update array with indexes after sorting
        for (int k = 0; k < size; k++) {
            res->sortedMatrix[i][k] = tempRow[k].indexOfOrig;
        }

    }

    //free resources
    free(tempRow);
    return res;
}

int Split(SPKDArray *kdArr, SPKDArray **left, SPKDArray **right, int coor) {

    //Asserts
    if (kdArr == NULL) {
        return 1;
    }

    //get dimention
    int dim = kdArr->dim;
    if (dim <= 0) {
        return 1;
    }

    if (kdArr->pointsArr == NULL || kdArr->pointsArr[0] == NULL || kdArr->sortedMatrix == NULL) {
        return 1;
    }

    int pointDim = spPointGetDimension(kdArr->pointsArr[0]);

    //create left and right
    int leftSize = dim % 2 == 0 ? dim / 2 : dim / 2 + 1;
    int rightSize = dim - leftSize;

    *right = (SPKDArray *) calloc(1, sizeof(SPKDArray));
    *left = (SPKDArray *) calloc(1, sizeof(SPKDArray));

    //in case of memory leak
    if (*right == NULL || *left == NULL) {
        DestroyKDArray(*right, rightSize);
        DestroyKDArray(*left, leftSize);
        return 1;
    }

    (*left)->dim = leftSize;
    (*right)->dim = rightSize;


    (*left)->pointsArr = (SPPoint **) calloc(leftSize, sizeof(SPPoint *));
    (*right)->pointsArr = (SPPoint **) calloc(rightSize, sizeof(SPPoint *));

    //in case of memory leak
    if ((*right)->pointsArr == NULL || (*left)->pointsArr == NULL) {
        DestroyKDArray(*right, rightSize);
        DestroyKDArray(*left, leftSize);
        return 1;
    }


    //create map array - each index has
    //if (map[i] > 0) - map[i] = point i in kdArr->pointsArr is in left kdarr with index  map[i]-1
    //else - map[i] = point i in kdArr->pointsArr is in right kdarr with index  -(map[i]-1)
    int *map = (int *) calloc(dim, sizeof(int));
    if (map == NULL) {
        DestroyKDArray(*right, rightSize);
        DestroyKDArray(*left, leftSize);
        return 1;
    }


    //create pointArr of left side + fill map with left element
    for (int i = 0; i < leftSize; i++) {
        int index = ((kdArr->sortedMatrix[coor][i]));
        (*left)->pointsArr[i] = spPointCopy(kdArr->pointsArr[index]);

        map[index] = i + 1;

        //In case of memory leak
        if ((*left)->pointsArr[i] == NULL) {
            free(map);
            DestroyKDArray(*right, rightSize);
            DestroyKDArray(*left, leftSize);
            return 1;
        }
    }

    //create pointArr of right side + fill map with left element
    for (int i = 0; i < rightSize; i++) {
        int index = ((kdArr->sortedMatrix[coor][leftSize + i]));
        (*right)->pointsArr[i] = spPointCopy(kdArr->pointsArr[index]);

        map[index] = -1 * (i + 1);

        //In case of memory leak
        if ((*right)->pointsArr[i] == NULL) {
            free(map);
            DestroyKDArray(*right, rightSize);
            DestroyKDArray(*left, leftSize);
            return 1;
        }
    }

    //create matrix left
    (*left)->sortedMatrix = (int **) calloc(pointDim, sizeof(int *));
    if ((*left)->sortedMatrix == NULL) {
        free(map);
        DestroyKDArray(*right, rightSize);
        DestroyKDArray(*left, leftSize);
        return 1;
    }

    //create matrix right
    (*right)->sortedMatrix = (int **) calloc(pointDim, sizeof(int *));
    if ((*right)->sortedMatrix == NULL) {
        free(map);
        DestroyKDArray(*right, rightSize);
        DestroyKDArray(*left, leftSize);
        return 1;
    }

    //fill dXn matrix
    for (int i = 0; i < pointDim; i++) {

        //allocate the i-th row
        (*left)->sortedMatrix[i] = (int *) calloc(leftSize, sizeof(int));
        (*right)->sortedMatrix[i] = (int *) calloc(rightSize, sizeof(int));

        //in case of memory leak or i-th row doesnt exsists in the original kdArr
        if ((*left)->sortedMatrix[i] == NULL || (*right)->sortedMatrix[i] == NULL || kdArr->sortedMatrix[i] == NULL) {
            free(map);
            DestroyKDArray(*right, rightSize);
            DestroyKDArray(*left, leftSize);
            return 1;
        }

        int whereInLeft = 0;
        int whereInRight = 0;

        //fill the i-th row
        for (int j = 0; j < dim; j++) {
            int forMapCheck = kdArr->sortedMatrix[i][j];
            int afterMapCheck = map[forMapCheck];

            //point in left array
            if (afterMapCheck > 0) {
                afterMapCheck = afterMapCheck - 1;
                (*left)->sortedMatrix[i][whereInLeft] = afterMapCheck;
                whereInLeft++;
            }

                //point in right array
            else {
                afterMapCheck = (-1 * afterMapCheck) - 1;
                (*right)->sortedMatrix[i][whereInRight] = afterMapCheck;
                whereInRight++;
            }
        }
    }

    free(map);
    return 0;
}

/***************
 *             *
 *  PART III   *
 *  KD - Tree  *
 *             *
 ***************
 */

KDTreeNode *InitKdTreeNode(int Dim, int Val, KDTreeNode *Left, KDTreeNode *Right, SPPoint *Data) {

    KDTreeNode *res = (KDTreeNode *) calloc(1, sizeof(KDTreeNode));
    if (res == NULL) {
        return NULL;
    }

    if (Data == NULL) {
        res->Data = NULL;
    } else {
        res->Data = spPointCopy(Data);

        if (res->Data == NULL) {
            free(res);
        }
    }


    res->Dim = Dim;
    res->Left = Left;
    res->Right = Right;
    res->Val = Val;

    return res;
}

KDTreeNode *InitKdTreeFromKdArray(SPKDArray *kdArray, spKDTreeSplitMethod SpCriteria, int forINCREMENTAL) {

    //Asserts
    if (kdArray == NULL || kdArray->pointsArr == NULL || kdArray->sortedMatrix == NULL) {
        return NULL;
    }

    //stop condition
    if (kdArray->dim == 1) {
        //TODO check if valid
        return InitKdTreeNode(-1, -1, NULL, NULL, kdArray->pointsArr[0]);
    }

    //find split index
    int dimOfPoints = spPointGetDimension(kdArray->pointsArr[0]);
    int howManyPoints = kdArray->dim;

    int i_thRow;
    if (SpCriteria == MAX_SPREAD) {
        i_thRow = FindMaxSpread(howManyPoints, dimOfPoints, kdArray);
    } else if (SpCriteria == RANDOM) {
        i_thRow = GetRandomIndex(dimOfPoints);
    } else {
        i_thRow = (forINCREMENTAL + 1) % dimOfPoints;
    }

    //create tree

    //split KDArray
    SPKDArray *leftArr = NULL;
    SPKDArray *rightArr = NULL;

    //split did not succeeded
    if (Split(kdArray, &leftArr, &rightArr, i_thRow)) {
        return NULL;
    }


    //create left tree
    KDTreeNode *leftTree = InitKdTreeFromKdArray(leftArr, SpCriteria, i_thRow);
    if (leftTree == NULL) {
        DestroyKDArray(rightArr, rightArr->dim);
        DestroyKDArray(leftArr, leftArr->dim);
        return NULL;
    }

    KDTreeNode *rightTree = InitKdTreeFromKdArray(rightArr, SpCriteria, i_thRow);
    if (rightTree == NULL) {
        DestroyKDArray(rightArr, rightArr->dim);
        DestroyKDArray(leftArr, leftArr->dim);
        DestroyKdTree(leftTree);
        return NULL;
    }

    int ithMedian = kdArray->sortedMatrix[i_thRow][leftArr->dim];

    KDTreeNode *res = InitKdTreeNode(i_thRow, ithMedian, leftTree, rightTree, NULL);

    if (res == NULL) {
        DestroyKDArray(rightArr, rightArr->dim);
        DestroyKDArray(leftArr, leftArr->dim);
        DestroyKdTree(leftTree);
        DestroyKdTree(rightTree);
        return NULL;
    }


    DestroyKDArray(rightArr, rightArr->dim);
	DestroyKDArray(leftArr, leftArr->dim);
    return res;

}

//TODO bpq will be initilaize outside
void kNearestNeighbors(KDTreeNode *curr, SPBPQueue *bpq, SPPoint *point) {

    bool searchedLeftTree = false;

    if (curr == NULL) {
        return;
    }

    /* Add the current point to the BPQ.  Note that this is a no-op if the
       point is not as good as the points we've seen so far.*/
    if (curr->Data != NULL) {
        spBPQueueEnqueue(bpq, spPointGetIndex(curr->Data), spPointL2SquaredDistance(curr->Data, point));
        return;
    }

    /* Recursively search the half of the tree that contains the test point. */
    double pointAtICoor = spPointGetAxisCoor(point, curr->Dim);

    if (pointAtICoor <= curr->Val) {
        searchedLeftTree = true;
        kNearestNeighbors(curr->Left, bpq, point);
    } else {
        kNearestNeighbors(curr->Right, bpq, point);
    }

    if (!spBPQueueIsFull(bpq) || (curr->Val - pointAtICoor) * (curr->Val - pointAtICoor) < spBPQueueMaxValue(bpq)) {

        //
        if (searchedLeftTree) {
            kNearestNeighbors(curr->Right, bpq, point);
        } else {
            kNearestNeighbors(curr->Left, bpq, point);
        }

    }

}


