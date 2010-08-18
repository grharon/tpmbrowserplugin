TPMSignApplet is used to call TPM signature API in client side.
The TPMSignApplet.jar needs to be signed with a certificate on server side.

Do the following if you change the TPMSignApplet source:
1. Generate a keystore if you do not have it. The following uses jdk tool to
   generate a keystore whose password is 123456
   keytool -genkey -keystore hello.store -alias hello
   
2. Sign:
   jarsigner -keystore hello.store -storepass 123456 -keypass 123456 -signedjar DigitalSignApplet.jar TPMSignApplet.jar hello
   
3. copy the DigitalSignApplet.jar to the directory where the server jsp locates.