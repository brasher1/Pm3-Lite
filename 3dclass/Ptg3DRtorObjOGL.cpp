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

#include "Ptg3DRtorObj.h"
#include "PtgSurface.h"

void drawRectTorus( GLfloat glfDisplayMode, GLfloat outerRadius, GLfloat innerRadius, 
	GLfloat height, GLfloat angle )
{
    GLfloat controlPnts[81*4];
    GLfloat *currKnots;
    int i, j;
    
    int sizeQ = 5;
    int sizeR;
    
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
	wj = 1;
	for( i = 0; i < sizeR; i++ )
	{
	    wi = i%2 ? cos(DEGRAD(phi)) : 1;

	    float x2 = 1.0, y2 = 1.0;
	    switch( j )
	    {
		case 2:
		case 3:
		    x2 = -1.0;
		    break;
	    }

	    switch( j )
	    {
		case 0:
		case 3:
		case 4:
		    y2 = -1.0;
		    break;
	    }

	    controlPnts[(i+j*sizeR)*4 + 0] =
		    (r*cos(DEGRAD(i*phi))*x2 + R*cos(DEGRAD(i*phi)) * wj);
	    controlPnts[(i+j*sizeR)*4 + 1] =
		    (r*sin(DEGRAD(i*phi))*x2 + R*sin(DEGRAD(i*phi)) * wj);
	    controlPnts[(i+j*sizeR)*4 + 2] = y2*height/2*wi;
	    controlPnts[(i+j*sizeR)*4 + 3] = wi*wj;
	}
    }

    GLUnurbsObj *rtorNurb = gluNewNurbsRenderer();
    gluNurbsProperty( rtorNurb, GLU_CULLING, GL_TRUE );
    gluNurbsProperty( rtorNurb, GLU_DISPLAY_MODE, glfDisplayMode );
//    gluNurbsProperty( rtorNurb, GLU_SAMPLING_METHOD, GLU_PARAMETRIC_ERROR );
//    gluNurbsProperty( rtorNurb, GLU_PARAMETRIC_ERROR, 1.0 );
    gluBeginSurface( rtorNurb );
    gluNurbsSurface( rtorNurb, sizeR + 3, currKnots, sizeQ + 2, vknots,
	    4, 4*sizeR, controlPnts, 3, 2, GL_MAP2_VERTEX_4 );
    gluEndSurface( rtorNurb );
    gluDeleteNurbsRenderer( rtorNurb );
}

void Ptg3DRtorObj::drawWireframe()
{
    if( outerRadius <= 0.0 || innerRadius < 0.0 || angle < 0.0 || height <= 0.0 )
	return;

    drawRectTorus( GLU_OUTLINE_PATCH, outerRadius, innerRadius, height, angle );
}

void Ptg3DRtorObj::drawSolid()
{
    if( outerRadius <= 0.0 || innerRadius < 0.0 || angle < 0.0 || height <= 0.0 )
	return;

    drawRectTorus( GLU_FILL, outerRadius, innerRadius, height, angle );
}
