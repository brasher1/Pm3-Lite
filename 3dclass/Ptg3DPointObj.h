#ifndef ___Ptg3DPointObj_h___
#define ___Ptg3DPointObj_h___

#include "Ptg3DNode.h"

class Ptg3DPointObj : public Ptg3DNode {
    ID_TAG(Ptg3DPointObj);

    public:
		Ptg3DPointObj();
		virtual ~Ptg3DPointObj();
		void setPosition( double x, double y, double z, Ptg3DNode *refNode = NULL );
		VECTOR getPosition( Ptg3DNode *refNode = NULL );
		void getPosition( Ptg3DNode *refNode, double &x, double &y, double &z );

    private:
		void initVars();

	protected:
		virtual void drawWireframe();
		virtual void drawSolid();
};

#endif	//#ifndef ___Ptg3DPointObj_h___
