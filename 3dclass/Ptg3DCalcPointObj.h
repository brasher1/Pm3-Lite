#ifndef ___Ptg3DCalcPointObj_h___
#define ___Ptg3DCalcPointObj_h___

#include "Ptg3DPointObj.h"

class Ptg3DCalcPointObj : public Ptg3DPointObj {
    ID_TAG(Ptg3DCalcPointObj);

    public:
		Ptg3DCalcPointObj();
		virtual ~Ptg3DCalcPointObj();
		void initVars();

		void setStdDev( double x, double y, double z );
		void setRayMiss( double rm );

		VECTOR getStdDev( void ){ return clone_vector( stdDev, 1, 3, 1, 3 ); }
		double getRayMiss( void ){ return rayMiss; }

	private:
		VECTOR stdDev;
		double rayMiss;
};

#endif	//#ifndef ___Ptg3DCalcPointObj_h___
