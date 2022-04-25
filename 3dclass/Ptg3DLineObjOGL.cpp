#ifdef WIN32
#include "stdafx.h"
#endif

#include <GL/gl.h>

#include "Ptg3DLineObj.h"

void Ptg3DLineObj::drawWireframe()
{
    glBegin( GL_LINE_STRIP );

    glVertex3d( vOrigin[1], vOrigin[2], vOrigin[3] );
    glVertex3d( vOrigin[1] + dfLength*vDirection[1],
		vOrigin[2] + dfLength*vDirection[2], 
		vOrigin[3] + dfLength*vDirection[3] );
    glEnd();
}

void Ptg3DLineObj::drawSolid()
{
    drawWireframe();
}
