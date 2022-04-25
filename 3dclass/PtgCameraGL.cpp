#ifdef WIN32
#include "stdafx.h"
#endif

#include <gl/gl.h>

#include "PtgCamera.h"

void PtgCamera::draw()
{
    switch( viewType )
    {
	case VT_ORTHO:
	    drawOrtho();
	    break;
	case VT_PERSPECTIVE:
	    drawPerspective();
	    break;
	default:
	    fprintf( stderr, "Unknown viewType\n" );
	    break;
    }

    translate( -transform[10], -transform[11], -transform[12] );
}

void PtgCamera::drawOrtho()
{
    fprintf( stderr, "Drawing ortho camera.\n" );
    ortho( leftClip, rightClip, bottomClip, topClip, nearClipDist, farClipDist );
}

void PtgCamera::drawPerspective()
{
    fprintf( stderr, "Drawing perspective camera.\n" );
    perspective( 40.0, aspectRatio, nearClipDist, farClipDist );
}
