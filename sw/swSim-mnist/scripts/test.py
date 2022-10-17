import numpy as np
import snnfunctions as sf
import read_mnist as rm

a = np.load("./data/standard_weight_256_84.npy")#np.load("./data/active-learning-weight-labels.npz")
X_test, test_label = rm.load_mnist(r'..\MNIST\download\MNIST\raw',kind='t10k')
images = np.load("./data/nomalizedDataTest.npy")
neurons = np.zeros(len(a), dtype=np.int)  
iterations = 100
vleak = 12
vth = 37
acc = 0
#n = 1
for (testimg, p) in zip(range(10000), range(1, 10001)):
    sf.processBar(p, 10000, "Testing")
    neuSpike = np.zeros(len(a), dtype= np.int)
    frequence = np.zeros(len(a), dtype=np.int)
    poisson_input_test = []
    poisson_input_test = np.asarray(sf.PoissonEncoder(iterations, poisson_input_test, images[testimg]))
    for test in poisson_input_test:
        for j in range(len(neurons)):
            vin = np.sum(test.reshape(784,) & a[j])#['weight'][j])
            (neurons[j], neuSpike[j]) = sf.neuCharge(neurons[j], vin, vleak, vth)
        frequence += neuSpike
    result = np.where(frequence == np.max(frequence))
    '''
    for i in range(len(result)):
        if (a['labels'][result[i]] == label).any():
           acc += 1
           break
        else:
            continue
    
    print(frequence)
    print(test_label[testimg])
    '''
    for i in range(len(result)):
        if (result[i] == test_label[testimg]).any():
            acc += 1
            break
    else:
        continue
#print(a['labels'][result], test_label[n])
#print(a['labels'])
#print(frequence)
#    if a['labels'][result] == label:
#        acc += 1
print("")
print("done!")
print(acc / 10000.00 * 100, "%")