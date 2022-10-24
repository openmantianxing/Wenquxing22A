import snnfunctions as sf
import datetime
import numpy as np

Img = np.load("./data/nomalizedData.npy")
train_labels = np.load("./data/label.npy")

start_time = datetime.datetime.now()
# initialize weight matrix
w_m_1 = sf.initialize_weight_matrix(784, 10)#np.load("./data/standard_weight_256.npy")
# initialize neuron
neuron_1 = np.zeros(10, dtype= np.int)
vth_1 = 30
vleak_1 = 15
iterations = 100
train_size = 1000
print("start standard training........................")
for img, label, p in zip(range(train_size), train_labels, range(1, train_size + 1)):
    poisson_input = []
    neuSpike_1 = np.zeros(10, dtype= np.int)
    poisson_input = np.asarray(sf.PoissonEncoder(iterations, poisson_input, Img[img]))
    frequence = np.zeros(len(neuron_1), dtype=np.int)
    for test in poisson_input:
        vin_1 = np.sum(test.reshape(784,) & w_m_1[label])
        (neuron_1[label], neuSpike_1[label]) = sf.neuCharge(neuron_1[label], vin_1, vleak_1, vth_1)
       if neuron_1[label] < 0:
            neuron_1[label] = 0
        w_m_1 = sf.train_network(neuSpike_1, test.reshape(784,), w_m_1, label, 256)
    sf.processBar(p, train_size, "Standard-Training")
print("")
print("done!")
np.save("./data/standard_weights.npy", w_m_1)

end_time = datetime.datetime.now()
print("running time: ", end_time - start_time)
