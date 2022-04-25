 
/* mathsubs.c */

#include <malloc.h>
#include <stdio.h>
#include <math.h>
#define PHOTO_FLOAT double
#include "mathsubs.h"
#include "photog_math.h"
#define TINY  1.0e-20
#define SMALL 1.0e-13
#include <setjmp.h>





int n_vect = 0;
int n_mat =0;
extern jmp_buf env;

VECTOR matmat(VECTOR m1,VECTOR m2)
{
  VECTOR r;

  r=vector(1,13);

  r[1] = m1[1]*m2[1] + m1[2]*m2[4] + m1[3]*m2[7];
  r[2] = m1[1]*m2[2] + m1[2]*m2[5] + m1[3]*m2[8];
  r[3] = m1[1]*m2[3] + m1[2]*m2[6] + m1[3]*m2[9];

  r[4] = m1[4]*m2[1] + m1[5]*m2[4] + m1[6]*m2[7];
  r[5] = m1[4]*m2[2] + m1[5]*m2[5] + m1[6]*m2[8];
  r[6] = m1[4]*m2[3] + m1[5]*m2[6] + m1[6]*m2[9];

  r[7] = m1[7]*m2[1] + m1[8]*m2[4] + m1[9]*m2[7];
  r[8] = m1[7]*m2[2] + m1[8]*m2[5] + m1[9]*m2[8];
  r[9] = m1[7]*m2[3] + m1[8]*m2[6] + m1[9]*m2[9];

  r[10] = m1[1]*m2[10] + m1[2]*m2[11] + m1[3]*m2[12] + m1[10];
  r[11] = m1[4]*m2[10] + m1[5]*m2[11] + m1[6]*m2[12] + m1[11];
  r[12] = m1[7]*m2[10] + m1[8]*m2[11] + m1[9]*m2[12] + m1[12];
  
  r[13] = 1.0;
  return(r);
}


VECTOR matinv(VECTOR m)
{
  VECTOR r;
  r=vector(1,12);
  
  PNTLST mat = matrix(1,3,1,3);
 
  (*mat)[1][1] = m[1];
  (*mat)[1][2] = m[4];
  (*mat)[1][3] = m[7];

  (*mat)[2][1] = m[2];
  (*mat)[2][2] = m[5];
  (*mat)[2][3] = m[8];

  (*mat)[3][1] = m[3];
  (*mat)[3][2] = m[6];
  (*mat)[3][3] = m[9];

  PNTLST id = matrix(1,3,1,3);

  (*id)[1][1] = 1.0;
  (*id)[2][2] = 1.0;
  (*id)[3][3] = 1.0;

//PNTLST LUK(int nrk, int nck, int nre, PNTLST teq, PNTLST tkn)
  PNTLST matinv = LUK(3, 3, 3, id, mat);


  r[1]=(*matinv)[1][1];
  r[2]=(*matinv)[1][2];
  r[3]=(*matinv)[1][3];

  r[4]=(*matinv)[2][1];
  r[5]=(*matinv)[2][2];
  r[6]=(*matinv)[2][3];

  r[7]=(*matinv)[3][1];
  r[8]=(*matinv)[3][2];
  r[9]=(*matinv)[3][3];

  r[10] = -(r[1]*m[10] + r[2]*m[11] + r[3]*m[12]);
  r[11] = -(r[4]*m[10] + r[5]*m[11] + r[6]*m[12]);
  r[12] = -(r[7]*m[10] + r[8]*m[11] + r[9]*m[12]);

  free_matrix(mat,1,3,1,3);
  free_matrix(id,1,3,1,3);
  free_matrix(matinv,1,3,1,3);


  return(r);
}

VECTOR matinv(PNTLST L)
{
  VECTOR v = vector(1,12);
  
  v[1] = (*L)[1][1];
  v[2] = (*L)[1][2];
  v[3] = (*L)[1][3];
  v[4] = (*L)[2][1];
  v[5] = (*L)[2][2];
  v[6] = (*L)[2][2];
  v[7] = (*L)[3][1];
  v[8] = (*L)[3][2];
  v[9] = (*L)[3][3];
  v[10] = 0.0;
  v[11] = 0.0;
  v[12] = 0.0;

  VECTOR r = matinv(v);
  free_vector(v,1,12);

  return(r);
}




VECTOR decomp_matrix(VECTOR RR,VECTOR s)
{ 
   PNTLST a = matrix(1,3,1,3);
  (*a)[1][1] = RR[1];
  (*a)[1][2] = RR[2];
  (*a)[1][3] = RR[3];
  (*a)[2][1] = RR[4];
  (*a)[2][2] = RR[5];
  (*a)[2][3] = RR[6];
  (*a)[3][1] = RR[7];
  (*a)[3][2] = RR[8];
  (*a)[3][3] = RR[9];

  VECTOR w=vector(1,3);

  PNTLST v=matrix(1,3,1,3);

  svdcmp(a,3,3,w,v);

//printf("decomp_matrix input matrix");
//print_matrix(a,1,3,1,3);

  s[1] = w[1];
  s[2] = w[2];
  s[3] = w[3];

  VECTOR vt = vector(1,12);
  vt[1] = (*v)[1][1];
  vt[2] = (*v)[2][1];
  vt[3] = (*v)[3][1];
  vt[4] = (*v)[1][2];
  vt[5] = (*v)[2][2];
  vt[6] = (*v)[3][2];
  vt[7] = (*v)[1][3];
  vt[8] = (*v)[2][3];
  vt[9] = (*v)[3][3];

  VECTOR rr = vector(1,12);
  rr[1] = (*a)[1][1];
  rr[2] = (*a)[1][2];
  rr[3] = (*a)[1][3];
  rr[4] = (*a)[2][1];
  rr[5] = (*a)[2][2];
  rr[6] = (*a)[2][3];
  rr[7] = (*a)[3][1];
  rr[8] = (*a)[3][2];
  rr[9] = (*a)[3][3];
  
  VECTOR ortho = matmat(rr,vt);

//  printf("rr vector");
//  print_vector(rr,1,12);

//  printf("rotation matrix");
//  print_vector(vt,1,12);

//  printf("ortho vector");
//  print_vector(ortho,1,12);

  free_vector(rr,1,12);

  free_vector(vt,1,12);
  free_matrix(v,1,3,1,3);
  free_matrix(a,1,3,1,3);

  return(ortho);
}




VECTOR get_matrix_scales(VECTOR RR)
{

  VECTOR s = vector(1,3);
  
  VECTOR ortho = decomp_matrix( RR, s);

  free_vector(ortho,1,3);
 
  return(s);
}



PHOTO_FLOAT pythag(PHOTO_FLOAT a, PHOTO_FLOAT b)
{
// computes sqrt(a*a + b*b) without destructive underflow of overflow
  PHOTO_FLOAT absa,absb;
  PHOTO_FLOAT results;

//printf("pythag a=%e,  b=%e\n",a,b);
  absa=fabs(a);
  absb=fabs(b);
  if(absa >absb)
  {
    return( absa*sqrt( 1.0 + SQR(absb/absa)));
  }
  else
  {
    return( absb == 0.0 ? 0.0 : absb*sqrt(1.0+SQR(absa/absb)) );
  }

}



int getquad(PNT p)
{
  if(p[1] > 0.0)
  {
    if(p[3] > 0.0)
    {
      return(1);
    }
    else
    {
      return(4);
    }
  }
  else
  {
    if(p[3] > 0.0) 
    {
      return(2);
    }
    else
    {
      return(3);
    }
  }
}


VECTOR getinitrot(VECTOR p1,VECTOR p2)
{
   // p1 to p2;

  int q1 = getquad(p1);
  int q2 = getquad(p2);
  VECTOR m = vector(1,13);
  PHOTO_FLOAT d1 = sqrt(p1[1]*p1[1] + p1[2]*p1[2]);
  PHOTO_FLOAT d2 = sqrt(p2[1]*p2[1] + p2[2]*p2[2]);

  q1 = q2-q1;

  if(q1 < 0 )q1=4+q1;

  if(q1 == 0)
  {
    m[1] = 1.0;  
    m[9] = 1.0;
  }

  if(q1 == 1)
  {
    m[3] = -1.0;  
    m[7] = 1.0;
  }
      
  if(q1 == 2)
  {
    m[1] = -1.0;  
    m[9] = -1.0;
  }

  if(q1 == 3)
  {
    m[3] = 1.0;  
    m[7] = -1.0;
  }

  m[5] = 1.0;
    
  m[13] = d1/d2;

  return(m);
}   



VECTOR getinittm(int np,PNTLST p1_p, PNTLST p2_p)
{
  
  VECTOR m;
  VECTOR dp1 = vector(1,3);
  VECTOR dp2 = vector(1,3);
  MATRIX p1 = *p1_p;
  MATRIX p2 = *p2_p;

  dp1[1] = p1[2][1] - p1[1][1];
  dp1[2] = p1[2][2] - p1[1][2];
  dp1[3] = p1[2][3] - p1[1][3];

  dp2[1] = p2[2][1] - p2[1][1];
  dp2[2] = p2[2][2] - p2[1][2];
  dp2[3] = p2[2][3] - p2[1][3];

  m=getinitrot(dp1,dp2);


  m[10] = m[13]*p2[1][1] - (m[1]*p1[1][1] + m[2]*p1[1][2] + m[3]*p1[1][3]);
  m[11] = m[13]*p2[1][2] - (m[4]*p1[1][1] + m[5]*p1[1][2] + m[6]*p1[1][3]);
  m[12] = m[13]*p2[1][3] - (m[7]*p1[1][1] + m[8]*p1[1][2] + m[9]*p1[1][3]);

  free_vector(dp1,1,3);
  free_vector(dp2,1,3);

  return(m);
}


long memcount(int d)
{
  static long memalloced=0;
  memalloced += d;
  if(memalloced < 0)
  {
     printf(" memalloced negative \n");
     getchar();
  }
  return(memalloced);
}

void *myalloc(unsigned size)
{
  static myalloc_init_flag = 0;
 

  void *mem;
/* printf("\nmemory %d.%ld\n",size,memalloced); */

  if(myalloc_init_flag == 0);
  {

#ifndef WIN32
    mallopt(M_DEBUG,0);   
#endif

    myalloc_init_flag = 1;
  }
    mem = malloc(size);

//printf("alloc = %p\n",mem);
    memcount(1);
    return(mem);
}

void myfree(char* mem)
{
//  printf("free = %p\n",mem);
  if(mem == NULL)return;
  free(mem);
  if(memcount(0) < 0)
  {
     printf(" memalloced negative \n");
     getchar();
  }
   memcount(-1);
  //mem= NULL;
  return;
}



void nrerror(char *error_text)
/* Numerical Recipes standard error handler */
{
printf("enter ERerror\n");
  fprintf(stderr,"Numerical Recipes run-time error...\n");
  fprintf(stderr,"%s\n",error_text);
  fprintf(stderr,"...now returning(longjmp) to system...\n");
  //exit(1);
  longjmp(env,1);
}

void clear_vector(PHOTO_FLOAT *v,int nl,int nh)
{
  int i;
  for(i=1;i<=nh;i++)v[i]=0.0;
  return;
}


PHOTO_FLOAT *vector(int nl,int nh)
/* allocates a PHOTO_FLOAT vector with range[nl..nh} */

{
  PHOTO_FLOAT *v;

  v=(PHOTO_FLOAT *)myalloc((unsigned) (nh-nl+1)*sizeof(PHOTO_FLOAT));
  if(v==NULL)nrerror("allocation failur in vector()");
  clear_vector(v-nl,nl,nh);
  n_vect++;
  return(v-nl);
}


VECTOR print_vector(VECTOR v,int nl,int nh)
{
  int i;

  printf("\n\n%d,%d\n",nl,nh);
  for(i=nl;i<=nh;i++)printf("%d %e\n",i,v[i]);
  return(v);
}



MATRIX *print_matrix(MATRIX *m,int nrl,int nrh,int ncl,int nch)
{
  int i,j;

  printf("\n\n%d,%d,%d,%d\n",nrl,nrh,ncl,nch);
  for(i=nrl;i<=nrh;i++)
  {
    printf("row = %d \n",i);
    for(j=ncl;j<=nch;j++)printf(",%e",(*m)[i][j]);
    printf("\n");
  }
  return(m);
} 

void add_vector(PHOTO_FLOAT *to,int l,int h, PHOTO_FLOAT *from,int fl,int fh)
{
  int i,j;

  for(i=l,j=fl;i<=h;i++,j++)to[i] +=from[j];

  return;
}



void subtract_vector(PHOTO_FLOAT *to,int l,int h, PHOTO_FLOAT *from,int fl,int fh)
{
  int i,j;

  for(i=l,j=fl;i<=h;i++,j++)to[i] -=from[j];

  return;
}


void copy_vector(PHOTO_FLOAT *to,int l,int h, PHOTO_FLOAT *from,int fl,int fh)
{
  int i,j;

  for(i=l,j=fl;i<=h;i++,j++)to[i]=from[j];

  return;
}





PHOTO_FLOAT *clone_vector(PHOTO_FLOAT *v,int nl,int nh,int newl,int newh)
{
  PHOTO_FLOAT *nv;
  nv = vector(newl,newh);
  copy_vector(nv,newl,newh,v,nl,nh);
  return(nv);
}





PHOTO_FLOAT *subvector(PHOTO_FLOAT *v,int oldl,int oldh,int newl)
{
  PHOTO_FLOAT *m;
  m = &(v[oldl])-newl;
  return(m);
}

void free_vector(PHOTO_FLOAT* &v,int nl,int nh)
/* frees a vector allocated by vector() */
{
  if(v == NULL)return;
  char *temp = (char *) (v+nl);
  myfree( temp);
  n_vect--;
  return;
}


void clear_matrix(MATRIX *m,int nrl,int nrh,int ncl,int nch)
{
  int i,j;

  for(i=nrl;i<=nrh;i++)
  {
    for(j=ncl;j<=nch;j++)
    {
      (*m)[i][j] = 0.0;
    }
  }
  return;
} 


PHOTO_FLOAT *cross_vector(PHOTO_FLOAT *a,PHOTO_FLOAT *b)
{
  PHOTO_FLOAT *c=vector(1,3);

  c[1] = a[2]*b[3] - a[3]*b[2];
  c[2] = a[3]*b[1] - a[1]*b[3];
  c[3] = a[1]*b[2] - a[2]*b[1];

  return(c);
}

PHOTO_FLOAT dot_vector(int n,PHOTO_FLOAT *a,PHOTO_FLOAT *b)
{
  int i;
  PHOTO_FLOAT d=0.0;

  for(i=1;i<=n;i++)
  {
     d += a[i]*b[i];
  }

  return(d);
}  


PHOTO_FLOAT vector_length(int n,PHOTO_FLOAT *a)
{
  PHOTO_FLOAT d = dot_vector(n,a,a);
  return(sqrt(d));
}

void multvector(int n,PHOTO_FLOAT *a, PHOTO_FLOAT p)
{
  int i;
  for(i=1;i<=n;i++)
  {
    a[i] = a[i]*p;
  }
  return;
}



PHOTO_FLOAT *unit_vector(int n,PHOTO_FLOAT *a)
{
  PHOTO_FLOAT *u = clone_vector(a,1,n,1,n);
  multvector(n,u,1/(vector_length(n,a)));
  return(u);
}

 struct mat_hdr_st
{
  MATRIX mat;
  int rl;
  int rh;
  int cl;
  int ch;
  int maxr;
  int maxp;
//  struct mat_hdr_st *hdr;
  MATRIX real_mat;
};
 
 typedef struct mat_hdr_st mat_hdr_typ;

mat_hdr_typ *mathdr(MATRIX *m)
{
 // return( *((mat_hdr_typ **)(m-1)) );
 return( (mat_hdr_typ *)m );
}


int get_matrix_nr(MATRIX *m)
{

  mat_hdr_typ *hd = mathdr(m);

  return( (hd->rh)-(hd->rl)+1);
}


int get_matrix_rl(MATRIX *m)
{

  mat_hdr_typ *hd = mathdr(m);
  return(hd->rl);
}


int get_matrix_rh(MATRIX *m)
{

  mat_hdr_typ *hd = mathdr(m);
  return(hd->rh);
}

void free_matrix_shell(MATRIX* &m)
{
  if(m == NULL)return;
  mat_hdr_typ *hd = mathdr(m);
//printf("enter free_matrix_shell\n");

//printf("lr %d, rh %d, cl %d, ch %d \n",hd->rl,hd->rh,hd->cl,hd->ch);
  if(hd->real_mat != NULL)
  {
//    printf("freeing matrix\n");
     myfree( (char *)(hd->real_mat));
  }
//  printf("freeing hd\n");
  myfree( (char *)hd);
  m = NULL;

  return;
}





MATRIX *matrix_shell(int nrl,int nrh,int ncl, int nch)

/* Allocates a PHOTO_FLOAT matrix with range [nrl..nrh] [ncl..nch] */

{
  int i;
  mat_hdr_typ *mh;
 //MATRIX *m;

  /* allocate pointers to rows */

  mh = (mat_hdr_typ*)myalloc( (unsigned)sizeof( mat_hdr_typ ) );
 
  if(mh==NULL)nrerror("allocate failure 1 in matrix_shell()");
  //mh->hdr = mh;

  mh->mat = NULL;
  mh->real_mat = NULL;

  mh->real_mat = (MATRIX  )myalloc( (unsigned)((nrh-nrl+1)*sizeof(PHOTO_FLOAT *)));
  if(mh->real_mat == NULL) 
  {
    MATRIX *temp = &(mh->real_mat);
    free_matrix_shell( temp );
    mh->real_mat = NULL;
    nrerror("allocatefailure 2 in matrix_shell()");
     return(NULL);
  }

  mh->mat = mh->real_mat - nrl;
  

  mh->rl = nrl; mh->rh = nrh; mh->cl = ncl; mh->ch = nch;

  mh->maxp = nrh-nrl+1;
  mh->maxr = mh->maxp;

  for(i=nrl;i<=nrh;i++)(mh->mat)[i]=NULL;

  return(&mh->mat);
}



MATRIX *matrix(int nrl,int nrh,int ncl,int nch )
{
   int i;

   MATRIX *mp = matrix_shell( nrl, nrh,ncl,nch);
   MATRIX m = *mp;
   mat_hdr_typ *hdr = ((mat_hdr_typ *)(mp));
  /* allocate rows and set pointers to them */
  for(i=nrl;i<=nrh;i++)
  {
    m[i] = (PHOTO_FLOAT *)myalloc((unsigned) (nch-ncl+1)*sizeof(PHOTO_FLOAT));
    if(m[i] == NULL)nrerror("allocate failure 2 in matrix()");
    m[i] -= ncl;
    //(hdr->maxr)++;
  }
  clear_matrix(mp,nrl,nrh,ncl,nch);
  n_mat++;
  return(mp);
}



int add_matrix_rows( MATRIX *mp, int nr)
{
   int temp,i;
   mat_hdr_typ *mh = ((mat_hdr_typ *)(mp));

   int nnr = nr - (mh->maxp - mh->maxr); // number of rows needed

   if(nnr > 0 )
   {
     MATRIX  real_om = mh->real_mat;  //old matrix
     MATRIX  om = mh->mat;

     if(nnr < 100)nnr = 100;
     mh->real_mat = (MATRIX  )myalloc( (unsigned)((nnr+mh->maxp)*sizeof(PHOTO_FLOAT *)));
     mh->mat = mh->real_mat - mh->rl;
    
     for(i = mh->rl; i<= mh->rh; i++)
     {
       (mh->mat)[i] = om[i];
     }

     // free old matrix;

     myfree((char *)(real_om));

     for(i=mh->rh+1;i<=mh->rh+nnr;i++)mh->mat[i] = NULL;
     mh->maxp += nnr;
   }

   for(i=mh->rh+1;i<=mh->rh+nr;i++)
   {
     (mh->mat)[i] = vector(mh->cl,mh->ch);
   }
   
   temp =mh->rh+1;
   mh->maxr += nr;
   mh->rh   += nr;

   return(temp);
} 




int add_matrix_shell_rows( MATRIX *mp, int nr)
{
   int temp,i;
   mat_hdr_typ *mh = ((mat_hdr_typ *)(mp));

   int nnr = nr - (mh->maxp - mh->maxr); // number of rows needed

   if(nnr > 0 )
   {
     MATRIX  om = mh->mat;  //old matrix
     MATRIX  real_om = mh->real_mat;

     if(nnr < 100)nnr = 100;
     mh->real_mat = (MATRIX  )myalloc( (unsigned)((nnr+mh->maxp)*sizeof(PHOTO_FLOAT *)));
     mh->mat = mh->real_mat - mh->rl;
     
     for(i = mh->rl; i<= mh->rh; i++)
     {
       (mh->mat)[i] = om[i];
     }

     // free old matrix;
     //for(i=mh->rl;i<=mh->rh;i++)myfree( (char *) ( (om)[i]+mh->cl ) );
     myfree((char *)real_om);

     for(i=mh->rh+1;i<=mh->rh+nnr;i++)mh->mat[i] = NULL;
     mh->maxp += nnr;
   }

   for(i=mh->rh+1;i<=mh->rh+nr;i++)
   {
     (mh->mat)[i] =NULL;
   }
   
   temp =mh-> rh+1;
   mh->maxr += nr;
   mh->rh   += nr;

   return(temp);
} 




void copy_matrix(MATRIX *to,int rl1,int rh1,int cl1,int ch1,
                 MATRIX *fr,int rl2,int rh2,int cl2,int ch2)
{
  int i,j;

  for(i=rl1,j=rl2;i<=rh1;i++,j++)
  {
    if((*fr)[j] == NULL)
    {
      if( (*to)[i] != NULL)free_vector( (*to)[i],cl2,ch2);
      (*to)[i] = NULL;
    }
    else
    {
     copy_vector((*to)[i],cl1,ch1,(*fr)[j],cl2,ch2);
    }
  }
  return;
}



MATRIX *clone_matrix(MATRIX *m,int rl,int rh,int cl,int ch,
                             int nrl,int nrh,int ncl,int nch)
{

  MATRIX *a;

  a=matrix(nrl,nrh,ncl,nch);
  copy_matrix(a,nrl,nrh,ncl,nch,m,rl,rh,cl,ch);

  return(a);
} 




void free_matrix(MATRIX* &m,int nrl,int nrh,int ncl,int nch)
/* frees a row allocated with matrix() */
{
  int i;
 
  if(m == NULL)return;
//printf("enter free_matrix\n");
  mat_hdr_typ *hd = mathdr(m);

  for(i=hd->rl;i<=hd->rh;i++)
  {
    if( (*m)[i] != NULL)myfree( (char *) ( (*m)[i]+ncl ) );
  }
  free_matrix_shell(m);
  m= NULL;
  n_mat--;
  return;
}


MATRIX *submatrix(MATRIX *a,int oldrl,int oldrh,int oldcl,int oldch,
                            int newrl,int newcl)
/* 
  Returns a submatrix with range [newrl..newrl+(oldrh-oldrl)],
  [newcl..newcl+(oldch-oldcl)]pointing to the existing matrix range
  a[oldrl...oldrh][oldcl..oldch].
*/

{
  MATRIX *m;
  int   i,j;
  

  /* allocate pointers to rows */

   m = matrix_shell(newrl,newrl + (oldrh-oldrl),newcl,newcl+oldch-oldcl );
  /* set pointers to rows */

  for(i=oldrl,j=newrl;i<=oldrh;i++,j++)
  { 
    (*m)[j] = (*a)[i]+oldcl-newcl;
  }
  
  return(m);
}


void free_submatrix(MATRIX* &m,int nrl,int nrh,int ncl,int nch)

/* frees a submatrix allocated by submatrix() */


{
  if(m==NULL)return;
//printf("enter free_submatrix\n");
  free_matrix_shell(m);
  m=NULL;
  return;
}



void normalize_matrix(PHOTO_FLOAT *ax,PHOTO_FLOAT *ay,PHOTO_FLOAT *az)
{
  PHOTO_FLOAT *ux,*uy,*uz;   

  PHOTO_FLOAT a = vector_length(3,az);
/* printf("length = %e,%e\n",a,b);*/


  ux = unit_vector(3,ax);
  uy = unit_vector(3,ay);
  uz = cross_vector(ux,uy);

 /* multvector(3,uz,a);*/

  copy_vector(az,1,3,uz,1,3);
  copy_vector(ax,1,3,ux,1,3);
  copy_vector(ay,1,3,uy,1,3);


  free_vector(ux,1,3);
  free_vector(uy,1,3);
  free_vector(uz,1,3);

  return;
}


MATRIX *transpose(MATRIX *m,int nrl,int nrh,int ncl,int nch)
{
  MATRIX *t;
  int i,j;

  t=matrix(ncl,nch,nrl,nrh);

  for(i=nrl;i<=nrh;i++)
  {
    for(j=ncl;j<=nch;j++)
    {
      ( (*t)[j])[i] = ( (*m)[i])[j];
    }
  }
  return(t);
}


MATRIX *matrix_mult( MATRIX *m1,int nrl1,int nrh1,int ncl1,int nch1,
                     MATRIX *m2,int nrl2,int nrh2,int ncl2,int nch2,
                     int nrl,int nrh,int ncl,int nch)
{
   int i,j,k;

   MATRIX *m = NULL;
   m = matrix(nrl1,nrh1,ncl2,nch2);
   if(m==NULL)
   {
     nrerror("allocation failure matrix_mult()");
     return(NULL);
   }
   for(i=nrl1;i<=nrh1;i++)
   {
    for(j=ncl2;j<=nch2;j++)
    {


      (*m)[i][j] = 0.0;
      for(k=ncl1;k<=nch1;k++)(*m)[i][j] += (*m1)[i][k]*(*m2)[k][j];

    }
  }
  return(m);
}      

PHOTO_FLOAT dot(PHOTO_FLOAT *v1,int l1,int h1, PHOTO_FLOAT *v2,int l2,int h2)
{
  int i,j;
  PHOTO_FLOAT d=0.0;
 
  for(i=l1,j=l2;i<=h1;i++,j++)
  {
    d += v1[i]*v2[j];
  }
  return(d);
}

PHOTO_FLOAT *matrix_vector(MATRIX *m,int rl,int rh,int cl,int ch,
                     PHOTO_FLOAT *v,int l,int h)
{
  PHOTO_FLOAT *r;
  int i,j;
  r = vector(l,h);
  for(i=l,j=rl;j<=rh;i++,j++)r[i]=dot( (*m)[j],cl,ch,v,l,h);
  return(r);
}



void ludcmp(PHOTO_FLOAT **a,int n,int *indx,PHOTO_FLOAT *d)
{
/*
Given an N X N matrix a[1...n][1...n], this routine replaces it by the LU
decomposition of a rowwise permutation of itself. a and n are input, a is 
output, arranged as in equation (2.3.14) above; index[i...n] is an output
vector which records the row permutation effected by the partial pivoting;
d is output as +/-1depending on whether the number of row interchanges
was even or odd respectively. This routine is used in combination with
lubkab() to solves linear equations or invert a matrix.
*/


  int     i,imax,j,k;
  PHOTO_FLOAT   big,dum,sum,temp;
  PHOTO_FLOAT   *vv;

  /* vv stores the implicit scaling of each row */
  vv=vector(1,n);
  *d=1.0;
/*
printf("this is A\n");
print_matrix(a,1,n,1,n);
*/
  /* get scaling for each row; */
  for(i=1;i<=n;i++)
  {
    big=0.0;
    /* get largest absolute value for this row */
    for(j=1;j<=n;j++)
    {
      temp = fabs(a[i][j]);
      if( temp > big) big=temp;
/* printf("test %e,%E,%f\n",a[i][j],temp,big); */
    }
    if(big == 0.0) nrerror("Singular matrix in routine LUDCMP");
   /* save the row scaling;*/
/* printf("big = %e\n",big);*/
    vv[i] = 1.0/big;
  }


  /* This is the loop over columns of Crout's method */
  for(j=1;j<= n;j++)
  {
    for(i=1;i<j;i++)
    {
      sum=a[i][j];
      for(k=1;k<i;k++)sum -= a[i][k]*a[k][j];
      a[i][j] = sum;
    }
    big=0.0; /* initialize search for largest pivot element */

    for(i=j;i<=n;i++)
    {
      sum=a[i][j];
      for(k=1;k<j;k++)
        sum -= a[i][k]*a[k][j];
      a[i][j]=sum;
      if( (dum=vv[i]*fabs(sum)) >= big) /* is the figure of merit for the */
      {                                /* pivot the best so far          */
         big = dum;
         imax = i;
      }
    }  
    if(j != imax)   /* do we need to interchange rows? */
    {
      for(k=1;k<=n;k++)
      {
        dum=a[imax][k];
        a[imax][k] = a[j][k];
        a[j][k] = dum;
      }
      *d = -(*d);            /* ...and change parity of d */
      vv[imax]=vv[j];       /* also interchange the scale factor */

    }

    indx[j] = imax;
    if (a[j][j] == 0.0)
    {
printf("********* used TINY **********\n");
      a[j][j] = TINY;
    }
    /*
       If the pivot element is zero the matrix is singular (at least to the
       precision of the algorithm ). For some applications on singular
       matrices, it is desirable to substitute TINY for zero. 
    */
 
    if(j != n)   /* now finally divid by the pivot element */
    {
      dum= 1.0/(a[j][j]);
      for(i=j+1;i<=n;i++) a[i][j] *= dum;
    }
  
  }  /* go back for the next column in the reduction */

  free_vector(vv,1,n);
/*
printf("exit first step\n");
for(i=0;i<n;i++)print_vector(a[indx[i]],1,n);
*/
  return;
}  
   





void lubksb(PHOTO_FLOAT **a,int n,int *indx, PHOTO_FLOAT *b)
/*
  Solves the set of n linear equations [A]*[X] = [B]. Here a[1..n][1..n]
  is input not as the matrix [A] but rather as its LU decomposition,
  determined by the routine ludcmp(). indx[1..n] is input as the permutation
  vector returned by ludcmp(). b[1..n] is input as the right-hand side
  vector [B],and returns with the solution vector [X]. a,n,and indx are 
  not modified by this routine and can be left in place for successive calls
  with different right-hand sides b[1..n]. This routine takes into account
  the possibility that b[1..n] will begin with many zero elements, so it is
  efficient for use in matrix inversion.
*/

{
  int     i,ii=0,ip,j;
  PHOTO_FLOAT   sum;

  /*
     When ii is set to a positive value, it will become the index of the
     first nonvanishing element of b. We now do the forward substitution
     equation 2.3.6. The only new wrinkle is to unscramble the permutation as
     we go.
  */
/*
printf( "left side = \n");
print_vector(b,1,n);
*/
  for(i=1;i<=n;i++)
  {
    ip = indx[i];
    sum = b[ip];
    b[ip] = b[i];
    if(ii)
    {
      for(j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
    }
    else
    {
      if(sum)ii=i;
    }
    b[i] = sum;
  }

  /*
    Now we do the back substitution, equation 2.3.7
  */

  for(i=n;i>=1;i--)
  {
    sum=b[i];
    for(j=i+1;j<=n;j++) sum -= (a[i][j])*b[j];   
    b[i]=sum/a[i][i];
  }

  return;
}


void mprove(PHOTO_FLOAT **a,PHOTO_FLOAT **alud,int n,int *indx,PHOTO_FLOAT *b,PHOTO_FLOAT *x)
/*
  Improves a solution vector x[1..n] of the linear set of equations
  [A]*[X]=[B]. The matrix a[1..n][1..n] and the vectors b[1..n] and x
  are input ,as is the dimension n. Also input is alud[1..n][1..n], the
  LU decomposition of a as returned by ludcmp(), and the vector indx[1..n]
  also returned by that routine. On output, only x is modified, to an
  improved set of values.
*/ 

{
  int j,i;
  double sdp;
  PHOTO_FLOAT *r;

  r=vector(1,n);

  /* calculate the right hand side,accumulating the residual */ 
  for(i=1;i<=n;i++)
  {
    sdp = -b[i];
    for(j=1;j<=n;j++)sdp +=a[i][j]*x[j];
    r[i]=sdp;
  }

  lubksb(alud,n,indx,r);
  /* solve for an error term and subtract it from the old solution */
  for(i=1;i<=n;i++)x[i] -=r[i];

  free_vector(r,1,n);

  return;
}


void pivit(short dim, double **A)
{
  short i,j,k,m,n;

  for(i=0; i < dim; i++)
  {


/*
printf("\n");
 for(n = 0;n<dim;n++)
 {
   for(m = 0; m < dim; m++)
   {
     printf(" %f  ", A[n][m]);
   }
   printf("\n");
 }
*/

     for(j=0;j<dim;j++)
     {
       if(j != i) A[i][j] = -A[i][j]/A[i][i];
     }
     (A[i])[i] = 1.0/(A[i])[i];

     for(j=0;j<dim;j++)
     {
       if(j != i)
       {
         for(k=0;k<dim;k++)
         {
           if(k != i) A[j][k] += A[j][i]*A[i][k];
         }
         A[j][i] = A[j][i]*A[i][i];
       }
     }
   }
  return;
} 



PHOTO_FLOAT *solve(MATRIX *a,PHOTO_FLOAT *L, int n)
{
    int i;
    PHOTO_FLOAT d;
    int *indx;
    PHOTO_FLOAT *x;
    MATRIX *m;

    indx= ((int *)myalloc(n*sizeof(int)) );
    indx--;
    x = clone_vector(L,1,n+1,1,n+1);
    m = clone_matrix(a,1,n,1,n,1,n,1,n);

    ludcmp(*m,n,indx,&d);
    lubksb(*m,n,indx,x);
/******
  mprove(a,m,n,indx,L,x);
******/
/********************
  for(i=n;i>0;i--)
  {
    if(i != indx[i])
    {
      PHOTO_FLOAT temp = x[i];
      x[i]=x[indx[i]];
      x[indx[i]]=temp;
    }
  }
*********************/

//    myfree((char *)indx+1);
    indx++;
    myfree( (char *)(indx) );

    free_matrix(m,1,n,1,n);
    return(x);
}

/*
typedef PHOTO_FLOAT PNT_A[2];
typedef PHOTO_FLOAT PNT3_A[3];
typedef PNT_A *PNT;
typedef PNT3_A *PNT3;
*/



     
 



PHOTO_FLOAT *lsq(MATRIX *A,PHOTO_FLOAT*L,int nu,int ne)
{
    int i;
    MATRIX *AT,*ATA;
    PHOTO_FLOAT *ATL,*R;

//    fprintf( stderr, "lsq: matrix A\n" );
//    print_matrix(A,1,ne,1,nu);

//    fprintf( stderr, "lsq: vector L\n" );
//    print_vector(L,1,ne); 

    AT = transpose(A,1,ne,1,nu);
    ATA = matrix_mult(AT,1,nu,1,ne,A,1,ne,1,nu,1,nu,1,nu);
    ATL = matrix_vector(AT,1,nu,1,ne,L,1,ne);
    R = solve(ATA,ATL,nu);

/* check */
/*************************** 
    if(ATA != NULL)free_matrix(ATA,1,nu,1,ne);
    ATA = matrix_mult(AT,1,nu,1,ne,A,1,ne,1,nu,1,nu,1,nu);

    CATL = matrix_vector(ATA,1,nu,1,nu,R,1,nu);


printf("DIFFERENCE\n");
for(i=1;i<=nu;i++)printf("%e,%e,%e\n",CATL[i],ATL[i],(CATL[i]-ATL[i]) ); 
****************************/
    
    if(AT != NULL)free_matrix(AT,1,nu,1,ne);
    if(ATA != NULL)free_matrix(ATA,1,nu,1,nu);
    if(ATL != NULL)free_vector(ATL,1,ne);

    return(R);
}






void svdcmp(PNTLST aa, int nr, int nc, VECTOR w, PNTLST vv)
{

/**
   Given a matrix a[1..nr] [1..mc], this routine cpmputes its singilar value
   decomposition, [A] = [U][W][V]t. The matrix U replaces a on output. The
   diagonal matrix of singular values W is output as a vector w[1..nc]. The
   matrix [V] (not the tranpose [V]t ) is out put as v[1..nc][1..nc]. 
**/


  int flag,i,its,j,jj,k,l,nm;
  PHOTO_FLOAT anorm,c,f,g,h,s,scale,x,y,z;
  PHOTO_FLOAT temp,temp1;
//printf("enter svdcmp\n");
//  print_matrix(aa,1,nr,1,nc);

  MATRIX a = *aa;
  MATRIX v =*vv;
  VECTOR rv1 = vector(1,nc);

  g = scale = anorm = 0.0;


  // Householder reduction to bidiagonal form

  for(i=1;i<=nc;i++)
  {
    l=i+1;
    rv1[i]=scale*g;
    g = s = scale = 0.0;

    if(i<=nr)
    {
      for(k=i;k<=nr;k++) 
      {
        //temp = a[k][i];
        //if(temp<0.0)temp=-temp;
        //scale += temp;
        scale += fabs(a[k][i]);
      }
      if(scale)
      {
        for(k=i;k<=nr;k++)
        {
          a[k][i] /= scale;
          s += a[k][i]*a[k][i];
        }
        f = a[i][i];

        g = -SIGN(sqrt(s),f);
        // g = sqrt(s);
        // if(f>0.0)g=-g;


        h = f*g-s;
        a[i][i] = f-g;

        for(j=l;j<=nc;j++)
        {
          for(s=0.0,k=i;k<=nr;k++) s += a[k][i]*a[k][j];
          f=s/h;
          for(k=i;k<=nr;k++) a[k][j] += f*a[k][i];
        }
        
        for(k=i;k<=nr;k++) a[k][i] *= scale;
      }

    }

    w[i] = scale*g;
    g = s = scale = 0.0;


    if(i<=nr && i != nc)
    {
      for(k=l;k<=nc;k++) 
      {
        // printf("scale temp = %e i = %d  k = %d\n",a[i][k],i,k);
       // temp = a[i][k];
       // if(temp<0.0)temp=-temp;
       // scale += temp;
        scale += fabs(a[i][k]);
      }
      if(scale)
      {
        
        for(k=l;k<=nc;k++)
        {
          a[i][k] /= scale;
          s += a[i][k]*a[i][k];
        }

        f = a[i][l];
        g = -SIGN(sqrt(s),f);
        // g = sqrt(s);
        // if(f>0.0)g=-g;



        h = f*g-s;
        a[i][l] = f-g;
        for(k=l;k<=nc;k++)rv1[k] = a[i][k]/h;

        for(j=l;j<=nr;j++)
        {
          for(s =0.0, k=l;k<=nc;k++) s += a[j][k]*a[i][k];
          for(k=l;k<=nc;k++) a[j][k] += s*rv1[k];
        }
        for(k=l;k<=nc;k++) a[i][k] *= scale;
      }
    
    }
    anorm =FMAX(anorm,(fabs(w[i])+fabs(rv1[i])));
    // temp = w[i];
    // if(temp<0.0)temp=-temp;
    // temp1 = rv1[i];
    // if(temp1<0.0)temp1=-temp1;
   //  temp = temp + temp1;
    // if(temp>anorm)anorm = temp;
//  printf("anorm = %e w[i] = %e, rv1[i]= %e  i=%d\n",anorm,w[i],rv1[i],i);
  }

  

  // Accumulation of right-hand transformations


  for(i=nc;i>=1;i--)
  {
    if(i<nc)
    {
      if(g)
      {
        // double division to avoid possible underflow
        for(j=l;j<=nc;j++)
                  v[j][i]=( a[i][j]/a[i][l])/g;
        for(j=l;j<=nc;j++)
        {
          for(s=0.0,k=l;k<=nc;k++) s += a[i][k]*v[k][j];
          for(k=l;k<=nc;k++) v[k][j] += s*v[k][i];
        }
      }

      for(j=l;j<=nc;j++) v[i][j] = v[j][i] = 0.0;

    }

    v[i][i] = 1.0;
    g = rv1[i];

    l=i;

  }

  // Accumulation of left-hand transform

  for(i=IMIN(nr,nc);i>=1;i--)
  {
    l=i+1;
    g=w[i];
    for(j=l;j<=nc;j++) a[i][j] = 0.0;

    if(g)
    {
      g = 1.0/g;
      for(j=l;j<=nc;j++)
      {
        for(s =0.0,k=l;k<=nr;k++) s += a[k][i]*a[k][j];
        f=(s/a[i][i])*g;
        for(k=i;k<=nr;k++) a[k][j] += f*a[k][i];
      }

      for(j=i;j<=nr;j++) a[j][i] *= g;
    }
    else
    {
      for(j=i;j<=nr;j++) a[j][i] = 0.0;
    }
    (a[i][i]) += 1.0;
  }


  // Diagonalization of the bidiagonal form: Loop over singular values
  //    and over allowed iterations


  for(k=nc;k>=1;k--)
  {
    for(its=1;its<=30;its++)
    {
//printf(" k= %d, its = %d\n",k,its);
      flag = 1;
      for(l=k;l>=1;l--)     //test for splitting
      {
        nm=l-1;             // note that rv1[1] is always zero


//printf(" 1 anorm,xx %e,  %e,  %e\n",anorm,rv1[l],(PHOTO_FLOAT)(fabs(rv1[l])+anorm));
        if( (PHOTO_FLOAT)(fabs(rv1[l])+anorm) == anorm)
     
        //if(rv1[l] == 0.0)
        {
          flag = 0;
          break;
        }

// printf(" 2 anorm,xx %e,  %e,  %e\n",anorm,w[nm],(PHOTO_FLOAT)(fabs(w[nm])+anorm));
        if( (PHOTO_FLOAT)(fabs(w[nm])+anorm) == anorm)break;
        //if(w[nm] == 0.0)break;
      }
      if(flag)
      {
        c=0.0;
        s=1.0;
        for(i=l;i<=k;i++)
        {
          f=s*rv1[i];
          rv1[i]=c*rv1[i];
// printf(" 3 anorm,xx %e,  %e,  %e\n",anorm,f,(PHOTO_FLOAT)(fabs(f)+anorm));
          if( (PHOTO_FLOAT)(fabs(f)+anorm)==anorm)break;
          //if(f == 0.0)break;
          g=w[i];
          h=pythag(f,g);
          //h=sqrt(f*f+g*g);
          w[i]=h;
          h=1.0/h;
          c=g*h;
          s = -f*h;
          for(j=1;j<=nr;j++)
          {
            y=a[j][nm];
            z=a[j][i];
            a[j][nm] = y*c+z*s;
            a[j][i] =  z*c - y*s;
          }
        }
      }

      z=w[k];
      if(l==k)      //convergence
      {
        if(z<0.0)   //singular value is made nonnegative
        {
          w[k] = -z;
          for(j=1;j<=nc;j++) v[j][k] = -v[j][k];
        }
        break;
      }

      if(its == 30) nrerror("no convergence in svdcmp");
      x=w[l];
      nm=k-1;
      y=w[nm];
      g=rv1[nm];
      h=rv1[k];
      f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
      g=pythag(f,1.0);
      //g=sqrt(f*f+1.0);
      
      f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
      //temp = g;
      //if(temp<0.0)temp=-temp;
      //if(f<0.0)temp=-temp;
      //f=((x-z)*(x+z)+h*((y/(f+temp))-h))/x;

      c=s=1.0;      //next QR transform
      for(j=l;j<=nm;j++)
      {
        i=j+1;
        g=rv1[i];
        y=w[i];
        h=s*g;
        g=c*g;
        z=pythag(f,h);
        //z=sqrt(f*f+h*h);
        rv1[j]=z;
        c=f/z;
        s=h/z;
        f=x*c+g*s;
        g=g*c-x*s;
        h=y*s;
        y *= c;
        for(jj=1;jj<=nc;jj++)
        {
          x = v[jj][j];
          z = v[jj][i];
          v[jj][j] = x*c+z*s;
          v[jj][i] = z*c-x*s;
        }
        z =pythag(f,h);
        //z=sqrt(f*f+h*h);
        w[j] = z;          //Rotation can be arbitrary if z = 0;
        if(z)
        {  
          z=1.0/z;
          c=f*z;
          s=h*z;
        }
        f=c*g+s*y;
        x=c*y-s*g;

        for(jj=1;jj<=nr;jj++)
        {
          y=a[jj][j];
          z=a[jj][i];
          a[jj][j]=y*c+z*s;
          a[jj][i]=z*c-y*s;
        }
      }
      rv1[l]=0.0;
      rv1[k]=f;
      w[k]=x;
    }
  }
  free_vector(rv1,1,nc);

//  print_matrix(aa,1,nr,1,nc);
//  print_vector(w,1,nc);
//  print_matrix(vv,1,nc,1,nc);

//  PNTLST at = transpose(aa,1,nr,1,nc);
//  PNTLST ata = matrix_mult(at,1,nc,1,nr,aa,1,nr,1,nc,1,nc,1,nc);
//  printf("a transpose * a\n");
//  print_matrix(ata,1,nc,1,nc);

//  PNTLST vt = transpose(vv,1,nc,1,nc);
//  PNTLST vtv = matrix_mult(vt,1,nc,1,nc,vv,1,nc,1,nc,1,nc,1,nc);
//  printf("v transpose * v\n");
//  print_matrix(vtv,1,nc,1,nc);
  
  





//printf("exit svdcmp\n");
  return;
}
                   
 




void svbksb(PNTLST u, VECTOR w, PNTLST v, int m, int n, VECTOR b, VECTOR x)
{

  //  Solves A.X = B for a vector X, where A is specified by the arrays u[1..m][1..n],
  //  w[1..n], v[1..n][1..n] as returned by svdcmp. m and n are the dimensions of a,
  //  and will be equal for square matrices. b[1..m] is the input right-hand side.
  //  x[1..n] is the output solution vector. No input quantities are destroyed, so the
  //  routine may be called sequentially with different b's.

  
  int jj,j,i;
  PHOTO_FLOAT s;

  VECTOR tmp = vector(1,n);


  // Calculate UtB

  for(j=1;j<=n;j++)
  {
    s=0.0;
    if(w[j])
    {
      for(i=1;i<=m;i++) s += (*u)[i][j]*b[i];
      s /= w[j];

    }

    tmp[j] = s;

  }

  
  // Matrix multiply by V to get answer

  for(j=1;j<=n;j++)
  {
    s=0.0;

    for(jj=1;jj<=n;jj++) s += (*v)[j][jj]*tmp[jj];
    
     x[j] = s;

  }

  free_vector(tmp,1,n);


  return;

}    



void svdvar(PNTLST v, int ma, VECTOR w, PNTLST cvm)
{

  //   To evaluate the covariance matrix cvm[1..ma][1..ma] of the fit for ma parameters
  // obtained by svdfit, call this routine with matrices v[1..ma][1..ma],w[ma] as returned
  // from svdfit.

  int i,j,k;

  PHOTO_FLOAT sum;

  VECTOR wti = vector(1,ma);

  for(i=1;i<=ma;i++)
  {
    wti[i] = 0.0;
    if(w[i]) wti[i] = 1.0/(w[i]*w[i]);
  }

  for( i=1;i<=ma;i++)
  {
    for(j=1;j<=i;j++)
    {
      for(sum = 0.0,k=1;k<=ma;k++) sum += (*v)[i][k]*(*v)[i][k]*wti[k];
      (*cvm)[j][i]=(*cvm)[i][j]=sum;
    }
  }
  free_vector(wti,1,ma);

  return;
}









void svdfit(PNTLST A, VECTOR y, VECTOR sig, int ndata, VECTOR a, int ma,
            PNTLST u, PNTLST v, VECTOR w, PHOTO_FLOAT *chisq,
            void (*funcs)(PHOTO_FLOAT,PNTLST, int)  )

{
   // Given a set of data points x[1..ndata],y[1..ndata], with individual standard
   // deviations sig[1..ndata], use Chi Squared minimization to determine the coefficients
   // a[1,ma] of the fitting function for(i=1;i<=ma;i++)y+=a[i]*afunc[i](x). Here we solve
   // the fitting equations using singular value decomposition of the ndata by ma matrix.
   // Arrays u[1..ndata][1..ma],v[1..ma][1..ma], and w[1..ma] provide workspace on input;
   // on output they define the singular value decomposition and can be used to obtain
   // the covariance matrix. The function returns values for the ma fit parameters a and
   // Chi Squared, chisq. The user supplies a routine funcs(x,afunc,ma) that returns the 
   // ma basis functions evaluated x= x in the array afunc[1..ma].


  int j,i;

  PHOTO_FLOAT TOL = 1.0e-19;

  PHOTO_FLOAT wmax,tmp,thresh,sum;

  VECTOR b = vector(1,ndata);

  //VECTOR afunc = vector(1,ma);

  // Accumulate coefficients of the fitting matrix

  VECTOR x = vector(1,ndata);


  for(i=1;i<=ndata;i++)
  {
    //*funcs(x[i],afunc,ma);

    tmp = 1.0/sig[i];
//printf("sig = %e\n",sig[i]);
    for(j=1;j<=ma;j++) (*u)[i][j]=(*A)[i][j]*tmp;

    b[i] = y[i]*tmp;

  }



  // Singular value decomposition
  svdcmp(u,ndata,ma,w,v);
//printf("interal w");
//print_vector(w,1,ma);
  //Edit the singular values, given TOL

  wmax = 0.0;

  for(j=1;j<=ma;j++)
  {
    if(w[j] > wmax) wmax = w[j];
  }

  thresh = TOL*wmax;

  for(j=1;j<=ma;j++)
  {
//printf(" w[%d] = %e ***********************************\n",j,w[j]);
    if(w[j]< thresh) w[j] = 0.0;
  }


  svbksb(u,w,v,ndata,ma,b,a);

  
  // Evaluate Chi Squared

  *chisq = 0.0;

  for(i=1;i<=ndata;i++)
  {
    //(*funcs)(x[i],afunc,ma);

    for(sum=0.0,j=1;j<=ma;j++)sum += a[j]*(*A)[i][j];

    tmp=(y[i]-sum)/sig[i];
    (*chisq) += tmp*tmp;
  }

  //free_vector(afunc,1,ma);
  free_vector(b,1,ndata);

  return;

}




#define SWAP(a,b) { swap=(a);(a)=(b);(b)=swap;}

void covsrt( PNTLST covar, int ma, int ia[], int mfit)
/**
  Expand in storage the covariance matrix covar, so as to take into account
parameters that are being held fixed. (For the latter, return zero covariances.)
**/
{
  int i,j,k;

  PHOTO_FLOAT swap;

  for(i=mfit+1;i<=ma;i++)
  {
    for(j=1;j<=i;j++)
    {
      (*covar)[i][j]=(*covar)[j][i]=0.0;
    }
  }

  k=mfit;

  for(j=ma;j>=1;j--)
  {
    if(ia[j])
    {
      for(i=1;i<=ma;i++)SWAP( (*covar)[i][k], (*covar)[i][j])
      for(i=1;i<=ma;i++)SWAP( (*covar)[k][i], (*covar)[j][i])
      k--;
    }
  }

  return;
}


 





void mrqcof(VECTOR x, VECTOR y, VECTOR sig,int ndata, VECTOR a, int ia[],
            int ma, PNTLST alpha, VECTOR beta, PHOTO_FLOAT *chisq ,
            void (*funcs)(VECTOR, VECTOR, PHOTO_FLOAT *, VECTOR ,int))


/**

  Used by mrqmin to evaluate the linearized fitting matrix alpha and vector beta
as in (15.5.8), and calculate chi squared.

**/

{
  int i,j,k,l,m,mfit=0;   

  PHOTO_FLOAT wt,sig2i,dy;

  VECTOR ymod = vector(1,ndata);
  
  PNTLST dyda = matrix(1,ndata,1,ma);

  for(j=1;j<=ma;j++)
  {
    if( ia[j] ) mfit++;
  }

  for(j=1;j<=mfit;j++)   // initialize (symmetric) alpha, beta.
  {
    for(k=1;k<=j;k++) (*alpha)[j][k]=0.0;
    beta[j] = 0.0;
  }

  *chisq = 0.0;
  
  //(*funcs)(x[i],a,&ymod,dyda,ma);

  for(i=1;i<=ndata;i++)  // summation loop over all data
  {
     
    sig2i=1.0/(sig[i]*sig[i]);

    dy = y[i]-ymod[i];

    for(j=0,l=1;l<=ma;l++)
    {
      if( ia[l] )
      {
        wt=(*dyda)[i][l]*sig2i;
        for(j++,k=0,m=1;m<=l;m++)
        {
          if( ia[m] ) (*alpha)[j][++k] += wt*(*dyda)[i][m];
        }
        beta[j] += dy*wt;
      }
    }

    *chisq += dy*dy*sig2i;

  }

  for(j=2;j<=mfit;j++)
  {
    for(k=1;k<j;k++) (*alpha)[k][j] = (*alpha)[j][k];
  }

  //free_vector(dyda,1,ma);

  return;
}








void mrqmin(VECTOR x, VECTOR y, VECTOR sig, int ndata, VECTOR a, int ia[],
            int ma, PNTLST covar, PNTLST alpha, PHOTO_FLOAT *chisq,
            void (*funcs)(PHOTO_FLOAT, VECTOR,PHOTO_FLOAT *,VECTOR, int), PHOTO_FLOAT *alamda)

/**

    Levenberg-Marquardt method, attempting to reduce the value chi squared of a
fit between a set of data points x[1..ndata], y[1..ndata], with individual
standard deviations sig[1..ndata], and a nonlinear function dependent on ma
coefficients a[1..ma]. The input array ia[1..ma] indicates by nonzero entries those
components that should be held fixed at their input values. The function returns
current best-fit values for the parameters a[1..ma], chi squared,chisq,. The
arrays covar[1..ma][1..ma], alpha[1..ma][1..ma] are used as work space during 
most iterations. Supply a routine funcs(x,a,yfit,dyda,ma) that evaluates the
fitting function yfit and its derivates dyda[1..ma] with respect to the fitting 
parameters a at x. On the first call provide an initial guess for the parameters 
a, and set alamda<0 for initialization (which then sets alamda=0.001). if a step
succeeds chisq becomes smaller and alamda decreases by a factor of 10. if a step
fails alamda grows by a factor of 10. You must call this routine repeatedly
until convergence is achieved. Then, make one finalcall with alamda=0, so the
covar[1..ma][1..ma] returns the covariance matrix, and alpha the curvature matrix.
(parameters held fixed will return zero covariance).

**/


{
  int j,k,l,m;
  static int mfit;
  static PHOTO_FLOAT ochisq;
  static VECTOR atry;
  static VECTOR beta;
  static VECTOR da;
  static VECTOR oneda;


  if(*alamda < 0.0)
  {
    atry = vector(1,ma);   // initialize
    beta = vector(1,ma);
    da   = vector(1,ma);

    for(mfit = 0, j=1;j<=ma;j++)
    {
      if( ia[j] ) mfit++;
    }

    VECTOR oneda = vector(1,mfit);
    *alamda = 0.001;
    
    //mrqcof(x,y,sig,ndata,a,ia,ma,alpha,beta,chisq,funcs);
    ochisq=(*chisq);

    for(j=1;j<=ma;j++)
    {
      atry[j] = a[j];
    }

  }                       // end initialization



  for(j=0,l=1;l<=ma;l++)  // alter linearized fitting matrix by augmenting 
  {                       // diagonal elements;
    if(ia[l])
    {
      for(j++,k=0,m=1;m<=ma;m++)
      {
        if(ia[m])
        {
          k++;
          (*covar)[j][k]=(*alpha)[j][k];
        }
      }
      (*covar)[j][j]=(*alpha)[j][j] ;
      oneda[j]=beta[j];
    }
  }
    
  //gaussj(covar,mfit,oneda,1);
  //PHOTO_FLOAT *solve(MATRIX *a,PHOTO_FLOAT *L, int n)
  VECTOR R = solve(covar,oneda,mfit);
  
  for(j=1;j<=mfit;j++) da[j]=R[j];

  free_vector(R,1,mfit);

  if( *alamda == 0.0)   //once converged, evaluate covariance matrix
  {
    //covsrt(covar,ma,ia,mfit);
    free_vector(oneda,1,mfit);
    free_vector(da,1,ma);
    free_vector(beta,1,ma);
    free_vector(atry,1,ma);
    return;
  }


  for(j=0,l=1;l<=ma;l++)   //did the trial succeed?
  {
    if(ia[l])atry[l] = a[l] + da[++j];
  }

  //mrqcof(x,y,sig,ndata,atry,ia,ma,covar,da,chisq,funcs);

  if( *chisq < ochisq)   // Success, accept the new solution.
  {
    *alamda *= 0.1;

    ochisq = (*chisq);
    for(j=0,l=1;l<=ma;l++)
    {
      if(ia[l])
      {
        for(j++,k=0,m=1;m<=ma;m++)
        {
          if(ia[m])
          {
            k++;
            (*alpha)[j][k] = (*covar)[j][k];
          }
        }

        beta[j]=da[j];
        a[l]=atry[l];
      }
    }
  }
  else  // failure, increase alamda and return.
  {
    *alamda *= 10.0;
    *chisq = ochisq;
  }

}
      




void BBlsq(PNTLST ATAx, PNTLST P, PNTLST Q, PNTLST S,VECTOR AWT, VECTOR s, VECTOR RR,
     int nu,int ne,int nf,int np, VECTOR photosum, VECTOR pntsum, PNTLST *fp,int nff,int npf, PHOTO_FLOAT *chisq)
{

  PHOTO_FLOAT TOL = 1.0e-19;
  int rc=0;
  for(int ii = 1; ii<=nf; ii++)
  {
    // PNTLST sub = submatrix( P, rc,rc+11,1,12,1,1);


     MATRIX sub = (*P)+rc;


      choldcinv(&sub,12);
    // free_submatrix(sub,1,12,1,12);
     rc+=12;
  }


  int d = 12*nf;
  int e = nu-d;




printf("d = %d, e = %d\n",d,e);
printf("calculating RP\n");
 // PNTLST RP = matrix_mult(R,1,e,1,d, P,1,d,1,d, 1,e,1,d);


 // PNTLST PQ = matrix(1,d,1,e);

  PNTLST RP = matrix(1,e,1,d);
/***
  for( ii = 1; ii <= e; ii++)
  {
    for(int ll = 0; ll < 12*nf; ll+=12)
    {
      for(int jj =1; jj<=12; jj++)
      {
        //(*RP)[ii][ll+jj] = 0.0;
        PHOTO_FLOAT temp = 0.0;
        for(int kk =1+ll; kk<=12+ll; kk++)
        {
          //(*RP)[ii][ll+jj] += (*Q)[ll+kk][ii]*(*P)[ll+kk][jj];
          //temp += (*Q)[ll+kk][ii]*(*P)[ll+kk][jj];
         temp += (*Q)[kk][ii]*(*P)[kk][jj];
         // (*PQ)[ll+jj][ii] = (*RP)[ii][ll+jj];
        }
        (*RP)[ii][ll+jj]=temp;

        PHOTO_FLOAT *Sp = (*S)[ii];
        PHOTO_FLOAT *Qpp = (*Q)[ll+jj];
        for(int nn = ii;nn<= e;nn++) Sp[nn] -= temp*Qpp[nn]; 

      }
    }
  }
//print_matrix(RP,1,e,1,d);
***/


    int pi=0;
    int qi=0;

    for(int inf=1;inf<=nff;inf++) // look at each photo
    {
      if( photosum[inf] > 0)  // if this photo is usable
      {
        int pj=0;
        int qj=0;
        for(int inp=1;inp<=npf;inp++)  // look at each pnt in photo
        {
          if(pntsum[inp] > 0) // if pnt is usable
          {
            if( (*(fp[inf-1]))[inp-1] != NULL ) // if pnt defined for this photo
            {
              for(int i=1;i<=12;i++)
              {
                for(int j=1;j<=3;j++)
                {
                  PHOTO_FLOAT temp = 0.0;
                  for(int k=1;k<= 12;k++)temp += (*P)[pi+i][pj+k]*(*Q)[qi+k][qj+j];
                  //(*PQ)[pi+i][qj+j] = temp;
                  (*RP)[qj+j][pi+i] = temp;

                }
              }




              int qii=0;
              int qjj=0;

              for(int inf2=1;inf2<=nff;inf2++) // look at each photo
              {
                if( photosum[inf2] > 0)  // if this photo is usable
                {
                  if(pi == qii)
                  {
                   
                    //int pjj=0;
                    int qjj=0;
                    for(int inp2=1;inp2<=npf;inp2++)  // look at each pnt in photo
                    {
                      if(pntsum[inp2] > 0) // if pnt is usable
                      {
                        if( (*(fp[inf2-1]))[inp2-1] != NULL ) // if pnt defined for this photo
                        {

                          if(qjj >= qj)
                          {
                            for(int i=1;i<=3;i++)
                            {
                              for(int j=1;j<=3;j++)
                              {
                                 PHOTO_FLOAT temp = 0.0;
                                 for(int k=1;k<= 12;k++)temp += (*RP)[qj+i][pi+k]*(*Q)[qii+k][qjj+j];
                                  //(*PQ)[pi+i][qj+j] = temp;
                                  //(*RP)[qj+i][pjj+j] = temp;
                                  (*S)[qj+i][qjj+j] -= temp;
                              }
                            }
                          }
                        }
                        qjj+=3;  
                      }
                    }  
                  }
                  qii+=12;
                }
              }
            }
            qj += 3;
            //printf("qj= %d\n",qj);
          }
        }
        qi += 12;
        pi += 12;
        pj += 12;
        //printf("qi=%d***\n",qi);
      }
    }





printf("calculating PQ\n");
/**
  PNTLST PQ = matrix(1,d,1,e);
  for( ii = 0; ii < nf*12; ii+=12)
  {
    for(int ll = 1; ll <= 12; ll++)
    {
      for(int jj =1; jj<=e; jj++)
      {
        (*PQ)[ii+ll][jj] = 0.0;
        for(int kk =1; kk<=12; kk++) (*PQ)[ii+ll][jj] += (*P)[ii+ll][kk]*(*Q)[ii+kk][jj];
      }
    }
  }
//print_matrix(PQ,1,d,1,e);
**/
/**  
printf("calculating RPQ\n");
 // PNTLST RPQ = matrix_mult(RP,1,e,1,d, Q,1,d,1,e, 1,e,1,e);

 // PNTLST RPQ = matrix(1,e,1,e);
  for(ii=1;ii<= e;ii++)
  {
    PHOTO_FLOAT *RProw = (*RP)[ii];
    PHOTO_FLOAT *Srow = (*S)[ii];
    for(int jj=ii;jj<=e;jj++)
    {
      //(*RPQ)[ii][jj]=0.0;
      PHOTO_FLOAT temp = 0.0;
      for(int kk=1;kk<=d;kk++)
      {
         //(*RPQ)[ii][jj] += (*RP)[ii][kk]*(*Q)[kk][jj];
         //temp += (*RP)[ii][kk]*(*Q)[kk][jj];
         temp += RProw[kk]*(*Q)[kk][jj];
      }
       // (*RPQ)[ii][jj] = temp;
       //(*S)[ii][jj] = (*S)[ii][jj]-temp;
       Srow[jj] -= temp;
    }
  }
**/
  //for(ii = 1;ii<=e;ii++)for(int jj=1;jj<=e;jj++)(*S)[ii][jj] = (*S)[ii][jj]-(*RPQ)[ii][jj]; 




printf("calculating INVERSE\n");
  
  choldcinv(S,e);


  //S is now ([S]-[R][P]I[Q])I
printf("calculating SRP\n");
/***
  PNTLST SRP = matrix_mult(S,1,e,1,e, RP,1,e,1,d, 1,e,1,d);



  PNTLST R = matrix(1,e,1,d);
  for(ii=1;ii<=e;ii++)for(int jj=1;jj<=d;jj++) (*R)[ii][jj] = -(*SRP)[ii][jj];

  //R is now -([S]-[R][P]I[Q])I ([R][P]I)

  free_matrix(SRP,1,e,1,d);
***/

printf("calculating PQS\n");
 
  //PNTLST PQS = matrix_mult(PQ,1,d,1,e, S,1,e,1,e, 1,d,1,e);

 // PNTLST PQS = matrix(1,d,1,e);
/***
  for(ii=1;ii<= d;ii++)
  {
    PHOTO_FLOAT *Qp = (*Q)[ii];
    for(int jj=1;jj<=e;jj++)
    {
      //(*PQS)[ii][jj]=0.0;
      //(*Q)[ii][jj]=0.0;
      PHOTO_FLOAT temp=0.0;
      PHOTO_FLOAT *Sp = (*S)[jj];
      for(int kk=1;kk<=e;kk++)
      {
         //(*PQS)[ii][jj] += (*RP)[kk][ii]*(*S)[kk][jj];
         //(*Q)[ii][jj] -= (*RP)[kk][ii]*(*S)[kk][jj];
         temp -= (*RP)[kk][ii]*Sp[kk];
      }
      Qp[jj]=temp;
    }
  }
***/


clear_matrix(Q,1,d,1,e);
    int pqi=0;
    for( inf=1;inf<=nff;inf++) // look at each photo
    {
      if( photosum[inf] > 0)  // if this photo is usable
      {
        int pqj=0;
        for(int inp=1;inp<=npf;inp++)  // look at each pnt in photo
        {
          if(pntsum[inp] > 0) // if pnt is usable
          {
            if( (*(fp[inf-1]))[inp-1] != NULL ) // if pnt defined for this photo
            {
              for(int sj=0;sj<e;sj+=3)
              {
                for(int i=1;i<=12;i++)
                {
                  for(int j=1;j<=3;j++)
                  {
                    PHOTO_FLOAT temp = 0.0;
                    for(int k=1;k<= 3;k++)temp -= (*RP)[pqj+k][pqi+i]*(*S)[pqj+k][sj+j];
                    (*Q)[pqi+i][sj+j] += temp;
                  }
                }               
              }
            }
            pqj+=3;
          }
        }
        pqi+=12;
      }
    }

  //for(ii=1;ii<=d;ii++)for(int jj=1;jj<=e;jj++) (*Q)[ii][jj] = -(*PQS)[ii][jj];


  //Q is now -([P]I [Q]) ([S]-[R][P]I[Q])I







printf("clearing RR\n");
for(ii=1; ii<=e+d; ii++) RR[ii]=0.0;

printf("calculating PQRSP\n");

 // PNTLST PQSRP = matrix_mult(PQS,1,d,1,e, RP,1,e,1,d, 1,d,1,d);

 // PNTLST PQSRP = matrix(1,d,1,d);

/***
  for( ii=1;ii<=d;ii++)
  {
    //PHOTO_FLOAT *Pp = (*PQSRP)[ii];
    PHOTO_FLOAT *Qp = (*Q)[ii];
    //for(int jj=1;jj<=d;jj++)
    for(int jj=ii;jj<=d;jj++)
    {
      //(*PQSRP)[ii][jj]=0.0;
      PHOTO_FLOAT temp=0.0;
      for(int kk=1;kk<=e;kk++)
      {
        //(*PQSRP)[ii][jj] -= (*Q)[ii][kk]*(*RP)[kk][jj];
        temp -= Qp[kk]*(*RP)[kk][jj];
      }
      //Pp[jj]=temp;
      //(*PQSRP)[jj][ii]=temp;
      RR[ii] += temp*AWT[jj];
      if(ii != jj)RR[jj] += temp*AWT[ii];

    }
  }
****/


    //clear_matrix(Q,1,d,1,e);
    int fotoj=0;
    for( inf=1;inf<=nff;inf++) // look at each photo
    {
      if( photosum[inf] > 0)  // if this photo is usable
      {
        int pnti=0;
        for(int inp=1;inp<=npf;inp++)  // look at each pnt in photo
        {
          if(pntsum[inp] > 0) // if pnt is usable
          {
            if( (*(fp[inf-1]))[inp-1] != NULL ) // if pnt defined for this photo
            {
              for(int si=0;si<d;si+=12)
              {
                for(int i=1;i<=12;i++)
                {
                  for(int j=1;j<=12;j++)
                  {
                    PHOTO_FLOAT temp = 0.0;
                    for(int k=1;k<= 3;k++)temp -= (*Q)[si+i][pnti+k]*(*RP)[pnti+k][fotoj+j];
                    //(*Q)[pqi+i][sj+j] += temp;
                    RR[si+i]+=temp*AWT[fotoj+j];
                    
                  }
                }               
              }
            }
            pnti+=3;
          }
        }
        fotoj+=12;
      }
    }

        





//  for(ii = 1;ii<=d;ii++)for(int jj=1;jj<=d;jj++) (*P)[ii][jj] = (*P)[ii][jj] + (*PQSRP)[ii][jj];



//  VECTOR AWT = vector(1,nu);

//  for(ii=1;ii<=nu;ii++)for(int jj=1;jj<=ne;jj++) AWT[ii] += (*A)[jj][ii]*L[jj]*(1.0/(s[jj]*s[jj]));

//printf("from sub AWT");
//print_vector(AWT,1,nu);


printf("calculating RR\n");

  for(ii=1;ii<=d;ii++)
  {
      //RR[ii]=0.0;
      //for(int jj=1;jj<=d;jj++) RR[ii] += (*PQSRP)[ii][jj]*AWT[jj];

      for( int   jj=1;jj<=e;jj++) RR[ii] += (*Q)[ii][jj]*AWT[d+jj];
  }

  for(ii=0;ii<12*nf;ii+=12)
  {
    for(int jj=1;jj<=12;jj++)
    {
      for(int kk=1;kk<=12;kk++) RR[ii+jj] += (*P)[ii+jj][kk]*AWT[ii+kk];
    }
  }


  for(ii=1;ii<=e;ii++)
  {
    //RR[d+ii]=0.0;
    //for(int jj=1;jj<=d;jj++) RR[d+ii] += (*R )[ii][jj]*AWT[jj];
    for(int jj=1;jj<=d;jj++) RR[d+ii] += (*Q )[jj][ii]*AWT[jj];
    for(    jj=1;jj<=e;jj++) RR[d+ii] += (*S)[ii][jj]*AWT[d+jj];
  }

//  free_vector(AWT,1,nu);

  //free_matrix(PQS,1,d,1,e);
  //free_matrix(PQSRP,1,d,1,d);
  free_matrix(RP,1,e,1,d);
  //free_matrix(PQ,1,d,1,e);
  //free_matrix(RPQ,1,e,1,e);



return;
}



void choldc(PNTLST a, int n, VECTOR p)
{

/******
  Given a positive-definite synnetric matrix a[1..n][1..n], this routine constructs its Cholesky
  decomposition, A=L.Lt . On input, only the upper triangle of a need be given; it is not
  modified. The Cholesky factor L is returned in the lower triangle of a, except for its
  diagonal elements which are returned in p[1..n].
*******/

  int i,j,k;
  PHOTO_FLOAT sum=0.0;

  for(i=1;i<=n;i++)
  {
    for(j=i;j<=n;j++)
    {
      for(sum=(*a)[i][j],k=i-1;k>=1;k--) sum -= (*a)[i][k]*(*a)[j][k];
      if(i==j)
      {
        if(sum<=0.0)nrerror("choldc failed");  // a,with rounding errors, is not positive definite
        p[i] = sqrt(sum);
      }
      else
      {
        (*a)[j][i]=sum/p[i];
      }
    }
  }

  return;
}


void choldcinv(PNTLST mat, int size)
{

// MATRIX INVERSION USING CHOLESKY DECOMPOSITION

    // PNTLST org = clone_matrix(mat,1,size,1,size,1,size,1,size);
     VECTOR di = vector(1,size);

     choldc(mat, size, di);


//get (lower left) L inverse

     for(int mm = 1; mm <= size; mm++)
     {
       PHOTO_FLOAT coe = (*mat)[mm][mm] = 1.0/di[mm];
       for(int nn = 1; nn < mm; nn++)
       {
         PHOTO_FLOAT sum = 0.0;
         for(int k=nn;k<mm;k++) sum -= (*mat)[mm][k]*(*mat)[k][nn];
         (*mat)[mm][nn] = sum*coe;
       }
     }
//print_matrix(tmp,1,12,1,12);  


     for(mm=1;mm<=size;mm++)
     {
       for(int nn=mm; nn<=size; nn++)
       {
         PHOTO_FLOAT sum = 0.0;         
         for(int k=nn; k<= size; k++)
         {
             sum += (*mat)[k][mm]*(*mat)[k][nn];
         }
         (*mat)[mm][nn] = sum;
       }
     }
//print_matrix(tmp,1,12,1,12);

     for(mm=1;mm<=size;mm++)
     {
       for (int nn = mm; nn<= size; nn++) (*mat)[nn][mm]=(*mat)[mm][nn];
     }

//print_matrix(mat,1,size,1,size);

    // PNTLST res = matrix_mult(org,1,size,1,size,mat,1,size,1,size,1,size,1,size);
    // printf(" should be idenity ");
    // print_matrix(res,1,size,1,size);
     free_vector(di,1,size);

  return;
}


void pnt2film(PHOTO_FLOAT fl, VECTOR tm, VECTOR pnt, PHOTO_FLOAT* x, PHOTO_FLOAT* y)
{
  /*******

   Projects 3d point onto the film plane defined by the focal lentth [fl]
   and the world to camera tmatrix [tm]. 2d projected point returned via x,y.
*******/

  PHOTO_FLOAT px = tm[1]*pnt[1] + tm[2]*pnt[2] + tm[3]*pnt[3] + tm[10];
  PHOTO_FLOAT py = tm[4]*pnt[1] + tm[5]*pnt[2] + tm[6]*pnt[3] + tm[11];
  PHOTO_FLOAT pz = tm[7]*pnt[1] + tm[8]*pnt[2] + tm[9]*pnt[3] + tm[12];
  

  PHOTO_FLOAT a=fl/pz;
  *x = a*px;
  *y = a*py;

  return;
}


void pnt2film_error(PHOTO_FLOAT fl, VECTOR tm, VECTOR pnt,VECTOR pp, PHOTO_FLOAT* dx, PHOTO_FLOAT* dy)
{

  PHOTO_FLOAT px,py;

  pnt2film(fl,tm,pnt,&px,&py);

  PHOTO_FLOAT a= fl/pp[3];
 
  *dx = a*pp[1] - px;
  *dy = a*pp[2] - py;

  return;
}







      
       




