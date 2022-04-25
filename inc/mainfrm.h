// mainfrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
//#define NO_DT_COVER
#include "common.h"
#include "../3dclass/ptgworld.h"
#include "mymdi.h"
#include "createcylinderbyedgedlg.h"
#include "CreateQuickCylinderDlg.h"
#include "circlebyedge.h"
#include "circleby3dpoint.h"
#include "PointGridBar.h"
#include "CreateCylinderByEndpointsDlg.h"
//#include "3dpoints.h"
//#include "3dpointf.h"
//#include "PtgWorld.h"

#include "engineer.h"
#include "maintena.h"

#ifndef __MAINFRM_H__
#define __MAINFRM_H__

#include "gridbar.h"
#include "cylinderdata.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	int m_PointBox;
//	C3DPointsDlg *pts;
//	C3DPointFrame *ptf;
	int m_rays;				// to turn on 3D point rays through bitmaps
	int m_overlay;			// to turn on 3D overlay drawing on bitmaps
	int m_multiSelect;
	int m_selected;

	PtgWorld ptg3DWorld;	// top node for all 3D type view items
	Ptg3DNode *currentElem; // current selected node in the Hierarchy
	Ptg3DNode *measurementRefNode;	// node with which coordinate info is displayed relative to

	CCreateCylinderByEdgeDlg *pCylDlg;
	CCreateQuickCylinderDlg *pQckCylDlg;
	CCircleByEdge *pCirDlg;
	CCircleBy3DPoint *pCir3DDlg;
	CCreateCylinderByEndpointsDlg *pEndPointCyliDlg;

   CDC m_dcMem;            // Compatible Memory DC for dialog
   CBitmap m_bmpABD;    // Bitmap to display
   HBITMAP m_hBmpOld;    // Handle of old bitmap to save

   BITMAP m_bmInfo;        // Bitmap Information structure
   CPoint m_pt;            // Position for upper left corner of bitmap
   CSize m_size;        // Size (width and height) of bitmap

   STARTUPINFO m_st;	 		//Info about the maint system process
   PROCESS_INFORMATION m_pInfo; //Info about the maint system process

   CMapPtrToPtr m_3DtoTree; //Maps 3d elements to the tree view in element list

// Operations
public:
	void UpdateUnits(GridRowData *);	
	Ptg3DNode *getCurrentElem(){return currentElem;}
	PointGridBar *get3DPointGridBar(){ return &m_wnd3DPoints; }

	virtual BOOL CreateClient(LPCREATESTRUCT lpCreateStruct,
					CMenu *pWindowMenu); //MYMDI
	CMyMdi m_MyMdiClientWnd; //MTMDI
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

void RepaintMeasurements();
void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);
void CreateOrActivateFrame(CDocTemplate* pTemplate,	CRuntimeClass* pViewClass);
void UpdateAllViews();
void setSelectMode(int mode);
void sizeTheSheets();
void createTheSheets();

//void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
//	CEditBar	m_wndEditBar;
	CToolBar    m_wndImageToolBar;
	CToolBar    m_wndGraphicsToolBar;
	PointGridBar m_wnd3DPoints;
//	CGridBar	m_wnd3DPoints;
	//CDialogBar  m_wnd3DPoints;
	BOOL 		m_bColor;
	BOOL 		m_bToolTips;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnViewToolbar();
	afx_msg void OnViewGraphic();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMeasure();
	afx_msg void OnUpdateViews(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSettingsOptions();
	afx_msg void OnEditZones();
	afx_msg void OnEditColumns();
	afx_msg void OnEditImages();
	afx_msg void OnEditTransformations();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAccelCopygrid();
	afx_msg void OnEditEquipdata();
	afx_msg void OnEditEquipimages();
	afx_msg void OnFileProjects();
	afx_msg void OnPaint();
	afx_msg void OnUpdateEditEquipdata(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRays();
	afx_msg void OnOverlay();
	afx_msg void OnMeasureCylinderbyedge();
	afx_msg void OnUpdateRays(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOverlay(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnMeasureCirclebyedge();
	afx_msg void OnMeasureCircleby3dpoint();
	afx_msg void OnViewElementlist();
	afx_msg void OnEditBundlecalculations();
	afx_msg void OnLfClick3dpointgrid(short Pane, long Row, long Column, long ClickPos, long FAR* Status);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnFileImportSurveyData();
	afx_msg void OnFileImportReviewModel();
	afx_msg void OnMeasureQuickCylinder();
	afx_msg void OnMeasureCylinderbyEndpoints();
	afx_msg void OnFileSaveWork();
	afx_msg void OnFileGetWork();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg void OnGridLeftClick();
	afx_msg void OnGridMouseMove(UINT flags, CPoint p );
	afx_msg void OnSheetTab();
	afx_msg void OnKillCreateCylinder();
	afx_msg void OnKillCreateCircle();
	afx_msg LONG OnCurrentElementChange(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUnitsChange(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnNewTreeNode(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnNew3DElement(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif //__MAINFRM_H__
/////////////////////////////////////////////////////////////////////////////
