#include "ScriptablePluginObjectBase.h"

#ifndef SCRIPTABLEPLUGINOBJECT_H
#define SCRIPTABLEPLUGINOBJECT_H

class ScriptablePluginObject : public ScriptablePluginObjectBase
{
public:
  ScriptablePluginObject(NPP npp)
    : ScriptablePluginObjectBase(npp)
  {
		char ch[]="HELLO WORLD";
		int len =strlen(ch);
		this->keyword = (char*) NPN_MemAlloc(len+1);
		strncpy(this->keyword, ch, len);
		this->keyword[len]='\0';
		this->rURL = NULL;
  }

	virtual ~ScriptablePluginObject() {
		if (keyword)
			NPN_MemFree(keyword);
		if (rURL) 
			NPN_MemFree(rURL);
	}
  virtual bool HasMethod(NPIdentifier name);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
  virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
private:
	char* keyword;
	char* rURL;
};

static NPObject *
AllocateScriptablePluginObject(NPP npp, NPClass *aClass)
{
  return new ScriptablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject,
                                 AllocateScriptablePluginObject);

#endif