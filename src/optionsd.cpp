// optionsd.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "optionsd.h"
#include "mainfrm.h"
#include "regfuncs.h"

#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_Units = -1;
	//}}AFX_DATA_INIT
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Radio(pDX, IDC_UNITS, m_Units);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

void COptionsDlg::OnOK() 
{
	UpdateData(TRUE);
	CFotobrowApp *pApp = (CFotobrowApp *)AfxGetApp();
	
	saveUnits(m_Units);
	pApp->SetUnits(m_Units);

	AfxGetApp()->m_pMainWnd->SendMessage( UM_UNITS_CHANGE, (WPARAM)NULL,(LPARAM)NULL );

//	((CMainFrame *)(pApp->m_pMainWnd))->RepaintMeasurements();
	CDialog::OnOK();
}

BOOL COptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Units = getUnits();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
