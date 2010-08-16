#ifndef JNITEST_H
#define JNITEST_H
#include "jni.h"
#define USER_CLASSPATH "./" // where to find Java class
#define JNI_VERSION JNI_VERSION_1_4

jint MyCreateJavaVM(JavaVM** pjvm,JNIEnv** penv);
void jnitest();

#endif