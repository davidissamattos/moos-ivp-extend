__author__ = 'David'

import numpy as np
from numpy.random import *
from scipy.interpolate import interp1d
import matplotlib.pyplot as plt
import os

a = np.array([[1, 1]],float)
b = np.array([[2, 4]],float)
c = np.array([[6, 8]],float)
d = np.array([[10, 12]],float)
e = np.array([[15, 20]],float)

f = np.concatenate((a,b,c,d,e))

f= f[1:,:]
print(f)
print(np.mean(f,axis=0))


#print(aaa)
#print(b)
#print(a[0,1])
#c = np.concatenate((a,b))
#print(c)