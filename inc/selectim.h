// selectim.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectImages dialog

class CSelectImages : public CDialog
{
// Construction
public:
	CSelectImages(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectImages)
	enum { IDD = IDD_IMAGES };
	CListBox	m_ImageList;
	CString	m_SelectedString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectImages)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectImages)
	afx_msg void OnDblclkImagelist();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
