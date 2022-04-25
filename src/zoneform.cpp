// zoneform.cpp : implementation file
//

/************************* Temporary hack during datatable switch *******/
#if FALSE
/************************* Temporary hack during datatable switch *******/

#include "stdafx.h"
#include "fotobrow.h"
#include "zoneform.h"

#include <datatbl.h>
//#include <cdatatbl.h>
//#include <dtmfc.h>

#include <transact.hpp>

#include "mathsubs.h"
#include "photog_math.h"

#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

double getInches( char *inputStr );
/////////////////////////////////////////////////////////////////////////////
// CZoneFormView

IMPLEMENT_DYNCREATE(CZoneFormView, CFormView)

CZoneFormView::CZoneFormView()
	: CFormView(CZoneFormView::IDD)
{
	//{{AFX_DATA_INIT(CZoneFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CZoneFormView::~CZoneFormView()
{
}

void CZoneFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoneFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoneFormView, CFormView)
	//{{AFX_MSG_MAP(CZoneFormView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_DELETE_ZONE, OnDeleteZone)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
	ON_CONTROL(DTN_UPDATE, IDD_ZONETABLE, OnDataUpdate )
	ON_CONTROL(DTN_CHANGE, IDD_ZONETABLE, OnDataChange )
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CZoneFormView diagnostics

#ifdef _DEBUG
void CZoneFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CZoneFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZoneFormView message handlers

void CZoneFormView::OnInitialUpdate() 
{
	UINT temp1;	
	// TODO: Add extra initialization here
	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_ZONETABLE);			
	if( DataTbl==NULL )
	{
		AfxMessageBox("BOMBED ON GetDlgItem");	
		return;
	}
	temp1=DataTbl->GetWndStyle();	    //CWnd
	DataTbl->SetColWidth(DTPOS_LABELS, 16);
	DataTbl->SetWndStyle(WS_VSCROLL|WS_HSCROLL|temp1|DTS_LARGEDATA);
	DataTbl->SetWndStyle(WS_MAXIMIZE|(~WS_VISIBLE)&(WS_VSCROLL|WS_HSCROLL|temp1|DTS_LARGEDATA));
	DataTbl->SetAccessPos(0,0);     
///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////	
	//InvalidateRect(NULL);
	//ShowWindow(SW_SHOWDEFAULT);
	CRect rect;
	GetClientRect(&rect);
	::SetWindowPos(this->GetSafeHwnd(), HWND_TOP, 0,0,rect.Width()-1,rect.Height()-1,SWP_NOMOVE);
	 
	
	for(c.First(); c.Okay(); c.Next() )
	{
		if( c.type == 128 )
		{
			TransAction a;
			a.Begin("wow");
			c.WriteLock();
			c.type = ZONE;
			c.Write();
			a.End();
			InsertRow();		
		}
		else if( c.type == ZONE )
			InsertRow();		
	}
	DataTbl->SetAccessPos((WPARAM)DTPOS_AFTERLAST, (LPARAM)DTPOS_AFTERLAST);
	DataTbl->InsertRow(DTA_ACCESS, NULL);		
	
//	CFormView::OnInitialUpdate();

	GetParent()->SetWindowText(GetDocument()->GetTitle()+" - Clipping Volumes");

//	SetWindowText( "Clipping Volumes");
}

void CZoneFormView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	RECT GridRect,rect;
	HWND hWnd;

	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_ZONETABLE);			
	if( DataTbl==NULL )
	{
//		AfxMessageBox("BOMBED ON GetDlgItem");	
		return;
	}

	if( DataTbl != NULL )
	{
		hWnd = ::GetDlgItem(DataTbl->GetSafeHwnd(), IDD_ZONETABLE);
		::GetWindowRect(DataTbl->GetSafeHwnd(),&GridRect );     
		GetWindowRect(&rect);
//		TRACE("rect.bottom=%d, GridRect.top=%d, width=%d, height=%d\n", rect.bottom,GridRect.top,cx-2,rect.bottom-GridRect.top-2 ); 
		if( cx > 0 && cy> 0 )     
		{	
			::SetWindowPos(DataTbl->GetSafeHwnd(),HWND_TOP,0,0,cx,cy,SWP_SHOWWINDOW);
		}
		InvalidateRect(NULL);
	}       
	ShowScrollBar(SB_BOTH, FALSE);
	
}

int CZoneFormView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rect;
	GetClientRect(&rect);
	::SetWindowPos( GetSafeHwnd(), HWND_TOP, 0,0, rect.Width(), rect.Height(), SWP_NOMOVE |
			SWP_FRAMECHANGED | SWP_DRAWFRAME );

	//PostMessage(WS_MAXIMIZE,0,0L);	

	return 0;
}

void CZoneFormView::OnDataUpdate()
{
//	AfxMessageBox("On Data Update\n");
}

void CZoneFormView::OnDataChange()
{
//	scsdwgsObj s;
//	AfxMessageBox("On Data Change\n");
	PropertiesObj p;
	COLNUM col;
 	ROWNUM row;
	double value;
 	union
 	{
    	char temp[128];
    	long l;
    	int i;
    } data;
	ZoneData z;
//	return;	    
	t->MakeCurrent();
	char temp[128];
	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_ZONETABLE);
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	TRACE("row=%ld:  col=%d\n", row, col);  
	
// 	db.GotoRecord(row);
 	DataTbl->SetAccessPos(13L, row);
 	DataTbl->ReadCell(DTA_ACCESS, (LPVOID)&(data.l));
// 	TRACE("The temp string is %s\n", data.temp);
// 	sprintf(temp,"The dba is %ld\n", data.l);          
//	AfxMessageBox(temp);
// 	TRACE("The short int is %d\n", data.i);
	if( data.l == 0 )
	{
//		AfxMessageBox("This is a new record");
	 	TransAction a;
	 	a.Begin("wow");
		memset(c.description,0,sizeof(c.description));
		memset(c.name,0,sizeof(c.name));
		time_t ti;
		time(&ti);
//		sprintf(c.class_name, "%s", asctime(localtime(&ti)));
		c.type = ZONE;
//		char temp[128];
	 	if( c.WriteLock() == FALSE )
		{
			sprintf(temp, "Error on WriteLock - %d", c.GetStatus());
			AfxMessageBox(temp);
		}
		DB_ADDR dba;
		c.NewObj();
		dba = c.GetDba();
	 	DataTbl->SetAccessPos(13L, row);
	 	DataTbl->UpdateCell(DTA_ACCESS, (LPVOID)&(dba));
		if( c.GetStatus() != S_OKAY )
		{
			sprintf(temp, "Error on NewObj - %d", c.GetStatus());
			AfxMessageBox(temp);
		}
		c.Write();
		if( c.GetStatus() != S_OKAY )
		{
			sprintf(temp, "Error on Write - %d", c.GetStatus());
			AfxMessageBox(temp);
		}
		memset(&(p.range.x1), 0, sizeof(p.range));
		p.NewObj();
		c.ConnectTo(p, PROPERTIES_SET);
		a.End();
		DataTbl->SetAccessPos((WPARAM)DTPOS_AFTERLAST, (LPARAM)DTPOS_AFTERLAST);
		DataTbl->InsertRow(DTA_ACCESS, NULL);		
	}
	else
	{
		c.GotoDirect(DbAddr(data.l));
//		AfxMessageBox("Existing Record");
	}
	if( c.Okay() )
	{
		PropertiesObj p;
	 	memset(data.temp,0,sizeof(data.temp));
	 	TransAction a;
	 	a.Begin("wow");
	 	c.WriteLock();
		switch( col )
		{
			case 0:
				DataTbl->ReadCell(DTA_CURRENT,(LPVOID)c.name);
				c.Write();
				break;		
			default:
				p.FromMember(&c, PROPERTIES_SET);
				if( p.Okay() )
				{
					DataTbl->ReadCell(DTA_CURRENT,(LPVOID)temp);
					DataTbl->ReadRow(DTA_ACCESS,&z);
//					AfxMessageBox(temp);
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
							z.SW_east = value;			
							break;
						case 8:
							z.SW_north = value;			
							break;
						case 9:
							z.SW_height = value;			
							break;
						case 10:
							z.NE_east = value;			
							break;
						case 11:
							z.NE_north = value;			
							break;
						case 12:
							z.NE_height = value;			
							break;
					}
	VECTOR vPoint1 = vector(1,3);
	VECTOR vPoint2 = vector(1,3);
	/* Get input data into TWW format */
	vPoint1[1] = z.SW_east;	vPoint1[2] = z.SW_north;	vPoint1[3] = z.SW_height;
	vPoint2[1] = z.NE_east;	vPoint2[2] = z.NE_north;	vPoint2[3] = z.NE_height;
	
	VECTOR vXform = (((CFotobrowApp *)AfxGetApp())->GetTransform()) - 1;

	/* Get inverse of current/old transform */
	VECTOR vXformInv = matinv(vXform);

	VECTOR vNewPoint1 = matpnt(vXformInv,vPoint1);
	VECTOR vNewPoint2 = matpnt(vXformInv,vPoint2);
	
	p.range.x1 = vNewPoint1[1];
	p.range.y1 = vNewPoint1[2];
	p.range.z1 = vNewPoint1[3];
	p.range.x2 = vNewPoint2[1];
	p.range.y2 = vNewPoint2[2];
	p.range.z2 = vNewPoint2[3];

	free_vector( vPoint1, 1,3);
	free_vector( vPoint2, 1,3);
	free_vector( vXformInv, 1,12);
	free_vector( vNewPoint1, 1,3);
	free_vector( vNewPoint2, 1,3);
	DataTbl->UpdateRow(DTA_ACCESS,&z);
					p.Write();
				}
				break;
		}	         
		a.End();
		if( col != 0 )	 // IF NOT EDITING LABEL FIELD
		{
//			char temp3[128];
//			sprintf(temp3,"value = %f", value );
//			AfxMessageBox(temp3);
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
			strcpy( temp, c.name );

		DataTbl->UpdateCell(DTA_CURRENT,(LPVOID)temp);
	}
}

void CZoneFormView::InsertRow()
{
	ZoneData z;
 	
	PropertiesObj p;

	p.FromMember(&c,PROPERTIES_SET);
	if( !p.Okay() )
	{
		TransAction a;
		a.Begin("Wow");
		memset(&(p.range.x1), 0, sizeof(p.range));
		p.NewObj();
		c.ConnectTo(p, PROPERTIES_SET);
		a.End();
	} 
	memset(&z, 0, sizeof(z));
	strcpy( z.name , c.name );
///////////////////////////////////////////////////////////
	VECTOR vPoint1 = vector(1,3);
	VECTOR vPoint2 = vector(1,3);

	/* Get input data into TWW format */
	vPoint1[1] = p.range.x1;	vPoint1[2] = p.range.y1;	vPoint1[3] = p.range.z1;
	vPoint2[1] = p.range.x2;	vPoint2[2] = p.range.y2;	vPoint2[3] = p.range.z2;

	/* Use a trick to get array to be 1 to 12 */
	VECTOR vXform = (((CFotobrowApp *)AfxGetApp())->GetTransform()) - 1;

//	/* Get inverse of current/old transform */
//	VECTOR vOldXformInv = matinv(vOldXform);

	/* Convert inputs that are in current/old system to photog coordinate system */
	VECTOR vNewPoint1 = matpnt( vXform, vPoint1);
	VECTOR vNewPoint2 = matpnt( vXform, vPoint2);
 //////////////////////////////////////////////////////////
	
	z.SW_east = vNewPoint1[1];
	z.SW_north = vNewPoint1[2];
	z.SW_height = vNewPoint1[3];
	z.NE_east	= vNewPoint2[1];
	z.NE_north = vNewPoint2[2];
	z.NE_height = vNewPoint2[3];

	free_vector(vPoint1,1,3);
	free_vector(vPoint2,1,3);
	free_vector(vNewPoint1,1,3);
	free_vector(vNewPoint2,1,3);
////////////////////////////////////////////////////
	if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
	{
		sprintf(z.szSW_east,"%10.2f", z.SW_east);
		sprintf(z.szSW_north,"%10.2f", z.SW_north);
		sprintf(z.szSW_height,"%10.2f", z.SW_height);
		sprintf(z.szNE_east,"%10.2f", z.NE_east);
		sprintf(z.szNE_north,"%10.2f", z.NE_north);
		sprintf(z.szNE_height,"%10.2f",  z.NE_height);
	}
	else
	{
		
		ConvertUnits(z.szSW_east, 30, z.SW_east/25.4 );
		ConvertUnits(z.szSW_north, 30, z.SW_north/25.4 );
		ConvertUnits(z.szSW_height, 30, z.SW_height/25.4 );
		ConvertUnits(z.szNE_east, 30,z.NE_east/25.4 );
		ConvertUnits(z.szNE_north, 30, z.NE_north/25.4 );
		ConvertUnits(z.szNE_height, 30,z.NE_height/25.4 );
	}
////////////////////////////////////////////////////	 
	
	z.dba = c.GetDba();
	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_ZONETABLE);
	DataTbl->SetAccessPos(0,DTPOS_AFTERLAST);
	long row = DataTbl->InsertRow(DTA_ACCESS, (LPVOID)&z );
}

void CZoneFormView::OnDeleteZone() 
{
	PropertiesObj p;
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
	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_ZONETABLE);
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	
 	TRACE("row=%ld:  col=%d\n", row, col);  
 	sprintf(temp, "row=%ld:  col=%d\n", row, col);  
//	AfxMessageBox(temp);

 	DataTbl->SetAccessPos(13L, row);
 	DataTbl->ReadCell(DTA_ACCESS, (LPVOID)&(data.l));
 	sprintf(temp,"The dba is %ld\n", data.l);          
//	AfxMessageBox(temp);

	c.GotoDirect(DbAddr(data.l));
 	memset(data.temp,0,sizeof(data.temp));

	if( c.Okay() )
	{
		p.FromMember(&c, PROPERTIES_SET);
//		AfxMessageBox("Found the member");
	 	TransAction a;
	 	a.Begin("wow");
		c.WriteLock();
		p.WriteLock();
		c.DisConnect(p);
		c.Delete();
		p.Delete();
		a.End();
		DataTbl->DeleteRow(DTA_ACCESS);
	}
}


void CZoneFormView::OnEditCopy() 
{
	COLNUM col;
 	ROWNUM row;
//	double value;		//value of new typed string
 	union
 	{
    	char temp[128];
    	long l;
    	int i;
    } data;
//	char temp[128];

	t->MakeCurrent();

	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_ZONETABLE);
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	TRACE("row=%ld:  col=%d\n", row, col);  
	
 	DataTbl->SetAccessPos(col, row);
 	DataTbl->ReadCell(DTA_ACCESS, (LPVOID)&(data));
// 	sprintf(temp,"The data is %s\n", data.temp);          
//	AfxMessageBox(temp);
	if( ::OpenClipboard(NULL) )
	{
		HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, sizeof(data.temp));
		LPVOID ptr = GlobalLock(handle);
		strcpy( (char *)ptr, data.temp );
		GlobalUnlock(handle);
		SetClipboardData( CF_TEXT, handle);
		::CloseClipboard();
	}
}

void CZoneFormView::OnEditPaste() 
{
	COLNUM col;
 	ROWNUM row;
//	double value;		//value of new typed string
 	union
 	{
    	char temp[128];
    	long l;
    	int i;
    } data;
//	char temp[128];

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

	CDataTbl *DataTbl = (CDataTbl *)GetDlgItem(IDD_ZONETABLE);
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	TRACE("row=%ld:  col=%d\n", row, col);  
	
 	DataTbl->SetAccessPos(col, row);
 	DataTbl->UpdateCell(DTA_ACCESS, (LPVOID)&(data));
	OnDataChange();
// 	sprintf(temp,"The data is %s\n", data.temp);          
//	AfxMessageBox(temp);
}

void CZoneFormView::OnEditCut() 
{

}

/************************* Temporary hack during datatable switch *******/
#endif
/************************* Temporary hack during datatable switch *******/
