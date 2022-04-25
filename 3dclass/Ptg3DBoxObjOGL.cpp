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

#include "Ptg3DBoxObj.h"

void drawbox( GLfloat glfDisplayMode, GLdouble xlen, GLdouble ylen, GLdouble zlen )
{
    GLdouble x0 = -xlen/2.0;
    GLdouble x1 = xlen/2.0;
    GLdouble y0 = -ylen/2.0;
    GLdouble y1 = ylen/2.0;
    GLdouble z0 = -zlen/2.0;
    GLdouble z1 = zlen/2.0;

    static GLdouble n[6][3] = {
		{-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] = {
		{ 0, 1, 2, 3 }, { 3, 2, 6, 7 }, { 7, 6, 5, 4 },
		{ 4, 5, 1, 0 }, { 5, 6, 2, 1 }, { 7, 4, 0, 3 }
    };
    GLdouble v[8][3], tmp;
    GLint i;

    if (x0 > x1) {
	tmp = x0; x0 = x1; x1 = tmp;
    }
    if (y0 > y1) {
	tmp = y0; y0 = y1; y1 = tmp; 
    }
    if (z0 > z1) {
	tmp = z0; z0 = z1; z1 = tmp; 
    }
    v[0][0] = v[1][0] = v[2][0] = v[3][0] = x0;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = x1;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = y0;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = y1;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = z0;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = z1;

    for (i = 0; i < 6; i++) {
		glBegin(glfDisplayMode);
		glNormal3dv(&n[i][0]);
		glVertex3dv(&v[faces[i][0]][0]);
		glNormal3dv(&n[i][0]);
		glVertex3dv(&v[faces[i][1]][0]);
		glNormal3dv(&n[i][0]);
		glVertex3dv(&v[faces[i][2]][0]);
		glNormal3dv(&n[i][0]);
		glVertex3dv(&v[faces[i][3]][0]);
		glEnd();
    }
}

void Ptg3DBoxObj::drawWireframe()
{
    if( xlen <= 0.0 || ylen <= 0.0 || zlen <= 0.0 )
	return;

    drawbox( GL_LINE_LOOP, xlen, ylen, zlen );
}

void Ptg3DBoxObj::drawSolid()
{
    if( xlen <= 0.0 || ylen <= 0.0 || zlen <= 0.0 )
	return;
	
    drawbox( GL_QUADS, xlen, ylen, zlen );
}
