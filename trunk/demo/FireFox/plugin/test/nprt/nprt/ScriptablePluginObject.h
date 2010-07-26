#ifndef SCRIPTABLEPLUGINOBJECT_H
#define SCRIPTABLEPLUGINOBJECT_H
#include "ScriptablePluginObjectBase.h"

static NPIdentifier sFoo_id;
static NPIdentifier sBar_id;
static NPIdentifier sDocument_id;
static NPIdentifier sBody_id;
static NPIdentifier sCreateElement_id;
static NPIdentifier sCreateTextNode_id;
static NPIdentifier sAppendChild_id;
static NPIdentifier sPluginType_id;
static NPObject *sWindowObj;

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