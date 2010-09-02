#ifndef JNITEST_H
#define JNITEST_H

char* jni_doSignature(bool& fail,char* randomString, char* tpmPass);
char* jni_getPublicKeyContent(bool& fail);
int loadJVM(const char* _classpath);
void destroyJVM();

#endif