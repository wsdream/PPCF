#########################################################
# run_rt.py 
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/2/6
# Last updated: 2015/02/11
# Implemented approaches: Privacy-preserving variants of 
# [UMEAN, IMEAN, UPCC, IPCC, UIPCC]
#########################################################

import numpy as np
import os, sys, time
import multiprocessing
sys.path.append('../')
from commons.utils import logger
from commons import utils
import evaluator
from commons import dataloader


# parameter config area
para = {'dataPath': '../data/',
        'dataName': 'dataset#1',
        'dataType': 'rt', # set the dataType as 'rt' or 'tp'
		'outPath': 'result/',
		'metrics': ['MAE', 'NMAE', 'RMSE'], # delete where appropriate
		'density': np.arange(0.05, 0.31, 0.05), # matrix density 
		'rounds': 20, # how many runs are performed at each matrix density
		'topK': 10, # the parameter of TopK similar users or services, the default value is
					# topK = 10 as in the reference paper
		'lambda': 0.9, # the combination coefficient of UPCC and IPCC
		'noiseType': 'uniform', # the type of noises: 'guassian' or 'uniform'
		'noiseRange': 0.5, # the range of the noises
		'saveTimeInfo': True, # whether to keep track of the running time
		'saveLog': True, # whether to save log into file
		'debugMode': False, # whether to record the debug info
		'parallelMode': False # whether to leverage multiprocessing for speedup
		}


startTime = time.time() # start timing
utils.setConfig(para) # set configuration
logger.info('==============================================')
logger.info('Approach: Privacy-preserving [UMEAN, IMEAN, UPCC, IPCC, UIPCC].')

# load the dataset
dataMatrix = dataloader.load(para)

# evaluate QoS prediction algorithm
evaluator.execute(dataMatrix, para)

logger.info('All done. Elaspsed time: ' + utils.formatElapsedTime(time.time() - startTime)) # end timing
logger.info('==============================================')