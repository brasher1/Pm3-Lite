#include <stdio.h>
#include <setjmp.h>

#include "CList.h"

#include "HierElem.h"

#include "PtgObjTransMatrix.h"
#include "Ptg3DNode.h"
#include "Ptg3DLineObj.h"
#include "Ptg3DCylinderObj.h"

#define WHEREAMI() printf( "LOCATION: %s.%d\n", __FILE__, __LINE__ );

int outputLevel = 0;
int debugOutputLevel = 0;
jmp_buf env;

void main( void )
{
    PtgObjTransMatrix theWorld;
    PtgObjTransMatrix aSite, aZone, anEquip;

    theWorld.setName( "World" );
    aSite.setName( "Test site" );
    aZone.setName( "Test zone" );
    anEquip.setName( "Test equip" );

    theWorld.addMember( &aSite );
    aSite.addMember( &aZone );
    aZone.addMember( &anEquip );

//aSite.setIsVisible( FALSE );

    for( int i = 0; i < 10; i++ )
    {
	char szPrimitiveName[128];
	Ptg3DNode *thePrimitive = new Ptg3DCylinderObj;
	sprintf( szPrimitiveName, "Cyli%d", i+1 );
	thePrimitive->setName( szPrimitiveName );
	thePrimitive->setColor( i, i+100, i+200 );
	if( (i % 2) == 0 )
	    thePrimitive->setIsVisible( FALSE );
	anEquip.addMember( thePrimitive );
	fprintf( stderr, "Added primitive %d.\n", i );
    }

    fprintf( stderr, "Done adding primitives to list, now have %d.\n", anEquip.getMemberList().GetCount() );
    
    POSITION theWorldPos, theSitePos, theZonePos, theEquipPos, thePrimPos;
    int nWorldCount, nSiteCount, nZoneCount, nEquipCount, nPrimCount;

    for( nWorldCount = 0, theWorldPos = theWorld.getMemberList().GetHeadPosition();
	    theWorldPos != NULL;
	    theWorld.getMemberList().GetNext(theWorldPos), nWorldCount++ )
    {
	HierElem *aSite = theWorld.getMemberList().GetAt(theWorldPos);

	fprintf( stderr, "Sitename = %s(%d) - visible(%d)\n", aSite->getName(), nWorldCount, aSite->isVisible() );
	
	for( nSiteCount = 0, theSitePos = aSite->getMemberList().GetHeadPosition();
		theSitePos != NULL;
		aSite->getMemberList().GetNext(theSitePos), nSiteCount++ )
	{
	    HierElem *aZone = aSite->getMemberList().GetAt(theSitePos);
	    fprintf( stderr, "  Zonename = %s(%d) - visible(%d)\n", aZone->getName(), nSiteCount, aZone->isVisible() );

	    for( nZoneCount = 0, theZonePos = aZone->getMemberList().GetHeadPosition();
		    theZonePos != NULL;
		    aZone->getMemberList().GetNext(theZonePos), nZoneCount++ )
	    {
		HierElem *anEquip = aZone->getMemberList().GetAt(theZonePos);
		fprintf( stderr, "    Equipname = %s(%d) - visible(%d)\n", anEquip->getName(), nZoneCount, anEquip->isVisible() );

		for( nEquipCount = 0, theEquipPos = anEquip->getMemberList().GetHeadPosition();
			theEquipPos != NULL;
			anEquip->getMemberList().GetNext(theEquipPos), nEquipCount++ )
		{
		    HierElem *aPrimitive = anEquip->getMemberList().GetAt(theEquipPos);
		    fprintf( stderr, "      Primname = %s(%d) - visible(%d) - color = %d, %d, %d - branch visible = %d\n",
			    aPrimitive->getName(), nEquipCount, aPrimitive->isVisible(), 
			    aPrimitive->getColorRed(), aPrimitive->getColorGreen(), aPrimitive->getColorBlue(), 
			    aPrimitive->isBranchVisible() );
		}
	    }
	}
    }
}
