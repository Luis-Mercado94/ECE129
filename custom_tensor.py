# File name custom_tenor.py
# Date of creation: Feb 5, 2020
# Author: Luis Mercado

#Description: To gather all images in dataset directory, and the directoies within
# dataset directory. Convert those images into pixel data array, and resize
# the images to a constant size, and store in numpy array (for both features and labels)
# and store the numpy array in a pickle file, so it can be used in another script
#
#Functions: create_training_data
# 	resize the images, and store the pixel data, features and labels, into numpy array.


import numpy as np
import matplotlib.pyplot as plt
import os
import cv2
from tqdm import tqdm
import random
import pickle

DATADIR = "dataset" #set the directory with all the image dataset

TYPE = ["aluminum", "glass", "plastic", "trash"] #directoies within the dataset directotry


IMG_SIZE = 200 #set an image size

training_data = [] #create training data array

def create_training_data(): #function for creating traning data
	for category in TYPE: #for each category

		path = os.path.join(DATADIR,category) #join paths again
		class_num = TYPE.index(category) #set class numbers based off directories in TYPE

		for img in tqdm(os.listdir(path)): #for each image in dataset, while tqgm is simply a progress bar
			try:
				img_array = cv2.imread(os.path.join(path,img)) #load image
				new_array = cv2.resize(img_array, (IMG_SIZE, IMG_SIZE)) #resize the image
				training_data.append([new_array, class_num]) #append the new image in the dataset
			except Exception as e:
				pass

create_training_data()
print(len(training_data)) #show number of items in traning set

random.shuffle(training_data) #shuffle the training set

for sample in training_data[:10]: #this is to simply show me that the data is shuffled
	print(sample[1])

X = [] #create the feature array
y = [] #create the label array

for features,label in training_data: #append feature and labels to the according array
	X.append(features)
	y.append(label)

print(X[0].reshape(-1, IMG_SIZE, IMG_SIZE, 3))
X = np.array(X).reshape(-1, IMG_SIZE, IMG_SIZE, 3) #convert feature and label array into numpy arrays, and reshape
y = np.array(y) #the arrays, with -1 being a catch all, IMG_SIZE being the image size, and the 3 being for RBG data
#next we save them to pickle files so that we do not need to calculate them again
pickle_out = open("X13.pickle","wb")
pickle.dump(X, pickle_out)
pickle_out.close()

pickle_out = open("y13.pickle","wb")
pickle.dump(y,pickle_out)
pickle_out.close()

pickle_in = open("X13.pickle","rb")
X = pickle.load(pickle_in)

pickle_in = open("y13.pickle","rb")
y = pickle.load(pickle_in)
