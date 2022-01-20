import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import math
import tensorflow as tf
import matplotlib.pyplot as plt
import keras
import pandas as pd
import numpy as np
import seaborn as sns

from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM
from keras.layers import Dropout
from keras.layers import *
from keras.callbacks import EarlyStopping

from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import mean_squared_error
from sklearn.metrics import mean_absolute_error
from sklearn.model_selection import train_test_split

from random import sample
import argparse
from util import *
import sys

def create_dataset(X, y, time_steps=1):

    Xs, ys = [], []

    for i in range(len(X) - time_steps):

        v = X.iloc[i:(i + time_steps)].values

        Xs.append(v)

        ys.append(y.iloc[i + time_steps])

    return np.array(Xs), np.array(ys)

args = get_args(sys.argv)

path,n,num_mae = get_args(args)
#print(path,n,num_mae)

# Δημιουργία dataframe διαβάζοντας τo csv που δόθηκε.

# Τοποθετήστε το δικό σας path 

df = pd.read_csv(path, '\t', header=None)

df_2 = df.iloc[:,0]

df = df.transpose()
new_header = df.iloc[0]
df = df[1:] 
df.columns = new_header

train_size = int(len(df) * 0.80)

test_size = len(df) - train_size

train, test = df.iloc[0:train_size], df.iloc[train_size:len(df)]



scaler = StandardScaler()

n_s = len(df.columns)
for j in range(0,n_s ):

    scaler = scaler.fit(train[[df_2[j]]])

    train[df_2[j]] = scaler.transform(train[[df_2[j]]])

    test[df_2[j]] = scaler.transform(test[[df_2[j]]])


TIME_STEPS = 30

# X_train, y_train = create_dataset(

#    train.iloc[:,[0]],

#    train[df_2[0]],

#    TIME_STEPS

# )

# X_train_set = X_train
# y_train_set = y_train

# for j in range(1,n_s ):

#   X_train, y_train = create_dataset(

#   train.iloc[:,[j]],

#   train[df_2[j]],

#   TIME_STEPS

#   )
  
#   X_train_set = np.concatenate((X_train_set,X_train),axis=0)
  
#   y_train_set = np.concatenate((y_train_set,y_train),axis=0)

# print(X_train.shape)
# print(y_train.shape)

# model = keras.Sequential()

# model.add(keras.layers.LSTM(

#     units=64,

#     #input_shape=(X_train.shape[1], X_train.shape[2])
#     input_shape=(X_train_set.shape[1], X_train_set.shape[2])

# ))

# model.add(keras.layers.Dropout(rate=0.2))

# #model.add(keras.layers.RepeatVector(n=X_train.shape[1]))
# model.add(keras.layers.RepeatVector(n=X_train_set.shape[1]))

# model.add(keras.layers.LSTM(units=64, return_sequences=True))

# model.add(keras.layers.Dropout(rate=0.2))

# model.add(

#   keras.layers.TimeDistributed(

#     #keras.layers.Dense(units=X_train.shape[2])
#     keras.layers.Dense(units=X_train_set.shape[2])

#   )

# )

# model.compile(loss='mae', optimizer='adam')

# history = model.fit(

#     #X_train, y_train,
#     X_train_set, y_train_set,

#     epochs=1,

#     batch_size=64,

#     validation_split=0.1,

#     shuffle=False

# )

# path ='myModel_B_py'
# model.save(path)

path ='myModel_B_359'
model = keras.models.load_model(path)

THRESHOLD = num_mae

k=n
for j in sample(range(0, len(df.columns)), k):

    anomalies = 0
    print(j)
    X_test, y_test = create_dataset(

    test.iloc[:,[j]],

    test[df_2[j]],

    TIME_STEPS

    )

    X_train, y_train = create_dataset(

    train.iloc[:,[j]],

    train[df_2[j]],

    TIME_STEPS

    )

    X_train_pred = model.predict(X_train)

    train_mae_loss = np.mean(np.abs(X_train_pred - X_train), axis=1)

    X_test_pred = model.predict(X_test)

    test_mae_loss = np.mean(np.abs(X_test_pred - X_test), axis=1)

    test_score_df = pd.DataFrame(index=test[TIME_STEPS:].index)

    test_score_df['loss'] = test_mae_loss

    test_score_df['threshold'] = THRESHOLD

    test_score_df['anomaly'] = test_score_df.loss > test_score_df.threshold

    test_score_df[df_2[j]] = test[TIME_STEPS:][df_2[j]]

    anomalies = test_score_df[test_score_df.anomaly == True]

    print("Time series:", j)
    plt.plot(
      test[TIME_STEPS:].index, 
    scaler.inverse_transform(test[TIME_STEPS:][df_2[j]]), 
    label='Predicted'
  )

    sns.scatterplot(  anomalies.index,
    scaler.inverse_transform(anomalies[df_2[j]]),
    color=sns.color_palette()[3],
    s=52,
    label='anomaly')

    plt.xticks(rotation=25)
    plt.legend()
    plt.show()
