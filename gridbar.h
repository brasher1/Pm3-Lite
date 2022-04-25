#ifndef __GRIDBAR_H__
#define __GRIDBAR_H__

class CGridBar: public CDialogBar
{
	DECLARE_DYNAMIC(CGridBar)
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridBar)
	//}}AFX_VIRTUAL

// Implementation
public:
//	virtual ~CGridBar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CGridBar)
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};
#endif //__GRIDBAR_H__
