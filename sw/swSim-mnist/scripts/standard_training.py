from http.client import IM_USED
from re import I, L
import snnfunctions as sf
import datetime
import numpy as np

Img = np.load("./data/nomalizedData.npy")
train_labels = np.load("./data/label.npy")

start_time = datetime.datetime.now()
# initialize weight matrix
w_m_1 = sf.initialize_weight_matrix(784, 10)#np.load("./data/standard_weight_256.npy")
w_m_2 = sf.initialize_weight_matrix(10, 10)
# initialize neuron
neuron_1 = np.zeros(10, dtype= np.int)
neuron_2 = np.zeros(10, dtype= np.int)
vth_1 = 30
vleak_1 = 15
vth_2 = 5
vleak_2 = 0
iterations = 100
print("start standard training........................")
for img, label, p in zip(range(300), train_labels, range(1, 300 + 1)):
    poisson_input = []
    neuSpike_1 = np.zeros(10, dtype= np.int)
    neuSpike_2 = np.zeros(10, dtype= np.int)
    poisson_input = np.asarray(sf.PoissonEncoder(iterations, poisson_input, Img[img]))
    frequence = np.zeros(len(neuron_1), dtype=np.int)
    for test in poisson_input:
        vin_1 = np.sum(test.reshape(784,) & w_m_1[label])
        (neuron_1[label], neuSpike_1[label]) = sf.neuCharge(neuron_1[label], vin_1, vleak_1, vth_1)
        vin_2 = np.sum(neuSpike_1 & w_m_2)
        (neuron_2[label], neuSpike_2[label]) = sf.neuCharge(neuron_2[label], vin_2, vleak_2, vth_2)
        if neuron_1[label] < 0:
            neuron_1[label] = 0
        if neuron_2[label] < 0:
            neuron_2[label] = 0
        w_m_1 = sf.train_network(neuSpike_1, test.reshape(784,), w_m_1, label, 256)
        w_m_2 = sf.train_network(neuSpike_2, neuSpike_1, w_m_2, label, 7)
    sf.processBar(p, 300, "Standard-Training")
print("")
print("done!")
np.save("./data/standard_weight_256_1.npy", w_m_1)
np.save("./data/standard_weight_10_2.npy", w_m_2)

end_time = datetime.datetime.now()
print("running time: ", end_time - start_time)
