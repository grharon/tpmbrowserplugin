#if !defined(AFX_HELLOWORLDPPG_H__728B068F_8499_476F_95DF_6C889BB4CDCA__INCLUDED_)
#define AFX_HELLOWORLDPPG_H__728B068F_8499_476F_95DF_6C889BB4CDCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// HelloWorldPpg.h : Declaration of the CHelloWorldPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CHelloWorldPropPage : See HelloWorldPpg.cpp.cpp for implementation.

class CHelloWorldPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CHelloWorldPropPage)
	DECLARE_OLECREATE_EX(CHelloWorldPropPage)

// Constructor
public:
	CHelloWorldPropPage();

// Dialog Data
	//{{AFX_DATA(CHelloWorldPropPage)
	enum { IDD = IDD_PROPPAGE_HELLOWORLD };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CHelloWorldPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLOWORLDPPG_H__728B068F_8499_476F_95DF_6C889BB4CDCA__INCLUDED)
