########################################################
# setup.py 
# setup script to build extension model
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/4/20
# Last updated: 2014/7/15
########################################################


import sys
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import shutil
import numpy

print('Build extension modules...')
print('==============================================')

ext_modules = [
    Extension('P_UIPCC',
              ['PPCF/P_UIPCC/P_UIPCC.pyx', 
               'PPCF/P_UIPCC/cP_UIPCC.cpp'
              ],
              language='c++',
              include_dirs=[numpy.get_include()],
              extra_compile_args=["-O2"]),
    Extension('P_PMF',
              ['PPCF/P_PMF/P_PMF.pyx', 
               'PPCF/P_PMF/cP_PMF.cpp'
              ],
              language='c++',
              include_dirs=[numpy.get_include()],
              extra_compile_args=["-O2"])
]

setup(
    name = 'Extended Cython module',
    cmdclass = {'build_ext': build_ext},
    ext_modules = ext_modules
)

shutil.move('P_UIPCC.so', 'PPCF/P_UIPCC.so')
shutil.move('P_PMF.so', 'PPCF/P_PMF.so')
print('==============================================')
print('Build done.\n')