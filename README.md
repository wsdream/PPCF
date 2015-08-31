##PPCF

This repository maintains a set of privacy-preserving QoS prediction approaches for Web service recommendation.

Read more information from our paper: 

>Jieming Zhu, Pinjia He, Zibin Zheng, and Michael R. Lyu, "**A Privacy-Preserving QoS Prediction Framework for Web Service Recommendation**," *in Proc. of IEEE ICWS*, 2015. [[Paper](http://jiemingzhu.github.io/pub/jmzhu_icws2015.pdf)][[Project page](http://wsdream.github.io/PPCF)]


###Related Links

- [Publication list of Web service recommendation research](https://github.com/wsdream/pywsrec/blob/master/docs/paperlist.rst)

- [WS-DREAM QoS datasets](https://github.com/wsdream/dataset)


###Dependencies
- Python 2.7 (https://www.python.org)
- numpy 1.8.1 (http://www.scipy.org)
- scipy 0.13.3 (http://www.scipy.org)
- pywsrec (https://github.com/wsdream/pywsrec)


###Usage

The benchmark is implemented as a Python package. For efficiency purpose, the core algorithms are written as Python extension using C++, and have been built into `pywsrec` package for common use.

1. Install `pywsrec` package
  
  Download the package: [https://github.com/wsdream/pywsrec/blob/master/dist/pywsrec.tar.gz](https://github.com/wsdream/pywsrec/blob/master/dist/pywsrec.tar.gz?raw=true),

  or use Git: `git clone https://github.com/wsdream/pywsrec.git`,

  Then install the package `python setup.py install --user`.    

2. Read `"readme.txt"` for each benchmark
3. Configure the parameters in benchmark script
  
  For example, in `run_rt.py`, you can config the `'parallelMode': True` if you are running a multi-core machine. You can also set `'rounds': 1` for testing, which make the execution finish soon.

3. Run the benchmark scripts
     
  ```    
    $ python run_rt.py
    $ python run_tp.py 
    ```
4. Check the evaluation results in "result/" directory. Note that the repository has maintained the evaluation results on [WS-DREAM datasets](https://github.com/wsdream/dataset) that are ready for immediate use.

**Note**: Our code is executable on Linux, MacOS, and Windows platforms. For more usage information, please check [pywsrec manual](https://github.com/wsdream/pywsrec/blob/master/docs/manual.rst).


###Feedback
For bugs and feedback, please post to [our issue page](https://github.com/wsdream/PPCF/issues). For any other enquires, please drop an email to our team (wsdream.maillist@gmail.com).


###License
[The MIT License (MIT)](./LICENSE)

Copyright &copy; 2015, [WS-DREAM](https://wsdream.github.io), CUHK

