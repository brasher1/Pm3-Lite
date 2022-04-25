// columnli.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "columnli.h"

#include <datatbl.h>
#include <cdatatbl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColumnLinesDlg dialog


CColumnLinesDlg::CColumnLinesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColumnLinesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColumnLinesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CColumnLinesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnLinesDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColumnLinesDlg, CDialog)
	//{{AFX_MSG_MAP(CColumnLinesDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColumnLinesDlg message handlers

void CColumnLinesDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RECT GridRect,rect;
	HWND hWnd;

	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_COLUMNTABLE);			
	if( DataTbl==NULL )
	{
		return;
	}

	if( DataTbl != NULL )
	{
		hWnd = ::GetDlgItem(DataTbl->GetSafeHwnd(), IDD_COLUMNTABLE);
		::GetWindowRect(DataTbl->GetSafeHwnd(),&GridRect );     
		GetWindowRect(&rect);
		TRACE("rect.bottom=%d, GridRect.top=%d, width=%d, height=%d\n", rect.bottom,GridRect.top,cx-2,rect.bottom-GridRect.top-2 ); 
		if( cx > 0 && cy> 0 )     
		{	
			::SetWindowPos(DataTbl->GetSafeHwnd(),HWND_TOP,0,0,cx,cy,SWP_SHOWWINDOW);
		}
		InvalidateRect(NULL);
	}       
	ShowScrollBar(SB_BOTH, FALSE);
	
}								

BOOL CColumnLinesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UINT temp1;	
	// TODO: Add extra initialization here
	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_COLUMNTABLE);			
	if( DataTbl==NULL )
	{
		AfxMessageBox("BOMBED ON GetDlgItem");	
		return FALSE;
	}
	temp1=DataTbl->GetWndStyle();	    //CWnd
	DataTbl->SetColWidth(DTPOS_LABELS, 16);
	DataTbl->SetWndStyle(WS_VSCROLL|WS_HSCROLL|temp1|DTS_LARGEDATA);
	DataTbl->SetWndStyle(WS_MAXIMIZE|(~WS_VISIBLE)&(WS_VSCROLL|WS_HSCROLL|temp1|DTS_LARGEDATA));
	DataTbl->SetAccessPos(0,0);     
	
	::SetWindowPos(this->GetSafeHwnd(), HWND_TOP, 0,0,(16+(45*6)+80+3)*2,200,SWP_NOMOVE);

	for(int i=0; i<20; i++)
		DataTbl->InsertRow(DTA_CURRENT, NULL);		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	   
