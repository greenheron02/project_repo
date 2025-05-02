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

# Map software inputs to hardware outputs
sw_inputs = "V,B,N,M,G,H,J,K,W,A,S,D"
sw_inputs = sw_inputs.split(",")
hw_inputs = "a,b,c,d,g,h,j,k,U,L,D,R".split(",")
keycodes = list(map(lambda i : ki.letters[i], sw_inputs))
outputs = dict(map(lambda i,j: (i, j) , hw_inputs, keycodes))

# Verify the outputs and whether the script is working
print(outputs)
print('okey')

if __name__ == '__main__': 
    # Get the open COM ports
    ports = serializing.get_ports() 
    # Find the Pico on one of the open ports, if there is no Pico an error will be thrown
    pico_port = serializing.find_pico(ports)
    # Open the serial port
    with serial.Serial(pico_port) as ser:
        while(True):
            # Strip whitespace and extra characters from serial input
            x = ser.readline() # 'b'\rtext\n\t''
            y = x.strip().decode() # 'hld text'
            code = y[3:].strip() # 'text'
            action = y[:-2] # 'hld'
            # If a button is being pressed, 
            if action == 'hld':
                # press the corresponding keyboard key
                ki.PressKey(outputs[code])
            elif action == 'rel':
                # otherwise release the corresponding keyboard key
                ki.ReleaseKey(outputs[code])    
