// 3dpoints.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "3dpoints.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
#include "common.h"

CDataTbl *DataTable=NULL;

/////////////////////////////////////////////////////////////////////////////
// C3DPointsDlg dialog
IMPLEMENT_DYNAMIC(C3DPointsDlg, CDialogBar)

BEGIN_MESSAGE_MAP(C3DPointsDlg, CDialogBar)
	//{{AFX_MSG_MAP(C3DPointsDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_CONTROL(DTN_LFCLICK, IDD_3DPOINTTABLE, OnGridLeftClick )
END_MESSAGE_MAP()


C3DPointsDlg::C3DPointsDlg(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(C3DPointsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



void C3DPointsDlg::AddRow( GridRowData *ptr)
{
	static int flag =0;
/*	AfxMessageBox("Sending a grid message");*/
/*	if( flag == 0 )
	{
		::SendMessage(DataTable->GetSafeHwnd(), DTM_SETACCESSPOS, 0, DTPOS_AFTERLAST);
		flag = 1;
	}
	::SendMessage(DataTable->GetSafeHwnd(), DTM_INSERTROW, DTA_ACCESS, (LPARAM)(LPVOID)ptr);
*/
	DataTable = (CDataTbl *)GetDlgItem(IDD_3DPOINTTABLE);
	DataTable->SetAccessPos(0,DTPOS_AFTERLAST);
	long row = DataTable->InsertRow(DTA_ACCESS, (LPVOID)ptr );

}

void C3DPointsDlg::OnSize(UINT nType, int cx, int cy) 
{
	RECT GridRect,rect,prect;
	HWND hWnd;
												 
	CDialogBar::OnSize(nType, cx, cy);

	DataTable = (CDataTbl *)GetWindow(GW_CHILD);			
	//CWnd
	TRACE("InONSIZE\n");
	if( DataTable != NULL )
	{
//		TRACE("The hWnd is %X\n", DataTbl->m_hWnd ); 
//		TRACE("The safe wnd is %X\n", GetDlgItem(IDD_SCSDATATBL)->GetSafeHwnd());
//		hWnd = ::GetDlgItem(m_hWnd, IDD_3DPOINTTABLE);
		::GetWindowRect(DataTable->GetSafeHwnd(),&GridRect );     
		GetWindowRect(&rect);
		GetParent()->GetWindowRect(&prect);
		TRACE("rect.bottom=%d, rect.top=%d, cx=%d, cy=%d, GridRect.top=%d, width=%d, height=%d\n", rect.bottom,rect.top, cx, cy, GridRect.top,cx-2,rect.bottom-GridRect.top-2 ); 
		cx = prect.right - prect.left;
		cy = prect.bottom - prect.top;
		if( cx > 0 && cy> 0 )     
		{	
			
			::SetWindowPos(this->GetSafeHwnd(),HWND_TOP,0,0,cx,cy,SWP_SHOWWINDOW|SWP_NOMOVE);
			::SetWindowPos(DataTable->GetSafeHwnd(),HWND_TOP,0,0,cx,cy,SWP_SHOWWINDOW|SWP_NOMOVE);
		}
		InvalidateRect(NULL);
	}       
	TRACE("WOW\n");
	ShowScrollBar(SB_BOTH, FALSE);
	TRACE("Leaving On Size\n");	
	// TODO: Add your message handler code here
}

void C3DPointsDlg::OnGridLeftClick()
{
	int row,col;

	DataTable = (CDataTbl *)GetWindow(GW_CHILD);			
	row = DataTable->GetNotifyRow();
	col = DataTable->GetNotifyCol();
	if( row == col )
		col=row;
}

BEGIN_EVENTSINK_MAP(C3DPointsDlg, CDialogBar)
    //{{AFX_EVENTSINK_MAP(C3DPointsDlg)
	ON_EVENT(C3DPointsDlg, IDC_3DPOINTGRID, 50 /* LfClick */, OnLfClick3dpointgrid, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void C3DPointsDlg::OnLfClick3dpointgrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status) 
{
	// TODO: Add your control notification handler code here
	
}
