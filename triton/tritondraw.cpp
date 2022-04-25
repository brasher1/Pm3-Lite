//tritondraw.cpp
//
#include "stdafx.h"
#include "triton.hpp"

AllElements::AllElements():Polymorph(ELEMENT_CLASSES, 11)
{
	    // BETTER WATCH OUT FOR THESE DEFINES
	    // THEIR KILLERS
    rtn = new int[LINE_REC-TRANSFORM+1];
    
// **CAUTION CAUTION CAUTION CAUTION*******************//
    Register(Box);
    rtn[BOX-TRANSFORM] = 0;//BOY YOU BETTER ADD THIS LINE TO
			     // ANY OTHER CLASSES YOU REGISTER
    Register(CirTorus);
    rtn[CIRTORUS-TRANSFORM] = 1;
    Register(Cone);	
    rtn[CONE-TRANSFORM] = 2;
    Register(Cylinder);
    rtn[CYLINDER-TRANSFORM] = 3;
    Register(RectTorus);
    rtn[RECTTORUS-TRANSFORM] = 4;
    Register(Sphere);
    rtn[SPHERE-TRANSFORM] = 5;
    Register(Pyramid);
    rtn[PYRAMID-TRANSFORM] = 6;
    Register(Complex);
    rtn[COMPLEX-TRANSFORM] = 7;
    Register(Point);
    rtn[POINT_REC-TRANSFORM-2] = 8;
    Register(Line);
    rtn[LINE_REC-TRANSFORM-2] = 9;
}
// IF YOU ADD ANY MORE TYPES PLEASE INCREASE THE SECOND ARG //
/*
#define TBOX 10004
#define TCIRTORUS 10005
#define TCONE 10006
#define TCYLINDER 10007
#define TRECTTORUS 10008
#define TSPHERE 10009
#define TPYRAMID 10010
#define TSNOUT 10011
#define TCOMPLEX 10012
#define TPOINT 10015
#define TLINE 10016
*/
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
AllElements::~AllElements()
{
	if( rtn != NULL )
		delete rtn;
}
