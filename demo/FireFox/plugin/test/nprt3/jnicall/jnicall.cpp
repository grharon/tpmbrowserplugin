#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdio.h>

#include "jnitest.h"
using namespace std;

void usage() {
	cerr<<"<program name> --debug --file <filename> --method <methodname> [--eof <eofline>] [--classpath <classpath>] [--args <argnum> <arg0, ...> ]"<<endl;
	cerr<<"  debug		: output debug info"<<endl;
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
	string classpath("");
	string eofstr("");
	int argnum = 0;
	bool debug = false;
	char** args = NULL;

	if (0) {
		cout<<" arguments : ";
		for (int i=0;i<argc;i++)
			cout<<argv[i]<<' ';
		cout<<endl;
	}

	for (int i=0;i<argc;i++) {
		if (!strcmp(argv[i],"--debug")) {
			debug = true;
		}
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

	if (debug) {
		cout<<"tmpfile = "<<tmpfile<<endl;
		cout<<"method = "<<method<<endl;
		cout<<"eof = "<<eofstr<<endl;
		cout<<"classpath = "<<classpath<<endl;
		cout<<"args = "<<argnum<<":";
		for (int i=0;i<argnum;i++)
			cout<<args[i]<<",";
		cout<<endl;
	}
	int v = loadJVM(classpath.c_str(), debug);
	if (debug) {
		cout<<"loadJVM = "<<v<<endl;
	}
	if (v != 0) {
		cerr<<"Cannot load JVM"<<endl;
		return -1;
	}

	char *ret = NULL;
	bool fail = true;
	if ((method == "doSignature") && (argnum == 2)) {
		ret = jni_doSignature(fail, args[0], args[1]);
	}
	else if ((method == "getPublicKeyContent") && (argnum == 0)) {
		ret = jni_getPublicKeyContent(fail);
	}
	if (debug) {
		if (ret)
			cout<<"return value is "<<ret<<endl;
		else
			cout<<"return value is null"<<endl;
	}

	FILE* f = fopen(tmpfile.c_str(),"wb");
	if (f) {
		if (ret) {
			int len = strlen(ret);
			int done = 0;
			if ((done = fwrite(ret,sizeof(char),len,f)) != len) {
				cerr<<"write "<<done<<" bytes, less than required "<<len<<" bytes.";
				fail = true;
			}
		}
/*
		fwrite(eofstr.c_str(),sizeof(char),eofstr.size(),f);
*/
		fflush(f);
		fclose(f);
	}
	else {
		cerr<<"cannot open "<<tmpfile<<endl;
		fail = true;
	}

	destroyJVM();
	if (fail) {
		unlink(tmpfile.c_str());
		return -1;
	}
	else
		return 0;
}