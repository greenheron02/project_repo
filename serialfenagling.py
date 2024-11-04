import serial 
from pynput import keyboard
from time import sleep
ser = serial.Serial('/dev/ttyACM0')
print(ser.name)
print("hamburgers")
counter = 1
keycontrol = keyboard.Controller()
while True: 
    x = ser.readline()
    x = x.decode("utf-8")
    # print(x[:-2] == 'a')
    x = x[:-2]
    if (x == 'a'): 
        keycontrol.press('a')
    elif (x == 'b'):
        keycontrol.press('b')
    print(f"{counter}: {x[:-1]}")
    counter = counter + 1

