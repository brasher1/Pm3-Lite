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

#include "Ptg3DCtorObj.h"
#include "PtgSurface.h"

#ifndef CALLBACK
#define CALLBACK
#endif

void CALLBACK cyliErrorCB( GLenum error )
{
    fprintf( stderr, "Error nurbing cylinder(%d) - %s\n", error,
	    gluErrorString(error) );
}

void drawCircTorus( GLfloat glfDisplayMode, GLfloat outerRadius,
	GLfloat innerRadius, GLfloat angle )
{
    GLfloat controlPnts[81*4];
    GLfloat *currKnots;
    int i, j;
    
    int sizeQ = 9;
    int sizeR = 9;
    
    if( angle <= 90 && angle > 0 )
    {
	currKnots = knots4;
	sizeR = 3;
    }
    else if( angle <= 180 && angle > 90 )
    {
	currKnots = knots3;
	sizeR = 5;
    }
    else if( angle <= 270 && angle > 180 )
    {
	currKnots = knots2;
	sizeR = 7;
    }
    else if( angle <= 360 && angle > 270 )
    {
	currKnots = knots1;
	sizeR = 9;
    }

    double phi = angle / (sizeR - 1);
    double wi, wj;
    double theta = 45;

    double r = (outerRadius - innerRadius) / 2.0;
    double R = innerRadius + r;
    
    for( j = 0; j < sizeQ; j++ )
    {
	wj = j%2 ? cos(DEGRAD(theta)) : 1;
	for( i = 0; i < sizeR; i++ )
	{
	    wi = i%2 ? cos(DEGRAD(phi)) : 1;

	    controlPnts[(i+j*sizeR)*4 + 0] =
		    (r*cos(DEGRAD(j*theta)) * cos(DEGRAD(i*phi)) + R*cos(DEGRAD(i*phi)) * wj);
	    controlPnts[(i+j*sizeR)*4 + 1] =
		    (r*cos(DEGRAD(j*theta)) * sin(DEGRAD(i*phi)) + R*sin(DEGRAD(i*phi)) * wj);
	    controlPnts[(i+j*sizeR)*4 + 2] = r * sin(DEGRAD(j*theta)) * wi;
	    controlPnts[(i+j*sizeR)*4 + 3] = wi*wj;
	}
    }

    GLUnurbsObj *ctorNurb = gluNewNurbsRenderer();
    gluNurbsCallback( ctorNurb, GLU_ERROR, (void(CALLBACK *)())cyliErrorCB );
    gluNurbsProperty( ctorNurb, GLU_CULLING, GL_TRUE );
    gluNurbsProperty( ctorNurb, GLU_DISPLAY_MODE, glfDisplayMode );
//    gluNurbsProperty( ctorNurb, GLU_SAMPLING_METHOD, GLU_PARAMETRIC_ERROR );
//    gluNurbsProperty( ctorNurb, GLU_PARAMETRIC_ERROR, 1.0 );
    gluBeginSurface( ctorNurb );
    gluNurbsSurface( ctorNurb, sizeR + 3, currKnots, sizeQ + 3, knots1,
	    4, 4*sizeR, controlPnts, 3, 3, GL_MAP2_VERTEX_4 );
    gluEndSurface( ctorNurb );
    gluDeleteNurbsRenderer( ctorNurb );
}

void Ptg3DCtorObj::drawWireframe()
{
    if( outerRadius <= 0.0 || innerRadius < 0.0 || angle < 0.0 )
	return;

    drawCircTorus( GLU_OUTLINE_PATCH, outerRadius, innerRadius, angle );
}

void Ptg3DCtorObj::drawSolid()
{
    if( outerRadius <= 0.0 || innerRadius < 0.0 || angle < 0.0 )
	return;

    drawCircTorus( GLU_FILL, outerRadius, innerRadius, angle );
}
