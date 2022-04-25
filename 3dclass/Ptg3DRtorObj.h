#ifndef ___Ptg3DRtorObj_h___
#define ___Ptg3DRtorObj_h___

#include "Ptg3DNode.h"

class Ptg3DRtorObj : public Ptg3DNode {
    ID_TAG(Ptg3DRtorObj);

    public:
	Ptg3DRtorObj();
	virtual ~Ptg3DRtorObj();
	void setOuterRadius( double r ) { innerRadius = r; }
	void setInnerRadius( double r ) { outerRadius = r; }
	void setHeight( double h ) { height = h; }
	void setAngle( double a ) { angle = a; }
	double getOuterRadius() { return outerRadius; }
	double getInnerRadius() { return innerRadius; }
	double getHeight() { return height; }
	double getAngle() { return angle; }

    private:
	double outerRadius;
	double innerRadius;
	double height;
	double angle;

	void initVars();

	protected:
	void drawWireframe();
	void drawSolid();
};

#endif	//#ifndef ___Ptg3DRtorObj_h___
