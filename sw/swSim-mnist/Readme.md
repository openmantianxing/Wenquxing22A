## Python Version for MNIST Recognition
### Usage
1. Make sure the dependences that are `Numpy` and `Scipy`; download the [MNIST dataset](http://yann.lecun.com/exdb/mnist/);
2. Make sure the path in `read_mnist.py` is right and run `preprocessing.py` to generate the input data with deskewing and OTSU method;
3. Run `standard_training.py` to train 10-neuron SNN. The save path for weight matrixes can be modified in it;
4. Run `active_learning.py` to train 20/40-neuron SNN or with more neurons. The save path for weight matrixes can be modified in it;
5. Run `test.py` for test;
6. Run `spikes_generator.py` to generate spikes with the data format for Wenquxing 22A; run `syn_generator.py` to generate synaptic weights with the data format for Wenquxing 22A.
7. For more detailed about Python Version, click [here](../README.md).