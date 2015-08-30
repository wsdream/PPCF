****************************************************************************
* README file for P-PMF
* Author: Jamie Zhu <jimzhu@GitHub>
* Last updated: 2015/08/14.  
****************************************************************************

This package implements a privacy-preserving QoS prediction approach, P-PMF,
which has been published in ICWS'15.

****************************************************************************
Reference and citation
****************************************************************************

Please refer to the following papers for the detailed descriptions of the 
implemented algorithms:

- Jieming Zhu, Pinjia He, Zibin Zheng, and Michael R. Lyu, "A Privacy-Preserving 
  QoS Prediction Framework for Web Service Recommendation," in Proc. of IEEE 
  ICWS, 2015.

IF YOU USE THIS PACKAGE IN PUBLISHED RESEARCH, PLEASE CITE THE ABOVE PAPERS. 
THANKS!

****************************************************************************
Dependencies
****************************************************************************

- Python 2.7 (https://www.python.org)
- numpy 1.8.1 (http://www.scipy.org)
- scipy 0.13.3 (http://www.scipy.org)
- pywsrec (https://github.com/wsdream/pywsrec)

The benchmark is implemented in Python. For efficiency purpose, the core 
algorithms are written as Python extension using C++, and have been built 
into pywsrec package for common use. 

****************************************************************************
Contents of this package
****************************************************************************

P-PMF/
|-- readme.txt                  - description of this package 
|-- run_rt.py                   - script file for running the experiments on 
|                                 response-time QoS data 
|-- run_tp.py                   - script file for running the experiments on 
|                                 throughput QoS data
|-- evaluator.py                - control execution and results collection of 
|                                 the QoS prediction algorithm
|-- result/                     - directory for storing evaluation results
|  |                              with metrics: (MAE, NMAE, RMSE, MRE, NPRE)
|  |-- dataset#1_rt_result.txt  - The response-time prediction result
|  |-- dataset#1_tp_result.txt  - The throughput prediction result

../commons/
|-- dataloader.py        - a script to load the dataset (with preprocessing)
|-- evallib.py           - common functions for evaluator.py
|-- utils.py             - a script containing a bag of useful utilities

****************************************************************************
Usage of this package
****************************************************************************

For ease of reproducing and compare with other approaches, we provide the 
detailed experimental results with five metrics (MAE, NMAE, RMSE, MRE, NPRE), 
under the "result/" directory, after running the above QoS prediction approaches
on the dataset: "data/dataset#1". E.g.,"result/rtResult_0.05.txt" records the 
evaluation results under matrix density = 5%. In particular, each experiment 
is run for 20 times and the average result (including std value) is reported. 
These results can be directly used for your research work.

More usage information is available at: https://github.com/wsdream/PPCF

****************************************************************************
Issues
****************************************************************************

In case of questions or problems, please do not hesitate to report to our 
issue page (https://github.com/wsdream/PPCF/issues). We will help ASAP. In 
addition, we will appreciate any contribution to refine and optimize this 
package.

****************************************************************************
License
****************************************************************************

The MIT License (MIT)
Copyright (c) 2015, WS-DREAM, CUHK (https://wsdream.github.io/)

