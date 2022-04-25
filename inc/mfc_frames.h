#ifndef _MFC_FRAMES_H_
#define _MFC_FRAMES_H_
// mfc_frames.h : header file
//
#include <afxwin.h>

/////////////////////////////////////////////////////////////////////////////
// CFrameMods frame

class CFrameMods : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameMods)
protected:
	CFrameMods();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameMods)
	public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
/*	virtual ~CFrameMods();*/

	// Generated message map functions
	//{{AFX_MSG(CFrameMods)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
/*	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFrameMods)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()*/
};

/////////////////////////////////////////////////////////////////////////////
#endif //_MFC_FRAMES_H_
