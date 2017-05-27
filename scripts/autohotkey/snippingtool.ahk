#NoTrayIcon
#SingleInstance force
Run %windir%\system32\SnippingTool.exe, , , NewPID
WinWait, ahk_class Microsoft-Windows-Tablet-SnipperToolbar
WinActivate, ahk_class Microsoft-Windows-Tablet-SnipperToolbar