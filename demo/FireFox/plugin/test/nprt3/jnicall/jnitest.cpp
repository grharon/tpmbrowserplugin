#include "jnitest.h"
#include <jni.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WINDOWS
#include <windows.h>
static HINSTANCE loadJVMLibrary(void);

typedef jint (JNICALL *CreateJavaVM_t) (JavaVM **pvm, void **penv, JavaVMInitArgs *args);
typedef jint (JNICALL *GetCreatedJavaVMs_t) (JavaVM **pvm, jsize, jsize*);
#endif

static JavaVM* jvm = NULL;
static JNIEnv* env = NULL;

int loadJVM(const char* _classpath) {
  JavaVMInitArgs vm_args;
  jint status;
  const int OPTNUM = 3;
  JavaVMOption options[OPTNUM];
#ifdef _WINDOWS
  CreateJavaVM_t createJavaVM;
  GetCreatedJavaVMs_t getCreatedJavaVMs;
#endif
  const char prefix[] = "-Djava.class.path=";
	char* classpath;
	if (_classpath)
		classpath = (char*) _classpath;
	else
		classpath = "";
  int len = strlen(prefix) + strlen(classpath);
  char* str = (char*) malloc(len+1);
  memcpy(str,prefix,strlen(prefix)+1);  
  strncat(str,classpath,strlen(classpath));
  str[len] = '\0';
  options[0].optionString =str;
  options[1].optionString ="-verbose:jni";
  options[2].optionString ="-Djava.compiler=NONE";
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
  printf("status = %d, jvm_count = %d\n",status,jvm_count);
  if (jvm_count > 0) {
    jvm = jvms[0];
    jvm->AttachCurrentThread((void**)&env, &vm_args);
  }
  else {
#ifdef _WINDOWS
    status = (*createJavaVM) (&jvm, (void**) &env, &vm_args);
#else
    status = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);
#endif
  }
  if (status == JNI_ERR) {
    fprintf(stderr,"Error creating VM\n");
    return -1;
  }
	return 0;
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

void destroyJVM(void) {
  /*
  // destroy JVM, but it really does no help
  status = jvm->DestroyJavaVM();
  if (status) {
    fprintf(stderr, "Can't destroy Java VM\n");
    exit(1);
  }
  if (hjvmlib) {
    FreeLibrary(hjvmlib);
    hjvmlib = NULL;
  }
  */
}

char* jni_getPublicKeyContent(bool& fail) {
  char* ret = NULL;
	fail = false;

  jboolean iscopy = JNI_FALSE;
  jstring retstr = NULL;
  jmethodID mid = NULL;
  jclass cls = env->FindClass("com/intel/splat/identityservice/tpm/TPMSign");
  if (cls == NULL) {
    goto destroy;
  }
  mid = env->GetStaticMethodID(cls,"getPublicKeyContent","()Ljava/lang/String;");
  if (mid == NULL) {
    goto destroy;
  }

  retstr = (jstring) env->CallStaticObjectMethod(cls,mid);
  if (retstr) {
    const char *value = env->GetStringUTFChars(retstr,&iscopy);//copy jstring
		int len = env->GetStringUTFLength(retstr);
		ret = (char*) malloc(len+1);
		strncpy(ret, value, len);
		ret[len] = '\0';
		printf("len = %d",len);
  }

destroy:
  // detect exceptions
  if (env->ExceptionOccurred()) {
    env->ExceptionDescribe();
		fail = true;
  }
  return ret;
}

char* jni_doSignature(bool& fail,char *randomString, char* tpmPass) {
	char *ret = NULL;
	fail = false;

	jboolean iscopy = JNI_TRUE;
  jstring retstr = NULL;
  jmethodID mid = NULL;
	jstring randomstr = NULL;
	jstring tpmpass = NULL;
	if (randomString)
		randomstr = env->NewStringUTF(randomString);
	if (tpmPass)
		tpmpass = env->NewStringUTF(tpmPass);

	jclass cls = env->FindClass("com/intel/splat/identityservice/tpm/TPMSign");
	if (cls == NULL) {
		goto destroy;
	}
	mid = env->GetStaticMethodID(cls,"doSignature","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
	if (mid == NULL) {
		goto destroy;
	}
	retstr = (jstring)env->CallStaticObjectMethod(cls,mid,randomstr,tpmpass);

  if (retstr) {
    ret = (char*) env->GetStringUTFChars(retstr,&iscopy);//copy jstring
  }

destroy:
	// detect exceptions
  if (env->ExceptionOccurred()) {
    env->ExceptionDescribe();
		fail = true;
  }
	return ret;
}

