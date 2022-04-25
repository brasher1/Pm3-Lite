#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>

#define XK_MISCELLANY
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <GL/GLwMDrawA.h>
#include <GL/gl.h>
#include <GL/glu.h>
extern "C" {
#include "aux.h"
}

#include "PtgCamera.h"
#include "Ptg3DLineObj.h"
#include "Ptg3DPyramidObj.h"
#include "Ptg3DBoxObj.h"
#include "Ptg3DCylinderObj.h"
#include "Ptg3DCtorObj.h"
#include "Ptg3DRtorObj.h"
#include "Ptg3DConeObj.h"
#include "Ptg3DFacetObj.h"

int outputLevel = 0;
int debugOutputLevel = 0;
jmp_buf env;

static String fallback_resources[] = {
    "*OpenGLTestFrame*shadowType: SHADOW_IN",
    "*OpenGLTestGLWidget*width: 300",
    "*OpenGLTestGLWidget*height: 300",
    "*OpenGLTestGLWidget*rgba: TRUE",
    "*OpenGLTestGLWidget*allocateBackground: TRUE",
//    "*OpenGLTestGLWidget*doublebuffer: TRUE", 
    NULL
};

GLXContext theGLXContext = NULL;

void initGLWindowCB( Widget wgt, XtPointer xtpClientData, XtPointer xtpCallData );
void exposeGLWindowCB( Widget wgt, XtPointer xtpClientData, XtPointer xtpCallData );
void resizeGLWindowCB( Widget wgt, XtPointer xtpClientData, GLwDrawingAreaCallbackStruct *gldacs);
void inputGLWindowCB( Widget wgt, XtPointer xtpClientData, GLwDrawingAreaCallbackStruct *gldacs);

extern void drawScene();
void doPicking( int nMouseX, int nMouseY );
Ptg3DNode *getPickedElement( PtgCamera *theCamera, Ptg3DNode *theStartNode, int nMouseX, int nMouseY );

int importReviewModel( char *pszReviewFile, Ptg3DNode *theParent );

PtgCamera *theCamera = NULL;
Ptg3DNode *theObjects = NULL;

char *pszReviewModel = NULL;

double latitude = 0.0;
double longitude = 0.0;

GLuint gliDrawlist = 0;

void addAxes( Ptg3DNode *theParent, GLfloat axisLength = 100.0 )
{
    Ptg3DNode *theAxes = new Ptg3DNode;
    theAxes->setDrawStyle( Ptg3DNode::DS_SOLIDS );
    Ptg3DLineObj *anAxis;
    anAxis = new Ptg3DLineObj;
    anAxis->setColor( 1.0, 0, 0, 0.4 );
    anAxis->setLength( axisLength );
    anAxis->setDirection( 1, 0, 0 );
    anAxis->setName( "X-axis" );
    theParent->addMember( anAxis );

    anAxis = new Ptg3DLineObj;
    anAxis->setColor( 0, 1.0, 0, 0.4 );
    anAxis->setLength( axisLength );
    anAxis->setDirection( 0, 1, 0 );
    anAxis->setName( "Y-axis" );
    theParent->addMember( anAxis );

    anAxis = new Ptg3DLineObj;
    anAxis->setColor( 0, 0, 1.0, 0.4 );
    anAxis->setLength( axisLength );
    anAxis->setDirection( 0, 0, 1 );
    anAxis->setName( "Z-axis" );
    theParent->addMember( anAxis );
}

void initCamera()
{
    theCamera = new PtgCamera;
    theCamera->setPosition( 300.0, 0.0, 1000.0 );
    theCamera->setClipping( 0.0, 10000.0 );
    theCamera->setOrthoClip( -500.0, 500.0, -500.0, 500.0 );
    theCamera->setLookAt( 1477002, 1744709, 40696 );
    theCamera->setLookDist( 10000 );

theCamera->setLookAt( 0, 0, 0 );
theCamera->setLookDist( 1000 );
}

void initElemList()
{
#if TRUE
    theObjects = new Ptg3DNode;
    theObjects->setColor( 1.0, 1.0, 1.0 );
    theObjects->setDrawStyle( Ptg3DNode::DS_SOLIDS );
//    theObjects->setDrawStyle( Ptg3DNode::DS_WIREFRAME );

    addAxes( theObjects, 500.0 );
    importReviewModel( pszReviewModel, theObjects );
#else
    VECTOR v = vector( 1, 12 );
    Ptg3DNode *theNode;

    theObjects = new Ptg3DNode;
    theObjects->setColor( 1.0, 1.0, 1.0 );
    theObjects->setDrawStyle( Ptg3DNode::DS_SOLIDS );
//    theObjects->setDrawStyle( Ptg3DNode::DS_WIREFRAME );

    addAxes( theObjects, 500.0 );

    v[1] = 1.0; v[2] = 0.0; v[3] = 0.0;
    v[4] = 0.0; v[5] = 1.0; v[6] = 0.0;
    v[7] = 0.0; v[8] = 0.0; v[9] = 1.0;
    v[10] = 0.0; v[11] = 0.0; v[12] = 0.0;
    
    v[10] = 0.0; v[11] = 0.0; v[12] = 0.0;
    Ptg3DCylinderObj *theCyli = new Ptg3DCylinderObj;
    theCyli->setHeight( 100.0 );
    theCyli->setRadius( 25.0 );
    theCyli->setName( "Cyli 1" );
    theCyli->setMatrix( v );
    theCyli->setColor( 1.0, 0.0, 0.0 );
//    theObjects->addMember( theCyli );

    v[10] = 300.0; v[11] = 0.0; v[12] = 0.0;
    Ptg3DPyramidObj *thePyra = new Ptg3DPyramidObj;
    thePyra->setColor( 0.5, 0.5, 0 );
    thePyra->setHeight( 200.0 );
    thePyra->setBaseDims( 100.0, 75.0 );
    thePyra->setTopDims( 0.0, 0.0 );
    thePyra->setOffsets( 60.0, 10.0 );
    thePyra->setName( "Pyramid 1" );
    thePyra->setMatrix( v );
    thePyra->setColor( 0.0, 1.0, 0.0 );
//    theObjects->addMember( thePyra );

    v[10] = 0.0; v[11] = 300.0; v[12] = 0.0;
    Ptg3DConeObj *theCone = new Ptg3DConeObj;
    theCone->setTopRadius( 50.0 );
    theCone->setBottomRadius( 100.0 );
    theCone->setHeight( 400.0 );
    theCone->setXOffset( 200.0 );
    theCone->setYOffset( 75.0 );
    theCone->setName( "Cone 1" );
    theCone->setMatrix( v );
    theCone->setColor( 0.0, 0.0, 1.0 );
//    theObjects->addMember( theCone ); 

    v[10] = 0.0; v[11] = 0.0; v[12] = 250.0;
    Ptg3DCtorObj *theCtor = new Ptg3DCtorObj;
    theCtor->setOuterRadius( 100.0 );
    theCtor->setInnerRadius( 50.0 );
    theCtor->setAngle( 270.0 );
    theCtor->setName( "Ctor 1" );
    theCtor->setMatrix( v );
    theCtor->setColor( 1.0, 0.5, 0.5 );
//    theObjects->addMember( theCtor ); 

    v[10] = 0.0; v[11] = 250.0; v[12] = 250.0;
    Ptg3DRtorObj *theRtor = new Ptg3DRtorObj;
    theRtor->setOuterRadius( 100.0 );
    theRtor->setInnerRadius( 50.0 );
    theRtor->setHeight( 75.0 );
    theRtor->setAngle( 270.0 );
    theRtor->setName( "Rtor 1" );
    theRtor->setMatrix( v );
    theRtor->setColor( 0.5, 0.5, 1.0 );
//    theObjects->addMember( theRtor ); 

  // Add a test facet
    Ptg3DFacetObj *theFacet = new Ptg3DFacetObj;
    theFacet->setName( "Facet 1" );
    theFacet->setColor( 1.0, 0.0, 0.0 );
    
    Ptg3DFacetVertexObj aVertex;
    aVertex.setNormal( 1.0, 0.0, 0.0 );

    aVertex.setVertex( 0.0, 100.0, 100.0 );
    theFacet->addVertex( aVertex );
    aVertex.setVertex( 0.0, 600.0, 100.0 );
    theFacet->addVertex( aVertex );
    aVertex.setVertex( 0.0, 600.0, 400.0 );
    theFacet->addVertex( aVertex );
    aVertex.setVertex( 0.0, 500.0, 400.0 );
    theFacet->addVertex( aVertex );
    aVertex.setVertex( 0.0, 500.0, 200.0 );
    theFacet->addVertex( aVertex );
    aVertex.setVertex( 0.0, 200.0, 200.0 );
    theFacet->addVertex( aVertex );
    aVertex.setVertex( 0.0, 200.0, 400.0 );
    theFacet->addVertex( aVertex );
    aVertex.setVertex( 0.0, 100.0, 400.0 );
    theFacet->addVertex( aVertex );
    aVertex.setVertex( 0.0, 100.0, 100.0 );
    theFacet->addVertex( aVertex );

    theObjects->addMember( theFacet );
#endif
}

void main( int argc, char *argv[] )
{
    if( argc != 2 )
    {
	fprintf( stderr, "USAGE: testogl review_ascii_file_name\n" );
//	exit( 0 );
    }

    pszReviewModel = argv[1];

    initCamera();
    initElemList();

    Arg args[20];
    int nargs;
    XtAppContext theAppContext;

    Widget theTopLevelWidget = XtAppInitialize(
	    &theAppContext, 
	    "OpenGLTest", 
	    (XrmOptionDescList)NULL, 
	    (Cardinal)0, 
	    (int *)&argc, 
	    (String *)argv, 
	    fallback_resources, 
	    (ArgList)NULL, 
	    0 );

    nargs = 0;
    Widget theFormWidget = XmCreateForm( theTopLevelWidget, "OpenGLTestForm", args, nargs );
    XtManageChild( theFormWidget );
    
    nargs = 0;
    XtSetArg( args[nargs], XtNx, 30 ); nargs++;
    XtSetArg( args[nargs], XtNy, 30 ); nargs++;
    XtSetArg( args[nargs], XmNbottomAttachment, XmATTACH_FORM ); nargs++;
    XtSetArg( args[nargs], XmNtopAttachment, XmATTACH_FORM ); nargs++;
    XtSetArg( args[nargs], XmNleftAttachment, XmATTACH_FORM ); nargs++;
    XtSetArg( args[nargs], XmNrightAttachment, XmATTACH_FORM ); nargs++;
    XtSetArg( args[nargs], XmNleftOffset, 30 ); nargs++;
    XtSetArg( args[nargs], XmNtopOffset, 30 ); nargs++;
    XtSetArg( args[nargs], XmNbottomOffset, 30 ); nargs++;
    XtSetArg( args[nargs], XmNrightOffset, 30 ); nargs++;
    Widget theFrameWidget = XmCreateFrame( theFormWidget, "OpenGLTestFrame", args, nargs );
    XtManageChild( theFrameWidget );

    nargs = 0;
    XtSetArg( args[nargs], GLwNrgba, TRUE ); nargs++;
    XtSetArg( args[nargs], GLwNallocateBackground, TRUE ); nargs++;
    XtSetArg( args[nargs], GLwNdepthSize, 24 ); nargs++;
    XtSetArg( args[nargs], XmNwidth, 300 ); nargs++;
    XtSetArg( args[nargs], XmNheight, 300 ); nargs++;
    Widget theGLWidget = GLwCreateMDrawingArea( theFrameWidget, "OpenGLTestGLWidget", args, nargs);
    XtManageChild( theGLWidget );
    XtAddCallback( theGLWidget, GLwNexposeCallback, (XtCallbackProc)exposeGLWindowCB, 0 );
    XtAddCallback( theGLWidget, GLwNresizeCallback, (XtCallbackProc)resizeGLWindowCB, 0 );
    XtAddCallback( theGLWidget, GLwNginitCallback, (XtCallbackProc)initGLWindowCB, 0 );
    XtAddCallback( theGLWidget, GLwNinputCallback, (XtCallbackProc)inputGLWindowCB, 0 );

    XtRealizeWidget( theTopLevelWidget );

    XtAppMainLoop( theAppContext );
}

void initGLWindowCB( Widget wgt, XtPointer xtpClientData, XtPointer xtpCallData )
{
    Arg args[1];
    XVisualInfo *vi;

    XtSetArg( args[0], GLwNvisualInfo, &vi );
    XtGetValues( wgt, args, 1 );

    theGLXContext = glXCreateContext( XtDisplay(wgt), vi, 0, GL_FALSE );
    GLwDrawingAreaMakeCurrent( wgt, theGLXContext );
}

void exposeGLWindowCB( Widget wgt, XtPointer xtpClientData, XtPointer xtpCallData )
{
    GLwDrawingAreaMakeCurrent( wgt, theGLXContext );
    drawScene();
}

void resizeGLWindowCB( Widget wgt, XtPointer xtpClientData, GLwDrawingAreaCallbackStruct *gldacs)
{
    GLwDrawingAreaMakeCurrent( wgt, theGLXContext );
    glViewport(0, 0, (GLint)gldacs->width-1, (GLint)gldacs->height-1);
    theCamera->setAspectRatio( ((GLint)gldacs->width-1) / ((GLint)gldacs->height-1) );
}

void inputGLWindowCB( Widget wgt, XtPointer xtpClientData, GLwDrawingAreaCallbackStruct *gldacs)
{
    char buffer[1];
    KeySym keysym;

    GLwDrawingAreaMakeCurrent( wgt, theGLXContext );
    switch(gldacs->event->type)
    {
	case KeyRelease:
	    /* It is necessary to convert the keycode to a keysym before
	     * it is possible to check if it is an escape
	     */
	    keysym = XLookupKeysym( &(gldacs->event->xkey), 0 );
	     
	    switch( keysym )
	    {
		case XK_Up:
		    if( ((XKeyEvent *)(gldacs->event))->state & ControlMask )
			latitude -= 30.0;
		    else
			latitude -= 5.0;   	
		    theCamera->setRotation( 0.0, latitude, longitude );
		    fprintf( stderr, "latitude, longitude = %lf, %lf\n", latitude, longitude );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;

		case XK_Down:
		    if( ((XKeyEvent *)(gldacs->event))->state & ControlMask )
			latitude += 30.0;
		    else
			latitude += 5.0;
		    theCamera->setRotation( 0.0, latitude, longitude );
		    fprintf( stderr, "latitude, longitude = %lf, %lf\n", latitude, longitude );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;

		case XK_Left:
		    if( ((XKeyEvent *)(gldacs->event))->state & ControlMask )
			longitude += 30.0;
		    else
			longitude += 5.0;
		    theCamera->setRotation( 0.0, latitude, longitude );
		    fprintf( stderr, "latitude, longitude = %lf, %lf\n", latitude, longitude );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;

		case XK_Right:
		    if( ((XKeyEvent *)(gldacs->event))->state & ControlMask )
			longitude -= 30.0;
		    else
			longitude -= 5.0;
		    theCamera->setRotation( 0.0, latitude, longitude );
		    fprintf( stderr, "latitude, longitude = %lf, %lf\n", latitude, longitude );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
	    
		case XK_w:
		    if( gliDrawlist != 0 )
		    {
			glDeleteLists( gliDrawlist, 1 );
			gliDrawlist = 0;
		    }
		    theObjects->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
		
		case XK_s:
		    if( gliDrawlist != 0 )
		    {
			glDeleteLists( gliDrawlist, 1 );
			gliDrawlist = 0;
		    }
		    theObjects->setDrawStyle( Ptg3DNode::DS_SOLIDS );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
		
	    
		case XK_Escape:
		    fprintf( stderr, "Cleaning up object list.\n" );
    
		    delete theCamera;
		    delete theObjects;
    
		    fprintf( stderr, "Finished cleaning up object list.\n" );
		    exit(0);
		case XK_p:
		    theCamera->setViewType( PtgCamera::VT_PERSPECTIVE );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
		case XK_o:
		    theCamera->setViewType( PtgCamera::VT_ORTHO );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
		case XK_f:
		case XK_g:
		{
		    static double left = -500.0;
		    static double right = 500.0;
		    static double bottom = -500.0;
		    static double top = 500.0;
		    if( keysym == XK_f )
		    {
			left *= 2.0; right *= 2.0; bottom *= 2.0; top *= 2.0;
		    }
		    else
		    {
			left /= 2.0; right /= 2.0; bottom /= 2.0; top /= 2.0;
		    }

		    fprintf( stderr, "setOrthoClip( %lf, %lf, %lf, %lf )\n", left, right, bottom, top );
		    theCamera->setOrthoClip( left, right, bottom, top );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
		}
	    }
	    break;

	case ButtonRelease:
//	    thePrimitives->setVisible( !(thePrimitives.isVisible()) );
	    switch( gldacs->event->xbutton.button )
	    {
		case Button1:
		    fprintf( stderr, "mouse x, y = %d, %d\n", 
			    gldacs->event->xbutton.x, gldacs->event->xbutton.y );
		    doPicking( gldacs->event->xbutton.x,
			    gldacs->event->xbutton.y );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
		case Button2:
		    if( theCamera->getLookDist() > 50.0 )
			theCamera->setLookDist( theCamera->getLookDist() - 3000.0 );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
		case Button3:
		    theCamera->setLookDist( theCamera->getLookDist() + 3000.0 );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
	    }
	    break;
    }
}

void doPicking( int nMouseX, int nMouseY )
{
    Ptg3DNode *theSelectedNode = getPickedElement( theCamera, theObjects, nMouseX, nMouseY );
    if( theSelectedNode != NULL )
    {
	fprintf( stderr, "You selected '%s'\n", theSelectedNode->getName() );
	VECTOR v = theSelectedNode->getMatrix( theObjects );
	if( v == NULL )
	    theCamera->setLookAt( 0.0, 0.0, 0.0 );
	else
	    theCamera->setLookAt( v[10], v[11], v[12] );
	free_vector( v, 1, 12 );
    }
    else
	fprintf( stderr, "You selected nothing!.\n" );
}

#define BUFSIZE 512
Ptg3DNode *getPickedElement( PtgCamera *theCamera, Ptg3DNode *theStartNode,
	int nMouseX, int nMouseY )
{
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];
    
    glGetIntegerv( GL_VIEWPORT, viewport );
    glSelectBuffer( BUFSIZE, selectBuf );
    glRenderMode( GL_SELECT );
    
    glInitNames();
    glPushName( -1 );
    
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    
    /*  create 5x5 pixel picking region near cursor location	*/
    glMatrixMode( GL_PROJECTION );
    gluPickMatrix( (GLdouble) nMouseX, (GLdouble) (viewport[3] - nMouseY), 5.0, 5.0, viewport );
    glMatrixMode( GL_MODELVIEW );
    theCamera->draw();
    theStartNode->draw();

    glPopMatrix ();
    glFlush ();

    hits = glRenderMode( GL_RENDER );

    fprintf( stderr, "hits = %d\n", hits );

    GLuint *ptr = (GLuint *)selectBuf;
    for( int i = 0; i < hits; i++ )
    {
	GLuint nNumNames = *ptr;
	GLuint *nIDS = ptr+3;
	ptr+= nNumNames+3;

	return( theStartNode->findByID( nNumNames-2, nIDS+2 ) );
    }
    
    return( NULL );
}

void drawScene()
{
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_COLOR_MATERIAL );
//    glEnable( GL_AUTO_NORMAL );
//    glEnable( GL_CULL_FACE );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, 2 );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT );
    GLfloat v[4] = { 0.3, 0.3, 0.3, 1.0 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, v );

    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );

    theCamera->draw();
fprintf( stderr, "drawScene(): Before calling theObjects->draw()\n" );
    theObjects->draw();
//    glCallList( gliDrawlist );
fprintf( stderr, "drawScene(): After calling theObjects->draw()\n" );

    glPopMatrix();
    
    glFlush();
}
