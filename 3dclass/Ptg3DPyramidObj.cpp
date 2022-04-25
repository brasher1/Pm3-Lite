#include "Ptg3DPyramidObj.h"

ID_NAME(Ptg3DPyramidObj);

Ptg3DPyramidObj::Ptg3DPyramidObj()
{
    initVars();
}

Ptg3DPyramidObj::~Ptg3DPyramidObj()
{
}

void Ptg3DPyramidObj::initVars()
{
    dfHeight = 0.0;
    dfBaseWidth = dfBaseDepth = 0.0;
    dfTopWidth = dfTopDepth = 0.0;
    dfWidthOffset = dfDepthOffset = 0.0;
}

void Ptg3DPyramidObj::setHeight( double h )
{
    dfHeight = h;
}

void Ptg3DPyramidObj::setBaseDims( double w, double d )
{
    dfBaseWidth = w;
    dfBaseDepth = d;
}

void Ptg3DPyramidObj::setTopDims( double w, double d )
{
    dfTopWidth = w;
    dfTopDepth = d;
}

void Ptg3DPyramidObj::setOffsets( double w, double d )
{
    dfWidthOffset = w;
    dfDepthOffset = d;
}
