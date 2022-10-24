<<<<<<< HEAD
=======
from contextlib import suppress
from turtle import pos
>>>>>>> 3f196b5a1765514cb028351f6e3e14da094bab51
import numpy as np
import  sys
import snnfunctions as sf

np.set_printoptions(suppress = True, linewidth=3000, threshold=np.inf)
iterations = 100
imgs = np.load('./data/nomalizedData_after_OTSU.npy')
labels = np.load('./data/label.npy')
for n in range(10):
    f = open("./fpga_data/"+str(n)+".txt","a+")
    for i in range(1000):
        sf.processBar(i, 1000, "Saving "+str(n))
        poisson_input = []
        if(labels[i] == n):
            data_out = []
            poisson_input = np.asarray(sf.PoissonEncoder(iterations, poisson_input, imgs[i])).reshape([100,784])
            for input in poisson_input:
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
        else:
            continue        
    print("")
f.close()