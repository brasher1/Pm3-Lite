#include "Ptg3DLineObj.h"

ID_NAME(Ptg3DLineObj);

Ptg3DLineObj::Ptg3DLineObj()
{
    initVars();
}

Ptg3DLineObj::~Ptg3DLineObj()
{
    free_vector( vOrigin, 1, 3 );
    free_vector( vDirection, 1, 3 );
}

void Ptg3DLineObj::initVars()
{
    dfLength = 0.0;
    vOrigin = vector( 1, 3 );
    vDirection = vector( 1, 3 );
}

void Ptg3DLineObj::setLength( double dfL )
{
    dfLength = dfL;
}

void Ptg3DLineObj::setOrigin( double x, double y, double z )
{
    vOrigin[1] = x;
    vOrigin[2] = y;
    vOrigin[3] = z;
}

void Ptg3DLineObj::setDirection( double x, double y, double z )
{
    double total = sqrt(x*x + y*y + z*z);
    vDirection[1] = x/total;
    vDirection[2] = y/total;
    vDirection[3] = z/total;
}

void Ptg3DLineObj::setEndPoint( double x, double y, double z )
{
    double delta[3];

    delta[0] = x - vOrigin[1];
    delta[1] = y - vOrigin[2];
    delta[2] = z - vOrigin[3];
    setDirection( delta[0], delta[1], delta[2] );
    setLength( sqrt( delta[0]*delta[0] + delta[1]*delta[1] +delta[2]*delta[2] ) );
}
