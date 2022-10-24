import read_mnist as rm
import snnfunctions as sf
import datetime
import numpy as np
import time

start_time = datetime.datetime.now()
print("start preprocessing..................")
n = 10000

(X_train, train_labels) = rm.load_mnist(r'..\MNIST\download\MNIST\raw', kind='train')
otsu_imgs = []
images = []
for (k, j) in zip(range(n),range(1, n + 1)):
    o = sf.OTSU(X_train[k].reshape([28,28]))
    otsu_imgs.append(o.Otsu())
    sf.processBar(j, n, "Otsuing")
print("")
# image normalization
for (img, i) in zip(otsu_imgs, range(1, n + 1)): 
    images.append(sf.deskew(img / 255.0))
    sf.processBar(i, n, "Normalization")
print("")
images = np.asarray(images)
np.save("./data/nomalizedData_after_OTSU.npy", images)
np.save("./data/label.npy", train_labels)


otsu_imgs_test = []
X_test, test_label = rm.load_mnist(r'..\MNIST\download\MNIST\raw',kind='t10k')
testimages = []
for (k, j) in zip(range(n),range(1, n + 1)):
    o = sf.OTSU(X_test[k].reshape([28,28]))
    otsu_imgs_test.append(o.Otsu())
    sf.processBar(j, n, "Otsuing")
print("")
# image normalization
for img in otsu_imgs_test: 
    testimages.append(sf.deskew(img / 255.0))
#soft_test = sf.imgSoft(testimages, soft_test, 0.80)
#soft_test = np.asarray(soft_test)
np.save("./data/nomalizedDataTest.npy", testimages)
np.save("./data/label_test.npy",test_label)


end_time = datetime.datetime.now()
print("done!")
print("running time: ", end_time - start_time)
