// PointGridBar.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "PointGridBar.h"
#include "elementListview.h"
#include <dtocx.h>
#include "dtrowset.h"
#include "dtrow.h"
#include "dtcurcell.h"
#include "dtcolumnset.h"

#include <math.h>

#include "3DClass/Ptg3DNode.h"
#include "3DClass/Ptg3DPointObj.h"
#include "3DClass/Ptg3DCalcPointObj.h"
#include "3DClass/Ptg3DCylinderObj.h"
#include "3DClass/Ptg3DBoxObj.h"

#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// Column index defines for node editor grid
//
#define NODEGRID_NAME		0
#define NODEGRID_POSEAST	1
#define NODEGRID_POSNORTH	2
#define NODEGRID_POSUP		3
#define NODEGRID_NODEPTR	4
#define NODEGRID_DBA		5

#define NODEGRID_NUMCOLUMNS	6

//
// Column index defines for point editor grid
//
#define POINTGRID_NAME		0
#define POINTGRID_POSX		1
#define POINTGRID_POSY		2
#define POINTGRID_POSZ		3
#define POINTGRID_DELTAX	4
#define POINTGRID_DELTAY	5
#define POINTGRID_DELTAZ	6
#define POINTGRID_DIST		7
#define POINTGRID_STDDEV_X	8
#define POINTGRID_STDDEV_Y	9
#define POINTGRID_STDDEV_Z	10
#define POINTGRID_RAYMISS	11
#define POINTGRID_NODEPTR	12
#define POINTGRID_DBA		13

#define POINTGRID_NUMCOLUMNS	14

//
// Column index defines for cylinder editor grid
//
#define CYLIGRID_NAME			0
#define CYLIGRID_DIAMETER		1
#define CYLIGRID_HEIGHT			2
#define CYLIGRID_CENTERPOSEAST	3
#define CYLIGRID_CENTERPOSNORTH	4
#define CYLIGRID_CENTERPOSUP	5
#define CYLIGRID_BEGEAST		6
#define CYLIGRID_BEGNORTH		7
#define CYLIGRID_BEGUP			8
#define CYLIGRID_ENDEAST		9
#define CYLIGRID_ENDNORTH		10
#define CYLIGRID_ENDUP			11
#define CYLIGRID_NODEPTR		12
#define CYLIGRID_DBA			13

#define CYLIGRID_NUMCOLUMNS		14

//
// Column index defines for box editor grid
//
#define BOXGRID_NAME			0
#define BOXGRID_LENGTH			1
#define BOXGRID_WIDTH			2
#define BOXGRID_HEIGHT			3
#define BOXGRID_CENTERPOSEAST	4
#define BOXGRID_CENTERPOSNORTH	5
#define BOXGRID_CENTERPOSUP		6
#define BOXGRID_NODEPTR			7

#define BOXGRID_NUMCOLUMNS		9


double getInches( char *inputStr );

int DataTableToGridRowData( CDTData &dtRow, GridRowData &grdRow );
int GridRowDataToDataTable( GridRowData &grdRow, CDTData &dtRow );

int add3DObjToGrid( CDatatbl *theTable, Ptg3DNode *theNode );
int add3DObjToGrid( CDatatbl *theTable, Ptg3DPointObj *thePoint );
int add3DObjToGrid( CDatatbl *theTable, Ptg3DCalcPointObj *thePoint );
int add3DObjToGrid( CDatatbl *theTable, Ptg3DCylinderObj *theCyli );
int add3DObjToGrid( CDatatbl *theTable, Ptg3DBoxObj *theBox );

int update3DObjInGrid( CDatatbl *theTable, Ptg3DPointObj *thePoint = NULL, long rowNum = -1 );

void DoubleToUnitsString( double dblValue, char *strValue )
{
	if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
		sprintf( strValue, "%15.2f", dblValue );
	else
		ConvertUnits(strValue, 30, dblValue/25.4 );
}

double ValueCellToMMDouble( CDTData row, int index )
{
	char strValue[256];
	double retValue = 0.0;

	row.GetItem( strValue, index );

	if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
		retValue = atof(strValue);
	else
		retValue = 25.4 * getInches(strValue);

	return retValue;
}

/////////////////////////////////////////////////////////////////////////////
// PointGridBar


int PointGridBar::sheetGrids[] = {
	IDC_NODE_GRID,
	IDC_POINT_GRID,
	IDC_CYLINDER_GRID,
	IDC_BOX_GRID
 };

PointGridBar::PointGridBar()
{
	currentSheetIndex = 1;
	currentNode = NULL;
}

PointGridBar::~PointGridBar()
{
}

BEGIN_MESSAGE_MAP(PointGridBar, CDialogBar)
	//{{AFX_MSG_MAP(PointGridBar)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_MESSAGE(UM_CURRENT_ELEMENT_CHANGE, OnCurrentElementChange )
	ON_MESSAGE(UM_UNITS_CHANGE, OnUnitsChange )
	ON_MESSAGE(UM_NEW_3D_ELEMENT, OnNew3DElement)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PointGridBar message handlers

BEGIN_EVENTSINK_MAP(PointGridBar, CDialogBar)
    //{{AFX_EVENTSINK_MAP(PointGridBar)
	ON_EVENT(PointGridBar, IDC_POINT_GRID, 50 /* LfClick */, OnLfClick3dPointGrid, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	ON_EVENT(PointGridBar, IDC_POINT_GRID, 16 /* RowChange */, OnRowChangePointGrid, VTS_I4)
	ON_EVENT(PointGridBar, IDC_POINT_GRID, 20 /* BeforeDeleteRow */, OnBeforeDeleteRowPointGrid, VTS_I4 VTS_PI4)
	ON_EVENT(PointGridBar, IDC_NODE_GRID, 6 /* Change */, OnChangeNodeGrid, VTS_I4 VTS_I4)
	ON_EVENT(PointGridBar, IDC_POINT_GRID, 6 /* Change */, OnChangePointGrid, VTS_I4 VTS_I4)
	ON_EVENT(PointGridBar, IDC_CYLINDER_GRID, 6 /* Change */, OnChangeCylinderGrid, VTS_I4 VTS_I4)
	ON_EVENT(PointGridBar, IDC_BOX_GRID, 6 /* Change */, OnChangeBoxGrid, VTS_I4 VTS_I4)
	ON_EVENT(PointGridBar, IDC_NODE_GRID, 50 /* LfClick */, OnLfClickNodeGrid, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	ON_EVENT(PointGridBar, IDC_CYLINDER_GRID, 50 /* LfClick */, OnLfClickCylinderGrid, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	ON_EVENT(PointGridBar, IDC_BOX_GRID, 50 /* LfClick */, OnLfClickBoxGrid, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void PointGridBar::OnLfClick3dPointGrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status) 
{
#if FALSE
	CDatatbl *pointGrid = (CDatatbl *)GetDlgItem( IDC_POINT_GRID );

	for( long i = 0; i < pointGrid->GetRowSet().GetCount(); i++ )
	{
		update3DObjInGrid( pointGrid, (Ptg3DPointObj *)NULL, i );
	}
#endif

#if FALSE
//	AfxMessageBox("Left Click in Grid Bar");

	if( Row < 0 )
		return;

	CDatatbl *m_PointGrid = (CDatatbl *)GetDlgItem( IDC_POINT_GRID );
	CDTData selectedRow = m_PointGrid->GetRowSet().GetItem(Row).GetValue();
	GridRowData selectedRowData;

	DataTableToGridRowData( selectedRow, selectedRowData );

	for( long i = 0; i < m_PointGrid->GetRowSet().GetCount(); i++ )
	{
		CDTData row = m_PointGrid->GetRowSet().GetItem(i).GetValue();
		GridRowData rowData;

		DataTableToGridRowData( row, rowData );

		rowData.dx = selectedRowData.x - rowData.x;
		rowData.dy = selectedRowData.y - rowData.y;
		rowData.dz = selectedRowData.z - rowData.z;
		rowData.dist = sqrt(rowData.dx*rowData.dx + rowData.dy*rowData.dy + rowData.dz*rowData.dz);
		UpdateUnits(&rowData);

		GridRowDataToDataTable( rowData, row );
		m_PointGrid->GetRowSet().GetItem(i).SetValue(row);
	}
#endif

	CDatatbl *pointGrid = (CDatatbl *)GetDlgItem( IDC_POINT_GRID );
	if( ClickPos == posOnHeading )
	{
		pointGrid->GetColumnSet().SetSortEnabled(TRUE);
		pointGrid->GetColumnSet().SetSortKey(sortPrimary,Column);
		pointGrid->GetColumnSet().SetSortAsc(sortPrimary,TRUE);
		pointGrid->GetColumnSet().SetSortCaseI(sortPrimary,TRUE);
	}
}

void PointGridBar::OnRowChangePointGrid(long Row) 
{
	CDatatbl *pointGrid = (CDatatbl *)GetDlgItem( IDC_POINT_GRID );

	for( long i = 0; i < pointGrid->GetRowSet().GetCount(); i++ )
	{
		update3DObjInGrid( pointGrid, (Ptg3DPointObj *)NULL, i );
	}
}

void PointGridBar::UpdateUnits(GridRowData *row)
{
	DoubleToUnitsString( row->x,	row->sz_x );
	DoubleToUnitsString( row->y,	row->sz_y );
	DoubleToUnitsString( row->z,	row->sz_z );
	DoubleToUnitsString( row->dx,	row->sz_dx );
	DoubleToUnitsString( row->dy,	row->sz_dy );
	DoubleToUnitsString( row->dz,	row->sz_dz );
	DoubleToUnitsString( row->dist,	row->sz_dist );
} 

void PointGridBar::OnSize(UINT nType, int cx, int cy) 
{
	int topBorder = 3;
	int bottomBorder = 3;
	int leftBorder = 3;
	int rightBorder = 3;

	CDialogBar::OnSize(nType, cx, cy);
	CRect dialogBarRect;
	GetClientRect( &dialogBarRect );

	CTabCtrl *tabCntrl = (CTabCtrl *)GetDlgItem( IDC_SPREADSHEET_TAGS );
	CRect tabControlRect;
	if( tabCntrl )
	{
		tabCntrl->GetClientRect( &tabControlRect );
		tabCntrl->SetWindowPos( &wndTop,
				0,
				dialogBarRect.Height() - (tabControlRect.Height() + bottomBorder),
				dialogBarRect.Width(), tabControlRect.Height(), SWP_SHOWWINDOW );
	}

	for( int i = 0; i < sizeof(sheetGrids) / sizeof(sheetGrids[0]); i++ )
	{
		CDatatbl *gridCntrl = (CDatatbl *)GetDlgItem(sheetGrids[i]);
		if( gridCntrl )
		{
			UINT flags = SWP_NOACTIVATE|SWP_SHOWWINDOW;
			if( i != currentSheetIndex )
				flags |= SWP_NOZORDER;

			gridCntrl->SetWindowPos( NULL, leftBorder, topBorder,
					dialogBarRect.Width() - (leftBorder + rightBorder),
					dialogBarRect.Height() - (topBorder + bottomBorder + tabControlRect.Height() - 1),
					flags );
			if( i == currentSheetIndex )
				gridCntrl->BringWindowToTop();
		}
	}

	InvalidateRect(NULL,TRUE);
}

void PointGridBar::OnEditCopy() 
{
	CWnd *currWnd = GetFocus();
	CDatatbl *dataTbl = (CDatatbl *)GetDlgItem(IDC_POINT_GRID);

	// Decided to be a little safe here.  If current control is not my data table,
	// I'm not gonna pass on the message.  May be OK to just pass on all copy messages
	// regardless, dunno
	if( currWnd == dataTbl )
		dataTbl->SendMessage( WM_COPY, 0, 0 );
}

void PointGridBar::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void PointGridBar::OnEditCut() 
{
	CWnd *currWnd = GetFocus();
	CDatatbl *dataTbl = (CDatatbl *)GetDlgItem(IDC_POINT_GRID);

	// Decided to be a little safe here.  If current control is not my data table,
	// I'm not gonna pass on the message.  May be OK to just pass on all cut messages
	// regardless, dunno
	if( currWnd == dataTbl )
		dataTbl->SendMessage( WM_CUT, 0, 0 );
}

void PointGridBar::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void PointGridBar::OnEditPaste() 
{
	CWnd *currWnd = GetFocus();
	CDatatbl *dataTbl = (CDatatbl *)GetDlgItem(IDC_POINT_GRID);

	// Decided to be a little safe here.  If current control is not my data table,
	// I'm not gonna pass on the message.  May be OK to just pass on all paste messages
	// regardless, dunno
	if( currWnd == dataTbl )
		dataTbl->SendMessage( WM_PASTE, 0, 0 );
}

void PointGridBar::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void PointGridBar::OnBeforeDeleteRowPointGrid(long Row, long FAR* Status)
{
//	AfxMessageBox( "PointGridBar::OnBeforeDeleteRowPointgrid" );
}

void PointGridBar::OnChangeNodeGrid(long Row, long Column)
{
	CDatatbl *nodeGrid = (CDatatbl *)GetDlgItem( IDC_NODE_GRID );
	CDTData currRow = nodeGrid->GetRowSet().GetItem(Row).GetValue();
	Ptg3DNode *currNode;
	char strValue[256];
	int didICreateANode = FALSE;

	currRow.GetItem( (long *)(&currNode), NODEGRID_NODEPTR );

	if( currNode == NULL )
	{
		currNode = new Ptg3DNode;
		currentNode->addMember( currNode );
		currRow.PutItem( (long)currNode, NODEGRID_NODEPTR );
		nodeGrid->GetRowSet().GetItem(Row).SetValue(currRow);
		didICreateANode = TRUE;
	}

	switch( Column )
	{
		case CYLIGRID_NAME:
			currRow.GetItem( strValue, Column );
			currNode->setName( strValue );
			break;

		default:
			break;
	}

	if( didICreateANode )
	{
		// Add to element list view...if its up
		CObList *pList = new CObList;
		((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
		POSITION pos = pList->GetHeadPosition();
		while( pos )
		{
			CDocument *pDoc = (CDocument *)(pList->GetNext(pos));
			POSITION pos2 = pDoc->GetFirstViewPosition();
			while( pos2 )
			{
				CView *view = pDoc->GetNextView(pos2);
				ASSERT(view);

				if( view->IsKindOf(RUNTIME_CLASS(CElementListView)) )
				{
					CFotobrowApp *theApp = (CFotobrowApp *)AfxGetApp();
					CMainFrame *theMainFrame = (CMainFrame *)(theApp->m_pMainWnd);
					HTREEITEM hTreeItem = (HTREEITEM)theMainFrame->m_3DtoTree[(void *)(currNode->getOwner())];
					if( hTreeItem )
					{
						((CElementListView *)view)->AddToDrawList( currNode, hTreeItem, TRUE, FALSE, FALSE );
					}
					view->Invalidate(TRUE);
				}
			}
		}

		while( pList->GetCount() )
		pList->RemoveTail();
		delete pList;
	}
}

void PointGridBar::OnChangePointGrid(long Row, long Column)
{
	CDatatbl *pointGrid = (CDatatbl *)GetDlgItem( IDC_POINT_GRID );
	CDTData currRow = pointGrid->GetRowSet().GetItem(Row).GetValue();
	Ptg3DPointObj *currPoint;
	char strValue[256];

	currRow.GetItem( (long *)(&currPoint), POINTGRID_NODEPTR );
	if( currPoint == NULL )
	{
		currPoint = new Ptg3DPointObj;
		currentNode->addMember( currPoint );
		currRow.PutItem( (long)currPoint, POINTGRID_NODEPTR );

		pointGrid->GetRowSet().GetItem(Row).SetValue(currRow);
	}

	// Here's the idea for this next section...take the current PointNode position,
	// transform it to the reference coordinate system (which is what the user should be
	// entering data in), update the changed value, and transform it back to update the
	// node.
	VECTOR vPoint = currPoint->getPosition( ((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);

//	VECTOR vTMatrix = currPoint->getMatrix(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
//	VECTOR vInvTMatrix = matinv(vTMatrix);
//	VECTOR vPoint = currPoint->getPosition();
//	VECTOR vXformPoint = matpnt( vTMatrix, vPoint );

	switch( Column )
	{
		case POINTGRID_NAME:
			currRow.GetItem( strValue, Column );
			currPoint->setName( strValue );
			break;
		case POINTGRID_POSX:
//			vXformPoint[1] = ValueCellToMMDouble( currRow, Column );
			vPoint[1] = ValueCellToMMDouble( currRow, Column );
			break;
		case POINTGRID_POSY:
//			vXformPoint[2] = ValueCellToMMDouble( currRow, Column );
			vPoint[2] = ValueCellToMMDouble( currRow, Column );
			break;
		case POINTGRID_POSZ:
//			vXformPoint[3] = ValueCellToMMDouble( currRow, Column );
			vPoint[3] = ValueCellToMMDouble( currRow, Column );
			break;
		default:
			break;
	}

//	free_vector( vPoint, 1, 3 );
//	vPoint = matpnt( vInvTMatrix, vXformPoint );
	currPoint->setPosition( vPoint[1], vPoint[2], vPoint[3], ((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode );
	free_vector( vPoint, 1, 3 );

//	free_vector( vTMatrix, 1, 12 );
//	free_vector( vInvTMatrix, 1, 12 );
//	free_vector( vPoint, 1, 3 );
//	free_vector( vXformPoint, 1, 3 );

	update3DObjInGrid( pointGrid, currPoint, Row );

	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->UpdateAllViews();
#if FALSE
	CDatatbl *m_PointGrid = (CDatatbl *)GetDlgItem( IDC_POINT_GRID );
	CDTData currentRow = m_PointGrid->GetRowSet().GetItem(Row).GetValue();
	GridRowData currentRowData;

	DataTableToGridRowData( currentRow, currentRowData );

	char *strValue = NULL;
	double *dblValue = NULL;

	switch( Column )
	{
		case 1:
			strValue = currentRowData.sz_x;
			dblValue = &(currentRowData.x);
			break;
		case 2:
			strValue = currentRowData.sz_y;
			dblValue = &(currentRowData.y);
			break;
		case 3:
			strValue = currentRowData.sz_z;
			dblValue = &(currentRowData.z);
			break;
	}

	if( strValue != NULL && dblValue != NULL )
	{
		if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
			*dblValue = atof(strValue);
		else
		{
			*dblValue = 25.4*getInches(strValue);
			ConvertUnits( strValue, 30, *dblValue / 25.4 );
		}
	}

	GridRowDataToDataTable( currentRowData, currentRow );
	m_PointGrid->GetRowSet().GetItem(Row).SetValue(currentRow);
#endif
}

void PointGridBar::OnChangeCylinderGrid(long Row, long Column)
{
	CDatatbl *cylinderGrid = (CDatatbl *)GetDlgItem( IDC_CYLINDER_GRID );
	CDTData currRow = cylinderGrid->GetRowSet().GetItem(Row).GetValue();

	Ptg3DCylinderObj *currCyli;
	char strValue[256];

	currRow.GetItem( (long *)(&currCyli), CYLIGRID_NODEPTR );

	if( currCyli == NULL )
	{
		currCyli = new Ptg3DCylinderObj;
		currentNode->addMember( currCyli );
		currRow.PutItem( (long)currCyli, CYLIGRID_NODEPTR );

		cylinderGrid->GetRowSet().GetItem(Row).SetValue(currRow);
	}

	switch( Column )
	{
		case CYLIGRID_NAME:
			currRow.GetItem( strValue, Column );
			currCyli->setName( strValue );
			break;

		case CYLIGRID_DIAMETER:
			currCyli->setRadius( ValueCellToMMDouble( currRow, Column ) / 2.0 );
			break;
		case CYLIGRID_HEIGHT:
			currCyli->setHeight( ValueCellToMMDouble( currRow, Column ) );
			break;

		default:
			break;
	}

	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->UpdateAllViews();
}

void PointGridBar::OnChangeBoxGrid(long Row, long Column)
{
	CDatatbl *boxGrid = (CDatatbl *)GetDlgItem( IDC_BOX_GRID );
	CDTData currRow = boxGrid->GetRowSet().GetItem(Row).GetValue();

	Ptg3DBoxObj *currBox;
	char strValue[256];

	currRow.GetItem( (long *)(&currBox), BOXGRID_NODEPTR );

	if( currBox == NULL )
	{
		currBox = new Ptg3DBoxObj;
		currentNode->addMember( currBox );
		currRow.PutItem( (long)currBox, BOXGRID_NODEPTR );

		boxGrid->GetRowSet().GetItem(Row).SetValue(currRow);
	}

	switch(Column)
	{
		case BOXGRID_NAME:
			currRow.GetItem(strValue, Column);
			currBox->setName( strValue );
			break;

		case BOXGRID_LENGTH:
			currBox->setXLen( ValueCellToMMDouble( currRow, Column ) );
			break;
		case BOXGRID_WIDTH:
			currBox->setYLen( ValueCellToMMDouble( currRow, Column ) );
			break;
		case BOXGRID_HEIGHT:
			currBox->setZLen( ValueCellToMMDouble( currRow, Column ) );
			break;
	}

	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->UpdateAllViews();
}

/********************
void PointGridBar::OnInsertedRowCylinderGrid(long Row) 
{
}
********************/

BOOL PointGridBar::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT *pResult )
{
	NMHDR *hdr;
	int index;
	hdr = (NMHDR *)lParam;
	switch( hdr->code )
	{
		case TCN_SELCHANGE:
			index = ::SendMessage(hdr->hwndFrom,TCM_GETCURSEL ,0,0);
			if( index >= 0 )
			{
				currentSheetIndex = index;
				CDatatbl *newActiveTab = (CDatatbl *)GetDlgItem( sheetGrids[currentSheetIndex] );
				newActiveTab->BringWindowToTop();
			}
			else
				MessageBox("I got nothing");
			break;
	}

	return CDialogBar::OnNotify(wParam, lParam, pResult);
}

BOOL PointGridBar::Create(CWnd* pParentWnd, UINT nStyle)
{
	BOOL returnVal;

	returnVal = CDialogBar::Create( pParentWnd, IDD_3DPOINTGRIDBAR, nStyle, IDD_3DPOINTGRIDBAR );

	CTabCtrl *sheet_tabs = (CTabCtrl *)GetDlgItem(IDC_SPREADSHEET_TAGS);
	ASSERT(sheet_tabs);
	TC_ITEM tc;

	tc.mask = TCIF_TEXT|TCIF_PARAM;

	tc.pszText = "Nodes";
	tc.lParam = 0;
	sheet_tabs->InsertItem(0, &tc);

	tc.pszText = "Points";
	tc.lParam = 1;
	sheet_tabs->InsertItem(1, &tc);

	tc.pszText = "Cylinders";
	tc.lParam = 2;
	sheet_tabs->InsertItem(2, &tc);

	tc.pszText = "Boxes";
	tc.lParam = 3;
	sheet_tabs->InsertItem(3, &tc);

//	tc.pszText = "Old Point Grid";
//	tc.lParam = 4;
//	sheet_tabs->InsertItem( 4, &tc );

//	tc.pszText = "Circles";
//	tc.lParam = 2;
//	sheet_tabs->InsertItem(2, &tc);
//	tc.pszText = "Targets";
//	tc.lParam = 3;
//	sheet_tabs->InsertItem(3, &tc);
//	tc.pszText = "Cameras";
//	tc.lParam = 4;
//	sheet_tabs->InsertItem(4, &tc);

	sheet_tabs->SetCurSel( currentSheetIndex );
	CWnd *cwnd = GetDlgItem(sheetGrids[currentSheetIndex]);
	cwnd->BringWindowToTop();
	cwnd->InvalidateRect(NULL,TRUE);
	
	return returnVal;
}

LONG PointGridBar::OnCurrentElementChange(WPARAM wParam, LPARAM lParam)
{
	BeginWaitCursor();
	currentNode = (Ptg3DNode *)wParam;

	if( currentNode == NULL )
	{
		EndWaitCursor();
		return TRUE;
	}

	if( !(IS_TYPE_OF(currentNode, Ptg3DNode) || IS_TYPE_OF(currentNode, PtgWorld)) )
		currentNode = (Ptg3DNode *)currentNode->getOwner();

	if( currentNode == NULL )
	{
		EndWaitCursor();
		return TRUE;
	}

	CDatatbl *nodeSheet = (CDatatbl *)GetDlgItem( IDC_NODE_GRID );
	CDatatbl *pointSheet = (CDatatbl *)GetDlgItem( IDC_POINT_GRID );
	CDatatbl *cyliSheet = (CDatatbl *)GetDlgItem( IDC_CYLINDER_GRID );
	CDatatbl *boxSheet = (CDatatbl *)GetDlgItem( IDC_BOX_GRID );

	nodeSheet->GetRowSet().Reset();
	pointSheet->GetRowSet().Reset();
	cyliSheet->GetRowSet().Reset();
	boxSheet->GetRowSet().Reset();

	POSITION pos;
	for( pos = currentNode->getMemberList().GetHeadPosition(); pos != NULL;
		currentNode->getMemberList().GetNext(pos) )
	{
	    Ptg3DNode *aNode = (Ptg3DNode *)(currentNode->getMemberList().GetAt(pos));

		if( IS_TYPE_OF( aNode, Ptg3DNode ) || IS_TYPE_OF( aNode, PtgWorld ) )
		{
			add3DObjToGrid( nodeSheet, aNode );
		}
		else if( IS_TYPE_OF( aNode, Ptg3DPointObj ) )
		{
			add3DObjToGrid( pointSheet, (Ptg3DPointObj *)aNode );
		}
		else if( IS_TYPE_OF( aNode, Ptg3DCalcPointObj ) )
		{
			add3DObjToGrid( pointSheet, (Ptg3DCalcPointObj *)aNode );
		}
		else if( IS_TYPE_OF( aNode, Ptg3DCylinderObj ) )
		{
			add3DObjToGrid( cyliSheet, (Ptg3DCylinderObj *)aNode );
		}
		else if( IS_TYPE_OF( aNode, Ptg3DBoxObj ) )
		{
			add3DObjToGrid( boxSheet, (Ptg3DBoxObj *)aNode );
		}
	}

	EndWaitCursor();
	return TRUE;
}

LONG PointGridBar::OnUnitsChange(WPARAM wParam, LPARAM lParam)
{
	BeginWaitCursor();
	// Update units for point grid.
	CDatatbl *dataTable = (CDatatbl *)GetDlgItem(IDC_POINT_GRID);
	ASSERT(dataTable);

#if FALSE
	for( long i = 0; i < dataTable->GetRowSet().GetCount(); i++ )
	{
		CDTData row = dataTable->GetRowSet().GetItem(i).GetValue();
		GridRowData rowData;

		DataTableToGridRowData( row, rowData );

		UpdateUnits(&rowData);

		GridRowDataToDataTable( rowData, row );

		dataTable->GetRowSet().GetItem(i).SetValue(row);
	}

	dataTable->InvalidateRect( NULL, TRUE );
#endif

	// Update units for primitive grids.
	OnCurrentElementChange( (WPARAM)currentNode, (LPARAM)NULL );

	EndWaitCursor();

	return TRUE;
}

LONG PointGridBar::OnNew3DElement(WPARAM wParam, LPARAM lParam)
{
	Ptg3DNode *aNode = (Ptg3DNode *)wParam;

	CDatatbl *nodeSheet = (CDatatbl *)GetDlgItem( IDC_NODE_GRID );
	CDatatbl *pointSheet = (CDatatbl *)GetDlgItem( IDC_POINT_GRID );
	CDatatbl *cyliSheet = (CDatatbl *)GetDlgItem( IDC_CYLINDER_GRID );
	CDatatbl *boxSheet = (CDatatbl *)GetDlgItem( IDC_BOX_GRID );

	if( IS_TYPE_OF( aNode, Ptg3DNode ) || IS_TYPE_OF( aNode, PtgWorld ) )
		add3DObjToGrid( nodeSheet, aNode );
	else if( IS_TYPE_OF( aNode, Ptg3DPointObj ) )
		add3DObjToGrid( pointSheet, (Ptg3DPointObj *)aNode );
	else if( IS_TYPE_OF( aNode, Ptg3DCalcPointObj ) )
		add3DObjToGrid( pointSheet, (Ptg3DCalcPointObj *)aNode );
	else if( IS_TYPE_OF( aNode, Ptg3DCylinderObj ) )
		add3DObjToGrid( cyliSheet, (Ptg3DCylinderObj *)aNode );
	else if( IS_TYPE_OF( aNode, Ptg3DBoxObj ) )
		add3DObjToGrid( boxSheet, (Ptg3DBoxObj *)aNode );

	return TRUE;
}

int add3DObjToGrid( CDatatbl *theTable, Ptg3DNode *theNode )
{
	CDTData rowData;
	VECTOR vTMatrix = theNode->getMatrix(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
	char tempStr[256];

	rowData.CreateArray( NODEGRID_NUMCOLUMNS );

	rowData.PutItem( theNode->getName(), NODEGRID_NAME );
	rowData.PutItem( (long)theNode, NODEGRID_NODEPTR );

	DoubleToUnitsString( vTMatrix[10], tempStr );	rowData.PutItem( tempStr, NODEGRID_POSEAST );
	DoubleToUnitsString( vTMatrix[11], tempStr );	rowData.PutItem( tempStr, NODEGRID_POSNORTH );
	DoubleToUnitsString( vTMatrix[12], tempStr );	rowData.PutItem( tempStr, NODEGRID_POSUP );

	theTable->GetRowSet().Add(dtAfterLast, rowData);

	free_vector( vTMatrix, 1, 12 );

	return TRUE;
}

int add3DObjToGrid( CDatatbl *theTable, Ptg3DPointObj *thePoint )
{
	CDTData rowData;
	char tempStr[256];

	VECTOR vTMatrix = thePoint->getMatrix(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
	VECTOR vPoint = thePoint->getPosition(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
//	VECTOR vXformPoint = matpnt( vTMatrix, vPoint );

	rowData.CreateArray(POINTGRID_NUMCOLUMNS);

	strcpy( tempStr, thePoint->getName() );

	rowData.PutItem( thePoint->getName(), POINTGRID_NAME );
	DoubleToUnitsString( vPoint[1], tempStr );	rowData.PutItem( tempStr, POINTGRID_POSX );
	DoubleToUnitsString( vPoint[2], tempStr );	rowData.PutItem( tempStr, POINTGRID_POSY );
	DoubleToUnitsString( vPoint[3], tempStr );	rowData.PutItem( tempStr, POINTGRID_POSZ );
	rowData.PutItem( (long)thePoint, POINTGRID_NODEPTR );

	theTable->GetRowSet().Add(dtAfterLast, rowData);

//	free_vector( vXformPoint, 1, 3 );
	free_vector( vPoint, 1, 3 );
	free_vector( vTMatrix, 1, 12 );

	return TRUE;
}

int add3DObjToGrid( CDatatbl *theTable, Ptg3DCalcPointObj *thePoint )
{
	CDTData rowData;
	char tempStr[256];

	VECTOR vTMatrix = thePoint->getMatrix(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
	VECTOR vPoint = thePoint->getPosition(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
//	VECTOR vXformPoint = matpnt( vTMatrix, vPoint );

	rowData.CreateArray(POINTGRID_NUMCOLUMNS);

	strcpy( tempStr, thePoint->getName() );

	rowData.PutItem( thePoint->getName(), POINTGRID_NAME );
	DoubleToUnitsString( vPoint[1], tempStr );	rowData.PutItem( tempStr, POINTGRID_POSX );
	DoubleToUnitsString( vPoint[2], tempStr );	rowData.PutItem( tempStr, POINTGRID_POSY );
	DoubleToUnitsString( vPoint[3], tempStr );	rowData.PutItem( tempStr, POINTGRID_POSZ );
	free_vector( vPoint, 1, 3 );
//	free_vector( vXformPoint, 1, 3 );

	vPoint = thePoint->getStdDev();
	rotpnt( vTMatrix, vPoint );
	rowData.PutItem( vPoint[1], POINTGRID_STDDEV_X );
	rowData.PutItem( vPoint[2], POINTGRID_STDDEV_Y );
	rowData.PutItem( vPoint[3], POINTGRID_STDDEV_Z );
	free_vector( vPoint, 1, 3 );

	rowData.PutItem( thePoint->getRayMiss(), POINTGRID_RAYMISS );

	rowData.PutItem( (long)thePoint, POINTGRID_NODEPTR );

	theTable->GetRowSet().Add(dtAfterLast, rowData);

	free_vector( vTMatrix, 1, 12 );

	return TRUE;
}

int add3DObjToGrid( CDatatbl *theTable, Ptg3DCylinderObj *theCyli )
{
	CDTData rowData;
	char tempStr[256];
printf( "add3DObjToGrid - Entering for Cyli '%s'\n", theCyli->getName() );

	VECTOR vTMatrix = theCyli->getMatrix(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
for( int i = 1; i <= 12; i++ ) printf( "vTMatrix[%d] = %lf\n", i, vTMatrix[i] );

	rowData.CreateArray(CYLIGRID_NUMCOLUMNS);

	VECTOR v1 = vector( 1, 3 );
	v1[1] = 0.0;	v1[2] = 0.0;	v1[3] = theCyli->getHeight() / 2.0;
	VECTOR vBegPnt = matpnt( vTMatrix, v1 );
for( i = 1; i <= 3; i++ ) printf( "vBegPnt[%d] = %lf\n", i, vBegPnt[i] );

	v1[3] = -v1[3];
	VECTOR vEndPnt = matpnt( vTMatrix, v1 );
for( i = 1; i <= 3; i++ ) printf( "vEndPnt[%d] = %lf\n", i, vEndPnt[i] );
	free_vector( v1, 1, 3 );

	rowData.PutItem( theCyli->getName(), CYLIGRID_NAME );
	DoubleToUnitsString( theCyli->getRadius() * 2.0 , tempStr );	rowData.PutItem( tempStr, CYLIGRID_DIAMETER );
	DoubleToUnitsString( theCyli->getHeight(), tempStr );			rowData.PutItem( tempStr, CYLIGRID_HEIGHT );
	DoubleToUnitsString( vTMatrix[10], tempStr );	rowData.PutItem( tempStr, CYLIGRID_CENTERPOSEAST );
	DoubleToUnitsString( vTMatrix[11], tempStr );	rowData.PutItem( tempStr, CYLIGRID_CENTERPOSNORTH );
	DoubleToUnitsString( vTMatrix[12], tempStr );	rowData.PutItem( tempStr, CYLIGRID_CENTERPOSUP );

	DoubleToUnitsString( vBegPnt[1], tempStr );	rowData.PutItem( tempStr, CYLIGRID_BEGEAST );
	DoubleToUnitsString( vBegPnt[2], tempStr );	rowData.PutItem( tempStr, CYLIGRID_BEGNORTH );
	DoubleToUnitsString( vBegPnt[3], tempStr );	rowData.PutItem( tempStr, CYLIGRID_BEGUP );

	DoubleToUnitsString( vEndPnt[1], tempStr );	rowData.PutItem( tempStr, CYLIGRID_ENDEAST );
	DoubleToUnitsString( vEndPnt[2], tempStr );	rowData.PutItem( tempStr, CYLIGRID_ENDNORTH );
	DoubleToUnitsString( vEndPnt[3], tempStr );	rowData.PutItem( tempStr, CYLIGRID_ENDUP );

	rowData.PutItem( (long)theCyli, CYLIGRID_NODEPTR );

	theTable->GetRowSet().Add(dtAfterLast, rowData);

	free_vector( vTMatrix, 1, 12 );
	free_vector( vBegPnt, 1, 3 );
	free_vector( vEndPnt, 1, 3 );
printf( "\n" );
	return TRUE;
}

int add3DObjToGrid( CDatatbl *theTable, Ptg3DBoxObj *theBox )
{
	CDTData rowData;
	char tempStr[256];

	VECTOR vTMatrix = theBox->getMatrix(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);

	rowData.CreateArray(BOXGRID_NUMCOLUMNS);

	rowData.PutItem( theBox->getName(), BOXGRID_NAME );
	DoubleToUnitsString( theBox->getXLen(), tempStr );	rowData.PutItem( tempStr, BOXGRID_LENGTH );
	DoubleToUnitsString( theBox->getYLen(), tempStr );	rowData.PutItem( tempStr, BOXGRID_WIDTH );
	DoubleToUnitsString( theBox->getZLen(), tempStr );	rowData.PutItem( tempStr, BOXGRID_HEIGHT );
	DoubleToUnitsString( vTMatrix[10], tempStr );	rowData.PutItem( tempStr, BOXGRID_CENTERPOSEAST );
	DoubleToUnitsString( vTMatrix[11], tempStr );	rowData.PutItem( tempStr, BOXGRID_CENTERPOSNORTH );
	DoubleToUnitsString( vTMatrix[12], tempStr );	rowData.PutItem( tempStr, BOXGRID_CENTERPOSUP );
	
	theTable->GetRowSet().Add(dtAfterLast, rowData);

	free_vector( vTMatrix, 1, 12 );

	return TRUE;
}

int update3DObjInGrid( CDatatbl *theTable, Ptg3DPointObj *thePoint, long rowNum )
{
	// Didn't know anything, not gonna do anything!!!!!
	if( thePoint == NULL && rowNum == -1 )
	{
		AfxMessageBox( "Didn't tell me anything to update!!!!!" );
		return FALSE;
	}

	// Didn't know the node, got to get it
	if( thePoint == NULL )
	{
		CDTData row = theTable->GetRowSet().GetItem(rowNum).GetValue();
		row.GetItem( (long *)(&thePoint), POINTGRID_NODEPTR );
	}

	// Still don't have node, even after trying to get it, so bomb
	if( thePoint == NULL )
	{
//		char errorMsg[256];
//		sprintf( errorMsg, "Row %ld has NULL node pointer", rowNum );
//		AfxMessageBox( errorMsg );

		return FALSE;
	}

	// Didn't know row number, must find it
	if( rowNum == -1 )
	{
		for( long i = 0; i < theTable->GetRowSet().GetCount(); i++ )
		{
			CDTData row = theTable->GetRowSet().GetItem(i).GetValue();
			Ptg3DPointObj *testPoint;
			row.GetItem( (long *)(&testPoint), POINTGRID_NODEPTR );

			if( thePoint == testPoint )
				rowNum = i;
		}
	}

	// Didn't know row number, and couldn't find it either
	if( rowNum == -1 )
	{
		char errorMsg[256];
		sprintf( errorMsg, "Cannot find '%s' in grid", thePoint->getName() );
		AfxMessageBox( errorMsg );

		return FALSE;
	}

///////////////
	CDTData rowData;
	char tempStr[256];

//	VECTOR vTMatrix = thePoint->getMatrix(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
	VECTOR vPoint = thePoint->getPosition(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
//	VECTOR vXformPoint = matpnt( vTMatrix, vPoint );

	rowData.CreateArray(POINTGRID_NUMCOLUMNS);

	rowData.PutItem( thePoint->getName(), POINTGRID_NAME );
	DoubleToUnitsString( vPoint[1], tempStr );	rowData.PutItem( tempStr, POINTGRID_POSX );
	DoubleToUnitsString( vPoint[2], tempStr );	rowData.PutItem( tempStr, POINTGRID_POSY );
	DoubleToUnitsString( vPoint[3], tempStr );	rowData.PutItem( tempStr, POINTGRID_POSZ );
	rowData.PutItem( (long)thePoint, POINTGRID_NODEPTR );

	if( IS_TYPE_OF( thePoint, Ptg3DCalcPointObj ) )
	{
		VECTOR v = ((Ptg3DCalcPointObj *)thePoint)->getStdDev();

		rowData.PutItem( v[1], POINTGRID_STDDEV_X );
		rowData.PutItem( v[2], POINTGRID_STDDEV_Y );
		rowData.PutItem( v[3], POINTGRID_STDDEV_Z );

		rowData.PutItem( ((Ptg3DCalcPointObj *)thePoint)->getRayMiss(), POINTGRID_RAYMISS );
	}


	long currRow = theTable->GetCurCell().GetRow();
	if( currRow != -1 && currRow != rowNum )
	{
		CDTData currRowData = theTable->GetRowSet().GetItem(currRow).GetValue();
		Ptg3DPointObj *currRowPoint;
		currRowData.GetItem( (long *)(&currRowPoint), POINTGRID_NODEPTR );

		// Current row may be a new node, in that case, just blow away the delta calcs
		if( currRowPoint )
		{
			VECTOR vCurrPoint = currRowPoint->getPosition(((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->measurementRefNode);
//			VECTOR vXformCurrPoint = matpnt( vTMatrix, vCurrPoint );

			DoubleToUnitsString( vCurrPoint[1] - vPoint[1], tempStr );
				rowData.PutItem( tempStr, POINTGRID_DELTAX );
			DoubleToUnitsString( vCurrPoint[2] - vPoint[2], tempStr );
				rowData.PutItem( tempStr, POINTGRID_DELTAY );
			DoubleToUnitsString( vCurrPoint[3] - vPoint[3], tempStr );
				rowData.PutItem( tempStr, POINTGRID_DELTAZ );

			DoubleToUnitsString(
				sqrt(
					(vCurrPoint[1] - vPoint[1]) * (vCurrPoint[1] - vPoint[1]) +
					(vCurrPoint[2] - vPoint[2]) * (vCurrPoint[2] - vPoint[2]) +
					(vCurrPoint[3] - vPoint[3]) * (vCurrPoint[3] - vPoint[3]) ),
					tempStr );
		}
		else
		{
			rowData.PutItem( "", POINTGRID_DELTAX );
			rowData.PutItem( "", POINTGRID_DELTAY );
			rowData.PutItem( "", POINTGRID_DELTAZ );

			rowData.PutItem( "", POINTGRID_DIST );
		}
		rowData.PutItem( tempStr, POINTGRID_DIST );
	}


	theTable->GetRowSet().GetItem(rowNum).SetValue(rowData);

//	free_vector( vTMatrix, 1, 12 );
	free_vector( vPoint, 1, 3 );
//	free_vector( vXformPoint, 1, 3 );

///////////////

	return TRUE;
}

/*************** To go from string to MM's ****************
	if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
		*dblValue = atof(strValue);
	else
		*dblValue = 25.4 * getInches(strValue);
***********************************************************/

/*************** To go from MM's to string ****************
	if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
		sprintf(column.sz_east_begin,"%10.2f", column.east_begin);
	else
		ConvertUnits(column.sz_east_begin, 30,  column.east_begin/25.4 );
***********************************************************/

void PointGridBar::OnLfClickNodeGrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status) 
{
	CDatatbl *pointGrid = (CDatatbl *)GetDlgItem( IDC_NODE_GRID );
	if( ClickPos == posOnHeading )
	{
		pointGrid->GetColumnSet().SetSortEnabled(TRUE);
		pointGrid->GetColumnSet().SetSortKey(sortPrimary,Column);
		pointGrid->GetColumnSet().SetSortAsc(sortPrimary,TRUE);
		pointGrid->GetColumnSet().SetSortCaseI(sortPrimary,TRUE);
	}
}

void PointGridBar::OnLfClickBoxGrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status) 
{
	CDatatbl *pointGrid = (CDatatbl *)GetDlgItem( IDC_BOX_GRID );
	if( ClickPos == posOnHeading )
	{
		pointGrid->GetColumnSet().SetSortEnabled(TRUE);
		pointGrid->GetColumnSet().SetSortKey(sortPrimary,Column);
		pointGrid->GetColumnSet().SetSortAsc(sortPrimary,TRUE);
		pointGrid->GetColumnSet().SetSortCaseI(sortPrimary,TRUE);
	}
}

void PointGridBar::OnLfClickCylinderGrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status) 
{
	CDatatbl *pointGrid = (CDatatbl *)GetDlgItem( IDC_CYLINDER_GRID );
	if( ClickPos == posOnHeading )
	{
		pointGrid->GetColumnSet().SetSortEnabled(TRUE);
		pointGrid->GetColumnSet().SetSortKey(sortPrimary,Column);
		pointGrid->GetColumnSet().SetSortAsc(sortPrimary,TRUE);
		pointGrid->GetColumnSet().SetSortCaseI(sortPrimary,TRUE);
	}
}
