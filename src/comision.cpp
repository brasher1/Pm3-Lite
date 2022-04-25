// comision.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "comision.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Commisioning property page

IMPLEMENT_DYNCREATE(Commisioning, CPropertyPage)

Commisioning::Commisioning() : CPropertyPage(Commisioning::IDD)
{
	//{{AFX_DATA_INIT(Commisioning)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

Commisioning::~Commisioning()
{
}

void Commisioning::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Commisioning)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Commisioning, CPropertyPage)
	//{{AFX_MSG_MAP(Commisioning)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Commisioning message handlers
