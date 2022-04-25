/* DgnFunctions.h */

#define DLLIMPORT	__declspec( dllimport )
#define DLLEXPORT	__declspec( dllexport )

#define FALSE 0

#define TRUE  1

typedef void* DGN;
typedef void* SHP;

 DLLIMPORT DGN openDgnFile(char* dgnFileName);

 DLLIMPORT void closeDgnFile(DGN dgn);

 DLLIMPORT void  dgnCreateCurrentNode(DGN dgn,char* nodeName, long nodeID) ;
 DLLIMPORT void  dgnPushNode(DGN dgn);
 DLLIMPORT void  dgnPopNode(DGN dgn);
 DLLIMPORT void  dgnPushCurrentTransform(DGN dgn);
 DLLIMPORT void  dgnPopCurrentTransform(DGN dgn);
 DLLIMPORT void dgnSetColor(DGN dgn,float* color );

 DLLIMPORT void dgnSetCurrentCoordSystemAbs(DGN dgn,double* xaxis,double* yaxis,double* zaxis,double* origin);
 DLLIMPORT void dgnSetCurrentCoordSystemRel(DGN dgn,double* xaxis,double* yaxis,double* zaxis,double* origin);

 DLLIMPORT void dgnCreateBox(DGN dgn,double Xlen,double YLen,double Zlen);
 DLLIMPORT void dgnCreateCylinder(DGN dgn,double Radius,double Height);
 DLLIMPORT void dgnCreatePyramid(DGN dgn,double XBott,double YBott, 
	                   double dfHeigt, double XOffSet, double dfYOffSet, 
					   double XTop, double YTop);

DLLIMPORT void dgnCreateRectTorus(DGN dgn,double innerRadius,double outerRadius,
						double Height, double radianAngle);

DLLIMPORT void dgnCreateCircTorus(DGN dgn,double torusRadius, double crossectionRadius, double sweepangle);

DLLIMPORT void dgnCreateCone(DGN dgn,double BottRad,double Height,double XOffSet,double YOffSet, double TopRad);
DLLIMPORT void dgnCreateOffSetCone(DGN dgn,double BottRad,double Height,
						 double XOffSet,double YOffSet,double TopRad);

DLLIMPORT SHP dgnStartShape(DGN dgn);
DLLIMPORT void dgnShapeVertex(DGN dgn,SHP shape,double x, double Y, double Z);
DLLIMPORT void dgnShapeComplete(DGN dgn,SHP shape);

 DLLIMPORT void setUORConversion(DGN dgn, double uorsPer,char* units);

 //DLLIMPORT void instanceCount();