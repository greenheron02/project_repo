import sys # stolen from https://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
import glob
import serial
import serial.tools.list_ports



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
            print(s.name) 
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


if __name__ == '__main__':
##    print(serial_ports())
    jorts = serial.tools.list_ports.comports()
    for port, desc, hwid in sorted(jorts):
        dog = hwid.split()
        if len(dog) > 1:
            dog = dog[2]
            dog = dog[4:]
            print(dog)
            if dog == 'E66178C1270A5C29':
                print(portnys,mtyd,yu,dymdyuyutdmyu)
        
        print("{}: {} [{}]".format(port, desc, hwid))
