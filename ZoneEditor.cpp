// ZoneEditor.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "ZoneEditor.h"

#include <datatbl.h>
#include <dtocx.h>
#include "dtrowset.h"
#include "dtrow.h"

#include <transact.hpp>

#include "mathsubs.h"
#include "photog_math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
#define FILEMASK 0xFFL
#define ADDRMASK 0x00FFFFFFL
#define FILESHIFT 24

#define GET_FNO(dba)	((FILE_NO)(FILEMASK & (dba >> FILESHIFT)))
#define GET_RNO(dba)	((F_ADDR)(ADDRMASK & dba))
///////////////////////////////////////////////////////////////////////////////

double getInches( char *inputStr );

int DataTableToZoneData( CDTData &dtRow, ZoneData &zoneData );
int ZoneDataToDataTable( ZoneData &zoneData, CDTData &dtRow );

/////////////////////////////////////////////////////////////////////////////
// CZoneEditor

IMPLEMENT_DYNCREATE(CZoneEditor, CFormView)

CZoneEditor::CZoneEditor()
	: CFormView(CZoneEditor::IDD)
{
	//{{AFX_DATA_INIT(CZoneEditor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CZoneEditor::~CZoneEditor()
{
}

void CZoneEditor::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoneEditor)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoneEditor, CFormView)
	//{{AFX_MSG_MAP(CZoneEditor)
	ON_WM_SIZE()
	ON_MESSAGE(UM_UNITS_CHANGE, OnUnitsChange )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoneEditor diagnostics

#ifdef _DEBUG
void CZoneEditor::AssertValid() const
{
	CFormView::AssertValid();
}

void CZoneEditor::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZoneEditor message handlers

BEGIN_EVENTSINK_MAP(CZoneEditor, CFormView)
    //{{AFX_EVENTSINK_MAP(CZoneEditor)
	ON_EVENT(CZoneEditor, IDC_ZONEGRID, 20 /* BeforeDeleteRow */, OnBeforeDeleteRowZonegrid, VTS_I4 VTS_PI4)
	ON_EVENT(CZoneEditor, IDC_ZONEGRID, 6 /* Change */, OnChangeZonegrid, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CZoneEditor::OnBeforeDeleteRowZonegrid(long Row, long FAR* Status) 
{
	PropertiesObj p;
	ZoneData zone;

	t->MakeCurrent();
	CDatatbl *DataTbl = (CDatatbl *)GetDlgItem( IDC_ZONEGRID );

	CDTData currentRow = DataTbl->GetRowSet().GetItem(Row).GetValue();
	DataTableToZoneData( currentRow, zone );

	c.GotoDirect(DbAddr(zone.dba));

	if( c.Okay() )
	{
		p.FromMember(&c, PROPERTIES_SET);
		if( p.Okay() )
		{
	 		TransAction a;
	 		a.Begin("wow");
			c.WriteLock();
			p.WriteLock();

// Here's the story with this one.  When I (jwb) was putting the ActiveX control Data Table
// in here, I stumbled upon this problem with DisConnect in the Classes object.  It would always
// say "incorrect set" or something like that.  I did discover that the properties set is named
// PROPERTIES_SET (to eliminate conflict with record type PROPERTIES ).  After carefully perusing 
// all of the occurences of PROPERTIES in the source, I concluded that something in Object
// Manager was still out of sync.  Replacing the line with DisConFrom fixed it.  I suspect that
// a rebuild all might have fixed it.  Just didn't want to take the time to do that right now.

//			c.DisConnect(p);
			c.DisConFrom(p, PROPERTIES_SET);

			c.Delete();
			p.Delete();
			a.End();
		}
		else
			AfxMessageBox( "Error going from classes to properties" );
	}
	else
		AfxMessageBox( "Error going to classes record" );
}

void CZoneEditor::OnChangeZonegrid(long Row, long Column) 
{
	ZoneData zone;
	PropertiesObj p;

	t->MakeCurrent();
	char temp[128];
	CDatatbl *DataTbl = (CDatatbl *)GetDlgItem(IDC_ZONEGRID);
 
	CDTData currentRow = DataTbl->GetRowSet().GetItem(Row).GetValue();
	DataTableToZoneData( currentRow, zone );

	if( zone.dba == 0 )
	{
	 	TransAction a;
	 	a.Begin("wow");
		memset(c.description,0,sizeof(c.description));
		memset(c.name,0,sizeof(c.name));
		c.type = ZONE;
	 	if( c.WriteLock() == FALSE )
		{
			sprintf(temp, "Error on WriteLock - %d", c.GetStatus());
			AfxMessageBox(temp);
		}

		DB_ADDR dba;
		c.NewObj();
		zone.dba = c.GetDba();
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

		ZoneDataToDataTable( zone, currentRow );
		DataTbl->GetRowSet().GetItem(Row).SetValue(currentRow);
	}
	else
	{
		c.GotoDirect(DbAddr(zone.dba));
	}

	if( c.Okay() )
	{
	 	TransAction a;
	 	a.Begin("wow");
	 	c.WriteLock();
		switch( Column )
		{
			case 0:
				strcpy( c.name, zone.name );
				c.Write();
				break;		
			default:
				p.FromMember( &c, PROPERTIES_SET );
				if( p.Okay() )
				{
					char *strValue = NULL;
					double *dblValue = NULL;

					switch( Column )
					{
						case 1:
							strValue = zone.szSW_east;
							dblValue = &(zone.SW_east);
							break;
						case 2:
							strValue = zone.szSW_north;
							dblValue = &(zone.SW_north);
							break;
						case 3:
							strValue = zone.szSW_height;
							dblValue = &(zone.SW_height);
							break;
						case 4:
							strValue = zone.szNE_east;
							dblValue = &(zone.NE_east);
							break;
						case 5:
							strValue = zone.szNE_north;
							dblValue = &(zone.NE_north);
							break;
						case 6:
							strValue = zone.szNE_height;
							dblValue = &(zone.NE_height);
							break;
					}

					if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
						*dblValue = atof(strValue);
					else
						*dblValue = 25.4 * getInches(strValue);

					VECTOR vPoint1 = vector(1,3);
					VECTOR vPoint2 = vector(1,3);
					/* Get input data into TWW format */
					vPoint1[1] = zone.SW_east;	vPoint1[2] = zone.SW_north;	vPoint1[3] = zone.SW_height;
					vPoint2[1] = zone.NE_east;	vPoint2[2] = zone.NE_north;	vPoint2[3] = zone.NE_height;
	
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

					p.Write();

					if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
					{
						sprintf(strValue,"%10.2f", *dblValue);
					}
					else
					{
						ConvertUnits(strValue, 30,  *dblValue/25.4 );
					}

					ZoneDataToDataTable( zone, currentRow );
					DataTbl->GetRowSet().GetItem(Row).SetValue(currentRow);
				}
				break;
		}	         
		a.End();
	}
}

void CZoneEditor::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	CDatatbl *DataTbl = (CDatatbl *)GetDlgItem( IDC_ZONEGRID );
	if( !DataTbl )
		return;

	for(c.First(); c.Okay(); c.Next() )
	{
		// An inspirational little bit of "Dave magic"
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

	GetParent()->SetWindowText(GetDocument()->GetTitle()+" - Clipping Volumes");
}

void CZoneEditor::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CDatatbl *DataTbl = (CDatatbl *)GetDlgItem( IDC_ZONEGRID );
	if( !DataTbl )
		return;

	if( cx && cy )
	{
		DataTbl->SetWindowPos( &wndTop, 0, 0, cx, cy, SWP_SHOWWINDOW );
		DataTbl->InvalidateRect(NULL,TRUE);
	}
	ShowScrollBar(SB_BOTH, FALSE);
}

void CZoneEditor::InsertRow()
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

	CDTData rowData;
	rowData.CreateArray(14);

	ZoneDataToDataTable( z, rowData );

	CDatatbl *DataTable = (CDatatbl *)GetDlgItem(IDC_ZONEGRID);
	DataTable->GetRowSet().Add(dtAfterLast, rowData);

}

int DataTableToZoneData( CDTData &dtRow, ZoneData &zoneData )
{
	dtRow.GetItem( zoneData.name, 0 );			// Zone Name
	dtRow.GetItem( &(zoneData.SW_east), 8 );	// Zone SW corner (mm double)
	dtRow.GetItem( &(zoneData.SW_north), 9 );	// Zone SW corner (mm double)
	dtRow.GetItem( &(zoneData.SW_height), 10 );	// Zone SW corner (mm double)
	dtRow.GetItem( &(zoneData.NE_east), 11 );	// Zone NE corner (mm double)
	dtRow.GetItem( &(zoneData.NE_north), 12 );	// Zone NE corner (mm double)
	dtRow.GetItem( &(zoneData.NE_height), 13 );	// Zone NE corner (mm double)
	dtRow.GetItem( zoneData.szSW_east, 1 );		// Zone SW corner (current units, string)
	dtRow.GetItem( zoneData.szSW_north, 2 );	// Zone SW corner (current units, string)
	dtRow.GetItem( zoneData.szSW_height, 3 );	// Zone SW corner (current units, string)
	dtRow.GetItem( zoneData.szNE_east, 4 );		// Zone NE corner (current units, string)
	dtRow.GetItem( zoneData.szNE_north, 5 );	// Zone NE corner (current units, string)
	dtRow.GetItem( zoneData.szNE_height, 6 );	// Zone NE corner (current units, string)
	dtRow.GetItem( &(zoneData.dba), 7 );		// Zone DBA

	return 0;
}

int ZoneDataToDataTable( ZoneData &zoneData, CDTData &dtRow )
{
	dtRow.PutItem( zoneData.name, 0 );			// Zone Name
	dtRow.PutItem( zoneData.SW_east, 8 );		// Zone SW corner (mm double)
	dtRow.PutItem( zoneData.SW_north, 9 );		// Zone SW corner (mm double)
	dtRow.PutItem( zoneData.SW_height, 10 );		// Zone SW corner (mm double)
	dtRow.PutItem( zoneData.NE_east, 11 );		// Zone NE corner (mm double)
	dtRow.PutItem( zoneData.NE_north, 12 );		// Zone NE corner (mm double)
	dtRow.PutItem( zoneData.NE_height, 13 );		// Zone NE corner (mm double)
	dtRow.PutItem( zoneData.szSW_east, 1 );		// Zone SW corner (current units, string)
	dtRow.PutItem( zoneData.szSW_north, 2 );	// Zone SW corner (current units, string)
	dtRow.PutItem( zoneData.szSW_height, 3 );	// Zone SW corner (current units, string)
	dtRow.PutItem( zoneData.szNE_east, 4 );	// Zone NE corner (current units, string)
	dtRow.PutItem( zoneData.szNE_north, 5 );	// Zone NE corner (current units, string)
	dtRow.PutItem( zoneData.szNE_height, 6 );	// Zone NE corner (current units, string)
	dtRow.PutItem( zoneData.dba, 7 );			// Zone DBA

	return 0;
}

LONG CZoneEditor::OnUnitsChange(WPARAM wParam, LPARAM lParam)
{
	CDatatbl *dataTable = (CDatatbl *)GetDlgItem(IDC_ZONEGRID);
	ASSERT(dataTable);

	for( long i = 0; i < dataTable->GetRowSet().GetCount(); i++ )
	{
		CDTData currentRow = dataTable->GetRowSet().GetItem(i).GetValue();
		ZoneData column;

		DataTableToZoneData( currentRow, column );
		UpdateUnits(&column);
		ZoneDataToDataTable( column, currentRow );

		dataTable->GetRowSet().GetItem(i).SetValue(currentRow);
	}

	dataTable->InvalidateRect( NULL, TRUE );

	return TRUE;
}

void CZoneEditor::UpdateUnits( ZoneData *zd )
{
	if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
	{
		sprintf(zd->szSW_east,"%10.2f",		zd->SW_east);
		sprintf(zd->szSW_north,"%10.2f",	zd->SW_north);
		sprintf(zd->szSW_height,"%10.2f",	zd->SW_height);
		sprintf(zd->szNE_east,"%10.2f",		zd->NE_east);
		sprintf(zd->szNE_north,"%10.2f",	zd->NE_north);
		sprintf(zd->szNE_height,"%10.2f",	zd->NE_height);
	}
	else
	{
		
		ConvertUnits(zd->szSW_east, 30,		zd->SW_east/25.4 );
		ConvertUnits(zd->szSW_north, 30,	zd->SW_north/25.4 );
		ConvertUnits(zd->szSW_height, 30,	zd->SW_height/25.4 );
		ConvertUnits(zd->szNE_east, 30,		zd->NE_east/25.4 );
		ConvertUnits(zd->szNE_north, 30,	zd->NE_north/25.4 );
		ConvertUnits(zd->szNE_height, 30,	zd->NE_height/25.4 );
	}
}