#include "jnitest.h"
#include <jni.h>
#include <stdlib.h>

#ifdef _WINDOWS
#include <windows.h>

static HINSTANCE loadJVMLibrary(void);
typedef jint (JNICALL *CreateJavaVM_t) (JavaVM **pvm, void **penv, JavaVMInitArgs *args);
typedef jint (JNICALL *GetCreatedJavaVMs_t) (JavaVM **pvm, jsize, jsize*);
#endif

static JNIEnv* env_a = NULL;

int loadJVM() {
	JavaVM* jvm = NULL;
  JavaVMInitArgs vm_args;
  jint status;
  const int OPTNUM = 3;
  JavaVMOption options[OPTNUM];
#ifdef _WINDOWS
  CreateJavaVM_t createJavaVM;
  GetCreatedJavaVMs_t getCreatedJavaVMs;
#endif
  options[0].optionString ="-verbose:jni";
  options[1].optionString ="-Djava.compiler=NONE";
	options[2].optionString ="-Djava.class.path=./;D:/TPM/TPMUtil/TPMUtils/bin/;D:/TPM/TPMUtil/TPMUtils/lib/";
  memset(&vm_args,0,sizeof(vm_args));
  vm_args.version = JNI_VERSION_1_2;
  vm_args.nOptions = OPTNUM;
  vm_args.ignoreUnrecognized = JNI_TRUE;
  vm_args.options = options;

  JavaVM *jvms[4];
  jsize jvm_count = 0;
#ifdef _WINDOWS
  HINSTANCE hjvmlib = loadJVMLibrary();
  createJavaVM = (CreateJavaVM_t)GetProcAddress(hjvmlib, "JNI_CreateJavaVM");
  getCreatedJavaVMs = (GetCreatedJavaVMs_t) GetProcAddress(hjvmlib, "JNI_GetCreatedJavaVMs");
#endif

#ifdef _WINDOWS
  status = getCreatedJavaVMs(jvms, 4, &jvm_count);
#else
  status = JNI_GetCreatedJavaVMs(jvms, 4, &jvm_count);
#endif
  fprintf(stderr,"status = %d, jvm_count = %d\n",status,jvm_count);
  if (jvm_count > 0) {
    jvm = jvms[0];
    jvm->AttachCurrentThread((void**)&env_a, &vm_args);
  }
  else {
#ifdef _WINDOWS
    status = (*createJavaVM) (&jvm, (void**) &env_a, &vm_args);
#else
    status = JNI_CreateJavaVM(&jvm, (void**) &env_a, &vm_args);
#endif
  }
	if (status != JNI_OK) {
    fprintf(stderr,"Error creating VM\n");
  }
	fprintf(stderr,"jvm = %x\n",jvm);
	return status;
}

void unloadJVM() {
  // destroy JVM, but it really does no help
	/*
  jint status = jvm->DestroyJavaVM();
  if (status) {
    fprintf(stderr, "Can't destroy Java VM\n");
  }
	*/
}

#ifdef _WINDOWS

static int GetStringFromRegistry(HKEY key, const char*name, char*buf, jint bufsize) {
  DWORD type,size;
  return RegQueryValueEx(key, name, 0, &type, 0, &size)==0
      && type == REG_SZ
      && size<(unsigned int) bufsize
      && RegQueryValueEx(key, name, 0, 0, (LPBYTE)buf, &size) == 0;
}

static void GetPublicJREHome(char *buf, jint bufsize) {
  HKEY key, subkey;
  char version[MAX_PATH];
  char *JRE_KEY = "SOFTWARE\\JavaSoft\\Java Runtime Environment";
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, JRE_KEY, 0, KEY_READ, &key) != 0) {
    fprintf(stderr, "Error opening registry key '%s'\n",JRE_KEY);
    exit(1);
  }
  if (!GetStringFromRegistry(key,"CurrentVersion",version,sizeof(version))) {
    fprintf(stderr, "Failed reading value of registry key:\n\t%s\\CurrentVersion\n",JRE_KEY);
    RegCloseKey(key);
    exit(1);
  }
  /* Find directory where the current version is installed. */
  if (RegOpenKeyEx(key, version, 0, KEY_READ, &subkey) != 0) {
    fprintf(stderr, "Error opening registry key '%s\\%s'\n", JRE_KEY, version);
    exit(1);
  }
  if (!GetStringFromRegistry(subkey,"JavaHome",buf,bufsize)) {
    fprintf(stderr, "Failed reading value of registry key:\n\t%s\\%s\\JavaHome\n", JRE_KEY, version);
    RegCloseKey(key);
    RegCloseKey(subkey);
    exit(1);
  }
  RegCloseKey(key);
  RegCloseKey(subkey);
}

static HINSTANCE loadJVMLibrary(void) {
  HINSTANCE h1, h2;
  char msvcdll[MAX_PATH];
  char javadll[MAX_PATH];
  GetPublicJREHome(msvcdll, MAX_PATH);
  strcpy(javadll, msvcdll);
  strncat(msvcdll, "\\bin\\msvcr71.dll", MAX_PATH - strlen(msvcdll));
  strncat(javadll, "\\bin\\client\\jvm.dll", MAX_PATH - strlen(javadll));
  msvcdll[MAX_PATH -1] = '\0';
  javadll[MAX_PATH -1] = '\0';
  h1 = LoadLibrary(msvcdll);
  if (h1 == NULL) {
    fprintf(stderr, "Can't load library %s\n",msvcdll);
    exit(1);
  }
  h2 = LoadLibrary(javadll);
  if (h2 == NULL) {
    fprintf(stderr, "Can't load library %s\n",javadll);
    exit(1);
  }
  return h2;
}
#endif

int my_initjni(JavaVM** jvm, JNIEnv** env) {
	// Attach current thread to JavaVM
	jint status;

	if (!env_a)
		return -1;
	env_a->GetJavaVM(jvm);
	status = (*jvm)->AttachCurrentThread((void**)env, NULL);
  if (status != JNI_OK) {
		fprintf(stderr, "Can't attach to JVM, return = %d\n", status);
		return -2;
  }
	return 0;
}

int my_finijni(JavaVM** jvm, JNIEnv** env) {
	// ignore exceptions here?
	jint status;
  if ((*env)->ExceptionOccurred()) {
    (*env)->ExceptionDescribe();
  }
	status = (*jvm)->DetachCurrentThread();
	if (status != JNI_OK) {
		fprintf(stderr, "Can't detach from JVM, return = %d\n", status);
		return -2;
	}
	return 0;
}

int jnitest() {
  JNIEnv *env;
	JavaVM *jvm;
  jint status;
  jclass cls;
  jmethodID mid[5];
  jstring jstr;
  jclass stringClass;
  jobjectArray args;

	if (my_initjni(&jvm, &env)) {
		return -1;
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
	my_finijni(&jvm, &env);
	return 0;
}

char* jni_doSignature(char* randomString, char* tpmPass) {
  JNIEnv *env;
	JavaVM *jvm;
	char *ret = NULL;
	jboolean iscopy = JNI_TRUE;
	if (my_initjni(&jvm, &env)) {
		return NULL;
	}
	jclass cls = env->FindClass("com/intel/splat/identityservice/tpm/TPMSign");
	if (cls == NULL) {
		goto destroy2;
	}
	jmethodID mid = env->GetStaticMethodID(cls,"doSignature","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
	if (mid == NULL) {
		goto destroy2;
	}
	jstring randomstr = NULL;
	if (randomString)
		randomstr = env->NewStringUTF(randomString);
	jstring tpmpass = env->NewStringUTF(tpmPass);
	jstring retstr = (jstring)env->CallStaticObjectMethod(cls,mid,randomstr,tpmpass);
	printf("retstr = %x, ",retstr);
	if (retstr) {
		const char* value = (char*) env->GetStringUTFChars(retstr,&iscopy);//copy jstring
		int len = env->GetStringUTFLength(retstr);//get string length
		printf("ret value is = %s\n",value);
	}
destroy2:
	// detect exceptions
  if (env->ExceptionOccurred()) {
    env->ExceptionDescribe();
  }
	my_finijni(&jvm, &env);
	return ret;
}

char* jni_getPublicKeyContent() {
  JNIEnv *env = NULL;
	JavaVM *jvm = NULL;
	char* ret = NULL;
	jboolean iscopy = JNI_TRUE;
	if (my_initjni(&jvm, &env)) {
		return NULL;
	}
	jclass cls = env->FindClass("com/intel/splat/identityservice/tpm/TPMSign");
	if (cls == NULL) {
		goto destroy;
	}
	jmethodID mid = env->GetStaticMethodID(cls,"getPublicKeyContent","()Ljava/lang/String;");
	if (mid == NULL) {
		goto destroy;
	}

	jstring retstr = (jstring) env->CallStaticObjectMethod(cls,mid);
	printf("retstr = %x, ",retstr);
	if (retstr) {
		const char *value = env->GetStringUTFChars(retstr,&iscopy);//copy jstring
		int len = env->GetStringUTFLength(retstr);//get string length
		printf("ret value is = %s\n",value);
//	ret = m_strdup((char*)value);
	}
	
destroy:
	// detect exceptions
  if (env->ExceptionOccurred()) {
    env->ExceptionDescribe();
  }
	my_finijni(&jvm, &env);
//	printf("ret = %x, %s\n",ret,ret);
	return ret;
}