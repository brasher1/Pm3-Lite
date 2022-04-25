// selecteq.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectEqImageDlg dialog
#include <afxcmn.h>
#include <vista.h>

#include "mainfrm.h"
#include "fotobrow.h"
#include "engineer.h"
#include "maintena.h"
#include "comision.h"
#include "construct.h"

class CSelectEqImageDlg : public CDialog
{
// Construction
public:
	CSelectEqImageDlg(CWnd* pParent = NULL);   // standard constructor
	void OpenImageByDba( DB_ADDR dba);

// Dialog Data
	//{{AFX_DATA(CSelectEqImageDlg)
	enum { IDD = IDD_SELECT_EQUIP };
	CButton	m_CloseButton;
	CTreeCtrl	m_EqTree;
	//}}AFX_DATA

	CPropertySheet *dlgPropertySheet;
	EngineeringData *engrPage;
	Maintenance *maintPage;
	Construction *constPage;
	Commisioning *commPage;
	

	HIMAGELIST m_hIml;  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectEqImageDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectEqImageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkEqList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkEqList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedEqList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUnlink();
	virtual void OnOK();
	afx_msg void OnOpenImage();
	afx_msg void OnEndlabeleditEqList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEquipTagData();
	afx_msg void OnBeginlabeleditEqList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
