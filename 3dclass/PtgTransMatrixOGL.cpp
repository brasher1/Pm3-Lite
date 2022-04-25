#ifdef WIN32
#include "stdafx.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "PtgTransMatrix.h"

/*
 * The transformation stored in the Node class is from child Node to parent Node.
 * It should be noted that the stored matrix will be mapped directly to the OpenGL
 * matrix as defined by OpenGL (i.e. storedMatrix[1] = OpenGL[0],
 * storedMatrix[2] = OpenGL[1],  etc.).  The stored matrix transform
 * components( 10,11,12) are mapped to OpenGL elements 12, 13, 14.  This is the
 * transpose of the matrix ordering defined for other photogrammetry math operations.
 */

void PtgTransMatrix::draw()
{
	if( theMatrix != NULL )
	{
		double dfMatrix[16];
		memset( dfMatrix, 0, sizeof(dfMatrix) );
    
#if FALSE
		dfMatrix[0] = theMatrix[1];
		dfMatrix[1] = theMatrix[2];
		dfMatrix[2] = theMatrix[3];
		dfMatrix[3] = 0.0;

		dfMatrix[4] = theMatrix[4];
		dfMatrix[5] = theMatrix[5];
		dfMatrix[6] = theMatrix[6];
		dfMatrix[7] = 0.0;

		dfMatrix[8] = theMatrix[7];
		dfMatrix[9] = theMatrix[8];
		dfMatrix[10] = theMatrix[9];
		dfMatrix[11] = 0.0;

		dfMatrix[12] = theMatrix[10];
		dfMatrix[13] = theMatrix[11];
		dfMatrix[14] = theMatrix[12];
		dfMatrix[15] = 1.0;
#endif

//#if FALSE
		dfMatrix[0] = theMatrix[1];
		dfMatrix[1] = theMatrix[4];
		dfMatrix[2] = theMatrix[7];
		dfMatrix[3] = 0.0;

		dfMatrix[4] = theMatrix[2];
		dfMatrix[5] = theMatrix[5];
		dfMatrix[6] = theMatrix[8];
		dfMatrix[7] = 0.0;

		dfMatrix[8] = theMatrix[3];
		dfMatrix[9] = theMatrix[6];
		dfMatrix[10] = theMatrix[9];
		dfMatrix[11] = 0.0;

		dfMatrix[12] = theMatrix[10];
		dfMatrix[13] = theMatrix[11];
		dfMatrix[14] = theMatrix[12];
		dfMatrix[15] = 1.0;

		glMultMatrixd( dfMatrix );
//#endif
	}
}
