// PointGridBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PointGridBar window

#ifndef __PointGridBar_h__
#define __PointGridBar_h__

#include "Datatbl.h"

#include "3dclass/Ptg3DNode.h"

class PointGridBar : public CDialogBar
{
// Construction
public:
	PointGridBar();
	BOOL Create(CWnd* pParentWnd, UINT nStyle);

// Attributes
public:

// Operations
public:
	void UpdateUnits(GridRowData *row);
	void RefreshGrid(){ OnUnitsChange((WPARAM)currentNode, (LPARAM)NULL); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PointGridBar)
	public:
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT *pResult );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PointGridBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(PointGridBar)
	afx_msg void OnLfClick3dPointGrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status);
	afx_msg void OnRowChangePointGrid(long Row);
	afx_msg void OnBeforeDeleteRowPointGrid(long Row, long FAR* Status);
	afx_msg void OnChangeNodeGrid(long Row, long Column);
	afx_msg void OnChangePointGrid(long Row, long Column);
	afx_msg void OnChangeCylinderGrid(long Row, long Column);
	afx_msg void OnChangeBoxGrid(long Row, long Column);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg LONG OnCurrentElementChange(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUnitsChange(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnNew3DElement(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLfClickNodeGrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status);
	afx_msg void OnLfClickCylinderGrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status);
	afx_msg void OnLfClickBoxGrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	static int sheetGrids[];
	int currentSheetIndex;
	Ptg3DNode *currentNode;
};

#endif	// #ifndef __PointGridBar_h__

/////////////////////////////////////////////////////////////////////////////
