#Requires AutoHotkey v2.0
#SingleInstance Force
esc::ExitApp

~a & ~s::
{
	while (GetKeyState("a", "p") && GetKeyState("s", "p"))
	{
		sleep 500
		if(GetKeyState("w", "p"))
		{	
			;ControlSend "E", "Edit1", "ChargerArcade"
			ToolTip "EEEE"
			return
		}
		else
		{
			ToolTip "No"
		}
	}
}