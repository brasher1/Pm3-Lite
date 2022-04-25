#ifndef __FOTOVIEW_H__
#define __FOTOVIEW_H__
// fotoview.h : header file
//
#define ZOOMIN			0x0001
#define ZOOMOUT			0x0002
#define MEASURE			0x0004
#define SELECT			0x0008
#define LB_MODE			0x4000
#define NORMAL			0x8000
#define TRACKING		0x0200

#include <GL/gl.h>
#include <GL/glu.h>

#include "../3DClass/Ptg3DNode.h"
#include "../3DClass/PtgCamera.h"

#include "../triton/triton.hpp"

/////////////////////////////////////////////////////////////////////////////
// CFotoView view

class CFotoView : public CView
{
protected:
	CFotoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFotoView)
	
// Attributes
public:
   HGLRC hglrc;
   HPALETTE m_pal;
   
   CStatusBar *m_StatusBar;
   void SetStatusString(char *s);
   double tmin[3], tmax[3];
      	
   
   ClassesObj 	m_class;
   AllElements 	m_all;
   ElementObj  *m_element;
   LineObj		m_line;
   Ptg3DNode::DrawStyle m_drawstyle;
	
	double clip_values[6];

   GLvoid *m_pBuffer;

   static int m_mode;
   static void SetMode(int flag) { m_mode = flag; }
   static int GetMode() { return m_mode; }
   void getMinMaxCamera(Ptg3DNode *elem);
   unsigned int list_num;
   void regenView();

	int clip_flag;
	int regen_flag;
// Operations
public:
	void DrawColumns();
	BOOL SelRelPal(BOOL bForceBkgnd);
	void SetDCPixelFormat (HDC hdc);
	Ptg3DNode *getPickedElement( PtgCamera *theCamera, Ptg3DNode *theStartNode,
				int nMouseX, int nMouseY );
	void DoOrthoClip(CRect& rect);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFotoView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFotoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFotoView)
	afx_msg void OnCamera();
	afx_msg void OnImage();
	afx_msg void OnTarget();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnSelect();
	afx_msg void OnSetOrtho();
	afx_msg void OnSetPerspective();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetCenter();
	afx_msg void OnClipvolume();
	afx_msg void OnUpdateZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomout(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnGraphicsDrawlist();
	afx_msg void OnViewFillmode();
	afx_msg void OnGraphicsClipzones();
	afx_msg void OnGraphicsFitview();
	afx_msg void OnGraphicsLight();
	afx_msg void OnGraphicsOpenglsettings();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LONG NewClipvolume(WPARAM w, LPARAM dba);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif
