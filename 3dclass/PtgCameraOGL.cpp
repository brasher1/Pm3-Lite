#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

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

    glTranslated( 0.0, 0.0, -lookdist );
    glRotated(-beta, 1.0, 0.0, 0.0);
    glRotated(-alpha, 0.0, 1.0, 0.0);
    glRotated(gamma, 0.0, 0.0, 1.0);	 
    glTranslated( -lookx, -looky, -lookz);

//    glTranslated( -transform[10], -transform[11], -transform[12] );
//    gluLookAt( -transform[10], -transform[11], -transform[12], 
//	    transform[1]*1000.0, transform[2]*1000.0, transform[3]*1000.0, 
//	    transform[4], transform[5], transform[6] );
}

void PtgCamera::drawOrtho()
{
    fprintf( stderr, "Drawing ortho camera.\n" );
    glMatrixMode( GL_PROJECTION );
    glOrtho( leftClip, rightClip, bottomClip, topClip, nearClipDist, farClipDist );
    glMatrixMode( GL_MODELVIEW );
}

void PtgCamera::drawPerspective()
{
    fprintf( stderr, "Drawing perspective camera.\n" );
    glMatrixMode( GL_PROJECTION );
    gluPerspective( fieldOfViewY, aspectRatio, nearClipDist, farClipDist );
    glMatrixMode( GL_MODELVIEW );
}
