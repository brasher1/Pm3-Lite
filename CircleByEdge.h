#ifndef __CIRCLEBYEDGE_H__
#define __CIRCLEBYEDGE_H__

// CircleByEdge.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCircleByEdge dialog

class CCircleByEdge : public CDialog
{
// Construction
public:
	CCircleByEdge(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCircleByEdge)
	enum { IDD = IDD_CIRCLE_BY_EDGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCircleByEdge)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCircleByEdge)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //__CIRCLEBYEDGE_H__