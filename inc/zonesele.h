// zonesele.h : header file
//

#include "../triton/triton.hpp"
/////////////////////////////////////////////////////////////////////////////
// CZoneSelectDlg dialog

class CZoneSelectDlg : public CDialog
{
// Construction
public:
	CZoneSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CZoneSelectDlg)
	enum { IDD = IDD_SELECT_ZONE };
	CListBox	m_ZoneList;
	CString	m_ZoneString;
	//}}AFX_DATA

	ClassesObj c;
	PropertiesObj p;
	BOOL InsertRow();

	void SendClipMessage(long dba);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoneSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CZoneSelectDlg)
	afx_msg void OnDblclkZonelist();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
