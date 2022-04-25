#ifdef WIN32
#include "stdafx.h"
#endif

#include <GL/gl.h>

#include "Ptg3DPointObj.h"

void Ptg3DPointObj::drawWireframe()
{
	glEnable( GL_POINT_SMOOTH );
	glPointSize( 3.0 );

    glBegin( GL_POINTS );
//	glBegin( GL_LINES );

//	glVertex3d( 0.0, 0.0, 0.0 );
//	glVertex3d( vPosition[1], vPosition[2], vPosition[3] );

//	glVertex3d( 0.0, 0.0, 0.0 );
//	glVertex3d( vPosition[1], -vPosition[3], vPosition[2] );

	glVertex3d( 0.0, 0.0, 0.0 );
//	glVertex3d( vPosition[1], vPosition[3], -vPosition[2] );

    glEnd();
}

void Ptg3DPointObj::drawSolid()
{
    drawWireframe();
}
