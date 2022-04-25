#include "Ptg3DCalcPointObj.h"

ID_NAME(Ptg3DCalcPointObj);

Ptg3DCalcPointObj::Ptg3DCalcPointObj()
{
    initVars();
}

Ptg3DCalcPointObj::~Ptg3DCalcPointObj()
{
    free_vector( stdDev, 1, 3 );
}

void Ptg3DCalcPointObj::initVars()
{
    stdDev = vector( 1, 3 );
	rayMiss = 0.0;
}

void Ptg3DCalcPointObj::setStdDev( double x, double y, double z )
{
	stdDev[1] = x;
	stdDev[2] = y;
	stdDev[3] = z;
}

void Ptg3DCalcPointObj::setRayMiss( double rm )
{
	rayMiss = rm;
}
