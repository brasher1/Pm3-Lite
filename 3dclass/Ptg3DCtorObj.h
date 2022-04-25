#ifndef ___Ptg3DCtorObj_h___
#define ___Ptg3DCtorObj_h___

#include "Ptg3DNode.h"

class Ptg3DCtorObj : public Ptg3DNode {
    ID_TAG(Ptg3DCtorObj);

    public:
	Ptg3DCtorObj();
	virtual ~Ptg3DCtorObj();
	void setOuterRadius( double r ) { outerRadius = r; }
	void setInnerRadius( double r ) { innerRadius = r; }
	void setAngle( double a ) { angle = a; }
	double getOuterRadius() { return outerRadius; }
	double getInnerRadius() { return innerRadius; }
	double getAngle() { return angle; }

    private:
	double outerRadius;
	double innerRadius;
	double angle;

	void initVars();

	protected:
	void drawWireframe();
	void drawSolid();
};

#endif	//#ifndef ___Ptg3DCtorObj_h___
