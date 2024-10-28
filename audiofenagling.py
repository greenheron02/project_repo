from __future__ import print_function 
from pycaw.pycaw import AudioUtilities, ISimpleAudioVolume 
from pynput import keyboard

def main():
    changing_volume = 0
    volval = 0
    sessions = AudioUtilities.GetAllSessions()
    for session in sessions:
        volume = session._ctl.QueryInterface(ISimpleAudioVolume)
        if session.Process and session.Process.name() == "opera.exe":
            changing_volume = volume
            volval = volume.GetMasterVolume() 
            print("Changing Volume of " + str(session.Process.name()))
            print("Current Volume " + str(volume.GetMasterVolume()))
    with keyboard.Events() as events: 
        for event in events: 
            if type(event) == keyboard.Events.Release: 
                if event.key == keyboard.Key.esc: 
                    break 
                elif event.key == keyboard.Key.up: 
                    volval += 0.1
                    changing_volume.SetMasterVolume(volval, None)
                    print("Current Volume " + str(volval))
                elif event.key == keyboard.Key.left: 
                    volval = 0.6
                    changing_volume.SetMasterVolume(volval, None)
                    print("Current Volume " + str(volval)) 
                elif event.key == keyboard.Key.down: 
                    volval -= 0.1 
                    changing_volume.SetMasterVolume(volval, None)
                    print("Current Volume " + str(volval)) 

    
            
if __name__ == "__main__":
    main()
