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

#include "Ptg3DCylinderObj.h"
#include "PtgSurface.h"

static GLuint gliCylinderDrawList = 0;

/* Draws a circle in the x-y plane with it's normal pointing in the +z direction */
void drawCircle( GLfloat glfDisplayMode, GLfloat radius )
{
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricDrawStyle( quadObj, glfDisplayMode );
    gluDisk( quadObj, 0.0, radius, 12, 1 );
    gluDeleteQuadric( quadObj );
}

void drawcylinder( GLfloat glfDisplayMode, GLdouble radius, GLdouble height )
{
	static int haveBeenHere = FALSE;
	GLUquadricObj *quadObj = NULL;

	glPushMatrix();

	quadObj = gluNewQuadric();
	gluQuadricDrawStyle( quadObj, glfDisplayMode );
	gluCylinder( quadObj, radius, radius, height, 12, 1 );
	gluDeleteQuadric( quadObj );

	if( glfDisplayMode != GLU_LINE )
	{
		glRotatef( 180.0, 1.0, 0.0, 0.0 );
		drawCircle( glfDisplayMode, radius );
		glRotatef( 180.0, 1.0, 0.0, 0.0 );
		glTranslatef( 0.0, 0.0, height );
		drawCircle( glfDisplayMode, radius );
	}

	glPopMatrix();
}

void Ptg3DCylinderObj::drawWireframe()
{
    if( height <= 0.0 || radius <= 0.0 )
	return;
	
    glTranslated( 0.0, 0.0, -height/2.0 );
    drawcylinder( GLU_LINE, radius, height );
}

void Ptg3DCylinderObj::drawSolid()
{
    if( height <= 0.0 || radius <= 0.0 )
	return;
	
    glTranslated( 0.0, 0.0, -height/2.0  );
    drawcylinder( GLU_FILL, radius, height );
}
