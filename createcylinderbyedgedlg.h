#ifndef __CREATE_CYLINDER_BY_EDGE_H__
#define __CREATE_CYLINDER_BY_EDGE_H__
// CreateCylinderByEdgeDlg.h : header file
//
#include "resource.h"
#include "2DEdge.h"

/////////////////////////////////////////////////////////////////////////////
// CCreateCylinderByEdgeDlg dialog

class CCreateCylinderByEdgeDlg : public CDialog
{
// Construction
public:
	void UpdateButtons(void);
	CCreateCylinderByEdgeDlg(CWnd* pParent = NULL);   // standard constructor
	~CCreateCylinderByEdgeDlg();

// Dialog Data
	//{{AFX_DATA(CCreateCylinderByEdgeDlg)
	enum { IDD = IDD_CYLINDERBYEDGE };
	CButton	m_Cancel;
	CButton	m_Back;
	CButton	m_Next;
	CButton	m_Ok;
	CString	m_Label;
	//}}AFX_DATA

	int m_state;
	int m_edge;
	CList< Edge_2D *, Edge_2D * > m_listEdges;
	Edge_2D *m_currentEdge;
	Point_2D m_endPoints[2];

	void UpdateLabel(char *str);
	void processEdge();
	void processDirPoint();
	int processEndPoint();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateCylinderByEdgeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateCylinderByEdgeDlg)
	afx_msg void OnBack();
	afx_msg void OnNext();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //__CREATE_CYLINDER_BY_EDGE_H__