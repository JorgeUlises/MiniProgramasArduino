#!/usr/bin/python
import serial
from flask import Flask
#ser = serial.Serial('/dev/ttyAMA0', baudrate=115200, databits=8, timeout=20, xonxoff=False, rtscts=False, dsrdtr=False)
#sleep(1)
#ser = serial.Serial('/dev/ttyACM0', 115200)
ser = serial.Serial('/dev/ttyACM0', 9600)

try:
    while True:
        #bytesToRead = ser.inWaiting()
        response = ser.readline()#bytesToRead)
        ser.flush()
        if len(response)>0:
            #print len(response)
            #a=list(response)[0]
            #a=format(ord(response), 'b')
            #print a
            print response
except KeyboardInterrupt:
    ser.close()
