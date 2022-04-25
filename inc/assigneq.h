// assigneq.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAssignEquipDlg dialog

class CAssignEquipDlg : public CDialog
{
private:
	CRect m_box;
	long dba;
		
// Construction
public:
	CAssignEquipDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAssignEquipDlg)
	enum { IDD = IDD_PICKEQUIP };
	CComboBox	m_AssignEquipBox;
	CString	m_EquipString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssignEquipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
   void SetImageData( CRect &r, long dba );


protected:

	// Generated message map functions
	//{{AFX_MSG(CAssignEquipDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
