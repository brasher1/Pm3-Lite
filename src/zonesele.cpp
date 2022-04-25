// zonesele.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "zonesele.h"
#include "fotoview.h"
#include "common.h"

#include "../triton/triton.hpp"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZoneSelectDlg dialog


CZoneSelectDlg::CZoneSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoneSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZoneSelectDlg)
	m_ZoneString = _T("");
	//}}AFX_DATA_INIT
}


void CZoneSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoneSelectDlg)
	DDX_Control(pDX, IDC_ZONELIST, m_ZoneList);
	DDX_LBString(pDX, IDC_ZONELIST, m_ZoneString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoneSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CZoneSelectDlg)
	ON_LBN_DBLCLK(IDC_ZONELIST, OnDblclkZonelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CZoneSelectDlg message handlers

void CZoneSelectDlg::OnDblclkZonelist() 
{
//	int i;
	DWORD dba = m_ZoneList.GetItemData( m_ZoneList.GetCurSel() );		
	c.GotoDirect(DbAddr(dba));
//	AfxMessageBox(c.class_name);
	p.FromMember(&c,PROPERTIES_SET);
	if( !p.Okay() )
	{
		AfxMessageBox("No zone range record");
		return;
	} 
	SendClipMessage((long)(p.GetDba()));
}

BOOL CZoneSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for(c.First(); c.Okay(); c.Next() )
	{
		if( c.type == ZONE )
			InsertRow();		
	}
	UpdateData(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CZoneSelectDlg::InsertRow() 
{
	int i;

	p.FromMember(&c,PROPERTIES_SET);
	if( !p.Okay() )
	{
		AfxMessageBox("No zone range record");
		return FALSE;
	} 
	i = m_ZoneList.AddString( c.name );
	if( i >= 0 )
	{
		m_ZoneList.SetItemData( i, (DWORD)c.GetDba() );
	}		
	return TRUE;
}

void CZoneSelectDlg::SendClipMessage( long dba )
{

	CDocument *pDoc;
	CObList *pList = new CObList;
	int flag = FALSE;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
	TRACE("Got the DOC List\n");
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		TRACE("Got the next DOC\n");
		pDoc = (CDocument *)(pList->GetNext(pos));
	   	POSITION pos = pDoc->GetFirstViewPosition();
	   	while (pos != NULL)
	   	{
	  		CView* pView = pDoc->GetNextView(pos);
	      	//pView->UpdateWindow();
			if( pView->IsKindOf( RUNTIME_CLASS( CFotoView) ) )
			{
				pView->PostMessage( UM_NEW_CLIP_VOLUME, 0, dba );  
			}
	   	}   
	}
	while( pList->GetCount() )
		pList->RemoveTail();
	delete pList;
}

void CZoneSelectDlg::OnOK() 
{
///////////////////////////////
//	int i;
	int index;
	index = m_ZoneList.GetCurSel();
	if( index == LB_ERR )
	{
		AfxMessageBox("You must make a choice");
		return;	
	}
	DWORD dba = m_ZoneList.GetItemData( index );		
	c.GotoDirect(DbAddr(dba));
//	AfxMessageBox(c.class_name);
	p.FromMember(&c,PROPERTIES_SET);
	if( !p.Okay() )
	{
		AfxMessageBox("No zone range record");
		return;
	} 
	SendClipMessage((long)(p.GetDba()));
///////////////////////////////		
	CDialog::OnOK();
}
