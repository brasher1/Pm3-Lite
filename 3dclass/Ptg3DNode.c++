#include "Ptg3DNode.h"

VECTOR vectorTMatrixTranspose( VECTOR v )
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

int Ptg3DNode::nUniqID = 0;

Ptg3DNode::Ptg3DNode()
{
    initVars();
}

Ptg3DNode::~Ptg3DNode()
{
    while( getMemberList().GetCount() > 0 )
	delete getMemberList().RemoveTail();

    if( nodeName != NULL )
	free( nodeName );
}

void Ptg3DNode::initVars()
{
    nMyUniqID = ++nUniqID;
    nodeName = NULL;
    currDrawStyle = DS_NONE;
    clearColor();
    nIsVisible = TRUE;
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
    
fprintf( stderr, "Ptg3DNode::findByName - Exiting\n" );
    return( NULL );
}

VECTOR Ptg3DNode::getMatrix( Ptg3DNode *refNode )
{
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
	    VECTOR vTemp2 = vectorTMatrixTranspose( vTemp );
	    VECTOR vTempResult = matmat( vTemp2, vResult );

fprintf( stderr, "topNode->getName() = %s\n", topNode->getName() );
fprintf( stderr, "vTemp = \n" );
print_vector( vTemp, 1, 12 );
fprintf( stderr, "vTempResult = \n" );
print_vector( vTempResult, 1, 12 );

	    free_vector( vTemp2, 1, 12 );
	    free_vector( vTemp, 1, 12 );
	    free_vector( vResult, 1, 12 );
	    vResult = vTempResult;
	}

	if( topNode == NULL || topNode == refNode )
	    bContinue = FALSE;
    }

    return vResult;
}

#if FALSE
int Ptg3DNode::isBranchVisible()
{
    HierElem *currOwner = this;
    int nVisible = TRUE;

    while( currOwner != NULL && nVisible )
    {
	nVisible = currOwner->isVisible();
	currOwner = currOwner->getOwner();
    }

    return nVisible;
}
#endif

