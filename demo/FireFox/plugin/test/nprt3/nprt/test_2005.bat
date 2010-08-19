copy /y ..\Debug\nprt2_2005.dll "C:\Program Files\Mozilla Firefox\plugins\nprt.dll"
echo "To see the debug console, close all existed firefox.exe instances."
"C:\Program Files\Mozilla Firefox\firefox.exe" "https://10.239.47.82:8443/splat/identityservice/home?idp=idp1" -console
