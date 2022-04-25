#ifdef WIN32
#include "stdafx.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef WIN32
#include <GL/glaux.h>
#else
extern "C" {
#include <aux.h>
}
#endif

#include "Ptg3DConeObj.h"
#include "PtgSurface.h"

void nurbsDrawCone( GLfloat glfDisplayMode, GLfloat bottomRadius, GLfloat topRadius, 
	GLfloat height, GLfloat xOffset, GLfloat yOffset )
{
    GLfloat controlPnts[81*4];
    GLfloat *currKnots;
    int i, j;
    
    int sizeQ = 2;
    int sizeR = 9;
    
    double theta = 45;
    
    double radi[2];
    radi[0] = bottomRadius;
    radi[1] = topRadius;
    
    /* Establish the offsets for each base */
    double offsets[2][3];
    offsets[0][0] = 0.0;
    offsets[0][1] = 0.0;
    offsets[0][2] = -height/2.0;
    offsets[1][0] = xOffset;
    offsets[1][1] = yOffset;
    offsets[1][2] = height/2.0;
    
    double offsetDist = sqrt( offsets[1][0]*offsets[1][0] + offsets[1][1]*offsets[1][1] );
    double alpha = atan( (offsetDist - topRadius) / offsets[1][2] );
    double beta = atan( (offsetDist + topRadius) / offsets[1][2] );
    
    double wi, wj;
    
    for( j = 0; j < sizeQ; j++ )
    {
	for( i = 0; i < sizeR; i++ )
	{
	    wi = i%2 ? cos(DEGRAD(theta)) : 1;
	    float x2 = 1.0, y2 = 1.0;

	    switch( i )
	    {
		case 3:
		case 4:
		case 5:
		    x2 = -1.0;
		    break;
		case 2:
		case 6:
		    x2 = 0.0;
		    break;
	    }
	    switch( i )
	    {
		case 0:
		case 4:
		case 8:
		    y2 = 0;
		    break;
		case 5:
		case 6:
		case 7:
		    y2 = -1;
		    break;
	    }
	    controlPnts[(i+j*sizeR)*4 + 0] = ((radi[j]*x2) + offsets[j][0])*wi;
	    controlPnts[(i+j*sizeR)*4 + 1] = ((radi[j]*y2) + offsets[j][1])*wi;
	    controlPnts[(i+j*sizeR)*4 + 2] = (offsets[j][2])*wi;
	    controlPnts[(i+j*sizeR)*4 + 3] = wi;
	}
    }

//#if FALSE
    GLUnurbsObj *coneNurb  = gluNewNurbsRenderer();
    gluNurbsProperty( coneNurb, GLU_DISPLAY_MODE, glfDisplayMode );
    gluNurbsProperty( coneNurb, GLU_CULLING, GL_TRUE );
//    gluNurbsProperty( coneNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE );
//    gluNurbsProperty( coneNurb, GLU_U_STEP, 500 );
//    gluNurbsProperty( coneNurb, GLU_V_STEP, 500 );
    gluBeginSurface( coneNurb );
    gluNurbsSurface( coneNurb, 12, knots1, 4, knots5,
	    4, 4*9, controlPnts, 3, 2, GL_MAP2_VERTEX_4 );
    gluEndSurface( coneNurb );
    gluDeleteNurbsRenderer( coneNurb );
//#endif
}

extern void drawCircle( GLfloat glfDisplayMode, GLfloat radius );

void drawCone( GLfloat glfDisplayMode, GLfloat bottomRadius, GLfloat topRadius, 
	GLfloat height, GLfloat xOffset, GLfloat yOffset )
{
    glPushMatrix();
    
    double theSkewMatrix[16] = {
	1.0, 0.0, 0.0, 0.0, 
	0.0, 1.0, 0.0, 0.0, 
	0.0, 0.0, 1.0, 0.0, 
	0.0, 0.0, 0.0, 1.0, 
    };

    theSkewMatrix[8] = xOffset/height;
    theSkewMatrix[9] = yOffset/height;

    theSkewMatrix[12] = -xOffset/2.0;
    theSkewMatrix[13] = -yOffset/2.0;
    theSkewMatrix[14] = -height/2.0;

    glMultMatrixd( theSkewMatrix );

    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricDrawStyle( quadObj, glfDisplayMode );
    gluCylinder( quadObj, bottomRadius, topRadius, height, 12, 1 );
    gluDeleteQuadric( quadObj );

    if( glfDisplayMode != GLU_LINE )
    {
	glRotated( 180.0, 1.0, 0.0, 0.0 );
	drawCircle( glfDisplayMode, bottomRadius );
	glRotated( 180.0, 1.0, 0.0, 0.0 );
	glTranslatef( 0.0, 0.0, height );
	drawCircle( glfDisplayMode, topRadius );
    }
    glPopMatrix();
}

void Ptg3DConeObj::drawWireframe()
{
    if( height <= 0.0 || bottomRadius < 0.0 || topRadius < 0.0 ||
	    (bottomRadius == 0.0 && topRadius == 0.0) )
	return;

    drawCone( GLU_LINE, bottomRadius, topRadius, height, xOffset, yOffset );
}

void Ptg3DConeObj::drawSolid()
{
    if( height <= 0.0 || bottomRadius < 0.0 || topRadius < 0.0 ||
	    (bottomRadius == 0.0 && topRadius == 0.0) )
	return;

    drawCone( GLU_FILL, bottomRadius, topRadius, height, xOffset, yOffset );
}
