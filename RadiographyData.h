// RadiographyData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRadiographyData dialog

class CRadiographyData : public CDialog
{
// Construction
public:
	CRadiographyData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRadiographyData)
	enum { IDD = IDD_RADIOGRAPHY };
	CString	m_AssocApp;
	CString	m_DataFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRadiographyData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRadiographyData)
	afx_msg void OnBrowseApp();
	afx_msg void OnBrowseDatafile();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnViewfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
