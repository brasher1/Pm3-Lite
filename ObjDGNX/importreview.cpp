/*COPYRIGHT******************************************************************
*									    *
*   Copyright (c) 1994 AsBuilt Data, Inc.  These coded instructions,	    *
*   statements, computer programs and source code contain unpublished	    *
*   trade secrets of AsBuilt Data, Inc. and are protected by Federal	    *
*   Copyright Law.  They may not be disclosed or copied, duplicated,	    *
*   distributed, demonstrated or licensed in any form, in whole or in	    *
*   part, without the prior written consent of AsBuilt Data, Inc.  Any	    *
*   permitted use shall be considered as a personal license to the	    *
*   authorized recipient only.  Any such permitted use is provided on a	    *
*   "as is" basis without any warranty of any kind, express or implied,	    *
*   and specifically without any warranty of merchantibility or fitness	    *
*   for a particular purpose.  In no event shall AsBuilt Data, Inc. or	    *
*   its affiliates be liable for any special, incidental, direct or	    *
*   indirect damages of any kind, or for any liabilities in excess of the   *
*   royalty fee payments actually made by Licensee hereunder regardless of  *
*   legal theory or negligence of AsBuilt Data, Inc.			    *
*									    *
******************************************************************COPYRIGHT*/

/*FF*************************************************************************
*									    *
*   FILE								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	05/05/95 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "toDgnFunctions.h"

//#include "output.h"

//#include "Ptg3DCylinderObj.h"
//#include "Ptg3DBoxObj.h"
//#include "Ptg3DCtorObj.h"
//#include "Ptg3DRtorObj.h"
//#include "Ptg3DConeObj.h"
//#include "Ptg3DPyramidObj.h"
//#include "Ptg3DFacetObj.h"
//#include "Ptg3DLineObj.h"

//VECTOR vectorTMatrixTransposeRotation( VECTOR v );


#if !defined OKAY
#define OKAY 0
#endif

#define MAX_LINE_LENGTH 1024

#define PI          3.1415926535897932384626433
#define DEGRAD(a)   (a*(PI/180))
#define RADDEG(a)   (a*(180/PI))

#define RVPRIM_PYRAMID         1
#define RVPRIM_BOX             2
#define RVPRIM_RTORUS          3
#define RVPRIM_CTORUS          4
#define RVPRIM_EDISH           5
#define RVPRIM_SDISH           6
#define RVPRIM_SNOUT           7
#define RVPRIM_CYLINDER        8
#define RVPRIM_SPHERE          9
#define RVPRIM_LINE           10
#define RVPRIM_FACET          11
#define RVPRIM_NONE1          12
#define RVPRIM_NONE2          13
#define RVPRIM_SRECTANGLE     14
#define RVPRIM_SANNULUS       15

typedef struct {
    int nEleType;
    char *pszEleName;
    int (*fpEleCB)( FILE *fp ,DGN dgn);
} eleCB; 

char *getFixedField( char *string, int fieldWidth, int fieldIndex );

int createHierLevel( FILE *fp, DGN dgn,float* oldcolor);
int createBox( FILE *fp, DGN dgn);
int createCylinder( FILE *fp, DGN dgn);
int createPyramid( FILE *fp, DGN dgn);
int createRectTorus( FILE *fp, DGN dgn);
int createCircTorus( FILE *fp, DGN dgn);
int createSnout( FILE *fp , DGN dgn);
int createSRect( FILE *fp, DGN dgn);
int createFacet( FILE *fp, DGN dgn);

#define NUM_RVPRIMS 15
eleCB eleHandlers[] = {
    { RVPRIM_PYRAMID,		"pyramid",			createPyramid }, 
    { RVPRIM_BOX,			"box",				createBox }, 
    { RVPRIM_RTORUS,	    "rect torus",		createRectTorus }, 
    { RVPRIM_CTORUS,	    "circ torus",		createCircTorus }, 
    { RVPRIM_EDISH,			"e dish",			NULL }, 
    { RVPRIM_SDISH,			"s dish",			NULL }, 
    { RVPRIM_SNOUT,			"snout",			createSnout }, 
    { RVPRIM_CYLINDER,	    "cylinder",			createCylinder }, 
    { RVPRIM_SPHERE,	    "sphere",			NULL }, 
    { RVPRIM_LINE,			"line",				createCylinder }, 
    { RVPRIM_FACET,			"facet",			createFacet }, 
    { RVPRIM_NONE1,			"none 1",			NULL }, 
    { RVPRIM_NONE2,			"none 2",			NULL }, 
    { RVPRIM_SRECTANGLE,    "srect",			createSRect }, 
    { RVPRIM_SANNULUS,	    "sheared annulus",	NULL }
};

float materialMap[][3] = {
    /*  1 */ { .51, .49, .52 },
    /*  2 */ { 1.0, 0.0, 0.0 },
    /*  3 */ { 1.0, .6, 0.0 },
    /*  4 */ { 1.0, 1.0, 0.0 },
    /*  5 */ { 0.0, 1.0, 0.0 },
    /*  6 */ { 0.0, 1.0, 1.0 },
    /*  7 */ { 0.0, 0.0, 1.0 },
    /*  8 */ { 1.0, 0.0, 1.0 },
    /*  9 */ { .65, .30, 0.0 },

    /* 10 */ { 1.0, 1.0, 1.0 },
    /* 11 */ { 1.0, .5, .5 },
    /* 12 */ { .7, .7, .7 },
    /* 13 */ { .45, .45, .45 },
    /* 14 */ { .5, .5, .7 },
    /* 15 */ { .95, .9, .95 },
    /* 16 */ { .75, .15, .05 },
    /* 17 */ { 0.0, .85, 0.0 },
    /* 18 */ { .95, .75, .5 },
    /* 19 */ { .7, .55, .10 },

    /* 20 */ { 0.0, 0.5, 1.0 },
    /* 21 */ { .7, .8, .4 },
    /* 22 */ { 1.0, 0.0, .4 },
    /* 23 */ { 0.0, .85, 0.0 },
    /* 24 */ { 1.0, .6, 0.0 },
    /* 25 */ { .95, .85, .7 },
    /* 26 */ { .7, .3, .15 },
    /* 27 */ { .5, .7, 1.0 },
    /* 28 */ { 1.0, 1.0, 1.0 },
    /* 29 */ { .1, .1, .1 },

    /* 30 */ { 1.0, 0.0, 0.0 },
    /* 31 */ { 1.0, 0.0, 0.0 },
    /* 32 */ { 1.0, 0.0, 0.0 },
    /* 33 */ { 1.0, 0.0, 0.0 },
    /* 34 */ { 1.0, 0.0, 0.0 },
    /* 35 */ { 1.0, 0.0, 0.0 },
    /* 36 */ { 1.0, 0.0, 0.0 },
    /* 37 */ { 1.0, 0.0, 0.0 },
    /* 38 */ { 1.0, 0.0, 0.0 },
    /* 39 */ { 1.0, 0.0, 0.0 },

    /* 40 */ { 1.0, 0.0, 0.0 },
    /* 41 */ { 1.0, 0.0, 0.0 },
    /* 42 */ { 1.0, 0.0, 0.0 },
    /* 43 */ { 1.0, 0.0, 0.0 },
    /* 44 */ { 1.0, 0.0, 0.0 },
    /* 45 */ { 1.0, 0.0, 0.0 },
    /* 46 */ { 1.0, 0.0, 0.0 },
    /* 47 */ { 1.0, 0.0, 0.0 },
    /* 48 */ { 1.0, 0.0, 0.0 },
    /* 49 */ { 1.0, 0.0, 0.0 },

    /* 50 */ { 1.0, 0.0, 0.0 },
    /* 51 */ { 1.0, 0.0, 0.0 },
    /* 52 */ { 1.0, 0.0, 0.0 },
    /* 53 */ { 1.0, 0.0, 0.0 },
    /* 54 */ { 1.0, 0.0, 0.0 },
    /* 55 */ { 1.0, 0.0, 0.0 },
    /* 56 */ { 1.0, 0.0, 0.0 },
    /* 57 */ { 1.0, 0.0, 0.0 },
    /* 58 */ { 1.0, 0.0, 0.0 },
    /* 59 */ { 1.0, 0.0, 0.0 },

    /* 60 */ { 1.0, 0.0, 0.0 },
    /* 61 */ { 1.0, 0.0, 0.0 },
    /* 62 */ { 1.0, 0.0, 0.0 },
    /* 63 */ { 1.0, 0.0, 0.0 },
    /* 64 */ { 1.0, 0.0, 0.0 },
};

static int nNumLines;
static int nPrimCount;

char *readLine( char *s, int n, FILE *fp )
{
    char *retval = fgets( s, n, fp );
    if( retval != NULL )
    {
	if( s[strlen(s)-1] == '\n' )
	    s[strlen(s)-1] = '\0';
    }
    nNumLines++;
//fprintf( stderr, "line %d\n", nNumLines );    
    return( retval );
}


int importReviewModel( char *pszReviewFile, char* dgnFileName);

main()
{

//	importReviewModel( "c:\\97-055.2prm", "c:\\output.dgn");
//	importReviewModel( "c:\\Hppveryl.prm", "c:\\output.dgn");
    importReviewModel( "c:\\TomTest.2prm", "c:\\output.dgn");
//	importReviewModel("\\\\Busbyj\\D\\ABD\\Program.cya\\97-055.2prm", "c:\\output.dgn");
	getchar();
	return 0;
}



#define FUNC "importReviewModel"


int importReviewModel( char *pszReviewFile, char* dgnFileName)
/*****************************************************************/
{
nNumLines = 0;
nPrimCount = 0;
    
    float defaultColor[3] = {0.0,0.0,0.0};
	
    if( pszReviewFile == NULL )
    {
	//ErrorOutput( "Invalid parameter passed to %s.\n", FUNC );
	printf( "Invalid parameter passed to %s.\n", FUNC );
	return( !OKAY );
    }


    FILE *fpReviewFile = fopen( pszReviewFile, "r" );
	DGN dgn = openDgnFile(dgnFileName);


    if( fpReviewFile == NULL )
    {
	printf( "Unable to open '%s'\n", pszReviewFile );
	return( !OKAY );
    }
 /*   
    double xaxis[3] = {0.0,3.0,0.0};
    double yaxis[3] = {0.0,0.0,3.0};
    double zaxis[3] = {3.0,0.0,0.0};
	double org[3] = {0.0,0.0,0.0};
	dgnPushCurrentTransform(dgn);
    dgnSetCurrentCoordSystemAbs(dgn,xaxis,yaxis,zaxis,org);
	dgnCreateBox(dgn,500,500,5000);
    dgnCreateOffSetCone(dgn,500,1000,0.0,0.0,100);

	dgnPopCurrentTransform(dgn);

*/

    char szLine[MAX_LINE_LENGTH];
    if(dgn != NULL)
    {
      while( readLine( szLine, sizeof(szLine), fpReviewFile ) != NULL )
      {
	  if( strcmp( szLine, "CNTB" ) == 0 )
	    createHierLevel( fpReviewFile,dgn, defaultColor);
      }
    }
	else
    {
		printf("Error openning DGN file - %s\n",dgnFileName);
	}
    fclose( fpReviewFile );
    closeDgnFile(dgn);

    //Output( 0, "Processed %d lines containing %d primitives.\n", nNumLines, nPrimCount );
    printf("Processed %d lines containing %d primitives.\n", nNumLines, nPrimCount );

    return( OKAY );
}
#undef FUNC

//int createHierLevel( FILE *fp, Ptg3DNode *theParent )
int createHierLevel( FILE *fp, DGN dgn,float* oldcolor)
{
    static int nHierLevel = 0;
    static long nodeID = 0;

    //Ptg3DNode *theNewNode = new Ptg3DNode;

    char szLine[MAX_LINE_LENGTH];

	char* name = (char*)malloc(MAX_LINE_LENGTH);
    

    readLine( szLine, sizeof(szLine), fp );	/* version line */
    readLine( name, sizeof(szLine), fp );	/* name */
    
    //theNewNode->setName( szLine );
	//[parentCoordSystem setNameTo:name];
   dgnPushNode(dgn);
   dgnPushCurrentTransform(dgn);
   //needed if not idenity dgnSetCurrentTransformRel(
   dgnCreateCurrentNode(dgn,name,nodeID++);

    readLine( szLine, sizeof(szLine), fp );	/* node translation wrt WORL */
    readLine( szLine, sizeof(szLine), fp );	/* node material(?) */
    int nMaterialIndex = atoi(szLine) - 1;

	/*
    theNewNode->setColor(
	    materialMap[nMaterialIndex][0], 
	    materialMap[nMaterialIndex][1], 
	    materialMap[nMaterialIndex][2] );
    theParent->addMember( theNewNode );
    */

	float color[3];
	color[0] = materialMap[nMaterialIndex][0];
	color[1] = materialMap[nMaterialIndex][1];
	color[2] = materialMap[nMaterialIndex][2];

	dgnSetColor(dgn,color);
    //DebugOutput( 4, "%*.*s%s\n", nHierLevel*2, nHierLevel*2, " ", theNewNode->getName() );
    //DebugOutput( 4, "%*.*s%s\n", nHierLevel*2, nHierLevel*2, " ",name );
    printf("%*.*s%s\n", nHierLevel*2, nHierLevel*2, " ",name );
    nHierLevel++;

    while( readLine( szLine, sizeof(szLine), fp ) != NULL )    /* next element(?) */
    {
		if( strcmp( szLine, "CNTE" ) == 0 )
		{
			readLine( szLine, sizeof(szLine), fp );	/* apparently another version line */
			nHierLevel--;
			dgnPopNode(dgn);
			dgnPopCurrentTransform(dgn);

          /*
			if( theNewNode->getMemberList().GetCount() == 1 )
			{
				Ptg3DNode *onlyChild = (Ptg3DNode *)(theNewNode->getMemberList().GetHead()); 
				if( !(onlyChild->getName()) || strlen(onlyChild->getName()) == 0 )
				{
					onlyChild->setName( theNewNode->getName() );
					theParent->addMember( onlyChild );
					theNewNode->getMemberList().RemoveHead();
					delete theNewNode;
				}
			}
          */
			dgnSetColor(dgn, oldcolor);
			return( OKAY );
		}
		else if( strcmp( szLine, "PRIM" ) == 0 )
		{
			readLine( szLine, sizeof(szLine), fp );	/* version line */
			readLine( szLine, sizeof(szLine), fp );	/* primitive type */
			int nPrimType = atoi(szLine);
			int nIsEleHandled = FALSE;
			for( int i = 0; i < NUM_RVPRIMS; i++ )
			{
				if( eleHandlers[i].nEleType == nPrimType )
				{
					if( eleHandlers[i].fpEleCB != NULL )
					{
					//	(eleHandlers[i].fpEleCB)( fp, theNewNode );
						(eleHandlers[i].fpEleCB)( fp, dgn );
						nIsEleHandled = TRUE;
						nPrimCount++;
					}
				}
			}
			if( !nIsEleHandled )
			{
	fprintf( stderr, "Primitive type %d not handled.\n", nPrimType );
	/*
			while( readLine( szLine, sizeof(szLine), fp ) != NULL &&
				   strcmp( szLine, "CNTE" ) != 0 );
			nHierLevel--;
			return( OKAY );
	*/
			}
		}
		else if( strcmp( szLine, "CNTB" ) == 0 )
		{
			createHierLevel( fp,dgn, color);
		}
    }
    nHierLevel--;
    
    /* If we got here, I think we hit EOF without ending a container */
	dgnSetColor(dgn,oldcolor);
	dgnPopNode(dgn);
	dgnPopCurrentTransform(dgn);
    return( !OKAY );
}



int setTransform( FILE *fp, /*Ptg3DNode *theNode,*/ DGN dgn, int bSwapAxes )
{
    char szLine[MAX_LINE_LENGTH];
    double v[13];
    //MATRIX *theMatrix = matrix( 0, 3, 0, 3 );
    //VECTOR v = vector( 1, 12 );

    //MATRIX *mZintoY = matrix( 0, 3, 0, 3 );
    //(*mZintoY)[0][0] = 1.0;	(*mZintoY)[0][1] = 0.0;   (*mZintoY)[0][2] = 0.0;   (*mZintoY)[0][3] = 0.0;
    //(*mZintoY)[1][0] = 0.0;	(*mZintoY)[1][1] = 0.0;   (*mZintoY)[1][2] = -1.0;  (*mZintoY)[1][3] = 0.0;
    //(*mZintoY)[2][0] = 0.0;	(*mZintoY)[2][1] = 1.0;   (*mZintoY)[2][2] = 0.0;   (*mZintoY)[2][3] = 0.0;
    //(*mZintoY)[3][0] = 0.0;	(*mZintoY)[3][1] = 0.0;   (*mZintoY)[3][2] = 0.0;   (*mZintoY)[3][3] = 1.0;

#if FALSE

    readLine( szLine, sizeof(szLine), fp );	// first line of xform *
    //(*theMatrix)[0][0] 
	  v[1]	= 1000.0*atof(getFixedField( szLine, 14, 0 ));
    //(*theMatrix)[1][0] 
	  v[4]	= 1000.0*atof(getFixedField( szLine, 14, 1 ));
    //(*theMatrix)[2][0] 
	  v[7]	= 1000.0*atof(getFixedField( szLine, 14, 2 ));

    v[10] = 1000.0*atof(getFixedField( szLine, 14, 3 ));

    readLine( szLine, sizeof(szLine), fp );	// second line of xform 
    //(*theMatrix)[0][1] 
		v[2] = 1000.0*atof(getFixedField( szLine, 14, 0 ));
    //(*theMatrix)[1][1] 
		v[5] = 1000.0*atof(getFixedField( szLine, 14, 1 ));
    //(*theMatrix)[2][1] 
		v[8] = 1000.0*atof(getFixedField( szLine, 14, 2 ));
    v[11] = 1000.0*atof(getFixedField( szLine, 14, 3 ));

    readLine( szLine, sizeof(szLine), fp );	// third line of xform 
    //(*theMatrix)[0][2] 
		v[3]= 1000.0*atof(getFixedField( szLine, 14, 0 ));
    //(*theMatrix)[1][2] 
		v[6] = 1000.0*atof(getFixedField( szLine, 14, 1 ));
    //(*theMatrix)[2][2] 
		v[9] = 1000.0*atof(getFixedField( szLine, 14, 2 ));
    v[12] = 1000.0*atof(getFixedField( szLine, 14, 3 ));
#endif

    readLine( szLine, sizeof(szLine), fp );	// first line of xform 
    //(*theMatrix)[0][0] 
	v[1]= 1000.0*atof(getFixedField( szLine, 14, 0 ));
    //(*theMatrix)[0][1] 
	v[2] = 1000.0*atof(getFixedField( szLine, 14, 1 ));
    //(*theMatrix)[0][2] 
	v[3] = 1000.0*atof(getFixedField( szLine, 14, 2 ));
    v[10] = 1000.0*atof(getFixedField( szLine, 14, 3 ));

    readLine( szLine, sizeof(szLine), fp );	// second line of xform 
    //(*theMatrix)[1][0] 
	v[4]= 1000.0*atof(getFixedField( szLine, 14, 0 ));
    //(*theMatrix)[1][1] 
	v[5] = 1000.0*atof(getFixedField( szLine, 14, 1 ));
    //(*theMatrix)[1][2] 
	v[6] = 1000.0*atof(getFixedField( szLine, 14, 2 ));
    v[11] = 1000.0*atof(getFixedField( szLine, 14, 3 ));

    readLine( szLine, sizeof(szLine), fp );	// third line of xform 
    //(*theMatrix)[2][0] 
	v[7] = 1000.0*atof(getFixedField( szLine, 14, 0 ));
    //(*theMatrix)[2][1] 
	v[8] = 1000.0*atof(getFixedField( szLine, 14, 1 ));
    //(*theMatrix)[2][2] 
	v[9] = 1000.0*atof(getFixedField( szLine, 14, 2 ));
    v[12] = 1000.0*atof(getFixedField( szLine, 14, 3 ));



    if( bSwapAxes )
    {
	//	MATRIX *mTemp = matrix_mult( mZintoY, 0, 3, 0, 3,
	//		theMatrix, 0, 3, 0, 3, 0, 3, 0, 3 );
	//	free_matrix( theMatrix, 0, 3, 0, 3 );
	//	theMatrix = mTemp;
    }
    
    //v[1] = (*theMatrix)[0][0];
    //v[2] = (*theMatrix)[0][1];
    //v[3] = (*theMatrix)[0][2];
    //v[4] = (*theMatrix)[1][0];
    //v[5] = (*theMatrix)[1][1];
    //v[6] = (*theMatrix)[1][2];
    //v[7] = (*theMatrix)[2][0];
    //v[8] = (*theMatrix)[2][1];
    //v[9] = (*theMatrix)[2][2];

//vectorTMatrixTransposeRotation( v );

    //theNode->setMatrix( v );

	double xaxis[3], yaxis[3], zaxis[3],origin[3];
	
	xaxis[0] = v[1];
	xaxis[1] = v[4];
	xaxis[2] = v[7];

	yaxis[0] = v[2];
	yaxis[1] = v[5];
	yaxis[2] = v[8];

    zaxis[0] = v[3];
	zaxis[1] = v[6];
	zaxis[2] = v[9];

	origin[0] = v[10];
	origin[1] = v[11];
	origin[2] = v[12];

    //set currrent coordSystem absolute as opposed to relative to current system
	dgnSetCurrentCoordSystemRel(dgn,xaxis,yaxis,zaxis,origin);

    //free_vector( v, 1, 12 );
    //free_matrix( theMatrix, 0, 3, 0, 3 );
    //free_matrix( mZintoY, 0, 3, 0, 3 );
    
    return( OKAY );
}

int createBox( FILE *fp , DGN dgn)
{
    //Ptg3DBoxObj *newBox = new Ptg3DBoxObj;

    char szLine[MAX_LINE_LENGTH];

	//instanceCount();

    dgnPushCurrentTransform(dgn);
    setTransform(fp,dgn,FALSE);

    readLine( szLine, sizeof(szLine), fp );	/* bounding box */
    readLine( szLine, sizeof(szLine), fp );	/* bounding box */

    readLine( szLine, sizeof(szLine), fp );	/* element parameters */
    double dfXLen = atof(getFixedField( szLine, 14, 0 ));
    double dfYLen = atof(getFixedField( szLine, 14, 1 ));
    double dfZLen = atof(getFixedField( szLine, 14, 2 ));
    
    //newBox->setXLen( dfXLen );
    //newBox->setYLen( dfYLen );
    //newBox->setZLen( dfZLen );
    //float color[4];
    //theParent->getColor( color );
    //newBox->setColor( color[0], color[1], color[2], color[3] );
    //dgnSetColor( color);
    
    //theParent->addMember( newBox );
	printf("start-");
	dgnCreateBox(dgn,dfXLen,dfYLen,dfZLen);

    dgnPopCurrentTransform(dgn);
    printf("end\n");
	//instanceCount();
    return( OKAY );
}

int createCylinder( FILE *fp, DGN dgn)
{
	
    //Ptg3DCylinderObj *newCyli = new Ptg3DCylinderObj;
    char szLine[MAX_LINE_LENGTH];
	//instanceCount();

    dgnPushCurrentTransform(dgn);
    setTransform( fp, dgn,/*newCyli,*/ FALSE );
	
	readLine( szLine, sizeof(szLine), fp );	/* bounding box */
    readLine( szLine, sizeof(szLine), fp );	/* bounding box */

    readLine( szLine, sizeof(szLine), fp );	/* element parameters */
    double dfRad = atof(getFixedField( szLine, 14, 0 ));
    double dfHei = atof(getFixedField( szLine, 14, 1 ));
    
    //newCyli->setRadius( dfRad );
    //newCyli->setHeight( dfHei );
    //float color[4];
    //theParent->getColor( color );
    //newCyli->setColor( color[0], color[1], color[2], color[3] );
    //dgnSetColor( color );
    
    //theParent->addMember( newCyli );
	printf("start-");
	dgnCreateCylinder(dgn,dfRad,dfHei);
    dgnPopCurrentTransform(dgn);
    printf("end\n");
	//instanceCount();


    return( OKAY );
}

int createPyramid( FILE *fp, DGN dgn)
{
    //Ptg3DPyramidObj *newPyra = new Ptg3DPyramidObj;
    char szLine[MAX_LINE_LENGTH];
	//instanceCount();

    dgnPushCurrentTransform(dgn);
    setTransform( fp,dgn, FALSE );

    readLine( szLine, sizeof(szLine), fp );	/* bounding box */
    readLine( szLine, sizeof(szLine), fp );	/* bounding box */

    readLine( szLine, sizeof(szLine), fp );	/* element parameters */
    double dfXBott = atof(getFixedField( szLine, 14, 0 ));
    double dfYBott = atof(getFixedField( szLine, 14, 1 ));
    double dfXTop  = atof(getFixedField( szLine, 14, 2 ));
    double dfYTop  = atof(getFixedField( szLine, 14, 3 ));

    readLine( szLine, sizeof(szLine), fp );
    double dfXOff  = atof(getFixedField( szLine, 14, 0 ));
    double dfYOff  = atof(getFixedField( szLine, 14, 1 ));
    double dfHeig  = atof(getFixedField( szLine, 14, 2 ));

    //newPyra->setTopDims( dfXTop, dfYTop );
    //newPyra->setBaseDims( dfXBott, dfYBott );
    //newPyra->setOffsets( dfXOff, dfYOff );
    //newPyra->setHeight( dfHeig );    
    //float color[4];
    //theParent->getColor( color );
    //newPyra->setColor( color[0], color[1], color[2], color[3] );
    //dgnSetColor( color);
    
    //theParent->addMember( newPyra );
    printf("start-");

	dgnCreatePyramid(dgn,dfXBott, dfYBott, dfHeig, dfXOff, dfYOff, dfXTop, dfYTop);
    dgnPopCurrentTransform(dgn); 
    printf("end\n");

		//instanceCount();


	return( OKAY );
}

int createRectTorus( FILE *fp , DGN dgn)
{
    //Ptg3DRtorObj *newRtor = new Ptg3DRtorObj;
    char szLine[MAX_LINE_LENGTH];
	//instanceCount();


	dgnPushCurrentTransform(dgn);
    setTransform( fp,dgn, FALSE );

    readLine( szLine, sizeof(szLine), fp );	/* bounding box */
    readLine( szLine, sizeof(szLine), fp );	/* bounding box */

    readLine( szLine, sizeof(szLine), fp );	/* element parameters */
    double dfRins = atof(getFixedField( szLine, 14, 0 ));
    double dfRout = atof(getFixedField( szLine, 14, 1 ));
    double dfHeight = atof(getFixedField( szLine, 14, 2 ));
    double dfAngle = atof(getFixedField( szLine, 14, 3 ));

    //newRtor->setInnerRadius( dfRins );
    //newRtor->setOuterRadius( dfRout );
    //newRtor->setHeight( dfHeight );
    //newRtor->setAngle( RADDEG(dfAngle) );
    //float color[4];
    //theParent->getColor( color );
    //dgnSetColor( color[0], color[1], color[2], color[3] );

    //theParent->addMember( newRtor );
	printf("start-");
    dgnCreateRectTorus(dgn,dfRins,dfRout,dfHeight,dfAngle);
    dgnPopCurrentTransform(dgn);
    printf("end\n");

		//instanceCount();


    return( OKAY );
}


int kount = 0;
int createCircTorus( FILE *fp, DGN dgn )
{
   // Ptg3DCtorObj *newCtor = new Ptg3DCtorObj;
    char szLine[MAX_LINE_LENGTH];

		//instanceCount();


	dgnPushCurrentTransform(dgn);
    setTransform( fp,dgn,FALSE );

    readLine( szLine, sizeof(szLine), fp );	/* bounding box */
    readLine( szLine, sizeof(szLine), fp );	/* bounding box */

    readLine( szLine, sizeof(szLine), fp );	/* element parameters */
    double dfRCenter = atof(getFixedField( szLine, 14, 0 ));
    double dfRadius = atof(getFixedField( szLine, 14, 1 ));
    double dfAngle = atof(getFixedField( szLine, 14, 2 ));

    //newCtor->setInnerRadius( dfRCenter - dfRadius );
    //newCtor->setOuterRadius( dfRCenter + dfRadius );
    //newCtor->setAngle( RADDEG(dfAngle) );
    //float color[4];
    //theParent->getColor( color );
    //dgnSetColor( color[0], color[1], color[2], color[3] );

    //theParent->addMember( newCtor );
//printf("kount = %d\n",++kount);
//if(kount >33)if(kount <35)
	printf("start-");

	 dgnCreateCircTorus(dgn,dfRCenter,dfRadius,dfAngle);

    dgnPopCurrentTransform(dgn);
    printf("end\n");

		//instanceCount();


    return( OKAY );
}

int createSnout( FILE *fp, DGN dgn )
{
    //Ptg3DConeObj *newCone = new Ptg3DConeObj;
    char szLine[MAX_LINE_LENGTH];

		//instanceCount();


	dgnPushCurrentTransform(dgn);
    setTransform( fp,dgn,FALSE );

    readLine( szLine, sizeof(szLine), fp );	/* bounding box */
    readLine( szLine, sizeof(szLine), fp );	/* bounding box */

    readLine( szLine, sizeof(szLine), fp );	/* element parameters */
    double dfBottRad = atof(getFixedField( szLine, 14, 0 ));
    double dfTopRad  = atof(getFixedField( szLine, 14, 1 ));
    double dfHeight  = atof(getFixedField( szLine, 14, 2 ));
    double dfXOff    = atof(getFixedField( szLine, 14, 3 ));
    double dfYOff    = atof(getFixedField( szLine, 14, 4 ));
    readLine( szLine, sizeof(szLine), fp );

    //newCone->setTopRadius( dfTopRad );
    //newCone->setBottomRadius( dfBottRad );
    //newCone->setHeight( dfHeight );
    //newCone->setXOffset( dfXOff );
    //newCone->setYOffset( dfYOff );
    //float color[4];
    //theParent->getColor( color );
    //dgnSetColor( color[0], color[1], color[2], color[3] );

    //theParent->addMember( newCone );
	if(dfHeight != 0.0)
    {
     	printf("start-");

	 dgnCreateOffSetCone(dgn,dfBottRad,dfHeight,dfXOff,dfYOff,dfTopRad);
    dgnPopCurrentTransform(dgn);
	}
    printf("end\n");

		//instanceCount();


    return( OKAY );
}

int createSRect( FILE *fp, DGN dgn )
{
    //Ptg3DBoxObj *newBox = new Ptg3DBoxObj;
    char szLine[MAX_LINE_LENGTH];

    //Ptg3DNode *newNode = new Ptg3DNode;

		//instanceCount();


	dgnPushCurrentTransform(dgn);
	setTransform( fp,dgn,FALSE );

    readLine( szLine, sizeof(szLine), fp );	/* bounding box */
    readLine( szLine, sizeof(szLine), fp );	/* bounding box */

    readLine( szLine, sizeof(szLine), fp );	/* element parameters */
    double dfXLen = atof(getFixedField( szLine, 14, 0 ));
    double dfYLen = atof(getFixedField( szLine, 14, 1 ));
    double dfZLen = atof(getFixedField( szLine, 14, 2 ));
    readLine( szLine, sizeof(szLine), fp );

    //newBox->setXLen( dfXLen );
    //newBox->setYLen( dfYLen );
    //newBox->setZLen( dfZLen );
    //float color[4];
    //theParent->getColor( color );
    //newBox->setColor( color[0], color[1], color[2], color[3] );

    //VECTOR v = vector( 1, 12 );
	double v[13];
    v[1] = v[5] = v[9] = 1.0;
    v[2] = v[3] = v[4] = v[6] = v[7] = v[8] = 0.0;
    v[10] = 0.0;
    v[11] = 0.0;
	v[12] = dfZLen/2.0;

    double xaxis[3],yaxis[3],zaxis[4],origin[3];

    xaxis[0] = v[1];
	xaxis[1] = v[4];
	xaxis[2] = v[7];

	yaxis[0] = v[2];
	yaxis[1] = v[5];
	yaxis[2] = v[8];

    zaxis[0] = v[3];
	zaxis[1] = v[6];
	zaxis[2] = v[9];

	origin[0] = v[10];
	origin[1] = v[11];
	origin[2] = v[12];

    //set currrent coordSystem relative to current system
	printf("start-");

	dgnSetCurrentCoordSystemRel(dgn,xaxis,yaxis,zaxis,origin);




    //newBox->setMatrix( v );
    //theParent->addMember( newNode );
    //free_vector( v, 1, 12 );

    //newNode->addMember( newBox );

	dgnCreateBox(dgn,dfXLen,dfYLen,dfZLen);
    dgnPopCurrentTransform(dgn);

    printf("end\n");
		//instanceCount();


    return( OKAY );
}

int createFacet( FILE *fp, DGN dgn)
{
    char szLine[MAX_LINE_LENGTH];
    //Ptg3DNode *facetGroup = new Ptg3DNode;

		//instanceCount();


	dgnPushCurrentTransform(dgn);
    setTransform( fp,dgn, FALSE );

    readLine( szLine, sizeof(szLine), fp );	/* bounding box */
    readLine( szLine, sizeof(szLine), fp );	/* bounding box */

    readLine( szLine, sizeof(szLine), fp );	/* number of facets */
    int numFacets = atoi(szLine);
    
    for( int i = 0; i < numFacets; i++ )
    {
		readLine( szLine, sizeof(szLine), fp );	/* number of loops (?????) */
		int numLoops = atoi(szLine);
		if( numLoops != 1 )
		{
			fprintf( stderr, "********************* Number of loops is NOT 1 *********************\n" );
			fprintf( stderr, "********************* Keep this review file ************************\n" );
			fprintf( stderr, "********************* And tell somebody that this happened *********\n" );
//			AfxMessageBox( "Number of loops in facet != 1" );
			return TRUE;
		}
		
		readLine( szLine, sizeof(szLine), fp );	/* number of vertices */
		int numVertices = atoi(szLine);
		
		//Ptg3DFacetObj *newFacet = new Ptg3DFacetObj;
		//Ptg3DFacetVertexObj aVertex;
		SHP shape = dgnStartShape(dgn);
		for( int j = 0; j < numVertices; j++ )
		{
			readLine( szLine, sizeof(szLine), fp ); /* vertex */
			//aVertex.setVertex(
			dgnShapeVertex(dgn,shape,
				atof(getFixedField( szLine, 14, 0 )), 
				atof(getFixedField( szLine, 14, 1 )), 
				atof(getFixedField( szLine, 14, 2 )) );

			readLine( szLine, sizeof(szLine), fp ); /* normal */
			//aVertex.setNormal(
			//	atof(getFixedField( szLine, 14, 0 )), 
			//	atof(getFixedField( szLine, 14, 1 )), 
			//	atof(getFixedField( szLine, 14, 2 )) );
			//newFacet->addVertex( aVertex );
		}
		dgnShapeComplete(dgn,shape);
		//facetGroup->addMember(newFacet);
    }
    
    //theParent->addMember( facetGroup );
	dgnPopCurrentTransform(dgn);
	//instanceCount();

    return( OKAY );
}

/*
 * Returns a pointer to a certain fixed width field in a string.
 * 
 * The first field IS 0(zero).
 */
char *getFixedField( char *string, int fieldWidth, int fieldIndex )
{
    static char *lastField = NULL;

    /* Clean up from last time thru */
    if( lastField != NULL )
    {
	free( lastField );
	lastField = NULL;
    }
    
    /* Can't get any fields if don't have input. */
    if( string != NULL )
    {
	/* Don't want to go off end of input. */
	if( fieldIndex * fieldWidth <= strlen(string) )
	{
	    char *currPos = string;
	
	    currPos += (fieldIndex * fieldWidth);
    
	    lastField = (char *)malloc( (fieldWidth+1) * sizeof(char) );
	    memset( lastField, 0, (fieldWidth+1) * sizeof(char) );
	    strncpy( lastField, currPos, fieldWidth );
	}
    }
    
    return( lastField );    
}
