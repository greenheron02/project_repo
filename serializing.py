import sys 
import glob 
import serial 
import serial.serialutil
import serial.tools.list_ports

# This script contains a series of helper functions for grabbing where the Raspberry Pi Pico is 

def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

# Lists each type of info given by an accessed COM port
def serial_ports_info(ports): 
    ports = serial.tools.list_ports.comports() 
    for port in sorted(ports): 
        print(port.description)
        print(port.device)
        print(port.device_path)
        print(port.hwid)
        print(port.interface)
        print(port.location)
        print(port.manufacturer)
        print(port.name)
        print(port.pid)
        print(port.product)

# Finds a raspberry pi pico among listed COM ports
def find_pico(ports): 
    for port in sorted(ports):
        if port.product == 'Pico': 
            return port.device

# Gets comports of computer and devices attached
def get_ports(): 
    return serial.tools.list_ports.comports()


# if __name__ == '__main__':
#     device_path = 0
#     print(serial_ports())
#     ports = serial.tools.list_ports.comports() 
#     # serial_ports_info(ports) 
#     device_path = find_pico(ports)
            
#     with serial.Serial(device_path) as ser: 
#         print(ser)
    
    

           

