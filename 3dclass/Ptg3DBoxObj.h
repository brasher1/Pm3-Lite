#ifndef ___Ptg3DBoxObj_h___
#define ___Ptg3DBoxObj_h___

#include "Ptg3DNode.h"

class Ptg3DBoxObj : public Ptg3DNode {
    ID_TAG(Ptg3DBoxObj);

    public:
	Ptg3DBoxObj();
	virtual ~Ptg3DBoxObj();
	void setXLen( double l ) { xlen = l; }
	void setYLen( double l ) { ylen = l; }
	void setZLen( double l ) { zlen = l; }
	double getXLen() { return xlen; }
	double getYLen() { return ylen; }
	double getZLen() { return zlen; }

    private:
	double xlen;
	double ylen;
	double zlen;

	void initVars();

	protected:
	void drawWireframe();
	void drawSolid();
};

#endif	//#ifndef ___Ptg3DBoxObj_h___
