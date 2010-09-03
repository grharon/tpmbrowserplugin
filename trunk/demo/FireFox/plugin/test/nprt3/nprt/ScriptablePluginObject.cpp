/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
#include "ScriptablePluginObject.h"
#include "ConstructablePluginObject.h"
#include "fix.h"

#include <npapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>

bool
ScriptablePluginObject::HasMethod(NPIdentifier name)
{
	// method for login.jsp test
	NPIdentifier doSignature_id = NPN_GetStringIdentifier("doSignature");
	NPIdentifier getPublicKeyContent_id = NPN_GetStringIdentifier("getPublicKeyContent");

  return ((name == doSignature_id) || (name == getPublicKeyContent_id));
}

bool
ScriptablePluginObject::HasProperty(NPIdentifier name)
{
	// method for Keyword value
	NPIdentifier keyword_id = NPN_GetStringIdentifier("keyword");
	NPIdentifier rURL_id = NPN_GetStringIdentifier("rURL");
	NPIdentifier code_id = NPN_GetStringIdentifier("code");
  return ((name == keyword_id) || 
		(name == rURL_id) ||
		(name == code_id));
}

bool
ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
	VOID_TO_NPVARIANT(*result);// set result to void
	if (!HasProperty(name)) {
		return false;
	}
	NPIdentifier keyword_id = NPN_GetStringIdentifier("keyword");
	NPIdentifier rURL_id = NPN_GetStringIdentifier("rURL");
	NPIdentifier code_id = NPN_GetStringIdentifier("code");
  if (name == keyword_id) {
		if (keyword)
			STRINGZ_TO_NPVARIANT(m_strdup(keyword), *result);// add reference here
  }
	else if (name == rURL_id) {
		if (rURL)
			STRINGZ_TO_NPVARIANT(m_strdup(rURL), *result);// add reference here
	}
	else if (name == code_id) {
		if (code)
			STRINGZ_TO_NPVARIANT(m_strdup(code), *result);// add reference here
	}
	return true;
}

bool
ScriptablePluginObject::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
/*
name : method name
args : arguments
argCount : number of arguments
result : return value
*/
{
	NPError err;
	if (!this->HasMethod(name))
		return false;
	//login.jsp test
	NPIdentifier doSignature_id = NPN_GetStringIdentifier("doSignature");
	NPIdentifier getPublicKeyContent_id = NPN_GetStringIdentifier("getPublicKeyContent");

	NPObject* sWindowNPObj;

	if ((err = NPN_GetValue(mNpp, NPNVWindowNPObject, &sWindowNPObj)) != NPERR_NO_ERROR) {
		printf("Error in getting NPNVWindowNPObject: %d\n",err);
		return false;
	}

	char *tmpdir = getenv("tmpdir");
	if (tmpdir == NULL)
#ifdef _WINDOWS
		tmpdir = "C:\\";
#else
		tmpdir = "~/"
#endif

	if (name == doSignature_id) {
		if ((argCount == 2) && (NPVARIANT_IS_STRING(args[0])) && (NPVARIANT_IS_STRING(args[1]))) {
			char *randomStr = NULL;
			char *tpmPass = NULL;
			NPString n_randomStr = NPVARIANT_TO_STRING(args[0]);
			NPString n_tpmPass = NPVARIANT_TO_STRING(args[1]);
			m_strFromNP(&randomStr,n_randomStr);
			m_strFromNP(&tpmPass,n_tpmPass);
			printf("input = %s, %s",randomStr, tpmPass);

			char* ret = NULL;
			char *fname = tempnam(tmpdir,"jni");
			if (fname == NULL)
				fname = "tmp";
			char* margs[10];
			margs[0] = "jnicall.exe";
			margs[1] = "--file";
			margs[2] = fname;
			margs[3] = "--method";
			margs[4] = "doSignature";
			margs[5] = "--args";
			margs[6] = "2";
			margs[7] = randomStr;
			margs[8] = tpmPass;
			margs[9] = NULL;
			// in windows use registry to find Firefox directory
			// in other OS, use path _spawnvp
			int rval = _spawnv(_P_WAIT,"c:\\jnicall",margs);
			if (rval) {
				fprintf(stderr,"error = %d\n",rval);
			}
			else {
				ret = getFileContent(fname);
				if (ret) {
					STRINGZ_TO_NPVARIANT(ret,*result); 
				}
				else {
					fprintf(stderr,"cannot read output file");
				}
				unlink(fname);
			}
			free(fname);
		}
		else {
			NPString str;
			str.UTF8Characters = "alert('usage: doSignature(String, String)');";
			str.UTF8Length = strlen(str.UTF8Characters);
			NPN_Evaluate(this->mNpp, sWindowNPObj, &str, NULL);
		}
	}
	else if (name == getPublicKeyContent_id) {
		if (argCount == 0) {
			char *ret = NULL;
			char *fname = tempnam(tmpdir,"jni");
			if (fname == NULL)
				fname = "tmp";
			char* margs[6];
			margs[0] = "jnicall.exe";
			margs[1] = "--file";
			margs[2] = fname;
			margs[3] = "--method";
			margs[4] = "getPublicKeyContent";
			margs[5] = NULL;
			int rval = _spawnv(_P_WAIT,"c:\\jnicall",margs);
			if (rval) {
				fprintf(stderr,"error = %d\n",rval);
			}
			else {
				ret = getFileContent(fname);
				if (ret) {
					STRINGZ_TO_NPVARIANT(ret,*result); 
				}
				else {
					fprintf(stderr,"cannot read output file");
				}
				unlink(fname);
			}
			free(fname);
		}
		else {
			NPString str;
			str.UTF8Characters = "alert('usage: getPublicKeyContent()');";
			str.UTF8Length = strlen(str.UTF8Characters);
			NPN_Evaluate(this->mNpp, sWindowNPObj, &str, NULL);
		}
	}
	NPN_ReleaseObject(sWindowNPObj);
  return true;
}

bool
ScriptablePluginObject::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  printf ("ScriptablePluginObject default method called!\n");

  STRINGZ_TO_NPVARIANT(m_strdup("default method return val"), *result);

  return true;
}

bool
ScriptablePluginObject::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
	if (!HasProperty(name)) {
		return false;
	}
	NPIdentifier keyword_id = NPN_GetStringIdentifier("keyword");
	NPIdentifier rURL_id = NPN_GetStringIdentifier("rURL");
	NPIdentifier code_id = NPN_GetStringIdentifier("code");

  if (name == keyword_id) {
		printf("call SetProperty::keyword\n");
		m_strFromVar(&keyword,*value);
	} else if (name == rURL_id) {
		printf("call SetProperty::rURL\n");
		m_strFromVar(&rURL,*value);
	} else if (name == code_id) {
		printf("call SetProperty::code\n");
		m_strFromVar(&code,*value);
	}
	// Release value?
	NPN_ReleaseVariantValue((NPVariant*)value);
	return true;
}

