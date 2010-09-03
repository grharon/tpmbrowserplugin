#if !defined(AFX_HELLOWORLDCTL_H__29856C15_5172_433B_B795_687B90E8EF3A__INCLUDED_)
#define AFX_HELLOWORLDCTL_H__29856C15_5172_433B_B795_687B90E8EF3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// HelloWorldCtl.h : Declaration of the CHelloWorldCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CHelloWorldCtrl : See HelloWorldCtl.cpp for implementation.

class CHelloWorldCtrl : public COleControl
{
	DECLARE_DYNCREATE(CHelloWorldCtrl)

// Constructor
public:
	CHelloWorldCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelloWorldCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CHelloWorldCtrl();

	DECLARE_OLECREATE_EX(CHelloWorldCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CHelloWorldCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CHelloWorldCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CHelloWorldCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CHelloWorldCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CHelloWorldCtrl)
	afx_msg BSTR Show();
	afx_msg BSTR getPublicKeyContent();
	afx_msg BSTR doSignature(BSTR,BSTR);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CHelloWorldCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CHelloWorldCtrl)
	dispidShow = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLOWORLDCTL_H__29856C15_5172_433B_B795_687B90E8EF3A__INCLUDED)
