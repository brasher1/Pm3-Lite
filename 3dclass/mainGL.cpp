#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <X11/keysym.h>
#include <X11/Xirisw/GlxMDraw.h>

#include <gl/gl.h>

#include "PtgCamera.h"
#include "Ptg3DLineObj.h"
#include "Ptg3DPyramidObj.h"
#include "Ptg3DBoxObj.h"
#include "Ptg3DCylinderObj.h"

int outputLevel = 0;
int debugOutputLevel = 0;
jmp_buf env;

static GLXconfig glxConfig[] = {
    { GLX_NORMAL, GLX_RGB, TRUE }, 
//    { GLX_NORMAL, GLX_DOUBLE, TRUE }, 
    { 0, 0, 0 }
};

static String fallback_resources[] = {
    "*GLTestFrame*shadowType: SHADOW_IN",
    "*GLTestGLWidget*width: 300",
    "*GLTestGLWidget*height: 300",
//    "*GLTestGLWidget*rgba: TRUE",
    "*GLTestGLWidget*allocateBackground: TRUE",
//    "*OpenGLTestGLWidget*doublebuffer: TRUE", 
    NULL
};

void initGLWindowCB( Widget wgt, XtPointer xtpClientData, XtPointer xtpCallData );
void exposeGLWindowCB( Widget wgt, XtPointer xtpClientData, XtPointer xtpCallData );
void resizeGLWindowCB( Widget wgt, XtPointer xtpClientData, GlxDrawCallbackStruct *gldacs);
void inputGLWindowCB( Widget wgt, XtPointer xtpClientData, GlxDrawCallbackStruct *gldacs);
extern void drawScene();
void doPicking( int nMouseX, int nMouseY );
Ptg3DNode *getPickedElement( PtgCamera *theCamera, Ptg3DNode *theStartNode,
	int nMouseX, int nMouseY );

PtgCamera *theCamera = NULL;
Ptg3DNode *theObjects = NULL;
Ptg3DNode *theAxes = NULL;
Ptg3DNode *theCameras = NULL;
Ptg3DNode *thePrimitives = NULL;

void loadidentity()
{
    Matrix m = {
	{ 1.0, 0.0, 0.0, 0.0 }, 
	{ 0.0, 1.0, 0.0, 0.0 }, 
	{ 0.0, 0.0, 1.0, 0.0 }, 
	{ 0.0, 0.0, 0.0, 1.0 }
    };
    
    loadmatrix( m );
}

void initElemList()
{
    theCamera = new PtgCamera;
    theObjects = new Ptg3DNode;
    theAxes = new Ptg3DNode;
    theCameras = new Ptg3DNode;
    thePrimitives = new Ptg3DNode;

    theCamera->setViewType( PtgCamera::VT_PERSPECTIVE );
    theCamera->setPosition( 300.0, 0.0, 1000.0 );
    theCamera->setClipping( 0.0, 100000.0 );
    theCamera->setOrthoClip( -500.0, 500.0, -500.0, 500.0 );

#if FALSE
    theObjects->setColor( 1.0, 0, 0 );
    theObjects->addMember( theAxes );
    theObjects->addMember( theCameras );
    theObjects->addMember( thePrimitives );

    // Add axes representation
    Ptg3DLineObj *aLine;
    aLine = new Ptg3DLineObj;
    aLine->setColor( 1.0, 0, 0 );
    aLine->setLength( 100.0 );
    aLine->setDirection( 1, 0, 0 );
    aLine->setName( "X-axis" );
    fprintf( stderr, "X-axis is ID = %d\n", aLine->getUniqID() );
    theAxes->addMember( aLine );

    aLine = new Ptg3DLineObj;
    aLine->setColor( 0, 1.0, 0 );
    aLine->setLength( 100.0 );
    aLine->setDirection( 0, 1, 0 );
    aLine->setName( "Y-axis" );
    fprintf( stderr, "Y-axis is ID = %d\n", aLine->getUniqID() );
    theAxes->addMember( aLine );

    aLine = new Ptg3DLineObj;
    aLine->setColor( 0, 0, 1.0 );
    aLine->setLength( 100.0 );
    aLine->setDirection( 0, 0, 1 );
    aLine->setName( "Z-axis" );
    fprintf( stderr, "Y-axis is ID = %d\n", aLine->getUniqID() );
    theAxes->addMember( aLine );

    // Add some primitives to theCameras
    VECTOR v = vector( 1, 12 );
    v[1] = 1.0;
    v[2] = 0.0;
    v[3] = 0.0;
    
    v[4] = 0.0;
    v[5] = 1.0;
    v[6] = 0.0;

    v[7] = 0.0;
    v[8] = 0.0;
    v[9] = 1.0;

    v[10] = 0.0;
    v[11] = 0.0;
    v[12] = 0.0;
    
    Ptg3DPyramidObj *thePyra = new Ptg3DPyramidObj;
    thePyra->setColor( 0.5, 0.5, 0 );
    thePyra->setHeight( 200.0 );
    thePyra->setBaseDims( 100.0, 75.0 );
    thePyra->setTopDims( 0.0, 0.0 );
    thePyra->setOffsets( 0.0, 0.0 );
    thePyra->setMatrix(v);
    thePyra->setName( "Pyramid 1" );
    theCameras->addMember( thePyra );

    v[10] = -300.0;
    thePyra = new Ptg3DPyramidObj;
    thePyra->setColor( 0, 0.5, 0.5 );
    thePyra->setHeight( 200.0 );
    thePyra->setBaseDims( 100.0, 75.0 );
    thePyra->setTopDims( 0.0, 0.0 );
    thePyra->setOffsets( 0.0, 0.0 );
    thePyra->setMatrix(v);
    thePyra->setName( "Pyramid 2" );
    theCameras->addMember( thePyra );

    // Add some primitives to thePrimitives
    v[1] = 1.0;
    v[2] = 0.0;
    v[3] = 0.0;
    
    v[4] = 0.0;
    v[5] = 1.0;
    v[6] = 0.0;

    v[7] = 0.0;
    v[8] = 0.0;
    v[9] = 1.0;

    v[10] = 0.0;
    v[11] = 0.0;
    v[12] = 600.0;

    Ptg3DBoxObj *theBox = new Ptg3DBoxObj;
    theBox->setColor( 0.5, 0.5, 0 );
    theBox->setXLen( 100.0 );
    theBox->setYLen( 200.0 );
    theBox->setZLen( 300.0 );
    theBox->setMatrix( v );
    theBox->setDrawStyle( Ptg3DNode::DS_SOLIDS );
    theBox->setName( "Box 1" );
    thePrimitives->addMember( theBox );

    theBox = new Ptg3DBoxObj;
    v[10] = 300.0;
    theBox->setColor( 0, 0.5, 0.5 );
    theBox->setXLen( 100.0 );
    theBox->setYLen( 200.0 );
    theBox->setZLen( 300.0 );
    theBox->setMatrix( v );
    theBox->setName( "Box 2" );
    thePrimitives->addMember( theBox );

    // Add some primitives to thePrimitives
    v[1] = 1.0;
    v[2] = 0.0;
    v[3] = 0.0;
    
    v[4] = 0.0;
    v[5] = 1.0;
    v[6] = 0.0;

    v[7] = 0.0;
    v[8] = 0.0;
    v[9] = 1.0;

    v[10] = 0.0;
    v[11] = -500.0;
    v[12] = 0.0;

    Ptg3DCylinderObj *theCyli = new Ptg3DCylinderObj;
    theCyli->setColor( 0.5, 0, 0.5 );
    theCyli->setRadius( 100.0 );
    theCyli->setHeight( 200.0 );
    theCyli->setMatrix( v );
    theCyli->setDrawStyle( Ptg3DNode::DS_SOLIDS );
    theCyli->setName( "Cyli 1" );
    thePrimitives->addMember( theCyli );

    theCyli = new Ptg3DCylinderObj;
    v[1] = 1.0;
    v[2] = 0.0;
    v[3] = 0.0;
    
    v[4] = 0.0;
    v[5] = 0.0;
    v[6] = 1.0;

    v[7] = 0.0;
    v[8] = -1.0;
    v[9] = 0.0;

    v[10] = 300.0;
    v[11] = 0.0;
    v[12] = 500.0;
    theCyli->setColor( 0.5, 0, 0.5 );
    theCyli->setRadius( 20.0 );
    theCyli->setHeight( 500.0 );
    theCyli->setMatrix( v );
    theCyli->setDrawStyle( Ptg3DNode::DS_WIREFRAME );
    theCyli->setName( "Cyli 2" );
    thePrimitives->addMember( theCyli );
#endif
}

void main( int argc, char *argv[] )
{
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
    XtSetArg( args[nargs], GlxNglxConfig, glxConfig ); nargs++;
    Widget theGLWidget = XtCreateManagedWidget( "GLTestGLWidget",
	glxMDrawWidgetClass, theFrameWidget, args, nargs );
    XtManageChild( theGLWidget );
    XtAddCallback( theGLWidget, GlxNexposeCallback, (XtCallbackProc)exposeGLWindowCB, 0 );
    XtAddCallback( theGLWidget, GlxNresizeCallback, (XtCallbackProc)resizeGLWindowCB, 0 );
    XtAddCallback( theGLWidget, GlxNginitCallback, (XtCallbackProc)initGLWindowCB, 0 );
    XtAddCallback( theGLWidget, GlxNinputCallback, (XtCallbackProc)inputGLWindowCB, 0 );

    XtRealizeWidget( theTopLevelWidget );

    XtAppMainLoop( theAppContext );
}

void initGLWindowCB( Widget wgt, XtPointer xtpClientData, XtPointer xtpCallData )
{
    GLXwinset( XtDisplay(wgt), XtWindow(wgt) );
}

void exposeGLWindowCB( Widget wgt, XtPointer xtpClientData, XtPointer xtpCallData )
{
    GLXwinset( XtDisplay(wgt), XtWindow(wgt) );

    drawScene();
}

void resizeGLWindowCB( Widget wgt, XtPointer xtpClientData,
	GlxDrawCallbackStruct *gldacs)
{
    GLXwinset( XtDisplay(wgt), XtWindow(wgt) );
    viewport( 0, (Screencoord) gldacs->width-1, 0, (Screencoord) gldacs->height-1 );

    theCamera->setAspectRatio( (gldacs->width-1) / (gldacs->height-1) );
}

void inputGLWindowCB( Widget wgt, XtPointer xtpClientData, GlxDrawCallbackStruct *gldacs)
{
    char buffer[1];
    KeySym keysym;


    switch(gldacs->event->type)
    {
	case KeyRelease:
	    /* It is necessary to convert the keycode to a keysym before
	     * it is possible to check if it is an escape
	     */
	    if( XLookupString((XKeyEvent *)gldacs->event, buffer, 1, &keysym, NULL ) == 1 )
	    {
		switch( keysym )
		{
		    case XK_Escape:
			fprintf( stderr, "Cleaning up object list.\n" );
	
			delete theCamera;
			delete theObjects;
	
			fprintf( stderr, "Finished cleaning up object list.\n" );
			exit(0);
		    case XK_P:
		    case XK_p:
			theCamera->setViewType( PtgCamera::VT_PERSPECTIVE );
			exposeGLWindowCB( wgt, NULL, NULL );
			break;
		    case XK_O:
		    case XK_o:
			theCamera->setViewType( PtgCamera::VT_ORTHO );
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
		    theCamera->movePosBy( 0.0, 0.0, -50.0 );
		    fprintf( stderr, "new camera pos = %lf, %lf, %lf\n", 
			    theCamera->getPosition()[0], 
			    theCamera->getPosition()[1], 
			    theCamera->getPosition()[2] );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
		case Button3:
		    theCamera->movePosBy( 0.0, 0.0, 50.0 );
		    fprintf( stderr, "new camera pos = %lf, %lf, %lf\n", 
			    theCamera->getPosition()[0], 
			    theCamera->getPosition()[1], 
			    theCamera->getPosition()[2] );
		    exposeGLWindowCB( wgt, NULL, NULL );
		    break;
	    }
	    break;
    }
}

void doPicking( int nMouseX, int nMouseY )
{
    Ptg3DNode *theSelectedNode = getPickedElement( theCamera, theObjects, 
	    nMouseX, nMouseY );
    if( theSelectedNode != NULL )
    {
	fprintf( stderr, "You selected '%s'\n", theSelectedNode->getName() );
	VECTOR v = theSelectedNode->getMatrix();
	theCamera->setPosition( v[10], v[11], theCamera->getPosition()[2] );
	free_vector( v, 1, 12 );
    }
    else
	fprintf( stderr, "You selected nothing!.\n" );
}

#define BUFSIZE 512
Ptg3DNode *getPickedElement( PtgCamera *theCamera, Ptg3DNode *theStartNode,
	int nMouseX, int nMouseY )
{
    Screencoord left, right, bottom, top;
    short selectBuf[BUFSIZE];
    long hits;
    
    getviewport( &left, &right, &bottom, &top );
    pick( selectBuf, BUFSIZE );
    initnames();
    pushname( -1 );
    pushmatrix();
    loadidentity();
    picksize( 5, 5 );

    theCamera->draw();
    theStartNode->draw();
    
    popmatrix();
    gflush();

    hits = endselect( selectBuf );
    fprintf( stderr, "hits = %ld\n", hits );

    short *ptr = (short *)selectBuf;
    for( int i = 0; i < hits; i++ )
    {
	short nNumNames = *ptr;
	short *nIDS = ptr+3;
	ptr+= nNumNames+3;

	return( theStartNode->findByID( nNumNames-2, (int *)(nIDS+2) ) );
    }
    
    return( NULL );
}

#if FALSE
void translateCamera( GLdouble x, GLdouble y, GLdouble z )
{
    glTranslated( -x, -y, -z );
}

void rotateCamera( GLdouble angle, GLdouble x, GLdouble y, GLdouble z )
{
    glRotated( -angle, x, y, z );
}
#endif

void drawScene()
{
    zbuffer( TRUE );
    zfunction( ZF_LEQUAL );
    RGBcolor( 0, 0, 0 );
    clear();
    
    pushmatrix();
    loadidentity();
    
    theCamera->draw();
    theObjects->draw();
    
    popmatrix();
    
    gflush();
}

