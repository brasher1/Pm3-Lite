// CircleDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "CircleDataDlg.h"
#include "ddxfxns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCircleDataDlg dialog


CCircleDataDlg::CCircleDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCircleDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCircleDataDlg)
	m_Radius = 0.0;
	m_East = 0.0;
	m_Height = 0.0;
	m_North = 0.0;
	//}}AFX_DATA_INIT
}


void CCircleDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCircleDataDlg)
	DDX_Length(pDX, IDC_RADIUS, m_Radius);
	DDX_Length(pDX, IDC_START_EAST, m_East);
	DDX_Length(pDX, IDC_START_HEIGHT, m_Height);
	DDX_Length(pDX, IDC_START_NORTH, m_North);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCircleDataDlg, CDialog)
	//{{AFX_MSG_MAP(CCircleDataDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCircleDataDlg message handlers
void CCircleDataDlg::setValues(double radius, double height, double *endpoints)
{
	m_Height = height;
	m_Radius = radius;
	m_East = endpoints[3];
	m_Height = endpoints[5];
	m_North = endpoints[4];

//	UpdateData(FALSE);

}

void CCircleDataDlg::getValues(double *radius, double *height, double *endpoints)
{
//	UpdateData(TRUE);

	endpoints[0] = m_East;
	endpoints[2] = m_Height;
	endpoints[1] = m_North;
	*height = m_Height;
	*radius = m_Radius;
}