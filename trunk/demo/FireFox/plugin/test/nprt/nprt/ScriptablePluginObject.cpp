#include "ScriptablePluginObject.h"
#include "ConstructablePluginObject.h"
#include "fix.h"
#include "npapi.h"

bool
ScriptablePluginObject::HasMethod(NPIdentifier name)
{
  return ((name == sFoo_id) || (name = sGetURL_id));
}

bool
ScriptablePluginObject::HasProperty(NPIdentifier name)
{
  return ((name == sBar_id) ||
          (name == sPluginType_id));
}

bool
ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
  VOID_TO_NPVARIANT(*result);

  if (name == sBar_id) {
    static int a = 17;

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
  if (name == sFoo_id) {
    printf ("foo called!\n");
	//MessageBox(NULL,"foo called!","ScriptablePlugin Sample",MB_OK);
	
    NPVariant docv;
    NPN_GetProperty(mNpp, sWindowObj, sDocument_id, &docv);

    NPObject *doc = NPVARIANT_TO_OBJECT(docv);//window.document -> docv -> doc

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

    NPN_ReleaseVariantValue(&docv);
	

    STRINGZ_TO_NPVARIANT(m_strdup("foo return val"), *result);// "foo return val" -> result
    return true;
  }
  if (name == sGetURL_id) {
	  /*
	  typedef struct tagPARAMPAIR {
		LPTSTR pName;
		LPTSTR pValue;
	  }PARAMPAIR,*PPARAMPAIR;
	  vector<PARAMPAIR> m_vecParamPair;
	  if ((args != NULL) && (argCount >= 2)) {
		  NPVariant npvName = args[0];
		  NPVariant npvValue = args[1];
		  if (NPVARIANT_IS_STRING(npvName) && NPVARIANT_IS_STRING(npvValue)) {
			  NPString npsName = NPVARIANT_TO_STRING(npvName);
			  NPString npsValue = NPVARIANT_TO_STRING(npvValue);
			  if (npsName.UTF8Characters && strlen(npsName.UTF8Characters) > 0) {// restrict this one not to be ""
				  int nLenName = strlen(npsName.UTF8Characters)+1;
				  int nLenValue = strlen(npsValue.UTF8Characters)+1;
				  PARAMPAIR paramPair;
				  paramPair.pName = new char[nLenName];
				  memset(paramPair.pName,0,nLenName);
				  paramPair.pValue = new char[nLenValue];
				  memset(paramPair.pValue,0,nLenValue);
				  strcpy(paramPair.pName,npsName.UTF8Characters);
				  strcpy(paramPair.pValue,npsValue.UTF8Characters);
				  m_vecParamPair.push_back(paramPair);
			  }
		  }
	  }
	  */
	return true;
  }
  return false;
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
  if (name == sBar_id) {
    printf ("bar set\n");

    return true;
  }
  return false;
}
