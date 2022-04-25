#ifndef ___Ptg3DFacetObj_h___
#define ___Ptg3DFacetObj_h___

#include "Ptg3DNode.h"

class Ptg3DFacetVertexObj {
    public:
	Ptg3DFacetVertexObj();
	Ptg3DFacetVertexObj( Ptg3DFacetVertexObj &aVertex );
	virtual ~Ptg3DFacetVertexObj();
	
	void setVertex( double x, double y, double z );
	void setNormal( double x, double y, double z );

	VECTOR getVertex( void );
	VECTOR getNormal( void );
    private:
	VECTOR vertex;
	VECTOR normal;
	
	void initVars();
};

class Ptg3DFacetObj : public Ptg3DNode {
    ID_TAG(Ptg3DFacetObj);

    public:
	Ptg3DFacetObj();
	virtual ~Ptg3DFacetObj();
	
	void addVertex( Ptg3DFacetVertexObj &newVertex );

    private:
	CList<Ptg3DFacetVertexObj *, Ptg3DFacetVertexObj *> vertices;
    
	void initVars();

	protected:
	void drawWireframe();
	void drawSolid();
};

#endif
