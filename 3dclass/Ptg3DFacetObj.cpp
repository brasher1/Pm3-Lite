#include "Ptg3DFacetObj.h"

ID_NAME(Ptg3DFacetObj);

//
// Ptg3DFacetVertexObj implementation
//
Ptg3DFacetVertexObj::Ptg3DFacetVertexObj()
{
    initVars();
}

Ptg3DFacetVertexObj::Ptg3DFacetVertexObj( Ptg3DFacetVertexObj & aVertex )
{
    initVars();
    
    setVertex( aVertex.vertex[1], aVertex.vertex[2], aVertex.vertex[3] );
    setNormal( aVertex.normal[1], aVertex.normal[2], aVertex.normal[3] );
}

Ptg3DFacetVertexObj::~Ptg3DFacetVertexObj()
{
    if( vertex )
    {
	free_vector( vertex, 1, 3 );
	vertex = NULL;
    }

    if( normal )
    {
	free_vector( normal, 1, 3 );
	normal = NULL;
    }
}

void Ptg3DFacetVertexObj::setVertex(double x, double y, double z)
{
    vertex[1] = x;
    vertex[2] = y;
    vertex[3] = z;
}

void Ptg3DFacetVertexObj::setNormal(double x, double y, double z)
{
    normal[1] = x;
    normal[2] = y;
    normal[3] = z;
}

void Ptg3DFacetVertexObj::initVars()
{
    vertex = vector( 1, 3 );
    setVertex( 0.0, 0.0, 0.0 );    
    normal = vector( 1, 3 );
    setNormal( 1.0, 0.0, 0.0 );
}

VECTOR Ptg3DFacetVertexObj::getVertex( void )
{
    if( vertex != NULL )
	return clone_vector( vertex, 1, 3, 1, 3 );
    else
	return NULL;
}

VECTOR Ptg3DFacetVertexObj::getNormal( void )
{
    if( normal != NULL )
	return clone_vector( normal, 1, 3, 1, 3 );
    else
	return NULL;
}

//
// Ptg3DFacetObj implementation
//
Ptg3DFacetObj::Ptg3DFacetObj()
{
    initVars();
}

Ptg3DFacetObj::~Ptg3DFacetObj()
{
    while( vertices.GetCount() > 0 )
	delete vertices.RemoveTail();
}

void Ptg3DFacetObj::initVars()
{
}

void Ptg3DFacetObj::addVertex( Ptg3DFacetVertexObj &aVertex )
{
    Ptg3DFacetVertexObj *newVertex = new Ptg3DFacetVertexObj( aVertex );
    vertices.AddTail( newVertex );
}
