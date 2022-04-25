// measured.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "measured.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasureDlg dialog


CMeasureDlg::CMeasureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasureDlg::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CMeasureDlg)
	m_Distance = _T("");
	m_Position1 = _T("");
	m_Position2 = _T("");
	//}}AFX_DATA_INIT
}


void CMeasureDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	//	OnFinalRelease is called.  This implementation deletes the 
	//	object.  Add additional cleanup required for your object before
	//	deleting it from memory.

	delete this;
}

void CMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasureDlg)
	DDX_Text(pDX, IDC_DISTANCE, m_Distance);
	DDX_Text(pDX, IDC_POSITION1, m_Position1);
	DDX_Text(pDX, IDC_POSITION2, m_Position2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasureDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasureDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMeasureDlg, CDialog)
	//{{AFX_DISPATCH_MAP(CMeasureDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasureDlg message handlers
