#ifndef ___Ptg3DCylinderObj_h___
#define ___Ptg3DCylinderObj_h___

#include "Ptg3DNode.h"

class Ptg3DCylinderObj : public Ptg3DNode {
    ID_TAG(Ptg3DCylinderObj);

    public:
	Ptg3DCylinderObj();
	virtual ~Ptg3DCylinderObj();
	void setRadius( double r ) { radius = r; }
	void setHeight( double h ) { height = h; }
	double getRadius() { return radius; }
	double getHeight() { return height; }

    private:
	double radius;
	double height;

	void initVars();

	protected:
	void drawWireframe();
	void drawSolid();
};

#endif	//#ifndef ___Ptg3DCylinderObj_h___
