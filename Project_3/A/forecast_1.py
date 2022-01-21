import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import math
import tensorflow as tf
import matplotlib.pyplot as plt
import keras
import pandas as pd
import numpy as np
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM
from keras.layers import Dropout
from keras.layers import *
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import mean_squared_error
from sklearn.metrics import mean_absolute_error
from sklearn.model_selection import train_test_split
from keras.callbacks import EarlyStopping
from random import sample
import argparse
from util import *
import sys

args = get_args(sys.argv)

path,n = get_args(args)
print(path,n)

df = pd.read_csv(path, '\t', header=None)

df = df.transpose()
new_header = df.iloc[0] 
df = df[1:] 
df.columns = new_header

list = [11, 260, 50, 134, 50]

train_size = int(len(df) * 0.80)
test_size = len(df) - train_size 
Time_step = 60

for j in list:

    training_set = df.iloc[:train_size,j:j+1].values
    test_set = df.iloc[train_size:,j:j+1].values

    sc = MinMaxScaler(feature_range = (0, 1))
    training_set_scaled = sc.fit_transform(training_set)

    # Creating a data structure with 60 time-steps and 1 output
    X_train = []
    y_train = []
    for i in range(Time_step, train_size):
        X_train.append(training_set_scaled[i-Time_step:i, 0])
        y_train.append(training_set_scaled[i, 0])
    X_train, y_train = np.array(X_train), np.array(y_train)
    X_train = np.reshape(X_train, (X_train.shape[0], X_train.shape[1], 1))

#####################################################################################
    #load model

    path ='myModel' + str(j)
    model = keras.models.load_model(path)

#####################################################################################

    dataset_train = df.iloc[:train_size,j:j+1]
    dataset_test = df.iloc[train_size:, j:j+1]
    dataset_total = pd.concat((dataset_train, dataset_test), axis = 0)
    inputs = dataset_total[len(dataset_total) - len(dataset_test) - Time_step:].values
    inputs = inputs.reshape(-1,1)
    inputs = sc.transform(inputs)
    X_test = []
    for i in range(Time_step, test_size + Time_step):
        X_test.append(inputs[i-Time_step:i, 0])
    X_test = np.array(X_test)
    X_test = np.reshape(X_test, (X_test.shape[0], X_test.shape[1], 1))
    # print(X_test.shape)

    predicted_stock_price = model.predict(X_test)
    predicted_stock_price = sc.inverse_transform(predicted_stock_price)

#####################################################################################

    print(j)
    plt.plot(dataset_test.values, color = 'red', label = 'Real')
    plt.plot(predicted_stock_price, color = 'blue', label = 'Predicted')
    plt.xticks(np.arange(0,test_size,Time_step))
    plt.legend()
    plt.show()