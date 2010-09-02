@echo off
echo "Close any firefox instance, and press enter to continue"
pause
copy /y ..\debug\*.dll "C:\Program Files\Mozilla Firefox\plugins\"
copy /y ..\debug\*.exe C:\
start firefox "./test.html" -console
