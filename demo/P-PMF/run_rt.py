########################################################
# run_rt.py 
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/2/6
# Last updated: 2015/02/11
# Implemented approach: P-PMF (Privacy-Preserving MF)
########################################################

import numpy as np
import os, sys, time
import logging
sys.path.append('../../')
from PPCF.commons import utils, dataloader
import benchmark
 

# parameter config area
para = {'dataPath': '../../data/',
        'dataName': 'dataset1',
        'dataType': 'rt', # set the dataType as 'rt' or 'tp'
        'outPath': 'result/',
        'metrics': ['MAE', 'NMAE', 'RMSE'], # delete where appropriate      
        'density': np.arange(0.05, 0.31, 0.05), # matrix density
        'rounds': 1, # how many runs are performed at each matrix density
        'dimension': 10, # dimenisionality of the latent factors
        'etaInit': 0.01, # inital learning rate. We use line search
                         # to find the best eta at each iteration
        'lambda': 12, # regularization parameter
        'maxIter': 300, # the max iterations
        'noiseType': 'uniform', # the type of noises: 'guassian' or 'uniform'
        'noiseRange': 0.5, # the range of the noises
        'saveTimeInfo': False, # whether to keep track of the running time
        'debugMode': False, # whether to record the debug info
        'parallelMode': True # whether to leverage multiprocessing for speedup
        }


startTime = time.time() # start timing
utils.setConfig(para) # set configuration
logging.info('==============================================')
logging.info('Approach: P-PMF (Privacy-Preserving MF)')

# load the dataset
dataMatrix = dataloader.load(para)

# evaluate QoS prediction algorithm
benchmark.execute(dataMatrix, para)

logging.info('All done. Elaspsed time: ' + utils.formatElapsedTime(time.time() - startTime)) # end timing
logging.info('==============================================')
