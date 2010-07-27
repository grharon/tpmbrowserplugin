#ifndef SCRIPTABLEPLUGINOBJECT_H
#define SCRIPTABLEPLUGINOBJECT_H
#include "ScriptablePluginObjectBase.h"

// Methods
extern NPIdentifier sFoo_id;
extern NPIdentifier sBar_id;
extern NPIdentifier sPluginType_id;
extern NPIdentifier sCreateElement_id;
extern NPIdentifier sCreateTextNode_id;
extern NPIdentifier sDocument_id;
extern NPIdentifier sAppendChild_id;
extern NPIdentifier sBody_id;
// modified 
extern NPIdentifier sGetURL_id;

class ScriptablePluginObject : public ScriptablePluginObjectBase
{
public:
  ScriptablePluginObject(NPP npp)
    : ScriptablePluginObjectBase(npp)
  {
  }

  virtual bool HasMethod(NPIdentifier name);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
  virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

static NPObject *
AllocateScriptablePluginObject(NPP npp, NPClass *aClass)
{
  return new ScriptablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject,
                                 AllocateScriptablePluginObject);

#endif