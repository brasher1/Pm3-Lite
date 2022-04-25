#ifdef WIN32
#include "stdafx.h"
#endif

#include <gl/gl.h>

#include "Ptg3DNode.h"

void Ptg3DNode::draw()
{
    if( !isVisible() )
	return;

    short oldColors[4];
    
    pushmatrix();
    pushname( nMyUniqID );
    gRGBcolor( &oldColors[0], &oldColors[1], &oldColors[2] );

    RGBcolor( (short)(rgbColors[0]*255), (short)(rgbColors[1]*255),
	    (short)(rgbColors[2]*255) );
fprintf( stderr, "setting color to %hd, %hd, %hd\n", (short)(rgbColors[0]*255),
	(short)(rgbColors[1]*255), (short)(rgbColors[2]*255) );
    transMatrix.draw();

    switch( getDrawStyle() )
    {
	case DS_SOLIDS:
	    drawSolid();
	    break;

	case DS_WIREFRAME:
	case DS_NONE:
	    drawWireframe();
	    break;
	    
	default:
	    fprintf( stderr, "Cannot draw in mode %d currently.\n", getDrawStyle() );
	    break;
    }

    POSITION pos;
    for( pos = getMemberList().GetHeadPosition(); pos != NULL; getMemberList().GetNext(pos) )
    {
	Ptg3DNode *anElem = (Ptg3DNode *)getMemberList().GetAt(pos);
	anElem->draw();
    }

    RGBcolor( oldColors[0], oldColors[1], oldColors[2] );

    popname();
    popmatrix();
}

void Ptg3DNode::drawWireframe()
{
}

void Ptg3DNode::drawSolid()
{
}
