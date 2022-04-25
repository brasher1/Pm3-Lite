#include "Ptg3DCtorObj.h"

ID_NAME(Ptg3DCtorObj);

Ptg3DCtorObj::Ptg3DCtorObj()
{
	initVars();
}

Ptg3DCtorObj::~Ptg3DCtorObj()
{
}

void Ptg3DCtorObj::initVars()
{
    outerRadius = 0.0;
    innerRadius = 0.0;
    angle = 90.0;
}
