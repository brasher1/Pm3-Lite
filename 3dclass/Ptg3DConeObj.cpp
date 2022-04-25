#include "Ptg3DConeObj.h"

ID_NAME(Ptg3DConeObj);

Ptg3DConeObj::Ptg3DConeObj()
{
	initVars();
}

Ptg3DConeObj::~Ptg3DConeObj()
{
}

void Ptg3DConeObj::initVars()
{
    topRadius = 0.0;
    bottomRadius = 0.0;
    height = 0.0;
    xOffset = 0.0;
    yOffset = 0.0;
}
