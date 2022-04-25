// BundleCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BundleCalcDlg.h"

////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "photog.h"
#include "mathsubs.h"
#include "photog_math.h"
//#include "ptgGlobals.h"
#include "bundleutils.h"
#include "output.h"
#include "vista.h"
#include "dbtype.h"

#include "rdm_utils.h"

//#include "internal.h"
#define TASKER		&Mytask,dbn

#ifdef _FOTOBROW_CPP_
	DB_TASK	Mytask;
	int dbn;
#else
	extern DB_TASK	Mytask;
	extern int dbn;
#endif
////////////////////////////////////////////////////////////////////
int bundlemain(char *,int);
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBundleCalcDlg dialog


CBundleCalcDlg::CBundleCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBundleCalcDlg::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CBundleCalcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBundleCalcDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CBundleCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBundleCalcDlg)
	DDX_Control(pDX, IDC_BUNDLE_LIST, m_BundleList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBundleCalcDlg, CDialog)
	//{{AFX_MSG_MAP(CBundleCalcDlg)
	ON_BN_CLICKED(IDC_CALCULATE, OnCalculate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBundleCalcDlg, CDialog)
	//{{AFX_DISPATCH_MAP(CBundleCalcDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IBundleCalcDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3DE252CF-31C0-11D0-B391-00A024949B4A}
static const IID IID_IBundleCalcDlg =
{ 0x3de252cf, 0x31c0, 0x11d0, { 0xb3, 0x91, 0x0, 0xa0, 0x24, 0x94, 0x9b, 0x4a } };

BEGIN_INTERFACE_MAP(CBundleCalcDlg, CDialog)
	INTERFACE_PART(CBundleCalcDlg, IID_IBundleCalcDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBundleCalcDlg message handlers

void CBundleCalcDlg::OnCalculate() 
{
	int index = m_BundleList.GetCurSel();
	CString s;
	m_BundleList.GetText(index,s);
//	AfxMessageBox(s);
	CButton *button = (CButton *)GetDlgItem(IDC_INCREMENTAL_BUNDLE);

	int x = bundlemain((char *)(const char *)s,0x0003 & (button->GetState()) );
	if( x == 0 )
		AfxMessageBox("bundle main succeeded");
	else
		AfxMessageBox("bundle main FAILURE");
	
}

void CBundleCalcDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CBundleCalcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int status;
	
	rdmRecLock(BUNDLE, "r", TASKER);
	status = dt_keyfrst(BUNDLE_SZTAG, TASKER);
	while( status == S_OKAY )
	{
		char bundle[SIZEOF_BUNDLE_SZTAG];
		dt_keyread(bundle, &Mytask);
//		AfxMessageBox(bundle);
		m_BundleList.AddString(bundle);
		status = dt_keynext(BUNDLE_SZTAG, TASKER);
	}
	rdmRecFree(BUNDLE,TASKER);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
