/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
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
		memcpy(this->keyword, ch, len);
		this->keyword[len]='\0';
		rURL = NULL;
		code = NULL;
  }

	virtual ~ScriptablePluginObject() {
		if (keyword)
			NPN_MemFree(keyword);
		if (rURL) 
			NPN_MemFree(rURL);
		if (code)
			NPN_MemFree(code);
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
	char* code;
};

static NPObject *
AllocateScriptablePluginObject(NPP npp, NPClass *aClass)
{
  return new ScriptablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject,
                                 AllocateScriptablePluginObject);

#endif