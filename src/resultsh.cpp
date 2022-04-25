// resultsh.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "resultsh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultSheetDlg

IMPLEMENT_DYNAMIC(CResultSheetDlg, CPropertySheet)

CResultSheetDlg::CResultSheetDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CResultSheetDlg::CResultSheetDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CResultSheetDlg::~CResultSheetDlg()
{
}


BEGIN_MESSAGE_MAP(CResultSheetDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CResultSheetDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResultSheetDlg message handlers
