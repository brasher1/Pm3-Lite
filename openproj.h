// openproj.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenProjectDlg dialog

class COpenProjectDlg : public CDialog
{
// Construction
public:
	COpenProjectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COpenProjectDlg)
	enum { IDD = IDD_OPEN_PROJECTS };
	CListBox	m_ProjectList;
	CString	m_ProjectPick;
	//}}AFX_DATA

	HKEY one;
	int setProjectsList(HKEY one1);//{ one = one1; AfxMessageBox("Setting the List"); return 0;}

	CPtrList list;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenProjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenProjectDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkProjectList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
