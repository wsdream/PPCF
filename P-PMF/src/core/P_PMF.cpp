/********************************************************
 * P_PMF.cpp
 * C++ implements on P_PMF
 * Author: Jamie Zhu <jimzhu@GitHub>
 * Created: 2014/5/6
 * Last updated: 2015/02/11
********************************************************/

#include <iostream>
#include <cstring>
#include <cmath>
#include "P_PMF.h"
using namespace std;

#define eps 1e-10


/********************************************************
 * predData are the output values
 *******************************************************/
void P_PMF(double *removedData, double *predData, int numUser, 
	int numService, int dim, double lmda, int maxIter, double etaInit, 
    double *Udata, double *Sdata, double *bs, bool debugMode)
{	
	// --- transfer the 1D pointer to 2D array pointer
    double **removedMatrix = vector2Matrix(removedData, numUser, numService);
    double **predMatrix = vector2Matrix(predData, numUser, numService);
    double **U = vector2Matrix(Udata, numUser, dim);
    double **S = vector2Matrix(Sdata, numService, dim);
    
    // --- create a set of temporal matries
    double **gradU = createMatrix(numUser, dim);
    double **gradS = createMatrix(numService, dim);
    double *gradbs = new double[numService];

    // --- iterate by standard gradient descent algorithm
    int iter, i, j, k;
    double lossValue;
    for (iter = 1; iter <= maxIter; iter++) {
        // update predMatrix and lossValue      
        updatePredMatrix(false, removedMatrix, predMatrix, U, S, bs, numUser, numService, dim);
        lossValue = loss(U, S, bs, removedMatrix, predMatrix, lmda, numUser, numService, dim);

        // log the debug info
        cout.setf(ios::fixed);
        if (debugMode) {
            cout << currentDateTime() << ": ";
            cout << "iter = " << iter << ", lossValue = " << lossValue << endl;
        } 

        // update gradients
    	gradLoss(U, S, bs, removedMatrix, predMatrix, gradU, gradS, gradbs, 
    		lmda, numUser, numService, dim);

    	// line search to find the best learning rate eta
		double eta = linesearch(U, S, bs, removedMatrix, lossValue, gradU, gradS, 
			gradbs, etaInit, lmda, numUser, numService, dim);

		// gradient descent updates
		for (k = 0; k < dim; k++) {
			// update U
			for (i = 0; i < numUser; i++) {
				U[i][k] -= eta * gradU[i][k];
			}
			// update S
			for (j = 0; j < numService; j++) {
				S[j][k] -= eta * gradS[j][k];
			}
		}
        // update bs
        for (int j = 0; j < numService; j++) {
            bs[j] -= eta * gradbs[j];
        }
    }

    // --- make predictions
    updatePredMatrix(true, removedMatrix, predMatrix, U, S, bs, numUser, numService, dim);

    delete2DMatrix(gradU);
    delete2DMatrix(gradS);
    delete ((char*) U);
    delete ((char*) S);
    delete gradbs;
    delete ((char*) removedMatrix);
    delete ((char*) predMatrix);
}


inline double sqr(double x) {return x * x;}

double loss(double **U, double **S, double *bs, double **removedMatrix,	double **predMatrix, 
    double lmda, int numUser, int numService, int dim)
{
	int i, j, k, g;
	double loss = 0;

	// cost
	for (i = 0; i < numUser; i++) {
		for (j = 0; j < numService; j++) {
			if (fabs(removedMatrix[i][j]) > eps) {
                loss += 0.5 * sqr(removedMatrix[i][j] - predMatrix[i][j]);
			}
		}
	}

	// L2 regularization
	for (k = 0; k < dim; k++) {
		for (i = 0; i < numUser; i++) {
			loss += 0.5 * lmda * sqr(U[i][k]);
		}
		for (j = 0; j < numService; j++) {
			loss += 0.5 * lmda * sqr(S[j][k]);
		}
	}
    for (j = 0; j < numService; j++) {
        loss += 0.5 * lmda * sqr(bs[j]);
    }    
    
	return loss;
}


void gradLoss(double **U, double **S, double *bs, double **removedMatrix, double **predMatrix, 
    double **gradU, double **gradS, double *gradbs, double lmda, int numUser, int numService, int dim)
{
	int i, j, k, g;
	double grad;

	// gradU
	for (i = 0; i < numUser; i++) {
		for (k = 0; k < dim; k++) {
			grad = 0;
			for (j = 0; j < numService; j++) {
				if (fabs(removedMatrix[i][j]) > eps) {
					grad += (removedMatrix[i][j] - predMatrix[i][j])
						* (-S[j][k]);
				} 
			}
			grad += lmda * U[i][k];
			gradU[i][k] = grad;
		}
	}

	// gradS
	for (j = 0; j < numService; j++) {
		for (k = 0; k < dim; k++) {
			grad = 0;
			for (i = 0; i < numUser; i++) {
				if (fabs(removedMatrix[i][j]) > eps) {
					grad += (removedMatrix[i][j] - predMatrix[i][j])
						* (-U[i][k]);
				} 
			}
			grad += lmda * S[j][k];
			gradS[j][k] = grad;			
		}
	}

    // gradbs
    for (j = 0; j < numService; j++) {
        grad = 0;
        for (i = 0; i < numUser; i++) {
            if (fabs(removedMatrix[i][j]) > eps) {
                grad += (removedMatrix[i][j] - predMatrix[i][j]);
            }
        }
        grad = -grad + lmda * bs[j];
        gradbs[j] = grad;
    }
}


double linesearch(double **U, double **S, double *bs, double **removedMatrix, double lastLossValue, 
    double **gradU, double **gradS, double *gradbs, double etaInit,	double lmda, int numUser, 
	int numService, int dim)
{
	double eta = etaInit;
	double lossValue;
	double **U1 = createMatrix(numUser, dim);
	double **S1 = createMatrix(numService, dim);
	double *bs1 = new double[numService];
	double **predMatrix1 = createMatrix(numUser, numService);

	int iter, i, j, k;
	for (iter = 0; iter < 20; iter++) {
    	// update U1, S1
		for (k = 0; k < dim; k++) {
			// update U1
			for (i = 0; i < numUser; i++) {
				U1[i][k] = U[i][k] - eta * gradU[i][k];
			}
			// update S1
			for (j = 0; j < numService; j++) {
				S1[j][k] = S[j][k] - eta * gradS[j][k];
			}
		}
		// update bs1
        for (j = 0; j < numService; j++) {
            bs1[j] = bs[j] - eta * gradbs[j];
        }

        updatePredMatrix(false, removedMatrix, predMatrix1, U1, S1, bs1, numUser, numService, dim);
        lossValue = loss(U1, S1, bs1, removedMatrix, predMatrix1, lmda, numUser, numService, dim);

        if (lossValue <= lastLossValue) {
            break;            
        }
        eta = eta / 2;
    }

    delete2DMatrix(U1);
    delete2DMatrix(S1);
    delete2DMatrix(predMatrix1);
    delete bs1;

    return eta;
}


void updatePredMatrix(bool flag, double **removedMatrix, double **predMatrix, double **U, 
	double **S, double *bs, int numUser, int numService, int dim)
{
	int i, j;
	for (i = 0; i < numUser; i++) {
		for (j = 0; j < numService; j++) {
			predMatrix[i][j] = 0;
			if (flag == true || fabs(removedMatrix[i][j]) > eps) {			
				predMatrix[i][j] = bs[j] + dotProduct(U[i], S[j], dim);
			}
		}
	}
}


double **vector2Matrix(double *vector, int row, int col)  
{
	double **matrix = new double *[row];
	if (!matrix) {
		cout << "Memory allocation failed in vector2Matrix." << endl;
		return NULL;
	}

	int i;
	for (i = 0; i < row; i++) {
		matrix[i] = vector + i * col;  
	}
	return matrix;
}


double dotProduct(double *vec1, double *vec2, int len)  
{
	double product = 0;
	int i;
	for (i = 0; i < len; i++) {
		product += vec1[i] * vec2[i];
	}
	return product;
}


double **createMatrix(int row, int col) 
{
    double **matrix = new double *[row];
    matrix[0] = new double[row * col];
    memset(matrix[0], 0, row * col * sizeof(double)); // Initialization
    int i;
    for (i = 1; i < row; i++) {
    	matrix[i] = matrix[i - 1] + col;
    }
    return matrix;
}


void delete2DMatrix(double **ptr) {
	delete ptr[0];
	delete ptr;
}


const string currentDateTime() 
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

