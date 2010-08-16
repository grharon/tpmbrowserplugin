copy /y ..\debug\nprt2.dll "C:\Program Files\Mozilla Firefox\plugins\nprt.dll"
echo "To see the debug console, close all existed firefox.exe instances."
"C:\Program Files\Mozilla Firefox\firefox.exe" ".\test.html" -console
