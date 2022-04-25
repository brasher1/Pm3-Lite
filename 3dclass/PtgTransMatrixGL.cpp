#ifdef WIN32
#include "stdafx.h"
#endif

#include <gl/gl.h>

#include "PtgTransMatrix.h"

void PtgTransMatrix::draw()
{
    float dfMatrix[4][4];

    dfMatrix[0][0] = theMatrix[1];
    dfMatrix[0][1] = theMatrix[2];
    dfMatrix[0][2] = theMatrix[3];
    dfMatrix[0][3] = 0.0;

    dfMatrix[1][0] = theMatrix[4];
    dfMatrix[1][1] = theMatrix[5];
    dfMatrix[1][2] = theMatrix[6];
    dfMatrix[1][3] = 0.0;

    dfMatrix[2][0] = theMatrix[7];
    dfMatrix[2][1] = theMatrix[8];
    dfMatrix[2][2] = theMatrix[9];
    dfMatrix[2][3] = 0.0;

    dfMatrix[3][0] = theMatrix[10];
    dfMatrix[3][1] = theMatrix[11];
    dfMatrix[3][2] = theMatrix[12];
    dfMatrix[3][3] = 1.0;

    multmatrix( dfMatrix );
}
