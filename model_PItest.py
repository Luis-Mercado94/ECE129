# File name model_test.py
# Date of creation: May 16, 2020
# Author: Luis Mercado

#Description: The main python script running the CV model. It simply takes a
#picture, using the camera on the raspberry pi, and sending that image to the
# cv function to be classified/

from cv_function import RunCV
from picamera import PiCamera
from time import sleep

camera = PiCamera() #turning on the camera
#camera.rotation=180 #flip it due to it being upside down with me
camera.start_preview()
time.sleep(1) #warm-up time for camera
camera.capture('test.jpg') #taking and saving picture

RunCV('test.jpg') #Running cv function with newly taken picture
