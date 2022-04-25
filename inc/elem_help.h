#ifndef __ELEM_HELP_H__
#define __ELEM_HELP_H__

#include "photog_math.h"
#include "2dedge.h"

int getSelectedEdge(Edge_2D *m_currentEdge, int flag = FALSE);
int getSelectedPoint(Point_2D &point, char *pszImageName = NULL);


// imageoverlay.cpp
int CorrectPnts( char *pszImageName, int nNumPnts, 
	PNTLST plUncorrPnts, PNTLST & plCorrPnts, double reductionFactor );
int FindTMatrix( char *pszBundleID, char *pszImageName, 
				VECTOR vTMatrix);


#endif //__ELEM_HELP_H__