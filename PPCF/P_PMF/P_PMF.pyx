########################################################
# P_PMF.pyx
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/2/6
# Last updated: 2015/02/11
########################################################

import time, sys
import numpy as np
cimport numpy as np # import C-API
from libcpp cimport bool


#======================================================#
# Make declarations on functions from cpp file
#======================================================#
cdef extern from "cP_PMF.h":
    void P_PMF(double *removedData, double *predData, int numUser, 
		int numService, int dim, double lmda, int maxIter, double etaInit, 
    	double *Udata, double *Sdata, double *bs, bool debugMode)


#======================================================#
# Function to perform the prediction algorithm
# Wrap up the C++ implementation
#======================================================#
def predict(removedMatrix, para):
    cdef int numService = removedMatrix.shape[1] 
    cdef int numUser = removedMatrix.shape[0] 
    cdef int dim = para['dimension']
    cdef double lmda = para['lambda']
    cdef int maxIter = para['maxIter']
    cdef double etaInit = para['etaInit']
    cdef bool debugMode = para['debugMode']

    # initialization
    cdef np.ndarray[double, ndim=2, mode='c'] U = np.random.rand(numUser, dim)        
    cdef np.ndarray[double, ndim=2, mode='c'] S = np.random.rand(numService, dim)
    cdef np.ndarray[double, ndim=1, mode='c'] bs = np.random.rand(numService)
    cdef np.ndarray[double, ndim=2, mode='c'] predMatrix = np.zeros((numUser, numService))

    # Wrap up cP_PMF.cpp
    P_PMF(
    	  <double *> (<np.ndarray[double, ndim=2, mode='c']> removedMatrix).data,
          <double *> predMatrix.data,
          numUser,
          numService,
          dim,
          lmda,
          maxIter,
          etaInit, 
          <double *> U.data,
          <double *> S.data,
          <double *> bs.data,
          debugMode
         )

    return predMatrix

