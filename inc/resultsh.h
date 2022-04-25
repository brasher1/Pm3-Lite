// resultsh.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResultSheetDlg

class CResultSheetDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CResultSheetDlg)

// Construction
public:
	CResultSheetDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CResultSheetDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultSheetDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResultSheetDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CResultSheetDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
