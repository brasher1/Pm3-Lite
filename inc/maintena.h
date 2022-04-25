// maintena.h : header file
//
#ifndef __MAINTENA_H__
#define __MAINTENA_H__

/////////////////////////////////////////////////////////////////////////////
// Maintenance dialog

class Maintenance : public CPropertyPage
{
	DECLARE_DYNCREATE(Maintenance)

// Construction
public:
	Maintenance();
	~Maintenance();

// Dialog Data
	//{{AFX_DATA(Maintenance)
	enum { IDD = IDD_MAINTANANCE };
	CListBox	m_EqListBox;
	CString	m_EqListString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(Maintenance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(Maintenance)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadiographyData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif //__MAINTENA_H__