// construct.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "construct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction property page

IMPLEMENT_DYNCREATE(Construction, CPropertyPage)

Construction::Construction() : CPropertyPage(Construction::IDD)
{
	//{{AFX_DATA_INIT(Construction)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

Construction::~Construction()
{
}

void Construction::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Construction)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Construction, CPropertyPage)
	//{{AFX_MSG_MAP(Construction)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction message handlers
