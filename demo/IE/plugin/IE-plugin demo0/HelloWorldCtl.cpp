// HelloWorldCtl.cpp : Implementation of the CHelloWorldCtrl ActiveX Control class.

#include "stdafx.h"
#include "HelloWorld.h"
#include "HelloWorldCtl.h"
#include "HelloWorldPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CHelloWorldCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CHelloWorldCtrl, COleControl)
	//{{AFX_MSG_MAP(CHelloWorldCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CHelloWorldCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CHelloWorldCtrl)
	DISP_FUNCTION(CHelloWorldCtrl, "Show", Show, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CHelloWorldCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CHelloWorldCtrl, COleControl)
	//{{AFX_EVENT_MAP(CHelloWorldCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CHelloWorldCtrl, 1)
	PROPPAGEID(CHelloWorldPropPage::guid)
END_PROPPAGEIDS(CHelloWorldCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CHelloWorldCtrl, "HELLOWORLD.HelloWorldCtrl.1",
	0x1fd70071, 0x431f, 0x44aa, 0x88, 0xb4, 0xec, 0xd3, 0x33, 0x55, 0xe7, 0x7c)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CHelloWorldCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DHelloWorld =
		{ 0x9a6f77a6, 0x15f8, 0x4441, { 0x95, 0x2b, 0x89, 0x10, 0xb4, 0xcf, 0x1, 0xe5 } };
const IID BASED_CODE IID_DHelloWorldEvents =
		{ 0xa870bf40, 0x9e35, 0x4db5, { 0x89, 0x2, 0xbc, 0x9e, 0x7d, 0x95, 0xa5, 0x43 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwHelloWorldOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CHelloWorldCtrl, IDS_HELLOWORLD, _dwHelloWorldOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldCtrl::CHelloWorldCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CHelloWorldCtrl

BOOL CHelloWorldCtrl::CHelloWorldCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_HELLOWORLD,
			IDB_HELLOWORLD,
			afxRegApartmentThreading,
			_dwHelloWorldOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldCtrl::CHelloWorldCtrl - Constructor

CHelloWorldCtrl::CHelloWorldCtrl()
{
	InitializeIIDs(&IID_DHelloWorld, &IID_DHelloWorldEvents);

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldCtrl::~CHelloWorldCtrl - Destructor

CHelloWorldCtrl::~CHelloWorldCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldCtrl::OnDraw - Drawing function

void CHelloWorldCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);
	pdc->SetBkMode(TRANSPARENT);
}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldCtrl::DoPropExchange - Persistence support

void CHelloWorldCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldCtrl::OnResetState - Reset control to default state

void CHelloWorldCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldCtrl::AboutBox - Display an "About" box to the user

void CHelloWorldCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_HELLOWORLD);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldCtrl message handlers

BSTR CHelloWorldCtrl::Show() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	strResult="123456789";
	return strResult.AllocSysString();
}
