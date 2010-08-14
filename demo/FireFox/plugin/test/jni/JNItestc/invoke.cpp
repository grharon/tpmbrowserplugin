/*
 * Compile commands are:
 * - Visual Studio
 * cl invoke.cpp /TP /link jvm.lib advapi32.lib
 */
#include <crtdbg.h>
#include <windows.h>
#include <jni.h>
#include <stdlib.h>
#define USER_CLASSPATH "../JNItest/bin" /* where Prog.class is */
#define JNI_VERSION JNI_VERSION_1_6
typedef JNIIMPORT jint (JNICALL *P_CreateJavaVM) (JavaVM **pvm, void **penv, JavaVMInitArgs *args);

/* Win32 version */
void *JNU_FindCreateJavaVM(char *vmlibpath)
{
  HINSTANCE hVM = LoadLibrary(vmlibpath);
  if (hVM == NULL) {
    return NULL;
  }
  return GetProcAddress(hVM, "JNI_CreateJavaVM");
}

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
  char *jvmPath = getJVMPath();
  printf("jvmPath = %s\n",jvmPath);
  //char jvmPath[100] = "C:\\Program Files\\Java\\jdk1.6.0_21\\jre\\bin\\client\\jvm.dll";
  if (jvmPath == NULL) {
    return -1;
  }
  void* ptr = JNU_FindCreateJavaVM(jvmPath);
  P_CreateJavaVM procJvm = (P_CreateJavaVM) ptr;
  if (procJvm == NULL)
    return -1;

  JavaVMOption options[1];
  options[0].optionString ="-Djava.class.path="USER_CLASSPATH;
  //ZeroMemory(&vm_args,sizeof(JavaVMInitArgs));
  vm_args.version = JNI_VERSION;
  vm_args.nOptions = 1;
  vm_args.ignoreUnrecognized = JNI_TRUE;
  vm_args.options = options;
  jint r   =   procJvm(&jvm,(void**)&env,&vm_args);
  if (r < 0)
    return r;
  if (pjvm != NULL)
    *pjvm = jvm;
  if (penv != NULL)
    *penv = env;
  return 0;
}

int main () {
  JNIEnv *env;
  JavaVM *jvm;
  jint res;
  jclass cls;
  jmethodID mid;
  jstring jstr;
  jclass stringClass;
  jobjectArray args;

  /* Create Java VM */
  res = MyCreateJavaVM(&jvm, &env);

  if (res < 0) {
    fprintf(stderr, "Can't create Java VM\n");
    exit(1);
  }
  cls = env->FindClass("Prog");
  if (cls == NULL) {
    goto destroy;
  }
  mid = env->GetStaticMethodID(cls, "main","([Ljava/lang/String;)V");
  if (mid == NULL) {
    goto destroy;
  }
  printf("version = %x\n",env->GetVersion());
  jstring arg1 = env->NewStringUTF(" TEST ");
  jmethodID mid1 = env->GetMethodID(cls,"<init>","()V");
  jobject obj = env->NewObject(cls,mid1);
  jmethodID mid2 = env->GetMethodID(cls,"test","()V");
  jobject obj2 = env->CallObjectMethod(obj,mid2);
  jmethodID mid3 = env->GetMethodID(cls,"getS","()Ljava/lang/String;");
  jmethodID mid4 = env->GetMethodID(cls,"setS","(Ljava/lang/String;)V");
  env->CallObjectMethod(obj,mid4,arg1);
  env->CallObjectMethod(obj,mid2);
  jstring str3 = (jstring) env->CallObjectMethod(obj,mid3);
  jboolean bool1 = JNI_TRUE;
  char* ch = (char*) env->GetStringUTFChars(str3,&bool1);
  printf("value from getS is: %s\n",ch);
  env->ReleaseStringUTFChars(str3,ch);
  jstr = env->NewStringUTF(" from C!");
  if (jstr == NULL) {
    goto destroy;
  }
  stringClass = env->FindClass("java/lang/String");
  args = env->NewObjectArray(1, stringClass, jstr);
  if (args == NULL) {
    goto destroy;
  }
  env->CallStaticVoidMethod(cls, mid, args);
destroy:
  if (env->ExceptionOccurred()) {
    env->ExceptionDescribe();
  }
  jvm->DestroyJavaVM();
  return 0;
}
