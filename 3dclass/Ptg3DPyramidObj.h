#ifndef ___Ptg3DPyramidObj_h___
#define ___Ptg3DPyramidObj_h___

#include "Ptg3DNode.h"

class Ptg3DPyramidObj : public Ptg3DNode {
    ID_TAG(Ptg3DPyramidObj);

    public:
	Ptg3DPyramidObj();
	virtual ~Ptg3DPyramidObj();
	void setHeight( double h );
	void setBaseDims( double w, double d );
	void setTopDims( double w, double d );
	void setOffsets( double w, double d );

	double getHeight(){ return dfHeight; }
	double getBaseWidth(){ return dfBaseWidth; }
	double getBaseDepth(){ return dfBaseDepth; }
	double getTopWidth(){ return dfTopWidth; }
	double getTopDepth(){ return dfTopDepth; }
	double getWidthOffset(){ return dfWidthOffset; }
	double getDepthOffset(){ return dfDepthOffset; }

    private:
	double dfHeight;
	double dfBaseWidth, dfBaseDepth;
	double dfTopWidth, dfTopDepth;
	double dfWidthOffset, dfDepthOffset;

	void initVars();

	protected:
	void drawWireframe();
	void drawSolid();
};

#endif	//#ifndef ___Ptg3DPyramidObj_h___
