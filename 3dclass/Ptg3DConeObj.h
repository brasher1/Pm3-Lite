#ifndef ___Ptg3DConeObj_h___
#define ___Ptg3DConeObj_h___

#include "Ptg3DNode.h"

class Ptg3DConeObj : public Ptg3DNode {
    ID_TAG(Ptg3DBoxObj);

    public:
	Ptg3DConeObj();
	virtual ~Ptg3DConeObj();
	void setTopRadius( double r ) { topRadius = r; }
	void setBottomRadius( double r ) { bottomRadius = r; }
	void setHeight( double h ) { height = h; }
	void setXOffset( double xo ) { xOffset = xo; }
	void setYOffset( double yo ) { yOffset = yo; }

	double getTopRadius() { return topRadius; }
	double getBottomRadius() { return bottomRadius; }
	double getHeight() { return height; }
	double getXOffset() { return xOffset; }
	double getYOffset() { return yOffset; }

    private:
	double topRadius;
	double bottomRadius;
	double height;
	double xOffset;
	double yOffset;

	void initVars();

	protected:
	void drawWireframe();
	void drawSolid();
};

#endif	//#ifndef ___Ptg3DConeObj_h___
