/* ToDgnFunctions.h */


#define FALSE 0

#define TRUE  1

typedef void* DGN;
typedef void* SHP;

 DGN openDgnFile(char* dgnFileName);

 void closeDgnFile(DGN dgn);

 void  dgnCreateCurrentNode(DGN dgn,char* nodeName, long nodeID) ;
 void  dgnPushNode(DGN dgn);
 void  dgnPopNode(DGN dgn);
 void  dgnPushCurrentTransform(DGN dgn);
 void  dgnPopCurrentTransform(DGN dgn);
 void dgnSetColor(DGN dgn,float* color );

 void dgnSetCurrentCoordSystemAbs(DGN dgn,double* xaxis,double* yaxis,double* zaxis,double* origin);
 void dgnSetCurrentCoordSystemRel(DGN dgn,double* xaxis,double* yaxis,double* zaxis,double* origin);

 void dgnCreateBox(DGN dgn,double Xlen,double YLen,double Zlen);
 void dgnCreateCylinder(DGN dgn,double Radius,double Height);
 void dgnCreatePyramid(DGN dgn,double XBott,double YBott, 
	                   double dfHeigt, double XOffSet, double dfYOffSet, 
					   double XTop, double YTop);

void dgnCreateRectTorus(DGN dgn,double innerRadius,double outerRadius,
						double Height, double radianAngle);

void dgnCreateCircTorus(DGN dgn,double torusRadius, double crossectionRadius, double sweepangle);

void dgnCreateCone(DGN dgn,double BottRad,double Height,double XOffSet,double YOffSet, double TopRad);
void dgnCreateOffSetCone(DGN dgn,double BottRad,double Height,
						 double XOffSet,double YOffSet,double TopRad);

SHP dgnStartShape(DGN dgn);
void dgnShapeVertex(DGN dgn,SHP shape,double x, double Y, double Z);
void dgnShapeComplete(DGN dgn,SHP shape);

 void setUORConversion(DGN dgn, double uorsPer,char* units);

 void instanceCount();