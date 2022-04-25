// columnfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColumnFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "../triton/triton.hpp"

class CColumnFormView : public CFormView
{
protected:
	CColumnFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CColumnFormView)

// Form Data
public:
	//{{AFX_DATA(CColumnFormView)
	enum { IDD = IDD_COLUMNGRID };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
   ClassesObj  m_class;
   LineObj		m_line;
   AllElements  m_all;
// Operations
public:
	void InsertRow();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColumnFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CColumnFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CColumnFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeleteColumn();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	afx_msg void OnDataUpdate();
	afx_msg void OnDataChange();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
