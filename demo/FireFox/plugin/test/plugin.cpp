/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
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
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */


#include <windows.h>
#include <windowsx.h>

#include "plugin.h"
#include "..\TPM_test\TPM_test.h"
#include "..\TPM_test_d\TPM_test_d.h"
#include <stdio.h>

#define MIME_TYPES_HANDLED	"application/basic-plugin"
#define PLUGIN_NAME "Basic Firefox Plugin"
#define PLUGIN_DESCRIPTION "Basic Firefox Plugin for test"

//////////////////////////////////////
//
// general initialization and shutdown
//
NPError NS_PluginInitialize()
{
  return NPERR_NO_ERROR;
}

void NS_PluginShutdown()
{
}

/////////////////////////////////////////////////////////////
//
// construction and destruction of our plugin instance object
//
nsPluginInstanceBase * NS_NewPluginInstance(nsPluginCreateData * aCreateDataStruct)
{
  if(!aCreateDataStruct)
    return NULL;

  nsPluginInstance * plugin = new nsPluginInstance(aCreateDataStruct->instance, aCreateDataStruct->argc, aCreateDataStruct->argn, aCreateDataStruct->argv);
  return plugin;
}

void NS_DestroyPluginInstance(nsPluginInstanceBase * aPlugin)
{
  if(aPlugin)
    delete (nsPluginInstance *)aPlugin;
}

////////////////////////////////////////
//
// nsPluginInstance class implementation
//
nsPluginInstance::nsPluginInstance(NPP aInstance, int16_t argc, char **argn, char **argv) : nsPluginInstanceBase(),
  mInstance(aInstance),
  mInitialized(FALSE),
  mCode(FALSE),
  mSrc(FALSE)
{
  mhWnd = NULL;
  mArgc = argc;
  mKey = NULL;
  mURL = NULL;
  for (int i = 0;i < argc; i++) {
	  if (!strcmp(argn[i],"code")) {
		  mCode = TRUE;
		  int len = strlen(argv[i]);
		  mKey = (char*) NPN_MemAlloc(len+1);
		  strncpy(mKey,argv[i],len);
		  mKey[len] = '\0';
	  }
	  else if (!strcmp(argn[i],"src")) {
		  mSrc = TRUE;
		  int len = strlen(argv[i]);
		  mURL = (char*) NPN_MemAlloc(len+1);
		  strncpy(mURL,argv[i],len);
		  mURL[len] = '\0';
	  }
  }
}

nsPluginInstance::~nsPluginInstance()
{
	if (mKey != NULL) {
		NPN_MemFree(mKey);
		mKey = NULL;
	}
	if (mURL != NULL) {
		NPN_MemFree(mURL);
		mURL = NULL;
	}
}

static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);
static WNDPROC lpOldProc = NULL;

NPBool nsPluginInstance::init(NPWindow* aWindow)
{
  if(aWindow == NULL)
    return FALSE;

  mhWnd = (HWND)aWindow->window;
  if(mhWnd == NULL)
    return FALSE;

  // subclass window so we can intercept window messages and
  // do our drawing to it
  lpOldProc = SubclassWindow(mhWnd, (WNDPROC)PluginWinProc);
  /*
  SubclassWindow(hwnd, lpfn) => SetWindowLongPtr(hwnd,GWLP_WNDPROC,lpfn)
  GWLP_WNDPROC:
  Sets a new address for the window procedure.
  store previous value to lpOldProc
  */

  // associate window with our nsPluginInstance object so we can access 
  // it in the window procedure
  SetWindowLongPtr(mhWnd, GWLP_USERDATA, (LONG_PTR)this);
  /*
  mhWnd:
	Handle to the window and, indirectly, the class to which the window belongs.
  GWL_USERDATA:
Sets the user data associated with the window. This data is intended for use by the application 
that created the window. Its value is initially zero.
  (LONG_PTR) this -> store (this) into WINDOW's data, so can use GetWindowLong to get (this)
  */

  mInitialized = TRUE;

  this->StreamTest();
  this->GetTest();
  this->PostTest();
  return TRUE;
}

void nsPluginInstance::shut()
{
  // subclass it back
  SubclassWindow(mhWnd, lpOldProc);
  mhWnd = NULL;
  mInitialized = FALSE;
}

NPBool nsPluginInstance::isInitialized()
{
  return mInitialized;
}

const char * nsPluginInstance::getVersion()
{
  return NPN_UserAgent(mInstance);
}

void CopyRECT(RECT* src, RECT* dst) {
	dst->bottom = src->bottom;
	dst->left = src->left;
	dst->top = src->top;
	dst->right = src->right;
}

static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg) {
    case WM_PAINT:
      {
	    char *string, *name, *ver;

		// draw a frame and display the string
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);//Prepares the window for painting
        RECT rc, oldrc;
        GetClientRect(hWnd, &oldrc);
		/*
		The GetClientRect function retrieves the coordinates of a window's client area. The client coordinates specify the upper-left and lower-right corners of the client area. Because client coordinates are relative to the upper-left corner of a window's client area, the coordinates of the upper-left corner are (0,0). 
	hWnd [in] Handle to the window whose client coordinates are to be retrieved. 
	lpRect [out] Pointer to a RECT structure that receives the client coordinates. The left and top members are zero. The right and bottom members contain the width and height of the window. 
		*/

        FrameRect(hdc, &oldrc, GetStockBrush(GRAY_BRUSH));
		/*
		Draws a border around the rectangle specified by lpRect.
		lpRect : Points to a RECT structure or CRect object that contains the logical coordinates of the upper-left and lower-right corners of the rectangle. You can also pass a CRect object for this parameter.
		pBrush : Identifies the brush to be used for framing the rectangle.
		*/

        // get our plugin instance object
        nsPluginInstance *plugin = (nsPluginInstance *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!plugin) {
          string = "Error occured";
          DrawText(hdc, string, -1, &oldrc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		  break;
        }
		TPM_test::TPM_test t = TPM_test::TPM_test();
		TPM_test_d::TPM_test_d td = TPM_test_d::TPM_test_d();
		if (t.isTPM() && td.isTPM()) {
			string="TPM library is available";
		}
		else {
			string="TPM library is not available";
		}
		CopyRECT(&oldrc,&rc);
		rc.bottom = (rc.top * 2 + rc.bottom) / 3;
		rc.right = (rc.left + rc.right) / 2;
		SetTextColor(hdc,0x00ff0000);
        DrawText(hdc, string, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		
		string = "Code not specified";
		if (plugin->mCode) {
			int len=strlen(plugin->mKey);
			string = (char *) NPN_MemAlloc(len + 10);
			sprintf(string,"Code = %s",plugin->mKey);
			string[len+9]='\0';
		}
		CopyRECT(&oldrc,&rc);
		rc.bottom = (rc.top * 2 + rc.bottom) / 3;
		rc.left = (rc.left + rc.right) / 2;
		SetTextColor(hdc,0x00ff0000);
        DrawText(hdc, string, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		if (plugin->mCode) {
			NPN_MemFree(string);
		}

		int nlen = strlen(t.getName());
		name = (char*) NPN_MemAlloc(nlen+1);
		strncpy(name,t.getName(),nlen);
		name[nlen] = '\0';
		CopyRECT(&oldrc,&rc);
		rc.top = (rc.top * 2 + rc.bottom) / 3;
		rc.bottom = (rc.top + rc.bottom * 2) / 3;
		rc.right = (rc.left + rc.right) / 2;
		SetTextColor(hdc,0x0000ff00);
        DrawText(hdc, name, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		NPN_MemFree(name);

		nlen = strlen(td.getName());
		name = (char*) NPN_MemAlloc(nlen+1);
		strncpy(name,td.getName(),nlen);
		name[nlen] = '\0';
		CopyRECT(&oldrc,&rc);
		rc.top = (rc.top * 2 + rc.bottom) / 3;
		rc.bottom = (rc.top + rc.bottom * 2) / 3;
		rc.left = (rc.left + rc.right) / 2;
		SetTextColor(hdc,0x0000ff00);
        DrawText(hdc, name, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		NPN_MemFree(name);

		int vlen = strlen(t.getVersion());
		ver = (char*) NPN_MemAlloc(vlen+1);
		strncpy(ver,t.getVersion(),vlen);
		ver[vlen] = '\0';
		CopyRECT(&oldrc,&rc);
		rc.top = (rc.top + rc.bottom * 2) / 3;
		rc.right = (rc.left + rc.right) / 2;
		SetTextColor(hdc,0x000000ff);
        DrawText(hdc, ver, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		NPN_MemFree(ver);

		vlen = strlen(td.getVersion());
		ver = (char*) NPN_MemAlloc(vlen+1);
		strncpy(ver,td.getVersion(),vlen);
		ver[vlen] = '\0';
		CopyRECT(&oldrc,&rc);
		rc.top = (rc.top + rc.bottom * 2) / 3;
		rc.left = (rc.left + rc.right) / 2;
		SetTextColor(hdc,0x000000ff);
        DrawText(hdc, ver, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		NPN_MemFree(ver);
		/*
		Call this member function to format text in the given rectangle
	lpszString : Points to the string to be drawn. If nCount is ¨C1, the string must be null-terminated.
	nCount : Specifies the number of chars in the string. If nCount is ¨C1, then lpszString is assumed to be a long pointer to a null-terminated string and DrawText computes the character count automatically.
	lpRect : Points to a RECT structure or CRect object that contains the rectangle (in logical coordinates) in which the text is to be formatted.
	str : A CString object that contains the specified characters to be drawn.
	nFormat : Specifies the method of formatting the text. It can be any combination of the values described for the uFormat parameter in DrawText in the Windows SDK. (combine using the bitwise OR operator):
		*/

		EndPaint(hWnd, &ps);//Marks the end of painting
      }
      break;
    default:
      break;
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
  /*
  The DefWindowProc function calls the default window procedure to provide default processing for any window messages that an application does not process. This function ensures that every message is processed. DefWindowProc is called with the same parameters received by the window procedure.
	hWnd [in] Handle to the window procedure that received the message. 
	Msg [in] Specifies the message. 
	wParam [in] Specifies additional message information. The content of this parameter depends on the value of the Msg parameter. 
	lParam [in] Specifies additional message information. The content of this parameter depends on the value of the Msg parameter. 
  */
}

NPError nsPluginInstance::StreamTest() {
  NPError err;
  NPStream* stream;

  char* myData = "<html><head><title>This is a message from my plug-in!</title></head><body><p><strong>This is a message from my plug-in!</strong></p></body></html>";
  int32_t myLength = strlen(myData) + 1;
  
  // Create the stream
  err = NPN_NewStream(this->mInstance, "text/html", "_blank", &stream);
  if (err != NPERR_NO_ERROR)
	  return err;
  // Push data into the stream
  err = NPN_Write(this->mInstance, stream, myLength, myData);
  if (err != NPERR_NO_ERROR)
	  return err;
 
  // Delete the stream. 
  err = NPN_DestroyStream(this->mInstance, stream, NPRES_DONE);
  return err;
}

NPError nsPluginInstance::PostTest() {
	if (!mSrc) 
		return NPERR_INVALID_URL;
	if (!mCode)
		return NPERR_GENERIC_ERROR;
	NPError err;
	char *buf = "654321";
	int16_t len = strlen(buf);
	err = NPN_PostURL(this->mInstance, this->mURL, "_blank", len+1, buf, FALSE);
	return err;
}

/* Drawing: see CDC Members IN MSDN*/
/*TODO:
NPError NPN_GetURL(NPP instance, const char *url, const char *target);
retrieve data in URL

NPError NPN_PostURL(NPP instance, const char *url, const char *target, uint32 len, const char 
*buf, NPBool file);
send data to URL

The target parameter represents the destination where the URL will be displayed, a window or 
frame. If target refers to the window or frame containing the plug-in instance, it is destroyed 
and the plug-in may be unloaded. If the target parameter is set to null, the application 
creates a new stream and delivers the data to the plug-in instance, through calls to 
NPP_NewStream , NPP_WriteReady  and NPP_Write , and NPP_DestroyStream .

*/
NPError nsPluginInstance::GetTest() {
	if (!mSrc) 
		return NPERR_INVALID_URL;
	if (!mCode)
		return NPERR_GENERIC_ERROR;
	NPError err;
	err = NPN_GetURL(this->mInstance, this->mURL, "_blank");
	return err;
}
