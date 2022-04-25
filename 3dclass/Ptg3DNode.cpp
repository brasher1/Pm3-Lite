#include "Ptg3DNode.h"

ID_NAME(Ptg3DNode);

VECTOR vectorLoadIdentity()
{
	VECTOR v = vector( 1, 12 );
	v[1] = v[5] = v[9] = 1.0;
	v[2] = v[3] = 0.0;
	v[4] = v[6] = 0.0;
	v[7] = v[8] = 0.0;
	v[10] = v[11] = v[12] = 0.0;

	return v;
}

VECTOR vectorTMatrixTransposeRotation( VECTOR v )
{
    VECTOR vReturn = vector( 1, 12 );
    
    vReturn[1] = v[1];
    vReturn[2] = v[4];
    vReturn[3] = v[7];
    vReturn[4] = v[2];
    vReturn[5] = v[5];
    vReturn[6] = v[8];
    vReturn[7] = v[3];
    vReturn[8] = v[6];
    vReturn[9] = v[9];

    vReturn[10] = v[10];
    vReturn[11] = v[11];
    vReturn[12] = v[12];

	return( vReturn );
}

VECTOR vectorTMatrixTransposeTranslation( VECTOR v )
{
    VECTOR vReturn = vector( 1, 12 );
    
    vReturn[1] = v[1];
    vReturn[2] = v[2];
    vReturn[3] = v[3];
    vReturn[4] = v[4];
    vReturn[5] = v[5];
    vReturn[6] = v[6];
    vReturn[7] = v[7];
    vReturn[8] = v[8];
    vReturn[9] = v[9];

	vReturn[10] = -(vReturn[1]*v[10] + vReturn[2]*v[11] + vReturn[3]*v[12]);
	vReturn[11] = -(vReturn[4]*v[10] + vReturn[5]*v[11] + vReturn[6]*v[12]);
	vReturn[12] = -(vReturn[7]*v[10] + vReturn[8]*v[11] + vReturn[9]*v[12]);

    return( vReturn );
}

int Ptg3DNode::nUniqID = 0;

Ptg3DNode::Ptg3DNode()
{
    initVars();
}

Ptg3DNode::~Ptg3DNode()
{
    if( nodeName != NULL )
	free( nodeName );
}

void Ptg3DNode::initVars()
{
    nMyUniqID = ++nUniqID;
    nodeName = NULL;
    currDrawStyle = DS_NONE;
    clearColor();
    bIsActive = TRUE;
    transMatrix.setIdentity();
}

void Ptg3DNode::setColor( float red, float green, float blue, float alpha )
{
    rgbColors[0] = red;
    rgbColors[1] = green;
    rgbColors[2] = blue;
    rgbColors[3] = alpha;
}

void Ptg3DNode::getColor( float retColors[4] )
{
    if( rgbColors[0] != -1.0 )
    {
	for( int i = 0; i < 4; i++ )
	    retColors[i] = rgbColors[i];
    }
    else if( getOwner() != NULL )
	((Ptg3DNode *)getOwner())->getColor( retColors );
    else
    {
	for( int i = 0; i < 4; i++ )
	    retColors[i] = 0.0;
    }
}

void Ptg3DNode::setName( const char *newName )
{
    if( nodeName != NULL )
	delete nodeName;
    nodeName = strdup( newName );
}

Ptg3DNode *Ptg3DNode::findByID( int nNumIDS, unsigned int naIDS[] )
{
    POSITION pos;
    for( pos = getMemberList().GetHeadPosition(); pos != NULL;
	    getMemberList().GetNext(pos) )
    {
	Ptg3DNode *aNode = (Ptg3DNode *)getMemberList().GetAt(pos);
	if( aNode->getUniqID() == naIDS[0] )
	{
	    if( nNumIDS == 1 )
		return( aNode );
	    else
		return( aNode->findByID( nNumIDS - 1, naIDS+1 ) );
	}
    }
    
    return( NULL );
}

Ptg3DNode::DrawStyle Ptg3DNode::getDrawStyle()
{
    if( currDrawStyle != DS_NONE )
	return currDrawStyle;

    if( getOwner() != NULL )
	return( ((Ptg3DNode *)getOwner())->getDrawStyle() );
    else
	return( currDrawStyle );
}

Ptg3DNode *Ptg3DNode::findByName( const char *findName, int bRecurse )
{
	if( getName() != NULL && strcmp( findName, getName() ) == 0 )
			return( this );

    POSITION pos;
    for( pos = getMemberList().GetHeadPosition(); pos != NULL; getMemberList().GetNext(pos) )
    {
		Ptg3DNode *aNode = (Ptg3DNode *)getMemberList().GetAt(pos);

		if( aNode->getName() != NULL && strcmp( findName, aNode->getName() ) == 0 )
			return( aNode );
		
		if( bRecurse )
		{
			Ptg3DNode *aNode2 = aNode->findByName( findName, bRecurse );
			if( aNode2 != NULL )
			return( aNode2 );
		}
    }

    return( NULL );
}

void Ptg3DNode::setMatrix( VECTOR v )
{
//	VECTOR v2 = vectorTMatrixTransposeRotation(v);
//	transMatrix.setMatrix( v2 );
//	free_vector( v2, 1, 12 );
	transMatrix.setMatrix( v );
}

VECTOR Ptg3DNode::getMatrix( Ptg3DNode *refNode )
{
	VECTOR vReturn = NULL;

	if( getOwner() != NULL && refNode != NULL )
	{
		Ptg3DNode *currNode = this;
		VECTOR vMatrix1 = vectorLoadIdentity();

		while( currNode != NULL )
		{
			VECTOR vCurrMatrix = currNode->getMatrix();
			VECTOR vTempMatrix = matmat( vCurrMatrix, vMatrix1 );

			free_vector( vMatrix1, 1, 12 );
			free_vector( vCurrMatrix, 1, 12 );
			vMatrix1 = vTempMatrix;

			currNode = (Ptg3DNode *)(currNode->getOwner());
		}

		currNode = refNode;
		VECTOR vMatrix2 = vectorLoadIdentity();

		while( currNode != NULL )
		{
			VECTOR vCurrMatrix = currNode->getMatrix();
			VECTOR vTempMatrix = matmat( vCurrMatrix, vMatrix2 );

			free_vector( vMatrix2, 1, 12 );
			free_vector( vCurrMatrix, 1, 12 );
			vMatrix2 = vTempMatrix;

			currNode = (Ptg3DNode *)(currNode->getOwner());
		}

		VECTOR vInvMatrix2 = matinv(vMatrix2);
		vReturn = matmat( vInvMatrix2, vMatrix1 );

		free_vector( vMatrix1, 1, 12 );
		free_vector( vMatrix2, 1, 12 );
		free_vector( vInvMatrix2, 1, 12 );
	}
	else
	{
		vReturn = transMatrix.getMatrix();
	}

	return vReturn;


#if FALSE
	// Ok, we've had to explain this waaaaay too many times...here is a semi-documented
	// explanation....
	//
	// The matrix is stored in TransMatrix with the rotation from parent to child and the
	// translation is from child to parent.  This is great and wonderful for handing it to
	// OpenGL.  However, our code external to the 3D class hierarchy needs a consistent
	// view of the matrix.  The preferred way would be a matrix from child to parent (this
	// matches the way Tom's matrices work).  So, this function will make that switch...and
	// setMatrix should make the switch as well.


	// If we're not asking relative to something else, just patch up matrix for child to
	// parent and return it.
	if( !refNode )
	{
		VECTOR v = transMatrix.getMatrix();
		VECTOR v2 = vectorTMatrixTransposeRotation(v);
		free_vector( v, 1, 12 );
		return v2;
	}

	// Otherwise...
	Ptg3DNode *currNode = this;

	VECTOR vCurrNode = currNode->getMatrix();
//	VECTOR vCurrNodeTranspose = vectorTMatrixTransposeRotation(vCurrNode);

	VECTOR vRightLeg = vCurrNode;
//	free_vector( vCurrNode, 1, 12 );

	currNode = (Ptg3DNode *)currNode->getOwner();

	while( currNode != NULL )
	{
		vCurrNode = currNode->getMatrix();
//		vCurrNodeTranspose = vectorTMatrixTransposeRotation(vCurrNode);

		VECTOR vTempRightLeg = matmat( vCurrNode, vRightLeg );
		
		free_vector( vCurrNode, 1, 12 );
//		free_vector( vCurrNodeTranspose, 1, 12 );
		free_vector( vRightLeg, 1, 12 );

		vRightLeg = vTempRightLeg;

		currNode = (Ptg3DNode *)currNode->getOwner();
	}

	currNode = refNode;

	vCurrNode = currNode->getMatrix();
	VECTOR vCurrNodeTranspose = matinv(vCurrNode);

	VECTOR vLeftLeg = vCurrNodeTranspose;
	free_vector( vCurrNode, 1, 12 );

	currNode = (Ptg3DNode *)currNode->getOwner();

	while( currNode != NULL )
	{
		vCurrNode = currNode->getMatrix();
		vCurrNodeTranspose = matinv(vCurrNode);

		VECTOR vTempLeftLeg = matmat( vLeftLeg, vCurrNodeTranspose );

		free_vector( vCurrNode, 1, 12 );
		free_vector( vCurrNodeTranspose, 1, 12 );
		free_vector( vLeftLeg, 1, 12 );

		vLeftLeg = vTempLeftLeg;

		currNode = (Ptg3DNode *)currNode->getOwner();
	}

	VECTOR vReturn = matmat( vLeftLeg, vRightLeg );
	free_vector( vRightLeg, 1, 12 );
	free_vector( vLeftLeg, 1, 12 );

	return vReturn;
#endif
#if FALSE
	if( !refNode )
	{
		VECTOR v = transMatrix.getMatrix();
		VECTOR v2 = vectorTMatrixTransposeRotation(v);
		free_vector(v, 1, 12 );
		return v2;
//		return transMatrix.getMatrix();
	}
	Ptg3DNode *currNode = this;

	VECTOR vCurrNode = currNode->getMatrix();
	VECTOR vCurrNodeTranspose = vectorTMatrixTransposeRotation(vCurrNode);

	VECTOR vRightLeg = vCurrNodeTranspose;
	free_vector( vCurrNode, 1, 12 );

	currNode = (Ptg3DNode *)currNode->getOwner();

	while( currNode != NULL )
	{
		vCurrNode = currNode->getMatrix();
		vCurrNodeTranspose = vectorTMatrixTransposeRotation(vCurrNode);

		VECTOR vTempRightLeg = matmat( vCurrNodeTranspose, vRightLeg );
		
		free_vector( vCurrNode, 1, 12 );
		free_vector( vCurrNodeTranspose, 1, 12 );
		free_vector( vRightLeg, 1, 12 );

		vRightLeg = vTempRightLeg;

		currNode = (Ptg3DNode *)currNode->getOwner();
	}

	currNode = refNode;

	vCurrNode = currNode->getMatrix();
	vCurrNodeTranspose = vectorTMatrixTransposeTranslation(vCurrNode);

	VECTOR vLeftLeg = vCurrNodeTranspose;
	free_vector( vCurrNode, 1, 12 );

	currNode = (Ptg3DNode *)currNode->getOwner();

	while( currNode != NULL )
	{
		vCurrNode = currNode->getMatrix();
		vCurrNodeTranspose = vectorTMatrixTransposeTranslation(vCurrNode);

		VECTOR vTempLeftLeg = matmat( vLeftLeg, vCurrNodeTranspose );

		free_vector( vCurrNode, 1, 12 );
		free_vector( vCurrNodeTranspose, 1, 12 );
		free_vector( vLeftLeg, 1, 12 );

		vLeftLeg = vTempLeftLeg;

		currNode = (Ptg3DNode *)currNode->getOwner();
	}

	VECTOR vReturn = matmat( vLeftLeg, vRightLeg );
	free_vector( vRightLeg, 1, 12 );
	free_vector( vLeftLeg, 1, 12 );

	return vReturn;
#endif

#if FALSE
	if( !refNode )
		return transMatrix.getMatrix();

	VECTOR vTargetToTopMost, vTargetToTopMostInv;
	VECTOR vRefToTopMost, vRefToTopMostInv;

	vTargetToTopMost = transMatrix.getMatrix();
	vTargetToTopMostInv = vectorTMatrixTransposeRotation(vTargetToTopMost);

	vRefToTopMost = refNode->getMatrix();
	vRefToTopMostInv = vectorTMatrixTransposeTranslation(vRefToTopMost);

	//
	// Find target relative to top of tree
	//
	Ptg3DNode *currNode = this;
	while( currNode->getOwner() != NULL )
	{
		currNode = (Ptg3DNode *)(currNode->getOwner());
		VECTOR vCurrNode = currNode->getMatrix();
		VECTOR vCurrNodeInv = vectorTMatrixTransposeRotation(vCurrNode);

		VECTOR vTempTargetToTopMost = matmat( vTargetToTopMostInv, vCurrNodeInv );

		free_vector( vCurrNode, 1, 12 );
		free_vector( vCurrNodeInv, 1, 12 );
		free_vector( vTargetToTopMostInv, 1, 12 );

		vTargetToTopMostInv = vTempTargetToTopMost;
	}

	//
	// Find reference node relative to top of tree
	//
	currNode = refNode;
	while( currNode->getOwner() != NULL )
	{
		currNode = (Ptg3DNode *)(currNode->getOwner());
		VECTOR vCurrNode = currNode->getMatrix();
		VECTOR vCurrNodeInv = vectorTMatrixTransposeTranslation( vCurrNode );

//		vCurrNode[10]*= -1.0;
//		vCurrNode[11]*= -1.0;
//		vCurrNode[12]*= -1.0;

		VECTOR vTempRefToTopMost = matmat( vRefToTopMost, vCurrNode );

		free_vector( vCurrNode, 1, 12 );
		free_vector( vCurrNodeInv, 1, 12 );
		free_vector( vRefToTopMost, 1, 12 );

		vRefToTopMost = vTempRefToTopMost;
	}


	VECTOR vTargetToRef = matmat( vRefToTopMost, vTargetToTopMostInv );
//	VECTOR vTargetToRefInv = vectorTMatrixTranspose( vTargetToRef );
//	free_vector( vTargetToRef, 1, 12 );
//	vTargetToRef = vTargetToRefInv;

	free_vector( vTargetToTopMost, 1, 12 );
	free_vector( vTargetToTopMostInv, 1, 12 );
	free_vector( vRefToTopMost, 1, 12 );
	free_vector( vRefToTopMostInv, 1, 12 );

	return vTargetToRef;
#endif

#if FALSE
    if( refNode == NULL )
	return transMatrix.getMatrix();

    VECTOR vResult = transMatrix.getMatrix();

    Ptg3DNode *topNode = this;
    int bContinue = TRUE;
    while( bContinue )
    {
	topNode = (Ptg3DNode *)(topNode->getOwner());
	if( topNode != NULL )
	{
	    VECTOR vTemp = topNode->getMatrix();
	    if( vTemp != NULL )
	    {
		VECTOR vTemp2 = vectorTMatrixTranspose( vTemp );
		VECTOR vTempResult = matmat( vTemp2, vResult );
    
		free_vector( vTemp2, 1, 12 );
		free_vector( vTemp, 1, 12 );
		free_vector( vResult, 1, 12 );
		vResult = vTempResult;
	    }
	}

	if( topNode == NULL || topNode == refNode )
	    bContinue = FALSE;
    }

    return vResult;
#endif
}

void Ptg3DNode::setDrawStyle( DrawStyle ds, int bSetMembers )
{
//fprintf( stderr, "setting drawstyle for elem '%s' to %d\n", getName(), ds );
    currDrawStyle = ds;
#if FALSE
    if( bSetMembers )
    {
	POSITION pos;
	for( pos = getMemberList().GetHeadPosition(); pos != NULL;
		getMemberList().GetNext(pos) )
	{
	    Ptg3DNode *aNode = (Ptg3DNode *)getMemberList().GetAt(pos);
	    aNode->setDrawStyle( ds, TRUE );
	}
    }
#endif
}
