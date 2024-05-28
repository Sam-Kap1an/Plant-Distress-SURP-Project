import numpy as np
from sklearn.utils import class_weight
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Activation, Flatten, Convolution1D, Dropout, MaxPooling1D


#%%
def train_model(X_train, Y_train, vb=0):

    ### Hyper parameters ###
    batch_size = 64
    num_epochs = 50
    kernel_size = 9
    pool_size = 4
    conv_depth_1 = 32
    conv_depth_2 = 64
    conv_depth_3 = 128
    drop_prob_1 = 0.5
    drop_prob_2 = 0.5
    drop_prob_3 = 0.5
    hidden_size = 128

    ### generating the network ###
    model = Sequential()

    # 1st conv block #
    model.add(Convolution1D(conv_depth_1, kernel_size = kernel_size, input_shape=(1001, 1), \
                            padding='same', activation='relu'))
    model.add(Convolution1D(conv_depth_1, kernel_size = kernel_size, \
                            padding='same', activation='relu'))
    model.add(MaxPooling1D(pool_size = pool_size))
    model.add(Dropout(drop_prob_1))

    # 2nd conv block #
    model.add(Convolution1D(conv_depth_2, kernel_size=kernel_size, \
                            padding='same', activation='relu'))
    model.add(Convolution1D(conv_depth_2, kernel_size=kernel_size, \
                            padding='same', activation='relu'))
    model.add(MaxPooling1D(pool_size = pool_size))
    model.add(Dropout(drop_prob_1))

    # 3rd conv block #
    if conv_depth_3 > 0:
        model.add(Convolution1D(conv_depth_3, kernel_size=kernel_size, \
                                padding='same', activation='relu'))
        model.add(Convolution1D(conv_depth_3, kernel_size=kernel_size, \
                                padding='same', activation='relu'))
        model.add(MaxPooling1D(pool_size = pool_size))
        model.add(Dropout(drop_prob_3))

    # Dense layers #
    model.add(Flatten())
    model.add(Dense(hidden_size, activation='relu'))
    model.add(Dropout(drop_prob_2))
    model.add(Dense(1, activation='sigmoid'))

    # Complie #
    model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['binary_accuracy'])

    # Fit #
    class_weights = class_weight.compute_class_weight('balanced', classes=np.unique(Y_train), y=Y_train)
    model.fit(X_train, Y_train, epochs = num_epochs, \
              batch_size = batch_size, verbose=vb, \
              class_weight={0: class_weights[0], 1:class_weights[1]})

    return model