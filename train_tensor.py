# File name train_tensor.py
# Date of creation: Feb 5, 2020
# Author: Luis Mercado

#Description: From the pickle file, created perviously, create a simple Sequential
# CNN model, using Tensorflow, and Keras.
#
#Functions: create_training_data
# 	resize the images, and store the pixel data, features and labels, into numpy array.





import tensorflow as tf
from tensorflow.keras.datasets import cifar10
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, Activation, Flatten
from tensorflow.keras.layers import Conv2D, MaxPooling2D
from tensorflow.keras.callbacks import TensorBoard
import pickle
import time

# NAME = "Recycle-CNN"
#Open and load the pickle files containg features and labels
pickle_in = open("X13.pickle","rb")
X = pickle.load(pickle_in)

pickle_in = open("y13.pickle","rb")
y = pickle.load(pickle_in)

X = X/255.0 #normalize the feature data to prevent the vanishing gradient

dense_layers = [0,1] #amount of dense layers (output layers)
conv_size = [16,32,64] #first layer has a filter of 16x3, second is 32x3, etc
conv_layers = [1,2,3] #convolution layers which perform feature extraction

for dense_layer in dense_layers:
	for layer_size in conv_size:
		for conv_layer in conv_layers:
			NAME = "{}-conv-{}-nodes-{}dense-{}".format(conv_layer, layer_size, dense_layer, int(time.time()))
			print(NAME) #display which layer it is on during training

			model = Sequential() #Sequential is the most simple model, so that is what we are using

			model.add(Conv2D(layer_size, (3,3), input_shape=X.shape[1:])) #hidden layer which performs convolution and outputs feature map
			model.add(Activation('relu')) #Acitvation functions act like a mathematical "gate" in between input feeding
			model.add(MaxPooling2D(pool_size=(2,2))) #of current neuron and its output to the next layer
			#relu is used because it does not saturate; the gradient is always high if neuron activates
			#relu is also very quickly to evaluate
			#max pooling extracts patches from the input feature maps, outputs the maximum value in each patch, and
			#discards all the other values. It is a 2x2 filter

			for l in range(conv_layer-1): #hidden layer that repeats above layer
				model.add(Conv2D(layer_size, (3,3)))
				model.add(Activation('relu'))
				model.add(MaxPooling2D(pool_size=(2,2)))

			model.add(Flatten()) #this converts our 3D feature maps to 1D feature vectors

			for _ in range(dense_layer): #fully connected layers which every input is connected to every learnable output
				model.add(Dense(layer_size))
				model.add(Activation('relu'))

			model.add(Dense(4)) #final output layer
			model.add(Activation('softmax')) #softmax takes the Dense layer output and converts it to meaningful probabilties
			#for each of the digits, which sum up to 1. It then makes a prediction based on the digit that has the highest
			#probabilties

			model.compile(loss='sparse_categorical_crossentropy', #measures how far the model's prediction is wrong
				optimizer='adam', #controls the learning rate, which defines how fast optimal weights for the model are calculates
				metrics=['accuracy']) #defines how the model's success rate is evaluated

			model.fit(X, y, batch_size=32, epochs=50, validation_split=0.1, callbacks=[tensorboard])
			#batch size determines numhber of samples in each mini batch_size
			#epoch is the amount of times it goes through training set
			#validation_split randomly splits the example set into a training set and test set and evaluates the model

model.save('Recycle13-CNN.model') #saves model
