#include <assert.h>

#include "PtgTransMatrix.h"

PtgTransMatrix::PtgTransMatrix()
{
    initVars();
}

PtgTransMatrix::PtgTransMatrix( VECTOR v )
{
    initVars();
    setMatrix( v );
}

void PtgTransMatrix::initVars()
{
    theMatrix = vector(1, 12);
    setIdentity();
}

PtgTransMatrix::~PtgTransMatrix()
{
    if( theMatrix )
    {
	free_vector( theMatrix, 1, 12 );    
	theMatrix = NULL;
    }
}

void PtgTransMatrix::setMatrix( VECTOR v )
{
    if( theMatrix == NULL )
	theMatrix = vector( 1, 12 );
    copy_vector( theMatrix, 1, 12, v, 1, 12 );
}

void PtgTransMatrix::setMatrix( PtgTransMatrix &potm )
{
    if( theMatrix == NULL )
	theMatrix = vector( 1, 12 );
    for( int i = 1; i <= 12; i++ )
	theMatrix[i] = potm.theMatrix[i];
}

VECTOR PtgTransMatrix::getMatrix()
{
    if( theMatrix != NULL )
	return clone_vector( theMatrix, 1, 12, 1, 12 );
    else
	return NULL;
}

double PtgTransMatrix::operator[]( int nIndex )
{
    assert( nIndex >= 1 );
    assert( nIndex <= 12 );

    if( theMatrix != NULL )
	return( theMatrix[nIndex] );
    else
	return 0.0;
}

void PtgTransMatrix::setIdentity()
{
    if( theMatrix != NULL )
    {
	for( int i = 1; i <= 12; i++ )
	    theMatrix[i] = 0.0;
	theMatrix[1] = theMatrix[5] = theMatrix[9] = 1.0;
    }
}
