# directkeys.py
# http://stackoverflow.com/questions/13564851/generate-keyboard-events
# msdn.microsoft.com/en-us/library/dd375731



import ctypes
from ctypes import wintypes
import time

user32 = ctypes.WinDLL('user32', use_last_error=True)

INPUT_MOUSE    = 0
INPUT_KEYBOARD = 1
INPUT_HARDWARE = 2

KEYEVENTF_EXTENDEDKEY = 0x0001
KEYEVENTF_KEYUP       = 0x0002
KEYEVENTF_UNICODE     = 0x0004
KEYEVENTF_SCANCODE    = 0x0008

MAPVK_VK_TO_VSC = 0

# List of all codes for keys:
# # msdn.microsoft.com/en-us/library/dd375731
# This is a utility list of keycodes for translation
# from string to keycode in the p2p2p2p.py script
LEFT = 0x25
UP = 0x26
RIGHT = 0x27
DOWN = 0x28
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
A = 0x41
B = 0x42
C = 0x43
D = 0x44
E = 0x45
F = 0x46
G = 0x47
H = 0x48
I = 0x49
J = 0x4a
K = 0x4b
L = 0x4c
M = 0x4d
N = 0x4e
O = 0x4f
P = 0x50
Q = 0x51
R = 0x52
S = 0x53
T = 0x54
U = 0x55
V = 0x56
W = 0x57
X = 0x58
Y = 0x59
Z = 0x5a

# C struct definitions

wintypes.ULONG_PTR = wintypes.WPARAM

class MOUSEINPUT(ctypes.Structure):
    _fields_ = (("dx",          wintypes.LONG),
                ("dy",          wintypes.LONG),
                ("mouseData",   wintypes.DWORD),
                ("dwFlags",     wintypes.DWORD),
                ("time",        wintypes.DWORD),
                ("dwExtraInfo", wintypes.ULONG_PTR))

class KEYBDINPUT(ctypes.Structure):
    _fields_ = (("wVk",         wintypes.WORD),
                ("wScan",       wintypes.WORD),
                ("dwFlags",     wintypes.DWORD),
                ("time",        wintypes.DWORD),
                ("dwExtraInfo", wintypes.ULONG_PTR))

    def __init__(self, *args, **kwds):
        super(KEYBDINPUT, self).__init__(*args, **kwds)
        # some programs use the scan code even if KEYEVENTF_SCANCODE
        # isn't set in dwFflags, so attempt to map the correct code.
        if not self.dwFlags & KEYEVENTF_UNICODE:
            self.wScan = user32.MapVirtualKeyExW(self.wVk,
                                                 MAPVK_VK_TO_VSC, 0)

class HARDWAREINPUT(ctypes.Structure):
    _fields_ = (("uMsg",    wintypes.DWORD),
                ("wParamL", wintypes.WORD),
                ("wParamH", wintypes.WORD))

class INPUT(ctypes.Structure):
    class _INPUT(ctypes.Union):
        _fields_ = (("ki", KEYBDINPUT),
                    ("mi", MOUSEINPUT),
                    ("hi", HARDWAREINPUT))
    _anonymous_ = ("_input",)
    _fields_ = (("type",   wintypes.DWORD),
                ("_input", _INPUT))

LPINPUT = ctypes.POINTER(INPUT)

def _check_count(result, func, args):
    if result == 0:
        raise ctypes.WinError(ctypes.get_last_error())
    return args

user32.SendInput.errcheck = _check_count
user32.SendInput.argtypes = (wintypes.UINT, # nInputs
                             LPINPUT,       # pInputs
                             ctypes.c_int)  # cbSize

# Functions

def PressKey(hexKeyCode):
    x = INPUT(type=INPUT_KEYBOARD,
              ki=KEYBDINPUT(wVk=hexKeyCode))
    user32.SendInput(1, ctypes.byref(x), ctypes.sizeof(x))

def ReleaseKey(hexKeyCode):
    x = INPUT(type=INPUT_KEYBOARD,ki=KEYBDINPUT(wVk=hexKeyCode,
                            dwFlags=KEYEVENTF_KEYUP))
    user32.SendInput(1, ctypes.byref(x), ctypes.sizeof(x))

def HoldKey(hexKeyCode):
    PressKey(hexKeyCode)
    time.aasleep(0.01)
    ReleaseKey(hexKeyCode)
    

if __name__ == "__main__":
    PressKey(A)
    time.sleep(0.5)
    ReleaseKey(A)
    print("Pressed")
