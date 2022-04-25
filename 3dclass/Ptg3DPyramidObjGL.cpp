#ifdef WIN32
#include "stdafx.h"
#endif

#include <gl/gl.h>

void myv3d( double a, double b, double c )
{
    double dfCoors[3];
    
    dfCoors[0] = a;
    dfCoors[1] = b;
    dfCoors[2] = c;
    
    v3d( dfCoors );
}

#include "Ptg3DPyramidObj.h"
void Ptg3DPyramidObj::drawSolid()
{
}

void Ptg3DPyramidObj::drawWireframe()
{
    if( dfHeight <= 0.0 || dfBaseWidth < 0.0 || dfTopWidth < 0.0 ||
	    (dfBaseWidth == 0.0 && dfTopWidth == 0.0) )
	return;

    // Draw from top to base
    bgnline();
    myv3d( dfTopWidth/2.0, dfTopDepth/2.0, 0.0 );
    myv3d( dfBaseWidth/2.0+dfWidthOffset, dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    endline();
    
    bgnline();
    myv3d( -dfTopWidth/2.0, dfTopDepth/2.0, 0.0 );
    myv3d( -dfBaseWidth/2.0+dfWidthOffset, dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    endline();

    bgnline();
    myv3d( -dfTopWidth/2.0, -dfTopDepth/2.0, 0.0 );
    myv3d( -dfBaseWidth/2.0+dfWidthOffset, -dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    endline();

    bgnline();
    myv3d( dfTopWidth/2.0, -dfTopDepth/2.0, 0.0 );
    myv3d( dfBaseWidth/2.0+dfWidthOffset, -dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    endline();

    // Draw top
    bgnline();
    myv3d( dfTopWidth/2.0, dfTopDepth/2.0, 0.0 );
    myv3d( -dfTopWidth/2.0, dfTopDepth/2.0, 0.0 );
    endline();
    
    bgnline();
    myv3d( -dfTopWidth/2.0, dfTopDepth/2.0, 0.0 );
    myv3d( -dfTopWidth/2.0, -dfTopDepth/2.0, 0.0 );
    endline();

    bgnline();
    myv3d( -dfTopWidth/2.0, -dfTopDepth/2.0, 0.0 );
    myv3d( dfTopWidth/2.0, -dfTopDepth/2.0, 0.0 );
    endline();

    bgnline();
    myv3d( dfTopWidth/2.0, -dfTopDepth/2.0, 0.0 );
    myv3d( dfTopWidth/2.0, dfTopDepth/2.0, 0.0 );
    endline();

    // Draw base
    bgnline();
    myv3d( dfBaseWidth/2.0+dfWidthOffset, dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    myv3d( -dfBaseWidth/2.0+dfWidthOffset, dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    endline();
    
    bgnline();
    myv3d( -dfBaseWidth/2.0+dfWidthOffset, dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    myv3d( -dfBaseWidth/2.0+dfWidthOffset, -dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    endline();

    bgnline();
    myv3d( -dfBaseWidth/2.0+dfWidthOffset, -dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    myv3d( dfBaseWidth/2.0+dfWidthOffset, -dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    endline();

    bgnline();
    myv3d( dfBaseWidth/2.0+dfWidthOffset, -dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    myv3d( dfBaseWidth/2.0+dfWidthOffset, dfBaseDepth/2.0+dfDepthOffset, dfHeight );
    endline();

}
