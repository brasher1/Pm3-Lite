// RadiographyData.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "RadiographyData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadiographyData dialog
class CBrowserDlg:public CFileDialog
{
	public:
	CBrowserDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL) :
		CFileDialog(bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		lpszDefExt,
		lpszFileName,
		dwFlags,
		lpszFilter,
		pParentWnd) {};
;

BOOL OnInitDialog() 
{
	CFileDialog::OnInitDialog(); 
	SetWindowText("Select an Application"); 
	return TRUE;
	}	
};


CRadiographyData::CRadiographyData(CWnd* pParent /*=NULL*/)
	: CDialog(CRadiographyData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRadiographyData)
	m_AssocApp = _T("");
	m_DataFile = _T("");
	//}}AFX_DATA_INIT
}


void CRadiographyData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRadiographyData)
	DDX_Text(pDX, IDC_SPECIALAPP, m_AssocApp);
	DDX_Text(pDX, IDC_DATAFILE, m_DataFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRadiographyData, CDialog)
	//{{AFX_MSG_MAP(CRadiographyData)
	ON_BN_CLICKED(IDC_BROWSE_APP, OnBrowseApp)
	ON_BN_CLICKED(IDC_BROWSE_DATAFILE, OnBrowseDatafile)
	ON_BN_CLICKED(IDC_VIEWFILE, OnViewfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadiographyData message handlers

void CRadiographyData::OnBrowseApp() 
{
	CBrowserDlg dlg(TRUE, "exe", NULL, 0, 
					"Executable Files (*.exe) | *.exe||");
//	AfxMessageBox(dlg.GetPathName());
	if( dlg.DoModal() == IDOK )
	{
		m_AssocApp = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CRadiographyData::OnBrowseDatafile() 
{
	CBrowserDlg dlg(TRUE, "*", NULL, 0, 
					"Data Files (*.*) | *.*||");
//	AfxMessageBox(dlg.GetPathName());
	if( dlg.DoModal() == IDOK )
	{
		m_DataFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CRadiographyData::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CRadiographyData::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CRadiographyData::OnViewfile() 
{
	STARTUPINFO start;
	memset(&start,0,sizeof(start));
	start.cb = sizeof(start);
	PROCESS_INFORMATION proc;

	UpdateData(TRUE);
	int i=0;
//	while(doc.datafile[i] != NULL )
//	{
//		if( doc.datafile[i] == '/' )
//			doc.datafile[i] = '\\';
//		i++;
//	} 
	CString fullcmd;
	fullcmd = m_AssocApp;
	fullcmd += " ";
//	fullcmd += (char *)(const char *)(((CMaintSysApp *)AfxGetApp())->m_DatabasePath),
	fullcmd += m_DataFile;
//	AfxMessageBox(fullcmd);
//	doc.FreeLock();
	CreateProcess(
		NULL,	// pointer to name of executable module 
		(char *)(const char *)fullcmd,	// pointer to command line string
		NULL,	// pointer to process security attributes 
		NULL,	// pointer to thread security attributes 
		TRUE,	// handle inheritance flag 
		NORMAL_PRIORITY_CLASS,	// creation flags 
		NULL,	// pointer to new environment block 
		NULL,	// pointer to current directory name 
		&start,	// pointer to STARTUPINFO 
		&proc   // pointer to PROCESS_INFORMATION  
	);
}
