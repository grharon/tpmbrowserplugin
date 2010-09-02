pause
copy /y ..\Debug\*.dll "C:\Program Files\Mozilla Firefox\plugins\"
copy /y ..\Debug\*.exe "C:\Program Files\Mozilla Firefox\plugins\"

echo "To see the debug console, close all existed firefox.exe instances."
"C:\Program Files\Mozilla Firefox\firefox.exe" "https://10.239.47.82:8443/splat/identityservice/home?idp=idp1" -console
