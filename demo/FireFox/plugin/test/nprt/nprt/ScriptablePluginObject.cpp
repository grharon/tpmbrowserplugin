#include "ScriptablePluginObject.h"
#include "ConstructablePluginObject.h"
#include "fix.h"
#include "npapi.h"

bool
ScriptablePluginObject::HasMethod(NPIdentifier name)
{
	NPIdentifier sFoo_id = NPN_GetStringIdentifier("foo");
	NPIdentifier sGetURL_id = NPN_GetStringIdentifier("GetURL");
  return ((name == sFoo_id) || (name == sGetURL_id));
}

bool
ScriptablePluginObject::HasProperty(NPIdentifier name)
{
	NPIdentifier sBar_id = NPN_GetStringIdentifier("bar");
	NPIdentifier sPluginType_id = NPN_GetStringIdentifier("PluginType");
  return ((name == sBar_id) ||
          (name == sPluginType_id));
}

bool
ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
	VOID_TO_NPVARIANT(*result);// set result to void

	NPIdentifier sBar_id = NPN_GetStringIdentifier("bar");
	NPIdentifier sPluginType_id = NPN_GetStringIdentifier("PluginType");

	//printf("bar_id = %d\n",sBar_id);
  if (name == sBar_id) {
    INT32_TO_NPVARIANT(a, *result);
    a += 5;
    return true;
  }

  if (name == sPluginType_id) {
    NPObject *myobj =
    NPN_CreateObject(mNpp, GET_NPOBJECT_CLASS(ConstructablePluginObject));
    if (!myobj) {
      return false;
    }

    OBJECT_TO_NPVARIANT(myobj, *result);
    return true;
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
	if (!this->HasMethod(name))
		return false;
	NPIdentifier sFoo_id = NPN_GetStringIdentifier("foo");
	NPIdentifier sGetURL_id = NPN_GetStringIdentifier("GetURL");
	NPObject* sWindowObj;
  NPN_GetValue(mNpp, NPNVWindowNPObject, &sWindowObj);
  NPVariant docv;
	NPIdentifier sDocument_id = NPN_GetStringIdentifier("document");
  NPIdentifier sBody_id = NPN_GetStringIdentifier("body");
  NPIdentifier sCreateElement_id = NPN_GetStringIdentifier("createElement");
  NPIdentifier sCreateTextNode_id = NPN_GetStringIdentifier("createTextNode");
  NPIdentifier sAppendChild_id = NPN_GetStringIdentifier("appendChild");

	NPN_GetProperty(mNpp, sWindowObj, sDocument_id, &docv);
	NPObject *doc = NPVARIANT_TO_OBJECT(docv);//window.document -> docv -> doc
	if (name == sFoo_id) {
    printf ("foo called!\n");
		//MessageBox(NULL,"foo called!","ScriptablePlugin Sample",MB_OK);

    NPVariant strv;
    STRINGZ_TO_NPVARIANT("div", strv);//"div" -> strv

    NPVariant divv;
    NPN_Invoke(mNpp, doc, sCreateElement_id, &strv, 1, &divv);//document.CreateElement(strv) -> divv

    STRINGZ_TO_NPVARIANT("I'm created by a plugin!", strv);

    NPVariant textv;
    NPN_Invoke(mNpp, doc, sCreateTextNode_id, &strv, 1, &textv);//document.CreateTextNode(strv) -> textv

    NPVariant v;
    NPN_Invoke(mNpp, NPVARIANT_TO_OBJECT(divv), sAppendChild_id, &textv, 1,
               &v);//divv.AppendChild(textv) -> v
    NPN_ReleaseVariantValue(&v);

    NPN_ReleaseVariantValue(&textv);

    NPVariant bodyv;
    NPN_GetProperty(mNpp, doc, sBody_id, &bodyv);

    NPN_Invoke(mNpp, NPVARIANT_TO_OBJECT(bodyv), sAppendChild_id, &divv, 1,
               &v);//bodyv.AppendChild(divv) -> v
    NPN_ReleaseVariantValue(&v);

    NPN_ReleaseVariantValue(&divv);
    NPN_ReleaseVariantValue(&bodyv);

    STRINGZ_TO_NPVARIANT(m_strdup("foo return val"), *result);// "foo return val" -> result
  }
  if (name == sGetURL_id) {
    printf("GetURL() called\n");
    NPIdentifier n = NPN_GetStringIdentifier("rURL");
	  NPVariant rval;

	  NPN_GetProperty(mNpp, doc, n, &rval);
		if (NPVARIANT_IS_STRING(rval)) {
		  NPError err = NPN_GetURL(mNpp,NPVARIANT_TO_STRING(rval).UTF8Characters, "_self");
      NPN_ReleaseVariantValue(&rval);
    }
		else {
		  NPError err = NPN_GetURL(mNpp, "http://www.google.com.hk/","_self");
		}
	  /*
	  if ((args != NULL) && (argCount >= 2)) {
		  NPVariant npvName = args[0];
		  NPVariant npvValue = args[1];
		  if (NPVARIANT_IS_STRING(npvName) && NPVARIANT_IS_STRING(npvValue)) {
			  NPString npsName = NPVARIANT_TO_STRING(npvName);
			  NPString npsValue = NPVARIANT_TO_STRING(npvValue);
			  if (npsName.UTF8Characters && strlen(npsName.UTF8Characters) > 0) {// restrict this one not to be ""
				  int nLenName = strlen(npsName.UTF8Characters)+1;
				  int nLenValue = strlen(npsValue.UTF8Characters)+1;
				  ...
			  }
		  }
	  }
	  */
    STRINGZ_TO_NPVARIANT(m_strdup("ok"), *result);
  }
  NPN_ReleaseVariantValue(&docv);
	NPN_ReleaseObject(sWindowObj);
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
	NPIdentifier sBar_id = NPN_GetStringIdentifier("bar");
  if (name == sBar_id) {
    printf ("bar set\n");
		a = 17;
    return true;
  }
  return false;
}
