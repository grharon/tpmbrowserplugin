#ifndef JNITEST_H
#define JNITEST_H

int jnitest();
int loadJVM();
void unloadJVM();
char* jni_doSignature(char* randomString, char* tpmPass);
char* jni_getPublicKeyContent();

#endif