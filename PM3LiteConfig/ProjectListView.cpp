// ProjectListView.cpp : implementation file
//

#include "stdafx.h"
#include "pm3liteconfig.h"
#include "ProjectListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectListView

IMPLEMENT_DYNCREATE(CProjectListView, CFormView)

CProjectListView::CProjectListView()
	: CFormView(CProjectListView::IDD)
{
	//{{AFX_DATA_INIT(CProjectListView)
	m_ProjectListValue = _T("");
	//}}AFX_DATA_INIT
}

CProjectListView::~CProjectListView()
{
}

void CProjectListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectListView)
	DDX_Control(pDX, IDC_LIST1, m_ProjectListBox);
	DDX_LBString(pDX, IDC_LIST1, m_ProjectListValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectListView, CFormView)
	//{{AFX_MSG_MAP(CProjectListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectListView diagnostics

#ifdef _DEBUG
void CProjectListView::AssertValid() const
{
	CFormView::AssertValid();
}

void CProjectListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectListView message handlers
