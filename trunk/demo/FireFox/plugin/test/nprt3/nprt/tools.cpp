#include "tools.h"
#include <iostream>
#include <stdio.h>
#include <npapi.h>
#include <npruntime.h>
using namespace std;

#ifdef XP_WIN
#include <windows.h>
static int GetStringFromRegistry(HKEY key, const char*name, char*buf, int bufsize) {
  DWORD type,size;
  return RegQueryValueEx(key, name, 0, &type, 0, &size)==0
      && type == REG_SZ
      && size<(unsigned int) bufsize
      && RegQueryValueEx(key, name, 0, 0, (LPBYTE)buf, &size) == 0;
}
#endif

const char* getTemporaryPath() {
#ifdef XP_WIN
	const char *ret = getenv("tmp");
	if (ret)
		return ret;
	ret = getenv("temp");
	if (ret)
		return ret;
	ret = getenv("tmpdir");
	if (ret)
		return ret;
	else
		return "C:\\";
#elif
	return "/tmp/";
#endif
}

const char* getHomePath() {
#ifdef XP_WIN
	const char *ret = getenv("home");
	if (ret)
		return ret;
	ret = getenv("usersprofile");
	if (ret)
		return ret;
	ret = getenv("allusersprofile");
	if (ret)
		return ret;
	else
		return "C:\\";
#else
	return "~/";
#endif
}

#ifdef XP_WIN
const char* getFirefoxPathFromReg() {
	HKEY key, subkey1, subkey2;
	char buf[MAX_PATH];
	char version[MAX_PATH];
	char *ff_key = "SOFTWARE\\Mozilla\\Mozilla Firefox";
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, ff_key, 0, KEY_READ, &key) != 0) {
		cerr<<"Error opening registry key '"<<ff_key<<"'"<<endl;
		return NULL;
	}
	if (!GetStringFromRegistry(key, "CurrentVersion",version,sizeof(version))) {
		cerr<<"Failed reading value of registry key:"<<endl;
		cerr<<"\t"<<ff_key<<"\\CurrentVersion"<<endl;
    RegCloseKey(key);
		return NULL;
  }
  if (RegOpenKeyEx(key, version, 0, KEY_READ, &subkey1) != 0) {
    cerr<<"Error opening registry key '"<<ff_key<<"\\"<<version<<"'"<<endl;
    RegCloseKey(key);
		return NULL;
  }
	if (RegOpenKeyEx(subkey1, "Main", 0, KEY_READ, &subkey2) != 0) {
    cerr<<"Error opening registry key '"<<ff_key<<"\\"<<version<<"\\Main'"<<endl;
    RegCloseKey(key);
    RegCloseKey(subkey1);
		return NULL;
	}
	if (!GetStringFromRegistry(subkey2,"Install Directory",buf,MAX_PATH)) {
		cerr<<"Failed reading value of registry key:"<<endl;
		cerr<<"\t"<<ff_key<<"\\"<<version<<"\\Main\\Install Directory"<<endl;
    RegCloseKey(key);
    RegCloseKey(subkey1);
    RegCloseKey(subkey2);
		return NULL;
  }
  RegCloseKey(key);
  RegCloseKey(subkey1);
  RegCloseKey(subkey2);
	int len = strlen(buf);
	char *ret = (char*) NPN_MemAlloc(len+1);
	strncpy(ret,buf,len);
	ret[len] = '\0';
	return ret;
}
#endif

const char* getFirefoxPath() {
#ifdef XP_WIN
	const char* ret = getFirefoxPathFromReg();
	if (ret)
		return ret;
	else
		return "C:\\Program Files\\Mozilla Firefox";
#else
	return "~/.firefox";
#endif
}

const char* getConfPath() {
	const char* home = getHomePath();
	if (!home)
		return NULL;
	int len = strlen(home) + strlen(conf_file)+1;
	char* ret = (char*) NPN_MemAlloc(len+1);
	_snprintf(ret,len+1,"%s/%s",home,conf_file);
	return ret;
}

const char* getSpawnPath() {
	const char* ffpath = getFirefoxPath();
	if (!ffpath)
		return NULL;
	int len = strlen(ffpath) + strlen(spawn_file) + strlen(plugin_path) + 2;
	char* ret = (char*) NPN_MemAlloc(len+1);
	_snprintf(ret,len+1,"%s/%s/%s",ffpath,plugin_path,spawn_file);
	return ret;
}

const char* getClassPath() {
	const char* fname = getConfPath();
	FILE *f = fopen(fname, "r");
	if (f) {
		fseek(f,0,SEEK_END);
		int total = ftell(f);
		fseek(f,0,SEEK_SET);
		char buf[MAX_PATH];
		if (fgets(buf,MAX_PATH,f) == NULL) {
			fclose(f);
			return "";
		}
		fclose(f);

		int len = strlen(buf);
		char *ret = (char*) NPN_MemAlloc(len+1);		
		_snprintf(ret,len+1,"%s",buf);
		if ((len > 0) && (ret[len-1] == '\n'))
			ret[len-1] = '\0';
		return ret;
	}
	else
		return "";
}

