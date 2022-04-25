// zoneform.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZoneFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <datatbl.h>
//#include <cdatatbl.h>
//#include <dtmfc.h>

#include "../triton/triton.hpp"

class CZoneFormView : public CFormView
{
protected:
	CZoneFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CZoneFormView)

// Form Data
public:
	//{{AFX_DATA(CZoneFormView)
	enum { IDD = IDD_ZONEINPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	ClassesObj c;

// Operations
public:
	void InsertRow();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoneFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CZoneFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CZoneFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeleteZone();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	afx_msg void OnDataUpdate();
	afx_msg void OnDataChange();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
