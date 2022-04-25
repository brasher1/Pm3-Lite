
/* ele_math.h */
#ifdef PHOTOG_DLL
#define PHOTOG_PORTING		//__declspec(dllexport)
#else
#define PHOTOG_PORTING		//__declspec(dllimport)
#endif

void rotpnt(VECTOR m , PNT p);

PHOTO_FLOAT intersect_lines_dir( VECTOR o1, VECTOR d1, VECTOR o2,
                         VECTOR d2, PHOTO_FLOAT *s1, PHOTO_FLOAT *s2 );

int make_cyl_by_lines(int nl,int *npl, PNTLST *pnts,PNTLST tms, PNTLST drp,
                        PNTLST sep, PNTLST setm , struct Cylinder &cyl,
                        struct _transform &trans);


int make_box_by_lines(int *nll,int *npl, PNTLST *pnts,PNTLST tms,
                        PNTLST sep, PNTLST setm , struct box &box,
                        struct _transform &trans);


int make_cone_by_lines(int nl,int *npl, PNTLST *pnts,PNTLST tms,
                        PNTLST sep, PNTLST setm , struct snout &snout,
                        struct _transform &trans);


int make_ctorus_by_edge(int nNumPnts, PNTLST plPnts,
    PNTLST plTMatrices, struct cir_torus &circtorus_rec, struct _transform &transform_rec );	    


int make_rtorus_by_edge(int *nll,int *npl, PNTLST *pnts,PNTLST tms,
                        PNTLST sep, PNTLST setm , struct rect_torus &rtor,
                        struct _transform &trans);


int make_sphere_by_edge(int nNumPnts, PNTLST plPnts,
    PNTLST plTMatrices, struct sphere &sph, struct _transform &transform_rec );	    


int make_snout_by_circle(int *nll,int *npl, PNTLST *pnts,PNTLST tms, struct snout &snt,
                        struct _transform &trans);

int make_cyl_by_3dpnts(int np,PNTLST pnts,PNTLST sep, PNTLST setm , struct cylinder &cyl,
                        struct _transform &trans);

int MakeCirc( int                 nl,    //number of 2d arcs ( number of photos)
	       int                *npl,   //number of points per each 2d arc,
               PNTLST             *pnts,  // list of 2d points 
	       PNTLST              tms,   // Tmatrix, one per photo;
               double            & radius,    
               struct _transform & trans );

int make_box_by_perp_lines(int *nll,int *npl, PNTLST *pnts,PNTLST tms,
                        PNTLST sep, PNTLST setm , struct box &box,
                        struct _transform &trans);

int MakeLineBy2DLines( int *np2DLinesPer3DLine, 
	int *npPntsPer2DLine, PNTLST *plp2DLinePnts, 
	PNTLST plp2DLineTMatrix, PNTLST plEndPnts,
	PNTLST plEndPntsTMatrix, struct Line2p & line2p );


int make_circle_by_3dpnts(int np,PNTLST pnts,double & radius,struct _transform & trans );

VECTOR prj_pnt_onto_plane(VECTOR pnt, VECTOR pl_org, VECTOR pl_dir);
VECTOR prj_pnt_onto_plane(VECTOR pnt, VECTOR pl_org, VECTOR pl_dir);
VECTOR intersect_line_beamSide(VECTOR cl_org, VECTOR cl_dir, VECTOR sd_dir, PHOTO_FLOAT dist, PHOTO_FLOAT width,
                               VECTOR ln_org, VECTOR ln_dir );
