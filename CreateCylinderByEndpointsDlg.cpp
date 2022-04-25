// CreateCylinderByEndpointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "CreateCylinderByEndpointsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateCylinderByEndpointsDlg dialog


CCreateCylinderByEndpointsDlg::CCreateCylinderByEndpointsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateCylinderByEndpointsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateCylinderByEndpointsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCreateCylinderByEndpointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateCylinderByEndpointsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateCylinderByEndpointsDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateCylinderByEndpointsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateCylinderByEndpointsDlg message handlers
