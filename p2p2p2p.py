import serial
import keyboardinputting as ki
import serializing 
import time 

# This script reads the button/joystick inputs sent by the Raspberry Pi Pico
# 
# The hw_inputs string contains the inputs sent from the Pico 
#
# The sw_inputs string contains the keyboard outputs for the computer
# 
# This script maps the hardware inputs to software outputs and sends keycodes to the 
# computer using the ki.PressKey function


sw_inputs = "V,B,N,M,G,H,J,K,W,A,S,D"
sw_inputs = sw_inputs.split(",")
hw_inputs = "a,b,c,d,g,h,j,k,U,L,D,R".split(",")
keycodes = list(map(lambda i : ki.letters[i], sw_inputs))
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
                # print(code + " " + action) 
                ki.PressKey(outputs[code])
            elif action == 'rel':
                # print(code + " " + action)
                ki.ReleaseKey(outputs[code])    
