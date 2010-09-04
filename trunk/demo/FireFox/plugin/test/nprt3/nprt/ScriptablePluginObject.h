/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
#include "ScriptablePluginObjectBase.h"

#ifndef SCRIPTABLEPLUGINOBJECT_H
#define SCRIPTABLEPLUGINOBJECT_H
#include "fix.h"

class ScriptablePluginObject : public ScriptablePluginObjectBase
{
public:
  ScriptablePluginObject(NPP npp)
    : ScriptablePluginObjectBase(npp)
  {
		// sample of initialized value : keyword	
		keyword = m_strdup("HELLO WORLD");
		// sample of uninitailized value : code
		code = NULL;
  }

	virtual ~ScriptablePluginObject() {
		// dealloc memory
		if (keyword)
			NPN_MemFree(keyword);
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
	// define ScriptablePluginObject vars here
	char* keyword;
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