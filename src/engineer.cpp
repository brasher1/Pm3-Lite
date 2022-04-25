// engineer.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "engineer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EngineeringData property page

IMPLEMENT_DYNCREATE(EngineeringData, CPropertyPage)

EngineeringData::EngineeringData() : CPropertyPage(EngineeringData::IDD)
{
	//{{AFX_DATA_INIT(EngineeringData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

EngineeringData::~EngineeringData()
{
}

void EngineeringData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EngineeringData)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EngineeringData, CPropertyPage)
	//{{AFX_MSG_MAP(EngineeringData)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// EngineeringData message handlers
