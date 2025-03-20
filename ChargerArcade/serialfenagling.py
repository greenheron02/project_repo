import subprocess
import serial
import serial.serialutil
import serial.tools.list_ports
from time import sleep
import keyboardinputting as ki 

ports = serial.tools.list_ports.comports()
for port, desc, hwid in sorted(ports):
    dog = hwid.split()
    if len(dog) > 1:
        dog = dog[2]
        dog = dog[4:]
        if dog == 'E66178C1270A5C29':
           comport = port
        
    print("{}: {} [{}]".format(port, desc, hwid))

if not comport:
    comport = input('Enter a COM port: ')

try: 
    ser = serial.Serial(comport)
    print('hams') 
except serial.serialutil.SerialException:
    print('No USB present idjit') 

    

done = False
## Map interface inputs into keycodes for windows output
sw_inputs = "W,A,S,D,UP,LEFT,RIGHT,DOWN"
sw_inputs = sw_inputs.split(",")
hw_inputs = "a,b,c,d,U,L,R,D".split(",")
keycodes = list(map(lambda i : ki.letters[i], sw_inputs))
outputs = dict(map(lambda i,j: (i, j) , hw_inputs, keycodes))
print(outputs)
print('okey')
while not done:
    data = ser.readline()
    data = data.decode()
    data = data[:-2]
    action = data[:-2]
    code = data[4:]
    if action == 'hld':
        print(code + 'is being held to ' + str(outputs[code]))
        ki.PressKey(outputs[code])
    if action == 'rel':
       print(code + 'is being released from' + str(outputs[code]))
       ki.ReleaseKey(outputs[code])

    ## print(action + ' ' + str(code))    
    


##ke_ys = {
##        'up':'0xff52', 
##        'down':'0xff54', 
##        'left':'0xff51',
##        'right':'0xff53',
##        'a':'0x0061', 
##        'b':'0x0062', 
##        'w':'0x0077',
##        's':'0x0073',
##        'd':'0x0064'
##    }

##done = False 
##
##while not done: 
##    x = ser.readline()
##    x = x.decode()
##    x = x[:-2] 
##    
##    keys_input = {"U":'up', "D":'down', 
##             "L":'left', "R":'right',
##             "a":'a', "b":'w',
##             "c":'d', "d":'s'}
##    cmd = x[:-2] 
##    x = x[4:]
##    print(f"type: {cmd}, x: {x}")
##    # print(len(cmd))


    # if x in keys_input.keys(): 
    #     x = keys_input[x]
    #     k = ke_ys[x]
    #     print(f"pressed key: {k}")
    #     if (cmd == "hld"): 
    #         touch_key(k)
    #     if (cmd == "rel"): 
    #         release_key(k)
