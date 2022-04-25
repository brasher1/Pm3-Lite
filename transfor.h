// transfor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransformDlg dialog

class CTransformDlg : public CDialog
{
// Construction
public:
	CTransformDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTransformDlg)
	enum { IDD = IDD_PDMS_TRANSFORM };
	double	m_Trans0;
	double	m_Trans1;
	double	m_Trans2;
	double	m_Trans3;
	double	m_Trans4;
	double	m_Trans5;
	double	m_Trans6;
	double	m_Trans7;
	double	m_Trans8;
	double	m_Trans9;
	double	m_Trans10;
	double	m_Trans11;
	//}}AFX_DATA

	CString m_ProjectID;

	void setProjectID(const char *string);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransformDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransformDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
