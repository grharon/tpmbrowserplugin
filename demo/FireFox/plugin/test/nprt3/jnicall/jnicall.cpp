#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdio.h>

#include "jnitest.h"
using namespace std;

void usage() {
	cerr<<"<program name> --file <filename> --method <methodname> [--eof <eofline>] [--classpath <classpath>] [--args <argnum> <arg0, ...> ]"<<endl;
	cerr<<"  filename : tmpfile name"<<endl;
	cerr<<"  method   : method to call"<<endl;
	cerr<<"  eof      : eof flag"<<endl;
	cerr<<"  classpath: classpath"<<endl;
	cerr<<"  argnum   : number of arguments"<<endl;
	cerr<<"  arg0,... : arguments"<<endl;
	exit(2);
}


int main(int argc, char** argv) {
	string tmpfile;
	string method;
	string classpath("D:/tail/TPM/TPM\ emluator/TPMSignatureApplet/bin/;D:/tail/TPM/TPM\ emluator/TPMSignatureApplet/lib/bouncycastle-jce-jdk13-112.jar;");
//	string classpath("D:/TPM/TPMUtil/TPMUtils/bin/;D:/TPM/TPMUtil/TPMUtils/lib/tpmj.jar");
	string eofstr("");
	int argnum = 0;
	char** args = NULL;

	printf("args = ");
	for (int i=0;i<argc;i++) {
		printf(" %s",argv[i]);
	}
	printf("\n");
	for (int i=0;i<argc;i++) {
		if (!strcmp(argv[i],"--file")) {
			if (++i == argc)
				usage();
			tmpfile = string(argv[i]);
		}
		else if (!strcmp(argv[i],"--method")) {
			if (++i == argc)
				usage();
			method = string(argv[i]);
		}
		else if (!strcmp(argv[i],"--eof")) {
			if (++i == argc)
				usage();
			eofstr = string(argv[i]);
		}
		else if (!strcmp(argv[i],"--classpath")) {
			if (++i == argc)
				usage();
			classpath = string(argv[i]);
		}
		else if (!strcmp(argv[i],"--args")) {
			if (++i == argc)
				usage();
			argnum = atoi(argv[i]);
			if (argnum > 0) {
				args = new char*[argnum];
				for (int j=0;j<argnum;j++) {
					if (++i == argc)
						usage();
					args[j] = argv[i];
				}
			}
		}
	}
	if ((tmpfile.empty()) || (method.empty())) {
		usage();
	}
	if (1) {
		cout<<"tmpfile = "<<tmpfile<<endl;
		cout<<"method = "<<method<<endl;
		cout<<"eof = "<<eofstr<<endl;
		cout<<"args = "<<argnum<<":";
		for (int i=0;i<argnum;i++)
			cout<<args[i]<<",";
		cout<<endl;
	}
	int v = loadJVM(classpath.c_str());
	printf("loadJVM = %d\n",v);
	char *ret = NULL;
	bool fail;
	if ((method == "doSignature") && (argnum == 2)) {
		ret = jni_doSignature(fail, args[0], args[1]);
	}
	if ((method == "getPublicKeyContent") && (argnum == 0)) {
		ret = jni_getPublicKeyContent(fail);
	}
	printf("ret = %s\n",ret);
	FILE* f = fopen(tmpfile.c_str(),"wb");
	if (f) {
		if (ret)
			fwrite(ret,sizeof(char),strlen(ret),f);
/*
		fwrite("\n",sizeof(char),1,f);
		fwrite(eofstr.c_str(),sizeof(char),eofstr.size(),f);
*/
		fflush(f);
		fclose(f);
	}
	destroyJVM();
	if (fail)
		return -1;
	else
		return 0;
}