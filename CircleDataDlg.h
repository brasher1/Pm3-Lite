// CircleDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCircleDataDlg dialog

class CCircleDataDlg : public CDialog
{
// Construction
public:
	CCircleDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCircleDataDlg)
	enum { IDD = IDD_CIRCLE };
	double	m_Radius;
	double	m_East;
	double	m_Height;
	double	m_North;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCircleDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void setValues(double radius, double height, double *endpoints);
	void getValues(double *radius, double *height, double *endpoints);

	
protected:

	// Generated message map functions
	//{{AFX_MSG(CCircleDataDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
