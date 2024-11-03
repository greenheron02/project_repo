import serial  
from pynput import keyboard 

portString = "COM5"
with serial.Serial(portString) as ser:
    x = ser.readline() 
    x = x.replace('\b\r\t\n','')
    if x.__len__() > 1: 
        x.strip()
    keycontrol = keyboard.Controller('/r/t/n')
    inputlist = ['a','b','x','y']
    if x == inputlist[0]: 
        keycontrol.press(inputlist[0])
    elif x == inputlist[1]: 
        keycontrol.press(inputlist[1])
    elif x == inputlist[2]: 
        keycontrol.press(inputlist[2])
    if x == inputlist[3]: 
        keycontrol.press(inputlist[3])

        
