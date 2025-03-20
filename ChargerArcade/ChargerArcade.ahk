#NoEnv
SetTitleMatchMode, 2

; Define a function that resizes the window
ResizeWindow(windowTitle, width, height) {
    IfWinExist, %windowTitle%
    {
        WinActivate
        WinMove, , , , , %width%, %height%
    }
}

; Listen for a specific hotkey or command-line arguments
; Example: Trigger with command-line parameters
if (A_Args.Count() >= 3) {
    ResizeWindow(A_Args[1], A_Args[2], A_Args[3])
} else {
    MsgBox, Insufficient arguments provided!
}
ExitApp
