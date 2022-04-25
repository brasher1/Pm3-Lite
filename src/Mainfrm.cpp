// mainfrm.cpp : implementation of the CMainFrame class
//
//#define NEW_MULTI_SHEETS  //took out for shipping version

#include "stdafx.h"
//#define NO_DT_COVER
#include "fotobrow.h"
#include <math.h>
#include "fotoview.h"
#include "fotobdoc.h"
#include "mainfrm.h"
#include <datatbl.h>
#include "splash.h"
#include "process.h"

//#include <cdatatbl.h>
//#include <dtmfc.h>
#include <dtocx.h>
#include "dtrowset.h"
#include "dtrow.h"

#include "zoneeditor.h"
#include "columnfo.h"
#include "tritondb.h"
#include "selectim.h"
#include "coordxfo.h"
#include <afxdlgs.h>
#include "selecteq.h"
#include "projects.h"
#include <new.h>
#include <stdio.h>
#include "common.h"
#include "3dpoints.h"
#include "optionsd.h"
#include "createcylinderbyedgedlg.h"
#include "circlebyedge.h"
#include "elementListview.h"
#include "bundlecalcdlg.h"
#include "columneditor.h"
#include "CreateCylinderByEndpointsDlg.h"

#include "3dclass/Ptg3DNode.h"
#include "3dclass/Ptg3DCalcPointObj.h"
#include "3dclass/Ptg3DBoxObj.h"
#include "3dclass/Ptg3DConeObj.h"
#include "3dclass/Ptg3DCtorObj.h"
#include "3dclass/Ptg3DCylinderObj.h"
#include "3dclass/Ptg3DFacetObj.h"
#include "3dclass/Ptg3DLineObj.h"
#include "3dclass/Ptg3DPyramidObj.h"
#include "3dclass/Ptg3DRtorObj.h"

#include "transact.hpp"
#include "triton\triton.hpp"

#define START_NODE "Model"

//#include "zonedlg.h"
//#include "columnli.h"

double getInches( char *inputStr );

int add3DObjToGrid( CDatatbl *theTable, Ptg3DCalcPointObj *thePoint );
int importReviewModel(char *s, Ptg3DNode * );
int importSurveyData(char *s, Ptg3DNode * );

int saveDBToNode( ClassesObj *classObj, Ptg3DNode *nodeObj );
int saveNodeToDB( Ptg3DNode *nodeObj, ClassesObj *classObj );

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

HINSTANCE hDataTable;


//CDataTbl *DataTbl2 = NULL;

#define DRAGGRID_TIMER		2
int mode=0;
char str[30];

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_COMMAND(ID_VIEW_GRAPHIC, OnViewGraphic)
	ON_WM_SETCURSOR()
	ON_COMMAND(IDC_MEASURE, OnMeasure)
	ON_UPDATE_COMMAND_UI(IDC_MEASURE, OnUpdateViews)
	ON_WM_SIZE()
	ON_COMMAND(ID_SETTINGS_OPTIONS, OnSettingsOptions)
	ON_COMMAND(ID_EDIT_ZONES, OnEditZones)
	ON_COMMAND(ID_EDIT_COLUMNS, OnEditColumns)
	ON_COMMAND(ID_EDIT_IMAGES, OnEditImages)
	ON_COMMAND(ID_EDIT_TRANSFORMATIONS, OnEditTransformations)
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_COMMAND(ID_ACCEL_COPYGRID, OnAccelCopygrid)
	ON_COMMAND(ID_EDIT_EQUIPDATA, OnEditEquipdata)
	ON_COMMAND(ID_EDIT_EQUIPIMAGES, OnEditEquipimages)
	ON_COMMAND(ID_FILE_PROJECTS, OnFileProjects)
	ON_WM_PAINT()
	ON_UPDATE_COMMAND_UI(ID_EDIT_EQUIPDATA, OnUpdateEditEquipdata)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_RAYS, OnRays)
	ON_COMMAND(IDC_OVERLAY, OnOverlay)
	ON_COMMAND(ID_MEASURE_CYLINDERBYEDGE, OnMeasureCylinderbyedge)
	ON_UPDATE_COMMAND_UI(IDC_RAYS, OnUpdateRays)
	ON_UPDATE_COMMAND_UI(IDC_OVERLAY, OnUpdateOverlay)
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MEASURE_CIRCLEBYEDGE, OnMeasureCirclebyedge)
	ON_COMMAND(ID_MEASURE_CIRCLEBY3DPOINT, OnMeasureCircleby3dpoint)
	ON_COMMAND(ID_VIEW_ELEMENTLIST, OnViewElementlist)
	ON_COMMAND(ID_EDIT_BUNDLECALCULATIONS, OnEditBundlecalculations)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_FILE_IMPORT_SURVEYDATA, OnFileImportSurveyData)
	ON_COMMAND(ID_FILE_IMPORT_REVIEWMODEL, OnFileImportReviewModel)
	ON_COMMAND(ID_MEASURE_QUICKCYLINDER, OnMeasureQuickCylinder)
	ON_COMMAND(ID_MEASURE_CYLINDERBYENDPOINTS, OnMeasureCylinderbyEndpoints)
	ON_COMMAND(ID_FILE_SAVE_WORK, OnFileSaveWork)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_CIRCLEBY3DPOINT, OnUpdateViews)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_CIRCLEBYEDGE, OnUpdateViews)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_CYLINDERBYEDGE, OnUpdateViews)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_QUICKCYLINDER, OnUpdateViews)
	ON_COMMAND(ID_FILE_GET_WORK, OnFileGetWork)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_INDEX, CMDIFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP_USING, CMDIFrameWnd::OnHelpUsing)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpIndex)
//	ON_CONTROL(DTN_LFCLICK, IDD_3DPOINTTABLE, OnGridLeftClick)
	ON_CONTROL(WM_LBUTTONUP, IDC_SPREADSHEET_TAGS, OnSheetTab )
	ON_COMMAND(ID_KILL_CREATE_CYLINDER, OnKillCreateCylinder )
	ON_MESSAGE(UM_CURRENT_ELEMENT_CHANGE, OnCurrentElementChange )
	ON_MESSAGE(UM_UNITS_CHANGE, OnUnitsChange )
	ON_MESSAGE(UM_NEW_TREE_NODE, OnNewTreeNode )
	ON_MESSAGE(UM_NEW_3D_ELEMENT, OnNew3DElement )
//	ON_CONTROL(WM_MOUSEMOVE, IDD_3DPOINTTABLE, OnGridMouseMove)
END_MESSAGE_MAP()
//BEGIN_EVENTSINK_MAP
/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars
	
// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_WINDOW_CASCADE,
	ID_WINDOW_TILE_HORZ,
	ID_WINDOW_TILE_VERT,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
	ID_CONTEXT_HELP,
};
static UINT BASED_CODE ImageButtons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	IDC_SELECT,
	IDC_ZOOMIN,
	IDC_ZOOMOUT,
	IDC_CENTER,
		ID_SEPARATOR,
	IDC_OVERLAY,
	IDC_RAYS,
		ID_SEPARATOR,
	ID_VIEW_DEFINEREGION,
	ID_VIEW_DELETEREGION,
	ID_VIEW_LABELREGION,
		ID_SEPARATOR,
	ID_EDIT_EQUIPDATA,
		ID_SEPARATOR,
	IDC_ORTHO,
	IDC_PERSPECTIVE
};

static UINT BASED_CODE GraphicButtons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	IDC_MEASURE
//	IDC_CAMERA,
//	IDC_POINT
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

//Thread function to load cameras
void InitWorld(void *data)
{
	((PtgWorld *)data)->InitWorld(NULL);
//	return 0;
}

int checkModelLocation( Ptg3DNode *testNode )
{
	Ptg3DNode *modelNode = GETWORLD.findByName( "Model", TRUE );
	if( modelNode == NULL )
	{
		AfxMessageBox( "Unable to locate node for modeling." );
		return FALSE;
	}

	Ptg3DNode *huntingTestNode = modelNode->findByName( testNode->getName(), TRUE );
	if( huntingTestNode != testNode )
	{
		AfxMessageBox( "Current node is not child of a node that is compatible with modeling." );
		return FALSE;
	}

	return TRUE;
}

int DataTableToGridRowData( CDTData &dtRow, GridRowData &grdRow )
{
	dtRow.GetItem(grdRow.name,				0 );
	dtRow.GetItem(&(grdRow.x),				12 );
	dtRow.GetItem(&(grdRow.y),				13 );
	dtRow.GetItem(&(grdRow.z),				14 );
	dtRow.GetItem(&(grdRow.dx),				15 );
	dtRow.GetItem(&(grdRow.dy),				16 );
	dtRow.GetItem(&(grdRow.dz),				17 );
	dtRow.GetItem(&(grdRow.dist),			18 );
	dtRow.GetItem(grdRow.sz_x,				1 );
	dtRow.GetItem(grdRow.sz_y,				2 );
	dtRow.GetItem(grdRow.sz_z,				3 );
	dtRow.GetItem(grdRow.sz_dx,				4 );
	dtRow.GetItem(grdRow.sz_dy,				5 );
	dtRow.GetItem(grdRow.sz_dz,				6 );
	dtRow.GetItem(grdRow.sz_dist,			7 );
	dtRow.GetItem(&(grdRow.std_dev_x),		8 );
	dtRow.GetItem(&(grdRow.std_dev_y),		9 );
	dtRow.GetItem(&(grdRow.std_dev_z),		10 );
	dtRow.GetItem(&(grdRow.ray_miss_dist),	11 );

	return 0;
}

int GridRowDataToDataTable( GridRowData &grdRow, CDTData &dtRow )
{
	dtRow.PutItem(grdRow.name,			0 );
	dtRow.PutItem(grdRow.x,				12 );
	dtRow.PutItem(grdRow.y,				13 );
	dtRow.PutItem(grdRow.z,				14 );
	dtRow.PutItem(grdRow.dx,			15 );
	dtRow.PutItem(grdRow.dy,			16 );
	dtRow.PutItem(grdRow.dz,			17 );
	dtRow.PutItem(grdRow.dist,			18 );
	dtRow.PutItem(grdRow.sz_x,			1 );
	dtRow.PutItem(grdRow.sz_y,			2 );
	dtRow.PutItem(grdRow.sz_z,			3 );
	dtRow.PutItem(grdRow.sz_dx,			4 );
	dtRow.PutItem(grdRow.sz_dy,			5 );
	dtRow.PutItem(grdRow.sz_dz,			6 );
	dtRow.PutItem(grdRow.sz_dist,		7 );
	dtRow.PutItem(grdRow.std_dev_x,		8 );
	dtRow.PutItem(grdRow.std_dev_y,		9 );
	dtRow.PutItem(grdRow.std_dev_z,		10 );
	dtRow.PutItem(grdRow.ray_miss_dist,	11 );

	return 0;
}

CMainFrame::CMainFrame()
{
	m_PointBox = FALSE;
	m_rays = FALSE;
	m_overlay = FALSE;
	m_multiSelect = FALSE;
	pCylDlg = NULL;
	pCirDlg = NULL;
	pCir3DDlg = NULL;

	//	pts = NULL;
}

CMainFrame::~CMainFrame()
{
	FreeLibrary(hDataTable);

//	if(pts )
//	{
//		delete pts;
//		pts = NULL;
//	}
#ifdef __THISISTOSTARTMAINTSYSTEM
	if( m_pInfo.hProcess )
		TerminateProcess(m_pInfo.hProcess, 0);
#endif //__THISISTOSTARTMAINTSYSTEM
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
	//	!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
	//	!m_wndToolBar.SetButtons(buttons,
	//	  sizeof(buttons)/sizeof(UINT)))
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		AfxMessageBox("Status Bar failed to create");
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_rays = m_overlay = TRUE; // Must set TRUE for common functions to work
	OnRays();
	OnOverlay();
	/*
	if (!m_wndEditBar.Init(this,m_bColor,m_bToolTips))
	{
		TRACE0("Failed to create editbar\n");
		return -1;      // fail to create
	}
  */
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

//	m_wndEditBar.SetWindowText(_T("Edit"));
//	m_wndEditBar.EnableDocking(CBRS_ALIGN_ANY /*| CBRS_FLOAT_MULTI*/);
//	DockControlBarLeftOf(&m_wndEditBar,&m_wndToolBar);
//#define NO_IMAGE_BAR
#ifndef NO_IMAGE_BAR
	if (!m_wndImageToolBar.Create(this, WS_CHILD | WS_VISIBLE |
			CBRS_TOP | ((m_bToolTips)?(CBRS_TOOLTIPS | CBRS_FLYBY):0)/*, IDW_BROWSE_BAR*/) ||
//		!m_wndImageToolBar.LoadBitmap(
//			IDR_VIEWS ) ||
//		!m_wndImageToolBar.SetButtons(ImageButtons, sizeof(ImageButtons)/sizeof(UINT)))
		!m_wndImageToolBar.LoadToolBar(IDR_VIEWS))
	{
		TRACE0("Failed to create ImageToolbar\n");
		return -1;      // fail to create
	}
	m_wndImageToolBar.SetWindowText(_T("View"));
	m_wndImageToolBar.EnableDocking(CBRS_ALIGN_ANY /*| CBRS_FLOAT_MULTI*/);
	DockControlBarLeftOf(&m_wndImageToolBar,&m_wndToolBar);
#endif
	if (!m_wndGraphicsToolBar.Create(this, WS_CHILD | WS_VISIBLE |
			CBRS_TOP | ((m_bToolTips)?(CBRS_TOOLTIPS | CBRS_FLYBY):0)/*, IDW_BROWSE_BAR*/) ||
//		!m_wndGraphicsToolBar.LoadBitmap(
//			IDR_MEASURE ) ||
//		!m_wndGraphicsToolBar.SetButtons(GraphicButtons, sizeof(GraphicButtons)/sizeof(UINT)))
		!m_wndGraphicsToolBar.LoadToolBar(IDR_MEASURE))
	{
		TRACE0("Failed to create ImageToolbar\n");
		return -1;      // fail to create
	}
	m_wndGraphicsToolBar.SetWindowText(_T("Measure"));
	m_wndGraphicsToolBar.EnableDocking(CBRS_ALIGN_ANY /*| CBRS_FLOAT_MULTI*/);
	DockControlBarLeftOf(&m_wndGraphicsToolBar,&m_wndToolBar);
	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
//	m_wndEditBar.SetBarStyle(m_wndEditBar.GetBarStyle() |
//		CBRS_TOOLTIPS | CBRS_FLYBY);
#ifndef NO_IMAGE_BAR
	m_wndImageToolBar.SetBarStyle(m_wndImageToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
#endif
	m_wndGraphicsToolBar.SetBarStyle(m_wndGraphicsToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

//	hDataTable = LoadLibrary("DTBL32.DLL");	
//	if( (UINT)hDataTable == NULL )
//	{
//		char temp[128];
//		sprintf(temp,"My error code from DTBL32.DLL LoadLibrary is %d", GetLastError() );
//		AfxMessageBox(temp);
//		return -1;
//	}
						//CDialogBar
	if( m_wnd3DPoints.Create(this, CBRS_BOTTOM ) == 0 )
		AfxMessageBox( "Unable to create 3D points dialog" );
	m_wnd3DPoints.EnableDocking(CBRS_ALIGN_ANY);
//	m_wnd3DPoints.m_bAutoDelete = 1;

///////////////////////////////////////////////////////////////////////
    CBitmap * pBmpOld;
        
    // Load bitmap resource - remember to call DeleteObject when done.
    VERIFY( m_bmpABD.LoadBitmap( IDB_PM3_BACKGROUND ) );

    // Get bitmap information    
    m_bmpABD.GetObject( sizeof(BITMAP), &m_bmInfo );
    m_size.cx = m_bmInfo.bmWidth;
    m_size.cy = m_bmInfo.bmHeight;

    // Hard code upper left position to place bitmap
    m_pt.x = 10;
    m_pt.y = 50;
    
    // Get temporary DC for dialog - Will be released in dc destructor
    CClientDC dc(this);
    
    // Create compatible memory DC using the dialogs DC
    VERIFY( m_dcMem.CreateCompatibleDC( &dc ) );
    
    // Select circle bitmap into DC.  
    // Get pointer to original bitmap
    // NOTE! This is temporary - save the handle instead
    VERIFY( pBmpOld = m_dcMem.SelectObject( &m_bmpABD ) );
    
    VERIFY( m_hBmpOld = (HBITMAP) pBmpOld->GetSafeHandle() );

    // local CCLientDC destructor for 'dc' will release device context
//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

///////////////////////////////////////////////////////////////////////
#ifdef __THISISTOSTARTMAINTSYSTEM
	memset ( &m_st, 0, sizeof(m_st));
	
	m_st.cb = sizeof(m_st);
	m_st.wShowWindow = SW_MINIMIZE;

	CreateProcess(NULL,"maintsys PM3LITE",NULL,NULL,TRUE,NORMAL_PRIORITY_CLASS,
			NULL,NULL,&m_st, &m_pInfo );
#endif
#if FALSE
	///////////////////////////////////////////////////////////////////////
AfxMessageBox("CMainFrame::CMainFrame() - 6");
	CDataTbl *DataTable;
//	AfxMessageBox("Left mouse click");
	DataTable = (CDataTbl *)m_wnd3DPoints.GetDlgItem(IDD_3DPOINTGRID);
	ASSERT(DataTable);
/*	if( DataTable->SetWndStyle(WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|WS_HSCROLL|
			DTS_OWNERDEF|DTS_EDIT|DTS_RESIZE|
			DTS_ROWNUMBERS|DTS_VGRID|DTS_HGRID) != DTE_OKAY )
		AfxMessageBox("SetWndStyle Failed");*/
	DWORD style = DataTable->SendMessage(DTM_GETWNDSTYLE,0,0L);
	if( DataTable->SetWndStyle( style|
			DTS_OWNERDEF|DTS_EDIT|DTS_RESIZE|
			DTS_ROWNUMBERS|DTS_VGRID|DTS_HGRID) != DTE_OKAY )
		AfxMessageBox("SetWndStyle Failed");
	DTCOLORSTRUCT Color;
	Color.BkgrdColor = RGB(255,255,255);
	Color.TextColor = RGB(0,0,0);
	Color.SelBkgrdColor = RGB(0,0,128);
	Color.SelTextColor = RGB(0,255,255);
	DataTable->SendMessage(DTM_SETDTCOLOR,0,(LPARAM)&Color);
	DataTable->SendMessage(DTM_SETGRIDCOLOR,0,RGB(255,0,0));

	DataTable = (CDataTbl *)m_wnd3DPoints.GetDlgItem(IDD_3DPOINTGRID);
	ASSERT(DataTable);
#else
	createTheSheets();
#endif	

#if FALSE
	CTabCtrl *sheet_tabs = (CTabCtrl *)m_wnd3DPoints.GetDlgItem(IDC_SPREADSHEET_TAGS);
	ASSERT(sheet_tabs);
	TC_ITEM tc;

	tc.mask = TCIF_TEXT|TCIF_PARAM;
	tc.pszText = "Points";
	tc.lParam = 0;
	sheet_tabs->InsertItem(0, &tc);
	tc.pszText = "Cylinders";
	tc.lParam = 1;
	sheet_tabs->InsertItem(1, &tc);
	tc.pszText = "Circles";
	tc.lParam = 2;
	sheet_tabs->InsertItem(2, &tc);
	tc.pszText = "Targets";
	tc.lParam = 3;
	sheet_tabs->InsertItem(3, &tc);
	tc.pszText = "Cameras";
	tc.lParam = 4;
	sheet_tabs->InsertItem(4, &tc);
#endif
	
	//DWORD  ThreadId;
	//CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)InitWorld,&ptg3DWorld,0,&ThreadId); 
	DWORD hThread;

	measurementRefNode = (Ptg3DNode *)&(ptg3DWorld);	// Initially, everything relative to world
	hThread = _beginthread(InitWorld, 0, &ptg3DWorld);
/*	SetEvent
HANDLE CreateEvent(
    LPSECURITY_ATTRIBUTES lpEventAttributes,	// pointer to security attributes  
    BOOL bManualReset,	// flag for manual-reset event 
    BOOL bInitialState,	// flag for initial state 
    LPCTSTR lpName 	// pointer to event-object name  
   );	

*/

	CSplashWnd::ShowSplashScreen(this);

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, "WaitFor3DWorld");
	WaitForSingleObject(hEvent,INFINITE);

//	while( GetThreadPriority((HANDLE)hThread) != THREAD_PRIORITY_ERROR_RETURN )
//		Sleep(300);

	//
	// Initialize current node to something...prefereably somewhere that we'd like it to be
	//
	currentElem = ptg3DWorld.findByName( "Model" );
//	currentElem = (Ptg3DNode *)&(ptg3DWorld);  //Set the current element to the World Node
	SendMessage(UM_CURRENT_ELEMENT_CHANGE, (WPARAM)currentElem, (LPARAM)NULL);

	MessageBeep(MB_OK);
	return TRUE;
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

void CMainFrame::CreateOrActivateFrame(CDocTemplate* pTemplate,
	CRuntimeClass* pViewClass)
{
	// If a check view or book view (specified by pViewClass)
	// already exists, then activate the MDI child window containing
	// the view.  Otherwise, create a new view for the document.
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
		CString s1;
		CString s2;
		pDoc->GetDocTemplate()->GetDocString(s1, CDocTemplate::regFileTypeId );
		pTemplate->GetDocString(s2, CDocTemplate::regFileTypeId );
		if( s1 == s2 )
		{
			TRACE("Matching Strings 1-%s,  2-%s\n", (const char *)s1,(const char *)s2);
			TRACE("The Document is already opened\n");
			flag = TRUE;
			break;
//			goto Exists;
		}
	}
	CMDIChildWnd* pNewFrame;
	if( flag )
	{
		TRACE("The Document exists and making a new frame\n");
		pNewFrame = (CMDIChildWnd*)(pTemplate->CreateNewFrame(pDoc, NULL));
		ASSERT(pNewFrame);
		TRACE("Calling Initial Update\n");
		pTemplate->InitialUpdateFrame(pNewFrame, pDoc, TRUE);
	}
	else
	{
		TRACE("New Doc creating a new frame\n");
		pDoc = pTemplate->OpenDocumentFile(NULL);
		pNewFrame = MDIGetActive();
		ASSERT(pNewFrame);
	}
	if (pNewFrame == NULL)
		return;     // not created
	ASSERT(pNewFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));
	pList->RemoveAll();
	delete pList;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
//	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
#ifndef NO_PALETTE_JUNK	
	CMDIFrameWnd::OnPaletteChanged(pFocusWnd);
	
	SendMessageToDescendants( WM_PALETTECHANGED,
								(WPARAM)(pFocusWnd->GetSafeHwnd()),
								(LPARAM)0, TRUE );
#endif
}

BOOL CMainFrame::OnQueryNewPalette() 
{
	// TODO: Add your message handler code here and/or call default
    // Tell the currently active view to realize in the foreground.
    CView* pView = GetActiveView();
    if (!pView) 
		return FALSE;
#ifndef NO_PALETTE_JUNK	
    int i = pView->SendMessage(WM_QUERYNEWPALETTE,
                              (WPARAM)0,
                              (LPARAM)0);
    // If the mapping didn't change tell all the other 
    // windows that they might want to repaint anyway.
    if (!i) {
        SendMessageToDescendants(WM_PALETTECHANGED,
                                 (WPARAM)(pView->GetSafeHwnd()),
                                 (LPARAM)0,
                                 TRUE);
    } 
    return (BOOL) i;
#else	
	return FALSE;
#endif
}


void CMainFrame::OnViewToolbar() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnViewGraphic() 
{
	// TODO: Add your command handler code here
	///////////////////////////////////////////////////////////////////////
//	CreateOrActivateFrame(((CFotobrowApp *)AfxGetApp())->m_pGraphicTemplate,
//		RUNTIME_CLASS(CFotoView));
	///////////////////////////////////////////////////////////////////////
	CDocument *pDoc;
	CObList *pList = new CObList;
	int flag = FALSE;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
	TRACE("Got the DOC List\n");
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{		
//		AfxMessageBox("A Doucment");
		TRACE("Got the next DOC\n");
		pDoc = (CDocument *)(pList->GetNext(pos));
		POSITION pos2 = pDoc->GetFirstViewPosition();
		CView *pView;
		while(  pos2!=NULL )
		{
			pView=pDoc->GetNextView(pos2);
			
//			AfxMessageBox("Another View");
			if( pView->IsKindOf(RUNTIME_CLASS(CFotoView)) )
			{
//				AfxMessageBox("Setting Active Window");
				(pView->GetParent())->BringWindowToTop();
				goto End;
			}
		}
	}
//	AfxMessageBox("Creating a grapohic view");
	CreateOrActivateFrame(((CFotobrowApp *)AfxGetApp())->m_pGraphicTemplate,
		RUNTIME_CLASS(CFotoView));
			
End:
//	AfxMessageBox("End");
	pList->RemoveAll();
	delete pList;
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if( hMainCursor == NULL )
		return CMDIFrameWnd::OnSetCursor(pWnd, nHitTest, message);
	else
	{
		::SetCursor(hMainCursor);
		return TRUE;
	}
}

void CMainFrame::OnMeasure() 
{
	CDatatbl *DataTable;
	char *p;

	if( !m_PointBox )
	{
		m_PointBox = TRUE;
//		m_wnd3DPoints.EnableDocking(CBRS_ALIGN_ANY);
		//DockControlBar(&m_wnd3DPoints);
//		RecalcLayout();
//		InvalidateRect(NULL);
	}
	

	_Point3d ptg[3];
	p = SearchForBundle();
	if( p != NULL )		
	{
		if( Calc3DPoint(p, ptg ) )
		{
			AfxMessageBox("Not Enough Points Selected");
			return;
		}

		char szTemp[128];
		sprintf( szTemp, "x,y,z = %lf,%lf,%lf\n", ptg[0].x, ptg[0].y, ptg[0].z );



//AfxMessageBox( szTemp );
		
		GridRowData row;
		memset(&row, 0, sizeof(row));
/*
		double *TRANS = ((double *)((CFotobrowApp *)(AfxGetApp()))->GetTransform());

		row.x = (TRANS[0]*ptg[0].x + TRANS[1]*ptg[0].y + TRANS[2]*ptg[0].z + TRANS[9] );
		row.y = (TRANS[3]*ptg[0].x + TRANS[4]*ptg[0].y + TRANS[5]*ptg[0].z + TRANS[10] );
		row.z = (TRANS[6]*ptg[0].x + TRANS[7]*ptg[0].y + TRANS[8]*ptg[0].z + TRANS[11] );
*/
//		ConvertPoint(&ptg[0],&ptg[0]);

		row.x = ptg[0].x;
		row.y = ptg[0].y;
		row.z = ptg[0].z;

		row.std_dev_x = ptg[1].x;
		row.std_dev_y = ptg[1].y;
		row.std_dev_z = ptg[1].z;
		row.ray_miss_dist = ptg[2].x;

		UpdateUnits(&row);

/* Old datatable code
		DataTable = (CDatatbl *)m_wnd3DPoints.GetDlgItem(IDC_3DPOINTGRID);
		ASSERT(DataTable);

		DataTable->SetAccessPos(0L,(ULONG)DTPOS_AFTERLAST);

		long rw = DataTable->InsertRow(DTA_ACCESS, (LPVOID)&row);
*/

		CDTData rowData;
		rowData.CreateArray(19);
		rowData.PutItem( "", 0 );					// Point Name
		rowData.PutItem( row.x, 1 );				// Point East (mm double)
		rowData.PutItem( row.y, 2 );				// Point North (mm double)
		rowData.PutItem( row.z, 3 );				// Point Height (mm double)
		rowData.PutItem( 0.0, 4 );					// Point Delta East (mm double)
		rowData.PutItem( 0.0, 5 );					// Point Delta North (mm double)
		rowData.PutItem( 0.0, 6 );					// Point Delta Height (mm double)
		rowData.PutItem( 0.0, 7 );					// Point Distance (mm double)
		rowData.PutItem( row.sz_x, 8 );				// Point East (current units, string)
		rowData.PutItem( row.sz_y, 9 );				// Point North (current units, string)
		rowData.PutItem( row.sz_z, 10 );			// Point Height (current units, string)
		rowData.PutItem( row.sz_dx, 11 );			// Point Delta East (current units, string)
		rowData.PutItem( row.sz_dy, 12 );			// Point Delta North (current units, string)
		rowData.PutItem( row.sz_dz, 13 );			// Point Delta Height (current units, string)
		rowData.PutItem( row.sz_dist, 14 );			// Point Distance (current units, string)
		rowData.PutItem( row.std_dev_x, 15 );		// Std. Dev. East (mm double)
		rowData.PutItem( row.std_dev_y, 16 );		// Std. Dev. North (mm double)
		rowData.PutItem( row.std_dev_z, 17 );		// Std. Dev. Height (mm double)
		rowData.PutItem( row.ray_miss_dist, 18 );	// Ray Miss (mm double)

		DataTable = (CDatatbl *)m_wnd3DPoints.GetDlgItem(IDC_3DPOINTGRID);
		DataTable->GetRowSet().Add(dtAfterLast, rowData);

///////////////////////////////
		Ptg3DCalcPointObj *newPoint = new Ptg3DCalcPointObj;
		newPoint->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
		newPoint->setActive( TRUE );
//		newPoint->setPosition( ptg[0].x, ptg[0].y, ptg[0].z, NULL /*&GETWORLD*/ );

// Still can't explain this swapping business
// But now I can explain it...the ConvertPoint function above was transforming the point to
// the measurement coordinate system.  Thats a no-no with the new grid point stuff and
// reference nodes.

//newPoint->setPosition( ptg[0].x, ptg[0].z, -ptg[0].y, NULL /*&GETWORLD*/ );
		newPoint->setPosition( ptg[0].x, ptg[0].y, ptg[0].z, NULL /*&GETWORLD*/ );

		newPoint->setStdDev( ptg[1].x, ptg[1].y, ptg[1].z );
		newPoint->setRayMiss( ptg[2].x );

		char tempStr[256];
		sprintf( tempStr, "Obj%d", newPoint->getUniqID() );
		newPoint->setName( tempStr );

//		currentElem->addMember( newPoint );
		AfxGetApp()->m_pMainWnd->SendMessage( UM_NEW_3D_ELEMENT, (WPARAM)newPoint, (LPARAM)NULL );

//		CDatatbl *pointSheet = (CDatatbl *)m_wnd3DPoints.GetDlgItem( IDC_POINT_GRID );
//		add3DObjToGrid( pointSheet, newPoint );

///////////////////////////////
//		CRect rect;
//		m_wnd3DPoints.GetClientRect(&rect);
//	//	DataTable->SetWindowPos(&wndTop, 0,0,rect.Width()*0.99,rect.Height(), SWP_NOMOVE|SWP_NOZORDER );
//		::SetWindowPos(DataTable->GetSafeHwnd(),HWND_TOP, 0,0,
//				(rect.right-rect.left)*0.99, rect.bottom-rect.top-18,
//				SWP_SHOWWINDOW);
///////////////////////////////
	}	
}

void CMainFrame::OnUpdateViews(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CObList *list = new CObList;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(list);
	if( list->GetCount() )
		pCmdUI->Enable();	
	else
		pCmdUI->Enable(FALSE);	
	while( list->GetCount() > 0 )
		list->RemoveTail();
	delete list;

}

#if FALSE
void CMainFrame::OnGridLeftClick()
{
//	char temp1[128];
	AfxMessageBox("Left Click 99");
	CDatatbl *DataTable;
//	AfxMessageBox("Left mouse click");
	DataTable = (CDatatbl *)m_wnd3DPoints.GetDlgItem(IDC_3DPOINTGRID);
	ASSERT(DataTable);
	int row = DataTable->GetNotifyRow();
	int col	= DataTable->GetNotifyCol();
	GridRowData data;

//	sprintf(temp1,"Initial row=%d, col=%d", row,col);
//	AfxMessageBox(temp1);
	if( (row >= 0 && row <= 100) )
	{
		DataTable->SetAccessPos(col, row);
		GridRowData temp;		
		DataTable->ReadCell( DTA_ACCESS, &str);
		if( DataTable->ReadRow( DTA_ACCESS, (LPVOID)&data) == DTE_OKAY )
		{
			int status = DataTable->SetAccessPos(0, 0);
			while( status == DTE_OKAY )
			{
				status = DataTable->ReadRow( DTA_ACCESS, &temp);
				if( status == DTE_OKAY )
				{
					temp.dx = (data.x - temp.x);
					temp.dy = (data.y - temp.y);
					temp.dz = (data.z - temp.z);
					temp.dist = sqrt(temp.dx*temp.dx + temp.dy*temp.dy + temp.dz*temp.dz);
					UpdateUnits(&temp);
					DataTable->UpdateRow( DTA_ACCESS|DTA_AUTOINC, &temp);
				}
			}
		}
	}
	mode = 5;	
	SetTimer(DRAGGRID_TIMER, 1500, NULL);
}
#endif

void CMainFrame::OnGridMouseMove(UINT flags, CPoint p )
{//WM_MOUSEMOVE
//	MessageBeep(MB_OK);	
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
#if FALSE
	// TODO: Add your message handler code here
//	CDataTbl *DataTable;
//	AfxMessageBox("Left mouse click");
#if FALSE
	DataTable = (CDataTbl *)m_wnd3DPoints.GetDlgItem(IDD_3DPOINTTABLE);
	ASSERT(DataTable);
	CRect rect;
	CRect prect;
	GetClientRect(&rect);
	m_wnd3DPoints.GetClientRect(&prect);
	::SetWindowPos(DataTable->GetSafeHwnd(),HWND_TOP, 0,0,
			(prect.right-prect.left)*0.99, prect.bottom-prect.top-18,
			SWP_NOMOVE|SWP_SHOWWINDOW);
	CTabCtrl *sheet_tabs = (CTabCtrl *)m_wnd3DPoints.GetDlgItem(IDC_SPREADSHEET_TAGS);
	ASSERT(sheet_tabs);
	::SetWindowPos(sheet_tabs->GetSafeHwnd(), HWND_TOP,0,prect.bottom-prect.top-20,
			0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
#else
	sizeTheSheets();
#endif
	InvalidateRect(NULL,TRUE);
#endif
}

void CMainFrame::OnSettingsOptions() 
{
	// TODO: Add your command handler code here
	COptionsDlg op;
	op.DoModal();	
}

void CMainFrame::UpdateUnits(GridRowData *row)
{
	if( ((CFotobrowApp *)AfxGetApp())->m_Units == INI_METRIC )
	{
		sprintf(row->sz_x, "%15.2f", row->x );
		sprintf(row->sz_y, "%15.2f", row->y );
		sprintf(row->sz_z, "%15.2f", row->z );
		sprintf(row->sz_dx, "%15.2f", row->dx );
		sprintf(row->sz_dy, "%15.2f", row->dy );
		sprintf(row->sz_dz, "%15.2f", row->dz );
		sprintf(row->sz_dist, "%15.2f", row->dist );
	}
	else
	{
		
		ConvertUnits(row->sz_x, 30, row->x/25.4 );
		ConvertUnits(row->sz_y, 30, row->y/25.4 );
		ConvertUnits(row->sz_z, 30, row->z/25.4 );
		ConvertUnits(row->sz_dx, 30, row->dx/25.4 );
		ConvertUnits(row->sz_dy, 30, row->dy/25.4 );
		ConvertUnits(row->sz_dz, 30, row->dz/25.4 );
		ConvertUnits(row->sz_dist, 30, row->dist/25.4 );
	}
} 

void CMainFrame::OnEditZones() 
{
	// TODO: Add your command handler code here
//	CTritonDbDoc *tdb = new CTritonDbDoc;
//	tdb.OnNewDocument();
/************************* Temporary hack during datatable switch *******/
//#if FALSE
/************************* Temporary hack during datatable switch *******/
	CreateOrActivateFrame(((CFotobrowApp *)AfxGetApp())->m_pZoneGridTemplate,
		RUNTIME_CLASS(CZoneEditor));
/************************* Temporary hack during datatable switch *******/
//#endif
/************************* Temporary hack during datatable switch *******/
}

void CMainFrame::OnEditColumns() 
{
	// TODO: Add your command handler code here
/************************* Temporary hack during datatable switch *******/
//#if FALSE
/************************* Temporary hack during datatable switch *******/
	CreateOrActivateFrame(((CFotobrowApp *)AfxGetApp())->m_pColumnGridTemplate,
		RUNTIME_CLASS(CColumnEditor));
/************************* Temporary hack during datatable switch *******/
//#endif
/************************* Temporary hack during datatable switch *******/
}

void CMainFrame::OnEditImages() 
{
	// TODO: Add your command handler code here
	CSelectImages images;
	images.DoModal();	
}

void CMainFrame::OnEditTransformations() 
{
	CCoordXformDlg xform;
	xform.DoModal();	
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	mode = 0;
	CMDIFrameWnd::OnLButtonUp(nFlags, point);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	KillTimer(DRAGGRID_TIMER);
	if( mode == 5 )
	{
//		OpenClipboard();
//		SetClipboardData( CF_TEXT, str);
//		CloseClipboard();		
	}
	
	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnAccelCopygrid() 
{
/* IF...the new grid control does all it should, it will handle the copy function */
#if FALSE
	CDatatbl *DataTable;
	AfxMessageBox("Copying Data");
	DataTable = (CDatatbl *)m_wnd3DPoints.GetDlgItem(IDC_3DPOINTGRID);
	ASSERT(DataTable);

//	GridRowData data;

	DataTable->ReadCell( DTA_CURRENT, &str);

/*
	OpenClipboard();
	SetClipboardData( CF_TEXT, str);
	AfxMessageBox( str );
	CloseClipboard();		
*/
	if( OpenClipboard() )
	{
		if( EmptyClipboard() )
		{
			HGLOBAL clipboardData = GlobalAlloc( GMEM_MOVEABLE|GMEM_DDESHARE, sizeof(str) );
			LPVOID ptrClipboardData = GlobalLock( clipboardData );
			strcpy( (char *)ptrClipboardData, str );
			GlobalUnlock(clipboardData);
			::SetClipboardData( CF_TEXT, clipboardData );
		}
		CloseClipboard();
	}
#endif
}

/////////////////////////////////////////////////////////////////////////
// These values came from the maint system resource.h file 
// The values in maint system resource file must not change or this
// will cease to function
#define ID_FILE_EQUIPDATA               32776
#define ID_FILE_SETCURRENT              32777
/////////////////////////////////////////////////////////////////////////
void CMainFrame::OnEditEquipdata() 
{
	static int flag = 0;
	long lParam;
	
	AfxMessageBox("Not Available in Demo Version");
	return;

	if( m_pInfo.hProcess && flag == 0 )
	{  //PROCESS_INFORMATION PostMessage
		flag = 1;
		lParam = 0;
	}
	else if( flag == 1 )
	{
		lParam = 0;
	}
//	PostThreadMessage(m_pInfo.dwThreadId, WM_COMMAND, ID_FILE_SETCURRENT,0L);	
	::PostMessage(HWND_BROADCAST, WM_COMMAND, ID_FILE_SETCURRENT,lParam);	
//	PostThreadMessage(m_pInfo.dwThreadId, WM_COMMAND, ID_FILE_EQUIPDATA,0L);	
	::PostMessage(HWND_BROADCAST, WM_COMMAND, ID_FILE_EQUIPDATA,lParam);	
}

void CMainFrame::OnEditEquipimages() 
{
	CSelectEqImageDlg eq;
	eq.DoModal();
}

void CMainFrame::RepaintMeasurements()
{
	CDatatbl *dataTable = (CDatatbl *)m_wnd3DPoints.GetDlgItem(IDC_3DPOINTGRID);
	ASSERT(dataTable);

	for( long i = 0; i < dataTable->GetRowSet().GetCount(); i++ )
	{
		CDTData row = dataTable->GetRowSet().GetItem(i).GetValue();
		GridRowData rowData;

		DataTableToGridRowData( row, rowData );

		UpdateUnits(&rowData);

		GridRowDataToDataTable( rowData, row );
//		dataTable->GetRowSet().SetItem(row);
	}

#if FALSE
	CDatatbl *DataTable;
	DataTable = (CDatatbl *)m_wnd3DPoints.GetDlgItem(IDC_3DPOINTGRID);
	ASSERT(DataTable);
//	GridRowData data;

	GridRowData temp;		
	int status = DataTable->SetAccessPos(0, 0);
	while( status == DTE_OKAY )
	{
		status = DataTable->ReadRow( DTA_ACCESS, &temp);
		if( status == DTE_OKAY )
		{
//			temp.dx = (data.x - temp.x);
//			temp.dy = (data.y - temp.y);
//			temp.dz = (data.z - temp.z);
//			temp.dist = sqrt(temp.dx*temp.dx + temp.dy*temp.dy + temp.dz*temp.dz);
			UpdateUnits(&temp);
			DataTable->UpdateRow( DTA_ACCESS|DTA_AUTOINC, &temp);
		}
	}
#endif
}

void CMainFrame::OnFileProjects() 
{
	CProjectSettings dlg;
	dlg.DoModal();

#if FALSE
	HKEY one,two,three;
	DWORD result;
	char value[256];
	DWORD length;
	char *str[] = {"SOFTWARE", "AsBuiltData", "Projects" };
	char *stuff[] = {"DatabasePath", "ImagePath" };

	one = HKEY_LOCAL_MACHINE;
	for( int i=0; i<3; i++ )
	{
		if( RegCreateKeyEx(one,str[i], 0L, NULL,REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &two, &result ) != ERROR_SUCCESS )
		{
			AfxMessageBox("Failed on key enuming");
			RegCloseKey(one);
			return;
		}
		RegCloseKey(one);
		one = two;
	}
	if( RegCreateKeyEx(one,"SCS Hatch", 0L, NULL,REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &two, &result ) != ERROR_SUCCESS )
	{
		AfxMessageBox("Failed");
		return;
	}			
	RegQueryValueEx( two, stuff[0], 0L, &result, (BYTE *)value, &length);
	AfxMessageBox(value);
	RegQueryValueEx( two, stuff[1], 0L, &result, (BYTE *)value, &length);
	AfxMessageBox(value);

	RegCloseKey(two);
	RegCloseKey(one);
#endif
	
}
void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC *pDC = GetWindowDC();
	CView *pView = GetActiveView();
//	dc.TextOut(100,100,"PM3 Lite and the associated data",32);
//	dc.TextOut(100,120,"are proprietary products of",27);
//	dc.TextOut(100,140,"As Built Data",13);
//	dc.TextOut(100,160,"See the About Box for details",30);
#ifdef WOW_IMAGE
	if( pDC )
	{
    //	pDC->BitBlt( m_pt.x, m_pt.y, m_size.cx, m_size.cy, &m_dcMem, 0, 0, SRCCOPY );
	//	pDC->BitBlt( 100, 100, 100, 100, &m_dcMem, 0, 0, SRCCOPY );
		pDC->StretchBlt(100,100,100,100, &m_dcMem, 0, 0, m_size.cx, m_size.cy, SRCCOPY );
	}//WM_NCPAINT
#endif	
	ReleaseDC(pDC);
	// Do not call CMDIFrameWnd::OnPaint() for painting messages
}


void CMainFrame::OnUpdateEditEquipdata(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}

int MyHandler(size_t size )
{
	AfxMessageBox("New Failed");
	return 0;
}
/*
void CMainFrame::OnFileMalloctest() 
{
	void *p;
	char temp[128];
	_set_new_handler(MyHandler);
	for( int i=0; i<1000000; i++ )
	{
//		if( malloc(1024*64) == NULL )
//			AfxMessageBox("NULL returned");
		p = new char[1024*64];
		if( p == NULL )
		{
			sprintf(temp,"NULL returned i=%d, size=%d",i,i*1024*64);
			AfxMessageBox(temp);
		}
		TRACE("count=%d\n",i);
	}	
}
*/

#if FALSE

void CMainFrame::OnEditCopy() 
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

	CDataTbl *DataTbl = (CDataTbl *)m_wnd3DPoints.GetDlgItem(IDD_3DPOINTTABLE);
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	
// 	sprintf(temp,"In mainFrame row=%ld:  col=%d\n", row, col);  
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
	 	sprintf(temp,"The data is %s\n", data.temp);          
//		AfxMessageBox(temp);
	}
}

void CMainFrame::OnEditPaste() 
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

	CDataTbl *DataTbl = (CDataTbl *)m_wnd3DPoints.GetDlgItem(IDD_3DPOINTTABLE);
 	col = DataTbl->GetNotifyCol();
 	row = DataTbl->GetNotifyRow();
 	TRACE("row=%ld:  col=%d\n", row, col);  
	
 	DataTbl->SetAccessPos(col, row);
 	DataTbl->UpdateCell(DTA_ACCESS, (LPVOID)&(data));
//	OnDataChange();
 	sprintf(temp,"The data is %s\n", data.temp);          
//	AfxMessageBox(temp);
}
#endif


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) 
{
	CMDIFrameWnd::OnEraseBkgnd(pDC);
	return TRUE;
}


void CMainFrame::OnRays() 
{
	char *str =  "RAYS";
	TEXTMETRIC t;
	CDC *c;
	c = GetDC();
	c->GetTextMetrics(&t);                  
	ReleaseDC(c);
	m_rays = !m_rays;
	m_wndStatusBar.SetPaneInfo(1,0,m_rays ? SBPS_NORMAL : SBPS_DISABLED, 
				(strlen(str))*t.tmAveCharWidth);
	m_wndStatusBar.SetPaneText(1,(char *)str);
	UpdateAllViews();
}

void CMainFrame::OnOverlay() 
{
	char *str = "OVERLAY";
	TEXTMETRIC t;
	CDC *c;
	c = GetDC();
	c->GetTextMetrics(&t);                  
	ReleaseDC(c);
	m_overlay = !m_overlay;
	m_wndStatusBar.SetPaneInfo(2,0,m_overlay ? SBPS_NORMAL : SBPS_DISABLED, 
				(strlen(str))*t.tmAveCharWidth);
	m_wndStatusBar.SetPaneText(2,(char *)str);
	UpdateAllViews();
}

void CMainFrame::UpdateAllViews()
{
	CDocument *pDoc;
	CObList *pList = new CObList;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		pDoc = (CDocument *)(pList->GetNext(pos));
		if( pDoc->IsKindOf( RUNTIME_CLASS( CFotobrowDoc) ) ||
			pDoc->IsKindOf( RUNTIME_CLASS( CFotogDoc) ) )
		{
			POSITION pos2 = pDoc->GetFirstViewPosition();
			while(pos2)
			{
				CFotobrowView *view = 
					((CFotobrowView *)(pDoc->GetNextView(pos2)));
				ASSERT(view);
				if( pDoc->IsKindOf( RUNTIME_CLASS( CFotobrowDoc) ) )
					view->updateMyRay(m_rays);
				if( pDoc->IsKindOf( RUNTIME_CLASS( CFotogDoc ) ) )
					((CFotoView *)view)->regenView();
			}
			pDoc->UpdateAllViews(NULL);
		}
	}
	while( pList->GetCount() )
		pList->RemoveTail();
	delete pList;
}

void CMainFrame::OnMeasureCylinderbyedge() 
{//CDialog
	setSelectMode(TRUE);
	pCylDlg = new CCreateCylinderByEdgeDlg;
	pCylDlg->Create(IDD_CYLINDERBYEDGE, this);
	pCylDlg->SetWindowPos( &wndTopMost, 0,0,0,0,
		SWP_NOSIZE|SWP_SHOWWINDOW);
}	

void CMainFrame::setSelectMode(int mode)
{
	m_multiSelect = mode;
//	if( !m_multiSelect )
//	{
		CDocument *pDoc;
		CObList *pList = new CObList;
		((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
		POSITION pos = pList->GetHeadPosition();
		while( pos )
		{
			pDoc = (CDocument *)(pList->GetNext(pos));
			if( pDoc->IsKindOf( RUNTIME_CLASS( CFotobrowDoc) ) )
			{
				POSITION pos2 = pDoc->GetFirstViewPosition();
				while(pos2)
				{
					CFotobrowView *view = 
						((CFotobrowView *)(pDoc->GetNextView(pos2)));
					ASSERT(view);
					view->cleanMultiSelect();
				}
//				pDoc->UpdateAllViews(NULL);
			}
		}
		while( pList->GetCount() )
			pList->RemoveTail();
		delete pList;
//	}
}

void CMainFrame::OnUpdateRays(CCmdUI* pCmdUI) 
{
	if( m_rays )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateOverlay(CCmdUI* pCmdUI) 
{
	if( m_overlay )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point) 
{
//	MessageBeep(MB_OK);
	
	CMDIFrameWnd::OnMouseMove(nFlags, point);
}

BOOL CMainFrame::CreateClient(LPCREATESTRUCT lpCreateStruct,CMenu *pWindowMenu)
{
	if( CMDIFrameWnd::CreateClient(lpCreateStruct, pWindowMenu))
	{
#undef SubclassWindow
		m_MyMdiClientWnd.SubclassWindow(m_hWndMDIClient);
		return TRUE;
	}
	else
		return FALSE;
}

void CMainFrame::OnClose() 
{
//	AfxMessageBox("On WM_CLOSE inside MainFrame");
//	((CFotobrowApp *)AfxGetApp())->ExitInstance();
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnKillCreateCylinder()
{
	if( pCylDlg	)
	{
		delete pCylDlg;
		pCylDlg = NULL;
	}
}

void CMainFrame::OnMeasureCirclebyedge() 
{
	setSelectMode(TRUE);
	pCirDlg = new CCircleByEdge;
	pCirDlg->Create(IDD_CIRCLE_BY_EDGE, this);
	pCirDlg->SetWindowPos( &wndTopMost, 0,0,0,0,
		SWP_NOSIZE|SWP_SHOWWINDOW);
}

void CMainFrame::OnKillCreateCircle()
{
	if( pCirDlg	)
	{
		delete pCirDlg;
		pCirDlg = NULL;
	}
}

void CMainFrame::OnMeasureCircleby3dpoint() 
{
	setSelectMode(FALSE);
	pCir3DDlg = new CCircleBy3DPoint;
	pCir3DDlg->Create(IDD_CIRCLE_BY_3DPOINT, this);
	pCir3DDlg->SetWindowPos( &wndTopMost, 0,0,0,0,
		SWP_NOSIZE|SWP_SHOWWINDOW);
}

void CMainFrame::sizeTheSheets()
{
	int sheets[] = {IDC_3DPOINTGRID, 
					IDD_3DCYLINDERTABLE,
					IDD_3DCIRCLETABLE,
					IDD_3DTARGETTABLE,
					IDD_3DCAMERATABLE};
	CDatatbl *DataTable;
	CRect rect;
	CRect prect;
AfxMessageBox( "CMainFrame::sizeTheSheets()" );
#ifdef NEW_MULTI_SHEETS
	for( int i=0; i<sizeof(sheets)/sizeof(sheets[0]); i++ )
	{
		DataTable = (CDataTbl *)m_wnd3DPoints.GetDlgItem(sheets[i]);
		ASSERT(DataTable);
		GetClientRect(&rect);
		m_wnd3DPoints.GetClientRect(&prect);
		::SetWindowPos(DataTable->GetSafeHwnd(),HWND_TOP, 0,0,
				(prect.right-prect.left)*0.99, prect.bottom-prect.top-18,
				SWP_SHOWWINDOW);
	}
	CTabCtrl *sheet_tabs = (CTabCtrl *)m_wnd3DPoints.GetDlgItem(IDC_SPREADSHEET_TAGS);
	ASSERT(sheet_tabs);
	::SetWindowPos(sheet_tabs->GetSafeHwnd(), HWND_TOP,0,prect.bottom-prect.top-19,
			0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
#else
		DataTable = (CDatatbl *)m_wnd3DPoints.GetDlgItem(IDC_3DPOINTGRID);
		ASSERT(DataTable);
		GetClientRect(&rect);
		m_wnd3DPoints.GetClientRect(&prect);
		::SetWindowPos(DataTable->GetSafeHwnd(),HWND_TOP, 0,0,
				(prect.right-prect.left)*0.99, prect.bottom-prect.top,
				SWP_SHOWWINDOW);
#endif	
}

void CMainFrame::createTheSheets() 
{
#if FALSE
	int sheets[] = {IDC_3DPOINTGRID,
					IDD_3DCYLINDERTABLE,
					IDD_3DCIRCLETABLE,
					IDD_3DTARGETTABLE,
					IDD_3DCAMERATABLE};

#ifdef NEW_MULTI_SHEETS
	for( int i=0; i<sizeof(sheets)/sizeof(sheets[0]); i++ )
	{
		CDataTbl *DataTable;
	//	AfxMessageBox("Left mouse click");
		DataTable = (CDataTbl *)m_wnd3DPoints.GetDlgItem(sheets[i]);
		ASSERT(DataTable);
	/*	if( DataTable->SetWndStyle(WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|WS_HSCROLL|
				DTS_OWNERDEF|DTS_EDIT|DTS_RESIZE|
				DTS_ROWNUMBERS|DTS_VGRID|DTS_HGRID) != DTE_OKAY )
			AfxMessageBox("SetWndStyle Failed");*/
		DWORD style = DataTable->SendMessage(DTM_GETWNDSTYLE,0,0L);
		if( DataTable->SetWndStyle( style|
				DTS_OWNERDEF|DTS_EDIT|DTS_RESIZE|
				DTS_ROWNUMBERS|DTS_VGRID|DTS_HGRID) != DTE_OKAY )
			AfxMessageBox("SetWndStyle Failed");
		DTCOLORSTRUCT Color;
		Color.BkgrdColor = RGB(255,255,255);
		Color.TextColor = RGB(0,0,0);
		Color.SelBkgrdColor = RGB(0,0,128);
		Color.SelTextColor = RGB(0,255,255);
		DataTable->SendMessage(DTM_SETDTCOLOR,0,(LPARAM)&Color);
		DataTable->SendMessage(DTM_SETGRIDCOLOR,0,RGB(255,0,0));
	}
#else
//	CDatatbl *DataTable;
	//	AfxMessageBox("Left mouse click");
//	DataTable = (CDatatbl *)m_wnd3DPoints.GetDlgItem(sheets[0]);
//	ASSERT(DataTable);

//	DataTable->SetBackColor( RGB(255,255,255) );
//	DataTable->SetForeColor( RGB(0,0,0) );
//	DataTable->SetSelBackColor( RGB(0,0,128) );
//	DataTable->SetSelForeColor( RGB(0,255,255) );
//	DataTable->SetGridLineColor( RGB(255,0,0) );
#if FALSE
	DWORD style = DataTable->SendMessage(DTM_GETWNDSTYLE,0,0L);
	if( DataTable->SetWndStyle( style|
			DTS_OWNERDEF|DTS_EDIT|DTS_RESIZE|
			DTS_ROWNUMBERS|DTS_VGRID|DTS_HGRID) != DTE_OKAY )
		AfxMessageBox("SetWndStyle Failed");
	DTCOLORSTRUCT Color;
	Color.BkgrdColor = RGB(255,255,255);
	Color.TextColor = RGB(0,0,0);
	Color.SelBkgrdColor = RGB(0,0,128);
	Color.SelTextColor = RGB(0,255,255);
	DataTable->SendMessage(DTM_SETDTCOLOR,0,(LPARAM)&Color);
	DataTable->SendMessage(DTM_SETGRIDCOLOR,0,RGB(255,0,0));
#endif

#endif

#endif
}

void CMainFrame::OnSheetTab()
{
	MessageBox("In on sheet tab");
}

BOOL CMainFrame::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	int idTabCtl;
	MessageBox("Got an Child Notify");
	switch (message)
	{
		case TCN_SELCHANGE:
			MessageBox("Got a change notification");
			idTabCtl = (int) LOWORD(wParam); 			
			break;
	}
	
	return CMDIFrameWnd::OnChildNotify(message, wParam, lParam, pLResult);
}

#if FALSE
BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
//	MessageBox("Got a Notify");
/*
typedef struct tagNMHDR { 
    HWND hwndFrom; 
    UINT idFrom; 
    UINT code; 
} NMHDR; CTabCtrl  TabCtrl_GetCurSel
*/	
	int sheets[] = {IDC_3DPOINTGRID, 
					IDD_3DCYLINDERTABLE,
					IDD_3DCIRCLETABLE,
					IDD_3DTARGETTABLE,
					IDD_3DCAMERATABLE};


#ifndef NEW_MULTI_SHEETS
	CMDIFrameWnd::OnNotify(wParam, lParam, pResult);
#endif
	
	NMHDR *hdr;
	int index;
//	HWND hwnd;
	CWnd *cwnd;
//	CDataTbl *DataTable
	hdr = (NMHDR *)lParam;
	switch( hdr->code )
	{
		case TCN_SELCHANGE:
			index = ::SendMessage(hdr->hwndFrom,TCM_GETCURSEL ,0,0);
			if( index >= 0 )
			{
				//char temp[128];
				//sprintf(temp, "The index is %d", index);
				//MessageBox(temp);
				cwnd = m_wnd3DPoints.GetDlgItem(sheets[index]);
				ASSERT(cwnd);
				::BringWindowToTop( cwnd->GetSafeHwnd() );
				::InvalidateRect(cwnd->GetSafeHwnd(),NULL,TRUE);
			}
			else
				MessageBox("I got nothing");
			break;
	}
	return CMDIFrameWnd::OnNotify(wParam, lParam, pResult);
}
#endif

void CMainFrame::OnViewElementlist() 
{
	CDocument *pDoc;
	CObList *pList = new CObList;
	int flag = FALSE;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
	TRACE("Got the DOC List\n");
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{		
//		AfxMessageBox("A Doucment");
		TRACE("Got the next DOC\n");
		pDoc = (CDocument *)(pList->GetNext(pos));
		POSITION pos2 = pDoc->GetFirstViewPosition();
		CView *pView;
		while(  pos2!=NULL )
		{
			pView=pDoc->GetNextView(pos2);
			
//			AfxMessageBox("Another View");
			if( pView->IsKindOf(RUNTIME_CLASS(CElementListView)) )
			{
//				AfxMessageBox("Setting Active Window");
				(pView->GetParent())->BringWindowToTop();
				goto End;
			}
		}
	}
//	AfxMessageBox("Creating a grapohic view");
	CreateOrActivateFrame(((CFotobrowApp *)AfxGetApp())->m_pElementListTemplate,
		RUNTIME_CLASS(CElementListView));
			
End:
//	AfxMessageBox("End");
	pList->RemoveAll();
	delete pList;
}

LONG CMainFrame::OnCurrentElementChange(WPARAM wParam, LPARAM lParam)
{
	if( wParam == NULL )
		AfxMessageBox("Current Ptg3DNode pointer was NULL in message - Bad Message!");
	else
	{
		currentElem = (Ptg3DNode *)wParam;
		//AfxMessageBox("Current Elem Change");
	}

	HTREEITEM hTreeItem = (HTREEITEM)m_3DtoTree[(void *)lParam];
//	if( !hTreeItem )
//	{	
//		AfxMessageBox("Not found");
//		return FALSE;
//	}

	CDocument *pDoc;
	CObList *pList = new CObList;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		pDoc = (CDocument *)(pList->GetNext(pos));
		POSITION pos2 = pDoc->GetFirstViewPosition();
		while(pos2)
		{
			CView *view = pDoc->GetNextView(pos2);
			ASSERT(view);
			view->SendMessage(UM_CURRENT_ELEMENT_CHANGE, lParam, (LPARAM)hTreeItem);
		}
	}

	m_wnd3DPoints.SendMessage( UM_CURRENT_ELEMENT_CHANGE, lParam, (LPARAM)hTreeItem );

	while( pList->GetCount() )
		pList->RemoveTail();
	delete pList;
	return TRUE;
}

LONG CMainFrame::OnUnitsChange(WPARAM wParam, LPARAM lParam)
{
	CDocument *pDoc;
	CObList *pList = new CObList;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		pDoc = (CDocument *)(pList->GetNext(pos));
		POSITION pos2 = pDoc->GetFirstViewPosition();
		while(pos2)
		{
			CView *view = pDoc->GetNextView(pos2);
			ASSERT(view);
			view->PostMessage( UM_UNITS_CHANGE, (WPARAM)wParam, (LPARAM)lParam);
		}
	}

	m_wnd3DPoints.PostMessage( UM_UNITS_CHANGE, (WPARAM)wParam, (LPARAM)lParam );

	while( pList->GetCount() )
		pList->RemoveTail();
	delete pList;

	return TRUE;
}

LONG CMainFrame::OnNew3DElement(WPARAM wParam, LPARAM lParam)
{
	// wParam is new node pointer
	// lParam is UNUSED
	Ptg3DNode *newElement = (Ptg3DNode *)wParam;
	
	// Add item to current node first....
	Ptg3DNode *parent = GETCURRENTELEM;

	if( !checkModelLocation( parent ) )
	{
		AfxMessageBox( "Modeling not supported under current node.  Item will be added under \"Model\" node." );
		parent = GETMODEL;
	}

	if( parent != NULL )
	{
		if( !(IS_TYPE_OF(parent, Ptg3DNode) || IS_TYPE_OF(parent, PtgWorld)) )
			parent = (Ptg3DNode *)(parent->getOwner());
		if( parent != NULL )
		{
			parent->addMember(newElement);
		}
	}

	// Tell element gridbar
	if( m_wnd3DPoints )
		m_wnd3DPoints.SendMessage( UM_NEW_3D_ELEMENT, wParam, lParam );

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
				HTREEITEM hTreeItem = (HTREEITEM)m_3DtoTree[(void *)(newElement->getOwner())];
				if( hTreeItem )
				{
					((CElementListView *)view)->AddToDrawList( newElement, hTreeItem, TRUE, FALSE, FALSE );
				}
				view->Invalidate(TRUE);
			}
		}
	}

	while( pList->GetCount() )
	pList->RemoveTail();
	delete pList;

	// Redraw all the necessary views
	((CMainFrame *)(((CFotobrowApp *)AfxGetApp()))->m_pMainWnd)->UpdateAllViews();

	return TRUE;
}

//ON_COMMAND

LONG CMainFrame::OnNewTreeNode(WPARAM wParam, LPARAM lParam)
{
	m_3DtoTree.SetAt((void *)wParam, (void *)lParam);
	return TRUE;
}


void CMainFrame::OnEditBundlecalculations() 
{
	CBundleCalcDlg dlg;
	dlg.DoModal();
}

BEGIN_EVENTSINK_MAP(CMainFrame, CMDIFrameWnd)
    //{{AFX_EVENTSINK_MAP(C3DPointsDlg)
	ON_EVENT(CMainFrame, IDC_3DPOINTGRID, 50 /* LfClick */, OnLfClick3dpointgrid, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMainFrame::OnLfClick3dpointgrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status) 
{
	CDatatbl *dataTable = (CDatatbl *)m_wnd3DPoints.GetDlgItem(IDC_3DPOINTGRID);
	ASSERT(dataTable);
	AfxMessageBox("Inside a left click");
	CDTData selectedRow = dataTable->GetRowSet().GetItem(Row).GetValue();
	GridRowData selectedRowData;

	DataTableToGridRowData( selectedRow, selectedRowData );

	for( long i = 0; i < dataTable->GetRowSet().GetCount(); i++ )
	{
		CDTData row = dataTable->GetRowSet().GetItem(i).GetValue();
		GridRowData rowData;

		DataTableToGridRowData( row, rowData );

		rowData.dx = selectedRowData.x - rowData.x;
		rowData.dy = selectedRowData.y - rowData.y;
		rowData.dz = selectedRowData.z - rowData.z;
		rowData.dist = sqrt(rowData.dx*rowData.dx + rowData.dy*rowData.dy + rowData.dz*rowData.dz);
		UpdateUnits(&rowData);

		GridRowDataToDataTable( rowData, row );
//		dataTable->GetRowSet().SetItem(row);
	}

	mode = 5;	
	SetTimer(DRAGGRID_TIMER, 1500, NULL);
}

void CMainFrame::OnEditCopy() 
{
	CWnd *currWnd = GetFocus();
//	CDatatbl *dataTbl = (CDatatbl *)GetDlgItem(IDC_ZONEGRID);

	// Decided to be a little safe here.  If current control is not my data table,
	// I'm not gonna pass on the message.  May be OK to just pass on all copy messages
	// regardless, dunno
//	if( currWnd == dataTbl )
		currWnd->SendMessage( WM_COPY, 0, 0 );
}

void CMainFrame::OnEditPaste() 
{
	CWnd *currWnd = GetFocus();
//	CDatatbl *dataTbl = (CDatatbl *)GetDlgItem(IDC_ZONEGRID);

	// Decided to be a little safe here.  If current control is not my data table,
	// I'm not gonna pass on the message.  May be OK to just pass on all paste messages
	// regardless, dunno
//	if( currWnd == dataTbl )
		currWnd->SendMessage( WM_PASTE, 0, 0 );
}

void CMainFrame::OnEditCut() 
{
	CWnd *currWnd = GetFocus();
//	CDatatbl *dataTbl = (CDatatbl *)GetDlgItem(IDC_ZONEGRID);

	// Decided to be a little safe here.  If current control is not my data table,
	// I'm not gonna pass on the message.  May be OK to just pass on all cut messages
	// regardless, dunno
//	if( currWnd == dataTbl )
		currWnd->SendMessage( WM_CUT, 0, 0 );
}

void CMainFrame::OnFileImportSurveyData() 
{
	CFileDialog theDialog( TRUE );
	if( theDialog.DoModal() == IDOK )
	{
		importSurveyData( (char *)(LPCTSTR)(theDialog.GetPathName()), GETCURRENTELEM );
		AfxMessageBox( theDialog.GetPathName() );
	}
}

void CMainFrame::OnFileImportReviewModel() 
{
	//
	// Here's the theory I had when I did this...
	//   A review model must be imported under the nodes that handle the axis swap and
	//   PDMS to Photog coordinate system transformation.  However, I would like to be
	//   able to segregate review files in some way.  Therefore, I think I will require
	//   the importing node to be under that leg, but not necessarily the axis swap node.
	//

//	Ptg3DNode *pdmsToPhotog = GETWORLD.findByName( "PtgCoorPdmsToPhotog", TRUE );
	Ptg3DNode *pdmsToPhotog = GETWORLD.findByName( "Review Model", TRUE );
	if( pdmsToPhotog == NULL )
	{
		AfxMessageBox( "Unable to locate node for importing models." );
		return;
	}

	char *currName = (char *)(GETCURRENTELEM->getName());
	Ptg3DNode *huntingCurrNode = pdmsToPhotog->findByName( GETCURRENTELEM->getName(), TRUE );
	if( pdmsToPhotog != GETCURRENTELEM && huntingCurrNode != GETCURRENTELEM )
	{
		AfxMessageBox( "Current node is not child of a node that is compatible with importing models." );
		return;
	}

	CFileDialog theDialog( TRUE );
	theDialog.m_ofn.Flags |= OFN_NOCHANGEDIR;
	if( theDialog.DoModal() == IDOK )
	{
//		Ptg3DNode *oldRefNode = measurementRefNode;

//		((CFotobrowApp *)AfxGetApp())->SetReferenceNode(pdmsToPhotog);
		Ptg3DNode *importNode = new Ptg3DNode;
		importNode->setName( (const char *)(theDialog.GetFileName()) );
		importNode->setActive(TRUE);
		importNode->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
		GETCURRENTELEM->addMember(importNode);
		importReviewModel((char *)(const char *)(theDialog.GetPathName()), importNode );

		// Update the element list view to reflect changes
		CDocument *pDoc;
		CObList *pList = new CObList;
		int flag = FALSE;
		((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
		POSITION pos = pList->GetHeadPosition();
		while( pos )
		{
			pDoc = (CDocument *)(pList->GetNext(pos));
			POSITION pos2 = pDoc->GetFirstViewPosition();
			CView *pView;
			while(  pos2!=NULL )
			{
				pView=pDoc->GetNextView(pos2);

				if( pView->IsKindOf(RUNTIME_CLASS(CElementListView)) )
				{
					HTREEITEM hTreeItem = (HTREEITEM)m_3DtoTree[(void *)(importNode->getOwner())];
					if( hTreeItem )
					{
						((CElementListView *)pView)->AddToDrawList( importNode, hTreeItem, TRUE, FALSE, TRUE );
					}

					pView->Invalidate(TRUE);
					goto End;
				}
			}
		}

End:
		pList->RemoveAll();
		delete pList;
		// End update of element list view
	}
}

void CMainFrame::OnMeasureQuickCylinder() 
{
	setSelectMode(FALSE);
	pQckCylDlg = new CCreateQuickCylinderDlg;
	pQckCylDlg->Create(IDD_QUICKCYLINDER, this);
	pQckCylDlg->SetWindowPos( &wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
}

void CMainFrame::OnMeasureCylinderbyEndpoints() 
{
	setSelectMode(FALSE);
	pEndPointCyliDlg = new CCreateCylinderByEndpointsDlg;
	pEndPointCyliDlg->Create( IDD_ENDPOINTCYLINDER, this );
	pEndPointCyliDlg->SetWindowPos( &wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
}

void CMainFrame::OnFileSaveWork() 
{
	printf( "Saving work\n" );
	t->MakeCurrent();

	TransAction theTransaction;
	theTransaction.Begin("SaveWork");

	ClassesObj modelClassObj;

	strcpy( modelClassObj.name, START_NODE );
	modelClassObj.Find( KeyObj(CLASSES_NAME, &(modelClassObj.name)) );
	if( modelClassObj.Okay() )
	{
		modelClassObj.Prune(True, True);
	}

	printf( "Creating 'Model' node in database.\n" );
	modelClassObj.NewObj();
	if( !modelClassObj.Okay() )
	{
		AfxMessageBox( "Error creating 'Model' node." );
		theTransaction.Abort();
		return;
	}

	Ptg3DNode *modelNode = GETMODEL;;
	if( !modelNode )
	{
		AfxMessageBox( "Unable to locate 'Model' node in hierarchy." );
		theTransaction.Abort();
		return;
	}

	POSITION pos;
	for( pos = modelNode->getMemberList().GetHeadPosition(); pos != NULL; modelNode->getMemberList().GetNext(pos) )
	{
		Ptg3DNode *childNode = (Ptg3DNode *)modelNode->getMemberList().GetAt(pos);

		if( !saveNodeToDB( childNode, &modelClassObj ) )
		{
			char szTempStr[256];
			sprintf( szTempStr, "Error saving node '%s' to database.", childNode->getName() );
			AfxMessageBox( szTempStr );
			theTransaction.Abort();
			return;
		}
	}

	theTransaction.End();
}

void CMainFrame::OnFileGetWork() 
{
	printf( "Getting work\n" );
	t->MakeCurrent();

	ClassesObj modelClassObj;
	strcpy( modelClassObj.name, START_NODE );
	modelClassObj.Find( KeyObj(CLASSES_NAME, &(modelClassObj.name)) );
	if( !modelClassObj.Okay() )
	{
		AfxMessageBox( "Error locating 'Model' node" );
		return;
	}

	Ptg3DNode *modelNode = GETMODEL;
	if( !modelNode )
	{
		AfxMessageBox( "Unable to locate 'Model' node in hierarchy." );
		return;
	}

	ClassesObj nextClass;
	for( nextClass.GotoMember(modelClassObj, CLASSES_CLASSES, FIRST);
		 nextClass.Okay();
		 nextClass.GotoMember(modelClassObj, CLASSES_CLASSES, NEXT) )
	{
		printf( "name = '%s'\n", nextClass.name );
		saveDBToNode(&nextClass, modelNode );
	}
}

int saveDBToNode( ClassesObj *classObj, Ptg3DNode *nodeObj )
{
	Ptg3DNode *newElement = NULL;
	// Determine if this Class object is a Node or a primitive container
	if( classObj->IsOwner(ELEMENT_CLASSES) )
	{
		AllElements ae;
		ae.GotoMember(*classObj,ELEMENT_CLASSES,FIRST);
		if( ae.Okay() )
		{
			switch(ae.RecType())
			{
				case BOX:		// 10004
				{
					Ptg3DBoxObj *newBox = new Ptg3DBoxObj;
					ae.Select(BOX);
					struct Box *sb = (struct Box *)(ae.RecData());
					newBox->setXLen(sb->xlen);
					newBox->setYLen(sb->ylen);
					newBox->setZLen(sb->zlen);

					newElement = newBox;
					break;
				}
				case CIRTORUS:	// 10005
				{
					Ptg3DCtorObj *newCtor = new Ptg3DCtorObj;
					ae.Select(CIRTORUS);
					struct CirTorus *sct = (struct CirTorus *)(ae.RecData());
					newCtor->setOuterRadius(sct->outerRadius);
					newCtor->setInnerRadius(sct->innerRadius);
					newCtor->setAngle(sct->angle);

					newElement = newCtor;
					break;
				}
				case CONE:		// 10006
				{
					Ptg3DConeObj *newCone = new Ptg3DConeObj;
					ae.Select(CONE);
					struct Cone *sc = (struct Cone *)(ae.RecData());
					newCone->setTopRadius(sc->topRadius);
					newCone->setBottomRadius(sc->bottomRadius);
					newCone->setHeight(sc->height);
					newCone->setXOffset(sc->xOffset);
					newCone->setYOffset(sc->yOffset);

					newElement = newCone;
					break;
				}
				case CYLINDER:	// 10007
				{
					Ptg3DCylinderObj *newCyli = new Ptg3DCylinderObj;
					ae.Select(CYLINDER);
					struct Cylinder *sc = (struct Cylinder *)(ae.RecData());
					newCyli->setRadius(sc->radius);
					newCyli->setHeight(sc->height);

					newElement = newCyli;
					break;
				}

				case RECTTORUS:	// 10008
				{
					Ptg3DRtorObj *newRtor = new Ptg3DRtorObj;
					ae.Select(RECTTORUS);
					struct RectTorus *srt = (struct RectTorus *)(ae.RecData());
					newRtor->setOuterRadius(srt->outerRadius);
					newRtor->setInnerRadius(srt->innerRadius);
					newRtor->setHeight(srt->height);
					newRtor->setAngle(srt->angle);

					newElement = newRtor;
					break;
				}
				case SPHERE:	// 10009
				{
/****************************************************************************
					Ptg3DSphereObj *newSphere = new Ptg3DSphereObj;
					ae.Select(SPHERE);
					struct Sphere *ss = (struct Sphere *)(ae.RecData());
					newSphere->setDiameter(ss.diameter);
					newSphere->setHeight(ss.height);
					newSphere->setRadius(ss.radius);

					newElement = newSphere;
****************************************************************************/
					break;
				}
				case PYRAMID:	// 10010
				{
					Ptg3DPyramidObj *newPyra = new Ptg3DPyramidObj;
					ae.Select(PYRAMID);
					struct Pyramid *sp = (struct Pyramid *)(ae.RecData());
					newPyra->setHeight(sp->height);
					newPyra->setBaseDims(sp->baseWidth, sp->BaseDepth);
					newPyra->setTopDims(sp->TopWidth, sp->TopDepth);
					newPyra->setOffsets(sp->WidthOffset, sp->DepthOffset);

					newElement = newPyra;
					break;
				}
				case COMPLEX:	// 10011
				{
					break;
				}
				case FACES:		// 10012
				{
					break;
				}
				case VERTICES:	// 10013
				{
					break;
				}
				case POINT_REC:	// 10014
				{
					Ptg3DPointObj *newPoint = new Ptg3DPointObj;
					ae.Select(POINT_REC);
					struct Point *sp = (struct Point *)(ae.RecData());
					newPoint->setPosition(sp->xpos, sp->ypos, sp->zpos);

					newElement = newPoint;
					break;
				}
				case LINE_REC:	// 10015
				{
					Ptg3DLineObj *newLine = new Ptg3DLineObj;
					ae.Select(LINE);
					struct Line *sl = (struct Line *)(ae.RecData());
					newLine->setOrigin(sl->x1, sl->y1, sl->z1);
					newLine->setEndPoint(sl->x2, sl->y2, sl->z2);

					newElement = newLine;
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
	else
		newElement = new Ptg3DNode;

	if( newElement )
	{
		if( classObj->name && classObj->name[0] )
			newElement->setName(classObj->name);

		TransformObj to;
		to.FromMember(classObj,TRANSFORMS);
		if( to.Okay() )
		{
			VECTOR vTMatrix = vector(1,12);

			vTMatrix[1] = to.a[0];		vTMatrix[2] = to.a[1];		vTMatrix[3] = to.a[2];
			vTMatrix[4] = to.a[4];		vTMatrix[5] = to.a[5];		vTMatrix[6] = to.a[6];
			vTMatrix[7] = to.a[8];		vTMatrix[8] = to.a[9];		vTMatrix[9] = to.a[10];
			vTMatrix[10] = to.a[3];		vTMatrix[11] = to.a[7];		vTMatrix[12] = to.a[11];

			newElement->setMatrix(vTMatrix);
			free_vector(vTMatrix,1,12);
		}
		newElement->setActive(TRUE);
		nodeObj->addMember(newElement);

		ClassesObj nextClass;

		for( nextClass.GotoMember(*classObj,CLASSES_CLASSES,FIRST);
			 nextClass.Okay();
			 nextClass.GotoMember(*classObj,CLASSES_CLASSES,NEXT) )
		{
			saveDBToNode(&nextClass, newElement );
		}
	}

	return TRUE;
}

int saveNodeToDB( Ptg3DNode *nodeObj, ClassesObj *classObj )
{
	static int indentLevel = 0;
	indentLevel++;
printf( "%*.*s%s\n", indentLevel*4, indentLevel*4, " ", nodeObj->getName() );

	// All Nodes (including element nodes) need a Classes object created and their name set in it...
	ClassesObj newNodeClassObj;
	memset( newNodeClassObj.RecData(), 0, sizeof(struct Classes) );
	if( nodeObj->getName() )
		strcpy( newNodeClassObj.name, nodeObj->getName() );
	newNodeClassObj.NewObj();
	newNodeClassObj.ConnectTo(*classObj,CLASSES_CLASSES);

	// All Nodes also have a transform...
	TransformObj newNodeTransform;
	memset( newNodeTransform.RecData(), 0, sizeof(struct Transform) );
	VECTOR vTMatrix = nodeObj->getMatrix();
	newNodeTransform.a[0] = vTMatrix[1];	newNodeTransform.a[1] = vTMatrix[2];	newNodeTransform.a[2] = vTMatrix[3];	newNodeTransform.a[3] = vTMatrix[10];
	newNodeTransform.a[4] = vTMatrix[4];	newNodeTransform.a[5] = vTMatrix[5];	newNodeTransform.a[6] = vTMatrix[6];	newNodeTransform.a[7] = vTMatrix[11];
	newNodeTransform.a[8] = vTMatrix[7];	newNodeTransform.a[9] = vTMatrix[8];	newNodeTransform.a[10] = vTMatrix[9];	newNodeTransform.a[11] = vTMatrix[12];
	newNodeTransform.a[12] = 0.0;			newNodeTransform.a[13] = 0.0;			newNodeTransform.a[14] = 0.0;			newNodeTransform.a[15] = 1.0;

	free_vector( vTMatrix, 1, 12 );
	newNodeTransform.NewObj();
	newNodeClassObj.ConnectTo(newNodeTransform,TRANSFORMS);

	// Handle each of our specific node types...
	if( IS_TYPE_OF(nodeObj, Ptg3DNode) )
	{
	}
	else if( IS_TYPE_OF(nodeObj, Ptg3DBoxObj) )
	{
		Ptg3DBoxObj *boxNode = (Ptg3DBoxObj *)nodeObj;

		BoxObj newBoxObj;
		memset( newBoxObj.RecData(), 0, sizeof(struct Box) );
		newBoxObj.xlen = boxNode->getXLen();
		newBoxObj.ylen = boxNode->getYLen();
		newBoxObj.zlen = boxNode->getZLen();
		newBoxObj.NewObj();
		newBoxObj.ConnectTo(newNodeClassObj,ELEMENT_CLASSES);
	}
	else if( IS_TYPE_OF(nodeObj, Ptg3DCalcPointObj) || 	IS_TYPE_OF(nodeObj, Ptg3DPointObj) )
	{
		Ptg3DPointObj *pointNode = (Ptg3DPointObj *)nodeObj;

		PointObj newPointObj;
		memset( newPointObj.RecData(), 0, sizeof(struct Point) );
		pointNode->getPosition( NULL, newPointObj.xpos, newPointObj.ypos, newPointObj.zpos );
		newPointObj.NewObj();
		newPointObj.ConnectTo(newNodeClassObj, ELEMENT_CLASSES);
	}
	else if( IS_TYPE_OF(nodeObj, Ptg3DConeObj) )
	{
	}
	else if( IS_TYPE_OF(nodeObj, Ptg3DCtorObj) )
	{
	}
	else if( IS_TYPE_OF(nodeObj, Ptg3DCylinderObj) )
	{
		Ptg3DCylinderObj *cylNode = (Ptg3DCylinderObj *)nodeObj;

		CylinderObj newCylinderObj;
		memset( newCylinderObj.RecData(), 0, sizeof(struct Cylinder) );
		newCylinderObj.height = cylNode->getHeight();
		newCylinderObj.radius = cylNode->getRadius();
		newCylinderObj.NewObj();
		newCylinderObj.ConnectTo(newNodeClassObj,ELEMENT_CLASSES);
	}
	else if( IS_TYPE_OF(nodeObj, Ptg3DFacetObj) )
	{
	}
	else if( IS_TYPE_OF(nodeObj, Ptg3DLineObj) )
	{
	}
	else if( IS_TYPE_OF(nodeObj, Ptg3DPyramidObj) )
	{
	}
	else if( IS_TYPE_OF(nodeObj, Ptg3DRtorObj) )
	{
	}

	// And finally, all Nodes have the ability to contain other nodes
	POSITION pos;
	for( pos = nodeObj->getMemberList().GetHeadPosition(); pos != NULL; nodeObj->getMemberList().GetNext(pos) )
	{
		Ptg3DNode *childNode = (Ptg3DNode *)nodeObj->getMemberList().GetAt(pos);
		saveNodeToDB( childNode, &newNodeClassObj );
	}

	indentLevel--;

	return TRUE;
}