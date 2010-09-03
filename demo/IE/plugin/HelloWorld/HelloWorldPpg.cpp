// HelloWorldPpg.cpp : Implementation of the CHelloWorldPropPage property page class.

#include "stdafx.h"
#include "HelloWorld.h"
#include "HelloWorldPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CHelloWorldPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CHelloWorldPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CHelloWorldPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CHelloWorldPropPage, "HELLOWORLD.HelloWorldPropPage.1",
	0x3d4a81f7, 0x254c, 0x48a6, 0xa1, 0x77, 0x40, 0x34, 0xaf, 0xa7, 0x65, 0x17)


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldPropPage::CHelloWorldPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CHelloWorldPropPage

BOOL CHelloWorldPropPage::CHelloWorldPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_HELLOWORLD_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldPropPage::CHelloWorldPropPage - Constructor

CHelloWorldPropPage::CHelloWorldPropPage() :
	COlePropertyPage(IDD, IDS_HELLOWORLD_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CHelloWorldPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldPropPage::DoDataExchange - Moves data between page and properties

void CHelloWorldPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CHelloWorldPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CHelloWorldPropPage message handlers
