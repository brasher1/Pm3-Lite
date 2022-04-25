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

#include "Ptg3DFacetObj.h"

#ifndef CALLBACK
#define CALLBACK
#endif

void CALLBACK tessBegin( GLenum type );
void CALLBACK tessEdgeFlag( GLboolean flag );
void CALLBACK tessVertex( void *data );
void CALLBACK tessEnd( void );
void CALLBACK tessError( GLenum errno );

void Ptg3DFacetObj::drawWireframe()
{
    glBegin( GL_LINE_STRIP );

    POSITION pos;
    for( pos = vertices.GetHeadPosition(); pos != NULL; vertices.GetNext(pos) )
    {
	Ptg3DFacetVertexObj *aVertex = (Ptg3DFacetVertexObj *)vertices.GetAt(pos);
	VECTOR vertex, normal;
	
	vertex = aVertex->getVertex();
	normal = aVertex->getNormal();

	glNormal3d( normal[1], normal[2], normal[3] );
	glVertex3d( vertex[1], vertex[2], vertex[3] );

	free_vector( vertex, 1, 3 );
	free_vector( normal, 1, 3 );
    }

    glEnd();
}

void Ptg3DFacetObj::drawSolid()
{
    VECTOR *vects = NULL;
    int numVertices = 0;
    GLUtriangulatorObj *tessObj = gluNewTess();
    
    gluTessCallback( tessObj, GLU_BEGIN, (void(CALLBACK *)())tessBegin );
    gluTessCallback( tessObj, GLU_EDGE_FLAG, (void(CALLBACK *)())tessEdgeFlag );
    gluTessCallback( tessObj, GLU_VERTEX, (void(CALLBACK *)())tessVertex );
    gluTessCallback( tessObj, GLU_END, (void(CALLBACK *)())tessEnd );
    gluTessCallback( tessObj, GLU_ERROR, (void(CALLBACK *)())tessError );

    gluBeginPolygon( tessObj );
    POSITION pos;
    for( pos = vertices.GetHeadPosition(); pos != NULL; vertices.GetNext(pos) )
    {
	Ptg3DFacetVertexObj *aVertex = (Ptg3DFacetVertexObj *)vertices.GetAt(pos);
	VECTOR normal;
	
	numVertices++;
	vects = (VECTOR *)realloc( vects, numVertices * sizeof(VECTOR) );
	vects[numVertices - 1] = aVertex->getVertex();
	normal = aVertex->getNormal();

	gluTessVertex( tessObj, vects[numVertices - 1] + 1, vects[numVertices - 1] + 1 );

	free_vector( normal, 1, 3 );
    }

    gluEndPolygon(tessObj);
    gluDeleteTess(tessObj);


    if( vects )
    {
	for( int i = 0; i < numVertices; i++ )
	    free_vector( vects[i], 1, 3 );
	free(vects);
    }
}

void CALLBACK tessBegin( GLenum type )
{
/***************************************************************
** glBegin mode values (from /usr/include/GL/gl.h)
**
** GL_POINTS		0x0000
** GL_LINES		0x0001
** GL_LINE_LOOP		0x0002
** GL_LINE_STRIP	0x0003
** GL_TRIANGLES		0x0004
** GL_TRIANGLE_STRIP	0x0005
** GL_TRIANGLE_FAN	0x0006
** GL_QUADS		0x0007
** GL_QUAD_STRIP	0x0008
** GL_POLYGON		0x0009
**
***************************************************************/
    glBegin( type );
}

void CALLBACK tessEdgeFlag( GLboolean flag )
{
}

void CALLBACK tessVertex( void *data )
{
    double *d = (double *)data;
    glVertex3d( d[0], d[1], d[2] );
}

void CALLBACK tessEnd( void )
{
    glEnd();
}

void CALLBACK tessError( GLenum errno )
{
    fprintf( stderr, "tessError - errno = %d(%s)\n", errno, gluErrorString(errno) );
}
