@echo off
echo "Close any firefox instance, and press enter to continue"
pause
copy /y ..\debug\nprt2.dll "C:\Program Files\Mozilla Firefox\plugins\nprt.dll"
start firefox "./test.html" -console
