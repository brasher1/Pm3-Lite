// gridbar.h
//

#include "stdafx.h"
#include "resource.h"
#include "gridbar.h"


IMPLEMENT_DYNAMIC(CGridBar, CDialogBar)

BEGIN_MESSAGE_MAP(CGridBar, CDialogBar)
	//{{AFX_MSG_MAP(CGridBar)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CGridBar::AssertValid() const
{
	CDialogBar::AssertValid();
}

void CGridBar::Dump(CDumpContext& dc) const
{
	CDialogBar::Dump(dc);
}

#endif //_DEBUG


void CGridBar::OnEditCopy() 
{
	AfxMessageBox("On Edit Copy in Grid Bar");
		
}

void CGridBar::OnEditCut() 
{
	AfxMessageBox("On Edit Cut in Grid Bar");
	
}

void CGridBar::OnEditPaste() 
{
	AfxMessageBox("On Edit Paste in Grid Bar");
	
}
