#if !defined(AFX_HELLOWORLD_H__056EE1D5_AE1C_4634_8E68_1C17C2FFD8F7__INCLUDED_)
#define AFX_HELLOWORLD_H__056EE1D5_AE1C_4634_8E68_1C17C2FFD8F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// HelloWorld.h : main header file for HELLOWORLD.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHelloWorldApp : See HelloWorld.cpp for implementation.

class CHelloWorldApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLOWORLD_H__056EE1D5_AE1C_4634_8E68_1C17C2FFD8F7__INCLUDED)
