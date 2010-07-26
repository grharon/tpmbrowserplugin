/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

//////////////////////////////////////////////////
//
// CPlugin class implementation
//

#include <stdio.h>
#ifdef XP_WIN
#include <windows.h>
#include <windowsx.h>
#endif

#ifdef XP_MAC
#include <TextEdit.h>
#endif

#ifdef XP_UNIX
#include <string.h>
#endif

#include "plugin.h"
#include "npfunctions.h"

CPlugin::CPlugin(NPP pNPInstance, char *aCode, char* aURL) :
  m_pNPInstance(pNPInstance),
  m_pNPStream(NULL),
  m_bInitialized(false),
  m_pScriptableObject(NULL),
  mCode(m_strdup(aCode)),
  mURL(m_strdup(aURL))
{
#ifdef XP_WIN
  m_hWnd = NULL;
#endif

  NPN_GetValue(m_pNPInstance, NPNVWindowNPObject, &sWindowObj);

  NPIdentifier n = NPN_GetStringIdentifier("foof");

  sFoo_id = NPN_GetStringIdentifier("foo");
  sBar_id = NPN_GetStringIdentifier("bar");
  sDocument_id = NPN_GetStringIdentifier("document");
  sBody_id = NPN_GetStringIdentifier("body");
  sCreateElement_id = NPN_GetStringIdentifier("createElement");
  sCreateTextNode_id = NPN_GetStringIdentifier("createTextNode");
  sAppendChild_id = NPN_GetStringIdentifier("appendChild");
  sPluginType_id = NPN_GetStringIdentifier("PluginType");

  NPVariant v;
  INT32_TO_NPVARIANT(46, v);

  NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

  NPVariant rval;
  NPN_GetProperty(m_pNPInstance, sWindowObj, n, &rval);

  if (NPVARIANT_IS_INT32(rval)) {
    printf("rval = %d\n", NPVARIANT_TO_INT32(rval));
  }

  n = NPN_GetStringIdentifier("document");

  if (!NPN_IdentifierIsString(n)) {
    NPString str;
    str.UTF8Characters = "alert('NPN_IdentifierIsString() test failed!');";
    str.UTF8Length = strlen(str.UTF8Characters);

    NPN_Evaluate(m_pNPInstance, sWindowObj, &str, NULL);
  }

  NPObject *doc;

  NPN_GetProperty(m_pNPInstance, sWindowObj, n, &rval);

  if (NPVARIANT_IS_OBJECT(rval) && (doc = NPVARIANT_TO_OBJECT(rval))) {
    n = NPN_GetStringIdentifier("title");

    NPN_GetProperty(m_pNPInstance, doc, n, &rval);

    if (NPVARIANT_IS_STRING(rval)) {
      printf ("title = %s\n", NPVARIANT_TO_STRING(rval).UTF8Characters);

      NPN_ReleaseVariantValue(&rval);
    }

    n = NPN_GetStringIdentifier("plugindoc");

    OBJECT_TO_NPVARIANT(doc, v);
    NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

    NPString str;
    str.UTF8Characters = "document.getElementById('result').innerHTML += '<p>' + 'NPN_Evaluate() test, document = ' + this + '</p>';";
    str.UTF8Length = strlen(str.UTF8Characters);

    NPN_Evaluate(m_pNPInstance, doc, &str, NULL);

    NPN_ReleaseObject(doc);
  }

  NPVariant barval;
  NPN_GetProperty(m_pNPInstance, sWindowObj, sBar_id, &barval);

  NPVariant arg;
  OBJECT_TO_NPVARIANT(sWindowObj, arg);

  NPN_InvokeDefault(m_pNPInstance, NPVARIANT_TO_OBJECT(barval), &arg, 1,
                    &rval);

  if (NPVARIANT_IS_INT32(rval) && NPVARIANT_TO_INT32(rval) == 4) {
    printf ("Default function call SUCCEEDED!\n");
  } else {
    printf ("Default function call FAILED!\n");
  }

  NPN_ReleaseVariantValue(&barval);
  NPN_ReleaseVariantValue(&rval);


#if 0
  n = NPN_GetStringIdentifier("prompt");

  NPVariant vars[3];
  STRINGZ_TO_NPVARIANT("foo", vars[0]);
  STRINGZ_TO_NPVARIANT("bar", vars[1]);
  STRINGZ_TO_NPVARIANT("foof", vars[2]);

  NPN_Invoke(sWindowObj, n, vars, 3, &rval);

  if (NPVARIANT_IS_STRING(rval)) {
    printf ("prompt returned '%s'\n", NPVARIANT_TO_STRING(rval).UTF8Characters);
  }

  NPN_ReleaseVariantValue(&rval);
#endif

  NPObject *myobj =
    NPN_CreateObject(m_pNPInstance,
                     GET_NPOBJECT_CLASS(ScriptablePluginObject));

  n = NPN_GetStringIdentifier("pluginobj");

  OBJECT_TO_NPVARIANT(myobj, v);
  NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

  NPN_GetProperty(m_pNPInstance, sWindowObj, n, &rval);

  printf ("Object set/get test ");

  if (NPVARIANT_IS_OBJECT(rval) && NPVARIANT_TO_OBJECT(rval) == myobj) {
    printf ("succeeded!\n");
  } else {
    printf ("FAILED!\n");
  }

  NPN_ReleaseVariantValue(&rval);
  NPN_ReleaseObject(myobj);

  const char *ua = NPN_UserAgent(m_pNPInstance);
  strcpy(m_String, ua);
}

CPlugin::~CPlugin()
{
  if (sWindowObj)
    NPN_ReleaseObject(sWindowObj);
  if (m_pScriptableObject)
    NPN_ReleaseObject(m_pScriptableObject);

  sWindowObj = 0;
}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);
static WNDPROC lpOldProc = NULL;
#endif

NPBool CPlugin::init(NPWindow* pNPWindow)
{
  if(pNPWindow == NULL)
    return false;

#ifdef XP_WIN
  m_hWnd = (HWND)pNPWindow->window;
  if(m_hWnd == NULL)
    return false;

  // subclass window so we can intercept window messages and
  // do our drawing to it
  lpOldProc = SubclassWindow(m_hWnd, (WNDPROC)PluginWinProc);

  // associate window with our CPlugin object so we can access 
  // it in the window procedure
  SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
#endif

  m_Window = pNPWindow;

  m_bInitialized = true;
  return true;
}

void CPlugin::shut()
{
#ifdef XP_WIN
  // subclass it back
  SubclassWindow(m_hWnd, lpOldProc);
  m_hWnd = NULL;
#endif

  m_bInitialized = false;
}

NPBool CPlugin::isInitialized()
{
  return m_bInitialized;
}

int16_t CPlugin::handleEvent(void* event)
{
#ifdef XP_MAC
  NPEvent* ev = (NPEvent*)event;
  if (m_Window) {
    Rect box = { m_Window->y, m_Window->x,
                 m_Window->y + m_Window->height, m_Window->x + m_Window->width };
    if (ev->what == updateEvt) {
      ::TETextBox(m_String, strlen(m_String), &box, teJustCenter);
    }
  }
#endif
  return 0;
}

// this will force to draw a version string in the plugin window
void CPlugin::showVersion()
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  strcpy(m_String, ua);

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, true);
  UpdateWindow(m_hWnd);
#endif

  if (m_Window) {
    NPRect r =
      {
        (uint16_t)m_Window->y, (uint16_t)m_Window->x,
        (uint16_t)(m_Window->y + m_Window->height),
        (uint16_t)(m_Window->x + m_Window->width)
      };

    NPN_InvalidateRect(m_pNPInstance, &r);
  }
}

// this will clean the plugin window
void CPlugin::clear()
{
  strcpy(m_String, "");

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, true);
  UpdateWindow(m_hWnd);
#endif
}

void CPlugin::getVersion(char* *aVersion)
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  char*& version = *aVersion;
  version = (char*)NPN_MemAlloc(1 + strlen(ua));
  if (version)
    strcpy(version, ua);
}

NPObject *
CPlugin::GetScriptableObject()
{
  if (!m_pScriptableObject) {
    m_pScriptableObject =
      NPN_CreateObject(m_pNPInstance,
                       GET_NPOBJECT_CLASS(ScriptablePluginObject));
  }

  if (m_pScriptableObject) {
    NPN_RetainObject(m_pScriptableObject);
  }

  return m_pScriptableObject;
}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg) {
    case WM_PAINT:
      {
        // draw a frame and display the string
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        FrameRect(hdc, &rc, GetStockBrush(BLACK_BRUSH));
        CPlugin * p = (CPlugin *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if(p) {
          if (p->m_String[0] == 0) {
            strcpy("foo", p->m_String);
          }

          DrawText(hdc, p->m_String, strlen(p->m_String), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }

        EndPaint(hWnd, &ps);
      }
      break;
    default:
      break;
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif
