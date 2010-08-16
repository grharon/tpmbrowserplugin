#include "jnitest.h"
#include <windows.h>
typedef JNIIMPORT jint (JNICALL *P_CreateJavaVM) (JavaVM **pvm, void **penv, JavaVMInitArgs *args);
typedef JNIIMPORT jint (JNICALL *P_GetCreatedJavaVMs) (JavaVM **pvm, jsize, jsize*);

char* getJVMPath() {
  HKEY hKey;
  LONG r = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\JavaSoft\\Java Runtime Environment", NULL,KEY_READ,&hKey);
  if (r != ERROR_SUCCESS) {
    return NULL;
  }
  char jvmPath[512];
  DWORD cbr = sizeof(jvmPath);
  DWORD type;
  r = RegQueryValueEx(hKey, "CurrentVersion",NULL,&type,(LPBYTE)jvmPath,&cbr);
  if (r != ERROR_SUCCESS) {
    RegCloseKey(hKey);
    return NULL;
  }
  jvmPath[cbr] = NULL;
  HKEY hKey2;
  r = RegOpenKeyEx(hKey,jvmPath,NULL,KEY_READ,&hKey2);
  if (r != ERROR_SUCCESS) {
    RegCloseKey(hKey);
    return NULL;
  }
  cbr = sizeof(jvmPath);
  r = RegQueryValueEx(hKey2, "RuntimeLib",NULL,&type,(LPBYTE)jvmPath,&cbr);
  RegCloseKey(hKey);
  RegCloseKey(hKey2);
  if (r != ERROR_SUCCESS)
    return NULL;
  char* c = new char[strlen(jvmPath)+1];
  strcpy(c,jvmPath);
  c[strlen(jvmPath)] = NULL;
  return c;
}

jint MyCreateJavaVM(JavaVM** pjvm,JNIEnv** penv) {
  JavaVM* jvm=NULL;
  JNIEnv* env=NULL;
  JavaVMInitArgs vm_args;
  JavaVMOption options[1];
  options[0].optionString ="-Djava.class.path="USER_CLASSPATH;
  ZeroMemory(&vm_args,sizeof(JavaVMInitArgs));
  vm_args.version = JNI_VERSION;
  vm_args.nOptions = 1;
  vm_args.ignoreUnrecognized = JNI_TRUE;
  vm_args.options = options;

  char *jvmPath = getJVMPath();
  printf("jvmPath = %s\n",jvmPath);
  //char jvmPath[100] = "C:\\Program Files\\Java\\jdk1.6.0_21\\jre\\bin\\client\\jvm.dll";
  if (jvmPath == NULL) {
    return -1;
  }

  HINSTANCE hVM = LoadLibrary(jvmPath);
  if (hVM == NULL) {
    return NULL;
  }
  void *addr_createjvm =GetProcAddress(hVM, "JNI_CreateJavaVM");
  void *addr_getjvms = GetProcAddress(hVM, "JNI_GetCreatedJavaVMs");
  if ((addr_createjvm == NULL) || (addr_getjvms == NULL)) {
    return -1;
  }
  P_CreateJavaVM procJvm = (P_CreateJavaVM) addr_createjvm;
  P_GetCreatedJavaVMs getJvm = (P_GetCreatedJavaVMs) addr_getjvms;

  jsize jvm_count = 0;
  jint res = 0;
  res = getJvm(&jvm, 1, &jvm_count);
  printf("res = %d, jvm = %x, jvm_count = %d\n",res,jvm,jvm_count);
  if (jvm_count == 0) {
    res = procJvm(&jvm,(void**)&env,&vm_args);
  }
  else {
    res = jvm->AttachCurrentThread((void **)&env, &vm_args);
  }
  if (res < 0)
    return res;
  if (pjvm != NULL)
    *pjvm = jvm;
  if (penv != NULL)
    *penv = env;
  return 0;
}

void jnitest() {
  JNIEnv *env;
  JavaVM *jvm;
  jint res;
  jclass cls;
  jmethodID mid[5];
  jstring jstr;
  jclass stringClass;
  jobjectArray args;

  // Create Java VM
  res = MyCreateJavaVM(&jvm, &env);
  if (res < 0) {
    fprintf(stderr, "Can't create Java VM\n");
    exit(1);
  }
	printf("version = %x\n",env->GetVersion());
	// Find class
  cls = env->FindClass("Prog");
  if (cls == NULL) {
    goto destroy;
  }
	// find methodID
  mid[0] = env->GetStaticMethodID(cls, "main","([Ljava/lang/String;)V");
  mid[1] = env->GetMethodID(cls,"<init>","()V");
  mid[2] = env->GetMethodID(cls,"test","()V");
  mid[3] = env->GetMethodID(cls,"getS","()Ljava/lang/String;");
  mid[4] = env->GetMethodID(cls,"setS","(Ljava/lang/String;)V");
	for (int i=0;i<5;i++) {
		if (mid[i] == NULL)
	    goto destroy;
  }

	// create java.lang.String
	jstring arg1 = env->NewStringUTF(" TEST ");
	if (arg1 == NULL) {
		goto destroy;
	}
	// call Prog() -> obj
  jobject obj = env->NewObject(cls,mid[1]);
	// call obj.test()
  jobject obj2 = env->CallObjectMethod(obj,mid[2]);
	// call obj.setS(arg1)
  env->CallObjectMethod(obj,mid[4],arg1);
	// call obj.test() again
  env->CallObjectMethod(obj,mid[2]);
	// call obj.getS() -> str3
  jstring str3 = (jstring) env->CallObjectMethod(obj,mid[3]);
	// convert jstring -> char*
  jboolean bool1 = JNI_TRUE;
  char* ch = (char*) env->GetStringUTFChars(str3,&bool1);
  printf("value from getS is: %s\n",ch);
  env->ReleaseStringUTFChars(str3,ch);

	// create java.lang.String
  jstr = env->NewStringUTF(" from C!");
  if (jstr == NULL) {
    goto destroy;
  }
	// FindClass
  stringClass = env->FindClass("java/lang/String");
	// create Array (jstr,) -> args
  args = env->NewObjectArray(1, stringClass, jstr);
  if (args == NULL) {
    goto destroy;
  }
	// call Prog.main(args)
  env->CallStaticVoidMethod(cls, mid[0], args);
destroy:
	// detect exceptions
  if (env->ExceptionOccurred()) {
    env->ExceptionDescribe();
  }
	// detach thread => errors?
	//jvm->DetachCurrentThread();
}