#ifndef _MATHSUBS_H_
#define _MATHSUBS_H_

#include <malloc.h>
#include <stdio.h>
#include <math.h>

#ifdef PHOTOG_DLL
#define PHOTOG_PORTING		//__declspec(dllexport)
#else
#define PHOTOG_PORTING		//__declspec(dllimport)
#endif

#define TINY  1.0e-20
#define SMALL 1.0e-13

#define PHOTO_FLOAT double

typedef PHOTO_FLOAT *VECTOR;
typedef VECTOR PNT;
typedef PHOTO_FLOAT **MATRIX;
typedef MATRIX *PNTLST;
typedef MATRIX **MATLST;

static PHOTO_FLOAT sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)

static PHOTO_FLOAT dmaxarg1,dmaxarg2;
#define FMAX(a,b) (dmaxarg1=(a),dmaxarg2=(b),(dmaxarg1)>(dmaxarg2) ? (dmaxarg1):(dmaxarg2))

//static PHOTO_FLOAT dminarg1,dminarg2;
//#define FMIN(a,b) (dminarg1=(a),dminarg2=(b),(dminarg1)<(dminarg2) ? (dminarg1):(dminarg2))

static int iminarg1, iminarg2;
#define IMIN(a,b) (iminarg1 = (a),iminarg2 = (b),(iminarg1)<(iminarg2)? (iminarg1):(iminarg2))

#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : - fabs(a) )
PHOTOG_PORTING VECTOR matmat(VECTOR m1,VECTOR m2);
VECTOR matinv(VECTOR m);
long memcount(int d);
void *myalloc(unsigned size);
void myfree(char* mem);
void nrerror(char *error_text);
void clear_vector(PHOTO_FLOAT *v,int nl,int nh);
PHOTOG_PORTING PHOTO_FLOAT *vector(int nl,int nh);
VECTOR getinittm(int np,PNTLST p1,PNTLST p2);
VECTOR print_vector(VECTOR v,int nl,int nh);
MATRIX *print_matrix(MATRIX *m,int nrl,int nrh,int ncl,int nch);
void add_vector(PHOTO_FLOAT *to,int l,int h, PHOTO_FLOAT *from,int fl,int fh);
void subtract_vector(PHOTO_FLOAT *to,int l,int h, PHOTO_FLOAT *from,int fl,int fh);
PHOTOG_PORTING void copy_vector(PHOTO_FLOAT *to,int l,int h, PHOTO_FLOAT *from,int fl,int fh);
PHOTOG_PORTING PHOTO_FLOAT *clone_vector(PHOTO_FLOAT *v,int nl,int nh,int newl,int newh);
PHOTO_FLOAT *subvector(PHOTO_FLOAT *v,int oldl,int oldh,int newl);
PHOTOG_PORTING void free_vector(PHOTO_FLOAT* &v,int nl,int nh);
void clear_matrix(MATRIX *m,int nrl,int nrh,int ncl,int nch);
PHOTO_FLOAT *cross_vector(PHOTO_FLOAT *a,PHOTO_FLOAT *b);
PHOTO_FLOAT dot_vector(int n,PHOTO_FLOAT *a,PHOTO_FLOAT *b);
PHOTO_FLOAT vector_length(int n,PHOTO_FLOAT *a);
void multvector(int n,PHOTO_FLOAT *a, PHOTO_FLOAT p);
PHOTO_FLOAT *unit_vector(int n,PHOTO_FLOAT *a);

PHOTOG_PORTING MATRIX *matrix(int nrl,int nrh,int ncl,int nch);
void copy_matrix(MATRIX *to,int rl1,int rh1,int cl1,int ch1,
                 MATRIX *fr,int rl2,int rh2,int cl2,int ch2);
MATRIX *clone_matrix(MATRIX *m,int rl,int rh,int cl,int ch,
                             int nrl,int nrh,int ncl,int nch);
PHOTOG_PORTING void free_matrix(MATRIX* &m,int nrl,int nrh,int ncl,int nch);
MATRIX* submatrix(MATRIX *a,int oldrl,int oldrh,int oldcl,int oldch,
                            int newrl,int newcl);
void free_submatrix(MATRIX* &m,int nrl,int nrh,int ncl,int nch);
void normalize_matrix(PHOTO_FLOAT *ax,PHOTO_FLOAT *ay,PHOTO_FLOAT *az);
MATRIX *transpose(MATRIX *m,int nrl,int nrh,int ncl,int nch);
PHOTOG_PORTING MATRIX *matrix_mult( MATRIX *m1,int nrl1,int nrh1,int ncl1,int nch1,
                     MATRIX *m2,int nrl2,int nrh2,int ncl2,int nch2,
                     int nrl,int nrh,int ncl,int nch);
PHOTO_FLOAT dot(PHOTO_FLOAT *v1,int l1,int h1, PHOTO_FLOAT *v2,int l2,int h2);
PHOTO_FLOAT *matrix_vector(MATRIX *m,int rl,int rh,int cl,int ch,
                     PHOTO_FLOAT *v,int l,int h);
void ludcmp(PHOTO_FLOAT **a,int n,int *indx,PHOTO_FLOAT *d);
void lubksb(PHOTO_FLOAT **a,int n,int *indx, PHOTO_FLOAT *b);
void mprove(PHOTO_FLOAT **a,PHOTO_FLOAT **alud,int n,int *indx,PHOTO_FLOAT *b,PHOTO_FLOAT *x);
void pivit(short dim, double **A);
PHOTO_FLOAT *solve(MATRIX *a,PHOTO_FLOAT *L, int n);
PHOTO_FLOAT *lsq(MATRIX *A,PHOTO_FLOAT*L,int nu,int ne);
MATRIX *matrix_shell(int nrl,int nrh,int ncl, int nch);
int add_matrix_rows( MATRIX *mp, int nr);
int add_matrix_shell_rows( MATRIX *mp, int nr);
void free_matrix_shell(MATRIX* &m);
int get_matrix_nr(MATRIX *m);
int get_matrix_rl(MATRIX *m);
int get_matrix_rh(MATRIX *m);
void svdcmp(PNTLST a, int nr, int nc, VECTOR w, PNTLST v);
PHOTO_FLOAT pythag(PHOTO_FLOAT a, PHOTO_FLOAT b);
void svdvar(PNTLST v, int ma, VECTOR w, PNTLST cvm);
void svdfit(PNTLST A, VECTOR y, VECTOR sig, int ndata, VECTOR a, int ma,
            PNTLST u, PNTLST v, VECTOR w, PHOTO_FLOAT *chisq,
            void (*funcs)(PHOTO_FLOAT,PNTLST, int)  );
void svbksb(PNTLST u, VECTOR w, PNTLST v, int m, int n, VECTOR b, VECTOR x);
VECTOR decomp_matrix(VECTOR RR,VECTOR s);
VECTOR get_matrix_scales(VECTOR RR);
void BBlsq(PNTLST ATAx, PNTLST P, PNTLST Q, PNTLST S,VECTOR AWT, VECTOR s, VECTOR RR,
     int nu,int ne,int nf,int np, VECTOR photosum, VECTOR pntsum, PNTLST *fp,int nff,int npf, PHOTO_FLOAT *chisq);
void choldc(PNTLST a, int n, VECTOR p);
void choldcinv(PNTLST mat, int size);
void pnt2film(PHOTO_FLOAT fl, VECTOR tm, VECTOR pnt, PHOTO_FLOAT* x, PHOTO_FLOAT* y);
void pnt2film_error(PHOTO_FLOAT fl, VECTOR tm, VECTOR pnt,VECTOR pp, PHOTO_FLOAT* dx, PHOTO_FLOAT* dy);

#endif
