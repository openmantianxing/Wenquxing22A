from torch import det
import snnfunctions as sf
import datetime
import numpy as np

testImg = np.load("./data/nomalizedData.npy")
train_labels = np.load("./data/label.npy")
weight = np.load("./data/standard_weight.npy")
trainImg = np.load("./data/preprocessedData.npy")
# initialize parameters
start_time = datetime.datetime.now()        # time calc
neurons = np.zeros(20, dtype=np.int)        # initialize neurons
neuronLabels = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10]) # teacher signals of each neuron
vleak = 15                                  # leaky voltage
vth = 30                                   # thrashold voltage
# weight defination
active_weight = sf.initialize_weight_matrix(784, 20)    
for i in range(len(weight)):
    active_weight[i] = weight[i]
iterations = 100
# active learning
print("active learning..................")
detect = 10
for (testimg, trainimg, label, p) in zip(testImg, trainImg, train_labels, range(1, len(testImg) + 1)):
    neuSpike = np.zeros(20, dtype= np.int)
    frequence = np.zeros(20, dtype=np.int)
    poisson_input_test = []
    poisson_input_train = []
    poisson_input_test = np.asarray(sf.PoissonEncoder(iterations, poisson_input_test, testimg))
    poisson_input_train = np.asarray(sf.PoissonEncoder(iterations, poisson_input_train, trainimg))
    # test
    for test in poisson_input_test:
        for j in range(detect):
            vin = np.sum(test.reshape(784,) & active_weight[j])
            (neurons[j], neuSpike[j]) = sf.neuCharge(neurons[j], vin, vleak, vth)
        frequence += neuSpike
    result = np.argmax(frequence)
    # learning
    if(neuronLabels[result] != label):
        num = np.where(neuronLabels == label)
        if(np.shape(num)[1] == 1):
            if(detect < 20):
                detect += 1
                neuronLabels[detect-1] = label
                neuSpike = np.zeros(20, dtype= np.int)
                for test in poisson_input_train:
                    vin = np.sum(test.reshape(784,) & active_weight[detect-1])
                    (neurons[detect-1], neuSpike[detect-1]) = sf.neuCharge(neurons[detect-1], vin, vleak, vth)
                    if neurons[detect-1] < 0:
                        neurons[detect-1] = 0
                    active_weight = sf.train_network(neuSpike, test.reshape(784,), active_weight, detect-1)
                    for i in range(20):
                        if neuronLabels[i] == label:
                            continue
                        else:
                            active_weight = sf.lpd(150, active_weight, i)
        else:
            for n in range(10, 20):
                if(label == neuronLabels[n]):
                    break
            for test in poisson_input_train:
                vin = np.sum(test.reshape(784,) & active_weight[n])
                (neurons[n], neuSpike[n]) = sf.neuCharge(neurons[n], vin, vleak, vth)
                if neurons[n] < 0:
                    neurons[n] = 0
                active_weight = sf.train_network(neuSpike, test.reshape(784,), active_weight, n)
                for i in range(20):
                    if neuronLabels[i] == label:
                        continue
                    else:
                        active_weight = sf.lpd(150, active_weight, i)
    sf.processBar(p, len(testImg), "Active-training")
print("")
print("done!")
np.savez('./data/active-learning-weight-labels',weight = active_weight, labels = neuronLabels)

end_time = datetime.datetime.now()
print("running time: ", end_time - start_time)