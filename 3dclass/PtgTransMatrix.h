#ifndef ___PtgTransMatrix_h___
#define ___PtgTransMatrix_h___

#include "photog_math.h"
#include "HierElem.h"

class PtgTransMatrix {
    public:
	PtgTransMatrix();
	PtgTransMatrix(VECTOR v);
	~PtgTransMatrix();
	void setMatrix( VECTOR v );
	void setMatrix( PtgTransMatrix &potm );
	VECTOR getMatrix();
	double operator[]( int nIndex );
	void setIdentity();
	void draw();
	
    private:
	VECTOR theMatrix;
	void initVars();
};

#endif	//#ifndef ___PtgTransMatrix_h___
