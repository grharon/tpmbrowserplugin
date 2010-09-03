@echo off
echo "Close any firefox instance, and press enter to continue"
pause
copy /y ..\Debug\*.dll "C:\Program Files\Mozilla Firefox\plugins\"
copy /y ..\Debug\*.exe C:\
start firefox "./test.html" -console
