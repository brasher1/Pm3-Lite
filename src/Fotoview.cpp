// fotoview.cpp : implementation file
//

#include "stdafx.h"
#include "fotobrow.h"
#include "fotoview.h"
#include "mainfrm.h"
#include "common.h"
#include "zonesele.h"
#include "drawlist.h"
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

GLfloat latitude=0, longitude=0, latinc=0.0, longinc=5.0;
GLfloat twist=0, twistinc=0.0;
GLdouble radius;

#define MASTER	1

static struct ortho {
	double left, right, top, bottom;
} Ortho;

static PtgCamera theCamera;

#include "photog.h"
#include "../photog/output.h"

#define GLOBE			1
#define CONE			2
#define CYLINDER		3

#define BLACK_INDEX		0
#define RED_INDEX		13
#define GREEN_INDEX		14
#define BLUE_INDEX		16

VECTOR vectorTMatrixTransposeRotation( VECTOR v );
int importReviewModel(char *s, Ptg3DNode * );

static UINT BASED_CODE image_indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};
/////////////////////////
//PtgCamera theCamera;
//Ptg3DNode *theObjects;
//Ptg3DNode *theCameraNode;
//Ptg3DNode *theRays;
//Ptg3DNode *theModel;

#define ZOOMDIST		10000

int CFotoView::m_mode = NORMAL;

//////////////////////////////////
double TheLine[3][3];
char *minmaxNames[2][3];
  
///////
//////
#ifdef __WOW_THIS_IS_OLD_CODE
#define FUNC "WriteCameraToPDMS"
VECTOR ConvertCameraMatrix( double theMatrix[] )
{
#if FALSE
	VECTOR tm = theMatrix - 1;
	VECTOR tminv = matinv( tm );
	VECTOR s = vector( 1, 3 );
	VECTOR tmRot = decomp_matrix( tm, s );

	tmRot[10] = tminv[10];
	tmRot[11] = tminv[11];
	tmRot[12] = tminv[12];

	free_vector( s, 1, 3 );
	free_vector( tminv, 1, 12 );

	return( tmRot );
#endif

    int i;
    
    VECTOR tm = vector( 1, 13 );
    for( i = 0; i < 12; i++ )
	tm[i+1] = theMatrix[i];
    VECTOR tminv = matinv(tm);

    VECTOR s = vector( 1, 3 );
    VECTOR tmRot = decomp_matrix( tm, s );

    MATRIX *u = matrix( 0, 3, 0, 3 );
	VECTOR ret = vector(1,13);

    ret[1] = tmRot[1];
    ret[2] = -tmRot[3];
    ret[3] = tmRot[2];
    
    ret[4] = -tmRot[4];
    ret[5] = tmRot[6];
    ret[6] = -tmRot[5];

    ret[7] = -tmRot[7];
    ret[8] = tmRot[9];
    ret[9] = -tmRot[8];
    
    ret[11]	= -tminv[12];
    ret[10]	= tminv[10];
    ret[12]	= tminv[11];

	
    free_vector( tm, 1, 13 );
    free_vector( tminv, 1, 13 );
    free_vector( s, 1, 3 );
    free_vector( tmRot, 1, 13 );
    free_matrix( u, 0, 3, 0, 3 );
    
    return( ret );

}
#undef FUNC


//////////////////////////////////

#define FUNC "FindTMatrix"
int FindTMatrix( char *pszBundleID, char *pszImageName, VECTOR vTMatrix )
{
//    FUNCENTRY(FUNC);

	int r;
    struct tmatrix tmatrix_rec;
    struct images img_rec;
    struct bundle bundle_rec;
    int snFound = FALSE;
    static LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' },
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }
    };

    
    memset( &bundle_rec, 0, sizeof(struct bundle) );
    strcpy( bundle_rec.szTag, pszBundleID );
    
    snFound = FALSE;
    r = dt_keyfind( BUNDLE_SZTAG, &(bundle_rec.szTag), TASKER );
    if( r == S_OKAY )
    {
	dt_setor( BUNDLE_TMATRIX, TASKER );
	r = dt_findfm( BUNDLE_TMATRIX, TASKER );
	while( r == S_OKAY && !snFound )
	{
	    dt_setmm( IMAGES_TMATRIX, BUNDLE_TMATRIX, TASKER );
	    dt_setro( IMAGES_TMATRIX, TASKER );
	    
	    dt_recread( &img_rec, TASKER );
	    if( strcmp( img_rec.szName, pszImageName ) == 0 )
		snFound = TRUE;
	    else
		dt_findnm( BUNDLE_TMATRIX, TASKER );
	}
    }
    
    if( snFound )
    {
	dt_setrm( IMAGES_TMATRIX, TASKER );
	dt_recread( &tmatrix_rec, TASKER );
	
	for( int i = 0; i < 12; i++ )
	    vTMatrix[i+1] = tmatrix_rec.dfCameraPos[i];

	return( S_OKAY );
    }
    else
    {
	fprintf( stderr, "%s: Could not find image %s in bundle %s\n", 
		FUNC, pszImageName, pszBundleID );
	return( !S_OKAY );
    }
}
#undef FUNC
void PtgOutputCameras( void )
{
	int r;
    static LOCK_REQUEST lr[] = {
	{ BUNDLE, 'r' }, 
	{ TMATRIX, 'r' }, 
	{ IMAGES, 'r' }, 
	{ PIXELS, 'r' }, 
	{ POINTS, 'r' }, 
	{ XYZ, 'r' }
    };
    struct bundle theBundle;
    struct images theImage;
    int nNumCamerasWritten = 0;
    VECTOR tm;

//	int CheckLocks(DB_TASK *task, int dbn);
//	CheckLocks(TASKER);
    if( rdmGroupLock( 6, lr, TASKER ) != S_OKAY )
    {
		fprintf( stderr, "Unable to read lock records\n" );
		AfxMessageBox("Could not lock record");
		exit(1);
    }

    tm = vector( 1, 13 ); 
	
	theCameraNode = new Ptg3DNode;
	theCameraNode->setName("Cameras");
	theCameraNode->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
	theCameraNode->setColor( 0.0, 1.0, 0.0 );
	theCameraNode->setActive(FALSE);
	theObjects->addMember(theCameraNode);
    r=dt_keyfrst( BUNDLE_SZTAG, TASKER );
    while( r == S_OKAY )
    {
	dt_recread( &theBundle, TASKER );
	dt_setor( BUNDLE_TMATRIX, TASKER );
	int r2 = dt_findfm( BUNDLE_TMATRIX, TASKER );
	while( r2 == S_OKAY )
	{
	    dt_setmm( IMAGES_TMATRIX, BUNDLE_TMATRIX, TASKER );
	    dt_setro( IMAGES_TMATRIX, TASKER );
	    dt_recread( &theImage, TASKER );

	    DB_ADDR_P ctab;
	    int csize;
	    dt_rdcurr( &ctab, &csize, TASKER );
	    if( !FindTMatrix( theBundle.szTag, theImage.szName, tm ) )
	    {
		double total = 0.0;
		for( int i = 1; i < 13; i++ )
		    total += tm[i];
		if( total == 0.0 )
		    TRACE( "%s - %s: Invalid tmatrix.\n", theBundle.szTag, theImage.szName );
		else
		{
//#if FALSE
			Ptg3DNode *newNode = new Ptg3DNode;
			VECTOR ret = ConvertCameraMatrix(tm+1);
			newNode->setMatrix( ret );
			newNode->setName(theImage.szName);			
			free_vector( ret, 1, 12 );
			theCameraNode->addMember( newNode );
			
		    Ptg3DPyramidObj *thePyra = new Ptg3DPyramidObj;
		    thePyra->setHeight( 600.0 );
		    thePyra->setBaseDims( 500.0, 500.0*0.75 );
		    thePyra->setTopDims( 0.0, 0.0 );
		    thePyra->setOffsets( 0.0, 0.0 );

			VECTOR v = vector( 1, 12 );
			v[1] = 1.0;
			v[5] = v[9] = -1.0;
			v[2] = v[3] = v[4] = v[6] = v[7] = v[8] = 0.0;
			v[10] = v[11] = 0.0;
			v[12] = 300.0;
			thePyra->setMatrix( v );

			thePyra->setColor(0.0,1.0,0.2);
//			thePyra->setName(theImage.szName);			
			thePyra->setDrawStyle(Ptg3DNode::DS_WIREFRAME); 
			thePyra->setActive(FALSE);
			free_vector(v,1,13);

		    newNode->addMember( thePyra );
//#endif
#if FALSE
		    Ptg3DPyramidObj *thePyra = new Ptg3DPyramidObj;
		    thePyra->setHeight( 600.0 );
		    thePyra->setBaseDims( 500.0, 500.0*0.75 );
		    thePyra->setTopDims( 0.0, 0.0 );
		    thePyra->setOffsets( 0.0, 0.0 );
			thePyra->setColor(0.0,1.0,0.2);
			thePyra->setName(theImage.szName);			
			VECTOR ret = ConvertCameraMatrix(tm+1);
		    thePyra->setMatrix(ret);
			thePyra->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
			free_vector(ret,1,13);

		    theObjects->addMember( thePyra );
#endif			
		    nNumCamerasWritten++;
		}
	    }
		
	    else
		fprintf( stderr, "Unable to find image '%s' in bundle '%s'.\n",
			theImage.szName, theBundle.szTag );
	    dt_wrcurr( ctab, TASKER );

	    r2=dt_findnm( BUNDLE_TMATRIX, TASKER );
	}
	r = dt_keynext( BUNDLE_SZTAG, TASKER );
    }

    rdmGroupFree( 6, lr, TASKER );

    Output( 0, "Wrote %d cameras.\n", nNumCamerasWritten );

    free_vector( tm, 1, 13 );
}
/////////////////////////////////
#endif //__WOW_THIS_IS_OLD_CODE

void KillMeHard()
{
//	delete theObjects;
}

BOOL bSetupPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;//PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;

    ppfd->iPixelType = PFD_TYPE_RGBA;
    ppfd->cColorBits = 8;

    ppfd->cDepthBits = 16;			//GLX_DEPTH_SIZE


    ppfd->cAccumBits = 0;			//ACCUM NOT SUPPORTED

    ppfd->cStencilBits = 0;			//GLX_STENCIL_SIZE

    pixelformat = ChoosePixelFormat(hdc, ppfd);

    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 )
    {
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);

        return FALSE;
    }

    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
    {
		char temp[128];
		sprintf(temp, "SetPixelFormat Failed : %d", GetLastError() );
        MessageBox(NULL, temp, "Error", MB_OK);
        return FALSE;
    }
							         
    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
void CFotoView::SetDCPixelFormat (HDC hdc)
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
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
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


/////////////////////////////////////////////////////////////////////////////
// CFotoView

IMPLEMENT_DYNCREATE(CFotoView, CView)

CFotoView::CFotoView()
{
//	if( theObjects )
	SetMode( NORMAL );
	m_drawstyle = Ptg3DNode::DS_INVISIBLE;
	regen_flag = TRUE;
	clip_flag = FALSE;
	m_pBuffer = NULL;
	m_pal = NULL;
}

CFotoView::~CFotoView()
{
	KillMeHard();
	
	if( m_pBuffer )
		free(m_pBuffer);

	if( m_StatusBar )
		delete m_StatusBar;

}


BEGIN_MESSAGE_MAP(CFotoView, CView)
	//{{AFX_MSG_MAP(CFotoView)
	ON_COMMAND(IDC_CAMERA, OnCamera)
	ON_COMMAND(IDC_IMAGE, OnImage)
	ON_COMMAND(IDC_TARGET, OnTarget)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDC_ZOOMIN, OnZoomin)
	ON_COMMAND(IDC_ZOOMOUT, OnZoomout)
	ON_COMMAND(IDC_SELECT, OnSelect)
	ON_COMMAND(ID_SET_ORTHO, OnSetOrtho)
	ON_COMMAND(ID_SET_PERSPECTIVE, OnSetPerspective)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDC_CENTER, OnSetCenter)
	ON_COMMAND(IDC_CLIPVOLUME, OnClipvolume)
	ON_UPDATE_COMMAND_UI(IDC_ZOOMIN, OnUpdateZoomin)
	ON_UPDATE_COMMAND_UI(IDC_ZOOMOUT, OnUpdateZoomout)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_GRAPHICS_DRAWLIST, OnGraphicsDrawlist)
	ON_COMMAND(ID_VIEW_FILLMODE, OnViewFillmode)
	ON_COMMAND(ID_GRAPHICS_CLIPZONES, OnGraphicsClipzones)
	ON_COMMAND(ID_GRAPHICS_FITVIEW, OnGraphicsFitview)
	ON_COMMAND(ID_GRAPHICS_LIGHT, OnGraphicsLight)
	ON_COMMAND(ID_GRAPHICS_OPENGLSETTINGS, OnGraphicsOpenglsettings)
	ON_COMMAND(IDC_PERSPECTIVE, OnSetPerspective)
	ON_COMMAND(IDC_ORTHO, OnSetOrtho)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_NEW_CLIP_VOLUME, NewClipvolume)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFotoView drawing
 int Points[][2] = {
 	{310,-110},
	{115,-715},
	{215,-185},
	{650,-544},
	{420,-380},
	{700,-290} };
int directions [] = {
	45,17,20,13,200,127};

#define RAD1A		-80
#define RAD1B		65
#define RAD2A		80
#define RAD2B		-65
#define RAD3A		80
#define RAD3B		65
#define RAD4		20


void CFotoView::OnDraw(CDC* pDC)
{
	
	BeginWaitCursor();
    CFotogDoc *pDoc = (CFotogDoc * )GetDocument();
//static GLvoid *buffer = NULL;
#ifndef NO_PALETTE_JUNK
	CPalette *pOldPal = pDC->SelectPalette(pDoc->GetPalette(),TRUE);
	pDC->RealizePalette();
#endif
/////////////////////////////////////
CRect rect;
GetClientRect(&rect);
TRACE("Height=%d, Width=%d\n", rect.Height(), rect.Width() );
//HDC hdc = pDC->GetSafeHdc();
HWND hwnd = GetSafeHwnd();
HDC hdc = ::GetDC(hwnd);

if( regen_flag )
{
//	glDrawBuffer(GL_FRONT_AND_BACK);
	TRACE( "Beginning regen\n" );
	wglMakeCurrent(hdc, hglrc );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glPushMatrix();
/////
	glClearColor(0.0,0.0,0.0,1.0);
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	glColor3d(0.2,0.2,0.2);

    theCamera.draw();
	glColor4d(0.2,0.2,0.2,1.0);

//	glCallList(MASTER);
	if( clip_flag )
	{
		GLdouble eqn[4];
		eqn[0] = 1.0; eqn[1] = 0.0; eqn[2] = 0.0; eqn[3] = -clip_values[0]; 
		glClipPlane( GL_CLIP_PLANE0, eqn );
		glEnable(GL_CLIP_PLANE0);
		eqn[0] = 0.0; eqn[1] = 1.0; eqn[2] = 0.0; eqn[3] = -clip_values[1]; 
		glClipPlane( GL_CLIP_PLANE1, eqn );
		glEnable(GL_CLIP_PLANE1);
		eqn[0] = 0.0; eqn[1] = 0.0; eqn[2] = 1.0; eqn[3] = -clip_values[2]; 
		glClipPlane( GL_CLIP_PLANE2, eqn );
		glEnable(GL_CLIP_PLANE2);
		eqn[0] = -1.0; eqn[1] = 0.0; eqn[2] = 0.0; eqn[3] = clip_values[3]; 
		glClipPlane( GL_CLIP_PLANE3, eqn );
		glEnable(GL_CLIP_PLANE3);
		eqn[0] = 0.0; eqn[1] = -1.0; eqn[2] = 0.0; eqn[3] = clip_values[4]; 
		glClipPlane( GL_CLIP_PLANE4, eqn );
		glEnable(GL_CLIP_PLANE4);
		eqn[0] = 0.0; eqn[1] = 0.0; eqn[2] = -1.0; eqn[3] = clip_values[5]; 
		glClipPlane( GL_CLIP_PLANE5, eqn );
		glEnable(GL_CLIP_PLANE5);
	}

	if( list_num == 0 )
	{
		list_num = glGenLists( 1 );

		glNewList( list_num, GL_COMPILE_AND_EXECUTE );

		GETWORLD.setDrawStyle( m_drawstyle );
//		double array[12] = {1.0,0.0,0.0,
//							0.0,0.0,1.0,
//							0.0,-1.0,0.0,
//							0.0,0.0,0.0};
		double array[12] = {1.0,0.0,0.0,
							0.0,0.0,-1.0,
							0.0,1.0,0.0,
							0.0,0.0,0.0};
		PtgTransMatrix trans;
		trans.setMatrix(array-1);
		trans.draw();
	    GETWORLD.draw();
		GETRAYS->draw();
		glEndList();
//		MessageBeep(MB_OK);
	}
	else
	{
		glCallList(list_num);	
	}
    glPopMatrix();
    
    glFlush();
  	regen_flag=FALSE;

	if( m_pBuffer != NULL )
		free( m_pBuffer );
    
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glRasterPos2i(0,0);//-rect.Width()/2, -rect.Height()/2);
	m_pBuffer = (GLvoid *)malloc( sizeof(GL_UNSIGNED_BYTE) * rect.Width() * rect.Height()*4 );
//	glReadPixels( -rect.Width()/2, -rect.Height()/2, rect.Width(), rect.Height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer );
//	glReadPixels( rect.Width()/2, rect.Height()/2, rect.Width(), rect.Height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer );  
	glReadPixels( 0,0, rect.Width(), rect.Height(), GL_RGBA, GL_UNSIGNED_BYTE, m_pBuffer );  	
	SwapBuffers(pDC->GetSafeHdc());
	TRACE( "Done with regen\n" );
}
else
{
//////////////////
//////////////////
	TRACE( "Beginning quick update\n" );
	wglMakeCurrent(hdc, hglrc );

    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glViewport(0, 0, rect.Width(), rect.Height());
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    TRACE("Viewport in OnDraw 3D view: %d, %d, %d, %d\n", viewport[0], viewport[1],
				viewport[2], viewport[3] );

    
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, rect.Width(), 0, rect.Height());
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRasterPos2i(0,0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glDrawPixels( rect.Width(), rect.Height(), GL_RGBA, GL_FLOAT, buffer );
	glDrawPixels( rect.Width(), rect.Height(), GL_RGBA, GL_UNSIGNED_BYTE, m_pBuffer );

//	glColor3f(0.9, 0.2, 0.1 );
//	glBegin(GL_LINES);
//	glVertex2i(0,0);
//	glVertex2i(rect.Width(), rect.Height());
//	glEnd();

	SwapBuffers(pDC->GetSafeHdc());
	TRACE( "Done with quick update\n" );
/*					 
    glViewport(0, 0, rect.Width(), rect.Height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, rect.Width(), 0, rect.Height());
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glReadBuffer(GL_BACK);
	glDrawBuffer(GL_FRONT);
	glRasterPos3i(0, 0, 1);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelZoom( 1.0, 1.0 );
	glCopyPixels(0,0, rect.Width(), rect.Height(), GL_COLOR); 
*/
}
/////////////////////////////////////////////	
	char temp[128];
	SetStatusString(temp);
#ifndef NO_PALETTE_JUNK
	pDC->SelectPalette(pOldPal, FALSE);
#endif
	wglMakeCurrent(NULL,NULL);
	::ReleaseDC(GetSafeHwnd(), hdc);
	EndWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////
// CFotoView diagnostics

#ifdef _DEBUG
void CFotoView::AssertValid() const
{
	CView::AssertValid();
}

void CFotoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFotoView message handlers

void CFotoView::OnCamera() 
{
	// TODO: Add your command handler code here
	
}

void CFotoView::OnImage() 
{
	// TODO: Add your command handler code here
	
}

void CFotoView::OnTarget() 
{
	// TODO: Add your command handler code here
	
}


						   //CView
int CFotoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style = lpCreateStruct->style|WS_HSCROLL|WS_VSCROLL;
	
	list_num = 0;
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	HWND hwnd = GetSafeHwnd();
	HDC hdc = ::GetDC(hwnd);
								 
	tmin[0]=tmin[1]=tmin[2]= DBL_MAX;
	tmax[0]=tmax[1]=tmax[2]= -DBL_MAX;
	    
	SetDCPixelFormat(hdc);
	hglrc = wglCreateContext(hdc );
	wglMakeCurrent(hdc, hglrc );
///
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_AUTO_NORMAL);
//	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 2);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT );
	GLfloat v[4] = { 0.45, 0.45, 0.45, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
///
	glEnable(GL_BLEND);
 	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//	initElemList();              //Took this call out to call upon loading.
	getMinMaxCamera(GETCAMERAS);  //used to be GETCAMERAS

/*
	printf( "Min cameras [0] = '%s'(%lf)\n", minmaxNames[0][0], tmin[0] );
	printf( "            [1] = '%s'(%lf)\n", minmaxNames[0][1], tmin[1] );
	printf( "            [2] = '%s'(%lf)\n", minmaxNames[0][2], tmin[2] );
	printf( "Max cameras [0] = '%s'(%lf)\n", minmaxNames[1][0], tmax[0] );
	printf( "            [1] = '%s'(%lf)\n", minmaxNames[1][1], tmax[1] );
	printf( "            [2] = '%s'(%lf)\n", minmaxNames[1][2], tmax[2] );
*/
////////////////////////////////////////////////////////////////////////////////
	OnGraphicsFitview();
#if FALSE
    theCamera.setPosition( (tmax[0]-tmin[0])/2+tmin[0], 
    						(tmax[1]-tmin[1])/2+tmin[1],
							(tmax[2]-tmin[2])/2+tmin[2] );
	// 
	theCamera.setLookDist( 10000.0 );
	theCamera.setLookAt( (tmax[0]-tmin[0])/2+tmin[0], 
    						(tmax[1]-tmin[1])/2+tmin[1],
							(tmax[2]-tmin[2])/2+tmin[2] );
	//
    theCamera.setClipping( -200000.0, 200000.0 );
#endif
	theCamera.setOrthoClip( Ortho.left, Ortho.right, Ortho.bottom, Ortho.top ); 
//	char temp[128];
//	sprintf(temp,"min0=%10.2f,min1=%10.2f,min2=%10.2f", tmin[0], tmin[1], tmin[2] );
//	AfxMessageBox(temp);
//	sprintf(temp,"max0=%10.2f,max1=%10.2f,max2=%10.2f", tmax[0], tmax[1], tmax[2] );
//	AfxMessageBox(temp);
////////////////////////////////////////////////////////////////////////////////
//AfxMessageBox("OnCreate 3");
//	DrawColumns();

	GETWORLD.setDrawStyle( m_drawstyle );
//	glNewList( MASTER, GL_COMPILE );
//	    theObjects->draw();
//	glEndList();		
	CRect rect;
	::GetClientRect(GetSafeHwnd(), &rect);

//	SetTimer( 1, 2000 , NULL);
	
	m_StatusBar = new CStatusBar;
	if (!m_StatusBar->Create(GetParent()) ||
		!m_StatusBar->SetIndicators(image_indicators,
		  sizeof(image_indicators)/sizeof(UINT)))
	{
		AfxMessageBox("Failed to create status bar");
		return -1;      // fail to create
	}
	ShowScrollBar(SB_BOTH, TRUE);
	SetScrollRange(SB_VERT, 0, 180, TRUE);
	SetScrollRange(SB_HORZ, 0, 360, TRUE);
	SetScrollPos(SB_VERT, 0, TRUE);
	SetScrollPos(SB_HORZ, 0, TRUE);
	wglMakeCurrent(NULL,NULL);
	::ReleaseDC(GetSafeHwnd(), hdc);
	return 0;
}

BOOL CFotoView::PreCreateWindow(CREATESTRUCT& cs) 
{
	//the view windows style bits must include WS_CLIPSIBLINGS and
	//WS_CLIPCHILDREN so that the wgl functions will work. This is
	//
	cs.style = cs.style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	return CView::PreCreateWindow(cs);
}

void CFotoView::OnSize(UINT nType, int cx, int cy) 
{
	double width, height;
	GLfloat aspect;

	TRACE("OnSize\nSize Message: %u,  cx=%d, cy=%d\n", nType, cx, cy);

	HWND hwnd = GetSafeHwnd();
	HDC hdc = ::GetDC(hwnd);

	wglMakeCurrent(hdc, hglrc );
	
	GLenum err;
	while( (err = glGetError()) != GL_NO_ERROR )
		TRACE("Before Viewport GLerror = %d\n", err);
//glgetError

	glViewport(0,0,cx,cy);
	while( (err = glGetError()) != GL_NO_ERROR )
		TRACE("After Viewport GLerror = %d\n", err);
	
	aspect = (GLfloat) cx/cy;
	theCamera.setAspectRatio( aspect);
	width = Ortho.right - Ortho.left;
	height = Ortho.top - Ortho.bottom;
	CRect x(Ortho.left, Ortho.top, Ortho.right, Ortho.bottom );
	DoOrthoClip(x);
	regen_flag = TRUE;
/*
	GLfloat height;
	GLfloat width = 10000.0;
	height = ((GLfloat)((GLfloat)cy/(GLfloat)cx))*width;
	*/
	wglMakeCurrent(NULL,NULL);
	::ReleaseDC(GetSafeHwnd(),hdc);
}

void CFotoView::OnTimer(UINT nIDEvent) 
{
	latitude += latinc;
	longitude += longinc;
	twist -= twistinc;
	
	CView::OnTimer(nIDEvent);
	
//	theCamera.movePosBy( 0.0, 0.0, 1000.0 );
	theCamera.setRotation( twist, latitude, longitude );
	regen_flag = TRUE;
	InvalidateRect(NULL, FALSE);

}

void CFotoView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	GetParent()->SetWindowText(GetDocument()->GetTitle()+" - Graphical View");

}

void CFotoView::DrawColumns() 
{
	Ptg3DNode *theColumnNode = new Ptg3DNode;
	theColumnNode->setName("Column Lines");
	theColumnNode->setDrawStyle(Ptg3DNode::DS_INVISIBLE);
	theColumnNode->setColor( 0.5, 0.5, 1.0 );
	theColumnNode->setActive(FALSE);
	GETWORLD.addMember(theColumnNode);

	for( m_class.First(); m_class.Okay(); m_class.Next() )
	{
		if( m_class.type == COLUMN )
		{

			m_all.FromOwner(&m_class, ELEMENT_CLASSES );
			m_element = m_all.GetElement();
			DbAddr dba(m_element->GetDba());
			m_line.GotoDirect( dba );
			
			Ptg3DLineObj *line = new Ptg3DLineObj();
			line->setOrigin(m_line.x1, -m_line.z1, m_line.y1);
			line->setEndPoint(m_line.x2, -m_line.z2, m_line.y2);
			line->setColor(0.0, 0.5, 1.0 );
			line->setName(m_class.name);
			line->setDrawStyle(Ptg3DNode::DS_WIREFRAME);
			theColumnNode->addMember(line);
		}
	}
}

void CFotoView::OnPaletteChanged(CWnd* pFocusWnd) 
{
	HDC hdc;
#ifndef NO_PALETTE_JUNK
	if ((m_pal != NULL) && (pFocusWnd != this)) 
	{
    	hdc = ::GetDC(GetSafeHwnd());
        ::SelectPalette (hdc, m_pal, FALSE);
        if (::RealizePalette (hdc))
            ::UpdateColors (hdc);
        ::ReleaseDC (GetSafeHwnd(), hdc);
    }
#endif
}

BOOL CFotoView::OnQueryNewPalette() 
{
#ifndef NO_PALETTE_JUNK
	if (m_pal != NULL) 
	{
		int n;
    	HDC hdc = ::GetDC(GetSafeHwnd());
		::SelectPalette (hdc, m_pal, FALSE);
        if (n = ::RealizePalette (hdc))
		{
			regen_flag = FALSE;
            ::InvalidateRect (GetSafeHwnd(), NULL, FALSE);
		}
        ::ReleaseDC (GetSafeHwnd(), hdc);
        return n;
    }
#endif
	return FALSE;
}

BOOL CFotoView::SelRelPal(BOOL bForceBkgnd)
{
	if( m_pal == NULL )
		return FALSE;

#ifndef NO_PALETTE_JUNK
    // We are going active, so realize our palette.
    CDC* pdc = GetDC();
    HPALETTE poldpal = ::SelectPalette(pdc->GetSafeHdc(),m_pal,
                                           bForceBkgnd);
    UINT u = ::RealizePalette(pdc->GetSafeHdc());
    ::SelectPalette(pdc->GetSafeHdc(), poldpal, TRUE);
    ::RealizePalette(pdc->GetSafeHdc());
    ReleaseDC(pdc);
    // If any colors have changed or we are in the
    // background, repaint the lot.
    if (u || bForceBkgnd) {
		regen_flag = FALSE;
	    InvalidateRect(NULL, TRUE); // Repaint.
    }
    return (BOOL) u; // TRUE if some colors changed.
#else
	return TRUE;
#endif
}

void CFotoView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
    if (bActivate) {
        // We are going active so realize our palette in
        // the foreground and tell the frame to notify
        // all other windows of the change.
#ifndef NO_PALETTE_JUNK
        SelRelPal(FALSE);

        CWnd* pMainFrame = AfxGetApp()->m_pMainWnd;
        pMainFrame->SendMessage(WM_PALETTECHANGED,
                                (WPARAM)GetSafeHwnd(),
                              0);
#endif
    }
}

void CFotoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	Ptg3DNode *picked;
	picked = getPickedElement( &theCamera, &GETWORLD, point.x, point.y);
// jwb 07/31/95 - fixed problem with picking giving NULL names after re-orienting camera pyramids
	if( picked != NULL && picked->getOwner() != NULL && nFlags&MK_CONTROL )
		OpenImageByName((char *)(((Ptg3DNode *)(picked->getOwner()))->getName()));
	else if( picked != NULL )
	{
		VECTOR v = picked->getMatrix(&GETWORLD);
//{
//	char temp[256];
//	sprintf( temp, "translation = %lf, %lf, %lf", v[10], v[11], v[12] );
//	AfxMessageBox( temp );
//}
		theCamera.setPosition(v[10], -v[12], theCamera.getPosition()[2] );
		theCamera.setLookAt( v[10],-v[12],v[11]);
		free_vector(v,1,12);
		regen_flag = TRUE;
		InvalidateRect(NULL,FALSE);
	}
	else
		AfxMessageBox("Did not hit anything");
	CView::OnLButtonUp(nFlags, point);
}

#define BUFSIZE 512
Ptg3DNode *CFotoView::getPickedElement( PtgCamera *theCamera, Ptg3DNode *theStartNode,
	int nMouseX, int nMouseY )
{
//	CDC *dc = GetDC();
//	ASSERT(dc);
	HWND hwnd = GetSafeHwnd();
	HDC hdc = ::GetDC(hwnd);
	wglMakeCurrent(hdc, hglrc );

//	ReleaseDC(dc);
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];
    
    glGetIntegerv( GL_VIEWPORT, viewport );
    glSelectBuffer( BUFSIZE, selectBuf );
    glRenderMode( GL_SELECT );
    
    glInitNames();
    glPushName( (unsigned long)-1 );
    TRACE("Viewport in 3D view get picked: %d, %d, %d, %d\n", viewport[0], viewport[1],
				viewport[2], viewport[3] );
    glPushMatrix();
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	CRect rect;
	GetClientRect(&rect);
	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = rect.Width();
	viewport[3] = rect.Height();
	TRACE("The New Value of Viewport is %d, %d, %d, %d\n",viewport[0], viewport[1],
				viewport[2], viewport[3] );
    gluPickMatrix( (GLdouble) nMouseX, (GLdouble) (viewport[3] - nMouseY), 5.0, 5.0, viewport );
//	gluPickMatrix( (GLdouble) nMouseX, (GLdouble) (rect.Width() - nMouseY), 5.0, 5.0, viewport );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    /*  create 5x5 pixel picking region near cursor location	*/

    theCamera->draw();
	double array[12] = {1.0,0.0,0.0,
						0.0,0.0,-1.0,
						0.0,1.0,0.0,
						0.0,0.0,0.0};
	PtgTransMatrix trans;
	trans.setMatrix(array-1);
	trans.draw();
    theStartNode->draw();

    glPopMatrix ();
    glFlush ();

    hits = glRenderMode( GL_RENDER );

  //  fprintf( stderr, "hits = %d\n", hits );
	TRACE("Inside getPicked\n");
    GLuint *ptr = (GLuint *)selectBuf;
    for( int i = 0; i < hits; i++ )
    {
		GLuint nNumNames = *ptr;
		GLuint *nIDS = ptr+3;
		ptr+= nNumNames+3;
		TRACE("Calling FindByID\n");
		wglMakeCurrent(NULL,NULL);
	    ::ReleaseDC(GetSafeHwnd(),hdc);
		Ptg3DNode *found = theStartNode->findByID( nNumNames-2, (unsigned int *)nIDS+2 ); 
        CWnd* pMainFrame = AfxGetApp()->m_pMainWnd;
		Ptg3DNode *found2;
		if( found->getName() == NULL ) 
			found2 = (Ptg3DNode *)(found->getOwner());
		else
			found2 = found;
		AfxGetApp()->m_pMainWnd->SendMessage(UM_CURRENT_ELEMENT_CHANGE,(WPARAM)found2,(LPARAM)found2);
		return( found );
    }
	wglMakeCurrent(NULL,NULL);
    ::ReleaseDC(GetSafeHwnd(),hdc);
    return( NULL );
}

void CFotoView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	int movement = 0;
	double factor = 0;
	switch( GetMode() )
	{
		case ZOOMIN:
			movement = -ZOOMDIST;
			factor = 0.5;
			break;
		case ZOOMOUT:
			movement = ZOOMDIST;
			factor = 2.0;
			break; 
	}
	if( movement )
	{
		theCamera.movePosBy( 0,0,movement);
		CRect rect(Ortho.left*=factor, Ortho.top*=factor, 
					Ortho.right*=factor, Ortho.bottom*=factor );
		DoOrthoClip(rect);
		theCamera.setLookDist( theCamera.getLookDist()*factor);
		regen_flag = TRUE;
		InvalidateRect(NULL,FALSE);
	}
	CView::OnRButtonUp(nFlags, point);
}

void CFotoView::OnZoomin() 
{
	SetMode(ZOOMIN);	
}

void CFotoView::OnZoomout() 
{
	SetMode(ZOOMOUT);	
}

void CFotoView::OnSelect() 
{
	SetMode(SELECT);	
}


void CFotoView::OnSetOrtho() 
{
	theCamera.setViewType( PtgCamera::VT_ORTHO );
	regen_flag = TRUE;
	InvalidateRect(NULL,FALSE);
}

void CFotoView::OnSetPerspective() 
{
	theCamera.setViewType( PtgCamera::VT_PERSPECTIVE );
	regen_flag = TRUE;
	InvalidateRect(NULL,FALSE);
}

void CFotoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
//	Ptg3DNode *picked;
//	picked = getPickedElement( &theCamera, theObjects, point.x, point.y);
//	if( picked != NULL)
//	{
//	}
	CView::OnLButtonDblClk(nFlags, point);
}

void CFotoView::OnSetCenter() 
{
	// TODO: Add your command handler code here
	
}

void CFotoView::OnClipvolume() 
{
	CZoneSelectDlg zone;
	zone.DoModal();

	regen_flag = TRUE;
	InvalidateRect(NULL,FALSE);
	CDC *dc = GetDC();
	ASSERT(dc);
	wglMakeCurrent(dc->GetSafeHdc(), hglrc );
	if( list_num != 0 )
		glDeleteLists(list_num,1);
	list_num = 0;
	wglMakeCurrent(NULL,NULL);
	ReleaseDC(dc);
}


void CFotoView::OnUpdateZoomin(CCmdUI* pCmdUI) 
{
	if( GetMode() != ZOOMIN )
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);	
}

void CFotoView::OnUpdateZoomout(CCmdUI* pCmdUI) 
{
	if( GetMode() != ZOOMOUT )
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);	
}

LONG CFotoView::NewClipvolume(WPARAM w, LPARAM dba) 
{
//  AfxMessageBox("Got the clip volume message");
	PropertiesObj p;
	p.ReadLock();
	p.GotoDirect(DbAddr(dba));
	p.FreeLock();
	
	
	clip_values[0] = p.range.x1;
	clip_values[1] = -p.range.z1;
	clip_values[2] = p.range.y1;
	clip_values[3] = p.range.x2;
	clip_values[4] = -p.range.z2;
	clip_values[5] = p.range.y2;

	clip_flag = TRUE;
	return 0L;
}

void CFotoView::DoOrthoClip(CRect& rect)
{
	double height = abs(rect.Height());
	double width = abs(rect.Width());

	RECT size;
	GetWindowRect(&size);
	CRect rect1 = size;
	int cx = abs(rect1.Width());
	int cy = abs(rect1.Height());

	if( height / (double)cy > width / (double)cx )
	{
//		theCamera.setPosition( Ortho.left + ((height/(double)cy)*cx)/2, 
//						Ortho.bottom + height / 2 , 200000.0);
		theCamera.setOrthoClip( -((height/(double)cy)*cx)/2, ((height/(double)cy)*cx)/2,
					 -(height/2.0), height/2.0 ); 
	}
	else
	{
//		theCamera.setPosition( Ortho.left + ((height/(double)cy)*cx)/2, 
//						Ortho.bottom + height / 2 , 200000.0);
		theCamera.setOrthoClip( -((height/(double)cy)*cx)/2, ((height/(double)cy)*cx)/2,
					 -(height/2.0), height/2.0 ); 
	}

}

void CFotoView::SetStatusString(char *s)
{
//	RECT rect;
	char temp1[128];
	sprintf(temp1,"View: %ld from NORTH, %ld from UP",GetScrollPos(SB_HORZ), 180-GetScrollPos(SB_VERT));

	m_StatusBar->SetPaneInfo(0,0,SBPS_STRETCH, (strlen(temp1)+1)*4);
	m_StatusBar->SetPaneText(0,(char *)temp1);
}

void CFotoView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int flag=0;
	switch( nSBCode )
	{
		case SB_BOTTOM://    Scroll to far left.
			longitude = 0;
			break;
		case SB_ENDSCROLL://    End scroll.
			flag = 1;
			break;
		case SB_LINEUP://    Scroll left.
			longitude = longitude - 1;
			break;
		case SB_LINEDOWN://    Scroll right.
			longitude = longitude + 1;
			break;
		case SB_PAGEUP://    Scroll one page left.
			longitude = longitude - 360.0/16.0;
			break;
		case SB_PAGEDOWN://    Scroll one page right.
			longitude = longitude + 360.0/16.0;
			break;
		case SB_TOP://    Scroll to far right.
			longitude = 360;
			break;
		case SB_THUMBPOSITION://    Scroll to absolute position. The current position is specified by the nPos 
		case SB_THUMBTRACK://    Drag scroll box to specified position. The current position is specified by 
			longitude = nPos;
			break;
	}
//	longitude = nPos;
	theCamera.setRotation( 0.0, latitude, longitude );
	SetScrollPos(SB_HORZ, longitude, TRUE);
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
	SetStatusString("wow");
	if( flag )
	{
		regen_flag = TRUE;
		InvalidateRect(NULL,FALSE);
	}
}

void CFotoView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int flag=0;
	switch( nSBCode )
	{
		case SB_BOTTOM://    Scroll to far left.
			latitude = 0;
			break;
		case SB_ENDSCROLL://    End scroll.
			flag = 1;
			break;
		case SB_LINEUP://    Scroll left.
			latitude = latitude - 1;
			break;
		case SB_LINEDOWN://    Scroll right.
			latitude = latitude + 1;
			break;
		case SB_PAGEUP://    Scroll one page left.
			latitude = latitude - 180.0/8.0;
			break;
		case SB_PAGEDOWN://    Scroll one page right.
			latitude = latitude + 180.0/8.0;
			break;
		case SB_TOP://    Scroll to far right.
			latitude = 360;
			break;
		case SB_THUMBPOSITION://    Scroll to absolute position. The current position is specified by the nPos 
		case SB_THUMBTRACK://    Drag scroll box to specified position. The current position is specified by 
			latitude = nPos;
			break;
	}
//	latitude = nPos;
	theCamera.setRotation( 0.0, latitude, longitude );
	SetScrollPos(SB_VERT, latitude, TRUE);
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
	SetStatusString("wow");
	if( flag )
	{
		regen_flag = TRUE;
		InvalidateRect(NULL,FALSE);
	}
}

BOOL CFotoView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void CFotoView::OnGraphicsDrawlist() 
{
	CDrawList dlg(NULL,&GETWORLD);
	dlg.DoModal();
	regen_flag = TRUE;
	InvalidateRect(NULL,FALSE);
	CDC *dc = GetDC();
	ASSERT(dc);
	wglMakeCurrent(dc->GetSafeHdc(), hglrc );

	if( list_num != 0 )
		glDeleteLists(list_num,1);
	list_num = 0;
	wglMakeCurrent(NULL,NULL);
	ReleaseDC(dc);
}

void CFotoView::OnViewFillmode() 
{
	if( m_drawstyle == Ptg3DNode::DS_SOLIDS )
		m_drawstyle = Ptg3DNode::DS_WIREFRAME;
	else
		m_drawstyle = Ptg3DNode::DS_SOLIDS;
	regen_flag = TRUE;
	InvalidateRect(NULL,FALSE);
}

void CFotoView::OnGraphicsClipzones() 
{
	CZoneSelectDlg zone;
	zone.DoModal();

	regen_flag = TRUE;
	InvalidateRect(NULL,FALSE);
	CDC *dc = GetDC();
	ASSERT(dc);
	wglMakeCurrent(dc->GetSafeHdc(), hglrc );

	if( list_num != 0 )
		glDeleteLists(list_num,1);
	list_num = 0;
	wglMakeCurrent(NULL,NULL);
	ReleaseDC(dc);
}

void CFotoView::getMinMaxCamera(Ptg3DNode *elem) 
{
    POSITION pos;
	
    if( !elem->getMemberList().IsEmpty() )
    {
		for( pos = elem->getMemberList().GetHeadPosition(); pos != NULL; elem->getMemberList().GetNext(pos) )
		{
		    Ptg3DNode *anElem = (Ptg3DNode *)elem->getMemberList().GetAt(pos);
		    getMinMaxCamera( anElem );
		}
	}
	else
	{
		VECTOR v = elem->getMatrix(&GETWORLD);

		for( int i=0; i<3; i++ )
			if( tmin[i] > v[10+i] )
			{
				tmin[i] = v[10+i];
				minmaxNames[0][i] = (char *)(((Ptg3DNode *)(elem/*->getOwner()*/))->getName());
			}
		for( i=0; i<3; i++ )
			if( tmax[i] < v[10+i] )
			{
				tmax[i] = v[10+i];
				minmaxNames[1][i] = (char *)(((Ptg3DNode *)(elem/*->getOwner()*/))->getName());
			}

		free_vector(v,1,12);
	}

	Ortho.left = tmin[0];
	Ortho.right = tmax[0];
	Ortho.top =  tmax[2];
	Ortho.bottom = tmin[2];
}

void CFotoView::OnGraphicsFitview() 
{
	double centroid[3];

	for( int i = 0; i < 3; i++ )
		centroid[i] = (tmax[i] + tmin[i]) / 2.0;

	theCamera.setPosition( centroid[0], -centroid[2], centroid[1] );
	theCamera.setLookDist( 10000.0 );
	theCamera.setLookAt( centroid[0], -centroid[2], centroid[1] );
#if FALSE
    theCamera.setPosition( (tmax[0]-tmin[0])/2+tmin[0], 
    						(tmax[1]-tmin[1])/2+tmin[1],
							(tmax[2]-tmin[2])/2+tmin[2] );
	// 
	theCamera.setLookDist( 10000.0 );
	theCamera.setLookAt( (tmax[0]-tmin[0])/2+tmin[0], 
    						(tmax[1]-tmin[1])/2+tmin[1],
							(tmax[2]-tmin[2])/2+tmin[2] );
	//
    theCamera.setClipping( -200000.0, 200000.0 );
#else if FALSE
    theCamera.setPosition( (tmax[0]-tmin[0])/2+tmin[0], 
    						-((tmax[2]-tmin[2])/2+tmin[2]),
							((tmax[1]-tmin[1])/2+tmin[1])
							 );
	// 
	theCamera.setLookDist( 10000.0 );
	theCamera.setLookAt( (tmax[0]-tmin[0])/2+tmin[0], 
    					-((tmax[2]-tmin[2])/2+tmin[2]),
						((tmax[1]-tmin[1])/2+tmin[1]));
	//
	/////////////////////////////////////
/*	MATRIX *m1 = matrix(1,3,1,3);
	MATRIX *m2 = matrix(1,3,1,3);
	
	
	(*m1)[1][1] = 1.0;
	(*m1)[1][2] = 0.0;
	(*m1)[1][3] = 0.0;
	(*m1)[2][1] = 0.0;
	(*m1)[2][2] = -cos(longitude);
	(*m1)[2][3] = sin(longitude);
	(*m1)[3][1] = 0.0;
	(*m1)[3][2] = -sin(longitude);
	(*m1)[3][3] = cos(longitude);

	(*m2)[1][1] = cos(latitude);
	(*m2)[1][2] = sin(latitude);
	(*m2)[1][3] = 0.0;
	(*m2)[2][1] = -sin(latitude);
	(*m2)[2][2] = cos(latitude);
	(*m2)[2][3] = 0.0;
	(*m2)[3][1] = 0.0;
	(*m2)[3][2] = 0.0;
	(*m2)[3][3] = 1.0;

	MATRIX *m3 = matrix_mult(m1,1,3,1,3,m2,1,3,1,3,1,3,1,3);
*/

	/////////////////////////////////////
    theCamera.setClipping( -200000.0, 200000.0 );
#endif
	regen_flag = TRUE;	
	InvalidateRect(NULL,FALSE);		
}

void CFotoView::OnGraphicsLight() 
{
	GLfloat v[4];

	CLightAdjust dlg;
	dlg.setLightValues(0.45,0.45,0.45,1.0);
	dlg.DoModal();
	dlg.getLightValues(&v[0],&v[1],&v[2],&v[3]);
	CDC *dc = GetDC();
	ASSERT(dc);
	wglMakeCurrent(dc->GetSafeHdc(), hglrc );

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
	regen_flag = TRUE;	
	InvalidateRect(NULL,FALSE);		
	wglMakeCurrent(NULL,NULL);
	ReleaseDC(dc);

}

void CFotoView::OnGraphicsOpenglsettings() 
{
	COpenGLSettings dlg;
	CDC *dc = GetDC();
	ASSERT(dc);
	wglMakeCurrent(dc->GetSafeHdc(), hglrc );
	if( dlg.DoModal() == IDOK )
	{
		long value = dlg.getOpenGLBits();
		if( value & ALPHATEST )
			glEnable(GL_ALPHA_TEST);
		else
			glDisable(GL_ALPHA_TEST);
		if( value & AUTONORMAL )
			glEnable(GL_AUTO_NORMAL);
		else
			glDisable(GL_AUTO_NORMAL);
		if( value & BLEND )
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
		if( value & COLORMATERIAL )
			glEnable(GL_COLOR_MATERIAL);
		else
			glDisable(GL_COLOR_MATERIAL);
		if( value & CULLFACE )
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		if( value & DEPTHTEST )
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		if( value & DITHER )
			glEnable(GL_DITHER);
		else
			glDisable(GL_DITHER);
		if( value & FOG )
			glEnable(GL_FOG);
		else
			glDisable(GL_FOG);
		if( value & LIGHTING )
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		if( value & LINESMOOTH )
			glEnable(GL_LINE_SMOOTH);
		else
			glDisable(GL_LINE_SMOOTH);
		if( value & LINESTIPPLE )
			glEnable(GL_LINE_STIPPLE);
		else
			glDisable(GL_LINE_STIPPLE);
		if( value & LOGICOP )
			glEnable(GL_LOGIC_OP);
		else
			glDisable(GL_LOGIC_OP);
		if( value & NORMALIZE )
			glEnable(GL_NORMALIZE);
		else
			glDisable(GL_NORMALIZE);
		if( value & POINTSMOOTH )
			glEnable(GL_POINT_SMOOTH);
		else
			glDisable(GL_POINT_SMOOTH);
		if( value & POLYGONSMOOTH )
			glEnable(GL_POLYGON_SMOOTH);
		else
			glDisable(GL_POLYGON_SMOOTH);
		if( value & POLYGONSTIPPLE )
			glEnable(GL_POLYGON_STIPPLE);
		else
			glDisable(GL_POLYGON_STIPPLE);
		if( value & SCISSORTEST )
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
		if( value & STENCILTEST )
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		if( value & TEXTURE1D )
			glEnable(GL_TEXTURE_1D);
		else
			glDisable(GL_TEXTURE_1D);
		if( value & TEXTURE2D )
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
	}
	regen_flag = TRUE;	
	InvalidateRect(NULL,FALSE);		
	ReleaseDC(dc);
	wglMakeCurrent(NULL,NULL);
}

void CFotoView::regenView()
{
	regen_flag = TRUE;
	InvalidateRect(NULL,FALSE);
	CDC *dc = GetDC();
	ASSERT(dc);
	wglMakeCurrent(dc->GetSafeHdc(), hglrc );

	if( list_num != 0 )
		glDeleteLists(list_num,1);
	list_num = 0;
	wglMakeCurrent(NULL,NULL);
	ReleaseDC(dc);
}

BOOL CFotoView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;//CView::OnEraseBkgnd(pDC);
}
