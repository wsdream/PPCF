########################################################
# evaluator.py
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/2/6
# Last updated: 2015/8/30
########################################################

import numpy as np 
import time
from commons.utils import logger
from commons import evallib
from pywsrec.PPCF import P_PMF
import multiprocessing


#======================================================#
# Function to evalute the approach at all settings
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
    evallib.summarizeResult(para)


#======================================================#
# Function to run the prediction approach at one setting
#======================================================#
def executeOneSetting(matrix, density, roundId, para):
    logger.info('density=%.2f, %2d-round starts.'%(density, roundId + 1))
    startTime = time.clock()

    # remove data matrix    
    logger.info('Removing data matrix...')
    (trainMatrix, testMatrix) = evallib.removeEntries(matrix, density, roundId) 

    # data perturbation by adding noises
    logger.info('Data perturbation...')
    (perturbMatrix, uMean, uStd) = randomPerturb(trainMatrix, para)

    # QoS prediction
    logger.info('QoS prediction...')
    predictedMatrix = P_PMF.predict(perturbMatrix, para)
    predictedMatrix = reNormalize(predictedMatrix, uMean, uStd)
    runningTime = float(time.clock() - startTime) 

    # evaluate the estimation error  
    evalResult = evallib.evaluate(testMatrix, predictedMatrix, para)
    result = (evalResult, runningTime)

    # dump the result at each density
    outFile = '%s%s_%s_result_%.2f_round%02d.tmp'%(para['outPath'], para['dataName'], 
        para['dataType'], density, roundId + 1)
    evallib.dumpresult(outFile, result)
    
    logger.info('density=%.2f, %2d-round done.'%(density, roundId + 1))
    logger.info('----------------------------------------------')


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
