// transfor.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "common.h"
#include "transfor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransformDlg dialog


CTransformDlg::CTransformDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransformDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransformDlg)
	m_Trans0 = 0.0;
	m_Trans1 = 0.0;
	m_Trans2 = 0.0;
	m_Trans3 = 0.0;
	m_Trans4 = 0.0;
	m_Trans5 = 0.0;
	m_Trans6 = 0.0;
	m_Trans7 = 0.0;
	m_Trans8 = 0.0;
	m_Trans9 = 0.0;
	m_Trans10 = 0.0;
	m_Trans11 = 0.0;
	//}}AFX_DATA_INIT
}


void CTransformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransformDlg)
	DDX_Text(pDX, IDC_MATRIX_ELEMENT0, m_Trans0);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT1, m_Trans1);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT2, m_Trans2);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT3, m_Trans3);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT4, m_Trans4);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT5, m_Trans5);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT6, m_Trans6);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT7, m_Trans7);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT8, m_Trans8);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT9, m_Trans9);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT10, m_Trans10);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT11, m_Trans11);
	//}}AFX_DATA_MAP
/*
	DDX_Text(pDX, IDC_MATRIX_ELEMENT0, ((CFotobrowApp *)AfxGetApp())->m_Transform[0]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT1, ((CFotobrowApp *)AfxGetApp())->m_Transform[1]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT2, ((CFotobrowApp *)AfxGetApp())->m_Transform[2]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT3, ((CFotobrowApp *)AfxGetApp())->m_Transform[3]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT4, ((CFotobrowApp *)AfxGetApp())->m_Transform[4]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT5, ((CFotobrowApp *)AfxGetApp())->m_Transform[5]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT6, ((CFotobrowApp *)AfxGetApp())->m_Transform[6]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT7, ((CFotobrowApp *)AfxGetApp())->m_Transform[7]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT8, ((CFotobrowApp *)AfxGetApp())->m_Transform[8]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT9, ((CFotobrowApp *)AfxGetApp())->m_Transform[9]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT10, ((CFotobrowApp *)AfxGetApp())->m_Transform[10]);
	DDX_Text(pDX, IDC_MATRIX_ELEMENT11, ((CFotobrowApp *)AfxGetApp())->m_Transform[11]);
*/
}


BEGIN_MESSAGE_MAP(CTransformDlg, CDialog)
	//{{AFX_MSG_MAP(CTransformDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTransformDlg message handlers

BOOL CTransformDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	double transform[12];
	getSurveyModelTransform((double *)&transform, m_ProjectID);
	m_Trans0 = transform[0];
	m_Trans1 = transform[1];
	m_Trans2 = transform[2];
	m_Trans3 = transform[3];
	m_Trans4 = transform[4];
	m_Trans5 = transform[5];
	m_Trans6 = transform[6];
	m_Trans7 = transform[7];
	m_Trans8 = transform[8];
	m_Trans9 = transform[9];
	m_Trans10 = transform[10];
	m_Trans11 = transform[11];
	UpdateData(FALSE);
/////////////////////////////////////////////////////////////pppppp
/////////////////////////////////////////////////////////////pppppp	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTransformDlg::OnOK() 
{
	UpdateData(TRUE);
	double transform[12];
///////////////////////
	transform[0] = m_Trans0;
	transform[1] = m_Trans1;
	transform[2] = m_Trans2;
	transform[3] = m_Trans3;
	transform[4] = m_Trans4;
	transform[5] = m_Trans5;
	transform[6] = m_Trans6;
	transform[7] = m_Trans7;
	transform[8] = m_Trans8;
	transform[9] = m_Trans9;
	transform[10] = m_Trans10;
	transform[11] = m_Trans11;

	saveSurveyModelTransform((double *)&transform, m_ProjectID);
////////////////////////	
	CDialog::OnOK();
}

void CTransformDlg::setProjectID(const char *string) 
{
	m_ProjectID = string;
//	AfxMessageBox(string);
}
