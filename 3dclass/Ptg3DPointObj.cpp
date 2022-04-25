#include "Ptg3DPointObj.h"

ID_NAME(Ptg3DPointObj);

Ptg3DPointObj::Ptg3DPointObj()
{
    initVars();
}

Ptg3DPointObj::~Ptg3DPointObj()
{
}

void Ptg3DPointObj::initVars()
{
}

void Ptg3DPointObj::setPosition( double x, double y, double z, Ptg3DNode *refNode )
{
	VECTOR vOldPntToRef = getMatrix( refNode );
	VECTOR vNewPntToRef = getMatrix( refNode );
	vNewPntToRef[10] = x;
	vNewPntToRef[11] = y;
	vNewPntToRef[12] = z;

	VECTOR vOldPntToRefInv = matinv( vOldPntToRef );
	VECTOR vDeltaMatrix = matmat( vOldPntToRefInv, vNewPntToRef );

	VECTOR vPntToParent = getMatrix(NULL);

	VECTOR vNewPntToParent = matmat( vPntToParent, vDeltaMatrix );

	setMatrix( vNewPntToParent );

	free_vector( vNewPntToParent, 1, 12 );
	free_vector( vPntToParent, 1, 12 );
	free_vector( vDeltaMatrix, 1, 12 );
	free_vector( vOldPntToRefInv, 1, 12 );
	free_vector( vNewPntToRef, 1, 12 );
	free_vector( vOldPntToRef, 1, 12 );

#if FALSE
	VECTOR vPntToRef = getMatrix( refNode );
	Ptg3DNode *parentNode = (Ptg3DNode *)getOwner();

	if( parentNode )
	{
		VECTOR vParentToRef = ((Ptg3DNode *)getOwner())->getMatrix( refNode );

		vPntToRef[10] = x;
		vPntToRef[11] = y;
		vPntToRef[12] = z;

		VECTOR vRefToParent = matinv(vParentToRef);
		VECTOR vPntToParent = matmat( vPntToRef, vParentToRef );

		setMatrix( vPntToParent );

		free_vector( vParentToRef, 1, 12 );
		free_vector( vRefToParent, 1, 12 );
		free_vector( vPntToParent, 1, 12 );
	}
	else
	{
		vPntToRef[10] = x;
		vPntToRef[11] = y;
		vPntToRef[12] = z;

		setMatrix( vPntToRef );
	}

	free_vector( vPntToRef, 1, 12 );
#endif

#if FALSE
	VECTOR v = getMatrix();

	// Can't really explain this swapping business...would like to go back through later
	// and find the reason.  But for now, it seems to work.
	v[10] = x;
	v[11] = z;
	v[12] = -y;

	setMatrix( v );
	free_vector( v, 1, 12 );
#endif
}

VECTOR Ptg3DPointObj::getPosition( Ptg3DNode *refNode )
{
	VECTOR vRetPoint = NULL;
	VECTOR vPoint = vector( 1, 3 );
	vPoint[1] = vPoint[2] = vPoint[3] = 0.0;

	if( refNode == NULL )
		refNode = (Ptg3DNode *)getOwner();

	VECTOR vTMatrix = getMatrix(refNode);
	vRetPoint = matpnt( vTMatrix, vPoint );

	free_vector( vTMatrix, 1, 12 );
	free_vector( vPoint, 1, 3 );

	return( vRetPoint );
	
//	VECTOR v = getMatrix();
//	VECTOR vRet = clone_vector( v, 10, 12, 1, 3 );
//	free_vector( v, 1, 12 );

//	return( vRet );
}

void Ptg3DPointObj::getPosition( Ptg3DNode *refNode, double &x, double &y, double &z )
{
	VECTOR vPosition = getPosition( refNode );

	x = vPosition[1];
	y = vPosition[2];
	z = vPosition[3];

	free_vector( vPosition, 1, 3 );
	return;
}