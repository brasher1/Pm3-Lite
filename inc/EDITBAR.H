#ifndef __EDITBAR_H__
#define __EDITBAR_H__
// editbar.h : header file
//

#ifndef __AFXEXT_H__
#include <afxext.h>         // for access to CToolBar
#endif
#include "searchbx.h"
#include "listerbx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditBar window

class CEditBar : public CToolBar
{
// Construction
public:
	BOOL m_bColor;
	CSearchBox m_SearchBox;
//	CListerBox m_ListBox;

	CEditBar();
	BOOL Init(CWnd* pParentWnd, BOOL bColor, BOOL bToolTips);
	void SetColor(BOOL Yes);
	BOOL SetHorizontal();
	BOOL SetVertical();
	int AddBundleString(const char *);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	CRect m_rectInsideVert;
	CRect m_rectInsideHorz;
	CSize m_sizeVert;
	CSize m_sizeHorz;

	virtual ~CEditBar();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
