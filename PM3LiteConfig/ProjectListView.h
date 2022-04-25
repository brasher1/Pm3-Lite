// ProjectListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProjectListView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CProjectListView : public CFormView
{
protected:
	CProjectListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CProjectListView)

// Form Data
public:
	//{{AFX_DATA(CProjectListView)
	enum { IDD = IDD_PROJLIST };
	CListBox	m_ProjectListBox;
	CString	m_ProjectListValue;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectListView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CProjectListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CProjectListView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
