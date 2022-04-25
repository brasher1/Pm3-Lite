// 3dpoints.h : header file
//
#ifndef __3DPOINTS_H__
#define __3DPOINTS_H__
/////////////////////////////////////////////////////////////////////////////
// C3DPointsDlg dialog

#include "common.h"

class C3DPointsDlg : public CDialogBar
{
	DECLARE_DYNCREATE(C3DPointsDlg)

// Construction
public:
	C3DPointsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(C3DPointsDlg)
	enum { IDD = IDD_3POINTS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	void AddRow( GridRowData *ptr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DPointsDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(C3DPointsDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLfClick3dpointgrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg void OnGridLeftClick();
	DECLARE_MESSAGE_MAP()
};
#endif //__3DPOINTS_H__
