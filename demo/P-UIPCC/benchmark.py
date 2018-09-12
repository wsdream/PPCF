########################################################
# evaluator.py
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/2/6
# Last updated: 2015/8/30
########################################################

import numpy as np
from numpy import linalg as LA
import time, sys, os
import random
import logging
import multiprocessing
import cPickle as pickle
from PPCF import P_UIPCC
from PPCF.commons import evaluator


#======================================================#
# Function to evalute the approach at each density
#======================================================#
def execute(matrix, para):
    # loop over each density and each round
    if para['parallelMode']: # run on multiple processes
        pool = multiprocessing.Pool()
        for den in para['density']: 
            for roundId in xrange(para['rounds']):
                pool.apply_async(executeOneSetting, (matrix, den, roundId, para))
        pool.close()
        pool.join()
    else: # run on single processes
        for den in para['density']:
            for roundId in xrange(para['rounds']):
                executeOneSetting(matrix, den, roundId, para)
    # summarize the dumped results
    summarizeResult(para)


#======================================================#
# Function to run the prediction approach at one setting
#======================================================#
def executeOneSetting(matrix, density, roundId, para):
    logging.info('density=%.2f, %2d-round starts.'%(density, roundId + 1))
    startTime = time.clock()
    timeResult = np.zeros(5)
    evalResult = np.zeros((len(para['metrics']), 5))

    # remove data matrix    
    logging.info('Removing entries from data matrix...')
    (trainMatrix, testMatrix) = evaluator.removeEntries(matrix, density, roundId) 

    # data perturbation by adding noises
    logging.info('Data perturbation...')
    (perturbMatrix, uMean, uStd) = randomPerturb(trainMatrix, para)

    # UMEAN
    logging.info('UMEAN prediction...')
    iterStartTime1 = time.clock()   
    predMatrixUMEAN = P_UIPCC.UMEAN(perturbMatrix)  
    timeResult[0] = time.clock() - iterStartTime1
    
    # IMEAN
    logging.info('IMEAN prediction...')
    iterStartTime2 = time.clock()          
    predMatrixIMEAN = P_UIPCC.IMEAN(perturbMatrix)
    timeResult[1] = time.clock() - iterStartTime2

    # UPCC
    logging.info('UPCC prediction...')
    iterStartTime3 = time.clock()
    predMatrixUPCC = P_UIPCC.UPCC(perturbMatrix, para)
    timeResult[2] = time.clock() - iterStartTime3 + timeResult[0]
    
    # IPCC
    logging.info('IPCC prediction...')
    iterStartTime4 = time.clock()         
    predMatrixIPCC = P_UIPCC.IPCC(perturbMatrix, para) 
    timeResult[3] = time.clock() - iterStartTime4 + timeResult[1]

    # UIPCC
    logging.info('UIPCC prediction...')
    iterStartTime5 = time.clock()       
    predMatrixUIPCC = P_UIPCC.UIPCC(perturbMatrix, predMatrixUPCC, predMatrixIPCC, para)    
    timeResult[4] = time.clock() - iterStartTime5 + timeResult[2] + timeResult[3]

    # evaluate the estimation error  
    predMatrixUMEAN = reNormalize(predMatrixUMEAN, uMean, uStd)
    evalResult[:, 0] = evaluator.evaluate(testMatrix, predMatrixUMEAN, para)

    predMatrixIMEAN = reNormalize(predMatrixIMEAN, uMean, uStd)
    evalResult[:, 1] = evaluator.evaluate(testMatrix, predMatrixIMEAN, para)
   
    predMatrixUPCC = reNormalize(predMatrixUPCC, uMean, uStd)
    evalResult[:, 2] = evaluator.evaluate(testMatrix, predMatrixUPCC, para)

    predMatrixIPCC = reNormalize(predMatrixIPCC, uMean, uStd)
    evalResult[:, 3] = evaluator.evaluate(testMatrix, predMatrixIPCC, para)

    predMatrixUIPCC = reNormalize(predMatrixUIPCC, uMean, uStd)
    evalResult[:, 4] = evaluator.evaluate(testMatrix, predMatrixUIPCC, para)

    # dump the result at each density
    result = (evalResult, timeResult)
    outFile = '%s%s_%s_result_%.2f_round%02d.tmp'%(para['outPath'], para['dataName'], 
        para['dataType'], density, roundId + 1)
    evaluator.dumpresult(outFile, result)
    
    logging.info('density=%.2f, %2d-round done.'%(density, roundId + 1))
    logging.info('----------------------------------------------')


#======================================================#
# Function to process the raw result files 
# Overide evalib.summarizeResult()
#======================================================#
def summarizeResult(para):
    approach = ['UMEAN', 'IMEAN', 'UPCC', 'IPCC', 'UIPCC']
    path = '%s%s_%s_result'%(para['outPath'], para['dataName'], para['dataType'])
    evalResults = np.zeros((len(para['density']), para['rounds'], len(para['metrics']), len(approach))) 
    timeResult = np.zeros((len(para['density']), para['rounds'], len(approach)))   

    k = 0
    for den in para['density']:
        for rnd in xrange(para['rounds']):
            inputfile = path + '_%.2f_round%02d.tmp'%(den, rnd + 1)
            with open(inputfile, 'rb') as fid:
                data = pickle.load(fid)
            (evalResults[k, rnd, :, :], timeResult[k, rnd, :]) = data
            os.remove(inputfile)
        k += 1
    for i in xrange(len(approach)):
        evaluator.saveSummaryResult(path + '_' + approach[i], evalResults[:, :, :, i], timeResult[:, :, i], para)


#======================================================#
# Function to perturb the entries of data matrix
#======================================================#
def randomPerturb(matrix, para):
    perturbMatrix = matrix.copy()
    (numUser, numService) = matrix.shape
    uMean = np.zeros(numUser)
    uStd = np.zeros(numUser)
    noiseRange = para['noiseRange']
    # z-score normalization
    for i in xrange(numUser):
        qos = matrix[i, :]
        qos = qos[qos != 0]
        mu = np.average(qos)
        sigma = np.std(qos)
        uMean[i] = mu
        uStd[i] = sigma
        perturbMatrix[i, :] = (perturbMatrix[i, :] - mu) / sigma
        if para['noiseType'] == 'guassian':
            noiseVec = np.random.normal(0, noiseRange, numService)
        elif para['noiseType'] == 'uniform':
            noiseVec = np.random.uniform(-noiseRange, noiseRange, numService)
        perturbMatrix[i, :] += noiseVec
    perturbMatrix[matrix == 0] = 0
    return (perturbMatrix, uMean, uStd)


#======================================================#
# Function to perturb the entries of data matrix
#======================================================#
def reNormalize(matrix, uMean, uStd):
    numUser = matrix.shape[0]
    resultMatrix = matrix.copy()
    for i in xrange(numUser):
        resultMatrix[i, :] = resultMatrix[i, :] * uStd[i] + uMean[i]
    resultMatrix[resultMatrix < 0] = 0
    return resultMatrix
