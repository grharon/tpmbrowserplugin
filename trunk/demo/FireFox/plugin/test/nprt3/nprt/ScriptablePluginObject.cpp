/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
#include <npapi.h>
#include <stdlib.h>

#include "ScriptablePluginObject.h"
#include "ConstructablePluginObject.h"
#include "fix.h"
#include "jnitest.h"

bool
ScriptablePluginObject::HasMethod(NPIdentifier name)
{
	// method for GETURL API test
	NPIdentifier GetURL_id = NPN_GetStringIdentifier("GetURL");
	// method for login.jsp test
	NPIdentifier doSignature_id = NPN_GetStringIdentifier("doSignature");
	NPIdentifier getPublicKeyContent_id = NPN_GetStringIdentifier("getPublicKeyContent");
	NPIdentifier jnitest_id = NPN_GetStringIdentifier("jnitest");

  return ((name == GetURL_id) || (name == doSignature_id) || 
		(name == getPublicKeyContent_id) || (name == jnitest_id) );
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
	//GetURL test
	NPIdentifier GetURL_id = NPN_GetStringIdentifier("GetURL");
	//login.jsp test
	NPIdentifier doSignature_id = NPN_GetStringIdentifier("doSignature");
	NPIdentifier getPublicKeyContent_id = NPN_GetStringIdentifier("getPublicKeyContent");
	NPIdentifier jnitest_id = NPN_GetStringIdentifier("jnitest");

	NPObject* sWindowNPObj;

	if ((err = NPN_GetValue(mNpp, NPNVWindowNPObject, &sWindowNPObj)) != NPERR_NO_ERROR) {
		printf("Error in getting NPNVWindowNPObject: %d\n",err);
		return false;
	}

	// GetURL code
  if (name == GetURL_id) {
    printf("GetURL() called\n");
		if (rURL) {
			if (NPN_GetURL(mNpp, rURL, "_self") != NPERR_NO_ERROR) {
				printf("error on converting GetURL\n");
			}
		} else {
			if (NPN_GetURL(mNpp, "http://www.google.com.hk/","_self") != NPERR_NO_ERROR) {
				printf("error on converting GetURL\n");
			}
		}
    STRINGZ_TO_NPVARIANT(m_strdup("ok"), *result);
  }
	else if (name == doSignature_id) {
		NPString str;
		str.UTF8Characters = "alert('call doSignature_id(String,String)');";
		str.UTF8Length = strlen(str.UTF8Characters);
		VOID_TO_NPVARIANT(*result);

		if ((argCount == 2) && (NPVARIANT_IS_STRING(args[0])) && (NPVARIANT_IS_STRING(args[1]))) {
			char *name = NULL;
			char *pwd = NULL;
			NPString nameS = NPVARIANT_TO_STRING(args[0]);
			NPString pwdS = NPVARIANT_TO_STRING(args[1]);
			m_strFromNP(&name,nameS);
			m_strFromNP(&pwd,pwdS);
			printf("%s %s",name,pwd);
			NPN_Evaluate(this->mNpp, sWindowNPObj, &str, NULL);
			char* ret = jni_doSignature(name,pwd);
			printf("ret=%x\n",ret);
			/*
	char buf[50];
	scanf("%s",buf);
			*/

			/*
			if (ret)
				STRINGZ_TO_NPVARIANT(m_strdup(ret),*result);
			*/
		}
	}
	else if (name == getPublicKeyContent_id) {
		NPString str;
		str.UTF8Characters = "alert('call getPublicKeyContent()');";
		str.UTF8Length = strlen(str.UTF8Characters);
		VOID_TO_NPVARIANT(*result);
		if (argCount == 0) {
			NPN_Evaluate(this->mNpp, sWindowNPObj, &str, NULL);
			char *ret = jni_getPublicKeyContent();
			printf("ret=%x\n",ret);
			/*
			if (ret) {
				STRINGZ_TO_NPVARIANT(m_strdup(ret), *result);
			}
			*/
		}
	}
	else if (name == jnitest_id) {
		jnitest();
		VOID_TO_NPVARIANT(*result);
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

