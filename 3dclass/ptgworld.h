#ifndef __PTGWORLD_H__
#define __PTGWORLD_H__
//PtgWorld.h
//
#ifdef WIN32
#include "stdafx.h"
#endif

#include "..\3DClass\Ptg3DNode.h"
//#include "photog.h"
//#include "../photog/output.h"
#include "photog_math.h"
//#include "triton.hpp"


class PtgWorld : public Ptg3DNode
{
	ID_TAG(PtgWorld);
private:
	Ptg3DNode *theSystem;
	Ptg3DNode *theReview;
	Ptg3DNode *theModel;

	Ptg3DNode *theCameras;
	Ptg3DNode *theAxes;
	Ptg3DNode *theTargets;
	Ptg3DNode *theReference;
	Ptg3DNode *theRays;
	Ptg3DNode *theCalculated;
	Ptg3DNode *theColumns;
	Ptg3DNode *theZones;

public:

	PtgWorld();
	~PtgWorld();
/////////////////////////////////////
	Ptg3DNode *getSystem() {return theSystem; }
	Ptg3DNode *getReview() {return theReview; }
	Ptg3DNode *getRays() {return theRays;}
	Ptg3DNode *getModel() {return theModel;}
	Ptg3DNode *getTargets() {return theTargets;}
	Ptg3DNode *getCameras() {return theCameras;}
	Ptg3DNode *getAxes() { return theAxes; }
	Ptg3DNode *getCalculated() { return theCalculated; }
	Ptg3DNode *getMeasurementRefs() { return theReference; }

	DWORD InitWorld(void *word);
	int CreateSystem( Ptg3DNode *parent = NULL );
	int CreateCameras( Ptg3DNode *parent = NULL );
	int CreateTargets( Ptg3DNode *parent = NULL );
	int CreateAxes( Ptg3DNode *parent = NULL );
	int CreateRays( Ptg3DNode *parent = NULL );

	int CreateReview( Ptg3DNode *parent = NULL );

	int CreateReference( Ptg3DNode *parent = NULL );

	int CreateModel( Ptg3DNode *parent = NULL );
	int CreateCalculated();
	int CreateColumns();
	int CreateZones();
	int FindTMatrix( char *pszBundleID, char *pszImageName, VECTOR vTMatrix );
	VECTOR ConvertCameraMatrix( double theMatrix[] );

};

#endif //__PTGWORLD_H__