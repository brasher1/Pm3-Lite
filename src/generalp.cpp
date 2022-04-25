// generalp.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "generalp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPageDlg property page

IMPLEMENT_DYNCREATE(CGeneralPageDlg, CPropertyPage)

CGeneralPageDlg::CGeneralPageDlg() : CPropertyPage(CGeneralPageDlg::IDD)
{
	//{{AFX_DATA_INIT(CGeneralPageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGeneralPageDlg::~CGeneralPageDlg()
{
}

void CGeneralPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralPageDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPageDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGeneralPageDlg message handlers
