// assigneq.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "assigneq.h"
#include "common.h"

#include <transact.hpp>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssignEquipDlg dialog


CAssignEquipDlg::CAssignEquipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssignEquipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssignEquipDlg)
	m_EquipString = _T("");
	//}}AFX_DATA_INIT
}


void CAssignEquipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssignEquipDlg)
	DDX_Control(pDX, IDC_ASSIGN_EQUIP, m_AssignEquipBox);
	DDX_CBString(pDX, IDC_ASSIGN_EQUIP, m_EquipString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAssignEquipDlg, CDialog)
	//{{AFX_MSG_MAP(CAssignEquipDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAssignEquipDlg message handlers

BOOL CAssignEquipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	rvqTask->MakeCurrent();
	
	equipObj eq;
	eq.ReadLock();
	for( eq.First(); eq.Okay(); eq.Next() )
	{
		m_AssignEquipBox.AddString(eq.name);				
	}	
	eq.FreeLock();


//	char temp[128];
//	for( int i=0; i<100000; i++ )
//	{
//		sprintf(temp,"WOW - %d", i);
//		m_AssignEquipBox.AddString(temp);
//	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAssignEquipDlg::SetImageData( CRect &r, long imagedba )
{
	m_box = r;
	dba = imagedba;
}

void CAssignEquipDlg::OnOK() 
{
	UpdateData(TRUE);

	rvqTask->MakeCurrent();
	image_ptrObj ip;
	TransAction a;//WNetGetUser
	a.Begin("wow");
	ip.WriteLock();
	ip.GotoDirect(DbAddr(dba));
	equipObj eq;
	eq.WriteLock();
	eq.Find(KeyObj(EQUIP_NAME, (Pvoid)(const char *)m_EquipString));
	if( eq.Okay() )
	{
		switch( AfxMessageBox("Equipment Entered Already Exists - Create Link??", MB_YESNO ) )
		{
			case IDYES:
				break;
			case IDNO:
				a.End();
				return;
				break;		
		}
	}
	else
	{
		strcpy( eq.name, m_EquipString );

		eq.NewObj();
	}
	image_equip_boxObj ipei;
	
	ipei.left = m_box.left;
	ipei.right = m_box.right;
	ipei.top = m_box.top;
	ipei.bottom = m_box.bottom;
	ipei.WriteLock();
	ipei.NewObj();
	char temp[128];

	if( !(ipei.Connect(ip)))
	{
		sprintf(temp, "Couldn't connect to image_ptr - %d", ipei.GetStatus() );
		AfxMessageBox(temp);
	}
	if( !(ipei.Connect(eq)))
	{
		sprintf(temp, "Couldn't connect to equip_ptr - %d", ipei.GetStatus() );
		AfxMessageBox(temp);
	}

	a.End();
	CDialog::OnOK();
}
