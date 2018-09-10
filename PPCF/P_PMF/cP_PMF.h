/********************************************************
 * cP_PMF.h
 * Author: Jamie Zhu <jimzhu@GitHub>
 * Created: 2014/5/6
 * Last updated: 2015/02/11
********************************************************/

#include <algorithm>
#include <iostream>
using namespace std;


/* Perform the core approach of P_PMF */
void P_PMF(double *removedData, double *predData, int numUser, 
	int numService, int dim, double lmda, int maxIter, double etaInit, 
    double *Udata, double *Sdata, double *bs, bool debugMode);

/* Compute the loss value */
double loss(double **U, double **S, double *bs, double **removedMatrix,	double **predMatrix, 
    double lmda, int numUser, int numService, int dim);

/* Compute the gradients of the loss function */
void gradLoss(double **U, double **S, double *bs, double **removedMatrix, double **predMatrix, 
    double **gradU, double **gradS, double *gradbs, double lmda, int numUser, int numService, int dim);

/* Perform line search to find the best learning rate */
double linesearch(double **U, double **S, double *bs, double **removedMatrix, double lastLossValue, 
    double **gradU, double **gradS, double *gradbs, double etaInit,	double lmda, int numUser, 
	int numService, int dim);

/* Update predMatrix */
void updatePredMatrix(bool flag, double **removedMatrix, double **predMatrix, double **U, 
	double **S, double *bs, int numUser, int numService, int dim);

/* Transform a vector into a matrix */ 
double **vector2Matrix(double *vector, int row, int col);

/* Compute the dot product of two vectors */
double dotProduct(double *vec1, double *vec2, int len);

/* Allocate memory for a 2D array */
double **createMatrix(int row, int col);

/* Free memory for a 2D array */ 
void delete2DMatrix(double **ptr); 

/* Get system time info */
const string currentDateTime();


