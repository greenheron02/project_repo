import serial
# import keyboardinputting as ki
import serializing 
import time 

# "W,A,S,D,UP,LEFT,RIGHT,DOWN"

letters ={ "LEFT" : 0x25,
    "UP"    : 0x26,
    "RIGHT" : 0x27,
    "DOWN"  : 0x28,
    "A" : 0x41,
    "B" : 0x42,
    "C" : 0x43,
    "D" : 0x44,
    "E" : 0x45,
    "F" : 0x46,
    "G" : 0x47,
    "H" : 0x48,
    "I" : 0x49,
    "J" : 0x4a,
    "K" : 0x4b,
    "L" : 0x4c,
    "M" : 0x4d,
    "N" : 0x4e,
    "O" : 0x4f,
    "P" : 0x50,
    "Q" : 0x51,
    "R" : 0x52,
    "S" : 0x53,
    "T" : 0x54,
    "U" : 0x55,
    "V" : 0x56,
    "W" : 0x57,
    "X" : 0x58,
    "Y" : 0x59,
    "Z" : 0x5a}

sw_inputs = "V,B,N,M,G,H,J,K,W,A,S,D"
sw_inputs = sw_inputs.split(",")
hw_inputs = "a,b,c,d,g,h,j,k,U,L,D,R".split(",")
keycodes = list(map(lambda i : letters[i], sw_inputs))
outputs = dict(map(lambda i,j: (i, j) , hw_inputs, keycodes))
print(outputs)
print('okey')
hld_flag = 0
if __name__ == '__main__': 
    ports = serializing.get_ports() 
    pico_port = serializing.find_pico(ports)
    with serial.Serial(pico_port) as ser:
        while(True):
            x = ser.readline() # 'b'\rtext\n\t''
            y = x.strip().decode() # 'hld text'
            code = y[3:].strip() # 'text'
            action = y[:-2] # 'hld'
            if action == 'hld':
                print(code + " " + action) 
                # ki.PressKey(outputs[code])
            elif action == 'rel':
                print(code + " " + action)
                # ki.ReleaseKey(outputs[code])    
