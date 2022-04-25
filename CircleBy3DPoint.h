// CircleBy3DPoint.h : header file
//
#ifndef __CIRCLEBY3DPOINT_H__
#define __CIRCLEBY3DPOINT_H__

#include "elem_help.h"
#include "photog.h"
#include "ele_math.h"

/////////////////////////////////////////////////////////////////////////////
// CCircleBy3DPoint dialog

class CCircleBy3DPoint : public CDialog
{
// Construction
public:
	CCircleBy3DPoint(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCircleBy3DPoint)
	enum { IDD = IDD_CIRCLE_BY_3DPOINT };
	int		m_iNumberOfPoints;
	//}}AFX_DATA

	//int m_iNumberOfPoints;
	PNTLST pl3DPnts;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCircleBy3DPoint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCircleBy3DPoint)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCalculate();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //__CIRCLEBY3DPOINT_H__