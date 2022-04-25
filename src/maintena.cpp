// maintena.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "maintena.h"
#include "common.h"
#include "radiographydata.h"
	   
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Maintenance property page
static rvqLiteTask *rvqthisTask;

IMPLEMENT_DYNCREATE(Maintenance, CPropertyPage)

Maintenance::Maintenance() : CPropertyPage(Maintenance::IDD)
{
	//{{AFX_DATA_INIT(Maintenance)
	m_EqListString = _T("");
	//}}AFX_DATA_INIT
	time_t t;
	time(&t);
	char temp1[128];
	sprintf(temp1,"%ld", t);
	rvqthisTask = new rvqLiteTask(temp1);
	rvqthisTask->MakeCurrent();
	CString temp = ((CFotobrowApp *)AfxGetApp())->m_DatabasePath + "rvq";
	rvqthisTask->SetPaths((char *)(const char *)temp,(char *)(const char *)temp,
					NULL, NULL, temp1 );
	if( !rvqthisTask->openDatabase("rvqlite") )
	{
		AfxMessageBox("Error opening database");
		delete rvqthisTask;
		rvqthisTask = NULL;
	}
//	AfxMessageBox("Constructed the maint page");
}

Maintenance::~Maintenance()
{
	delete rvqthisTask;
//	AfxMessageBox("Called delete for rvqthisTask");
	rvqthisTask = NULL;
}

void Maintenance::DoDataExchange(CDataExchange* pDX)
{										 //CListBox
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Maintenance)
	DDX_Control(pDX, IDC_EQ_LIST, m_EqListBox);
	DDX_LBString(pDX, IDC_EQ_LIST, m_EqListString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Maintenance, CPropertyPage)
	//{{AFX_MSG_MAP(Maintenance)
	ON_BN_CLICKED(IDC_RADIOGRAPHY_DATA, OnRadiographyData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Maintenance message handlers

BOOL Maintenance::OnInitDialog() 
{
	int stops[] = { 2, 37, 119, 207, 261 };
	CPropertyPage::OnInitDialog();
	m_EqListBox.SetTabStops( 5, stops);
//////////////////////////////////XXX
///////////////
	rvqthisTask->MakeCurrent();

	equipObj eq;
//AfxMessageBox("Constructed the maint page - Locking Eq");	
	eq.ReadLock();
	char temp[128];
//	MessageBeep(MB_ICONEXCLAMATION);
	for( eq.First(); eq.Okay(); eq.Next() )
	{
//		MessageBeep(MB_OK);
		sprintf(temp,"Area\t%s\tthird\tfourth\tfifth", eq.name);
		m_EqListBox.AddString(temp);	
	}	
	UpdateData(TRUE);
	eq.FreeLock();
//AfxMessageBox("Constructed the maint page - Freeing EQ");
//////////////////////////////////XXX		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Maintenance::OnRadiographyData() 
{
	CRadiographyData dlg;
	dlg.DoModal();
	
}
