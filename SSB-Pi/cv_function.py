# import RPi.GPIO as GPIO
import cv2
import tensorflow as tf
import keras

def RunCV(filepath):
# 	GPIO.setmode(GPIO.BCM) # setup BCM GPIO numbering
# 	GPIO.setup(23,GPIO.out)
# 	GPIO.setup(24.GPIO.out) #set both GPIO 23 and 24 as output pins
	def prepare(filepath):
		IMG_SIZE = 200
		img_array = cv2.imread(filepath) #,cv2.IMREAD_GRAYSCALE)
		new_array = cv2.resize(img_array,(IMG_SIZE,IMG_SIZE))
		return new_array.reshape(-1,IMG_SIZE,IMG_SIZE,3)

	model = tf.keras.models.load_model("Recycle13-CNN.model")

	prediction = model.predict([prepare(filepath)]) #this will give prediction results for each category
	print(prediction)
	index = model.predict_classes(prepare(filepath)) #this will give which cateogry has the highest probabilty
	if index[0] == 0: #if else loop comparing which category has the highest probabilty
		x = "Aluminum"
# 		GPIO.out(23,GPIO.HIGH) #set GPIO 23 as high
	elif index[0] == 1:
                x = "Glass"
# 		GPIO.out(24,GPIO.HIGH) # set GPIO 24 as high
	elif index[0] == 2:
                x = "Plastic"
# 		GPIO.out(23,GPIO.HIGH) # set both GPIO 23 and 24 as high
# 		GPIO.out(24,GPIO.HIGH)
	else:
                x = "Waste"
# 		GPIO.out(23,GPIO.LOW) #set both GPIO 23 and 24 as low
# 		GPIO.out(24,GPIO.LOW)

# 	GPIO.cleanup() #cleanly exit GPIO programs
	return x
