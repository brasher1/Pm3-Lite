#include "Ptg3DRtorObj.h"

ID_NAME(Ptg3DRtorObj);

Ptg3DRtorObj::Ptg3DRtorObj()
{
	initVars();
}

Ptg3DRtorObj::~Ptg3DRtorObj()
{
}

void Ptg3DRtorObj::initVars()
{
    outerRadius = 0.0;
    innerRadius = 0.0;
    height = 0.0;
    angle = 90.0;
}
