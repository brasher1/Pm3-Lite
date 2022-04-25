// splitfra.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "splitfra.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame

IMPLEMENT_DYNCREATE(CSplitFrame, CMDIFrameWnd)

CSplitFrame::CSplitFrame()
{
}

CSplitFrame::~CSplitFrame()
{
}


BEGIN_MESSAGE_MAP(CSplitFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CSplitFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplitFrame message handlers

BOOL CSplitFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	return m_wndSplitter.Create( this, 2,2,CSize(10,10), pContext);
			
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
