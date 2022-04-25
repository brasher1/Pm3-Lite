#ifndef ___PtgCamera_h___
#define ___PtgCamera_h___

#include "mathsubs.h"

class PtgCamera {
    public:
	enum ViewType {
	    VT_ORTHO,
	    VT_PERSPECTIVE
	};

	PtgCamera();
	~PtgCamera();
	void setPosition( const double x, const double y, const double z );

	void setLookAt(const double x, const double y, const double z );
	void setLookDist(const double dist){ lookdist = dist; }
	void setRotation( const double x, const double y, const double z );

	double getLookDist() { return lookdist; }

	void setDirection( const double x, const double y, const double z );
	void setUpDirection( const double x, const double y, const double z );

	void setAspectRatio( const double r );
	void setFieldOfViewY( const double fovy );
	const double *getPosition() const { return( &transform[10] ); };
	void movePosBy( const double x, const double y, const double z );
	void setClipping( const double n, const double f );
	void setOrthoClip( const double left, const double right, const double bottom, const double top );
	void setViewType( const ViewType vt ) { viewType = vt; }
	ViewType getViewType() const { return viewType; }
	void draw();
    
    private:
	double focalLength;
	double aspectRatio;
	double fieldOfViewY;
	VECTOR transform;
	double nearClipDist;
	double farClipDist;

	double alpha, beta, gamma;
	double lookx, looky, lookz;
	double lookdist;
	double leftClip, rightClip;
	double bottomClip, topClip;
	ViewType viewType; 

	void initVars();
	void drawOrtho();
	void drawPerspective();
};

#endif	//#ifndef ___PtgCamera_h___
