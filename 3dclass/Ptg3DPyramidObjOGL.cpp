#ifdef WIN32
#include "stdafx.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "Ptg3DPyramidObj.h"
void Ptg3DPyramidObj::drawSolid()
{
    double dfHalfBaseWidth = dfBaseWidth/2.0;
    double dfHalfBaseDepth = dfBaseDepth/2.0;
    double dfHalfTopWidth = dfTopWidth/2.0;
    double dfHalfTopDepth = dfTopDepth/2.0;
    double dfHalfHeight = dfHeight/2.0;
    double dfHalfDepthOffset = dfDepthOffset/2.0;
    double dfHalfWidthOffset = dfWidthOffset/2.0;

    GLfloat v[2][4][3];

    v[0][0][0] = -dfHalfWidthOffset+dfHalfBaseWidth;
    v[0][0][1] = -dfHalfDepthOffset-dfHalfBaseDepth;
    v[0][0][2] = -dfHalfHeight;

    v[0][1][0] = -dfHalfWidthOffset+dfHalfBaseWidth;
    v[0][1][1] = -dfHalfDepthOffset+dfHalfBaseDepth;
    v[0][1][2] = -dfHalfHeight;

    v[0][2][0] = -dfHalfWidthOffset-dfHalfBaseWidth;
    v[0][2][1] = -dfHalfDepthOffset+dfHalfBaseDepth;
    v[0][2][2] = -dfHalfHeight;

    v[0][3][0] = -dfHalfWidthOffset-dfHalfBaseWidth;
    v[0][3][1] = -dfHalfDepthOffset-dfHalfBaseDepth;
    v[0][3][2] = -dfHalfHeight;


    v[1][0][0] = dfHalfWidthOffset+dfHalfTopWidth;
    v[1][0][1] = dfHalfDepthOffset-dfHalfTopDepth;
    v[1][0][2] = dfHalfHeight;

    v[1][1][0] = dfHalfWidthOffset+dfHalfTopWidth;
    v[1][1][1] = dfHalfDepthOffset+dfHalfTopDepth;
    v[1][1][2] = dfHalfHeight;

    v[1][2][0] = dfHalfWidthOffset-dfHalfTopWidth;
    v[1][2][1] = dfHalfDepthOffset+dfHalfTopDepth;
    v[1][2][2] = dfHalfHeight;

    v[1][3][0] = dfHalfWidthOffset-dfHalfTopWidth;
    v[1][3][1] = dfHalfDepthOffset-dfHalfTopDepth;
    v[1][3][2] = dfHalfHeight;

    // Draw base
    glBegin( GL_QUADS );
    glVertex3d( v[0][0][0], v[0][0][1], v[0][0][2] );
    glVertex3d( v[0][1][0], v[0][1][1], v[0][1][2] );
    glVertex3d( v[0][2][0], v[0][2][1], v[0][2][2] );
    glVertex3d( v[0][3][0], v[0][3][1], v[0][3][2] );
//    glEnd();

    // Draw top
//    glBegin( GL_QUADS );
    glVertex3d( v[1][0][0], v[1][0][1], v[1][0][2] );
    glVertex3d( v[1][1][0], v[1][1][1], v[1][1][2] );
    glVertex3d( v[1][2][0], v[1][2][1], v[1][2][2] );
    glVertex3d( v[1][3][0], v[1][3][1], v[1][3][2] );
//    glEnd();

//    glBegin( GL_QUADS );
    glVertex3d( v[0][1][0], v[0][1][1], v[0][1][2] );
    glVertex3d( v[1][1][0], v[1][1][1], v[1][1][2] );
    glVertex3d( v[1][2][0], v[1][2][1], v[1][2][2] );
    glVertex3d( v[0][2][0], v[0][2][1], v[0][2][2] );
//    glEnd();

//    glBegin( GL_QUADS );
    glVertex3d( v[0][0][0], v[0][0][1], v[0][0][2] );
    glVertex3d( v[1][0][0], v[1][0][1], v[1][0][2] );
    glVertex3d( v[1][1][0], v[1][1][1], v[1][1][2] );
    glVertex3d( v[0][1][0], v[0][1][1], v[0][1][2] );
//    glEnd();

//    glBegin( GL_QUADS );
    glVertex3d( v[0][2][0], v[0][2][1], v[0][2][2] );
    glVertex3d( v[1][2][0], v[1][2][1], v[1][2][2] );
    glVertex3d( v[1][3][0], v[1][3][1], v[1][3][2] );
    glVertex3d( v[0][3][0], v[0][3][1], v[0][3][2] );
//    glEnd();

//    glBegin( GL_QUADS );
    glVertex3d( v[0][3][0], v[0][3][1], v[0][3][2] );
    glVertex3d( v[1][3][0], v[1][3][1], v[1][3][2] );
    glVertex3d( v[1][0][0], v[1][0][1], v[1][0][2] );
    glVertex3d( v[0][0][0], v[0][0][1], v[0][0][2] );
    glEnd();
}

void Ptg3DPyramidObj::drawWireframe()
{
    double dfHalfBaseWidth = dfBaseWidth/2.0;
    double dfHalfBaseDepth = dfBaseDepth/2.0;
    double dfHalfTopWidth = dfTopWidth/2.0;
    double dfHalfTopDepth = dfTopDepth/2.0;
    double dfHalfHeight = dfHeight/2.0;
    double dfHalfDepthOffset = dfDepthOffset/2.0;
    double dfHalfWidthOffset = dfWidthOffset/2.0;

    GLfloat v[2][4][3];

    v[0][0][0] = -dfHalfWidthOffset+dfHalfBaseWidth;
    v[0][0][1] = -dfHalfDepthOffset-dfHalfBaseDepth;
    v[0][0][2] = -dfHalfHeight;

    v[0][1][0] = -dfHalfWidthOffset+dfHalfBaseWidth;
    v[0][1][1] = -dfHalfDepthOffset+dfHalfBaseDepth;
    v[0][1][2] = -dfHalfHeight;

    v[0][2][0] = -dfHalfWidthOffset-dfHalfBaseWidth;
    v[0][2][1] = -dfHalfDepthOffset+dfHalfBaseDepth;
    v[0][2][2] = -dfHalfHeight;

    v[0][3][0] = -dfHalfWidthOffset-dfHalfBaseWidth;
    v[0][3][1] = -dfHalfDepthOffset-dfHalfBaseDepth;
    v[0][3][2] = -dfHalfHeight;


    v[1][0][0] = dfHalfWidthOffset+dfHalfTopWidth;
    v[1][0][1] = dfHalfDepthOffset-dfHalfTopDepth;
    v[1][0][2] = dfHalfHeight;

    v[1][1][0] = dfHalfWidthOffset+dfHalfTopWidth;
    v[1][1][1] = dfHalfDepthOffset+dfHalfTopDepth;
    v[1][1][2] = dfHalfHeight;

    v[1][2][0] = dfHalfWidthOffset-dfHalfTopWidth;
    v[1][2][1] = dfHalfDepthOffset+dfHalfTopDepth;
    v[1][2][2] = dfHalfHeight;

    v[1][3][0] = dfHalfWidthOffset-dfHalfTopWidth;
    v[1][3][1] = dfHalfDepthOffset-dfHalfTopDepth;
    v[1][3][2] = dfHalfHeight;

    // Draw base
    glBegin( GL_LINE_LOOP );
    glVertex3d( v[0][0][0], v[0][0][1], v[0][0][2] );
    glVertex3d( v[0][1][0], v[0][1][1], v[0][1][2] );
    glVertex3d( v[0][2][0], v[0][2][1], v[0][2][2] );
    glVertex3d( v[0][3][0], v[0][3][1], v[0][3][2] );
    glEnd();

    // Draw top
    glBegin( GL_LINE_LOOP );
    glVertex3d( v[1][0][0], v[1][0][1], v[1][0][2] );
    glVertex3d( v[1][1][0], v[1][1][1], v[1][1][2] );
    glVertex3d( v[1][2][0], v[1][2][1], v[1][2][2] );
    glVertex3d( v[1][3][0], v[1][3][1], v[1][3][2] );
    glEnd();

    glBegin( GL_LINES );
    glVertex3d( v[0][0][0], v[0][0][1], v[0][0][2] );
    glVertex3d( v[1][0][0], v[1][0][1], v[1][0][2] );

    glVertex3d( v[0][1][0], v[0][1][1], v[0][1][2] );
    glVertex3d( v[1][1][0], v[1][1][1], v[1][1][2] );

    glVertex3d( v[0][2][0], v[0][2][1], v[0][2][2] );
    glVertex3d( v[1][2][0], v[1][2][1], v[1][2][2] );

    glVertex3d( v[0][3][0], v[0][3][1], v[0][3][2] );
    glVertex3d( v[1][3][0], v[1][3][1], v[1][3][2] );
    glEnd();
}
