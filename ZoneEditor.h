// ZoneEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZoneEditor form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "triton\triton.hpp"

class CZoneEditor : public CFormView
{
protected:
	CZoneEditor();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CZoneEditor)

// Form Data
public:
	//{{AFX_DATA(CZoneEditor)
	enum { IDD = IDD_ZONEEDITOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	ClassesObj c;

// Operations
public:
	void InsertRow();
	void UpdateUnits( ZoneData *zd );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoneEditor)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CZoneEditor();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CZoneEditor)
	afx_msg void OnBeforeDeleteRowZonegrid(long Row, long FAR* Status);
	afx_msg void OnChangeZonegrid(long Row, long Column);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LONG OnUnitsChange(WPARAM wParam, LPARAM lParam);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
