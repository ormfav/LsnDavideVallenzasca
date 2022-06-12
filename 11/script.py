import os
os.environ['KMP_DUPLICATE_LIB_OK']='True'
import tensorflow as tf
from tensorflow import keras

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Activation
from tensorflow.keras import backend as K
from tensorflow.keras.utils import get_custom_objects

import numpy as np


def TestNN (Ntrains, sigmas, Nepochss):
    m=2
    b=1
    for i in range(len(Nepochss)-1):
       Nepochss[i+1]-=Nepochss[i] 
    # open and clear output files
    fout_m = open("out/111-m_diff.csv","a")
    fout_b = open("out/111-b_diff.csv","a")
    fout_m.truncate(0)
    fout_b.truncate(0)
    for Ntrain in Ntrains:
        Nvalid = Ntrain // 10
        #Reset rangen dove lo piazzo?
        # np.random.seed(0)
        # tf.random.set_seed(0)
        #creating training and validation datasets
        x_train = np.random.uniform(-1, 1, Ntrain)
        x_valid = np.random.uniform(-1, 1, Nvalid)
        x_valid.sort()
        y_target = m * x_valid + b 
        for sigma in sigmas:
            y_train = np.random.normal(m * x_train + b, sigma) 
            y_valid = np.random.normal(m * x_valid + b, sigma) 
            #preparing for model fit
            #creating and compiling the model
            model = tf.keras.Sequential()
            model.add(Dense(1, input_shape=(1,)))
            model.compile(optimizer='sgd', loss='mse', metrics=['mse'])
            for Nepochs in Nepochss:
                #optimizing the model
                ## model.fit(epochs=20)=four calls of
                ## model.fit(epochs=5)
                history = model.fit(x=x_train, y=y_train, 
                                    batch_size=32, 
                                    epochs=Nepochs,
                                    shuffle=True, 
                                    validation_data=(x_valid, y_valid))
                # output
                mb = model.get_weights()
                printm=str(mb[0]-m)
                printb=str(mb[1]-b)
                fout_m.write(printm)
                fout_b.write(printb)


