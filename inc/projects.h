// projects.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProjectSettings dialog

class CProjectSettings : public CDialog
{
// Construction
public:
	CProjectSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectSettings)
	enum { IDD = IDD_PROJECT_SETTINGS };
	CListBox	m_ProjectList;
	CString	m_DatabasePath;
	CString	m_ImagePath;
	CString	m_ProjectID;
	CString	m_ProjectSel;
	CString	m_ReductionFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectSettings)
	virtual BOOL OnInitDialog();
	afx_msg void OnRemove();
	afx_msg void OnApply();
	afx_msg void OnOK();
	afx_msg void OnDblclkProjlist();
	afx_msg void OnSurveyXform();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
