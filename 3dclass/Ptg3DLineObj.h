#ifndef ___Ptg3DLineObj_h___
#define ___Ptg3DLineObj_h___

#include "Ptg3DNode.h"

class Ptg3DLineObj : public Ptg3DNode {
    ID_TAG(Ptg3DLineObj);

    public:
	Ptg3DLineObj();
	virtual ~Ptg3DLineObj();
	void setLength( double dfL );
	void setOrigin( double x, double y, double z );
	void setDirection( double x, double y, double z );
	void setEndPoint( double x, double y, double z );

    private:
	VECTOR vOrigin;
	VECTOR vDirection;
	double dfLength;
	
	void initVars();

	protected:
	virtual void drawWireframe();
	virtual void drawSolid();
};

#endif	//#ifndef ___Ptg3DLineObj_h___
