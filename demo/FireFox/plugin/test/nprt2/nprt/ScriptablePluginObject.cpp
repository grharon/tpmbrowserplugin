#include "ScriptablePluginObject.h"
#include "ConstructablePluginObject.h"
#include "fix.h"
#include "npapi.h"

bool
ScriptablePluginObject::HasMethod(NPIdentifier name)
{
	// method for GETURL API test
	NPIdentifier GetURL_id = NPN_GetStringIdentifier("GetURL");
	// method for login.jsp test
	NPIdentifier doSignature_id = NPN_GetStringIdentifier("doSignature");
	NPIdentifier getPublicKeyContent_id = NPN_GetStringIdentifier("getPublicKeyContent");

  return ((name == GetURL_id) || (name == doSignature_id) || (name == getPublicKeyContent_id));
}

bool
ScriptablePluginObject::HasProperty(NPIdentifier name)
{
	// method for Keyword value
	NPIdentifier keyword_id = NPN_GetStringIdentifier("keyword");
  return (name == keyword_id);
}

bool
ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
	VOID_TO_NPVARIANT(*result);// set result to void
	NPIdentifier keyword_id = NPN_GetStringIdentifier("keyword");
  if (name == keyword_id) {
		STRINGZ_TO_NPVARIANT(m_strdup(this->keyword), *result);
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

	NPObject* sPluginElementObj;
	NPObject* sWindowNPObj;

	if ((err = NPN_GetValue(mNpp, NPNVPluginElementNPObject, &sPluginElementObj)) != NPERR_NO_ERROR) {
		printf("Error in getting NPNVPluginElementNPObject: %d\n",err);
		return false;
	}

	if ((err = NPN_GetValue(mNpp, NPNVWindowNPObject, &sWindowNPObj)) != NPERR_NO_ERROR) {
		printf("Error in getting NPNVWindowNPObject: %d\n",err);
		return false;
	}

	// GetURL code
  if (name == GetURL_id) {
    printf("GetURL() called\n");
    NPIdentifier n = NPN_GetStringIdentifier("rURL");
	  NPVariant rval;

	  NPN_GetProperty(mNpp, sPluginElementObj, n, &rval);
		if (NPVARIANT_IS_STRING(rval)) {
			if (NPN_GetURL(mNpp,NPVARIANT_TO_STRING(rval).UTF8Characters, "_self") != NPERR_NO_ERROR) {
				printf("error on converting GetURL\n");
			}
		}
		else {
			if (NPN_GetURL(mNpp, "http://www.google.com.hk/","_self") != NPERR_NO_ERROR) {
				printf("error on converting GetURL\n");
			}
		}
    NPN_ReleaseVariantValue(&rval);
    STRINGZ_TO_NPVARIANT(m_strdup("ok"), *result);
  }
	else if (name == doSignature_id) {
		if ((argCount == 2) && (NPVARIANT_IS_STRING(args[0])) && (NPVARIANT_IS_STRING(args[1]))) {
			NPString nameS = NPVARIANT_TO_STRING(args[0]);
			NPString pwdS = NPVARIANT_TO_STRING(args[1]);
			NPString str;
			str.UTF8Characters = "alert('call doSignature_id(String,String)');";
			str.UTF8Length = strlen(str.UTF8Characters);
			NPN_Evaluate(this->mNpp, sWindowNPObj, &str, NULL);
			STRINGZ_TO_NPVARIANT(m_strdup("signature"), *result);
		}
		else VOID_TO_NPVARIANT(*result);// set result to void, cause an error?
	}
	else if (name == getPublicKeyContent_id) {
		if (argCount == 0) {
			NPString str;
			//str.UTF8Characters = "alert('call getPublicKeyContent_id()');";
			str.UTF8Characters = "HELLO WORLD";
			str.UTF8Length = strlen(str.UTF8Characters);
			NPN_Evaluate(this->mNpp, sWindowNPObj, &str, NULL);
			STRINGZ_TO_NPVARIANT(m_strdup("publickeycontent"), *result);
		}
		else VOID_TO_NPVARIANT(*result);// set result to void, cause an error?
	}
	NPN_ReleaseObject(sPluginElementObj);
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
	NPIdentifier sKeyword_id = NPN_GetStringIdentifier("keyword");
  if (name == sKeyword_id) {
    printf ("keyword set\n");
		if NPVARIANT_IS_STRING(*value) {			
			NPString nkw = NPVARIANT_TO_STRING(*value);
			int len = nkw.UTF8Length;
			if (this->keyword)
				NPN_MemFree(this->keyword);
			this->keyword = (char*) NPN_MemAlloc(len+1);
			strncpy(this->keyword,nkw.UTF8Characters,len);
			this->keyword[len] = '\0';
		}
    return true;
  }
  return false;
}
