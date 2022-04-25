// searchbx.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "searchbx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListerBox

CListerBox::CListerBox()
{
}

CListerBox::~CListerBox()
{
}

BEGIN_MESSAGE_MAP(CListerBox, CComboBox)
	//{{AFX_MSG_MAP(CListerBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListerBox message handlers

BOOL CListerBox::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message != WM_KEYDOWN) || (pMsg->wParam != VK_RETURN))
		return CComboBox::PreTranslateMessage(pMsg);

	// when the enter key is hit in the ComboBox we want to add the string
	// to the top of the list and hilight it.  We also want to limit the
	// list to the last 15 entries.
	if ((pMsg->lParam & 0x40000000) == 0)   // Not a repeat.
	{
		CString strText;
		GetWindowText(strText);
		InsertString(0, strText);
		SetCurSel(0);
		if (GetCount() > 15)
			DeleteString(GetCount()-1);
	}
	return TRUE;
}
