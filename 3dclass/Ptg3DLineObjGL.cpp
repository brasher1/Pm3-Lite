#ifdef WIN32
#include "stdafx.h"
#endif

#include <gl/gl.h>

#include "Ptg3DLineObj.h"

void Ptg3DLineObj::drawWireframe()
{
    double dfEndpoint[3];
    
    for( int i = 0; i < 3; i++ )
	dfEndpoint[i] = vOrigin[i+1] + dfLength*vDirection[i+1];
fprintf( stderr, "drawing line from %lf, %lf, %lf to %lf, %lf, %lf\n", 
	vOrigin[1], vOrigin[2], vOrigin[3],
	dfEndpoint[0], dfEndpoint[1], dfEndpoint[2] );
    bgnline();
    v3d( &(vOrigin[1]) );
    v3d( dfEndpoint );
    endline();
}

void Ptg3DLineObj::drawSolid()
{
    drawWireframe();
}
