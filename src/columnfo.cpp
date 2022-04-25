// columnfo.cpp : implementation file
//

/************************* Temporary hack during datatable switch *******/
#if FALSE
/************************* Temporary hack during datatable switch *******/

#include "stdafx.h"
#include "fotobrow.h"
#include "columnfo.h"
#include <datatbl.h>
//#include <cdatatbl.h>
//#include <dtmfc.h>
#include <transact.hpp>

#include "common.h"


#include "mathsubs.h"
#include "photog_math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

double getInches( char *inputStr );
/////////////////////////////////////////////////////////////////////////////
// CColumnFormView

IMPLEMENT_DYNCREATE(CColumnFormView, CFormView)

CColumnFormView::CColumnFormView()
	: CFormView(CColumnFormView::IDD)
{
	//{{AFX_DATA_INIT(CColumnFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CColumnFormView::~CColumnFormView()
{
}

void CColumnFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColumnFormView, CFormView)
	//{{AFX_MSG_MAP(CColumnFormView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_DELETE_COLUMN, OnDeleteColumn)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
	ON_CONTROL(DTN_UPDATE, IDD_COLUMNTABLE, OnDataUpdate )
	ON_CONTROL(DTN_CHANGE, IDD_COLUMNTABLE, OnDataChange )
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CColumnFormView diagnostics

#ifdef _DEBUG
void CColumnFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CColumnFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CColumnFormView message handlers

void CColumnFormView::OnInitialUpdate() 
{
	UINT temp1;	
	// TODO: Add extra initialization here
	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_COLUMNTABLE);			
	if( DataTbl==NULL )
	{
		AfxMessageBox("BOMBED ON GetDlgItem");	
		return;
	}
	temp1=DataTbl->GetWndStyle();	    //CWnd
	DataTbl->SetColWidth(DTPOS_LABELS, 16);
	DataTbl->SetWndStyle(WS_VSCROLL|WS_HSCROLL|temp1|DTS_LARGEDATA);
	DataTbl->SetWndStyle(WS_MAXIMIZE|(~WS_VISIBLE)&(WS_VSCROLL|WS_HSCROLL|temp1|DTS_LARGEDATA));
///////////////////////////////////////////////////////////////////////////////////
/*	if( DataTable->SetWndStyle(WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|WS_HSCROLL|
			DTS_OWNERDEF|DTS_EDIT|DTS_RESIZE|
			DTS_ROWNUMBERS|DTS_VGRID|DTS_HGRID) != DTE_OKAY )
		AfxMessageBox("SetWndStyle Failed");*/
	DWORD style = DataTbl->SendMessage(DTM_GETWNDSTYLE,0,0L);
//	if( DataTbl->SetWndStyle( style|
//			DTS_OWNERDEF|DTS_EDIT|DTS_RESIZE|
//			DTS_ROWNUMBERS|DTS_VGRID|DTS_HGRID) != DTE_OKAY )
//		AfxMessageBox("SetWndStyle Failed");
	DTCOLORSTRUCT Color;
	Color.BkgrdColor = RGB(255,255,255);
	Color.TextColor = RGB(0,0,0);
	Color.SelBkgrdColor = RGB(0,0,128);
	Color.SelTextColor = RGB(0,255,255);
	DataTbl->SendMessage(DTM_SETDTCOLOR,0,(LPARAM)&Color);
	DataTbl->SendMessage(DTM_SETGRIDCOLOR,0,RGB(255,0,0));

///////////////////////////////////////////////////////////////////////////////////
	DataTbl->SetAccessPos(0,0);     
	
	//InvalidateRect(NULL);
	//ShowWindow(SW_SHOWDEFAULT);
	CRect rect;
	GetClientRect(&rect);
	::SetWindowPos(this->GetSafeHwnd(), HWND_TOP, 0,0,rect.Width(),rect.Height(),SWP_NOMOVE);
	int i=0;	 
	for(m_class.First(); m_class.Okay(); m_class.Next() )
	{
		if( m_class.type == COLUMN )
		{
			InsertRow();
			i++;
		}
	}
	DataTbl->SetAccessPos((WPARAM)DTPOS_AFTERLAST, (LPARAM)DTPOS_AFTERLAST);
	DataTbl->InsertRow(DTA_ACCESS, NULL);		

	CFormView::OnInitialUpdate();

	GetParent()->SetWindowText(GetDocument()->GetTitle()+" - Column Definitions");
}

void CColumnFormView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
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
		if( cx > 0 && cy> 0 )     
		{	
			::SetWindowPos(DataTbl->GetSafeHwnd(),HWND_TOP,0,0,cx,cy,SWP_SHOWWINDOW);
		}
		InvalidateRect(NULL);
	}       
	ShowScrollBar(SB_BOTH, FALSE);
	
}

int CColumnFormView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
	{
		AfxMessageBox("Couldn't create form");
		return -1;
	}
	
	CRect rect;
	GetClientRect(&rect);
	::SetWindowPos( GetSafeHwnd(), HWND_TOP, 0,0, rect.Width(), rect.Height(), SWP_NOMOVE |
			SWP_FRAMECHANGED | SWP_DRAWFRAME );

	
	return 0;
}


void CColumnFormView::OnDataUpdate()
{
//	AfxMessageBox("On Data Update\n");
}

void CColumnFormView::OnDataChange()
{
	COLNUM col;
 	ROWNUM row;
	double value;		//value of new typed string
 	union
 	{
    	char temp[128];
    	long l;
    	int i;
    } data;
	ColumnData column;

	t->MakeCurrent();
	char temp[128];
	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_COLUMNTABLE);
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	TRACE("row=%ld:  col=%d\n", row, col);  
	
 	DataTbl->SetAccessPos(13L, row);
 	DataTbl->ReadCell(DTA_ACCESS, (LPVOID)&(data.l));
 	sprintf(temp,"The dba is %ld\n", data.l);          
//	AfxMessageBox(temp);
	if( data.l == 0 )
	{
		//AfxMessageBox("This is a new record");
	 	TransAction a;
	 	a.Begin("wow");
		memset(m_class.description,0,sizeof(m_class.description));
		memset(m_class.name,0,sizeof(m_class.name));
		time_t ti;
		time(&ti);
		m_class.type = COLUMN;
		sprintf(m_class.name, "%s", asctime(localtime(&ti)));
		char temp[128];
	 	if( m_class.WriteLock() == FALSE )
		{
			sprintf(temp, "Error on WriteLock - %d", m_class.GetStatus());
			AfxMessageBox(temp);
		}
		DB_ADDR dba;
		m_class.NewObj();
		dba = m_class.GetDba();
	 	DataTbl->SetAccessPos(13L, row);
	 	DataTbl->UpdateCell(DTA_ACCESS, (LPVOID)&(dba));
		if( m_class.GetStatus() != S_OKAY )
		{
			sprintf(temp, "Error on NewObj - %d", m_class.GetStatus());
			AfxMessageBox(temp);
		}
		m_class.Write();
		if( m_class.GetStatus() != S_OKAY )
		{
			sprintf(temp, "Error on Write - %d", m_class.GetStatus());
			AfxMessageBox(temp);
		}
		memset(m_line.RecData(), 0, m_line.RecDataSize() );
		m_line.NewObj();
		m_line.ConnectTo(m_class, ELEMENT_CLASSES);
		a.End();
		DataTbl->SetAccessPos((WPARAM)DTPOS_AFTERLAST, (LPARAM)DTPOS_AFTERLAST);
		DataTbl->InsertRow(DTA_ACCESS, NULL);		
  	}
	else
	{
		m_class.GotoDirect(DbAddr(data.l));
//		AfxMessageBox("Existing Record");
	}
	if( m_class.Okay() )
	{
		memset(data.temp,0,sizeof(data.temp));
	 	TransAction a;
	 	a.Begin("wow");
	 	m_class.WriteLock();
		switch( col )
		{
			case 0:
				DataTbl->ReadCell(DTA_CURRENT,(LPVOID)m_class.name);
				m_class.Write();
				break;		
			default:
				m_all.FromOwner(&m_class, ELEMENT_CLASSES);
				if( m_all.Okay() )
				{
			//		AfxMessageBox("Found the member");
					ElementObj *e1 = m_all.GetElement();
					DbAddr dba(e1->GetDba());
					m_line.GotoDirect(dba);
					DataTbl->ReadRow(DTA_ACCESS, &column);
					DataTbl->ReadCell(DTA_CURRENT,(LPVOID)temp);
					if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
						value = atof(temp);
					else
					{
						double inches = getInches(temp);
						value = inches*25.4;
					}
					switch( col )
					{
						case 7:
							column.east_begin = value;
							break;
						case 8:
							column.north_begin = value;			
							break;
						case 9:
							column.height_begin = value;
							break;
						case 10:
							column.east_end = value;
							break;
						case 11:
							column.north_end = value;
							break;
						case 12:
							column.height_end = value;
							break;
					}
	VECTOR vPoint1 = vector(1,3);
	VECTOR vPoint2 = vector(1,3);
	/* Get input data into TWW format */
	vPoint1[1] = column.east_begin;	
	vPoint1[2] = column.north_begin;	
	vPoint1[3] = column.height_begin;
	vPoint2[1] = column.east_end;	
	vPoint2[2] = column.north_end;	
	vPoint2[3] = column.height_end;
	
	VECTOR vXform = (((CFotobrowApp *)AfxGetApp())->GetTransform()) - 1;

	/* Get inverse of current/old transform */
	VECTOR vXformInv = matinv(vXform);

	VECTOR vNewPoint1 = matpnt(vXformInv,vPoint1);
	VECTOR vNewPoint2 = matpnt(vXformInv,vPoint2);
	
	m_line.x1 = vNewPoint1[1];
	m_line.y1 = vNewPoint1[2];
	m_line.z1 = vNewPoint1[3];
	m_line.x2 = vNewPoint2[1];
	m_line.y2 = vNewPoint2[2];
	m_line.z2 = vNewPoint2[3];

	free_vector( vPoint1, 1,3);
	free_vector( vPoint2, 1,3);
	free_vector( vXformInv, 1,12);
	free_vector( vNewPoint1, 1,3);
	free_vector( vNewPoint2, 1,3);
	DataTbl->UpdateRow(DTA_ACCESS,&column);
	
					m_line.Write();
				}
				break;
		}	         
		a.End();
		if( col != 0 )	 // IF NOT EDITING LABEL FIELD
		{
			if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
			{
				sprintf(temp,"%10.2f", value);
			}
			else
			{
				ConvertUnits(temp, 30,  value/25.4 );
			}
		}
		else
			strcpy(temp, (char *)m_class.name );
			
		DataTbl->UpdateCell(DTA_CURRENT,(LPVOID)temp);
	}

}

void CColumnFormView::InsertRow()
{
	ColumnData column;
 //	AfxMessageBox("wqwqwqwow");	
	m_all.FromOwner(&m_class,ELEMENT_CLASSES);
	if( !m_all.Okay() )
	{
		TransAction a;
		a.Begin("Wow");
		memset(m_line.RecData(), 0, m_line.RecDataSize());
		m_line.NewObj();
		m_line.ConnectTo(m_class, ELEMENT_CLASSES);
		a.End();
	} 
//	m_line.l_x1=1;
//	m_line.l_y1=2;
//	m_line.l_z1=3;
//	m_line.l_x2=4;
//	m_line.l_y2=5;
//	m_line.l_z2=6;
	memset(m_line.RecData(), 0, m_line.RecDataSize());
			
	ElementObj *e1 = m_all.GetElement();
	DbAddr dba(e1->GetDba());
	m_line.GotoDirect(dba);
	memset(&column, 0, sizeof(column));
	strcpy( column.name , m_class.name );
//////////////////////////////////////////////////////////////////////////
	VECTOR vPoint1 = vector(1,3);
	VECTOR vPoint2 = vector(1,3);

	/* Get input data into TWW format */
	vPoint1[1] = m_line.x1;	
	vPoint1[2] = m_line.y1;	
	vPoint1[3] = m_line.z1;
	vPoint2[1] = m_line.x2;	
	vPoint2[2] = m_line.y2;	
	vPoint2[3] = m_line.z2;

	/* Use a trick to get array to be 1 to 12 */
	VECTOR vXform = (((CFotobrowApp *)AfxGetApp())->GetTransform()) - 1;

//	/* Get inverse of current/old transform */
//	VECTOR vOldXformInv = matinv(vOldXform);

	/* Convert inputs that are in current/old system to photog coordinate system */
	VECTOR vNewPoint1 = matpnt( vXform, vPoint1);
	VECTOR vNewPoint2 = matpnt( vXform, vPoint2);
 //////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
	column.east_begin = vNewPoint1[1];
	column.north_begin = vNewPoint1[2];
	column.height_begin = vNewPoint1[3];
	column.east_end	= vNewPoint2[1];
	column.north_end = vNewPoint2[2];
	column.height_end = vNewPoint2[3];
	
	free_vector(vPoint1,1,3);
	free_vector(vPoint2,1,3);
	free_vector(vNewPoint1,1,3);
	free_vector(vNewPoint2,1,3);

////////////////////////////////////////////////////
	if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
	{
		sprintf(column.sz_east_begin,"%10.2f", column.east_begin);
		sprintf(column.sz_north_begin,"%10.2f", column.north_begin);
		sprintf(column.sz_height_begin,"%10.2f", column.height_begin);
		sprintf(column.sz_east_end,"%10.2f", column.east_end);
		sprintf(column.sz_north_end,"%10.2f", column.north_end);
		sprintf(column.sz_height_end,"%10.2f",  column.height_end);
	}
	else
	{
		
		ConvertUnits(column.sz_east_begin, 30,  column.east_begin/25.4 );
		ConvertUnits(column.sz_north_begin, 30, column.north_begin/25.4 );
		ConvertUnits(column.sz_height_begin, 30, column.height_begin/25.4 );
		ConvertUnits(column.sz_east_end, 30, column.east_end/25.4 );
		ConvertUnits(column.sz_north_end, 30, column.north_end/25.4 );
		ConvertUnits(column.sz_height_end, 30, column.height_end/25.4 );
	}
////////////////////////////////////////////////////	 
/*	sprintf(column.sz_east_begin,"%10.2f", m_line.l_x1);
	sprintf(column.sz_north_begin,"%10.2f", m_line.l_y1);
	sprintf(column.sz_height_begin,"%10.2f", m_line.l_z1);
	sprintf(column.sz_east_end,"%10.2f", m_line.l_x2);
	sprintf(column.sz_north_end,"%10.2f", m_line.l_y2);
	sprintf(column.sz_height_end,"%10.2f",  m_line.l_z2);
*/
	column.dba = m_class.GetDba();
	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_COLUMNTABLE);
	if( DataTbl == NULL )
		AfxMessageBox("NO DATATABLE");
	DataTbl->SetAccessPos(0,DTPOS_AFTERLAST);
	long row = DataTbl->InsertRow(DTA_ACCESS, (LPVOID)&column );
}

void CColumnFormView::OnDeleteColumn() 
{
	COLNUM col;
 	ROWNUM row;
 	union
 	{
    	char temp[128];
    	long l;
    	int i;
    } data;
	t->MakeCurrent();
	char temp[128];
	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_COLUMNTABLE);
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	
 	TRACE("row=%ld:  col=%d\n", row, col);  
 	sprintf(temp, "row=%ld:  col=%d\n", row, col);  
//	AfxMessageBox(temp);

 	DataTbl->SetAccessPos(13L, row);
 	DataTbl->ReadCell(DTA_ACCESS, (LPVOID)&(data.l));
 	sprintf(temp,"The dba is %ld\n", data.l);          
//	AfxMessageBox(temp);

	m_class.GotoDirect(DbAddr(data.l));
	m_all.FromOwner(&m_class, ELEMENT_CLASSES);
	if( m_all.Okay() )
	{
//		AfxMessageBox("Found the member");
		ElementObj *e1 = m_all.GetElement();
		DbAddr dba(e1->GetDba());
		m_line.GotoDirect(dba);
	 	TransAction a;
	 	a.Begin("wow");
	 	m_class.WriteLock();
		m_line.WriteLock();
		m_line.DisConnect(m_class);
		m_line.Delete();
		m_class.Delete();
		a.End();
		DataTbl->DeleteRow(DTA_ACCESS);

	}
}

void CColumnFormView::OnEditCopy() 
{
	COLNUM col;
 	ROWNUM row;
	double value;		//value of new typed string
 	union
 	{
    	char temp[128];
    	long l;
    	int i;
    } data;
	char temp[128];

	t->MakeCurrent();

	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_COLUMNTABLE);	 
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	
 //	sprintf(temp, "Inside the ColumnGrid Box row=%ld:  col=%d\n", row, col);  
//	AfxMessageBox(temp);
		
 	DataTbl->SetAccessPos(col, row);
 	DataTbl->ReadCell(DTA_ACCESS, (LPVOID)&(data));
// 	sprintf(temp,"The data is %s\n", data.temp);          
//	AfxMessageBox(temp);
	if( ::OpenClipboard(NULL) )
	{
		HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, sizeof(temp));
		LPVOID ptr = GlobalLock(handle);
		strcpy( (char *)ptr, data.temp );
		GlobalUnlock(handle);
		SetClipboardData( CF_TEXT, handle);
		::CloseClipboard();
	}
}

void CColumnFormView::OnEditPaste() 
{
	COLNUM col;
 	ROWNUM row;
	double value;		//value of new typed string
 	union
 	{
    	char temp[128];
    	long l;
    	int i;
    } data;
	char temp[128];

	if( ::OpenClipboard(NULL) )
	{
		HANDLE handle;
		handle = GetClipboardData( CF_TEXT );
		if( handle != NULL )
		{
			LPVOID ptr = GlobalLock(handle);
			strcpy(  data.temp, (char *)ptr );
			GlobalUnlock(handle);
		}
		::CloseClipboard();
	}
//	AfxMessageBox(data.temp);

	t->MakeCurrent();

	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_COLUMNTABLE);
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	TRACE("row=%ld:  col=%d\n", row, col);  
	
 	DataTbl->SetAccessPos(col, row);
 	DataTbl->UpdateCell(DTA_ACCESS, (LPVOID)&(data));
	OnDataChange();
// 	sprintf(temp,"The data is %s\n", data.temp);          
//	AfxMessageBox(temp);
}

void CColumnFormView::OnEditCut() 
{

}
/************************* Temporary hack during datatable switch *******/
#endif
/************************* Temporary hack during datatable switch *******/
