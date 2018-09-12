## PPCF

This repository maintains privacy-preserving QoS prediction approaches for Web service recommendation.

Read more information from our paper: 

>Jieming Zhu, Pinjia He, Zibin Zheng, and Michael R. Lyu, "**A Privacy-Preserving QoS Prediction Framework for Web Service Recommendation**," *in Proc. of IEEE ICWS*, 2015. [[Paper](http://jiemingzhu.github.io/pub/jmzhu_icws2015.pdf)][[Project page](http://wsdream.github.io/PPCF)]


### Dependencies
- Python 2.7
- numpy
- scipy 
- cython

### Usage

The benchmark is implemented in Python. For efficiency purpose, the core algorithms are written as Python extension using C++.

1. Download the package: [https://github.com/wsdream/PPCF/archive/master.zip](https://github.com/wsdream/PPCF/archive/master.zip),

   or use Git: `git clone https://github.com/wsdream/PPCF.git`.

2. Download [WS-DREAM datasets](https://github.com/wsdream/wsdream-dataset) to the `data` folder.

3. Build extension modules based on Cython

   ```
   $ cd PPCF/
   $ python setup.py build_ext --inplace
   ```  

4. Run the demo scripts
     
   ```
   $ cd PPCF/demo/P-PMF
   $ python run_rt.py
   $ python run_tp.py 
   ```
5. Check the evaluation results in "result/" directory. Note that we have already provided the results of 20 random runs in the result directory for your quick reference.


### Feedback
For bugs or feedback, please post to [our issue page](https://github.com/wsdream/PPCF/issues). 


### License
[The MIT License (MIT)](./LICENSE)


