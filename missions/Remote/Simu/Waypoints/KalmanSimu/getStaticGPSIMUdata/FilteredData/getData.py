__author__ = 'David'

import numpy as np
from numpy.random import *
from scipy.interpolate import interp1d
import matplotlib.pyplot as plt
import os

infile = open('filterdata.alog','r')
navx = np.array([[0, 0]],float)
navy = np.array([[0, 0]],float)
navgyrox = np.array([[0, 0]],float)
navgyroy = np.array([[0, 0]],float)
navgyroz = np.array([[0, 0]],float)
navaccelx = np.array([[0, 0]],float)
navaccely = np.array([[0, 0]],float)
navaccelz = np.array([[0, 0]],float)
navtime = np.array([[0, 0]],float)
navlong = np.array([[0, 0]],float)
navlat = np.array([[0, 0]],float)
navheading = np.array([[0, 0]],float)



for line in infile:
    data = np.array([[1, 1]],float)
    l = line.split()
    if l[1]=="NAV_X":
        data[0, 0] = l[0]
        data[0, 1] = l[3]
        navx = np.concatenate((navx, data))
    elif l[1]=="NAV_Y":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navy = np.concatenate((navy,data))
    elif l[1]=="NAV_GYROX":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navgyrox = np.concatenate((navgyrox,data))
    elif l[1]=="NAV_GYROY":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navgyroy = np.concatenate((navgyroy,data))
    elif l[1]=="NAV_GYROZ":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navgyroz = np.concatenate((navgyroz,data))
    elif l[1]=="NAV_ACCELX":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navaccelx = np.concatenate((navaccelx,data))
    elif l[1]=="NAV_ACCELY":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navaccely = np.concatenate((navaccely,data))
    elif l[1]=="NAV_ACCELZ":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navaccelz = np.concatenate((navaccelz,data))
    elif l[1]=="NAV_TIME":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navtime = np.concatenate((navtime,data))
    elif l[1]=="NAV_LAT":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navlat =np.concatenate((navlat,data))
    elif l[1]=="NAV_LONG":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navlong =np.concatenate((navlong,data))
    elif l[1]=="NAV_HEADING":
        data[0,0] = l[0]
        data[0,1] = l[3]
        navheading = np.concatenate((navheading,data))

#Retirando o elemento inicial que nao conta (vem da inicializacao)
navx = navx[1:,:]
navy = navy[1:,:]
navgyrox = navgyrox[1:,:]
navgyroy = navgyroy[1:,:]
navgyroz = navgyroz[1:,:]
navaccelx = navaccelx[1:,:]
navaccely = navaccely[1:,:]
navaccelz = navaccelz[1:,:]
navlat = navlat[1:,:]
navlong = navlong[1:,:]
navtime = navtime[1:,:]
navheading = navheading[1:,:]



print(navx[:,1].mean(axis=0))
print(navy[:,1].mean(axis=0))
print(navgyrox[:,1].mean(axis=0))
print(navgyroy[:,1].mean(axis=0))
print(navgyroz[:,1].mean(axis=0))
print(navaccelx[:,1].mean(axis=0))
print(navaccely[:,1].mean(axis=0))
print(navaccelz[:,1].mean(axis=0))
print(navheading[:,1].mean(axis=0))


infile.close()