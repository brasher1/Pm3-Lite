// 3dpointf.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "3dpointf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DPointFrame dialog


C3DPointFrame::C3DPointFrame()
{
}

C3DPointFrame::~C3DPointFrame()
{

}



BEGIN_MESSAGE_MAP(C3DPointFrame, CDlgPanelFrame)
	//{{AFX_MSG_MAP(C3DPointFrame)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// C3DPointFrame message handlers

void C3DPointFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDlgPanelFrame::OnHScroll(nSBCode, nPos, pScrollBar);
}

void C3DPointFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDlgPanelFrame::OnVScroll(nSBCode, nPos, pScrollBar);
}

int C3DPointFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDlgPanelFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
