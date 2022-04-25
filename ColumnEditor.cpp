// ColumnEditor.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "ColumnEditor.h"

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

int DataTableToColumnData( CDTData &dtRow, ColumnData &colData );
int ColumnDataToDataTable( ColumnData &colData, CDTData &dtRow );

/////////////////////////////////////////////////////////////////////////////
// CColumnEditor

IMPLEMENT_DYNCREATE(CColumnEditor, CFormView)

CColumnEditor::CColumnEditor()
	: CFormView(CColumnEditor::IDD)
{
	//{{AFX_DATA_INIT(CColumnEditor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CColumnEditor::~CColumnEditor()
{
}

void CColumnEditor::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnEditor)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColumnEditor, CFormView)
	//{{AFX_MSG_MAP(CColumnEditor)
	ON_WM_SIZE()
	ON_MESSAGE(UM_UNITS_CHANGE, OnUnitsChange )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColumnEditor diagnostics

#ifdef _DEBUG
void CColumnEditor::AssertValid() const
{
	CFormView::AssertValid();
}

void CColumnEditor::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CColumnEditor message handlers

BEGIN_EVENTSINK_MAP(CColumnEditor, CFormView)
    //{{AFX_EVENTSINK_MAP(CColumnEditor)
	ON_EVENT(CColumnEditor, IDC_COLUMNGRID, 6 /* Change */, OnChangeColumngrid, VTS_I4 VTS_I4)
	ON_EVENT(CColumnEditor, IDC_COLUMNGRID, 20 /* BeforeDeleteRow */, OnBeforeDeleteRowColumngrid, VTS_I4 VTS_PI4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CColumnEditor::OnChangeColumngrid(long Row, long Column) 
{
	ColumnData column;

	t->MakeCurrent();
	CDatatbl *DataTbl = (CDatatbl *)GetDlgItem(IDC_COLUMNGRID);
 
	CDTData currentRow = DataTbl->GetRowSet().GetItem(Row).GetValue();
	DataTableToColumnData( currentRow, column );

//	sprintf(temp,"The dba is %ld\n", data.l);          
//	AfxMessageBox(temp);
	if( column.dba == 0 )
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
//		DB_ADDR dba;
		m_class.NewObj();

//		dba = m_class.GetDba();
//	 	DataTbl->SetAccessPos(13L, row);
//	 	DataTbl->UpdateCell(DTA_ACCESS, (LPVOID)&(dba));
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

		column.dba = m_class.GetDba();
		ColumnDataToDataTable( column, currentRow );
		DataTbl->GetRowSet().GetItem(Row).SetValue(currentRow);

		a.End();
//		DataTbl->SetAccessPos((WPARAM)DTPOS_AFTERLAST, (LPARAM)DTPOS_AFTERLAST);
//		DataTbl->InsertRow(DTA_ACCESS, NULL);		
  	}
	else
	{
		m_class.GotoDirect(DbAddr(column.dba));
//		AfxMessageBox("Existing Record");
	}
	if( m_class.Okay() )
	{
//		memset(data.temp,0,sizeof(data.temp));
	 	TransAction a;
	 	a.Begin("wow");
	 	m_class.WriteLock();
		switch( Column )
		{
			case 0:
				strcpy( m_class.name, column.name );
//				DataTbl->ReadCell(DTA_CURRENT,(LPVOID)m_class.name);
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

					char *strValue = NULL;
					double *dblValue = NULL;

					switch(Column)
					{
						case 1:
							strValue = column.sz_east_begin;
							dblValue = &(column.east_begin);
							break;
						case 2:
							strValue = column.sz_north_begin;
							dblValue = &(column.north_begin);
							break;
						case 3:
							strValue = column.sz_height_begin;
							dblValue = &(column.height_begin);
							break;
						case 4:
							strValue = column.sz_east_end;
							dblValue = &(column.east_end);
							break;
						case 5:
							strValue = column.sz_north_end;
							dblValue = &(column.north_end);
							break;
						case 6:
							strValue = column.sz_height_end;
							dblValue = &(column.height_end);
							break;
					}

					if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
						*dblValue = atof(strValue);
					else
						*dblValue = 25.4 * getInches(strValue);

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

					column.east_begin = vPoint1[1];
					column.north_begin = vPoint1[2];
					column.height_begin = vPoint1[3];
					column.east_end = vPoint2[1];
					column.north_end = vPoint2[2];
					column.height_end = vPoint2[3];

					free_vector( vPoint1, 1,3);
					free_vector( vPoint2, 1,3);
					free_vector( vXformInv, 1,12);
					free_vector( vNewPoint1, 1,3);
					free_vector( vNewPoint2, 1,3);


				//	DataTbl->UpdateRow(DTA_ACCESS,&column);
					
					m_line.Write();
					if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
					{
						sprintf(strValue,"%10.2f", *dblValue);
					}
					else
					{
						ConvertUnits(strValue, 30,  *dblValue/25.4 );
					}

					ColumnDataToDataTable( column, currentRow );
					DataTbl->GetRowSet().GetItem(Row).SetValue(currentRow);
				}
				break;
		}	         
		a.End();
	}
}

void CColumnEditor::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	CDatatbl *DataTbl = (CDatatbl *)GetDlgItem( IDC_COLUMNGRID );
	if( !DataTbl )
		return;

	for( m_class.First(); m_class.Okay(); m_class.Next() )
	{
		if( m_class.type == COLUMN )
			InsertRow();
	}

	GetParent()->SetWindowText(GetDocument()->GetTitle() + " - Column Definitions" );
}

void CColumnEditor::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	CDatatbl *DataTbl = (CDatatbl *)GetDlgItem( IDC_COLUMNGRID );
	if( !DataTbl )
		return;

	if( cx && cy )
	{
		DataTbl->SetWindowPos( &wndTop, 0, 0, cx, cy, SWP_SHOWWINDOW );
		DataTbl->InvalidateRect(NULL,TRUE);
	}
	ShowScrollBar(SB_BOTH, FALSE);
}

void CColumnEditor::InsertRow() 
{
	ColumnData column;

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
	memset(m_line.RecData(), 0, m_line.RecDataSize());
			
	ElementObj *e1 = m_all.GetElement();
	if( !e1 )
	{
		AfxMessageBox( "Error getting element" );
		return;
	}
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
	UpdateUnits( &column );
	column.dba = m_class.GetDba();

	CDTData rowData;
	rowData.CreateArray(14);

	ColumnDataToDataTable( column, rowData );

	CDatatbl *DataTable = (CDatatbl *)GetDlgItem(IDC_COLUMNGRID);
	DataTable->GetRowSet().Add(dtAfterLast, rowData);
}

int DataTableToColumnData( CDTData &dtRow, ColumnData &colData )
{
	dtRow.GetItem( colData.name, 0 );				// Column Name
	dtRow.GetItem( &(colData.east_begin), 8 );		// Column East Begin (mm double)
	dtRow.GetItem( &(colData.north_begin), 9 );		// Column North Begin (mm double)
	dtRow.GetItem( &(colData.height_begin), 10 );	// Column Height Begin (mm double)
	dtRow.GetItem( &(colData.east_end), 11 );		// Column East End (mm double)
	dtRow.GetItem( &(colData.north_end), 12 );		// Column North End (mm double)
	dtRow.GetItem( &(colData.height_end), 13 );		// Column Height End (mm double)
	dtRow.GetItem( colData.sz_east_begin, 1 );		// Column East Begin (current units, string)
	dtRow.GetItem( colData.sz_north_begin, 2 );		// Column North Begin (current units, string)
	dtRow.GetItem( colData.sz_height_begin, 3 );	// Column Height Begin (current units, string)
	dtRow.GetItem( colData.sz_east_end, 4 );		// Column East End (current units, string)
	dtRow.GetItem( colData.sz_north_end, 5 );		// Column North End (current units, string)
	dtRow.GetItem( colData.sz_height_end, 6 );		// Column Height End (current units, string)
	dtRow.GetItem( &(colData.dba), 7 );				// Column DBA

	return 0;
}

int ColumnDataToDataTable( ColumnData &colData, CDTData &dtRow )
{
	dtRow.PutItem( colData.name, 0 );				// Column Name
	dtRow.PutItem( colData.east_begin, 8 );			// Column East Begin (mm double)
	dtRow.PutItem( colData.north_begin, 9 );		// Column North Begin (mm double)
	dtRow.PutItem( colData.height_begin, 10 );		// Column Height Begin (mm double)
	dtRow.PutItem( colData.east_end, 11 );			// Column East End (mm double)
	dtRow.PutItem( colData.north_end, 12 );			// Column North End (mm double)
	dtRow.PutItem( colData.height_end, 13 );		// Column Height End (mm double)
	dtRow.PutItem( colData.sz_east_begin, 1 );		// Column East Begin (current units, string)
	dtRow.PutItem( colData.sz_north_begin, 2 );		// Column North Begin (current units, string)
	dtRow.PutItem( colData.sz_height_begin, 3 );	// Column Height Begin (current units, string)
	dtRow.PutItem( colData.sz_east_end, 4 );		// Column East End (current units, string)
	dtRow.PutItem( colData.sz_north_end, 5 );		// Column North End (current units, string)
	dtRow.PutItem( colData.sz_height_end, 6 );		// Column Height End (current units, string)
	dtRow.PutItem( colData.dba, 7 );				// Column DBA

	return 0;
}

void CColumnEditor::OnBeforeDeleteRowColumngrid(long Row, long FAR* Status) 
{
	ColumnData column;

	t->MakeCurrent();
	CDatatbl *DataTbl = (CDatatbl *)GetDlgItem(IDC_COLUMNGRID);

	CDTData currentRow = DataTbl->GetRowSet().GetItem(Row).GetValue();
	DataTableToColumnData( currentRow, column );

	m_class.GotoDirect(DbAddr(column.dba));
	m_all.FromOwner(&m_class, ELEMENT_CLASSES);
	if( m_all.Okay() )
	{
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
	}
}

LONG CColumnEditor::OnUnitsChange(WPARAM wParam, LPARAM lParam)
{
	CDatatbl *dataTable = (CDatatbl *)GetDlgItem(IDC_COLUMNGRID);
	ASSERT(dataTable);

	for( long i = 0; i < dataTable->GetRowSet().GetCount(); i++ )
	{
		CDTData currentRow = dataTable->GetRowSet().GetItem(i).GetValue();
		ColumnData column;

		DataTableToColumnData( currentRow, column );
		UpdateUnits(&column);
		ColumnDataToDataTable( column, currentRow );

		dataTable->GetRowSet().GetItem(i).SetValue(currentRow);
	}

	dataTable->InvalidateRect( NULL, TRUE );

	return TRUE;
}

void CColumnEditor::UpdateUnits(ColumnData *column)
{
	if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
	{
		sprintf(column->sz_east_begin,"%10.2f",		column->east_begin);
		sprintf(column->sz_north_begin,"%10.2f",	column->north_begin);
		sprintf(column->sz_height_begin,"%10.2f",	column->height_begin);
		sprintf(column->sz_east_end,"%10.2f",		column->east_end);
		sprintf(column->sz_north_end,"%10.2f",		column->north_end);
		sprintf(column->sz_height_end,"%10.2f",		column->height_end);
	}
	else
	{
		
		ConvertUnits(column->sz_east_begin, 30,		column->east_begin/25.4 );
		ConvertUnits(column->sz_north_begin, 30,	column->north_begin/25.4 );
		ConvertUnits(column->sz_height_begin, 30,	column->height_begin/25.4 );
		ConvertUnits(column->sz_east_end, 30,		column->east_end/25.4 );
		ConvertUnits(column->sz_north_end, 30,		column->north_end/25.4 );
		ConvertUnits(column->sz_height_end, 30,		column->height_end/25.4 );
	}
} 

