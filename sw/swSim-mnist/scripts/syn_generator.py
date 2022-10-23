import string
import numpy as np
import  sys
import snnfunctions as sf
np.set_printoptions(suppress = True, linewidth=3000, threshold=np.inf)
#initial_weight = (sf.initialize_weight_matrix(784, 10))
w_m = np.load('./data/standard_weight_256_4.npy')
'''
iterations = 100
imgs = np.load("./nomalizedData_after_OTSU.npy")
labels = np.load("./data/label.npy")
n = 0
poisson_input = []
poisson_input = np.asarray(sf.PoissonEncoder(iterations, poisson_input, imgs[0])).reshape([100,784])
'''
f = open("./fpga_data/syn_trained.txt","a+")
data_out = []
for input in w_m:
    data = []
    for j in range(12):
        d = np.uint64(0)
        for k in range(64):
            d += np.uint64(input[7 + j*64 + k])
            if(k != 63):    d <<= np.uint64(1)
            else:           continue
        data.append(d)
    data_out.append(data)
print(data_out, file=f)
data_out = np.asarray(data_out)
print(np.shape(data_out))

f.close()
    
    


