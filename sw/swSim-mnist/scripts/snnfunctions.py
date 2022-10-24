import random
import numpy as np
import scipy as sp
from scipy.ndimage import interpolation
import sys
import time

def processBar(done, total, str):
    pro = round(done / total * 100)
    print("\r",str,"[", "▓" * (pro // 2), end="] {}%".format(pro))
    sys.stdout.flush()

def moments(image):
    c0,c1 = np.mgrid[:image.shape[0],:image.shape[1]] # A trick in numPy to create a mesh grid
    totalImage = np.sum(image) #sum of pixels
    m0 = np.sum(c0*image)/totalImage #mu_x
    m1 = np.sum(c1*image)/totalImage #mu_y
    m00 = np.sum((c0-m0)**2*image)/totalImage #var(x)
    m11 = np.sum((c1-m1)**2*image)/totalImage #var(y)
    m01 = np.sum((c0-m0)*(c1-m1)*image)/totalImage #covariance(x,y)
    mu_vector = np.array([m0,m1]) # Notice that these are \mu_x, \mu_y respectively
    covariance_matrix = np.array([[m00,m01],[m01,m11]]) # Do you see a similarity between the covariance matrix
    return mu_vector, covariance_matrix

def deskew(image):
    c,v = moments(image)
    alpha = v[0,1]/v[0,0]
    affine = np.array([[1,0],[alpha,1]])
    ocenter = np.array(image.shape)/2.0
    offset = c-np.dot(affine,ocenter)
    return interpolation.affine_transform(image,affine,offset=offset)


def initialize_weight_matrix(input_num, output_num):
    #weight_matrix = [[1] * input_num for n in range(output_num)]
    weight_matrix = np.random.randint(0, 2, (output_num, input_num))
    return weight_matrix

def lpd(wexp, weight_matrix, teacher_signal):
    dw = np.sum(weight_matrix[teacher_signal]) - wexp
    lpd_poss = dw / np.sum(weight_matrix[teacher_signal])
    if lpd_poss < 0:
        lpd_poss = 0
    else:
        for i in range(len(weight_matrix[teacher_signal])):
            if weight_matrix[teacher_signal][i] == 1:
                weight_matrix[teacher_signal][i] = (random.random() >= lpd_poss).astype(int)
    return weight_matrix

def train_network(neuSpike, input, weight_matrix, teacher_signal, wexp):
    if neuSpike[teacher_signal] == 1:
        for i in range(len(input)):
            if input[i] == 1:
                weight_matrix[teacher_signal][i] = 1
        weight_matrix = lpd(wexp, weight_matrix, teacher_signal)
    else:
        weight_matrix = weight_matrix
    for j in range(len(neuSpike)):
        if j == teacher_signal:
            continue
        else:
            weight_matrix = lpd(wexp, weight_matrix, j)
    return weight_matrix

def neuCharge(vneuron, vin, vleak, vth):
    if vneuron >= vth:
        return 0, 1
    else:
        return vneuron + vin - vleak, 0

def PoissonEncoder(steps, poissonOut,inputImg):
    for i in range(steps):
        r = np.random.rand(28, 28)
        poissonOut.append(np.asarray(r <= inputImg).astype(int))
    return poissonOut

def imgSoft(inputImg, softed, ath):
    th = np.ones((28, 28)) * ath
#    softed = inputImg * 1e8
    for (img, i) in zip(inputImg, range(1, len(inputImg) + 1)):
        softed.append(np.asarray(img >= th).astype(int))
        processBar(i, len(inputImg), "Softing")
    print("")
    return softed
	
# 大律法
class OTSU():
    def __init__(self, img):
        self.img = img
        num_list = self.Pixel_num()
        self.num_list = num_list
        rate_list = self.Pixel_rate()
        self.rate_list = rate_list
        optimal_pixel = self.Optimal_partition()
        self.optimal_pixel = optimal_pixel

    def Pixel_num(self):
        num = [0 for _ in range(256)]
        a = np.shape(self.img)
        for i in range(a[0]):
            for j in range(a[1]):
                num[self.img[i][j]] += 1
        return num
 
    def Pixel_rate(self):
        rate_list = []
        n = sum(self.num_list)
        for i in range(len(self.num_list)):
            rate = self.num_list[i] / n
            rate_list.append(rate)
        return rate_list
 
    def Optimal_partition(self):
        deltaMax = 0
        T = 0
        for i in range(256):
            w1 = w2 = u1 = u2 = 0
            u1tmp = u2tmp = 0
            deltaTmp = 0
            for j in range(256):
                if (j <= i):
                    w1 += self.rate_list[j]
                    u1tmp += j * self.rate_list[j]
                else:
                    w2 += self.rate_list[j]
                    u2tmp += j * self.rate_list[j]
            if w1 == 0:
                u1 = 0
            else:
                u1 = u1tmp / w1
            if w2 == 0:
                u2 = 0
            else:
                u2 = u2tmp / w2
            deltaTmp = w1 * w2 * ((u1- u2) ** 2)
            if deltaTmp > deltaMax:
                deltaMax = deltaTmp
                T = i
        return T
 
    def Otsu(self):
        a = np.shape(self.img)
        new_img = np.zeros((a[0], a[1]))
        for i in range(a[0]):
            for j in range(a[1]):
                if self.img[i][j] > self.optimal_pixel:
                    new_img[i][j] = 255
                else:
                    new_img[i][j] = 0
        return new_img

























