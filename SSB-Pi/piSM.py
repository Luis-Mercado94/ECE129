# pi_sm.py

import serial
import time
from ssb_pi import SSB
from ssb_object import SSBObj
import dbInterface

# State machine & SSB object are instantiated
ssbobj = SSBObj()
ssb = SSB(ssbobj)

s1 = serial.Serial(ssbobj.port,ssbobj.rate)
s1.flushInput()
s1.flushOutput()

while True:
#     n = input("Command:")
    if s1.in_waiting > 0:
        ssb.received = s1.readline().decode('utf-8').rstrip()
        print(ssb.received)        
        ssb.received = ssb.received.split(" ")
        ssb.command = ssb.received[0]
        print(ssb.command)
        
#     print(realtime)
#     print(realtime.encode())
#     print("%s"%realtime.encode())
        ssb.on_event(ssbobj)
        ssb.on_event(ssbobj)
#         s1.write(b"ENTERING CV")