@echo off
echo "write your defined java classpath to %HOME%/tpm_plugin.conf"
echo "Close any firefox instance, and press enter to continue"
pause
copy /y ..\Debug\*.dll "C:\Program Files\Mozilla Firefox\plugins\"
copy /y ..\Debug\*.exe "C:\Program Files\Mozilla Firefox\plugins\"

start firefox "./test.html" -console
rem "https://localhost:8443/splat/identityservice/home?idp=idp1"
