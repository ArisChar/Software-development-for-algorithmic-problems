
import tensorflow as tf
from keras.layers import Input, Dense, Conv1D, MaxPooling1D, UpSampling1D, BatchNormalization, LSTM, RepeatVector
from keras.models import Model
from keras.models import model_from_json
from keras import regularizers
import datetime
import time
import requests as req
import json
import pandas as pd
import pickle
import os
import numpy as np
import keras
from sklearn.preprocessing import MinMaxScaler
from tqdm import tqdm
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from random import sample
import argparse
from util import *
import sys



args = get_args(sys.argv)

path,query, outd, outq = get_args(args)

RANDOM_SEED = 42

np.random.seed(RANDOM_SEED)
tf.random.set_seed(RANDOM_SEED)


window_length = 10
encoding_dim = 3
epochs = 100
# path = "nasdaq2007_17.csv"


# Δημιουργία dataframe διαβάζοντας τo csv που δόθηκε.
# Τοποθετήστε το δικό σας path 

df = pd.read_csv(path, '\t', header=None)


df = df.transpose()
new_header = df.iloc[0] 
df = df[1:]
df.columns = new_header



train_size = int(len(df) * 0.80)
test_size = len(df) - train_size


scaler = MinMaxScaler()
# x_train_nonscaled = np.array([df.iloc[:,0:1].values[i-window_length:i].reshape(-1, 1) for i in tqdm(range(window_length+1,len(df.iloc[:,0:1])))])
x_train_scaled = np.array([scaler.fit_transform(df.iloc[:,0:1].values[i-window_length:i].reshape(-1, 1)) for i in tqdm(range(window_length+1,len(df.iloc[:,0:1])))])

x_train = x_train_scaled[:-test_size]
x_test = x_train_scaled[-test_size:]

for j in range(1, 100):
    # x_train_nonscaled = np.array([df.iloc[:,j:j+1].values[i-window_length:i].reshape(-1, 1) for i in tqdm(range(window_length+1,len(df.iloc[:,j:j+1])))])
    x_train_scaled = np.array([scaler.fit_transform(df.iloc[:,j:j+1].values[i-window_length:i].reshape(-1, 1)) for i in tqdm(range(window_length+1,len(df.iloc[:,j:j+1])))])

    x_train = np.append(x_train, x_train_scaled[:-test_size], axis=0)
    x_test = np.append(x_test, x_train_scaled[-test_size:], axis=0)



def plot_examples(stock_input, stock_decoded):
    val = int(test_size/10)
    n = 10  
    plt.figure(figsize=(20, 4))
    for i, idx in enumerate(list(np.arange(0, test_size, val))):
        # display original
        ax = plt.subplot(2, n, i + 1)
        if i == 0:
            ax.set_ylabel("Input", fontweight=600)
        else:
            ax.get_yaxis().set_visible(False)
        plt.plot(stock_input[idx])
        ax.get_xaxis().set_visible(False)
        

        # display reconstruction
        ax = plt.subplot(2, n, i + 1 + n)
        if i == 0:
            ax.set_ylabel("Output", fontweight=600)
        else:
            ax.get_yaxis().set_visible(False)
        plt.plot(stock_decoded[idx])
        ax.get_xaxis().set_visible(False)
        
        
def plot_history(history):
    plt.figure(figsize=(15, 5))
    ax = plt.subplot(1, 2, 1)
    plt.plot(history.history["loss"])
    plt.title("Train loss")
    ax = plt.subplot(1, 2, 2)
    plt.plot(history.history["val_loss"])
    plt.title("Test loss")


x_train_deep = x_train.reshape((len(x_train), np.prod(x_train.shape[1:])))
x_test_deep = x_test.reshape((len(x_test), np.prod(x_test.shape[1:])))


input_window = Input(shape=(window_length,1))
x = Conv1D(16, 3, activation="relu", padding="same")(input_window) # 10 dims
#x = BatchNormalization()(x)
x = MaxPooling1D(2, padding="same")(x) # 5 dims
x = Conv1D(1, 3, activation="relu", padding="same")(x) # 5 dims
#x = BatchNormalization()(x)
encoded = MaxPooling1D(2, padding="same")(x) # 3 dims

encoder = Model(input_window, encoded)

# 3 dimensions in the encoded layer
x = Conv1D(1, 3, activation="relu", padding="same")(encoded) # 3 dims
#x = BatchNormalization()(x)
x = UpSampling1D(2)(x) # 6 dims
x = Conv1D(16, 2, activation='relu')(x) # 5 dims
#x = BatchNormalization()(x)
x = UpSampling1D(2)(x) # 10 dims
decoded = Conv1D(1, 3, activation='sigmoid', padding='same')(x) # 10 dims
autoencoder = Model(input_window, decoded)
autoencoder.summary()

autoencoder.compile(optimizer='adam', loss='binary_crossentropy')


# history = autoencoder.fit(x_train, x_train,
#                 epochs=epochs,
#                 batch_size=1024,
#                 shuffle=True,
#                 validation_data=(x_test, x_test))


# plot_history(history)

path ='myModel_c'
autoencoder = keras.models.load_model(path)

decoded_stocks = autoencoder.predict(x_test)

endecoded_stocks = encoder.predict(x_test)

endecoded_stocks = np.reshape(endecoded_stocks, (endecoded_stocks.size,1) )

endecoded_stocks = scaler.inverse_transform(endecoded_stocks)
val = int(endecoded_stocks.shape[0]/window_length)
endecoded_stocks= np.reshape(endecoded_stocks, (val , window_length) )

df = pd.DataFrame(endecoded_stocks)
df = df.transpose()
df.to_csv(outd, sep ='\t')