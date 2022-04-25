#include "stdafx.h"
#include "fotobrow.h"

#include "3DClass/Ptg3DNode.h"
#include "3DClass/Ptg3DPointObj.h"
#include "3DClass/Ptg3DCalcPointObj.h"
#include "3DClass/Ptg3DCylinderObj.h"
#include "3DClass/Ptg3DBoxObj.h"
#include "3DClass/Ptg3DRtorObj.h"
#include "3DClass/Ptg3DCtorObj.h"
#include "3DClass/Ptg3DPyramidObj.h"
#include "3DClass/Ptg3DConeObj.h"


#include "ObjDGNX/DgnFunctions.h"

#define PI          3.1415926535897932384626433
#define DEGRAD(a)   (a*(PI/180))
#define RADDEG(a)   (a*(180/PI))

int saveNodeToDGN( DGN dgn, Ptg3DNode *node )
{
    POSITION pos;
	VECTOR vTMatrix;
	double xaxis[3];
	double yaxis[3];
	double zaxis[3];
	double origin[3];

	vTMatrix = node->getMatrix();
	xaxis[0] = vTMatrix[1];		yaxis[0] = vTMatrix[2];		zaxis[0] = vTMatrix[3];
	xaxis[1] = vTMatrix[4];		yaxis[1] = vTMatrix[5];		zaxis[1] = vTMatrix[6];
	xaxis[2] = vTMatrix[7];		yaxis[2] = vTMatrix[8];		zaxis[2] = vTMatrix[9];

	origin[0] = vTMatrix[10];
	origin[1] = vTMatrix[11];
	origin[2] = vTMatrix[12];

	dgnPushCurrentTransform(dgn);
	dgnSetCurrentCoordSystemRel(dgn, xaxis, yaxis, zaxis, origin );

	if( IS_TYPE_OF( node, Ptg3DNode ) /*|| IS_TYPE_OF( node, PtgWorld )*/ )
	{
	}
	else if( IS_TYPE_OF( node, Ptg3DPointObj ) || IS_TYPE_OF( node, Ptg3DCalcPointObj ) )
	{
//		add3DObjToGrid( pointSheet, (Ptg3DCalcPointObj *)aNode );
	}
	else if( IS_TYPE_OF( node, Ptg3DBoxObj ) )
	{
		Ptg3DBoxObj *aBox = (Ptg3DBoxObj *)node;

		dgnCreateBox( dgn, aBox->getXLen(), aBox->getYLen(), aBox->getZLen() );
	}
	else if( IS_TYPE_OF( node, Ptg3DCylinderObj ) )
	{
//		add3DObjToGrid( cyliSheet, (Ptg3DCylinderObj *)aNode );
		Ptg3DCylinderObj *aCylinder = (Ptg3DCylinderObj *)node;

		if( aCylinder->getRadius() != 0.0 && aCylinder->getHeight() != 0.0 )
			dgnCreateCylinder(dgn, ((Ptg3DCylinderObj *)node)->getRadius(), ((Ptg3DCylinderObj *)node)->getHeight() );
	}
	else if( IS_TYPE_OF( node, Ptg3DPyramidObj ) )
	{
		Ptg3DPyramidObj *aPyra = (Ptg3DPyramidObj *)node;

		dgnCreatePyramid( dgn, aPyra->getBaseWidth(), aPyra->getBaseDepth(), aPyra->getHeight(),
				aPyra->getWidthOffset(), aPyra->getDepthOffset(), aPyra->getTopWidth(), aPyra->getTopDepth() );
	}
	else if( IS_TYPE_OF( node, Ptg3DRtorObj ) )
	{
		Ptg3DRtorObj *aRtor = (Ptg3DRtorObj *)node;

		dgnCreateRectTorus( dgn, aRtor->getInnerRadius(), aRtor->getOuterRadius(),
				aRtor->getHeight(), DEGRAD(aRtor->getAngle()) );
	}
	else if( IS_TYPE_OF( node, Ptg3DCtorObj ) )
	{
		Ptg3DCtorObj *aCtor = (Ptg3DCtorObj *)node;

		double torusRadius = (aCtor->getInnerRadius() + aCtor->getOuterRadius()) / 2.0;
		double crossSectionRadius = (aCtor->getOuterRadius() - aCtor->getInnerRadius()) / 2.0;
		dgnCreateCircTorus( dgn, torusRadius, crossSectionRadius, DEGRAD(aCtor->getAngle()) );
	}
	else if( IS_TYPE_OF( node, Ptg3DConeObj ) )
	{
		Ptg3DConeObj *aCone = (Ptg3DConeObj *)node;

		dgnCreateOffSetCone( dgn, aCone->getBottomRadius(), aCone->getHeight(), aCone->getXOffset(),
				aCone->getYOffset(), aCone->getTopRadius() );
	}

    for( pos = node->getMemberList().GetHeadPosition(); pos != NULL; node->getMemberList().GetNext(pos) )
    {
		Ptg3DNode *aNode = (Ptg3DNode *)node->getMemberList().GetAt(pos);
		saveNodeToDGN(dgn, aNode);
    }
    
	dgnPopCurrentTransform(dgn);

	return TRUE;
}