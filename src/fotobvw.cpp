// fotobvw.cpp : implementation file
//
//#define SUPERBUGHUNT

#include "stdafx.h"
#include "fotobrow.h"
#include "fotobvw.h"
#include "fotobdoc.h"
#include "../rvqlite/rvqlite1.hpp"
#include "common.h"
#include "assigneq.h"
#include "mainfrm.h"
#include "drawlist.h"
#include "photog.h"
#include "photog_math.h"
#include <transact.hpp>
#include "lightadjust.h"
#include "openglsettings.h"
#include <float.h>
#include <GL/glaux.h>

#include "../3Dclass/Ptg3DPyramidObj.h"
#include "../3Dclass/Ptg3DLineObj.h"
#include "../3Dclass/PtgCamera.h"
#include "../3Dclass/Ptg3DCylinderObj.h"
#include "../3Dclass/Ptg3DBoxObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))
#define MAX(a,b)     ((a) > (b) ? (a) : (b))
#define MIN(a,b)     ((a) < (b) ? (a) : (b))


int mode_holder;
int CFotobrowView::m_mode = NORMAL;


//PtgCamera theCamera;
//extern Ptg3DNode *theObjects;
//extern Ptg3DNode *theCameraNode;
//extern Ptg3DNode *theRays;
//extern Ptg3DNode *theModel;

static GLfloat latitude=2.0, longitude=3.0, latinc=1.3, longinc=5.0;
static GLfloat twist=4.0, twistinc=2.0;
static GLdouble radius;

#define MASTER	1

static struct ortho {
	double left, right, top, bottom;
} Ortho;

void redrawZoomViewOverlay( CFotobrowView *view, Ptg3DNode *theOverlay );
void updateRay( Ptg3DNode *theNode, char *szName, int bIsSelected, long lXPos, 
			   long lYPos, double reductionFactor );



void qinitElemList()
{
//	if( GETWORLD == NULL )
//	{
//		theObjects = new Ptg3DNode;
//		theObjects->setName("WORLD");
//		theObjects->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
//		theObjects->setActive(TRUE);
//	}

//	theRays = new Ptg3DNode;
//	theRays->setName("RAYS");
//	theRays->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
//	theRays->setActive(TRUE);
//	GETWORLD.addMember(theRays);

//	theModel = new Ptg3DNode;
//	theModel->setName("MODEL");
//	theModel->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
//	theModel->setActive(TRUE);
//	GETOVERLAY->addMember(theModel);
//AfxMessageBox("Inside qinit");
	rdmRecLock( XYZ, "r", TASKER );
	rdmRecLock( POINTS, "r", TASKER );
	struct points pt;
	struct xyz survey;
	int numTargets = 0;
	int status = dt_recfrst( POINTS, TASKER );
	while( status == S_OKAY )
	{
//		AfxMessageBox("Inside While");
		dt_recread( &pt, TASKER );
		if( pt.cType == 'S' || pt.cType == 's' )
		{
			dt_setor( POINTS_SURVEY_XYZ, TASKER );
			dt_findlm( POINTS_SURVEY_XYZ, TASKER );
			dt_recread( &survey, TASKER );
			Ptg3DCylinderObj *pCyl = new Ptg3DCylinderObj;
			pCyl->setName(pt.szLabel);
			pCyl->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
			pCyl->setActive(TRUE);
			pCyl->setHeight(20);
			pCyl->setRadius(5);
			pCyl->setColor( 0.0, 1.0, 0.5 );
			double array[12] = {
				1.0,	0.0,	 0.0,
				0.0,	0.0,	-1.0,
				0.0,	1.0,	 0.0,
				0.0,	0.0,	 0.0 };

			array[9] = survey.dfCoor[0];
			array[10] = survey.dfCoor[1];
			array[11] = survey.dfCoor[2];
			pCyl->setMatrix(array-1);
			GETTARGETS->addMember(pCyl);
//			MessageBeep(MB_OK);
			dt_setro( POINTS_SURVEY_XYZ, TASKER );

			numTargets++;
		}
		status = dt_recnext( TASKER );
		
	} 

	TRACE( "added %d targets\n", numTargets );

	rdmRecFree(XYZ, TASKER );
	rdmRecFree(POINTS, TASKER);

	// Create node to represent the calculated coordinate system
	Ptg3DNode *measureXform = new Ptg3DNode;
	measureXform->setName( "Measurement Reference Node" );

	VECTOR v1 = ((CFotobrowApp *)AfxGetApp())->m_Transform - 1;
	VECTOR v2 = vector( 1, 12 );
	v2[1] = 1.0;	v2[2] = 0.0;	v2[3] = 0.0;
	v2[4] = 0.0;	v2[5] = 0.0;	v2[6] = 1.0;
	v2[7] = 0.0;	v2[8] = -1.0;	v2[9] = 0.0;
	v2[10] = 0.0;	v2[11] = 0.0;	v2[12] = 0.0;

//	VECTOR v3 = matmat( v1, v2 );
	VECTOR v4 = matinv( v1 );
//	VECTOR v4 = vector( 1, 12 );

//	v4[1] = v1[1];	v4[2] = v1[4];	v4[3] = v1[7];
//	v4[4] = v1[2];	v4[5] = v1[5];	v4[6] = v1[8];
//	v4[7] = v1[3];	v4[8] = v1[6];	v4[9] = v1[9];

//	v4[10] = v1[10];
//	v4[11] = v1[11];
//	v4[12] = v1[12];

	measureXform->setMatrix( v4 );
	free_vector( v2, 1, 12 );
//	free_vector( v3, 1, 12 );
	free_vector( v4, 1, 12 );

#if FALSE
	VECTOR v = matinv( ((CFotobrowApp *)AfxGetApp())->m_Transform - 1 );
	VECTOR v1 = vector( 1, 12 );
	v1[1] = 1.0;	v1[2] = 0.0;	v1[3] = 0.0;
	v1[4] = 0.0;	v1[5] = 0.0;	v1[6] = 1.0;
	v1[7] = 0.0;	v1[8] = -1.0;	v1[9] = 0.0;
	v1[10] = 0.0;	v1[11] = 0.0;	v1[12] = 0.0;
	VECTOR v2 = matmat( v1, v );
	measureXform->setMatrix(v2);
	free_vector( v, 1, 12 );
	free_vector( v1, 1, 12 );
	free_vector( v2, 1, 12 );
#endif

	GETWORLD.addMember( measureXform  );
	((CFotobrowApp *)AfxGetApp())->SetReferenceNode(measureXform);

}

/////////////////////////////////////////////////////////////////////////////
// CFotobrowView
#define STD_STR			"NewEquip"

IMPLEMENT_DYNCREATE(CFotobrowView, CView)

static UINT BASED_CODE image_indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};

CFotobrowView::CFotobrowView()
{
	EnableAutomation();
//this is a 	
//#ifdef SUPERBUGHUNT	
//	if( theObjects == NULL )
//		qinitElemList();
//#endif // SUPERBUGHUNT	
//	memset(0,0,100000000);
	m_scale = 1;
	m_StatusBar = NULL;
	m_selected = FALSE;
	m_mode = NORMAL;
//	m_rays = FALSE;
	
	if( rvqTask != NULL )
		return;

	rvqTask = new rvqLiteTask;
	rvqTask->MakeCurrent();
	CString temp = ((CFotobrowApp *)AfxGetApp())->m_DatabasePath + "rvq";
//	AfxMessageBox(temp);
	rvqTask->SetPaths((char *)(const char *)temp,
					  (char *)(const char *)temp,
					  NULL, NULL );
	if( !rvqTask->openDatabase("rvqlite") )
	{
		AfxMessageBox("Error opening database");
		delete rvqTask;
		rvqTask = NULL;
	}
}

CFotobrowView::~CFotobrowView()
{
//	updateRay( theObjects, (char *)(const char *)(((CFotobrowDoc *)GetDocument())->szName),
//			FALSE, m_sel.x, m_sel.y, 	
//			((CFotobrowApp*)AfxGetApp())->m_imageReduction );

	if( m_StatusBar )
		delete m_StatusBar;
	CObList list;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(&list);

	if( m_pointList.GetCount()	> 1 )
	{
		while( m_pointList.GetCount() )
			m_pointList.RemoveTail();
	}

	if( list.GetCount()	> 1 )
	{
		while( list.GetCount() )
			list.RemoveTail();
		return;
	}
#ifdef SUPERBUGHUNT	
	delete theObjects;
	theObjects=NULL;
#endif// SUPERBUGHUNT	
	while( list.GetCount() )
		list.RemoveTail();
	if( rvqTask )
	{
		delete rvqTask;
		rvqTask=NULL;
	}
}

void CFotobrowView::OnFinalRelease()
{
	// When the last reference for an automation object is released
	//	OnFinalRelease is called.  This implementation deletes the 
	//	object.  Add additional cleanup required for your object before
	//	deleting it from memory.

	delete this;
}


BEGIN_MESSAGE_MAP(CFotobrowView, CView)
	//{{AFX_MSG_MAP(CFotobrowView)
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(IDC_ZOOMIN, OnZoomin)
	ON_COMMAND(IDC_ZOOMOUT, OnZoomout)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_ACCEL_SCROLLDOWN, OnAccelScrolldown)
	ON_COMMAND(ID_ACCEL_SCROLLLEFT, OnAccelScrollleft)
	ON_COMMAND(ID_ACCEL_SCROLLRIGHT, OnAccelScrollright)
	ON_COMMAND(ID_ACCEL_SCROLLUP, OnAccelScrollup)
	ON_COMMAND(ID_ACCEL_SCROLLPAGEDOWN, OnAccelScrollpagedown)
	ON_COMMAND(ID_ACCEL_SCROLLPAGELEFT, OnAccelScrollpageleft)
	ON_COMMAND(ID_ACCEL_SCROLLPAGERIGHT, OnAccelScrollpageright)
	ON_COMMAND(ID_ACCEL_SCROLLPAGEUP, OnAccelScrollpageup)
	ON_COMMAND(ID_ACCEL_FITTOVIEW, OnAccelFittoview)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDC_SELECT, OnSelect)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_VIEW_DEFINEREGION, OnViewDefineregion)
	ON_COMMAND(ID_VIEW_DELETEREGION, OnViewDeleteregion)
	ON_COMMAND(ID_VIEW_LABELREGION, OnViewLabelregion)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	ON_WM_TIMER()
	ON_COMMAND(ID_GRAPHICS_DRAWLIST, OnGraphicsDrawlist)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFotobrowView, CView)
	//{{AFX_DISPATCH_MAP(CFotobrowView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFotobrowView drawing

void CFotobrowView::OnDraw(CDC* pDC)
{
	CFotobrowDoc* pDoc = (CFotobrowDoc* )GetDocument();
	CDIB *dib = pDoc->GetDIB();
	TRACE("OnDraw\n");
	CPoint scroll;
	CRect r;
	GetClientRect(&r);
//#ifndef NO_PALETTE_JUNK
	CPalette *pOldPal = pDC->SelectPalette(pDoc->GetPalette(),TRUE);
	pDC->RealizePalette();
//#endif
//#define NO_BITMAP_DRAWING
#ifndef NO_BITMAP_DRAWING
	CBrush fillBrush;
	fillBrush.CreateHatchBrush(HS_BDIAGONAL, RGB(0,255,0));
	CBrush *oldBrush = pDC->SelectObject(&fillBrush);
	if( (int)(r.Width()/m_scale)*m_scale < r.Width() || 
					(int)(r.Height()/m_scale)*m_scale < r.Height() )
	{
		::PatBlt(pDC->GetSafeHdc(), 0, (r.Height()/m_scale)*m_scale, r.Width(), r.Height(), PATCOPY );
		::PatBlt(pDC->GetSafeHdc(), (int)(r.Width()/m_scale)*m_scale, 0, r.Width(), r.Height(), PATCOPY );
	}
	if( m_currPick.x - r.Width()/(2*m_scale) < 0 )
	{
		::PatBlt(pDC->GetSafeHdc(), 0,0, -(m_currPick.x*m_scale - r.Width()/(2)),r.Height(), PATCOPY );
	}
	if( m_currPick.y - r.Height()/(2*m_scale) < 0 )
	{
		::PatBlt(pDC->GetSafeHdc(), 0,0, r.Width(), -(m_currPick.y*m_scale - r.Height()/(2)), PATCOPY );
	}

	if( m_currPick.x + r.Width()/(2*m_scale) > dib->GetWidth() )
	{
		::PatBlt(pDC->GetSafeHdc(), (r.Width()/2)+(dib->GetWidth()-m_currPick.x)*m_scale,0, r.Width(), r.Height(), PATCOPY );
	}
	if( m_currPick.y + r.Height()/(2*m_scale) > dib->GetHeight() )
	{
		::PatBlt(pDC->GetSafeHdc(), 0, (r.Height()/2)+(dib->GetHeight()-m_currPick.y)*m_scale, r.Width(), r.Height(), PATCOPY );
	} 
	pDC->SelectObject(oldBrush);

	::StretchDIBits( pDC->GetSafeHdc(),
		0,0,
		(int)(r.Width()/m_scale)*m_scale,
		(int)(r.Height()/m_scale)*m_scale,
		m_currPick.x - r.Width()/(2*m_scale),
		dib->GetHeight() - m_currPick.y - r.Height()/(2*m_scale),
		r.Width()/m_scale,
		r.Height()/m_scale,
		dib->GetBitsAddress(),
		dib->GetBitmapInfoAddress(),
		DIB_RGB_COLORS,
		SRCCOPY);
	DrawCross();
	DrawPoints();

	if( m_scale == 1 )
		DrawBoxes();	
#endif //NO_BITMAP_DRAWING
	if( ((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_rays ||
		((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_overlay )
	{
		TRACE("=========Drawing OpenGL===========\n");
		HWND hwnd = GetSafeHwnd();
		HDC hdc = ::GetDC(hwnd);
		wglMakeCurrent(hdc, m_hglrc );
		TRACE("Make Current\n");
		glClearColor(1.0,1.0,1.0,0.0);
		glClear( GL_DEPTH_BUFFER_BIT );
		int val[4];//glViewport
		glGetIntegerv(GL_VIEWPORT, val);
		TRACE("The Viewport is %d, %d, %d, %d\n", val[0], val[1], val[2], val[3] );  
	
		TRACE("gl Cleared Things\n");

		if( ((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_rays )
			redrawZoomViewOverlay( this, GETRAYS );
		if( ((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_overlay )
			redrawZoomViewOverlay( this, &GETWORLD );

/*
		if( ((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_rays )
			redrawZoomViewOverlay( this, GETRAYS );
		if( ((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_overlay )
		{
			redrawZoomViewOverlay( this, GETSYSTEM );
			redrawZoomViewOverlay( this, GETMODEL );
			redrawZoomViewOverlay( this, GETREVIEW );
		}
*/
		wglMakeCurrent(NULL,NULL);
		::ReleaseDC(GetSafeHwnd(),hdc);
		//GL FIX
		TRACE("=====Finished Drawing OpenGL======\n");
	}
	//#ifndef NO_PALETTE_JUNK
	pDC->SelectPalette(pOldPal, FALSE);
//#endif
}

/////////////////////////////////////////////////////////////////////////////
// CFotobrowView diagnostics

#ifdef _DEBUG
void CFotobrowView::AssertValid() const
{
	CView::AssertValid();
}

void CFotobrowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFotobrowView message handlers

void CFotobrowView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPoint p = DeviceToImage(point);
	if( GetMode() == ZOOMOUT )
		ZoomMe(m_scale - 1);
	else if( GetMode() == ZOOMIN )
		ZoomMe(m_scale + 1);
	CenterOnPixel(p);											   
	SetStatusString(point);
	CView::OnLButtonUp(nFlags, point);
}

void CFotobrowView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint curr(m_currPick.x,m_currPick.y);
//	int temp;
	CFotobrowDoc *pDoc = (CFotobrowDoc *)GetDocument();
	CDIB *dib = pDoc->GetDIB();
	
	switch( nSBCode )
	{				
		case SB_BOTTOM://    Scroll to far left.
			curr.y = 0;
			break;
		case SB_ENDSCROLL://    End scroll.
			return;
			break;
		case SB_LINEUP://    Scroll left.
			curr.y = m_currPick.y - 1;
			break;
		case SB_LINEDOWN://    Scroll right.
			curr.y = m_currPick.y + 1;
			break;
		case SB_PAGEUP://    Scroll one page left.
			curr.y = m_currPick.y - 20;
			break;
		case SB_PAGEDOWN://    Scroll one page right.
			curr.y = m_currPick.y + 20;
			break;
		case SB_TOP://    Scroll to far right.
			curr.y = dib->GetHeight();
			break;
		case SB_THUMBPOSITION://    Scroll to absolute position. The current position is specified by the nPos 
		case SB_THUMBTRACK://    Drag scroll box to specified position. The current position is specified by 
			curr.y = nPos;
			break;
	}
	CenterOnPixel(curr);
	
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFotobrowView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint curr(m_currPick.x,m_currPick.y);
//	int temp;
	CFotobrowDoc *pDoc = (CFotobrowDoc *)GetDocument();
	CDIB *dib = pDoc->GetDIB();
	
	switch( nSBCode )
	{				
		case SB_LEFT://    Scroll to far left.
			curr.x = 0;
			break;
		case SB_ENDSCROLL://    End scroll.
			return;
//			curr.x = dib->GetWidth();
			break;
		case SB_LINELEFT://    Scroll left.
			curr.x = m_currPick.x - 1;
			break;
		case SB_LINERIGHT://    Scroll right.
			curr.x = m_currPick.x + 1;
			break;
		case SB_PAGELEFT://    Scroll one page left.
			curr.x = m_currPick.x - 20;
			break;
		case SB_PAGERIGHT://    Scroll one page right.
			curr.x = m_currPick.x + 20;
			break;
		case SB_RIGHT://    Scroll to far right.
			curr.x = dib->GetWidth();
			break;
		case SB_THUMBPOSITION://    Scroll to absolute position. The current position is specified by the nPos 
		case SB_THUMBTRACK://    Drag scroll box to specified position. The current position is specified by 
			curr.x = nPos;
			break;
	}
	CenterOnPixel(curr);
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CFotobrowView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::OnScroll(nScrollCode, nPos, bDoScroll);
}

BOOL CFotobrowView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::OnScrollBy(sizeScroll, bDoScroll);
}

void CFotobrowView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
//	CDC *dc = GetDC();
//	ASSERT(dc);
	TRACE("OnSize\nSize Message: %u,  cx=%d, cy=%d\n", nType, cx, cy);

	HWND hwnd = GetSafeHwnd();
	HDC hdc = ::GetDC(hwnd);
	wglMakeCurrent(hdc, m_hglrc );
	GLenum err;
	while( (err = glGetError()) != GL_NO_ERROR )
		TRACE("Before Viewport GLerror = %d\n", err);
//glgetError

	glViewport(0,0,cx,cy);
	while( (err = glGetError()) != GL_NO_ERROR )
		TRACE("After Viewport GLerror = %d\n", err);
	
	//	GLfloat aspect = (GLfloat)cx/cy;
//	theCamera.setAspectRatio(aspect);
	//GL FIX
	wglMakeCurrent(NULL,NULL);
//	ReleaseDC(dc);
	::ReleaseDC(GetSafeHwnd(),hdc);
	// TODO: Add your message handler code here
	
}

void CFotobrowView::OnInitialUpdate() 
{
	// TODO: Add your specialized code here and/or call the base class
	CFotobrowDoc *pDoc = (CFotobrowDoc *)GetDocument();
	ASSERT(pDoc);
	CSize sz;
	sz.cx = pDoc->GetDIB()->GetWidth() * m_scale;
	sz.cy = pDoc->GetDIB()->GetHeight() * m_scale;
	SetScrollRange(SB_HORZ, 0, sz.cx, TRUE);
	SetScrollRange(SB_VERT, 0, sz.cy, TRUE);
//	ResizeParentToFit();
	CPoint temp;
	temp.x = sz.cx / 2;
	temp.y = sz.cy / 2;
	CenterOnPixel(temp);
	SetStatusString(temp);
	CView::OnInitialUpdate();

//	CDocument *pDoc = GetDocument();
	char temp1[128];
	CString test = pDoc->GetPathName();
	sprintf(temp1, "name - %s", (const char *)test );
	char *p;
	if( p = strrchr( test, '\\' ) )
		strcpy( temp1, strrchr( test, '\\' )+1 );
	else
		strcpy( temp1, test);
	CString temp2(temp1);
	temp2.MakeLower();

	//AfxMessageBox((const char *)temp2);
}

void CFotobrowView::setImageName()
{
	CFotobrowDoc *pDoc = (CFotobrowDoc *)GetDocument();
	ASSERT(pDoc);

	rdmRecLock( IMAGES, "r", TASKER );
	if( dt_keyfind( IMAGES_SZNAME, (const char *)pDoc->szName, TASKER ) == S_OKAY )
	{
	//	AfxMessageBox("Got the image");
		rvqTask->MakeCurrent();
		long dba2;
		dt_crget( (unsigned long *)&dba2, TASKER );
		newone.ReadLock();
		newone.Find( KeyObj( IMAGE_PTR_DBA, (Pvoid)&dba2 ));
		if( !newone.Okay() )
		{
			TransAction a;
			newone.image_ptr::dba = dba2;
			a.Begin("wow");
			newone.WriteLock();
			newone.NewObj();
			a.End();
		}
		else
		{
		//	AfxMessageBox("Found IT");
			newone.FreeLock();
		}
		char temp[128];
		sprintf(temp,"dba=%lu", dba2 );
//		AfxMessageBox(temp);
	}
	else
	{
		AfxMessageBox("Couldn't find Image");
	}
	rdmRecFree( IMAGES, TASKER );
}

void CFotobrowView::SetStatusString(CPoint &point) 
{
	RECT rect;
	// TODO: Add your message handler code here and/or call default
	char temp1[128];		//	CDocument
	GetClientRect(&rect);
//	CPoint p = GetScrollPos();
//////
    CFotobrowDoc* pDoc = (CFotobrowDoc *)GetDocument();
	if( pDoc == FALSE )
		return;
	ASSERT(pDoc);
	CDIB *dib = pDoc->GetDIB();
//////
	CPoint pos = DeviceToImage(point);
	
	sprintf(temp1,"Position:%04d,%04d    Selected:%04d,%04d   Zoom: %d", 
			pos.x, dib->GetHeight() - pos.y, 
			(int)m_sel.x, 
			(int)m_sel.y, 
			m_scale );
	m_StatusBar->SetPaneInfo(0,0,SBPS_STRETCH, (strlen(temp1)+1)*4);
	m_StatusBar->SetPaneText(0,(char *)temp1);
}

int CFotobrowView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	TRACE("Pre OnCreate\n");								 
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
/////////////////////////////////////////////////////////////////////////
	TRACE("OnCreate After If\n");								 
	HWND hwnd = GetSafeHwnd();
	HDC hdc = ::GetDC(hwnd);
	SetDCPixelFormat(hdc);
	m_hglrc = wglCreateContext(hdc );
	wglMakeCurrent(hdc, m_hglrc );
//	SetTimer(1,2000,NULL);
	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
//	glEnable(GL_AUTO_NORMAL);
//	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 2);
//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT );
	GLfloat v[4] = { 0.45, 0.45, 0.45, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
///
//	glEnable(GL_BLEND);
 //	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//AfxMessageBox("OnCreate 2");
/////////////////////////////////////////////////////////////////////////
	m_StatusBar = new CStatusBar;
	if (!m_StatusBar->Create(GetParent()) ||
		!m_StatusBar->SetIndicators(image_indicators,
		  sizeof(image_indicators)/sizeof(UINT)))
	{
		AfxMessageBox("Failed to create status bar");
		return -1;      // fail to create
	}
	//GL FIX
	wglMakeCurrent(NULL,NULL);
	::ReleaseDC(GetSafeHwnd(),hdc);
	return 0;
}

void CFotobrowView::DrawCross()
{
	CDC *pDC = GetDC();
	CRect client;
//	if( m_scale == 1 )
//	{
//		DrawBullseye(m_sel);
//		return;
//	}
	GetClientRect(&client);
	int wxsize = client.right - client.left;
	int wysize = client.bottom - client.top;
	int nx = ((int)((wxsize / 2)/m_scale)/*+1*/)*m_scale;
	int ny = ((int)((wysize / 2)/m_scale)/*+1*/)*m_scale;
	CBrush brush;
	if( m_selected )
		brush.CreateSolidBrush(RGB(255,0,0));
	else
		brush.CreateSolidBrush(RGB(255,255,255));
	CBrush *old_brush = pDC->SelectObject(&brush);
	CRect cross(nx,0, nx+m_scale, wysize-1);
	pDC->FrameRect(cross,&brush);
	cross.SetRect(0,ny, wxsize-1, ny+m_scale);
	pDC->FrameRect(cross,&brush);
	if( m_selected )
		pDC->Rectangle(nx,ny,nx+m_scale, ny+m_scale);
	pDC->SelectObject(old_brush);
	ReleaseDC(pDC);
}

void CFotobrowView::CenterOnPixel(CPoint &p)
{
	CFotobrowDoc *pDoc = (CFotobrowDoc *)GetDocument();
	CDIB *dib = pDoc->GetDIB();

	if( p.x < 0 )
		p.x = 0;
	else if( p.x > dib->GetWidth()-1 )
		p.x = dib->GetWidth()-1;
	if( p.y < 0 )
		p.y = 0;
	else if( p.y > dib->GetHeight()-1 )
		p.y = dib->GetHeight()-1;

	m_currPick.x = p.x;
	m_currPick.y = p.y;
	m_sel.x = m_currPick.x;
	m_sel.y = dib->GetHeight() - p.y;
	SetScrollPos(SB_VERT, m_currPick.y);
	SetScrollPos(SB_HORZ, m_currPick.x);
	
	if( ((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_rays )
	{
//#ifdef SUPERBUGHUNT	
			updateRay( GETRAYS, (char *)(const char *)(((CFotobrowDoc *)GetDocument())->szName),
				m_selected, m_sel.x, m_sel.y, 	
				((CFotobrowApp*)AfxGetApp())->m_imageReduction );
//#endif// SUPERBUGHUNT	
		UpdateAllViews();
	}
	InvalidateRect(NULL, FALSE);
}

CPoint CFotobrowView::DeviceToImage(CPoint &point)
{
	CPoint temp;
	CRect r;

	GetClientRect(&r);
	temp.x = m_currPick.x + (point.x - r.Width()/2)/m_scale;
	temp.y = m_currPick.y + (point.y - r.Height()/2)/m_scale;

	return temp;
}

void CFotobrowView::OnZoomin() 
{
	// TODO: Add your command handler code here
	SetMode(ZOOMIN);
}

void CFotobrowView::OnZoomout() 
{
	// TODO: Add your command handler code here
	SetMode(ZOOMOUT);	
}

void CFotobrowView::ZoomMe(int scale) 
{
	if( scale < 1 )
		return;
	m_scale = scale;
	InvalidateRect(NULL, FALSE);	
}

void CFotobrowView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if( !((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_multiSelect )
	{
		m_selected = !m_selected;
		DrawCross();
		if( ((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_rays )
		{
			updateRay( GETRAYS, (char *)(const char *)(((CFotobrowDoc *)GetDocument())->szName),
				m_selected, m_sel.x, m_sel.y, 	
				((CFotobrowApp*)AfxGetApp())->m_imageReduction );
			UpdateAllViews();
		}
	}
	else
	{//CList CPoint
		m_pointList.AddTail(new CPoint(m_sel.x, m_sel.y));
		DrawPoints();
//		InvalidateRect(NULL,TRUE);
	}
	CView::OnRButtonUp(nFlags, point);
}

void CFotobrowView::OnAccelScrolldown() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_VSCROLL, SB_LINEDOWN, 0L );	
}

void CFotobrowView::OnAccelScrollleft() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_HSCROLL, SB_LINELEFT, 0L );	
}

void CFotobrowView::OnAccelScrollright() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_HSCROLL, SB_LINERIGHT, 0L );	
}

void CFotobrowView::OnAccelScrollup() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_VSCROLL, SB_LINEUP, 0L );	
}

void CFotobrowView::OnAccelScrollpagedown() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0L );	
}

void CFotobrowView::OnAccelScrollpageleft() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_HSCROLL, SB_PAGELEFT, 0L );	
}

void CFotobrowView::OnAccelScrollpageright() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_HSCROLL, SB_PAGERIGHT, 0L );	
}

void CFotobrowView::OnAccelScrollpageup() 
{
	// TODO: Add your command handler code here
	PostMessage(WM_VSCROLL, SB_PAGEUP, 0L );	
}

void CFotobrowView::OnAccelFittoview() 
{
	// TODO: Add your command handler code here
	CDC *pDC = GetDC();
	CFotobrowDoc* pDoc = (CFotobrowDoc* )GetDocument();
	CDIB *dib = pDoc->GetDIB();
	CRect r;
	GetClientRect(&r);
#ifndef NO_PALETTE_JUNK
	CPalette *pOldPal = pDC->SelectPalette(pDoc->GetPalette(),TRUE);
	pDC->RealizePalette();
#endif
	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	::StretchDIBits( pDC->GetSafeHdc(),
		0,0,
		r.Width(),
		r.Height(),
		0,0,
		dib->GetWidth(),
		dib->GetHeight(),
		dib->GetBitsAddress(),
		dib->GetBitmapInfoAddress(),
		DIB_RGB_COLORS,
		SRCCOPY);
#ifndef NO_PALETTE_JUNK
	pDC->SelectPalette(pOldPal, FALSE);
	
#endif
	ReleaseDC(pDC);
}

BOOL CFotobrowView::OnQueryNewPalette() 
{
//#ifndef NO_PALETTE_JUNK
    return SelRelPal(FALSE); // Realize in the foreground.
//#else
	return TRUE;
//#endif
//	
//	return CView::OnQueryNewPalette();
}

void CFotobrowView::OnPaletteChanged(CWnd* pFocusWnd) 
{
//	CView::OnPaletteChanged(pFocusWnd);
	
	// TODO: Add your message handler code here
    // See if the change was caused by us and ignore it if not.
//#ifndef NO_PALETTE_JUNK
    if (pFocusWnd != this) {
        SelRelPal(TRUE); // Realize in the background. 
    }
//#endif	
}

// Select and realise our palette.
BOOL CFotobrowView::SelRelPal(BOOL bForceBkgnd)
{
#ifndef NO_PALETTE_JUNK
    // We are going active, so realize our palette.
    CDC* pdc = GetDC();
    CFotobrowDoc *pDoc = (CFotobrowDoc * )GetDocument();
//	if( pDoc == NULL )
//		return FALSE;
    CPalette *poldpal = pdc->SelectPalette(pDoc->GetPalette(),
                                           bForceBkgnd);
	if( poldpal == NULL )
	{
		ReleaseDC(pdc);
		return FALSE;
	}
    UINT u = pdc->RealizePalette();
    pdc->SelectPalette(poldpal, TRUE);
    pdc->RealizePalette();
    ReleaseDC(pdc);
    // If any colors have changed or we are in the
    // background, repaint the lot.
    if (u || bForceBkgnd) {
        InvalidateRect(NULL, TRUE); // Repaint.
    }
    return (BOOL) u; // TRUE if some colors changed.
#else
	return TRUE;
#endif
}

void CFotobrowView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
#ifndef NO_PALETTE_JUNK
    if (bActivate) {
        // We are going active so realize our palette in
        // the foreground and tell the frame to notify
        // all other windows of the change.
        SelRelPal(FALSE);
        CWnd* pMainFrame = AfxGetApp()->m_pMainWnd;
        pMainFrame->SendMessage(WM_PALETTECHANGED,
                                (WPARAM)GetSafeHwnd(),
                                0);
    }
#endif
//	
//	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CFotobrowView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetStatusString(point);	
	CView::OnMouseMove(nFlags, point);
}

void CFotobrowView::OnSelect() 
{
	// TODO: Add your command handler code here
	SetMode(SELECT);	
}

//---------------------------------------------------------------------
//
// Function:   TrackMouse
//
// Purpose:    This routine is called when the left mouse button is
//             held down.  It will continuously draw a rectangle
//             showing where the user has selected for cutting to the
//             clipboard.  When this routine is called, lpClipRect's
//             top/left should point at the point in the client area
//             where the left mouse button was hit.  It will return the
//             full sized rectangle the user selected.  Never allow the
//             rubber band to extend beyond the DIB's margins.
//
//             Code was stolen almost verbatim from ShowDIB.
//
// Parms:      hWnd       == Handle to this MDI child window.
//             lpClipRect == Rectangle enclosed by tracking box.
//             cxDIB      == Width of DIB.  Won't allow tracking box
//                             to go beyond the width.
//             cyDIB      == Height of DIB.  Won't allow tracking box
//                             to go beyond the height.
//
// History:   Date      Reason
//             ????     Created
//             9/1/91   Added cxDIB/cyDIB to not allow garbage
//                        to be pasted to the clipboard.
//
//---------------------------------------------------------------------

void CFotobrowView::TrackMouse (HWND hWnd, LPRECT lpClipRect, int cxDIB, int cyDIB)
{
//   CDC * pDC;
   MSG   msg;
   POINT ptOrigin, ptStart;
   RECT  client;
   HDC hDC;
   HBRUSH  hBrush;
   HBRUSH  hOldBrush;

//   pDC = GetDC();
	hDC = ::GetDC(GetSafeHwnd());
  // SetCapture (hWnd);
   GetClientRect (&client);
	CRect rcClient(&client);

      // Get mouse coordinates relative to origin of DIB.  Then
      //  setup the clip rectangle accordingly (it already should
      //  contain the starting point in its top/left).

   ptOrigin.x         = GetScrollPos (SB_HORZ);
   ptOrigin.y         = GetScrollPos (SB_VERT);
         	
   lpClipRect->top   += ptOrigin.y;
   lpClipRect->left  += ptOrigin.x;
   lpClipRect->right  = lpClipRect->left;
   lpClipRect->bottom = lpClipRect->top;
   ptStart.x          = lpClipRect->left;    // Need to remember the
   ptStart.y          = lpClipRect->top;     //  starting point.

      // Display the starting coordinates.

//   pDC->SetWindowOrg (ptOrigin.x, ptOrigin.y);
   ::SetWindowOrgEx (hDC, ptOrigin.x, ptOrigin.y, NULL );
   
   DrawSelect (hDC, *lpClipRect, DSTINVERT, STD_STR);


    // Eat mouse messages until a WM_LBUTTONUP is encountered. Meanwhile
    // continue to draw a rubberbanding rectangle and display it's dimensions

   for (;;)
      {
      WaitMessage ();

      if (PeekMessage (&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
         {
            // Erase the old.

         DrawSelect (hDC, *lpClipRect, DSTINVERT , STD_STR );


            // Determine new coordinates.

         lpClipRect->left   = ptStart.x;
         lpClipRect->top    = ptStart.y;
         lpClipRect->right  = LOWORD (msg.lParam) + ptOrigin.x;
         lpClipRect->bottom = HIWORD (msg.lParam) + ptOrigin.y;
         NormalizeRect (lpClipRect);


            // Keep the rectangle within the bounds of the DIB.

         lpClipRect->left   = MAX(lpClipRect->left,   (rcClient.Width()/(2*m_scale)));
         lpClipRect->top    = MAX(lpClipRect->top,    (rcClient.Height()/(2*m_scale)));
         lpClipRect->right  = MAX(lpClipRect->right,  (rcClient.Width()/(2*m_scale)));
         lpClipRect->bottom = MAX(lpClipRect->bottom, (rcClient.Height()/(2*m_scale)));

         lpClipRect->left   = MIN(lpClipRect->left,   cxDIB+(rcClient.Width()/(2*m_scale)));
         lpClipRect->top    = MIN(lpClipRect->top,    cyDIB+(rcClient.Height()/(2*m_scale)));
         lpClipRect->right  = MIN(lpClipRect->right,  cxDIB+(rcClient.Width()/(2*m_scale)));
         lpClipRect->bottom = MIN(lpClipRect->bottom, cyDIB+(rcClient.Height()/(2*m_scale)));



            // Draw the new rectangle.

         DrawSelect (hDC, *lpClipRect, DSTINVERT , STD_STR );


            // If the button is released, quit.

         if (msg.message == WM_LBUTTONUP)
		 {
			DrawSelect( hDC, *lpClipRect , DSTINVERT , STD_STR);
			hBrush = ::CreateSolidBrush(	RGB( 0,255,0) );
			hOldBrush = (HBRUSH)::SelectObject( hDC, hBrush );
			DrawSelect( hDC, *lpClipRect , PATCOPY , STD_STR);
		    ::SelectObject( hDC, hOldBrush );
		    ::DeleteObject( hBrush );
			char temp[128];
			CRect r(*lpClipRect);
			
			r.left -= rcClient.Width()/(2*m_scale);
			r.right -= rcClient.Width()/(2*m_scale);
			r.top = cyDIB - r.top + rcClient.Height()/(2*m_scale);
			r.bottom = cyDIB - r.bottom + rcClient.Height()/(2*m_scale);

			sprintf(temp, "x1=%d, y1=%d .... x2=%d, y2=%d", r.left, r.top,
					r.right, r.bottom );

			CAssignEquipDlg equip;
			long dba = newone.GetDba();
			equip.SetImageData(r, dba);
			equip.DoModal();
						
//			AfxMessageBox(temp);
			SetMode(mode_holder);
			InvalidateRect(NULL, TRUE);
         	break;
		 }
        }
      else
         continue;
      }


      // Clean up.

//   ReleaseCapture ();
   ::ReleaseDC(GetSafeHwnd(), hDC);
}




//---------------------------------------------------------------------
//
// Function:   NormalizeRect
//
// Purpose:    Insure that the upper/left corner of the rectangle is
//             kept in rect.top/rect.right.  Swaps around coordinates
//             in the rectangle to make sure this is true.
//
//             Code was stolen verbatim from ShowDIB.
//
// Parms:      lpRect == Far pointer to RECT to normalize.
//
// History:   Date      Reason
//             ????     Created
//
//---------------------------------------------------------------------

void CFotobrowView::NormalizeRect (LPRECT lpRect)
{
    if (lpRect->right < lpRect->left)
        SWAP (lpRect->right,lpRect->left);

    if (lpRect->bottom < lpRect->top)
        SWAP (lpRect->bottom,lpRect->top);

}



//---------------------------------------------------------------------
//
// Function:   DrawSelect
//
// Purpose:    Draw the rubberbanding rectangle with the specified
//             dimensions on the specified DC.  Rectangle includes
//             a string with its dimensions centered within it.
//
//             Code was stolen almost verbatim from ShowDIB.
//
// Parms:      hDC    == DC to draw into.
//             rcClip == Rectangle to draw.
//
// History:   Date      Reason
//             ????     Created
//
//---------------------------------------------------------------------

void CFotobrowView::DrawSelect (HDC hDC, RECT rcClip, DWORD dwROP , const char *string)
{
   char    szStr[80];
//   DWORD   dwExt;
   SIZE dwExt;
   CSize   ext;
   int     x, y, nLen, dx, dy;
//   CDC *   pDCBits;
   HDC     hDCBits;
   HBITMAP hBitmap;
   
      // Don't have anything to do if the rectangle is empty.

   if (IsRectEmpty (&rcClip))
      return;
      // Draw rectangular clip region

   ::PatBlt (hDC,
   			rcClip.left,
            rcClip.top,
            rcClip.right - rcClip.left,
            2,
            dwROP);

   ::PatBlt (hDC,
   			rcClip.left,
            rcClip.bottom,
            2,
            -(rcClip.bottom - rcClip.top),
            dwROP);

   ::PatBlt (hDC,
   			rcClip.right - 1,
            rcClip.top,
            2,
            rcClip.bottom - rcClip.top,
            dwROP);

   ::PatBlt (hDC,
   			rcClip.right,
            rcClip.bottom - 1,
            -(rcClip.right - rcClip.left),
            2,
            dwROP);


      // Format the dimensions string ...

//   wsprintf (szStr,
//   	         "%dx%d",
//               rcClip.right - rcClip.left,
//   	         rcClip.bottom - rcClip.top );
	wsprintf ( szStr, "New Object");
               nLen = lstrlen(string);


      // ... and center it in the rectangle

   GetTextExtentPoint32 (hDC, string, nLen, &dwExt );
  // ext     = pDC->GetTextExtent (szStr, nLen);   
   dx      = dwExt.cx;
   dy      = dwExt.cy;
   x       = (rcClip.right  + rcClip.left - dx) / 2;
   y       = (rcClip.bottom + rcClip.top  - dy) / 2;
   hDCBits = ::CreateCompatibleDC(hDC);
//      		AfxMessageBox("Cannot Create Compatible DC");


      // Output the text to the DC

   ::SetTextColor (hDCBits,RGB (0, 0, 255));
   ::SetBkColor(hDCBits,RGB (0,   0,   0));

   if (hBitmap = ::CreateBitmap (dx, dy, 1, 1, NULL))
      {
      hBitmap = (HBITMAP)::SelectObject (hDCBits, (HGDIOBJ)hBitmap);

      ::ExtTextOut (hDCBits, 0, 0, 0, NULL, string, nLen, NULL);
	  if( dwROP != PATCOPY )
      	::BitBlt(hDC, x, y, dx, dy, hDCBits, 0, 0, SRCINVERT);
	  else
		::BitBlt(hDC, x, y, dx, dy, hDCBits, 0, 0, SRCPAINT);
      hBitmap = (HBITMAP)::SelectObject (hDCBits, hBitmap);

      ::DeleteObject (hBitmap);
      }

   ::DeleteDC(hDCBits);
}

void CFotobrowView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDC *pDC;
	RECT rcClip;
	
	if( m_mode == DEFINEREGION )
	{
		CFotobrowDoc* pDoc = (CFotobrowDoc* )GetDocument();
		CDIB *dib = pDoc->GetDIB();
	
		pDC = GetDC();
		rcClip.top = point.y;
		rcClip.left = point.x;
		TrackMouse(NULL, &rcClip, dib->GetWidth(), dib->GetHeight() );
		ReleaseDC(pDC);	
	}
//	CView::OnLButtonDown(nFlags, point);
}

void CFotobrowView::OnViewDefineregion() 
{
	// TODO: Add your command handler code here
	mode_holder = GetMode();
	m_mode = DEFINEREGION;
}

void CFotobrowView::OnViewDeleteregion() 
{
	// TODO: Add your command handler code here
	
}

void CFotobrowView::OnViewLabelregion() 
{
	// TODO: Add your command handler code here
	
}

void CFotobrowView::DrawBoxes()
{
	image_equip_boxObj box;
	equipObj eq;
	HBRUSH hBrush;
	HBRUSH hOldBrush;
	RECT rect;
	HDC hDC;
	RECT client;
	POINT ptOrigin;

//////////////////////////
	hDC = ::GetDC(GetSafeHwnd());
   	GetClientRect (&client);
	CRect rcClient(&client);

      // Get mouse coordinates relative to origin of DIB.  Then
      //  setup the clip rectangle accordingly (it already should
      //  contain the starting point in its top/left).

   ptOrigin.x         = GetScrollPos (SB_HORZ);
   ptOrigin.y         = GetScrollPos (SB_VERT);
         	
   ::SetWindowOrgEx (hDC, ptOrigin.x, ptOrigin.y, NULL );

//////////////////////////
	box.ReadLock();
	eq.ReadLock();
	newone.ReadLock();
	CFotobrowDoc* pDoc = (CFotobrowDoc* )GetDocument();
	CDIB *dib = pDoc->GetDIB();
	if( !newone.Okay())
		return;
	for( box.GotoMember(newone, IMAGE_BOX, FIRST );
		box.Okay(); box.GotoMember(newone, IMAGE_BOX, NEXT ) )
	{
		eq.FromMember( &box, EQUIP_BOX );
		if( eq.Okay() )
		{
			rect.top = box.top;
			rect.bottom = box.bottom;
			rect.left = box.left;
			rect.right = box.right;
		rect.left += rcClient.Width()/(2*m_scale);
		rect.right += rcClient.Width()/(2*m_scale);
		rect.top = dib->GetHeight() - rect.top + rcClient.Height()/(2*m_scale);
		rect.bottom = dib->GetHeight() - rect.bottom + rcClient.Height()/(2*m_scale);
//			DrawSelect( hDC, *lpClipRect , DSTINVERT );
			hBrush = ::CreateSolidBrush(	RGB( 0,255,0) );
			hOldBrush = (HBRUSH)::SelectObject( hDC, hBrush );
			DrawSelect( hDC, rect, PATCOPY , eq.name);
		    ::SelectObject( hDC, hOldBrush );
		    ::DeleteObject( hBrush );
		}
		else
			AfxMessageBox("SCREW UP BABY - Database corrupted?");
	}
	box.FreeLock();
	eq.FreeLock();
	newone.FreeLock();
	::ReleaseDC(GetSafeHwnd(),hDC);
}


void CFotobrowView::OnFilePrintSetup() 
{
	// TODO: Add your command handler code here
	
}


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CFotobrowView::SetDCPixelFormat (HDC hdc)
{
    HANDLE hHeap;
    int nColors, i;
    LPLOGPALETTE lpPalette;
	//LOGPALETTE
    BYTE byRedMask, byGreenMask, byBlueMask;

    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof (PIXELFORMATDESCRIPTOR),             // Size of this structure
        1,                                          // Version number
        PFD_DRAW_TO_WINDOW |                        // Flags
        PFD_SUPPORT_OPENGL,
        //PFD_SINGLEBUFFER,
        PFD_TYPE_RGBA,                              // RGBA pixel values
        24,                                         // 24-bit color
        0, 0, 0, 0, 0, 0,                           // Don't care about these
        0, 0,                                       // No alpha buffer
        0, 0, 0, 0, 0,                              // No accumulation buffer
        32,                                         // 32-bit depth buffer
        0,                                          // No stencil buffer
        0,                                          // No auxiliary buffers
        PFD_MAIN_PLANE,                             // Layer type
        0,                                          // Reserved (must be 0)
        0, 0, 0                                     // No layer masks
    };

    int nPixelFormat;
    
    nPixelFormat = ChoosePixelFormat (hdc, &pfd);
    SetPixelFormat (hdc, nPixelFormat, &pfd);

    DescribePixelFormat (hdc, nPixelFormat, sizeof (PIXELFORMATDESCRIPTOR),
        &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        nColors = 1 << pfd.cColorBits;
        hHeap = GetProcessHeap ();

        (LPLOGPALETTE) lpPalette = (LPLOGPALETTE)HeapAlloc (hHeap, 0,
            sizeof (LOGPALETTE) + (nColors * sizeof (PALETTEENTRY)));
            
        lpPalette->palVersion = 0x300;
        lpPalette->palNumEntries = nColors;

        byRedMask = (1 << pfd.cRedBits) - 1;
        byGreenMask = (1 << pfd.cGreenBits) - 1;
        byBlueMask = (1 << pfd.cBlueBits) - 1;

        for (i=0; i<nColors; i++) {
            lpPalette->palPalEntry[i].peRed =
                (((i >> pfd.cRedShift) & byRedMask) * 255) / byRedMask;
            lpPalette->palPalEntry[i].peGreen =
                (((i >> pfd.cGreenShift) & byGreenMask) * 255) / byGreenMask;
            lpPalette->palPalEntry[i].peBlue =
                (((i >> pfd.cBlueShift) & byBlueMask) * 255) / byBlueMask;
            lpPalette->palPalEntry[i].peFlags = 0;
        }

        m_pal = CreatePalette (lpPalette);
        HeapFree (hHeap, 0, lpPalette);
#ifndef NO_PALETTE_JUNK
        if (m_pal != NULL) {
            SelectPalette (hdc, m_pal, FALSE);
            RealizePalette (hdc);
        }
#endif
    }
}

void CFotobrowView::OnTimer(UINT nIDEvent) 
{
	latitude += latinc;
	longitude += longinc;
	twist -= twistinc;
	
	CView::OnTimer(nIDEvent);
	
//	theCamera.movePosBy( 0.0, 0.0, 1000.0 );
//	theCamera.setRotation( twist, latitude, longitude );
//	InvalidateRect(NULL, FALSE);

//	CView::OnTimer(nIDEvent);
}

BOOL CFotobrowView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style = cs.style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	return CView::PreCreateWindow(cs);
}

void CFotobrowView::UpdateAllViews()
{
	if( !((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_rays &&
		!((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_overlay )
		return;


	((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->UpdateAllViews();
////////////////////////Update all graphical Windows//////////////////
	CDocument *pDoc;
	CObList *pList = new CObList;
	int flag = FALSE;
	((CFotobrowApp *)AfxGetApp())->GetDocumentList(pList);
	TRACE("Got the DOC List\n");
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		TRACE("Got the next DOC\n");
		pDoc = (CDocument *)(pList->GetNext(pos));
		if( pDoc->IsKindOf( RUNTIME_CLASS( CFotobrowDoc) ) &&
			pDoc != this->GetDocument() )
		{
			pDoc->UpdateAllViews(NULL);
		}
	}
	while( pList->GetCount() )
		pList->RemoveTail();
	delete pList;
//////////////////////////////////////////////////////////////////
}

void CFotobrowView::SetSelected(int flag)
{ 
	m_selected=flag; 
	if( ((CMainFrame *)((CFotobrowApp *)AfxGetApp())->m_pMainWnd)->m_rays ) 
	{
		updateRay( GETRAYS, 
			(char *)(const char *)(((CFotobrowDoc *)GetDocument())->szName),
			m_selected, m_sel.x, m_sel.y, 	
			((CFotobrowApp*)AfxGetApp())->m_imageReduction );
		UpdateAllViews();
	}
}

void CFotobrowView::updateMyRay(int on_off)
{
	if( m_selected )
		updateRay( GETRAYS, 
			(char *)(const char *)(((CFotobrowDoc *)GetDocument())->szName),
			on_off, m_sel.x, m_sel.y, 	
			((CFotobrowApp*)AfxGetApp())->m_imageReduction );
}

void CFotobrowView::DrawPoints()
{
	CBrush brush;
	CDC *pDC = GetDC();
	CRect client;
	GetClientRect(&client);
//	char tempstr[1024];
	brush.CreateSolidBrush(RGB(255,0,0));
	CBrush *old_brush = pDC->SelectObject(&brush);
    POSITION pos;
    for( pos = m_pointList.GetHeadPosition(); pos != NULL; 
			m_pointList.GetNext(pos) )
    {
		CPoint *aPoint = m_pointList.GetAt(pos);
		CPoint temp;
		CRect r;
		GetClientRect(&r);
		CPoint p = r.TopLeft();
		temp.x = m_scale*(aPoint->x - m_sel.x) + r.Width()/2 + p.x;
		temp.y = -m_scale*(aPoint->y - m_sel.y) + r.Height()/2 + p.y;
		//sprintf(tempstr,"Client:%d,%d,%d,%d\r\naPoint:%d,%d\r\n"
		//	"m_currPick:%d,%d\r\ntemp:%d,%d\r\n",
		//	r.left, r.top, r.right, r.bottom,
		//	aPoint->x, aPoint->y,
		//	m_currPick.x, m_currPick.y,
		//	temp.x, temp.y);
	//	AfxMessageBox(tempstr);
		CRect rect(temp.x-2*m_scale,temp.y-2*m_scale, 
			temp.x+2*m_scale, temp.y+2*m_scale);
		pDC->Rectangle(rect);
	}
	pDC->SelectObject(old_brush);
	ReleaseDC(pDC);
}

void CFotobrowView::cleanMultiSelect()
{
	while( m_pointList.GetCount() )
		delete m_pointList.RemoveTail();
	m_selected = FALSE;
	InvalidateRect(NULL,TRUE);
}

void CFotobrowView::OnGraphicsDrawlist() 
{
	CDrawList dlg(NULL,&GETWORLD);
	dlg.DoModal();
	InvalidateRect(NULL,FALSE);
	
//	regen_flag = TRUE;
//	HWND hwnd = GetSafeHwnd();
//	HDC hdc = ::GetDC(hwnd);
//CDC *dc = GetDC();
//ASSERT(dc);
//	wglMakeCurrent(hdc, m_hglrc );
//	wglMakeCurrent(NULL,NULL);
//	::ReleaseDC(hwnd, hdc);
}

void CFotobrowView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::OnPrint(pDC, pInfo);
}

void CFotobrowView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
