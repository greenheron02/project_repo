import subprocess
import serial
import serial.serialutil 

try: 
    ser = serial.Serial('/dev/ttyACM0')
except serial.serialutil.SerialException:
    print('No USB present idjit') 


def press_key(key): 
    subprocess.run(["xdotool","key",key])

def touch_key(key): 
    subprocess.run(["xdotool","keydown",key])

def release_key(key): 
    subprocess.run(["xdotool","keyup",key])

    


ke_ys = {
        'up':'0xff52', 
        'down':'0xff54', 
        'left':'0xff51',
        'right':'0xff53',
        'a':'0x0061', 
        'b':'0x0062', 
        'w':'0x0077',
        's':'0x0073',
        'd':'0x0064'
    }

done = False 

while not done: 
    x = ser.readline()
    x = x.decode()
    x = x[:-2] 
    
    keys_input = {"U":'up', "D":'down', 
             "L":'left', "R":'right',
             "a":'a', "b":'w',
             "c":'d', "d":'s'}
    cmd = x[:-2] 
    x = x[4:]
    print(f"type: {cmd}, x: {x}")
    # print(len(cmd))


    # if x in keys_input.keys(): 
    #     x = keys_input[x]
    #     k = ke_ys[x]
    #     print(f"pressed key: {k}")
    #     if (cmd == "hld"): 
    #         touch_key(k)
    #     if (cmd == "rel"): 
    #         release_key(k)
