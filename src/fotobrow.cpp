// fotobrow.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
//#define NO_DT_COVER
#define _FOTOBROW_CPP_
#include "fotobrow.h"

//#include "common.h"
#include "openproj.h"
#include "splash.h"

#include "mainfrm.h"
#include "fotobdoc.h"
#include "fotobvw.h"
#include "zoneeditor.h"
#include "columnfo.h"
#include "tritondb.h"
#include "fotoview.h"
#include "mfc_frames.h"
#include "regfuncs.h"
#include "elementlistview.h"
#include "columneditor.h"

//#define VISTACALLS
#include "../triton\tritont.hpp"
#include "transact.hpp"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define COPYPROTFILE "datadump.bin"

//nt outputLevel=10;			/* always have most basic output */
//int debugOutputLevel=10;	        /* always start with debug off */
HCURSOR hMainCursor;

void qinitElemList();  

//HCURSOR hViewCursor;
/////////////////////////////////////////////////////////////////////////////
// CFotobrowApp

BEGIN_MESSAGE_MAP(CFotobrowApp, CWinApp)
	//{{AFX_MSG_MAP(CFotobrowApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFotobrowApp construction

CFotobrowApp::CFotobrowApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pMainWnd = NULL;
	databaseOpenFlag = FALSE;
}

CFotobrowApp::~CFotobrowApp()
{
	// TODO: add destruction code here,
//	WriteProfile();
//	AfxMessageBox("I am here - inside the app destructor");		
	if( m_pMainWnd )
	{
		delete m_pMainWnd;
		m_pMainWnd = NULL;
	}
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CFotobrowApp object

CFotobrowApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.
static const CLSID BASED_CODE clsid =
{ 0x55445a0, 0x43df, 0x11ce, { 0x88, 0xa8, 0x0, 0x20, 0xaf, 0x19, 0xdf, 0x3a } };

/////////////////////////////////////////////////////////////////////////////
// CFotobrowApp initialization

BOOL CFotobrowApp::InitInstance()
{
	AfxEnableControlContainer();
	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}
	int flag=0;

#ifdef WANT_COPY_PROTECTION
	struct Tim { time_t t1,t2,t3; } tim;
	time_t t4;
	char temp8[128];
	FILE *out = fopen(COPYPROTFILE,"r+");
	if( out == NULL )
		return FALSE;	
	if( fread(&tim,12,1, out) != 1 )
	{
		AfxMessageBox( "Fread Failed");
		return FALSE;
	}

	time(&t4);

//	sprintf(temp8,"t1=%ld, t2=%ld, t3=%ld, t4=%ld", tim.t1, tim.t2, tim.t3, t4);
//	AfxMessageBox(temp8); 

	if( tim.t1 == 0 || tim.t2 == 0 )
		flag = 1;

	if( t4 < tim.t3 )
		flag = 2;
	if( t4 < tim.t1 )
		flag = 2;
	if( t4 > tim.t2 )
		flag = 3;
	CString s;
	switch( flag )
	{
		case 1:
			s = "Error in Evaluation File";
			break;
		case 2:
			s = "Time & Date has been Jumbled";
			break;
		case 3:
			s = "Evaluation Period Over";
			break;
		case 0:
			tim.t3 = t4;
			fseek( out, 0, SEEK_SET);
			fwrite(	&tim,1,12,out);
			fclose(out);
			break;
	}
	if( flag )
	{
		AfxMessageBox(s);
		return FALSE;
	}	
#endif //WANT_COPY_PROTECTION

	// This looks like a good time to open up a console for some debugging output
#ifdef _DEBUG
	::AllocConsole();
	freopen( "CONOUT$", "r+", stdout );
	freopen( "CONOUT$", "r+", stderr );
#endif

	printf( "PM3 Lite console initialized!\n" );

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	m_Transform[0] = 1.0; m_Transform[1] = 0.0; m_Transform[2] = 0.0;	
	m_Transform[3] = 0.0; m_Transform[4] = 0.0; m_Transform[5] = 1.0;	
	m_Transform[6] = 0.0; m_Transform[7] = -1.0; m_Transform[8] = 0.0;	
	m_Transform[9] = 0.0; m_Transform[10] = 0.0; m_Transform[11] = 0.0;	

	KillTheTAF();
	hMainCursor = NULL; //NULL the app cursor

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	Enable3dControls();

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	
	m_pDocTemplate = new CMultiDocTemplate(
		IDR_BITMAPTYPE,
		RUNTIME_CLASS(CFotobrowDoc),
		RUNTIME_CLASS(CMDIChildWnd),          // standard MDI child frame
		RUNTIME_CLASS(CFotobrowView));
	AddDocTemplate(m_pDocTemplate);

	m_pGraphicTemplate = new CMultiDocTemplate(
		IDR_GRAPHICS,
		RUNTIME_CLASS(CFotogDoc),
		RUNTIME_CLASS(CFrameMods),          // standard MDI child frame
		RUNTIME_CLASS(CFotoView));
	AddDocTemplate(m_pGraphicTemplate);

/************************* Temporary hack during datatable switch *******/
//#if FALSE
/************************* Temporary hack during datatable switch *******/
	m_pZoneGridTemplate = new CMultiDocTemplate(
		IDR_ZONEGRID2,
		RUNTIME_CLASS(CRaimaDatabaseDoc),
		RUNTIME_CLASS(CFrameMods),          // standard MDI child frame
		RUNTIME_CLASS(CZoneEditor));
	AddDocTemplate(m_pZoneGridTemplate);
/************************* Temporary hack during datatable switch *******/
//#endif
/************************* Temporary hack during datatable switch *******/

/************************* Temporary hack during datatable switch *******/
//#if FALSE
/************************* Temporary hack during datatable switch *******/
	m_pColumnGridTemplate = new CMultiDocTemplate(
		IDR_COLUMNGRID,
		RUNTIME_CLASS(CRaimaDatabaseDoc),
		RUNTIME_CLASS(CFrameMods),          // standard MDI child frame
		RUNTIME_CLASS(CColumnEditor));
	AddDocTemplate(m_pColumnGridTemplate);
/************************* Temporary hack during datatable switch *******/
//#endif
/************************* Temporary hack during datatable switch *******/

	m_pElementListTemplate = new CMultiDocTemplate(
		IDR_ELEMENTLIST,
		RUNTIME_CLASS(CRaimaDatabaseDoc),
		RUNTIME_CLASS(CFrameMods),          // standard MDI child frame
		RUNTIME_CLASS(CElementListView));
	AddDocTemplate(m_pElementListTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, m_pDocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	if( !ReadProfile() )
		return FALSE;

	int i;
	dt_opentask(&Mytask);
	dt_lockcomm(LMC_INTERNAL,&Mytask);
	
	dt_on_opt(READONLY, &Mytask);

	dt_dbuserid("DJB3",&Mytask);
	CString temp2;

	temp2 = m_DatabasePath + "photog";
//	AfxMessageBox(temp2);
	if( (i=dt_open((char *)(const char *)temp2,"s", &Mytask)) != S_OKAY)
	{
		char temp[128];
		sprintf(temp,"The return code is %d", i);
		AfxMessageBox(temp);
		dt_closetask( &Mytask );
	}
	databaseOpenFlag = TRUE;
//	dt_close(&Mytask);
	dbn = CURR_DB;


	// create main MDI Frame window
	CMainFrame* pMainFrame;
	m_pMainWnd = pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	AfxGetApp()->m_pMainWnd->SendMessage(UM_CURRENT_ELEMENT_CHANGE,(WPARAM)GETMODEL,(LPARAM)GETMODEL);

//	qinitElemList();  //moved so that targets would be loaded on init
	
	HICON hIcon1 = LoadIcon(IDR_MAINFRAME);
	pMainFrame->SetIcon(hIcon1, TRUE);
	HICON hIcon2 = LoadIcon(IDI_SMALL_MAINFRAME);
	pMainFrame->SetIcon(hIcon2,FALSE);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes();

	// Parse the command line to see if launched as OLE server
	if (RunEmbedded() || RunAutomated())
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	// simple command line parsing
	if (m_lpCmdLine[0] == '\0')
	{
		// create a new (empty) document
//		OnFileNew();
	}
	else
	{
		// open an existing document
		OpenDocumentFile(m_lpCmdLine);
	}

	CString temp = m_DatabasePath + "tri";
	t = new TritonTask((char *)(const char *)temp);
	TransAction a1;
	a1.Begin("wow");
	LineObj l;
	l.x1 = 1;
	l.y1 = 1;
	l.z1 = 1;
	l.NewObj();
	a1.End();

//	m_Units = GetProfileInt( INI_BITMAPS, INI_UNITS, 1 );
//	m_ImagePath = GetProfileString( INI_BITMAPS, INI_IMAGEPATH, INI_IMAGEPATHDEF);

//	AfxMessageBox("After CLose");
//	dt_closetask(&Mytask);

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();



	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   CDC m_dcMem;            // Compatible Memory DC for dialog
   CBitmap m_bmpABD;    // Bitmap to display
   HBITMAP m_hBmpOld;    // Handle of old bitmap to save

   BITMAP m_bmInfo;        // Bitmap Information structure
   CPoint m_pt;            // Position for upper left corner of bitmap
   CSize m_size;        // Size (width and height) of bitmap
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CBitmap * pBmpOld;
        
    // Load bitmap resource - remember to call DeleteObject when done.
    VERIFY( m_bmpABD.LoadBitmap( IDB_PM3_SYMBOL ) );

    // Get bitmap information    
    m_bmpABD.GetObject( sizeof(BITMAP), &m_bmInfo );
    m_size.cx = m_bmInfo.bmWidth;
    m_size.cy = m_bmInfo.bmHeight;

    // Hard code upper left position to place bitmap
    m_pt.x = 10;
    m_pt.y = 10;
    
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
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
    dc.StretchBlt( m_pt.x, m_pt.y, m_size.cx/3.2, m_size.cy/1.58, &m_dcMem, 0, 0, 
    			m_size.cx, m_size.cy, SRCCOPY );
	// Do not call CDialog::OnPaint() for painting messages
}


// App command to run the dialog
void CFotobrowApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CFotobrowApp commands

void CFotobrowApp::AddDocToList(CDocument *p)
{
	//POSITION pos;
	/*
	void *pos;
	m_DocList.AddTail(p);
	TRACE("ADDING\n");
	for( pos = m_DocList.GetHeadPosition(); pos != NULL; )
	{
		CDocument *doc = (CDocument *)m_DocList.GetNext(pos);
	    TRACE("DOC name - %s\n", doc->GetPathName() );
	}
	*/
}

void CFotobrowApp::DelDocFromList(CDocument *p)
{
//CobList
	//POSITION pos;
	/*
	void *pos;
	m_DocList.RemoveAt(m_DocList.Find(p));
	TRACE("REMOVING\n");
	for( pos = m_DocList.GetHeadPosition(); pos != NULL; )
	{
		CDocument *doc = (CDocument *)m_DocList.GetNext(pos);
	    TRACE("DOC name - %s\n", doc->GetPathName() );
	}
	TRACE("After Removing");
	*/
}

int CFotobrowApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
//	AfxMessageBox("Deleting the Task");
//	AfxMessageBox("Calling Delete t");
//	delete t;		// Don't delete the task DJB 6/28/96 Caused a GP on Exit in RemoveVistaDb@VistaTask
					// But commenting the delete out is a memory leak. DJB 9/28/96
//	AfxMessageBox("After Delete t");

	// This looks like a good time to kill our console
#ifdef _DEBUG
	::FreeConsole();
#endif

	if( isDatabaseOpen() )
	{
		dt_close(&Mytask);
		dt_closetask(&Mytask);
		if( rvqTask )
			delete rvqTask;
	}
//	AfxMessageBox("Deleted the task");
//	WriteProfile();
	return TRUE;
}

void CFotobrowApp::OnFileNew() 
{
	// TODO: Add your command handler code here
//	AfxMessageBox("I am in File new");
//	OnFileNew();
}

void CFotobrowApp::GetDocumentList(CObList * pDocList)
{
	 ASSERT(pDocList->IsEmpty());
     POSITION pos = GetFirstDocTemplatePosition();
     while (pos)
     {
          CDocTemplate* pTemplate =
 					(CDocTemplate*)GetNextDocTemplate(pos);
          POSITION pos2 = pTemplate->GetFirstDocPosition();
          while (pos2)
          {
              CDocument * pDocument;
               if ((pDocument=pTemplate->GetNextDoc(pos2)) != NULL)
                         pDocList->AddHead(pDocument);
          }
     }
}

void CFotobrowApp::KillTheTAF()
{
	CStdioFile taf;
	if( taf.Open("vista.taf", CFile::modeRead ) )
	{
		taf.Close();
		if( remove("vista.taf") )
			AfxMessageBox("Could not delete the TAF file");
	}		
}

int CFotobrowApp::ReadProfile()
{
//	char temp[128];
	CString input;

//	m_Units = GetProfileInt( INI_BITMAPS, INI_UNITS, 1 );
//	m_ImagePath = GetProfileString( INI_BITMAPS, INI_IMAGEPATH, INI_IMAGEPATHDEF);
//	AfxMessageBox(m_ImagePath);
/*	m_DatabasePath = GetProfileString( INI_DATABASE, INI_DBPATH, INI_DBPATHDEF );*/
	if( !ReadRegistry() )
		return FALSE;
	m_Units = getUnits();
	getMeasurementTransform(m_Transform, m_ProjectName);
//	char temp[128];
//	sprintf(temp,"Proj:%s, trans[0]:%f", m_ProjectName, m_Transform[0]);
//	AfxMessageBox(temp);
	/*
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM0, "1.0");
	m_Transform[0] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM1, "0.0");
	m_Transform[1] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM2, "0.0");
	m_Transform[2] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM3, "0.0");
	m_Transform[3] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM4, "0.0");
	m_Transform[4] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM5, "-1.0");
	m_Transform[5] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM6, "0.0");
	m_Transform[6] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM7, "1.0");
	m_Transform[7] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM8, "0.0");
	m_Transform[8] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM9, "0.0");
	m_Transform[9] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM10, "0.0");
	m_Transform[10] = atof((const char *)input);
	input = GetProfileString(INI_TRANSFORM, INI_TRANSFORM11, "0.0");
	m_Transform[11] = atof((const char *)input);
*/
	return TRUE;
}

void CFotobrowApp::WriteProfile()
{
//	char temp[128];
//	sprintf(temp,"Proj:%s, trans[0]:%f", m_ProjectName, m_Transform[0]);
//	AfxMessageBox(temp); 
//	m_Transform[0] = -5.989;
	saveMeasurementTransform(m_Transform, m_ProjectName);	
/*	char temp[128];
	if( m_Transform[0] == 0 &&
		m_Transform[1] == 0 &&
		m_Transform[2] == 0 &&
		m_Transform[3] == 0 &&
		m_Transform[4] == 0 &&
		m_Transform[5] == 0 &&
		m_Transform[6] == 0 &&
		m_Transform[7] == 0 &&
		m_Transform[8] == 0 &&
		m_Transform[9] == 0 &&
		m_Transform[10] == 0 &&
		m_Transform[11] == 0 )
	{
		m_Transform[0] = m_Transform[4] = m_Transform[8] = 1;
	}
		

	sprintf(temp, "%f", m_Transform[0] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM0, 	temp);
	sprintf(temp, "%f", m_Transform[1] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM1, 	temp);
	sprintf(temp, "%f", m_Transform[2] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM2, 	temp);
	sprintf(temp, "%f", m_Transform[3] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM3, 	temp);
	sprintf(temp, "%f", m_Transform[4] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM4, 	temp);
	sprintf(temp, "%f", m_Transform[5] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM5, 	temp);
	sprintf(temp, "%f", m_Transform[6] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM6, 	temp);
	sprintf(temp, "%f", m_Transform[7] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM7, 	temp);
	sprintf(temp, "%f", m_Transform[8] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM8, 	temp);
	sprintf(temp, "%f", m_Transform[9] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM9, 	temp);
	sprintf(temp, "%f", m_Transform[10] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM10, 	temp);
	sprintf(temp, "%f", m_Transform[11] );
	WriteProfileString( INI_TRANSFORM, INI_TRANSFORM11, 	temp);

	WriteProfileInt( INI_BITMAPS, INI_UNITS, m_Units );
//	WriteProfileString( INI_BITMAPS, INI_IMAGEPATH, m_ImagePath);
*/
}

int CFotobrowApp::ReadRegistry()
{
//	AfxMessageBox("Createing the open dialog");
	COpenProjectDlg dlg;
	if( dlg.DoModal() == IDCANCEL )
	{
		PostQuitMessage(0);
		return FALSE;
	}
	else
		return TRUE;
}

void CFotobrowApp::WriteRegistry()
{
	
}


afx_msg LONG CFotobrowApp::OnSetProject( struct EnvStrings *pStr )
{
	m_DatabasePath = pStr->dbPath;
	m_ImagePath = pStr->imgPath;
	m_ProjectName = pStr->projectID;
	m_imageReduction = atof(pStr->imageReductionFactor);
	return 0L;
}

void CFotobrowApp::SetReferenceNode( Ptg3DNode *newRefNode )
{
	((CMainFrame *)m_pMainWnd)->measurementRefNode = newRefNode;

	((CMainFrame *)m_pMainWnd)->get3DPointGridBar()->RefreshGrid();
}

void CFotobrowApp::SetTransform( double data[] )
{
	// 09/15/97 - jwb
	// Here's an explanation for what's going on here.  From here on,
	// PM3 Lite is going to attempt to only deal with transforms in a
	// form that is child to parent.  Just so happens, the measurement transform
	// is calculated from Photog to Measurement.  This is the way it has
	// always been stored.  For storeage in the hierarchy however, the matrix
	// needs to be from measurement to photog.
	for( int i = 0; i < 12; i++ )
		m_Transform[i] = data[i];
	saveMeasurementTransform(m_Transform, m_ProjectName);

	Ptg3DNode *refNode = GETWORLD.findByName( "Default" );
	if( refNode )
	{
		VECTOR v1 = m_Transform - 1;
		VECTOR v4 = matinv(v1);

		refNode->setMatrix(v4);
		free_vector( v4, 1, 12 );
	}

	((CMainFrame *)m_pMainWnd)->get3DPointGridBar()->RefreshGrid();
}
