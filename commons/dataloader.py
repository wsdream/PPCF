########################################################
# dataloader.py
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/2/6
# Last updated: 2015/8/29
########################################################

import numpy as np 
from utils import logger
import os


#======================================================#
# Function to load the dataset
#======================================================#
def load(para):
    datafile = para['dataPath'] + para['dataName'] + '/' + para['dataType'] + 'Matrix.txt'
    logger.info('Loading data: %s'%os.path.abspath(datafile))
    dataMatrix = np.loadtxt(datafile)
    dataMatrix = preprocess(dataMatrix, para)
    logger.info('Data size: %d users * %d services'\
        %(dataMatrix.shape[0], dataMatrix.shape[1]))
    logger.info('Loading data done.')
    logger.info('----------------------------------------------') 
    return dataMatrix


#======================================================#
# Function to preprocess the dataset which
# deletes the invalid values
#======================================================#
def preprocess(matrix, para):
    if para['dataType'] == 'rt':
        matrix = np.where(matrix == 0, -1, matrix)
        matrix = np.where(matrix >= 20, -1, matrix)
    elif para['dataType'] == 'tp':
        matrix = np.where(matrix == 0, -1, matrix)
    return matrix
