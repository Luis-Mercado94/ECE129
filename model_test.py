import cv2
import tensorflow as tf
from Glass_model_test import Glass_test
from Aluminum_model_test import Aluminum_test
from Plastic_model_test import Plastic_test
from Trash_model_test import Trash_test

#CATEGORIES = ["aluminum", "glass", "plastic"]
print('Now conducting CV model automated test framework')
Glass_test()
print('\nFinished Glass\n')
print('Now conducting Aluminum test\n')
Aluminum_test()
print('Finished Aluminum\n')
print('Now conducting Plastic test\n')
Plastic_test()
print('Finished Plastic\n')
print('Now conductin Trash test\n')
Trash_test()
print('Finished Trash\n')
print("Framework complete")
