// mfc_frames.cpp
//
#include "stdafx.h"

#include "mfc_frames.h"
#include "fotobrow.h"

/////////////////////////////////////////////////////////////////////////////
// CFrameMods

IMPLEMENT_DYNCREATE(CFrameMods, CMDIChildWnd)

CFrameMods::CFrameMods()
{
}

BEGIN_MESSAGE_MAP(CFrameMods, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFrameMods)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameMods message handlers

BOOL CFrameMods::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// only do this once
//	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);
	ASSERT(m_hMenuShared == NULL);      // only do once
/////
	CString title;
	TRACE("Beginning LoadFrame\n");         
	if( nIDResource == IDR_GRAPHICS )   
	{
//		AfxMessageBox("Got graphic title");
		title = "Graphical View";
	}
	else if( nIDResource == IDR_ZONEGRID2 )
	{
//		AfxMessageBox("Got Zone title");
		title = "Clipping Volumes";
	}
	else if( nIDResource == IDR_COLUMNGRID )
	{                     
//		AfxMessageBox("Got COLUMN title");
		title = "Column Definitions";
	}

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	// parent must be MDI Frame (or NULL for default)
	ASSERT(pParentWnd == NULL || pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	// will be a child of MDIClient
	ASSERT(!(dwDefaultStyle & WS_POPUP));
	dwDefaultStyle |= WS_CHILD;

	// if available - get MDI child menus from doc template
	ASSERT(m_hMenuShared == NULL);      // only do once
	CMultiDocTemplate* pTemplate;
	if (pContext != NULL &&
		(pTemplate = (CMultiDocTemplate*)pContext->m_pNewDocTemplate) != NULL)
	{
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)));
		// get shared menu from doc template
		m_hMenuShared = pTemplate->m_hMenuShared;
		m_hAccelTable = pTemplate->m_hAccelTable;
	}
	else
	{
		TRACE0("Warning: no shared menu/acceltable for MDI Child window\n");
			// if this happens, programmer must load these manually
	}

	CString strFullString, strTitle;
	if (strFullString.LoadString(nIDResource))
		AfxExtractSubString(strTitle, strFullString, 0);    // first sub-string

	ASSERT(m_hWnd == NULL);
	if (!CMDIChildWnd::Create(GetIconWndClass(dwDefaultStyle, nIDResource),
	  title, dwDefaultStyle, rectDefault,
	  (CMDIFrameWnd*)pParentWnd, pContext))
		return FALSE;   // will self destruct on failure normally
    
//    delete wow;
	// it worked !
	return TRUE;
}

BOOL CFrameMods::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( CMDIChildWnd::PreCreateWindow(cs)==0 ) return FALSE;
	cs.style&=~(LONG)FWS_ADDTOTITLE;
	return TRUE;
}
