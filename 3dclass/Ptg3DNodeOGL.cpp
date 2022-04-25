#ifdef WIN32
#include "stdafx.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "Ptg3DNode.h"

/*
 * The transformation stored in the Node class is from child Node to parent Node.
 * It should be noted that the stored matrix will be mapped directly to the OpenGL
 * matrix as defined by OpenGL (i.e. storedMatrix[1] = OpenGL[0],
 * storedMatrix[2] = OpenGL[1],  etc.).  The stored matrix transform
 * components( 10,11,12) are mapped to OpenGL elements 12, 13, 14.  This is the
 * transpose of the matrix ordering defined for other photogrammetry math operations.
 */

void Ptg3DNode::draw()
{
    if( !isActive() )
	return;

//static int nLevel = 0;
//fprintf( stderr, "%*.*s%s - %04x\n", 2*nLevel, 2*nLevel, " ", nodeName, getDrawStyle() );
//nLevel+=1;

//    GLfloat oldColors[4];
    GLfloat newColors[4];

    glPushMatrix();
    glPushName( nMyUniqID );
//    glGetFloatv( GL_CURRENT_COLOR, oldColors );
    glPushAttrib( GL_COLOR_BUFFER_BIT );

    getColor( newColors );
    glColor4fv( newColors );

    transMatrix.draw();

    switch( getDrawStyle() )
    {
	case DS_SOLIDS:
	    drawSolid();
	    break;

	case DS_WIREFRAME:
	    drawWireframe();
	    break;

	case DS_INVISIBLE:
	    break;

	default:
	    fprintf( stderr, "Cannot draw element %s in mode %d currently.\n", nodeName, getDrawStyle() );
	    break;
    }

    GLenum it;
    if( (it = glGetError()) != GL_NO_ERROR )
    {
	fprintf( stderr, "GLERROR - %d\n", it );
    }

    POSITION pos;
    if( !getMemberList().IsEmpty() )
    {
	for( pos = getMemberList().GetHeadPosition(); pos != NULL; getMemberList().GetNext(pos) )
	{
	    Ptg3DNode *anElem = (Ptg3DNode *)getMemberList().GetAt(pos);
	    anElem->draw();
	}
    }

    glPopAttrib();
//    glColor4fv( oldColors );

    glPopName();
    glPopMatrix();

//nLevel-=1;
}

void Ptg3DNode::drawWireframe()
{
}

void Ptg3DNode::drawSolid()
{
}
