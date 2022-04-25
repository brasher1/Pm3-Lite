// coordxfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoordXformDlg dialog

class CCoordXformDlg : public CDialog
{
// Construction
public:
	CCoordXformDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCoordXformDlg)
	enum { IDD = IDD_COORD_XFORM };
	double	m_CalcEast;
	double	m_CalcNorth;
	double	m_CalcUp;
	double	m_Point1East;
	double	m_Point1North;
	double	m_Point2East;
	double	m_Point2North;
	int		m_AxisDir;
	double	m_RealEast;
	double	m_RealNorth;
	double	m_RealUp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoordXformDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCoordXformDlg)
	virtual void OnOK();
	afx_msg void OnMakeIdentity();
	afx_msg void OnImportData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
