// 3dpointf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DPointFrame dialog
#include "dlgpanfm.h"

class C3DPointFrame : public CDlgPanelFrame
{
	DECLARE_DYNCREATE(C3DPointFrame)
// Construction
public:
	C3DPointFrame();   // standard constructor

// Implementation
protected:
	virtual ~C3DPointFrame();

private:
	// Generated message map functions
	//{{AFX_MSG(C3DPointFrame)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
