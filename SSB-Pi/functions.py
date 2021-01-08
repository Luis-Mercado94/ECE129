# functions.py

import smtplib, ssl
import time
import pygame
import dbInterface
import random

def emailGS():
    port = 465  # For SSL
    smtp_server = "smtp.gmail.com"
    sender_email = "smartslugbinsc@gmail.com"  # Enter your address
    receiver_email = "george.r.leece@gmail.com"  # Enter receiver address
    password = "Uc$c20$1ug\n"
    message = """\
    Subject: Smart Slug Bin

    The Smart Slug Bin has identified a full bin."""


    context = ssl.create_default_context()
    with smtplib.SMTP_SSL(smtp_server, port, context=context) as server:
        server.login(sender_email, password)
        server.sendmail(sender_email, receiver_email, message)
    return True
        
def runDisplay(obj):
    if obj.itemType == "Trash":
        pic = pygame.image.load("trash.png")
        pygame.display.set_mode(pic.get_size())
        main_surface = pygame.display.get_surface()
        main_surface.blit(pic, (0,0))
        pygame.display.update()
    elif obj.itemContamination == "Dirty":
        pic = pygame.image.load("dirty.jpeg")
        pygame.display.set_mode(pic.get_size())
        main_surface = pygame.display.get_surface()
        main_surface.blit(pic, (0,0))
        pygame.display.update()
    else:
        pic = pygame.image.load("recyclable.jpeg")
        pygame.display.set_mode(pic.get_size())
        main_surface = pygame.display.get_surface()
        main_surface.blit(pic, (0,0))
        pygame.display.update()
        
def runCV():
    # Camera takes a picture and saves the photo to a database
    # Saved image is then inserted into the CNN model
    # type is saved with the image & returned to SM
    cvType = "Trash";
    return cvType

def randomCV():
    type = random.randint(1,4)
    if type == 1:
        return 'Aluminum'
    elif type == 2:
        return 'Plastic'
    elif type == 3:
        return 'Glass'
    else:
        return 'Trash'

def randomWeight():
    return random.randrange(1,100)

def contamination(obj):
    if(obj.itemType == "Aluminum" and obj.itemWeight < 25):
        return "Clean"
    elif(obj.itemType == "Glass" and obj.itemWeight < 75):
        return "Clean"
    elif(obj.itemType == "Plastic" and obj.itemWeight < 50):
        return "Clean"
    else:
        return "Dirty"
        
        
def checkBC(cvType, bcType):
    if(cvType == bcType):
        return True
    else:
        return False
    
# def checkBCDB(obj): # Merlin will code? REPLACED WITH read_entry in dbInterface.py
#     print(obj.received)
#     # Input: Barcode | Output: True/False
#     db.Interface.read_entry(c, conn, obj)
    
# def obtainBCDBtype(barcode): # Merlin will code? # REPLACED WITH read_entry in dbInterface.py
#     # Input: Barcode | Output: The type (Aluminum, Glass, Plastic, or Trash) 
#     return input("bcType:")
# 
# def obtainBCDBweight(barcode):# Merlin will code? # REPLACED WITH read_entry in dbInterface.py
#     # Input: Barcode | Output: The empty weight
#     return input("bcWeight:")

def addBCtoDB(barcode): # Merlin will code?
    # Input: Barcode | Output: newBC == True
    newBC = True
    return True
    # Shouldn't need as Arduino will control display screen's power in order to
    # conserve energy. The display will update before being turned on.
    # def closeDisplay(time):       
    #     if time=="TIMEOUT":
    #         pygame.display.quit()
