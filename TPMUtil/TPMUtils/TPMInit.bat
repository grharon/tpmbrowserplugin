@echo off

:: check the lib directory exist or not
if not exist lib (
    echo "lib directory does not exist, please make sure to run this bat in TPMUtils project root home"
    goto End
)

if "%1"=="" (
    echo "please input two parameters, one is OS name like windows, vista or windows7, the other is TPM key password."
    goto End
)

if "%2"=="" (
    echo "please input two parameters, one is OS name like windows, vista or windows7, the other is TPM key password."
    goto End
)

:: build tpmutil.jar and initialize the context
call ant jar

set CLASSPATH=.;"%USERPROFILE%"\bcprov-jdk15-131.jar;"%USERPROFILE%"\tpmutils.jar

call java -classpath %CLASSPATH% -Djava.library.path="%USERPROFILE%" com.intel.splat.identityservice.tpm.TPMInitKey %1 %2

:End
