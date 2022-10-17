from http.client import ImproperConnectionState
from cv2 import normalize
import read_mnist as rm
import snnfunctions as sf
import datetime
import numpy as np
import time

start_time = datetime.datetime.now()
print("start preprocessing..................")
(X_train, train_labels) = rm.load_mnist(r'..\MNIST\download\MNIST\raw', kind='train')
images = []
# image normalization
for (img, i) in zip(X_train, range(1, len(X_train) + 1)): 
    images.append(sf.deskew(img.reshape(28, 28) / 255.0))
    sf.processBar(i, len(X_train), "Normalization")
print("")
images = np.asarray(images)
np.save("./data/nomalizedData.npy", images)
np.save("./data/label.npy", train_labels)
X_test, test_label = rm.load_mnist(r'..\MNIST\download\MNIST\raw',kind='t10k')
testimages = []
#soft_test = []
# image normalization
for img in X_test: 
    testimages.append(sf.deskew(img.reshape(28, 28) / 255.0))
#soft_test = sf.imgSoft(testimages, soft_test, 0.80)
#soft_test = np.asarray(soft_test)
np.save("./data/nomalizedDataTest.npy", testimages)
end_time = datetime.datetime.now()
print("done!")
print("running time: ", end_time - start_time)
