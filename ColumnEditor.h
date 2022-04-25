// ColumnEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColumnEditor form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "triton\triton.hpp"

class CColumnEditor : public CFormView
{
protected:
	CColumnEditor();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CColumnEditor)

// Form Data
public:
	//{{AFX_DATA(CColumnEditor)
	enum { IDD = IDD_COLUMNEDITOR };
	//}}AFX_DATA

// Attributes
public:
   ClassesObj  m_class;
   LineObj		m_line;
   AllElements  m_all;

// Operations
public:
	void InsertRow();
	void UpdateUnits(ColumnData *row);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColumnEditor)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CColumnEditor();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CColumnEditor)
	afx_msg void OnChangeColumngrid(long Row, long Column);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBeforeDeleteRowColumngrid(long Row, long FAR* Status);
	afx_msg LONG OnUnitsChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnInsertedRowColumngrid(long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
