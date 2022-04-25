// generalp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralPageDlg dialog

class CGeneralPageDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CGeneralPageDlg)

// Construction
public:
	CGeneralPageDlg();
	~CGeneralPageDlg();

// Dialog Data
	//{{AFX_DATA(CGeneralPageDlg)
	enum { IDD = IDD_GENERAL };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralPageDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
