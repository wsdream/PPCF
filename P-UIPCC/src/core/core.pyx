#########################################################
# core.pyx
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/2/6
# Last updated: 2015/02/11
#########################################################

import numpy as np
from utilities import *
cimport numpy as np # import C-API
from libcpp cimport bool


#########################################################
# Make declarations on functions from cpp file
#
cdef extern from "UIPCC.h":
    void UPCC_core(double *removedData, int numUser, int numService, int topK, 
        double *predData, bool isUserSimilarity)
#########################################################


#########################################################
# Function to perform UMEAN
# return the predicted matrix
#
def UMEAN(removedMatrix):
    predMatrix = IMEAN(removedMatrix.T).T
    return predMatrix
#########################################################


#########################################################
# Function to perform IMEAN
# return the predicted matrix
#
def IMEAN(removedMatrix):  
    numUser = removedMatrix.shape[0]
    idxMatrix = (removedMatrix != 0)
    columnSum = np.sum(idxMatrix, axis=0)   
    imean = np.sum(removedMatrix, axis=0) / (columnSum
            + np.spacing(1)) # avoid divide-by-zero
    predMatrix = np.tile(imean, (numUser, 1))
    return predMatrix
#########################################################


#########################################################
# Function to perform UPCC_wrapper
# return the predicted matrix
#
def UPCC_wrapper(removedMatrix, para, isUserSimilarity):
    cdef int numUser = removedMatrix.shape[0]
    cdef int numService = removedMatrix.shape[1]
    cdef int topK = para['topK']
    cdef np.ndarray[double, ndim=2, mode='c'] predMatrix = \
        np.zeros((numUser, numService), dtype=np.float64)
    UPCC_core(<double *> (
        <np.ndarray[double, ndim=2, mode='c']> removedMatrix).data, 
        numUser, 
        numService,
        topK, 
        <double *> predMatrix.data,
        <bool> isUserSimilarity)
    return predMatrix
#########################################################


#########################################################
# Function to perform UPCC
# return the predicted matrix
#
def UPCC(removedMatrix, para):
    predMatrix = UPCC_wrapper(removedMatrix, para, True)
    return predMatrix
#########################################################


#########################################################
# Function to perform IPCC
# return the predicted matrix
#
def IPCC(removedMatrix, para):
    # use copy() to make sure the transpose makes effect 
    # in C-type memory
    removedMatrix_T = removedMatrix.T.copy()
    predMatrix = UPCC_wrapper(removedMatrix_T, para, False)
    predMatrix = predMatrix.T
    return predMatrix
#########################################################


#########################################################
# Function to perform UIPCC
# return the predicted matrix
#
def UIPCC(removedMatrix, predMatrixUPCC, predMatrixIPCC, para)  :
    lmd = para['lambda']
    predMatrix = lmd * predMatrixUPCC + (1 - lmd) * predMatrixIPCC
    return predMatrix
#########################################################


