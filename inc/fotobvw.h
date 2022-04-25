#ifndef __FOTOBVW_H__
#define __FOTOBVW_H__
// fotobvw.h : header file
//
#define ZOOMIN			0x0001
#define ZOOMOUT			0x0002
#define MEASURE			0x0004
#define SELECT			0x0008
#define LB_MODE			0x4000
#define NORMAL			0x8000
#define DEFINEREGION	0x0200
#define DELETEREGION	0x0400
#define LABELREGION		0x0800

/////////////////////////////////////////////////////////////////////////////
// CFotobrowView view
#include "../rvqlite/rvqlite1.hpp"
#include <afxtempl.h>
#include "mainfrm.h"

class CFotobrowView : public CView
{
protected:
	CFotobrowView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFotobrowView)

	image_ptrObj newone;

// Attributes
public:
	int m_scale;
	int m_selected;
//	int m_selectMode;
	//int m_rays;
	CPoint m_sel;
	CPoint m_currPick;
	CStatusBar *m_StatusBar;
    HGLRC m_hglrc;
    HPALETTE m_pal;
	CList<CPoint *, CPoint *> m_pointList;

// Operations
public:

	int CheckSelected() { return m_selected; }
	CPoint GetSelected() { return m_sel; }
	void SetSelected(int flag);
	void SetStatusString(CPoint &p);
	void DrawCross();
	void CenterOnPixel(CPoint &p);
	CPoint DeviceToImage(CPoint &point);
	void ZoomMe(int scale);
	BOOL SelRelPal(BOOL bForceBkgnd);
    void DrawSelect (HDC pDC, RECT rcClip, DWORD dwROP, const char *str);
    void NormalizeRect (LPRECT lpRect);
    void TrackMouse (HWND hWnd, LPRECT lpClipRect, int cxDIB, int cyDIB);
	void DrawBoxes();
	void DrawPoints();
	void setImageName();
	void SetDCPixelFormat (HDC hdc);
	static int m_mode;
	static void SetMode(int flag) { m_mode = flag; }
	static int GetMode() { return m_mode; }
	void UpdateAllViews();
	void updateMyRay(int on_off);
	void cleanMultiSelect();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFotobrowView)
	public:
	virtual void OnFinalRelease();
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFotobrowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFotobrowView)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAccelScrolldown();
	afx_msg void OnAccelScrollleft();
	afx_msg void OnAccelScrollright();
	afx_msg void OnAccelScrollup();
	afx_msg void OnAccelScrollpagedown();
	afx_msg void OnAccelScrollpageleft();
	afx_msg void OnAccelScrollpageright();
	afx_msg void OnAccelScrollpageup();
	afx_msg void OnAccelFittoview();
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelect();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewDefineregion();
	afx_msg void OnViewDeleteregion();
	afx_msg void OnViewLabelregion();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnGraphicsDrawlist();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFotobrowView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__FOTOBVW_H__
