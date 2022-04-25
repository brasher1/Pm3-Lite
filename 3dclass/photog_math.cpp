
/*FF*************************************************************************
*                                                                           * 
*   Copyright (c) 1993 As-Built Data, Inc.  These coded instructions,	    *
*   statements, computer programs and source code contain unpublished	    *
*   trade secrets of As-Built Data, Inc. and are protected by Federal	    *
*   Copyright Law.  They may not be disclosed or copied, duplicated,	    *
*   distributed, demonstrated or licensed in any form, in whole or in	    *
*   part, without the prior written consent of As-Built Data, Inc.  Any	    *
*   permitted use shall be considered as a personal license to the	    *
*   authorized recipient only.  Any such permitted use is provided on a	    *
*   "as is" basis without any warranty of any kind, express or implied,	    *
*   and specifically without any warranty of merchantibility or fitness	    *
*   for a particular purpose.  In no event shall As-Built Data, Inc. be	    *
*   liable for any special, incidental, direct or indirect damages of	    *
*   any kind, regardless of legal theory or negligence of		    *
*   As-Built Data, Inc.							    *
*									    *
****************************************************************************/
/* pgmmath.c */

#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include <setjmp.h>
#include "mathsubs.h"
#include "output.h"

/**************************/
#if FALSE
#if !defined DB_ADDR
#define DB_ADDR long
#define DB_ADDR_XXXX 1
#endif
extern jmp_buf env;
extern int envi;

typedef struct BUNDLEDATA
{
    DB_ADDR dbaBundle;	    /* dba of the bundle for which the data applies */
    int nNumPhotos;	    /* number of photos in bundle */
    int nNumPoints;	    /* number of points in the bundle */
    PNTLST plTMatrix;	    /* array of transformation matrix vectors [nNumPhotos] */
    DB_ADDR *dbaTMatrix;    /* dba of each transformation matrix */
    PNTLST *plPixels;	    /* array of point lists containing the corrected film points
                                     [nNumPhotos][nNumPoints][3] */
    DB_ADDR *dbaPnts;	    /* array of arrays of the dba of the points record 
                               for each corrected film point */
    PNTLST plCalcPnts;	    /* point list for all calculated xyz's in the bundle [nNumPoints][3] */
    DB_ADDR *dbaCalcPnts;   /* dba of each calculated xyz in the master list */
    PNTLST plSurvPnts;	    /* point list for all surveyed xyz's in the master list [nNumPoints][3] */
} BundleData;
#if defined DB_ADDR_XXXX
#undef DB_ADDR_XXXX
#undef SB_ADDR
#endif



#endif
/**************************/



#include "photog_math.h"

//void bndl(BundleData *bd );
void bundle(BundleData *bd );


/**
void myallinfo()
{

  struct mallinfo  m = mallinfo();

  printf("   MALLINFO \n");
  printf(" ARENA    = %d\n",m.arena);    
  printf(" ORDBLKS  = %d\n",m.ordblks);    
  printf(" SMBLKS   = %d\n",m.smblks);    
  printf(" HBLKHD   = %d\n",m.hblkhd);    
  printf(" HBLKS    = %d\n",m.hblks);    
  printf(" USMBLKS  = %d\n",m.usmblks);    
  printf(" FSMBLKS  = %d\n",m.fsmblks);    
  printf(" UORDBLKS = %d\n",m.uordblks);    
  printf(" FORDBLKS = %d\n",m.fordblks);    
  printf(" KEEPCOST = %d\n",m.arena);    

  return;
}    
**/



void display_tm(VECTOR RR)
{
  int i,j,k;

  k = 1;

Output(0,"\n Tranform matrix\n");
  for(i=1;i<=4;i++)
  {
    for(j = 1;j<=3;j++)
    {
      if(RR[k] > 0.0)printf(" ");
      printf("   %e",RR[k++]);
    }
    printf("\n");
    if(i==3)printf("\n");
  }
  printf("\n");
/*
  FLOAT om =  fabsf(sqrt( RR[1]*RR[1] +  RR[2]*RR[2] +  RR[3]*RR[3] ) -1.0);
        om += fabsf(sqrt( RR[4]*RR[4] +  RR[5]*RR[5] +  RR[6]*RR[6] ) -1.0);
        om += fabsf(sqrt( RR[7]*RR[7] +  RR[8]*RR[8] +  RR[9]*RR[9] ) -1.0);

        om += fabsf(sqrt( RR[1]*RR[1] +  RR[4]*RR[4] +  RR[7]*RR[7] ) -1.0);
        om += fabsf(sqrt( RR[2]*RR[2] +  RR[5]*RR[5] +  RR[8]*RR[8] ) -1.0);
        om += fabsf(sqrt( RR[3]*RR[3] +  RR[6]*RR[6] +  RR[9]*RR[9] ) -1.0);

        om += fabsf(RR[1]*RR[4] +  RR[2]*RR[5] +  RR[3]*RR[6] );
        om += fabsf( RR[1]*RR[7] +  RR[2]*RR[8] +  RR[3]*RR[9] );
        om += fabsf( RR[4]*RR[7] +  RR[5]*RR[8] +  RR[6]*RR[9] );

        om += fabsf( RR[1]*RR[2] +  RR[4]*RR[5] +  RR[7]*RR[8] );
        om += fabsf( RR[1]*RR[3] +  RR[4]*RR[6] +  RR[7]*RR[9] );
        om += fabsf( RR[2]*RR[3] +  RR[5]*RR[6] +  RR[8]*RR[9] );
  
  om = om/12.0;

  printf("      orth measure -  %e     (0.0 nominal)\n\n",om);      
*/


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

//print_matrix(a,1,3,1,3);

  svdcmp(a,3,3,w,v);
/**
  printf("############################\n");
  printf(" results from svdcmp\n");
  print_vector(w,1,3);
  print_matrix(a,1,3,1,3);
  print_matrix(v,1,3,1,3);
**/

  Output(0,"\n Ortho measure   = %e,  %e,  %e  \n",w[1],w[2],w[3]);
  Output(0," nominal values  = %e,  %e,  %e  \n",1.0,1.0,1.0);

  free_vector(w,1,3);
  free_matrix(v,1,3,1,3);
  free_matrix(a,1,3,1,3);

  return;
}

/**
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
**/
VECTOR matpnt(VECTOR m, VECTOR p)
{
  VECTOR r = vector(1,3);

  r[1] = m[1]*p[1] + m[2]*p[2] + m[3]*p[3] + m[10];
  r[2] = m[4]*p[1] + m[5]*p[2] + m[6]*p[3] + m[11];
  r[3] = m[7]*p[1] + m[8]*p[2] + m[9]*p[3] + m[12];

  return(r);
}



VECTOR crosspnt( VECTOR p1, VECTOR p2)
{
  VECTOR p = vector(1,3);

  p[1] = p1[2]*p2[3] - p1[3]*p2[2];
  p[2] = p1[3]*p2[1] - p1[1]*p2[3];
  p[3] = p1[1]*p2[2] - p1[2]*p2[1];

  return(p);
}


PNT get3dpnt_e(int nf,PNTLST pnts, FLOAT **tmxs ,PNTLST error)
{

  //printf(" new get 3dpoint !!!!!!!!!!\n");
  int nu = 3; /* number of unknowns for least square calculations - x,y,z of point*/
  int ne = 3*nf; /* number of equations - three per input point */

  FLOAT chisq = 0.0;
  FLOAT fl = -65.124;
//  FLOAT fl = -65.4676;
  FLOAT lamda = 1.0;

  PNTLST A = matrix(1,ne,1,nu);
  VECTOR L = vector(1,ne);

  VECTOR R=vector(1,nu);
  VECTOR w=vector(1,nu);

  PNTLST u=matrix(1,ne,1,nu);
  PNTLST v=matrix(1,nu,1,nu);

  VECTOR sig=vector(1,ne);

     // int ndata = 2*nf;
    //  int ma = 3;
   
  int e = 0;
  for(int i=1;i<=nf;i++)
  {
    
    sig[e+1] = sig[e+2] = sig[e+3] = 1.0/3.0;

    L[e+1] = (*pnts)[i][2]*(tmxs)[i][12] - (*pnts)[i][3]*(tmxs)[i][11]; 
    L[e+2] = (*pnts)[i][3]*(tmxs)[i][10] - (*pnts)[i][1]*(tmxs)[i][12]; 
    L[e+3] = (*pnts)[i][1]*(tmxs)[i][11] - (*pnts)[i][2]*(tmxs)[i][10]; 

    (*A)[e+1][1] = (*pnts)[i][3]*(tmxs)[i][4] - (*pnts)[i][2]*(tmxs)[i][7];
    (*A)[e+1][2] = (*pnts)[i][3]*(tmxs)[i][5] - (*pnts)[i][2]*(tmxs)[i][8];
    (*A)[e+1][3] = (*pnts)[i][3]*(tmxs)[i][6] - (*pnts)[i][2]*(tmxs)[i][9];

    (*A)[e+2][1] = (*pnts)[i][1]*(tmxs)[i][7] - (*pnts)[i][3]*(tmxs)[i][1];
    (*A)[e+2][2] = (*pnts)[i][1]*(tmxs)[i][8] - (*pnts)[i][3]*(tmxs)[i][2];
    (*A)[e+2][3] = (*pnts)[i][1]*(tmxs)[i][9] - (*pnts)[i][3]*(tmxs)[i][3];

    (*A)[e+3][1] = (*pnts)[i][2]*(tmxs)[i][1] - (*pnts)[i][1]*(tmxs)[i][4];
    (*A)[e+3][2] = (*pnts)[i][2]*(tmxs)[i][2] - (*pnts)[i][1]*(tmxs)[i][5];
    (*A)[e+3][3] = (*pnts)[i][2]*(tmxs)[i][3] - (*pnts)[i][1]*(tmxs)[i][6];

    e += 3;
  }

  svdfit(A,L,sig,ne,R,nu,u,v,w,&chisq,NULL);

   free_matrix(A,1,ne,1,nu);
   free_vector(L,1,ne);

  //VECTOR R=free_vector(R,1,nu);
  free_vector(w,1,nu);

  free_matrix(u,1,ne,1,nu);
  free_matrix(v,1,nu,1,nu);
  free_vector(sig,1,ne);


  nu = 3;
  ne = 2*nf;

  A = matrix(1,ne,1,nu);
  L = vector(1,ne);

  VECTOR D=vector(1,nu);
  w=vector(1,nu);

  u=matrix(1,ne,1,nu);
  v=matrix(1,nu,1,nu);
  sig = vector(1,ne);

  int flag = 1;
  FLOAT oldchisq = 1.0e+15;
  for(int k=1;k<=10;k++)
  {
    e = 0;
    for(i=1;i<=nf;i++)
    {

      sig[e+1] = sig[e+2] = 0.0127/3.0;

      FLOAT x = tmxs[i][1]*R[1] + tmxs[i][2]*R[2] + tmxs[i][3]*R[3] + tmxs[i][10];  
      FLOAT y = tmxs[i][4]*R[1] + tmxs[i][5]*R[2] + tmxs[i][6]*R[3] + tmxs[i][11];  
      FLOAT z = tmxs[i][7]*R[1] + tmxs[i][8]*R[2] + tmxs[i][9]*R[3] + tmxs[i][12];  

      L[e+1] = fl*((*pnts)[i][1]/(*pnts)[i][3]) - fl*(x/z);
      L[e+2] = fl*((*pnts)[i][2]/(*pnts)[i][3]) - fl*(y/z);

      (*A)[e+1][1] = (fl/z)*tmxs[i][1] - (fl*(x/(z*z)))*tmxs[i][7];
      (*A)[e+1][2] = (fl/z)*tmxs[i][2] - (fl*(x/(z*z)))*tmxs[i][8];
      (*A)[e+1][3] = (fl/z)*tmxs[i][3] - (fl*(x/(z*z)))*tmxs[i][9];

      (*A)[e+2][1] = (fl/z)*tmxs[i][4] - (fl*(y/(z*z)))*tmxs[i][7];
      (*A)[e+2][2] = (fl/z)*tmxs[i][5] - (fl*(y/(z*z)))*tmxs[i][8];
      (*A)[e+2][3] = (fl/z)*tmxs[i][6] - (fl*(y/(z*z)))*tmxs[i][9];

      e += 2;
    }

    //printf(" vector L");
    //print_vector(L,1,ne);
/**
    if(flag)
    {
      flag=0;
      oldchisq = 0.0;
      for(i=1;i<=ne;i++)
      {
      
        oldchisq += L[i]*L[i];
      }
//printf("oldchisq = %e\n",oldchisq);
    }
**/
   
//    printf("matrix A");
//    print_matrix(A,1,ne,1,nu);

    svdfit(A,L,sig,ne,D,nu,u,v,w,&chisq,NULL);
//printf(" final w ");
//print_vector(w,1,nu);


    //printf("matrix A");
    //print_matrix(A,1,ne,1,nu);
  
    //printf("chisq = %e\n",chisq);
   // printf("vector D");
   // print_vector(D,1,nu);

    for(i=1;i<=nu;i++)R[i] += D[i];
    if(oldchisq - chisq < oldchisq*1.0e-10)break;
    oldchisq = chisq;

/*****
    FLOAT tmpchisq = 0.0;
    lamda=1.0;   

    for(int jj =1;jj <=12;jj++)
    {
      FLOAT tx = R[1] + lamda*D[1];
      FLOAT ty = R[2] + lamda*D[2];
      FLOAT tz = R[3] + lamda*D[3];
      for(i=1;i<=nf;i++)
      {
        FLOAT x = tmxs[i][1]*tx + tmxs[i][2]*ty + tmxs[i][3]*tz + tmxs[i][10];  
        FLOAT y = tmxs[i][4]*tx + tmxs[i][5]*ty + tmxs[i][6]*tz + tmxs[i][11];  
        FLOAT z = tmxs[i][7]*tx + tmxs[i][8]*ty + tmxs[i][9]*tz + tmxs[i][12];  

        FLOAT t1 = fl*((*pnts)[i][1]/(*pnts)[i][3]) - fl*(x/z);
        FLOAT t2 = fl*((*pnts)[i][2]/(*pnts)[i][3]) - fl*(y/z);
      
        tmpchisq += t1*t1 + t2*t2;
      }

      //printf("tmpchisq = %e\n",tmpchisq);
      if(tmpchisq <= oldchisq)
      {
        for(i=1;i<=nu;i++)R[i] += lamda*D[i];
        //oldchisq = tmpchisq;
        break;
      }
      else
      {
        printf("lamda = %e\n",lamda);
        lamda *= 0.1;
      }
    }
     
    if(lamda == 1.0)
    {
      if(tmpchisq <= oldchisq)
      {
        if(oldchisq - tmpchisq < oldchisq*1.0e-8)
        {
          //printf("ending\n");
          break;
        }
      }
    }

   oldchisq = tmpchisq;
******/

  }

//printf(" final L ");
//print_vector(L,1,ne);
//printf(" final U ");
//print_matrix(u,1,ne,1,nu);
//printf(" final v ");
//print_matrix(v,1,nu,1,nu);
//printf(" final w ");
//print_vector(w,1,nu);
/**
  PNTLST cvm = matrix(1,nu,1,nu);

  svdvar(v,nu,w,cvm);

  printf("covar matrix");
  print_matrix(cvm,1,nu,1,nu);

  free_matrix(cvm,1,nu,1,nu);
**/

// calculate error

  for(i=1;i<=nf;i++)
  {

   PNT pnt = matpnt(tmxs[i],R);
   FLOAT lng = vector_length(3,pnt);
   
   VECTOR c = crosspnt(pnt,(*pnts)[i]);
   
   lng = 10000.0/lng;
//   Output(0,"foto %d ge3dpoint error - %e,  %e, %e\n"
//                    ,i,c[1]*lng,c[2]*lng,c[3]*lng);
   (*error)[i][1] = c[1]*lng;
   (*error)[i][2] = c[2]*lng;
   (*error)[i][3] = c[3]*lng;

   free_vector(pnt,1,3);
   free_vector(c,1,3);
  }

   free_matrix(A,1,ne,1,nu);
   free_vector(L,1,ne);

  free_vector(D,1,nu);
  free_vector(w,1,nu);

  free_matrix(u,1,ne,1,nu);
  free_matrix(v,1,nu,1,nu);
  free_vector(sig,1,ne);


//printf(" exit get3dpnt - %d\n",memcount(0));

  return(R);
}
  
  
PNT get3dpnt_e_pld(int nf,PNTLST pnts, FLOAT **tmxs ,PNTLST error)
{

/*
  This routine calculates a 3d point from 2 or more points identified
  in different photos.



  nf   <= number of points (also number of photos).

  pnts <= list of 2d photo points

  tmxs <= list of transfer matrices per each point (from world to photo )

  
*/



  VECTOR R= NULL;
  int i,j;
  int nu = 3; /* number of unknowns f0r least square calculations - x,y,z of point*/
  int ne = 3*nf; /* number of equations - three per input point */


  /* Parallel Ray Test */

  /* first each input point must be transformed to world or common system */
  /* for each photo ( point )*/
  for(i=1;i<=nf-1;i++)
  {
 
    /* invert photo matrix to get transform from photo to world system */
    VECTOR ti = matinv( tmxs[i] );

    /* transform input point as vector from photo origin to world system*/
    PNT    pi = vector(1,3);
 
    pi[1] = ti[1]*(*pnts)[i][1] + ti[2]*(*pnts)[i][2] + ti[3]*(*pnts)[i][3];
    pi[2] = ti[4]*(*pnts)[i][1] + ti[5]*(*pnts)[i][2] + ti[6]*(*pnts)[i][3];
    pi[3] = ti[7]*(*pnts)[i][1] + ti[8]*(*pnts)[i][2] + ti[9]*(*pnts)[i][3];

    /* insure resulting vector is unit vector */
    FLOAT li = 1.0/sqrt(pi[1]*pi[1] +  pi[2]*pi[2] +  pi[3]*pi[3]);
    pi[1] *= li; 
    pi[2] *= li; 
    pi[3] *= li; 

    free_vector(ti,1,12);

 
     /* perform parallel ray test with other input point vectors */
    for(j=i+1;j<=nf;j++)
    {
      VECTOR tj = matinv( tmxs[j] );
      PNT    pj = vector(1,3);
      pj[1] = tj[1]*(*pnts)[j][1] + tj[2]*(*pnts)[j][2] + tj[3]*(*pnts)[j][3];
      pj[2] = tj[4]*(*pnts)[j][1] + tj[5]*(*pnts)[j][2] + tj[6]*(*pnts)[j][3];
      pj[3] = tj[7]*(*pnts)[j][1] + tj[8]*(*pnts)[j][2] + tj[9]*(*pnts)[j][3];

      FLOAT lj = 1.0/sqrt(pj[1]*pj[1] +  pj[2]*pj[2] +  pj[3]*pj[3]);
      pj[1] *= lj; 
      pj[2] *= lj; 
      pj[3] *= lj; 
      
      free_vector(tj,1,12);

      FLOAT dp = pi[1]*pj[1] + pi[2]*pj[2] + pi[3]*pj[3];

      printf("Parallel Ray Test  %d and %d   %e\n",i,j,dp);
 
      free_vector(pj,1,3);
    }

    free_vector(pi,1,3);
  }


  /* End parallel Ray Test */



//printf(" enter get3dpnt - %d\n",memcount(0));

  /* 

     least squares 

     |L| = |A| |R]

  */

  PNTLST A = matrix(1,ne,1,nu);
  VECTOR L = vector(1,ne);

  /* setup equations for least squares solutions */
  /* for x,y,z of each point */

      int flag=1;
      FLOAT lamda = 10.0;
      FLOAT chisq,oldchi;
      FLOAT fl = -65.124;
    //  FLOAT fl = -65.4676;

  for(j = 1;j<=40;j++)
  {

    if(j==1)
    {

      for(i=1;i<=nf;i++)  // loop over fotos
      {
         FLOAT wf = 1.0;
         PNT pnt;
         VECTOR a;

        a=tmxs[i];
        pnt = (*pnts)[i]; // get foto coordinates for this foto

        if(j<1)
        {
          PNT p = matpnt(a,R);
          FLOAT lng = vector_length(3,p);
          free_vector(p,1,3);
          wf = 10000.0/lng;
        }

        L[i*3-2] = wf*(pnt[3]*a[11]-pnt[2]*a[12]);
        L[i*3-1] = wf*(pnt[1]*a[12]-pnt[3]*a[10]);
        L[i*3]   = wf*(pnt[2]*a[10]-pnt[1]*a[11]);

        (*A)[i*3-2][1] = wf*(pnt[2]*a[7] - pnt[3]*a[4]);
        (*A)[i*3-2][2] = wf*(pnt[2]*a[8] - pnt[3]*a[5]);
        (*A)[i*3-2][3] = wf*(pnt[2]*a[9] - pnt[3]*a[6]);

        (*A)[i*3-1][1] = wf*(pnt[3]*a[1] - pnt[1]*a[7]);
        (*A)[i*3-1][2] = wf*(pnt[3]*a[2] - pnt[1]*a[8]);
        (*A)[i*3-1][3] = wf*(pnt[3]*a[3] - pnt[1]*a[9]);

        (*A)[i*3][1] = wf*(pnt[1]*a[4] - pnt[2]*a[1]);
        (*A)[i*3][2] = wf*(pnt[1]*a[5] - pnt[2]*a[2]);
        (*A)[i*3][3] = wf*(pnt[1]*a[6] - pnt[2]*a[3]);
     

      }
//printf("from get3dpoint");
//printf("nf = %d\n",nf);
//print_vector(L,1,ne);

//print_matrix(A,1,ne,1,nu);

      if(R != NULL)free_vector(R,1,3);
    
      R = lsq(A,L,nu,ne);

      R[1] += 100.0;
      R[2] += 100.0;
      R[3] += 100.0;

    }

    else
    {




      VECTOR y=vector(1,2*nf);
      VECTOR x=vector(1,2*nf);
      VECTOR sig=vector(1,2*nf);

      VECTOR a=vector(1,3);
      VECTOR w=vector(1,3);

      PNTLST u=matrix(1,2*nf,1,3);
      PNTLST v=matrix(1,3,1,3);

      int ndata = 2*nf;
      int ma = 3;

   

      //for(int l=1;l<=4;l++)
      {
   
        for(i=1;i<=2*nf;i++)sig[i]=1.0;

        int e=1;
        FLOAT prechi = 0.0;
        FLOAT dQdx = 0.0;
        FLOAT dQdy = 0.0;
        FLOAT dQdz = 0.0;

        for(i=1;i<=nf;i++)
        {
          VECTOR at=tmxs[i];
          VECTOR pnt = (*pnts)[i]; // get foto coordinates for this foto

          PNT p = matpnt(at,R);

          FLOAT k  = fl/pnt[3];
          FLOAT kk = fl/p[3];

printf("k = %e, kk = %e\n",k,kk);

          y[e]   = k*pnt[1]-kk*p[1];      
          y[e+1] = k*pnt[2]-kk*p[2];

          prechi += y[e]*y[e] + y[e+1]*y[e+1];

          (*A)[e][1] = kk*(at[1]-at[7]*(p[1]/p[3]));
          (*A)[e][2] = kk*(at[2]-at[8]*(p[1]/p[3]));
          (*A)[e][3] = kk*(at[3]-at[9]*(p[1]/p[3]));
  
          (*A)[e+1][1] = kk*(at[4]-at[7]*(p[2]/p[3]));
          (*A)[e+1][2] = kk*(at[5]-at[8]*(p[2]/p[3]));
          (*A)[e+1][3] = kk*(at[6]-at[9]*(p[2]/p[3]));

         dQdx += y[e]*(*A)[e][1] + y[e+1]*(*A)[e+1][1];
         dQdy += y[e]*(*A)[e][2] + y[e+1]*(*A)[e+1][2];
         dQdz += y[e]*(*A)[e][3] + y[e+1]*(*A)[e+1][3];

          e += 2;

        }

        e -= 2;
        
printf("prechi = %e\n",prechi);
getchar();
printf("dQdx = %e, dQdy = %e, dQdz = %e\n",dQdx,dQdy,dQdz);
/**
        for(i=1;i<=3;i++)
        {
          FLOAT sum = 0.0;
      
          for(int jj = 1; jj <= 3;jj++)
          {
            if(i==jj)
            {
              for(int ii=1;ii<=2*nf;ii++)sum += ((*A)[ii][i])*((*A)[ii][i]);
          
              (*A)[e+i][jj] = sqrt(lamda*sum);
            }
            else
            {
              (*A)[e+i][jj] = 0.0;
            }
          }
        }
**/
        if(flag)
        {
printf("flag = 0\n");
          flag = 0;
          oldchi = prechi;
          //break;

        }
        else
        {

          if(fabs(oldchi-prechi)< prechi*1.0e-5)break;
          //if(prechi == oldchi)break;
          if(prechi > oldchi)
          {

printf("chisq increased\n");                
            //FLOAT coef = 1.0/(1.0+lamda);
            R[1] -= a[1];
            R[2] -= a[2];
            R[3] -= a[3];

            lamda = 0.1*lamda;
printf("lamda = %e\n",lamda);       
           // coef = 1.0/(1.0+lamda);
           // R[1] += coef*a[1];
           // R[2] += coef*a[2];
           // R[3] += coef*a[3];

           // break;
          }
          else
          {
printf("chisq decreased\n"); 
            oldchi = prechi;               
            lamda = 10.0*lamda;
printf("lamda = %e\n",lamda);       
           // break;
          }

          

        }
print_matrix(A,1,2*nf,1,3);
print_vector(y,1,2*nf);

      }
      svdfit(A,y,sig,ndata,a,ma,u,v,w,&chisq,NULL);




/**
      e=1;
      for(i=1;i<=nf;i++)
      {
        VECTOR at=tmxs[i];
        VECTOR pnt = (*pnts)[i]; // get foto coordinates for this foto

        PNT p = matpnt(at,R);

        FLOAT k  = fl/pnt[3];
        FLOAT kk = fl/p[3];

printf("k = %e, kk = %e\n",k,kk);

        y[e]   = k*pnt[1]-kk*p[1];      
        y[e+1] = k*pnt[2]-kk*p[2];

        y[e] -= (kk*(at[1]-at[7]*(p[1]/p[3])))*a[1];
        y[e] -= (kk*(at[2]-at[8]*(p[1]/p[3])))*a[2];
        y[e] -= (kk*(at[3]-at[9]*(p[1]/p[3])))*a[3];

        y[e+1] -= (kk*(at[4]-at[7]*(p[2]/p[3])))*a[1];
        y[e+1] -= (kk*(at[5]-at[8]*(p[2]/p[3])))*a[2];
        y[e+1] -= (kk*(at[6]-at[9]*(p[2]/p[3])))*a[3];

        e += 2;

      }
**/

print_vector(y,1,3*nf);



printf("return from svdfit\n");
printf(" chisq = %e\n",chisq);
printf(" a = %e,%e.%e\n",a[1],a[2],a[3]);

      FLOAT coef = 1.0/(1.0+lamda);
      R[1] += a[1];
      R[2] += a[2];
      R[3] += a[3];

printf("R\n");
print_vector(R,1,3);

      free_vector(y,1,2*nf);
      free_vector(x,1,2*nf);
      free_vector(sig,1,2*nf);
      free_vector(a,1,2*nf);
      free_vector(w,1,2*nf);

      free_matrix(u,1,2*nf,1,3);
      free_matrix(v,1,3,1,3);
             
    }
    
  }


  free_vector(L,1,ne);
  free_matrix(A,1,ne+nu,1,nu);


// calculate error

  for(i=1;i<=nf;i++)
  {

   PNT pnt = matpnt(tmxs[i],R);
   FLOAT lng = vector_length(3,pnt);
   
   VECTOR c = crosspnt(pnt,(*pnts)[i]);
   
   lng = 10000.0/lng;
//   printf("foto %d ge3dpoint error - %e,  %e, %e\n"
//                    ,i,c[1]*lng,c[2]*lng,c[3]*lng);
   (*error)[i][1] = c[1]*lng;
   (*error)[i][2] = c[2]*lng;
   (*error)[i][3] = c[3]*lng;

   free_vector(pnt,1,3);
   free_vector(c,1,3);
  }

//printf(" exit get3dpnt - %d\n",memcount(0));

  return(R);
}







PNT get3dpnt_np(int nf,PNTLST pnts, FLOAT **tmxs )
{
  PNTLST error = matrix(1,nf,1,3);
  
  PNT pnt = get3dpnt_e(nf,pnts,tmxs,error);

  free_matrix(error,1,nf,1,3);

  return(pnt);
}







PNT get3dpnt(int nf,PNTLST pnts, FLOAT **tmxs )
{

/*
  This routine calculates a 3d point from 2 or more points identified
  in different points.

  The actual math is performed in routine get3dpnt_e. This routine calculates
  and outputs the ray miss errors. 


  nf   <= number of points (fotos).

  pnts <= list of 2d photo points

  tmxs <= list of transfer matrices per point (from  
*/

  PNTLST error = matrix(1,nf,1,3);
  
  PNT pnt = get3dpnt_e(nf,pnts,tmxs,error);
  for(int i=1;i<=nf;i++)
  {
   printf("foto %d ge3dpoint error - %e,  %e, %e\n"
                    ,i,(*error)[i][1],(*error)[i][2],(*error)[i][3]);
  }

  free_matrix(error,1,nf,1,3);

  return(pnt);
}






VECTOR idmatrix(void)
{
  static FLOAT in[12] = { 1.0,0.0,0.0,
                          0.0,1.0,0.0,
                          0.0,0.0,1.0,
                          0.0,0.0,0.0 };

  return(in-1);
}


void forcesquare( FLOAT *L, FLOAT *R, FLOAT **A)
{
  forcesquare(L,R,A,1.0e3);
  return;
}

void forcesquare( FLOAT *L, FLOAT *R, FLOAT **A,FLOAT wf)
{

  FLOAT c1=wf;
  FLOAT c2=2.0*wf;

   
    L[1] = c1*(1.0-R[1]*R[1]-R[2]*R[2]-R[3]*R[3]);
    L[2] = c1*(1.0-R[4]*R[4]-R[5]*R[5]-R[6]*R[6]);
    L[3] = c1*(1.0-R[7]*R[7]-R[8]*R[8]-R[9]*R[9]);

    L[4] = c1*(1.0-R[1]*R[1]-R[4]*R[4]-R[7]*R[7]);
    L[5] = c1*(1.0-R[2]*R[2]-R[5]*R[5]-R[8]*R[8]);
    L[6] = c1*(1.0-R[3]*R[3]-R[6]*R[6]-R[9]*R[9]);
  
    L[7] = c1*(-R[1]*R[4]-R[2]*R[5]-R[3]*R[6]);
    L[8] = c1*(-R[4]*R[7]-R[5]*R[8]-R[6]*R[9]);
    L[9] = c1*(-R[7]*R[1]-R[8]*R[2]-R[9]*R[3]);

    A[1][1] = c2*R[1];
    A[1][2] = c2*R[2];
    A[1][3] = c2*R[3];

    A[2][4] = c2*R[4];
    A[2][5] = c2*R[5];
    A[2][6] = c2*R[6];

    A[3][7] = c2*R[7];
    A[3][8] = c2*R[8];
    A[3][9] = c2*R[9];
  
    A[4][1] = c2*R[1];
    A[4][4] = c2*R[4];
    A[4][7] = c2*R[7];

    A[5][2] = c2*R[2];
    A[5][5] = c2*R[5];
    A[5][8] = c2*R[8];

    A[6][3] = c2*R[3];
    A[6][6] = c2*R[6];
    A[6][9] = c2*R[9];

    A[7][1] = c1*R[4];
    A[7][4] = c1*R[1];
    A[7][2] = c1*R[5];
    A[7][5] = c1*R[2];
    A[7][3] = c1*R[6];
    A[7][6] = c1*R[3];
 
    A[8][4] = c1*R[7];
    A[8][7] = c1*R[4];
    A[8][5] = c1*R[8];
    A[8][8] = c1*R[5];
    A[8][6] = c1*R[9];
    A[8][9] = c1*R[6];

    A[9][7] = c1*R[1];
    A[9][1] = c1*R[7];
    A[9][8] = c1*R[2];
    A[9][2] = c1*R[8];
    A[9][9] = c1*R[3];
    A[9][3] = c1*R[9];


   return;
}


void forceortho( VECTOR L, VECTOR R, PNTLST A)
{

        L[1] = R[1] - R[5]*R[9] + R[6]*R[8];
        L[2] = R[2] - R[6]*R[7] + R[4]*R[9];
        L[3] = R[3] - R[4]*R[8] + R[5]*R[7];


        L[10] = 1.0  - R[1]*R[1] - R[2]*R[2] - R[3]*R[3]; 
        L[11] = 1.0  - R[4]*R[4] - R[5]*R[5] - R[6]*R[6]; 
        L[12] = 1.0  - R[7]*R[7] - R[8]*R[8] - R[9]*R[9]; 

        (*A)[1][1] = -1.0;
        (*A)[1][5] =  R[9];
        (*A)[1][6] = -R[8];
        (*A)[1][8] =  R[6];
        (*A)[1][9] = -R[5];

        (*A)[2][2] = -1.0;
        (*A)[2][4] = -R[9];
        (*A)[2][6] =  R[7];
        (*A)[2][7] =  R[6];
        (*A)[2][9] = -R[4];

        (*A)[3][3] = -1.0;
        (*A)[3][4] =  R[8];
        (*A)[3][5] = -R[7];
        (*A)[3][7] = -R[5];
        (*A)[3][8] =  R[4];
     
        L[4] = R[4] - R[8]*R[3] + R[9]*R[2];
        L[5] = R[5] - R[9]*R[1] + R[7]*R[3];
        L[6] = R[6] - R[7]*R[2] + R[8]*R[1];

        (*A)[4][4] = -1.0;
        (*A)[4][2] = -R[9];
        (*A)[4][3] =  R[8];
        (*A)[4][8] =  R[3];
        (*A)[4][9] = -R[2];

        (*A)[5][5] = -1.0;
        (*A)[5][1] =  R[9];
        (*A)[5][3] = -R[7];
        (*A)[5][7] = -R[3];
        (*A)[5][9] =  R[1];

        (*A)[6][6] = -1.0;
        (*A)[6][1] = -R[8];
        (*A)[6][2] =  R[7];
        (*A)[6][7] =  R[2];
        (*A)[6][8] = -R[1];

        L[7] = R[7] - R[2]*R[6] + R[3]*R[5];
        L[8] = R[8] - R[3]*R[4] + R[1]*R[6];
        L[9] = R[9] - R[1]*R[5] + R[2]*R[4];

        (*A)[7][7] = -1.0;
        (*A)[7][2] =  R[6];
        (*A)[7][3] = -R[5];
        (*A)[7][5] = -R[3];
        (*A)[7][6] =  R[2];

        (*A)[8][8] = -1.0;
        (*A)[8][1] = -R[6];
        (*A)[8][3] =  R[4];
        (*A)[8][4] =  R[3];
        (*A)[8][6] = -R[1];
      
        (*A)[9][9] = -1.0;
        (*A)[9][1] =  R[5];
        (*A)[9][2] = -R[4];
        (*A)[9][4] = -R[2];
        (*A)[9][5] =  R[1];

        (*A)[10][1] = 2.0*R[1];
        (*A)[10][2] = 2.0*R[2];
        (*A)[10][3] = 2.0*R[3];

        (*A)[11][4] = 2.0*R[4];
        (*A)[11][5] = 2.0*R[5];
        (*A)[11][6] = 2.0*R[6];

        (*A)[12][7] = 2.0*R[7];
        (*A)[12][8] = 2.0*R[8];
        (*A)[12][9] = 2.0*R[9];

  return;
}




 VECTOR findscale(VECTOR K)
{
  
  int nu = 12;
  int ne = 18;

  VECTOR R = vector(1,nu);

  PNTLST A = matrix(1,ne,1,nu);
  VECTOR L = vector(1,ne);
  
  copy_vector(R,1,9,K,1,9);
  R[10] = 1.0;
  R[11] = 1.0;
  R[12] = 1.0;

  for(int loop =1; loop <= 50; loop++)
  {

      int e = 10;
      int si = 10;
      int ki = 1;

    forcesquare( L, R, *A);

    for(int i=1;i<=3;i++)
    { 

      L[e] = K[ki] - R[ki]*R[si];
      (*A)[e][ki] = R[si];
      (*A)[e][si] = R[ki];

      e++;
      ki++;

      L[e] = K[ki] - R[ki]*R[si+1];
      (*A)[e][ki] = R[si+1];
      (*A)[e][si+1] = R[ki];
  
      e++;
      ki++;

      L[e] = K[ki] - R[ki]*R[si+2];
      (*A)[e][ki] = R[si+2];
      (*A)[e][si+2] = R[ki];
  
      e++;
      ki++;
   }
/*    
printf("vector L - scale\n");
print_vector(L,1,ne);

printf("matrix A\n");
print_matrix(A,1,ne,1,nu);
*/
   VECTOR D=lsq(A,L,nu,ne);
    add_vector(R,1,nu,D,1,nu);

//printf("vector D  scale \n");
//print_vector(D,1,nu);

    free_vector(D,1,nu);
//printf("vector R  scale\n");
//print_vector(R,1,nu);
  }

  free_matrix(A,1,ne,1,nu);
  free_vector(L,1,ne);

  return(R);
}    
  








FLOAT srot(int np,PNTLST p,PNTLST xyz)
{
  /* routine to calculate Tmatrix to take points p to xyz */

  FLOAT max,sum, sf;
  FLOAT *D;
  int flag;
  int nu= 13;
  int ne = 9 + 3*np;

  int i,k;
printf("enter srot - %ld\n",memcount(0));

  FLOAT *R = vector(1,nu);
  MATRIX *A = matrix(1,ne,1,nu);
  FLOAT *L = vector(1,ne);

  R[1] = R[5] = R[9] = R[13] = 1.0;
  

  for(k=1;k<=250;k++)
  {
    
     forcesquare( L, R, *A);
    for(i=1;i<=np;i++)
    {
      L[7+i*3] = (*xyz)[i][1] - R[13]*((*p)[i][1]*R[1] +(*p)[i][2]*R[2] + (*p)[i][3]*R[3])
                            - R[10];

      L[8+i*3] = (*xyz)[i][2] - R[13]*((*p)[i][1]*R[4] +(*p)[i][2]*R[5] + (*p)[i][3]*R[6])
                            - R[11];

      L[9+i*3] = (*xyz)[i][3] - R[13]*((*p)[i][1]*R[7] +(*p)[i][2]*R[8] + (*p)[i][3]*R[9])
                            - R[12];

      (*A)[7+i*3][1] = R[13]*(*p)[i][1];
      (*A)[7+i*3][2] = R[13]*(*p)[i][2];
      (*A)[7+i*3][3] = R[13]*(*p)[i][3];
      (*A)[7+i*3][10] = 1;
      (*A)[7+i*3][13] = (*p)[i][1]*R[1] +(*p)[i][2]*R[2] + (*p)[i][3]*R[3];


      (*A)[8+i*3][4] = R[13]*(*p)[i][1];
      (*A)[8+i*3][5] = R[13]*(*p)[i][2];
      (*A)[8+i*3][6] = R[13]*(*p)[i][3];
      (*A)[8+i*3][11] = 1;
      (*A)[8+i*3][13] = (*p)[i][1]*R[4] +(*p)[i][2]*R[5] + (*p)[i][3]*R[6];


      (*A)[9+i*3][7] = R[13]*(*p)[i][1];
      (*A)[9+i*3][8] = R[13]*(*p)[i][2];
      (*A)[9+i*3][9] = R[13]*(*p)[i][3];
      (*A)[9+i*3][12] = 1;
      (*A)[9+i*3][13] = (*p)[i][1]*R[7] +(*p)[i][2]*R[8] + (*p)[i][3]*R[9];
    }  

/*
printf("vector L - sort\n");
print_vector(L,1,ne);

printf("matrix A\n");
print_matrix(A,1,ne,1,nu);
*/
    D=lsq(A,L,nu,ne);
    add_vector(R,1,nu,D,1,nu);
/*
printf("vector D\n");
print_vector(D,1,nu);


printf("vector R\n");
print_vector(R,1,nu);
*/    
    flag = 0;

    for(i=1;i<=nu;i++)
    {
      if(D[i]<0.0)D[i] = -D[i];
      if(D[i]>1.0e-12)
      {
        flag = 1;
      }
    }
    free_vector(D,1,np);
    if(flag == 0)break;
  }

  max =sum=0.0;
  for(i =10;i<=ne;i=i++)
  {
      //FLOAT error = sqrt(L[i]*L[i] + L[i+1]*L[i+1] + L[i+2]*L[i+2]); 
      sum += L[i];    
      //if(error > max)max = error;
  }

  sum = sum/(np*3);
  printf("average error = %e, max error = %e\n",sum,max);
  
  for( i = 1; i <= np; i++ )
  {
    float x, y, z;
    
    x = R[13]*((*p)[i][1]*R[1] +(*p)[i][2]*R[2] + (*p)[i][3]*R[3])
                            + R[10];

    y = R[13]*((*p)[i][1]*R[4] +(*p)[i][2]*R[5] + (*p)[i][3]*R[6])
                            + R[11];

    z = R[13]*((*p)[i][1]*R[7] +(*p)[i][2]*R[8] + (*p)[i][3]*R[9])
                            + R[12];
			    
    (*p)[i][1] = x;
    (*p)[i][2] = y;
    (*p)[i][3] = z;

  }
  
  sf = R[13];
  free_vector(R,1,nu);
  free_vector(L,1,ne);
  free_matrix(A,1,ne,1,nu);
  return(sf);
}



    double tsumdx = 0.0;
    double tsumdy = 0.0;
    double tsumxsqr = 0.0;
    double tsumysqr = 0.0;
    int    tnum = 0;

//    PNTLST avgcp = NULL;
//    int    numcp = 0;

VECTOR getfilmcorr(int np,FLOAT fl, PNTLST fud_p,PNTLST fp_p)
{
   // np     - number of fiducial points
   // fl     - camera focal length
   // fud_p  - list of measured fiducial point coordinates (2d)
   // fp_p   - list of nominal  fiducial point coordinates (2d)

  /*  given a set of 2d coordinates of the measured fiducial positions (film coordinates)
      along with their nominal values (camera coordinate system), this routine will
      calculate the 2d transform (matrix) to take points from the film coordinate system
      to the camera system;
  */



    int i;
    FLOAT ***Ap;
    FLOAT **A,*R;  
    FLOAT *L,*l;
    MATRIX fud = *fud_p;
    MATRIX fp = *fp_p;
    // double maxrms =0.027;
    double maxrms =0.055;

  Output(0,"\n\nFIDUCIAL CALCULATIONS \n");
  Output(0,"  find transform from scanned image (photo)system to film (camera) system\n\n");

    //if(avgcp == NULL)avgcp = matrix(1,np,1,2);

    for(;;)
  {    

    int ne = np*2;
    int nu = 6;

    Ap = matrix(1,ne,1,nu);
    A = *Ap;

    L = vector(1,ne);

    ne = 0;
    for( i = 1; i <= np; i++ )
    {

      int ii;
      if(fud[i][1] != 0.0)
      {
        ne++;
	ii=ne*2-2;

	L[ii+1] = fp[i][1];
	A[ii+1][1] = fud[i][1];
	A[ii+1][2] = fud[i][2];
	A[ii+1][3] = 1.0;

	L[ii+2] = fp[i][2];
	A[ii+2][4] = fud[i][1];
	A[ii+2][5] = fud[i][2];
	A[ii+2][6] = 1.0;
      }
    }
    
    R=lsq(Ap,L,nu,ne+ne);


    PNTLST cp = matrix(1,np,1,2);

    double num = 0.0;
    double total = 0.0;
    double maxval = 0.0;
    double sumdx = 0.0;
    double sumdy = 0.0;
    double sumxsqr = 0.0;
    double sumysqr = 0.0;

    int maxi = 0;
    
    double ttsumdx = 0.0;
    double ttsumdy = 0.0;
    double ttsumxsqr = 0.0;
    double ttsumysqr = 0.0;
    int    ttnum = 0;

printf("calculated fiducial deltas from actual, num pnts = %d\n",ne);
    for( i = 1; i <= np; i++ )
    {
       if(fud[i][1] != 0.0)
       {
	   // printf("%e,%e\n",fp[i][1],fp[i][2]);
	   (*cp)[i][1] = R[1] * fud[i][1] + R[2] * fud[i][2] + R[3];
	   (*cp)[i][2] = R[4] * fud[i][1] + R[5] * fud[i][2] + R[6];




//           (*avgcp)[i][1] = ((numcp*(*avgcp)[i][1]) + (*cp)[i][1])/(numcp+1);
//           (*avgcp)[i][2] = ((numcp*(*avgcp)[i][2]) + (*cp)[i][2])/(numcp+1);
//           numcp += 1;
	   
	   num++;
	   double dx = (*cp)[i][1] - fp[i][1];
	   double dy = (*cp)[i][2] - fp[i][2];
	   double temp = sqrt(dx*dx + dy*dy);
           sumdx += dx;
           sumdy += dy;
           sumxsqr += dx*dx;    
           sumysqr += dy*dy;    

           ttsumdx += dx;
           ttsumdy += dy;
           ttsumxsqr += dx*dx;    
           ttsumysqr += dy*dy;    
  

	   if(temp > maxval)
	   {
	     maxval = temp;
	     maxi = i;
	   }
	   total += temp;
    
	   printf( "%e,%e\n", dx,dy);
           // printf( " square r error = %e, %e\n",dx*dx + dy*dy,sqrt(dx*dx + dy*dy));
       }
    }

printf(" fiducals");
print_matrix(cp,1,np,1,2);

    ttnum += num;
    tnum += ttnum;
    tsumdx += ttsumdx;
    tsumdy += ttsumdy;
    tsumxsqr += ttsumxsqr;
    tsumysqr += ttsumysqr;

    total = total/num;
printf("average delta = %f\n",total);

    //printf(" average dx    = %e\n",sumdx/num);
    //printf(" average dy    = %e\n",sumdy/num);
    //printf(" average dx,dy = %e\n",(sumdx+sumdy)/(num*2));
    //printf(" root mean square dx    = %e, %e\n",sumxsqr/num,sqrt(sumxsqr/num)); 
    //printf(" root mean square dy    = %e, %e\n",sumysqr/num,sqrt(sumysqr/num)); 
    //printf(" root mean square error = %e, %e\n",
    //                 (sumxsqr+sumysqr)/(num),sqrt((sumxsqr+sumysqr)/(num))); 


   // printf(" t average dx    = %e\n",tsumdx/tnum);
   // printf(" t average dy    = %e\n",tsumdy/tnum);
   // printf(" t average dx,dy = %e\n",(tsumdx+tsumdy)/(tnum*2));
   // printf(" t root mean square dx    = %e, %e\n",tsumxsqr/tnum,sqrt(tsumxsqr/tnum)); 
   // printf(" t root mean square dy    = %e, %e\n",tsumysqr/tnum,sqrt(tsumysqr/tnum)); 
   // printf(" t root mean square error = %e, %e\n",
   //                  (tsumxsqr+tsumysqr)/(tnum),sqrt((tsumxsqr+tsumysqr)/(tnum))); 

    if(total >= 0.05)printf("******** maxval exceeds maxrms ********\n");

    //getchar();

    free_vector(L,1,ne);
    free_matrix(Ap,1,ne,1,nu);

    //if(total < 0.05)
    if(maxval < maxrms)
    {

      Output(0," Transformation matrix from photo to camera \n");
      Output(0,"    %e     %e\n",R[1],R[2]);
      Output(0,"    %e     %e\n\n",R[4],R[5]);
      Output(0,"    %e     %e\n",R[3],R[6]);

      return(R);
    }
    else
    {

      tnum -= ttnum;
      tsumdx -= ttsumdx;
      tsumdy -= ttsumdy;
      tsumxsqr -= ttsumxsqr;
      tsumysqr -= ttsumysqr;


      if(ne <= 4)
      {
         printf(" +++++++++++ too many fiducials in error +++++++++\n");
         return(R);
      }
      else
      {
          fud[maxi][1] = 0.0;
          fud[maxi][2] = 0.0;
          printf(" * * * * * fiducial pnt %d not used * * * * *\n",maxi);
      }
    }

  }
}


void radial_corr(int np ,PNTLST pnt_p,VECTOR rcf)
{
    FLOAT x,y,r,r2,r3,r5,r7,dr;
   int i;
   MATRIX pnt = *pnt_p;

   Output(3,"\n\nRADICAL CORRECTION OF INPUT POINTS\n");

   for(i=1;i<=np;i++)
   {
     x=pnt[i][1];
     y=pnt[i][2];
    
      x = x-rcf[6];  // new radial corr
      y = y-rcf[7];  // new radial corr

     r2 = x*x + y*y;
     r = sqrt(r2);
     r3 = r*r2;
     r5 = r3*r2;
     r7 = r5*r2;

     //dr = rcf[1]*r + rcf[2]*r3 + rcf[3]*r5;  //old radial correct
      dr = rcf[1]*r3 + rcf[2]*r5 + rcf[3]*r7; // new radial corr
 
//printf("x,y before radial correction");
//print_matrix(pnt_p,1,np,1,2);
     pnt[i][1] = x + (dr*x)/r + (r2+2.0*x*x)*rcf[4] + 2.0*rcf[5]*x*y;
     pnt[i][2] = y + (dr*y)/r + (r2+2.0*y*y)*rcf[5] + 2.0*rcf[4]*x*y;
//printf("x,y after radial correction");
//print_matrix(pnt_p,1,np,1,2);

     FLOAT delta = sqrt( (x-pnt[i][1])*(x-pnt[i][1]) + (y-pnt[i][2])*(y-pnt[i][2]) );
     FLOAT lng   = sqrt( x*x + y*y);

     Output(3,"Pnt - %d\n",i);
     Output(3," radial correction, org lngth  %e \n   change   absolute %e,  relative %e \n\n",
                                         lng, delta, delta/lng);

  }
  return;
}

PNTLST corrfilmpnt(int npt,FLOAT fl, VECTOR R, PNTLST pnt_p, VECTOR rcf)
{
   int i;
   PNTLST uv_p = matrix(1,npt,1,3);
   MATRIX uv = *uv_p;
   MATRIX pnt = *pnt_p;

//printf("enter corrfilmpnt\n");
//print_vector(R,1,6);
   for(i=1;i<=npt;i++)
   {
       FLOAT x,y;


      x = R[1]*pnt[i][1]+
          R[2]*pnt[i][2]+R[3];

      y = R[4]*pnt[i][1]+
          R[5]*pnt[i][2]+R[6];

       uv[i][1] = x;
       uv[i][2] = y;
       uv[i][3] = fl;

//printf("x,y = %e,%e\n",x,y);
   }

   radial_corr(npt,uv_p,rcf);

   for(i=1;i<=npt;i++)
   {
     VECTOR temp = unit_vector(3,uv[i]);
     copy_vector(uv[i],1,3,temp,1,3);
     free_vector(temp,1,3);
   }

//printf("nv matrix \n");
//print_matrix(uv_p,1,npt,1,3);
   return(uv_p);
}




PNTLST LUK(int nrk, int nck, int nre, PNTLST teq, PNTLST tkn)
{

//  [eq] = [u]*[kn]
//  solves for [U]  nrk - rows in [kn]  , columns in [u]
//                  nck = columns in [kn],  columns in [eq]
//                  nre  rows in [eq],  rows in [u]
  int i=0;

//  PNTLST  tkn= transpose(kn,1,nrk,1,nck);

//printf("teq matrix\n");
//print_matrix(teq,1,nck,1,nre);

//printf("tkn matrix\n");
//print_matrix(tkn,1,nck,1,nrk);

  PNTLST  A = matrix(1,nck*nre,1,nrk*nre);

  for( i = 0; i < nre; i++)
  {
    PNTLST tmp = submatrix(A,(i*nck)+1,(i*nck)+nck, (i*nrk)+1,(i*nrk)+nrk,1,1);
//print_matrix(a,1,nck*nre,1,nck*nre); 
    copy_matrix(tmp, 1,nck,1,nrk,tkn,1,nck,1,nrk);
    free_submatrix(tmp,1,nck,1,nrk);
  }

//  free_matrix(tkn,1,nck,1,nrk);


//printf("matrix A \n");
//print_matrix(A,1,nck*nre,1,nrk*nre);

  VECTOR L = vector(1,nck*nre);

 PNTLST eq = transpose(teq,1,nck,1,nre);
//printf(" matrix eq\n");
//print_matrix(eq,1,nre,1,nck);
  for(i=0;i<nre;i++)
  {
    VECTOR tmp = subvector(L, (i*nck)+1, (i*nck)+nck,1);
    copy_vector(tmp,1,nck, (*eq)[i+1],1,nck );
  }

  free_matrix(eq,1,nck,1,nre);

//printf(" vector L \n");
//print_vector(L,1,nck*nre);

  int nu = nre*nrk;
  int ne = nre*nck;

//printf("num equ = %d,  num unk = %d\n",ne,nu);

  VECTOR R=NULL;

  if(nu == ne)
  { 
//printf("calling solve\n");



    R = solve(A,L,ne);


  }
  else
  {
    if(ne > nu)
    {

//printf("calling lsq\n");
      R = lsq(A,L,nu,ne);
    }
    else
    {
      printf("******** ERROR in LUK - ne < nu *******\n");
      R=NULL;
    }
  }

//printf(" vector R\n");
//print_vector(R,1,nre*nrk);

  free_vector(L,1,nck*nre);
  free_matrix(A,1,nck*nre,1,nrk*nre);

  if(R == NULL)return(NULL);

  PNTLST u = matrix(1,nre,1,nrk);
  for(i = 0;i<nre;i++)
  {
    VECTOR temp = subvector(R,(i*nrk)+1,(i*nrk)+nrk,1);
    copy_vector((*u)[i+1],1,nrk,temp,1,nrk);
  }

  free_vector(R,1,nu);
  return(u);
}


/***
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
***/

FLOAT getcamera( int np, PNTLST p, PNTLST rpp, VECTOR RR )
{
  int flag,k,i;
  int nu = 12;
  int ne = 9+3*np;

  FLOAT sum,max;
  VECTOR L = vector(1,ne);
  VECTOR D = NULL;
//printf("enter getcamera - %d\n",memcount(0));

  PNTLST A = matrix(1,ne,1,nu);

  VECTOR R = vector(1,13);
  R[1] = 1.0;
  R[5] = 1.0;
  R[9] = 1.0;
  R[10] = 1.0;
  R[11] = 1.0;
  R[12] = 1.0;

  PNTLST rp = matrix_shell(1,np,1,3);

  for(i=1;i<=np; i++)
  {

    (*rp)[i] = matpnt(RR,(*rpp)[i]);
  }    




  for(k=1;k<=400;k++)
  {

    if(R[5] < 0.0)
    {
       R[4] = -R[4];
       R[5] = -R[5];
       R[6] = -R[6];

       R[7] = -R[7];
       R[8] = -R[8];
       R[9] = -R[9];

     //  printf("*******FLIPPED *******\n");
    }
    
    forcesquare( L, R, *A);
    
    for(int i = 1; i<= np; i++)
    {
    
      VECTOR tp = matpnt(R, (*rp)[i] );
//printf("pnt = %e,%e,%e\n",tp[1],tp[2],tp[3]);
     // VECTOR temp = unit_vector(3,tp);
        
     // multvector(3,temp,10000.0);
     // free_vector(temp,1,3);
     // tp = temp;

      VECTOR fp = (*p)[i];
      
      L[7+ 3*i ]= -tp[2]*fp[3] + tp[3]*fp[2];
      L[8+ 3*i ]= -tp[3]*fp[1] + tp[1]*fp[3];
      L[9+ 3*i ]= -tp[1]*fp[2] + tp[2]*fp[1];

       
      (*A)[7+ 3*i][4] = fp[3]*(*rp)[i][1];
      (*A)[7+ 3*i][5] = fp[3]*(*rp)[i][2];
      (*A)[7+ 3*i][6] = fp[3]*(*rp)[i][3];
      (*A)[7+ 3*i][11] = fp[3];

      (*A)[7+ 3*i][7] = -fp[2]*(*rp)[i][1];
      (*A)[7+ 3*i][8] = -fp[2]*(*rp)[i][2];
      (*A)[7+ 3*i][9] = -fp[2]*(*rp)[i][3];
      (*A)[7+ 3*i][12] = -fp[2];


 

      (*A)[8+ 3*i][7] = fp[1]*(*rp)[i][1];
      (*A)[8+ 3*i][8] = fp[1]*(*rp)[i][2];
      (*A)[8+ 3*i][9] = fp[1]*(*rp)[i][3];
      (*A)[8+ 3*i][12] = fp[1];

      (*A)[8+ 3*i][1] = -fp[3]*(*rp)[i][1];
      (*A)[8+ 3*i][2] = -fp[3]*(*rp)[i][2];
      (*A)[8+ 3*i][3] = -fp[3]*(*rp)[i][3];
      (*A)[8+ 3*i][10] = -fp[3];



      (*A)[9+ 3*i][1] = fp[2]*(*rp)[i][1];
      (*A)[9+ 3*i][2] = fp[2]*(*rp)[i][2];
      (*A)[9+ 3*i][3] = fp[2]*(*rp)[i][3];
      (*A)[9+ 3*i][10] = fp[2];

      (*A)[9+ 3*i][4] = -fp[1]*(*rp)[i][1];
      (*A)[9+ 3*i][5] = -fp[1]*(*rp)[i][2];
      (*A)[9+ 3*i][6] = -fp[1]*(*rp)[i][3];
      (*A)[9+ 3*i][11] = -fp[1];

      free_vector(tp,1,3);
    }


//printf("vector L - getcamera\n");
//print_vector(L,1,ne);
/*
printf("matrix A\n");
print_matrix(A,1,ne,1,nu);
*/
    D=lsq(A,L,nu,ne);
    //add_vector(R,1,nu,D,1,nu);
    for(i=1;i<=nu;i++)R[i] += 0.3*D[i];
//printf("vector D - getcamera,%d\n",k);
//print_vector(D,1,nu);


// printf("vector R\n");
// print_vector(R,1,nu);

    flag = 0;

    for(i=1;i<=nu;i++)
    {
      if(D[i]<0.0)D[i] = -D[i];
      if(D[i]>1.0e-10)
      {
        flag = 1;
      }
    }
    free_vector(D,1,nu);

    max =sum=0.0;

    for(i =10;i<=ne;i++)
    {
        FLOAT error = L[i];
        if(error<0.0)error=-error; 
        sum += error;    
        if(error > max)max = error;
    }

  
    sum = sum/(np*3);
    if(sum > 1.0e50)flag=0;
// printf("getcamera  average error = %e, max error = %e\n",sum,max);

    if(flag == 0)break;

  }
  free_vector(L,1,ne);
  free_matrix(A,1,ne,1,nu);

  free_matrix(rp,1,np,1,3);

  VECTOR tm = matmat(R,RR);
  copy_vector(RR,1,12,tm,1,12);

  free_vector(R,1,12);

  free_vector(tm,1,12);


 // printf(" results getcamera \n");
//  print_vector(RR,1,12);
display_tm(RR);

 printf("getcamera  average error = %e, max error = %e\n\n",sum,max);
//printf("exit getcamera - %d\n",memcount(0));



  return(sum);

}



FLOAT getcamera_2( int np, PNTLST p, PNTLST rpp, VECTOR RR )
{

  //  np = number of points in p and rpp lista
  //   p = list of photo points (rays)
  // rpp = list of 3d points 
  //  RR = initial estimate of transformation needed to take points in rpp list
  //       to photo system such that they will be intersected by rays in p list.


  // this routine uses a least squares fit to determine the camera position and 
  // for a given photo when provided with pairs of photo points and measured points.
  // At least three sets of matched points must be provided. 
  // 
  // An initial approx. transform RR must be provided to indicate proper quadrant info.
  // On return this matrix will be updated with the final results.


  int flag,k,i;
  int nu = 12+np; // 12 unknowns - camera position matrix, 1 t par. per point
  int ne = 9+3*np;// 9 equations for matrix orth., 3 per point for vector difference
                  // a1*x + a2*y + a3*z + a10 = t*px
                  // a4*x + a5*y + a6*z + a11 = t*py
                  // a7*x + a8*y + a9*z + a12 = t*pz

  FLOAT sum,max;
  VECTOR L = vector(1,ne);
  VECTOR D = NULL;
//printf("enter getcamera - %d\n",memcount(0));

  PNTLST A = matrix(1,ne,1,nu);

  VECTOR R = vector(1,nu);

  // initialize matrix to unity
  R[1] = 1.0;
  R[5] = 1.0;
  R[9] = 1.0;
  R[10] = 1.0;
  R[11] = 1.0;
  R[12] = 1.0;

  PNTLST rp = matrix_shell(1,np,1,3);



  for(i=1;i<=np; i++)
  {

    (*rp)[i] = matpnt(RR,(*rpp)[i]);
  }    



  // Begin Least Squares Iterative Solution

  //   linearization of equations
  
  // 0 = (a1*xi + a2*yi + a3*zi + a10 - ti*pxi) + xi*da1 + yi*da2 + zi*da3 + da10 - pxi*dti
  // 0 = (a4*xi + a5*yi + a6*zi + a11 - ti*pyi) + xi*da4 + yi*da5 + zi*da6 + da11 - pyi*dti
  // 0 = (a7*xi + a8*yi + a9*zi + a12 - ti*pzi) + xi*da7 + yi*da8 + zi*da9 + da12 - pzi*dti

  //     one set of equations per point;
  //      after iteration unknowns 
  //            a1 = a1 + da1
  //            a2 = a2 + da2
  //               .
  //               .
  //            a12 = a12 + da12
  //
  //            t1 = t1 + dt1
  //            t2 = t2 + dt2
  //               .
  //               .
  //            tn = tn + dtn  where n is number of points


  for(k=1;k<=400;k++)
  {

    if(R[5] < 0.0)
    {
       R[4] = -R[4];
       R[5] = -R[5];
       R[6] = -R[6];

       R[7] = -R[7];
       R[8] = -R[8];
       R[9] = -R[9];

//       printf("*******FLIPPED *******\n");
    }
    
    forcesquare( L, R, *A);
  
    int e = 10;
    int ti = 13;
    
    for(int i = 1; i<= np; i++)
    {
    
      VECTOR tp = matpnt(R, (*rp)[i] );   // target point in photo system
//printf("pnt = %e,%e,%e\n",tp[1],tp[2],tp[3]);
     // VECTOR temp = unit_vector(3,tp);
        
     // multvector(3,temp,10000.0);
     // free_vector(temp,1,3);
     // tp = temp;

      VECTOR fp = (*p)[i];

       if(k == 1) // estimate initial values for R's, only done on first iteration
       {
          FLOAT temp = fp[1]*tp[1] + fp[2]*tp[2] + fp[3]*tp[3];
       
          temp = temp-100.0;
          if(temp< 0.0)temp = 0;
    
          R[ti] = sqrt(temp);

       }

      
      L[e  ] = fp[1]*(100 + R[ti]*R[ti]) - tp[1];
      L[e+1] = fp[2]*(100 + R[ti]*R[ti]) - tp[2];
      L[e+2] = fp[3]*(100 + R[ti]*R[ti]) - tp[3];

       
      (*A)[e  ][1] = (*rp)[i][1];
      (*A)[e  ][2] = (*rp)[i][2];
      (*A)[e  ][3] = (*rp)[i][3];
      (*A)[e  ][10] = 1.0;

      (*A)[e  ][ti] = -2.0*fp[1]*R[ti];


      (*A)[e+1][4] = (*rp)[i][1];
      (*A)[e+1][5] = (*rp)[i][2];
      (*A)[e+1][6] = (*rp)[i][3];
      (*A)[e+1][11] = 1.0;

      (*A)[e+1][ti] = -2.0*fp[2]*R[ti];


      (*A)[e+2][7] = (*rp)[i][1];
      (*A)[e+2][8] = (*rp)[i][2];
      (*A)[e+2][9] = (*rp)[i][3];
      (*A)[e+2][12] = 1.0;

      (*A)[e+2][ti] = -2.0*fp[3]*R[ti];

    e += 3;
    ti += 1;
    free_vector(tp,1,3);
    }


//printf("vector L - getcamera\n");
//print_vector(L,1,ne);
/*
printf("matrix A\n");
print_matrix(A,1,ne,1,nu);
*/
    D=lsq(A,L,nu,ne);
    //add_vector(R,1,nu,D,1,nu);
    for(i=1;i<=nu;i++)R[i] += 0.3*D[i];
//printf("vector D - getcamera,%d\n",k);
//print_vector(D,1,nu);


// printf("vector R\n");
// print_vector(R,1,nu);

    flag = 0;

    for(i=1;i<=nu;i++)
    {
      if(D[i]<0.0)D[i] = -D[i];
      if(D[i]>1.0e-10)
      {
        flag = 1;
      }
    }
    free_vector(D,1,nu);

    max =sum=0.0;

    for(i =10;i<=ne;i++)
    {
        FLOAT error = L[i];
        if(error<0.0)error=-error; 
        sum += error;    
        if(error > max)max = error;
    }

  
    sum = sum/(np*3);
    if(sum > 1.0e50)flag=0;
// printf("getcamera  average error = %e, max error = %e\n",sum,max);

    if(flag == 0)break;

  }


    for(i =10;i<=ne;i++)
    {

        printf("/n Ray errors");
        printf("[%d]  %e\n",i-10,L[i]);
       // FLOAT error = L[i];
       // if(error<0.0)error=-error; 
       // sum += error;    
       // if(error > max)max = error;
    }




  free_vector(L,1,ne);
  free_matrix(A,1,ne,1,nu);

  free_matrix(rp,1,np,1,3);

  VECTOR tm = matmat(R,RR);
  copy_vector(RR,1,12,tm,1,12);

  free_vector(R,1,12);

  free_vector(tm,1,12);


 // printf(" results getcamera \n");
//  print_vector(RR,1,12);
display_tm(RR);

 printf("getcamera  average error = %e, max error = %e\n",sum,max);
//printf("exit getcamera - %d\n",memcount(0));



  return(sum);

}



/**

void get_camera3_funcs(PNTLST data, VECTOR a,VECTOR ymod,PNTLST dyda,int ndata,int ma)
{

    int e = 10;
    int ti = 13;
 
    int np = (ndata-9)/5;
    FLOAT fl = -65.124;

    PNTLST rp = data;
    
    for(int i = 1; i<= np; i++)
    {
    
      VECTOR tp = matpnt(a, (*rp)[i] );   // target point in photo system

      VECTOR fp = (*p)[i]; // photo point 

      FLOAT px = fp[1]*(fl/fp[3]);
      FLOAT py = fp[2]*(fl/fp[3]);

      FLOAT xu = a[ti];
      FLOAT yu = a[ti+1];
      FLOAT zu = a[ti+2];
      
      ymod[e  ] = xu - tp[1];
      ymod[e+1] = yu - tp[2];
      ymod[e+2] = zu - tp[3];


      FLOAT temp  = fl/zu;
      FLOAT temp2 = temp/zu;

      ymod[e+3] = temp*xu;
      ymod[e+4] = temp*yu;

      (*dyda)[e  ][1] =  (*rp)[i][1];
      (*dyda)[e  ][2] =  (*rp)[i][2];
      (*dyda)[e  ][3] =  (*rp)[i][3];
      (*dyda)[e  ][10] = 1.0;
      (*dyda)[e  ][ti ] = -1.0;

      (*dyda)[e+1][4] =  (*rp)[i][1];
      (*dyda)[e+1][5] =  (*rp)[i][2];
      (*dyda)[e+1][6] =  (*rp)[i][3];
      (*dyda)[e+1][11] = 1.0;
      (*dyda)[e+1][ti+1] = -1.0;

      (*dyda)[e+2][7] =  (*rp)[i][1];
      (*dyda)[e+2][8] =  (*rp)[i][2];
      (*dyda)[e+2][9] =  (*rp)[i][3];
      (*dyda)[e+2][12] = 1.0;
      (*dyda)[e+2][ti+2] = -1.0;

                    
                     // px - fl*(xu/zu) = (fl/zu)*dxu - ((fl*xu)/(zu*zu))dzu;
                     // py - fl*(yu/zu) = (fl/zu)*dyu - ((fl*yu)/(zu*zu))dzu;
 
      (*dyda)[e+3][ti  ] = temp;
      (*dyda)[e+3][ti+2] = -temp2*xu;

      (*dyda)[e+4][ti+1] = temp;
      (*dyda)[e+4][ti+2] = -temp2*yu;
  }

}

**/

void square_matrix(VECTOR RR)
{

//printf("enter square matrix\n");
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

//print_matrix(a,1,3,1,3);

  svdcmp(a,3,3,w,v);
/**
  printf("############################\n");
  printf(" results from svdcmp\n");
  print_vector(w,1,3);
  print_matrix(a,1,3,1,3);
  print_matrix(v,1,3,1,3);
**/

  //Output(0,"\n Ortho measure = %e,  %e,  %e  \n",w[1],w[2],w[3]);
 // Output(0," nominal values  = %e,  %e,  %e  \n",1.0,1.0,1.0);

  RR[1] = (*a)[1][1]*(*v)[1][1] + (*a)[1][2]*(*v)[1][2] + (*a)[1][3]*(*v)[1][3];
  RR[2] = (*a)[1][1]*(*v)[2][1] + (*a)[1][2]*(*v)[2][2] + (*a)[1][3]*(*v)[2][3];
  RR[3] = (*a)[1][1]*(*v)[3][1] + (*a)[1][2]*(*v)[3][2] + (*a)[1][3]*(*v)[3][3];

  RR[4] = (*a)[2][1]*(*v)[1][1] + (*a)[2][2]*(*v)[1][2] + (*a)[2][3]*(*v)[1][3];
  RR[5] = (*a)[2][1]*(*v)[2][1] + (*a)[2][2]*(*v)[2][2] + (*a)[2][3]*(*v)[2][3];
  RR[6] = (*a)[2][1]*(*v)[3][1] + (*a)[2][2]*(*v)[3][2] + (*a)[2][3]*(*v)[3][3];

  RR[7] = (*a)[3][1]*(*v)[1][1] + (*a)[3][2]*(*v)[1][2] + (*a)[3][3]*(*v)[1][3];
  RR[8] = (*a)[3][1]*(*v)[2][1] + (*a)[3][2]*(*v)[2][2] + (*a)[3][3]*(*v)[2][3];
  RR[9] = (*a)[3][1]*(*v)[3][1] + (*a)[3][2]*(*v)[3][2] + (*a)[3][3]*(*v)[3][3];

  free_vector(w,1,3);
  free_matrix(v,1,3,1,3);
  free_matrix(a,1,3,1,3);
//print_vector(RR,1,12);

  return;
}



FLOAT getcamera3( int np, PNTLST p, PNTLST rp, VECTOR RR, int fs )
{

  //  np = number of points in p and rpp lista
  //   p = list of photo points (rays)
  // rpp = list of 3d points 
  //  RR = initial estimate of transformation needed to take points in rpp list
  //       to photo system such that they will be intersected by rays in p list.


  // this routine uses a least squares fit to determine the camera position and 
  // for a given photo when provided with pairs of photo points and measured points.
  // At least three sets of matched points must be provided. 
  // 
  // An initial approx. transform RR must be provided to indicate proper quadrant info.
  // On return this matrix will be updated with the final results.

  //int np = 3;
  int flag,k,i;
  int nu = 11;    // 12 unknowns - camera position matrix
                       // + 3 per point x,y,z;

  int ne = 3*np;// 9 equations for matrix orth., 5 per point for vector difference

                     // xu = (a1*x + a2*y + a3*z + a10)
                     // yu = (a4*x + a5*y + a6*z + a11)
                     // zu = (a7*x + a8*y + a9*z + a12)

                     // px = fl*(xu/zu)
                     // py = fl*(yu/zu)

                    // px = fl*(a1*x + a2*y + a3*z + a10)/(a7*x + a8*y + a9*z + a12)
                    // py = fl*(a4*x + a5*y + a6*z + a11)/(a7*x + a8*y + a9*z + a12)

  FLOAT fl = -65.124;
//  FLOAT fl = -65.4676;
  FLOAT sum,max;
  VECTOR L = vector(1,ne);
//printf("enter getcamera_3 - %d\n",memcount(0));
if(fs)
{
  Output(0,"\nGET CAMERA LOCATION AND ORENTATION\n");
  Output(0,"\n   Number of Points = %d\n\n",np);
}
  PNTLST A = matrix(1,ne,1,nu);

  VECTOR R = vector(1,12+3*np);
  copy_vector(R,1,12,RR,1,12);
      VECTOR sig;

      VECTOR a;
      VECTOR w;

      PNTLST u;
      PNTLST v;

      FLOAT chisq;

if(np >= 6) // closed form solution for initial conditions
{

      sig=vector(1,ne);

      a=vector(1,nu);
      w=vector(1,nu);

      u=matrix(1,ne,1,nu);
      v=matrix(1,nu,1,nu);



  for(k=1;k<=ne;k++)sig[k] = 1.0;
  for(k=1;k<=1;k++)
  {

    int e = 1;
    
    for(int i = 1; i<= np; i++)
    {

      //VECTOR tp = matpnt(RR, (*rp)[i] );   // target point in photo system
    
      //FLOAT xr = tp[1];
     // FLOAT yr = tp[2];
     // FLOAT zr = tp[3];

       FLOAT xr = (*rp)[i][1];
       FLOAT yr = (*rp)[i][2];
       FLOAT zr = (*rp)[i][3];

      FLOAT xp = (*p)[i][1];
      FLOAT yp = (*p)[i][2];
      FLOAT zp = (*p)[i][3];



    L[e  ] = yp;
    L[e+1] = xp;
    L[e+2] = 0.0;


    (*A)[e  ][4] = xr*zp;
    (*A)[e  ][5] = yr*zp;
    (*A)[e  ][6] = zr*zp;
    (*A)[e  ][11] = zp;

    (*A)[e  ][7] = -xr*yp;
    (*A)[e  ][8] = -yr*yp;
    (*A)[e  ][9] = -zr*yp;


    (*A)[e+1][1] = xr*zp;
    (*A)[e+1][2] = yr*zp;
    (*A)[e+1][3] = zr*zp;
    (*A)[e+1][10] = zp;

    (*A)[e+1][7] = -xr*xp;
    (*A)[e+1][8] = -yr*xp;
    (*A)[e+1][9] = -zr*xp;

    (*A)[e+2][1] = xr*yp;
    (*A)[e+2][2] = yr*yp;
    (*A)[e+2][3] = zr*yp;
    (*A)[e+2][10] = yp;

    (*A)[e+2][4] = -xr*xp;
    (*A)[e+2][5] = -yr*xp;
    (*A)[e+2][6] = -zr*xp;
    (*A)[e+2][11] = -xp;


    e += 3;
    }


//printf("vector L - getcamera\n");
//print_vector(L,1,ne);

//printf("matrix A\n");
//print_matrix(A,1,ne,1,nu);


//getchar();

     



// printf("vector R\n");
// print_vector(R,1,nu);
//printf("+++++++++++++ ok =  ++++++++++++++\n");
//        printf("xxxxxx calling svdfit xxxxxxxx\n");
        svdfit(A,L,sig,ne,a,nu,u,v,w,&chisq,NULL);
/**
  PNTLST cvm = matrix(1,nu,1,nu);

  svdvar(v,nu,w,cvm);

 // printf("covar matrix");
//  print_matrix(cvm,1,nu,1,nu);

  free_matrix(cvm,1,nu,1,nu);
**/
   //VECTOR D=lsq(A,L,nu,ne);

Output(4,"returned chisq = %e\n",chisq);

       for(i=1;i<=nu;i++)R[i] =a[i];
//printf("vector a - getcamera,%d\n",k);
//print_vector(R,1,nu);

     VECTOR R2 = vector(1,nu);
     for(i=1;i<=nu;i++)R2[i] = R[i]*R[i];

     VECTOR cr = vector(1,6);
     cr[1] = R2[1] + R2[2] + R2[3];
//printf("test a12 = %e\n",sqrt(1.0/cr[1]));
     cr[2] = R2[4] + R2[5] + R2[6];
//printf("test a12 = %e\n",sqrt(1.0/cr[2]));
     cr[3] = R2[7] + R2[8] + R2[9];
//printf("test a12 = %e\n",sqrt(1.0/cr[3]));

     cr[4] = R2[1] + R2[4] + R2[7];
//printf("test a12 = %e\n",sqrt(1.0/cr[4]));
     cr[5] = R2[2] + R2[5] + R2[8];
//printf("test a12 = %e\n",sqrt(1.0/cr[5]));
     cr[6] = R2[3] + R2[6] + R2[9];
//printf("test a12 = %e\n",sqrt(1.0/cr[6]));

     FLOAT K,K2;
     K=K2=0.0;
     for(i=1;i<=6;i++)
     {
        K += cr[i];
        K2 += cr[i]*cr[i];
     }

     R[12] = sqrt(K/K2);
     if(R[5] < 0.0)R[12] = -R[12];

    for(i=1;i<=11;i++)R[i] *= R[12];

   // printf("++++ results R scaled +++++++");
   //enter square matrix print_vector(R,1,12);
     
  }




  free_vector(L,1,ne);
  free_matrix(A,1,ne,1,nu);


      free_vector(sig,1,ne);
      free_vector(a,1,nu);
      free_vector(w,1,nu);

      free_matrix(u,1,ne,1,nu);
      free_matrix(v,1,nu,1,nu);


  square_matrix( R);

   //printf("vector a - getcamera,%d\n",k);
   //print_vector(R,1,nu);


  } 


   ne = 9+5*np;
   nu = 12+3*np;

   L = vector(1,ne);
//printf("enter getcamera_3 - %d\n",memcount(0));

   A = matrix(1,ne,1,nu);

  // R = vector(1,nu+1);

   sig=vector(1,ne);

   a=vector(1,nu);
   VECTOR oa=vector(1,nu);
   FLOAT lamda = 1.0;
   w=vector(1,nu);

   u=matrix(1,ne,1,nu);
   v=matrix(1,nu,1,nu);

   FLOAT sum1 = 0.0;
   FLOAT sum2 = 0.0;
   FLOAT max1 = 0.0;
   FLOAT max2 = 0.0;
   int   mp1 = 0;
   int   mp2 = 0;

   FLOAT oldchisq = 0.0;
   flag = 0;
  int mode = 0;
  VECTOR P = vector(1,nu);
  PNTLST cur = matrix(1,nu,1,nu);

  FLOAT osig = 10.0;
  if(fs) osig = 0.001;
  FLOAT osigs = 0.1;
  if(fs)osigs = 0.001;


  for(k=1;k<=60;k++)
  {

    forcesquare( L, R, *A,1.0);
    for(i=1;i<=9;i++) sig[i] = .1;

    sig[1] = sig[2] = sig[3] = osig;
    sig[4] = sig[5] = sig[6] = osig;

    int e = 9;
    int ti = 12;

   sum1 = 0.0;
   sum2 = 0.0;
   max1 = 0.0;
   max2 = 0.0;

   // if(R[5] < 0.0)for(int ii=1; ii<=12;ii++)R[ii] = -R[ii];


    for(int i = 1; i<= np; i++)
    {

      if(k==1)
      {
        FLOAT tmpx = R[1]*(*rp)[i][1] + R[2]*(*rp)[i][2] + R[3]*(*rp)[i][3] + R[10];
        FLOAT tmpy = R[4]*(*rp)[i][1] + R[5]*(*rp)[i][2] + R[6]*(*rp)[i][3] + R[11];
        FLOAT tmpz = R[7]*(*rp)[i][1] + R[8]*(*rp)[i][2] + R[9]*(*rp)[i][3] + R[12];
//printf("tempz = %e\n",tmpz);
        FLOAT lng  = tmpx*(*p)[i][1] + tmpy*(*p)[i][2] + tmpz*(*p)[i][3];

        R[ti+1] = 0.5*(tmpx + lng*(*p)[i][1]);
        R[ti+2] = 0.5*(tmpy + lng*(*p)[i][2]);
        R[ti+3] = 0.5*(tmpz + lng*(*p)[i][3]);
      }

      sig[e+1] = sig[e+2] = sig[e+3] = 1.0/3.0;

      L[e+1] = R[ti+1] - R[1]*(*rp)[i][1] - R[2]*(*rp)[i][2] - R[3]*(*rp)[i][3] - R[10];
      L[e+2] = R[ti+2] - R[4]*(*rp)[i][1] - R[5]*(*rp)[i][2] - R[6]*(*rp)[i][3] - R[11];
      L[e+3] = R[ti+3] - R[7]*(*rp)[i][1] - R[8]*(*rp)[i][2] - R[9]*(*rp)[i][3] - R[12];;
   
      FLOAT dd = sqrt(L[e+1]*L[e+1] + L[e+2]*L[e+2] + L[e+3]*L[e+3]);
      sum1 += dd;
      if( dd > max1)
      {
        max1 = dd;
        mp1 = i;
      }

      (*A)[e+1][1] = (*rp)[i][1];
      (*A)[e+1][2] = (*rp)[i][2];
      (*A)[e+1][3] = (*rp)[i][3];

      (*A)[e+1][ti+1] = -1.0;
      (*A)[e+1][10] = 1.0;


      (*A)[e+2][4] = (*rp)[i][1];
      (*A)[e+2][5] = (*rp)[i][2];
      (*A)[e+2][6] = (*rp)[i][3];

      (*A)[e+2][ti+2] = -1.0;
      (*A)[e+2][11] = 1.0;


      (*A)[e+3][7] = (*rp)[i][1];
      (*A)[e+3][8] = (*rp)[i][2];
      (*A)[e+3][9] = (*rp)[i][3];

      (*A)[e+3][ti+3] = -1;
      (*A)[e+3][12] = 1.0;


      sig[e+4] = sig[e+5] = 0.0127/3.0;

      L[e+4] = fl*((*p)[i][1]/(*p)[i][3]) - fl*(R[ti+1]/R[ti+3]);
      L[e+5] = fl*((*p)[i][2]/(*p)[i][3]) - fl*(R[ti+2]/R[ti+3]);

       dd = sqrt(L[e+4]*L[e+4] + L[e+5]*L[e+5]);
      sum2 += dd;
      if( dd > max2)
      {
        max2 = dd;
        mp2 = i;
      }

      (*A)[e+4][ti+1] = fl/R[ti+3];
      (*A)[e+4][ti+3] = -fl*(R[ti+1]/(R[ti+3]*R[ti+3]));

      (*A)[e+5][ti+2] = fl/R[ti+3];
      (*A)[e+5][ti+3] = -fl*(R[ti+2]/(R[ti+3]*R[ti+3]));


      e += 5;
      ti += 3;
    }



    
    //chisq = 0.0;
   // for(int ii=1;ii<=ne;ii++)chisq += L[ii]*L[ii]/(sig[ii]*sig[ii]);

   // printf("my chisq = %e\n\n",chisq);

    // if(mode == 0)  if(fabs(oldchisq - chisq)<= chisq*1.0e-6)break;



//printf("vector L - getcamera\n");
//print_vector(L,1,ne);

//printf("matrix A");
//print_matrix(A,1,ne,1,nu);

//printf("+++++++++++++ okkkk =  ++++++++++++++\n");
//        printf("xxxxxx calling svdfit xxxxxxxx\n");

      
        svdfit(A,L,sig,ne,a,nu,u,v,w,&chisq,NULL);
//printf("vector w");
//print_vector(w,1,nu);
Output(3,"xxx chisq = %e\n",chisq);

    if(fabs(oldchisq - chisq)<= chisq*1.0e-10)
    {
      flag += 1;
    }
    else
    {
      flag=0;
    }

    if(flag == 3)if( lamda > 0.9)break;

    if(chisq < 1.0e-25)
    {
       mode += 1;
    }
    else
    {
       mode = 0;
    }
    
    if(mode ==3)if( lamda > 0.9)break;



    for(i=1;i<=nu;i++)R[i] += a[i];



    oldchisq = chisq;


  }












  copy_vector(RR,1,12,R,1,12);
  //  printf("vector R");
  //  print_vector(R,1,nu);

 // printf(" results getcamera \n");
//  print_vector(RR,1,12);


//******** square matrix **********
  int pm = 1;
  if(R[15] < 0)pm=-1;
  for(i = 12+3;i<=nu;i+=3)
  {
    int tpm = 1;
    if(R[i] < 0)tpm = -1;
    if(tpm != pm)
    {
      pm = 0;
//printf("%%%%%%%% diff z val %%%%%%\n");
    }
  }

  free_vector(R,1,12);

   
if(pm == 1)
{
  //printf("*************** Z values not negative *************\n");
  for(i=1;i<=12;i++)RR[i] = -RR[i];
}

if(RR[5] <= 1.0/sqrt(2))chisq = 1.0e5;

FLOAT zx = RR[2]*RR[6] - RR[3]*RR[5];
FLOAT zy = RR[3]*RR[4] - RR[1]*RR[6];
FLOAT zz = RR[1]*RR[5] - RR[2]*RR[4];

 FLOAT iv = sqrt( (RR[7]-zx)*(RR[7]-zx) + (RR[8]-zy)*(RR[8]-zy) + (RR[9]-zz)*(RR[9]-zz) );

//printf(" RH value = %e\n",iv);

if(iv > 0.5)chisq = 1.0e5;

if(fs)
{
 display_tm(RR);
 Output(0,"\n    ERROR SUMMARY - 3d point\n");
 Output(0," average error = %e, max error = %e (for point %d)\n\n",sum1/(np),max1,mp1);
 Output(0,"    ERROR SUMMARY - 2d point on film plane\n");
 Output(0," average error = %e, max error = %e (for point %d)\n\n",sum2/(np),max2,mp1);
}
//printf("exit getcamera - %d\n",memcount(0));



  return(chisq);

}







void ffor(FLOAT val)
{

  int i=int(val);
  int j= int(((val-((FLOAT)i))*1.0e6)+0.5);
  printf("%6d.%06d",i,j);
  return;
}


int bundlecheck(BundleData &bd)
{
  PNTLST tm = bd.plTMatrix;
  PNTLST cpx = bd.plCalcPnts;
  PNTLST *fp = bd.plPixels;
  PNTLST sp = bd.plSurvPnts;
  int    nf = bd.nNumPhotos;
  int    np = bd.nNumPoints;

   FLOAT dx,dy,x,y;      
   FLOAT var = 0;
   FLOAT n = 0;
   FLOAT svar = 0;
   FLOAT sn = 0;

   PNT raypnt;
   {
    for(int i=0;i<nf;i++)
    {
      if( (*tm)[i] == NULL)
      {
        printf("\n ####    CAN NOT RUN CHECK BUNDLE     ####\n");
        printf(  "  ONE OR MORE PHOTOS HAVE A NULL T-MATRIX\n");
        return(0);
      }
    }
   }


  if(nf ==1)                          // if single photo bundle
  {
    //printf("\n Photo - %s  \n",bd.pszPhotoNames[0]);
    printf("\n FILM PLANE ERROR - delta between image point and projection of target onto image\n\n");
    for(int i =0;i < np;i++)          // for each point
    {
      if( (*(fp[0]))[i] != NULL)        // if pixel point provided
      {
        if( (*sp)[i] != NULL )      // if target point provided
        {
          pnt2film_error(bd.vfl[0],(*tm)[0],(*sp)[i],(*(fp[0]))[i],&dx,&dy);
          var += dx*dx + dy*dy;
          n++;
          printf(" point - %-15.15s  ",bd.pszPointNames[i]);
         // printf("%#15.6g     \n",sqrt(dx*dx + dy*dy));
          ffor(sqrt(dx*dx + dy*dy));
          printf("\n");
        }
      }
    }
printf("\n");

    var = var/n;
    printf("\n   variance =");
    ffor(var); 
    printf("       RMS error = ");
    ffor(sqrt(var));
    printf("    samples = %d\n",int(n) );

    return(0);
  }

  FLOAT maxvar = 0.0;
  FLOAT maxsvar = 0.0;
  int maxi,maxj,maxsi,maxsj;

  for(int i =0;i < np;i++)          // for each point
  {
    if( (*cpx)[i] != NULL)                // if a calculated point provided
    {
      for(int j=0;j<nf;j++)               // for each photo
      {
        if( (*(fp[j]))[i] != NULL)        // if pixel point provided
        {
          pnt2film_error(bd.vfl[j],(*tm)[j],(*cpx)[i],(*(fp[j]))[i],&dx,&dy);
        
          FLOAT tvar = dx*dx + dy*dy;
          var += tvar;
          n++;
          if(tvar > maxvar)
          {
            maxvar = tvar;
            maxi = i;
            maxj = j;
          }
        }
      }
         
      if( (*sp)[i] != NULL)
      {
        FLOAT dx,dy,dz;

        dx = (*sp)[i][1] - (*cpx)[i][1];
        dy = (*sp)[i][2] - (*cpx)[i][2];
        dz = (*sp)[i][3] - (*cpx)[i][3];


        FLOAT tsvar = dx*dx+dy*dy+dz*dz;
        svar += tsvar;
        sn++;
          if(tsvar > maxsvar)
          {
            maxsvar = tsvar;
            maxsi = i;
           // maxsj = j;
          }
      }
    }
  }


  var = var/n;
  if(int(sn) > 0.0)svar = svar/sn;
  


printf("\n\n   BUNDLE - %s\n",bd.pszBundleName);
printf("\n FILM PLANE ERROR - delta between image point and projection of estimated point onto image\n\n");

printf("   variance = ");
ffor(var);
printf("       RMS error = ");
ffor(sqrt(var));
printf("    samples = %d\n",int(n) );


printf("\n   maximum RMS error =");
ffor(sqrt(maxvar));
printf("   for point %-15.15s in photo %-15.15s\n",bd.pszPointNames[maxi],bd.pszPhotoNames[maxj]);

printf("\n 3D ERROR - delta between target point and  estimated point \n\n");
if(int(sn) > 0.0)
{
//printf("   variance = %g       RMS error = %g    samples = %d\n",svar,sqrt(svar),int(sn) );
printf("   variance = ");
ffor(svar);
printf("       RMS error = ");
ffor(sqrt(svar));
printf("    samples = %d\n",int(sn) );

printf("\n   maximum RMS error =");
ffor(sqrt(maxsvar));
printf("   for point %-15.15s\n",bd.pszPointNames[maxsi]);
}
else
{
  printf(" NO TARGET POINTS\n\n");
}
printf("\n     ********** 2D ERRORS grouped by points ********** \n\n");
printf("                              EP vs PP       RP vs PP       SP vs PP\n");
  for( i =0;i < np;i++)          // for each point
  {

    FLOAT avgc = 0.0;
    FLOAT avgr = 0.0;

    FLOAT navgc = 0;
    FLOAT navgr = 0;

    int npp = 0;
    for(int j=0;j< nf;j++)
    { 
      if( (*(fp[j]))[i] != NULL)npp++;
    }

    if(npp>1)
    {
      PNTLST pnts = matrix_shell(1,npp,1,3);
      PNTLST tmtxs = matrix_shell(1,np,1,12);

      int k = 0;
      for(j=0;j<nf;j++)
      {
        if( (*(fp[j]))[i] != NULL)
        {
          k++;
          (*pnts)[k] = (*(fp[j]))[i];
          (*tmtxs)[k] = (*tm)[j];
        }
      }
          //PNT get3dpnt_np(int nf,PNTLST pnts, FLOAT **tmxs )          
      raypnt = get3dpnt_np(npp,pnts,*tmtxs);
      free_matrix_shell(pnts);
      free_matrix_shell(tmtxs);
    }


    if( (*cpx)[i] != NULL)                // if a calculated point provided
    {

      printf("\n point - %s\n",bd.pszPointNames[i]);
      for(int j=0;j<nf;j++)               // for each photo
      {
        if( (*(fp[j]))[i] != NULL)        // if pixel point provided
        {

          printf("   Photo - %-15.15s",bd.  pszPhotoNames[j]);
          pnt2film_error(bd.vfl[j],(*tm)[j],(*cpx)[i],(*(fp[j]))[i],&dx,&dy);
          //printf("%#15.6g      ",sqrt(dx*dx + dy*dy));
          ffor(sqrt(dx*dx + dy*dy));
          printf("  ");
          avgc += dx*dx + dy*dy;
          navgc++;

          if( npp>1)
          {
            pnt2film_error(bd.vfl[j],(*tm)[j],raypnt,(*(fp[j]))[i],&dx,&dy);
            //printf(" %#15.12g      ",sqrt(dx*dx + dy*dy));
            ffor(sqrt(dx*dx + dy*dy));
            printf("  ");
            avgr += dx*dx + dy*dy;
            navgr++;

          }
          else
          {
            printf("               ");
          }

          if( (*sp)[i] != NULL )
          {
            //pnt2film(bd.vfl[j],(*tm)[j],(*cpx)[i],&x,&y);
            //pnt2film(bd.vfl[j],(*tm)[j],(*sp)[i],&dx,&dy);
            //dx = x-dx;
            //dy = y-dy;
            //printf("  %#15.12g",sqrt(dx*dx + dy*dy));
            pnt2film_error(bd.vfl[j],(*tm)[j],(*sp)[i],(*(fp[j]))[i],&dx,&dy);
            ffor(sqrt(dx*dx + dy*dy));
          }
          printf("\n");
        }
      }
      if(navgc > 1.0)
      {
        printf("\n                          ");
        ffor(sqrt(avgc/navgc));

        printf("  ");
        ffor(sqrt(avgr/navgr));
        printf("\n");
      }
    }
   

  }

printf("\n\n     ********** 2D ERRORS grouped by photos ********** \n\n");
printf("                                 EP vs PP            EP vs SP\n");

  for(int j =0;j < nf;j++)          // for each photo
  {
    printf("\n Photo - %s  \n",bd.pszPhotoNames[j]);
    for(int i=0;i<np;i++)               // for each photo
    {
      if( (*(fp[j]))[i] != NULL)        // if pixel point provided
      {
printf("   point - %-15.15s",bd.pszPointNames[i]);
        if( (*cpx)[i] != NULL)                // if a calculated point provided
        {

          pnt2film_error(bd.vfl[j],(*tm)[j],(*cpx)[i],(*(fp[j]))[i],&dx,&dy);
          printf("   ");
          ffor(sqrt(dx*dx + dy*dy));
          printf("      ");

          if( (*sp)[i] != NULL )
          {
            pnt2film(bd.vfl[j],(*tm)[j],(*cpx)[i],&x,&y);
            pnt2film(bd.vfl[j],(*tm)[j],(*sp)[i],&dx,&dy);
            dx = x-dx;
            dy = y-dy;
            printf(" ");
            ffor(sqrt(dx*dx + dy*dy));
            //%#15.6g,%#15.6g  %#15.6g",dx,dy,sqrt(dx*dx + dy*dy));
          }
        }
        printf("\n");
      }
    }
  }


printf("\nDEFINITIONS:\n\n");
printf("  E  - best estimate of a points location resulting from the bundling process    (3D)\n");
printf("  EP - the E point projected onto the image plane of a photo in which it appears (2D)\n\n");
printf("  S  - surveyed point from field measurements                                    (3D)\n");
printf("  SP - surveyed point projected onto the image plane of photo                    (2D)\n\n");
printf("  PP - pixel point or user selected point on a photograph (defines a ray)        (2D)\n\n");
printf("  R  - a (ray) point calculated from two or more PP points                       (3D)\n");
printf("  RP - a ray point projected onto the image plane of a photo                     (2D)\n\n");


  return(0);
}

void newerBB(BundleData &bd)
{

  PNTLST tm = bd.plTMatrix;
  PNTLST cpx = bd.plCalcPnts;
  PNTLST *fp = bd.plPixels;
  PNTLST sp = bd.plSurvPnts;
  int    nf = bd.nNumPhotos;
  int    np = bd.nNumPoints;

   FLOAT fl = -65.124;
 //  FLOAT fl = -65.4676;

  FLOAT chisq;
  FLOAT oldchisq = 0.0;  
  VECTOR pntsum   = vector(1,np); // will contain count of photos each point is in
  VECTOR photosum = vector(1,nf); // will contain count of usuable points for each photo


  // for each point get count of the number of photos in which it appears

  for(int inp =0;inp < np;inp++)          // for each point
  {
    pntsum[inp+1] = 0.0;                  // initialize number of fotos this pnt in to 0
    for(int inf=0;inf<nf;inf++)           // for each photo
    {
      if(  (*(fp[inf]))[inp] != NULL )    // if point defined for this photo
      {
        pntsum[inp+1] += 1.0;             // increment count of photo this point is in
      }
    }
  }




  int nup = 0;  // number of usable photo points
  int nut = 0;  // number of usable target points
  int nuf = 0;  // number of usable fotos
  int nxyz = 0; // number of 3d points;

  // determine the number of usable points and number of usable target points

  for(inp=1;inp<=np;inp++)       // for each pnt 
  {
    if( (*sp)[inp-1] == NULL)    //if not target point
    {
      if( pntsum[inp] < 2.0)     //if less than two photo points
      {
        pntsum[inp]=0.0;         //this point not usable
      }
      else                       // point is in two or more photos
      {
        nup += pntsum[inp];      // increment count of usable foto points for this point
        nxyz++;
      }
    }
    else
    {
      if( pntsum[inp] > 0)       // pnt is a target point so it can be used if it is in at
      {                          //     least one photo

        nup += pntsum[inp];        // increment count of usable foto points for this point
        nut += 1;                  // increment count of usable target points
        nxyz++;
      }
    }
  } 
      


  // determine the number of usable photos

  for( int inf = 1;inf <= nf;inf++)        // for each photo
  {

    int psum = 0;
    for(inp=1;inp<=np;inp++)                 // for each point 
    {
      if( (*(fp[inf-1]))[inp-1] != NULL )      // if point in this photo
      {
        if( pntsum[inp] > 0)psum += 1;           // if point is a usable point, increment count
      }
    }
    
    photosum[inf] = psum;                  
    if(psum > 2)
    {
      nuf += 1;
    }
    else
    {
      Output(0,"******** photo %d (base 1) not usable, only %d usable common/target points ********\n",inf,psum);
      return;
    }
  }

  int nu = 12*nuf + 3*nxyz;            // number of unknowns, 12 transform parameters per photo
                                       //                      3 coordinates per point
   
  int ne =  12*nuf + 2*nup + 3*nut;    // number of equations 12 per foto for transform constraints
                                       //                      2 per point for film plane constraints
                                       //                      3 per target point for miss distance

printf(" nuf = %d, nup = %d, nut = %d\n",nuf,nup,nut);

/**
printf("vector pntsum");
print_vector(pntsum,1,np);
printf("vector photosum");
print_vector(photosum,1,nf);
**/


  // allocate required matrices

    VECTOR R = vector(1,nu);
    VECTOR L = vector(1,ne);

    VECTOR sig = vector(1,ne);

    VECTOR a=vector(1,nu);
    VECTOR w=vector(1,nu);

   // PNTLST A = matrix(1,ne,1,nu);

   // PNTLST u=matrix(1,ne,1,nu);
   // PNTLST v=matrix(1,nu,1,nu);









  // initialize R with photo transforms

  int ti = 1;
  for(inf=1;inf<=nf;inf++)
  {
    if( photosum[inf] > 0)
    {
      for(int i=1;i<=12;i++) R[ti++] = (*tm)[inf-1][i];
    }
  }


  // initialize R with calculated 3d points

  for(inp=1;inp<=np;inp++)  // for each point
  {
    if(pntsum[inp] > 0) // if pnt is usable
    {
      if( (*cpx)[inp-1] != NULL ) // if calculated point point provided for this photo
      {
        for(int i=1;i<=3;i++) R[ti++] = (*cpx)[inp-1][i];
        //printf("initialized with calculated point\n");
      }
      else
      {
        if( (*sp)[inp-1] != NULL ) // if calculated point point provided for this photo
        {
          for(int i=1;i<=3;i++) R[ti++] = (*sp)[inp-1][i];
         // printf("initialized with target point\n");
        }
        else
        {

          printf("@@@@@@@@ error usable photo pnt with no calculated point or target point @@@@@@@@@\n");
        }
      }
    }
  }


  VECTOR AWT = vector(1,nu);
  PNTLST Q = matrix(1,12*nf,1,nu-12*nf);
  PNTLST S = matrix(1,nu-12*nf,1,nu-12*nf);
  PNTLST P = matrix(1,12*nf,1,12/**nf*/);
  int bflag = 0;




  // MAIN LOOP
  int sflag =1.0;
  FLOAT pchisq = 0.0;
  FLOAT old_pchisq = 0.0;
  FLOAT CC = 1.0;
  FLOAT C  = 1.0;;
  for(int k=1;k<=40;k++)
  {
    
   // PNTLST ATA = matrix(1,nu,1,nu);
    clear_matrix(Q,1,12*nf,1,nu-12*nf);
    clear_matrix(S,1,nu-12*nf,1,nu-12*nf);
    clear_matrix(P,1,12*nf,1,12/**nf*/);
    clear_vector(AWT,1,nu);
    // 
    int e   = 1;
    int tmi = 1;
    for(inf=1;inf<=nf;inf++)
    {
      if( photosum[inf] > 0)  // if this photo is usable
      {
        for(int i=0;i<12;i++)sig[e+i] = (1.0/(nup+3*nut));
        //sig[e+9]=sig[e+10]=(1.0/(nup+3*nut));
       // sig[e+11]=(0.1/(nup+3*nut));
        FLOAT w = nup+3*nut;
              w *=w;
        
        //PNTLST AA = submatrix( A,e,e+11,tmi,tmi+11,1,1);
        //PNTLST AA = submatrix( A,e,e+11,tmi,tmi+11,1,1);

        PNTLST AA = matrix(1,12,1,12);
         
        forceortho( &(L[e-1]),&(R[tmi-1]), AA );


        for(int ii = 1;ii<=12;ii++)
        {
          for(int jj=1; jj<=12; jj++)
          {
             AWT[tmi+ii-1] += (*AA)[jj][ii]* L[e+jj-1]*(1.0/(sig[e+jj-1]*sig[e+jj-1]));
          }
        }





        for( ii = 1; ii <= 12; ii++)
        {
          for(int jj=1; jj <= 12; jj++)          
          {
            
            for(int kk=1; kk <= 12; kk++)
            {
              FLOAT acm = (*AA)[kk][ii]*(*AA)[kk][jj];
              //(*ATA)[ii][jj] += acm;
              (*P)[ii+tmi-1][jj]   += acm;
            }
          
            //(*ATA)[ii][jj] *= w;
            (*P)[ii+tmi-1][jj]   *= w;    
          }
        }

        free_matrix(AA,1,12,1,12);
        e += 12;
        tmi += 12;
      }
    }


    tmi  = 0;
    e = e-1;

//FLOAT awx = 0.0;
//FLOAT awy = 0.0;
//int   wnp = 0;

    for(inf=1;inf<=nf;inf++) // look at each photo
    {


//FLOAT zl = R[tmi+7]*R[tmi+7] + R[tmi+8]*R[tmi+8] + R[tmi+9]*R[tmi+9];
//zl = sqrt(zl);
//FLOAT wx = (R[tmi+1]*R[tmi+7] + R[tmi+2]*R[tmi+8] + R[tmi+3]*R[tmi+9])/zl;
//FLOAT wy = (R[tmi+4]*R[tmi+7] + R[tmi+5]*R[tmi+8] + R[tmi+6]*R[tmi+9])/zl;
//R[tmi+7] /= zl;
//R[tmi+8] /= zl;
//R[tmi+9] /= zl;


//printf("%d zl = %e,    wx = %e,  wy = %e,  np = %e\n",inf,zl,wx,wy,photosum[inf]);

//      awx += wx * photosum[inf];
//      awy += wy * photosum[inf];
//      wnp += photosum[inf];



      int pnti = nuf*12;
      int Qpnti = 0;
      int Spnti = 0;
      if( photosum[inf] > 0)  // if this photo is usable
      {
        for(inp=1;inp<=np;inp++)  // look at each pnt in photo
        {
          if(pntsum[inp] > 0) // if pnt is usable
          {
            if( (*(fp[inf-1]))[inp-1] != NULL ) // if pnt defined for this photo
            {
               FLOAT px = (*(fp[inf-1]))[inp-1][1];
               FLOAT py = (*(fp[inf-1]))[inp-1][2];
               FLOAT pz = (*(fp[inf-1]))[inp-1][3];
                 
               FLOAT xr = R[pnti+1];  
               FLOAT yr = R[pnti+2];
               FLOAT zr = R[pnti+3];
 
               FLOAT x = xr*R[tmi+1] + yr*R[tmi+2] + zr*R[tmi+3] + R[tmi+10];
               FLOAT y = xr*R[tmi+4] + yr*R[tmi+5] + zr*R[tmi+6] + R[tmi+11];
               FLOAT z = xr*R[tmi+7] + yr*R[tmi+8] + zr*R[tmi+9] + R[tmi+12];

              sig[e+1] = sig[e+2] = 0.0127/2.0;
              FLOAT w = 1.0/sig[e+1];
                    w *= w;              
//printf("px,py, %e,%e   x,y %e,%e\n",px,py,x,y);
              L[e+1] = fl*(px/pz) - fl*(x/z);
              L[e+2] = fl*(py/pz) - fl*(y/z);

 
              FLOAT t1 = /* (*A)[e+1][pnti+1] = */ (fl*R[tmi+1]/z) - fl*x*R[tmi+7]/(z*z);
                 AWT[pnti+1] += t1*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
              FLOAT acm = t1*t1*w;
                 //(*ATA)[pnti+1][pnti+1] += acm;
                 (*S)[Spnti+1][Spnti+1] += acm;

              FLOAT t2 = /* (*A)[e+1][pnti+2] =*/ (fl*R[tmi+2]/z) - fl*x*R[tmi+8]/(z*z);
                 AWT[pnti+2] += t2*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t2*w;
                 //(*ATA)[pnti+2][pnti+1] += acm;
                 (*S)[Spnti+2][Spnti+1] += acm;
                 //(*ATA)[pnti+1][pnti+2] += acm;
                 (*S)[Spnti+1][Spnti+2] += acm;
                 acm = t2*t2*w;
                 //(*ATA)[pnti+2][pnti+2] += acm;
                 (*S)[Spnti+2][Spnti+2] += acm;

              FLOAT t3 = /* (*A)[e+1][pnti+3] = */ (fl*R[tmi+3]/z) - fl*x*R[tmi+9]/(z*z);
                 AWT[pnti+3] += t3*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t3*w;
                 //(*ATA)[pnti+3][pnti+1] += acm;
                 (*S)[Spnti+3][Spnti+1] += acm;
                 //(*ATA)[pnti+1][pnti+3] += acm;
                 (*S)[Spnti+1][Spnti+3] += acm;
                 acm = t2*t3*w;
                 //(*ATA)[pnti+3][pnti+2] += acm;
                 (*S)[Spnti+3][Spnti+2] += acm;
                // (*ATA)[pnti+2][pnti+3] += acm;
                 (*S)[Spnti+2][Spnti+3] += acm;
                 acm = t3*t3*w;
                 //(*ATA)[pnti+3][pnti+3] += acm;
                 (*S)[Spnti+3][Spnti+3] += acm;

              FLOAT t4 = /* (*A)[e+1][tmi+1] =*/ (fl*xr/z);
                 AWT[tmi+1] += t4*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t4*w;
                 //(*ATA)[tmi+1][pnti+1] += acm;
                 (*Q)[tmi+1][Qpnti+1]  += acm;
                 //(*ATA)[pnti+1][tmi+1] += acm;
                 acm = t2*t4*w;
                 //(*ATA)[tmi+1][pnti+2] += acm;
                 (*Q)[tmi+1][Qpnti+2]  += acm;
                 //(*ATA)[pnti+2][tmi+1] += acm;
                 acm = t3*t4*w;
                 //(*ATA)[tmi+1][pnti+3] += acm;
                  (*Q)[tmi+1][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+1] += acm;
                 acm = t4*t4*w;
                // (*ATA)[tmi+1][tmi+1]  += acm;
                   (*P)[tmi+1][1]  += acm;

              FLOAT t5 = /* (*A)[e+1][tmi+2] =*/ (fl*yr/z);
                    AWT[tmi+2] += t5*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t5*w;
                 //(*ATA)[tmi+2][pnti+1] += acm;
                  (*Q)[tmi+2][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+2] += acm;
                 acm = t2*t5*w;
                 //(*ATA)[tmi+2][pnti+2] += acm;
                  (*Q)[tmi+2][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+2] += acm;
                 acm = t3*t5*w;
                //(*ATA)[tmi+2][pnti+3] += acm;
                  (*Q)[tmi+2][Qpnti+3] += acm;
                // (*ATA)[pnti+3][tmi+2] += acm;
                 acm = t4*t5*w;
                // (*ATA)[tmi+2][tmi+1]  += acm;
                   (*P)[tmi+2][1]  += acm;
                 //(*ATA)[tmi+1][tmi+2]  += acm;
                   (*P)[tmi+1][2]  += acm;
                 acm = t5*t5*w;
                 //(*ATA)[tmi+2][tmi+2]  += acm;
                   (*P)[tmi+2][2]  += acm;

              FLOAT t6 = /* (*A)[e+1][tmi+3] =*/ (fl*zr/z);
                    AWT[tmi+3] += t6*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t6*w;
                 //(*ATA)[tmi+3][pnti+1] += acm;
                  (*Q)[tmi+3][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+3] += acm;
                 acm = t2*t6*w;
                // (*ATA)[tmi+3][pnti+2] += acm;
                  (*Q)[tmi+3][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+3] += acm;
                 acm = t3*t6*w;
                // (*ATA)[tmi+3][pnti+3] += acm;
                  (*Q)[tmi+3][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+3] += acm;
                 acm = t4*t6*w;
                 //(*ATA)[tmi+3][tmi+1]  += acm;
                   (*P)[tmi+3][1]  += acm;
                 //(*ATA)[tmi+1][tmi+3] += acm;
                   (*P)[tmi+1][3] += acm;
                 acm = t5*t6*w;
                 //(*ATA)[tmi+3][tmi+2]  += acm;
                   (*P)[tmi+3][2]  += acm;
                 //(*ATA)[tmi+2][tmi+3] += acm;
                   (*P)[tmi+2][3] += acm;
                 acm = t6*t6*w;
                 //(*ATA)[tmi+3][tmi+3]  += acm;
                   (*P)[tmi+3][3]  += acm;

              FLOAT t7 = /* (*A)[e+1][tmi+10] =*/ (fl/z);
                    AWT[tmi+10] += t7*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t7*w;
                 //(*ATA)[tmi+10][pnti+1] += acm;
                  (*Q)[tmi+10][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+10] += acm;
                 acm = t2*t7*w;
                 //(*ATA)[tmi+10][pnti+2] += acm;
                  (*Q)[tmi+10][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+10] += acm;
                 acm = t3*t7*w;
                 //(*ATA)[tmi+10][pnti+3] += acm;
                  (*Q)[tmi+10][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+10] += acm;
                 acm = t4*t7*w;
                 //(*ATA)[tmi+10][tmi+1]  += acm;
                   (*P)[tmi+10][1]  += acm;
                 //(*ATA)[tmi+1][tmi+10]  += acm;
                   (*P)[tmi+1][10]  += acm;
                 acm = t5*t7*w;
                 //(*ATA)[tmi+10][tmi+2]  += acm;
                   (*P)[tmi+10][2]  += acm;
                 //(*ATA)[tmi+2][tmi+10]  += acm;
                 (*P)[tmi+2][10]  += acm;
                 acm = t6*t7*w;
                 //(*ATA)[tmi+10][tmi+3]  += acm;
                   (*P)[tmi+10][3]  += acm;
                 //(*ATA)[tmi+3][tmi+10]  += acm;
                   (*P)[tmi+3][10]  += acm;
                 acm = t7*t7*w;
                 //(*ATA)[tmi+10][tmi+10] += acm;
                   (*P)[tmi+10][10] += acm;

              FLOAT t8  = /* (*A)[e+1][tmi+7] = */  - fl*x*xr/(z*z);
                    AWT[tmi+7] += t8*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t8*w;
                 //(*ATA)[tmi+7][pnti+1] += acm;
                  (*Q)[tmi+7][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+7] += acm;
                 acm = t2*t8*w;
                 //(*ATA)[tmi+7][pnti+2] += acm;
                  (*Q)[tmi+7][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+7] += acm;
                 acm = t3*t8*w;
                 //(*ATA)[tmi+7][pnti+3] += acm;
                  (*Q)[tmi+7][Qpnti+3] += acm;
                // (*ATA)[pnti+3][tmi+7] += acm;
                 acm = t4*t8*w;
                 //(*ATA)[tmi+7][tmi+1]  += acm;
                   (*P)[tmi+7][1]  += acm;
                // (*ATA)[tmi+1][tmi+7]  += acm;
                   (*P)[tmi+1][7]  += acm;
                 acm = t5*t8*w;
                 //(*ATA)[tmi+7][tmi+2]  += acm;
                   (*P)[tmi+7][2]  += acm;
                 //(*ATA)[tmi+2][tmi+7]  += acm;
                   (*P)[tmi+2][7]  += acm;
                 acm = t6*t8*w;
                 //(*ATA)[tmi+7][tmi+3]  += acm;
                   (*P)[tmi+7][3]  += acm;
                 //(*ATA)[tmi+3][tmi+7]  += acm;
                   (*P)[tmi+3][7]  += acm;
                 acm = t7*t8*w;
                 //(*ATA)[tmi+7][tmi+10] += acm;
                   (*P)[tmi+7][10] += acm;
                 //(*ATA)[tmi+10][tmi+7]  += acm;
                   (*P)[tmi+10][7]  += acm;
                 acm = t8*t8*w;
                 //(*ATA)[tmi+7][tmi+7]  += acm;
                   (*P)[tmi+7][7]  += acm;

              FLOAT t9  = /* (*A)[e+1][tmi+8] = */ - fl*x*yr/(z*z);
                    AWT[tmi+8] += t9*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t9*w;
                 //(*ATA)[tmi+8][pnti+1] += acm;
                  (*Q)[tmi+8][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+8] += acm;
                 acm = t2*t9*w;
                 //(*ATA)[tmi+8][pnti+2] += acm;
                  (*Q)[tmi+8][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+8] += acm;
                 acm = t3*t9*w;
                // (*ATA)[tmi+8][pnti+3] += acm;
                  (*Q)[tmi+8][Qpnti+3] += acm;
                // (*ATA)[pnti+3][tmi+8] += acm;
                 acm = t4*t9*w;
                 //(*ATA)[tmi+8][tmi+1]  += acm;
                  (*P)[tmi+8][1]  += acm;
                 //(*ATA)[tmi+1][tmi+8]  += acm;
                   (*P)[tmi+1][8]  += acm;
                 acm = t5*t9*w;
                 //(*ATA)[tmi+8][tmi+2]  += acm;
                   (*P)[tmi+8][2]  += acm;
                 //(*ATA)[tmi+2][tmi+8]  += acm;
                   (*P)[tmi+2][8]  += acm;
                 acm = t6*t9*w;
                 //(*ATA)[tmi+8][tmi+3]  += acm;
                   (*P)[tmi+8][3]  += acm;
                 //(*ATA)[tmi+3][tmi+8]  += acm;
                   (*P)[tmi+3][8]  += acm;
                 acm = t7*t9*w;
                 //(*ATA)[tmi+8][tmi+10] += acm;
                   (*P)[tmi+8][10] += acm;
                 //(*ATA)[tmi+10][tmi+8] += acm;
                  (*P)[tmi+10][8] += acm;
                 acm = t8*t9*w;
                 //(*ATA)[tmi+8][tmi+7]  += acm;
                   (*P)[tmi+8][7]  += acm;
                // (*ATA)[tmi+7][tmi+8]  += acm;
                   (*P)[tmi+7][8]  += acm;
                 acm = t9*t9*w;
                 //(*ATA)[tmi+8][tmi+8]  += acm;
                   (*P)[tmi+8][8]  += acm;

              FLOAT t10 = /* (*A)[e+1][tmi+9] = */ - fl*x*zr/(z*z);
                    AWT[tmi+9] += t10*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t10*w;
                 //(*ATA)[tmi+9][pnti+1] += acm;
                  (*Q)[tmi+9][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+9] += acm;
                 acm = t2*t10*w;
                 //(*ATA)[tmi+9][pnti+2] += acm;
                  (*Q)[tmi+9][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+9] += acm;
                 acm = t3*t10*w;
                 //(*ATA)[tmi+9][pnti+3] += acm;
                  (*Q)[tmi+9][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+9] += acm;
                 acm = t4*t10*w;
                 //(*ATA)[tmi+9][tmi+1]  += acm;
                   (*P)[tmi+9][1]  += acm;
                 //(*ATA)[tmi+1][tmi+9]  += acm;
                   (*P)[tmi+1][9]  += acm;
                 acm = t5*t10*w;
                 //(*ATA)[tmi+9][tmi+2]  += acm;
                   (*P)[tmi+9][2]  += acm;
                // (*ATA)[tmi+2][tmi+9]  += acm;
                   (*P)[tmi+2][9]  += acm;
                 acm = t6*t10*w;
                 //(*ATA)[tmi+9][tmi+3]  += acm;
                   (*P)[tmi+9][3]  += acm;
                 //(*ATA)[tmi+3][tmi+9]  += acm;
                   (*P)[tmi+3][9]  += acm;
                 acm = t7*t10*w;
                 //(*ATA)[tmi+9][tmi+10] += acm;
                   (*P)[tmi+9][10] += acm;
                 //(*ATA)[tmi+10][tmi+9] += acm;
                   (*P)[tmi+10][9] += acm;
                 acm = t8*t10*w;
                 //(*ATA)[tmi+9][tmi+7]  += acm;
                   (*P)[tmi+9][7]  += acm;
                 //(*ATA)[tmi+7][tmi+9]  += acm;
                   (*P)[tmi+7][9]  += acm;
                 acm = t9*t10*w;
                 //(*ATA)[tmi+9][tmi+8]  += acm;
                   (*P)[tmi+9][8]  += acm;
                 //(*ATA)[tmi+8][tmi+9]  += acm;
                   (*P)[tmi+8][9]  += acm;
                 acm = t10*t10*w;
                 //(*ATA)[tmi+9][tmi+9]  += acm;
                   (*P)[tmi+9][9]  += acm;

              FLOAT t11 = /* (*A)[e+1][tmi+12] = */ - fl*x/(z*z);
                    AWT[tmi+12] += t11*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
                 acm = t1*t11*w;
                 //(*ATA)[tmi+12][pnti+1] += acm;
                   (*Q)[tmi+12][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+12] += acm;
                 acm = t2*t11*w;
                 //(*ATA)[tmi+12][pnti+2] += acm;
                  (*Q)[tmi+12][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+12] += acm;
                 acm = t3*t11*w;
                 //(*ATA)[tmi+12][pnti+3] += acm;
                  (*Q)[tmi+12][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+12] += acm;
                 acm = t4*t11*w;
                 //(*ATA)[tmi+12][tmi+1]  += acm;
                   (*P)[tmi+12][1]  += acm;
                 //(*ATA)[tmi+1][tmi+12]  += acm;
                   (*P)[tmi+1][12]  += acm;
                 acm = t5*t11*w;
                 //(*ATA)[tmi+12][tmi+2]  += acm;
                   (*P)[tmi+12][2]  += acm;
                 //(*ATA)[tmi+2][tmi+12]  += acm;
                   (*P)[tmi+2][12]  += acm;
                 acm = t6*t11*w;
                 //(*ATA)[tmi+12][tmi+3]  += acm;
                   (*P)[tmi+12][3]  += acm;
                 //(*ATA)[tmi+3][tmi+12]  += acm;
                   (*P)[tmi+3][12]  += acm;
                 acm = t7*t11*w;
                 //(*ATA)[tmi+12][tmi+10] += acm;
                   (*P)[tmi+12][10] += acm;
                 //(*ATA)[tmi+10][tmi+12] += acm;
                   (*P)[tmi+10][12] += acm;
                 acm = t8*t11*w;
                 //(*ATA)[tmi+12][tmi+7]  += acm;
                   (*P)[tmi+12][7]  += acm;
                 //(*ATA)[tmi+7][tmi+12]  += acm;
                   (*P)[tmi+7][12]  += acm;
                 acm = t9*t11*w;
                 //(*ATA)[tmi+12][tmi+8]  += acm;
                   (*P)[tmi+12][8]  += acm;
                 //(*ATA)[tmi+8][tmi+12]  += acm;
                   (*P)[tmi+8][12]  += acm;
                 acm = t10*t11*w;
                // (*ATA)[tmi+12][tmi+9]  += acm;
                   (*P)[tmi+12][9]  += acm;
                 //(*ATA)[tmi+9][tmi+12]  += acm;
                   (*P)[tmi+9][12]  += acm;
                 acm = t11*t11*w;
                 //(*ATA)[tmi+12][tmi+12] += acm;
                   (*P)[tmi+12][12] += acm;


 
              t1 = /* (*A)[e+2][pnti+1] = */ (fl*R[tmi+4]/z) - fl*y*R[tmi+7]/(z*z);
                    AWT[pnti+1] += t1*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
           
                  acm = t1*t1*w;
                 //(*ATA)[pnti+1][pnti+1] += acm;
                 (*S)[Spnti+1][Spnti+1] += acm;

              t2 = /* (*A)[e+2][pnti+2] = */ (fl*R[tmi+5]/z) - fl*y*R[tmi+8]/(z*z);
                    AWT[pnti+2] += t2*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t2*w;
                 //(*ATA)[pnti+2][pnti+1] += acm;
                 (*S)[Spnti+2][Spnti+1] += acm;
                 //(*ATA)[pnti+1][pnti+2] += acm;
                 (*S)[Spnti+1][Spnti+2] += acm;

                 acm = t2*t2*w;
                 //(*ATA)[pnti+2][pnti+2] += acm;
                 (*S)[Spnti+2][Spnti+2] += acm;

              t3 = /* (*A)[e+2][pnti+3] = */ (fl*R[tmi+6]/z) - fl*y*R[tmi+9]/(z*z);
                    AWT[pnti+3] += t3*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t3*w;
                 //(*ATA)[pnti+3][pnti+1] += acm;
                 (*S)[Spnti+3][Spnti+1] += acm;
                 //(*ATA)[pnti+1][pnti+3] += acm;
                 (*S)[Spnti+1][Spnti+3] += acm;
                 acm = t2*t3*w;
                 //(*ATA)[pnti+3][pnti+2] += acm;
                 (*S)[Spnti+3][Spnti+2] += acm;
                 //(*ATA)[pnti+2][pnti+3] += acm;
                 (*S)[Spnti+2][Spnti+3] += acm;
                 acm = t3*t3*w;
                 //(*ATA)[pnti+3][pnti+3] += acm;
                 (*S)[Spnti+3][Spnti+3] += acm;

              t4 = /* (*A)[e+2][tmi+4] = */ (fl*xr/z);
                    AWT[tmi+4] += t4*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t4*w;
                 //(*ATA)[tmi+4][pnti+1] += acm;
                  (*Q)[tmi+4][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+4] += acm;
                 acm = t2*t4*w;
                 //(*ATA)[tmi+4][pnti+2] += acm;
                  (*Q)[tmi+4][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+4] += acm;
                 acm = t3*t4*w;
                 //(*ATA)[tmi+4][pnti+3] += acm;
                  (*Q)[tmi+4][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+4] += acm;
                 acm = t4*t4*w;
                 //(*ATA)[tmi+4][tmi+4]  += acm;
                   (*P)[tmi+4][4]  += acm;

              t5 = /* (*A)[e+2][tmi+5] = */ (fl*yr/z);
                    AWT[tmi+5] += t5*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t5*w;
                 //(*ATA)[tmi+5][pnti+1] += acm;
                  (*Q)[tmi+5][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+5] += acm;
                 acm = t2*t5*w;
                 //(*ATA)[tmi+5][pnti+2] += acm;
                  (*Q)[tmi+5][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+5] += acm;
                 acm = t3*t5*w;
                 //(*ATA)[tmi+5][pnti+3] += acm;
                  (*Q)[tmi+5][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+5] += acm;
                 acm = t4*t5*w;
                 //(*ATA)[tmi+5][tmi+4]  += acm;
                   (*P)[tmi+5][4]  += acm;
                 //(*ATA)[tmi+4][tmi+5]  += acm;
                   (*P)[tmi+4][5]  += acm;
                 acm = t5*t5*w;
                 //(*ATA)[tmi+5][tmi+5]  += acm;
                   (*P)[tmi+5][5]  += acm;

              t6 = /* (*A)[e+2][tmi+6] = */ (fl*zr/z);
                    AWT[tmi+6] += t6*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t6*w;
                 //(*ATA)[tmi+6][pnti+1] += acm;
                  (*Q)[tmi+6][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+6] += acm;
                 acm = t2*t6*w;
                 //(*ATA)[tmi+6][pnti+2] += acm;
                  (*Q)[tmi+6][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+6] += acm;
                 acm = t3*t6*w;
                 //(*ATA)[tmi+6][pnti+3] += acm;
                  (*Q)[tmi+6][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+6] += acm;
                 acm = t4*t6*w;
                 //(*ATA)[tmi+6][tmi+4]  += acm;
                   (*P)[tmi+6][4]  += acm;
                 //(*ATA)[tmi+4][tmi+6]  += acm;
                   (*P)[tmi+4][6]  += acm;
                 acm = t5*t6*w;
                 //(*ATA)[tmi+6][tmi+5]  += acm;
                   (*P)[tmi+6][5]  += acm;
                 //(*ATA)[tmi+5][tmi+6]  += acm;
                   (*P)[tmi+5][6]  += acm;
                 acm = t6*t6*w;
                 //(*ATA)[tmi+6][tmi+6]  += acm;
                   (*P)[tmi+6][6]  += acm;

              t7 = /* (*A)[e+2][tmi+11] = */ (fl/z);
                    AWT[tmi+11] += t7*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t7*w;
                 //(*ATA)[tmi+11][pnti+1] += acm;
                  (*Q)[tmi+11][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+11] += acm;
                 acm = t2*t7*w;
                 //(*ATA)[tmi+11][pnti+2] += acm;
                  (*Q)[tmi+11][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+11] += acm;
                 acm = t3*t7*w;
                 //(*ATA)[tmi+11][pnti+3] += acm;
                  (*Q)[tmi+11][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+11] += acm;
                 acm = t4*t7*w;
                 //(*ATA)[tmi+11][tmi+4]  += acm;
                   (*P)[tmi+11][4]  += acm;
                 //(*ATA)[tmi+4][tmi+11]  += acm;
                   (*P)[tmi+4][11]  += acm;
                 acm = t5*t7*w;
                 //(*ATA)[tmi+11][tmi+5]  += acm;
                   (*P)[tmi+11][5]  += acm;
                 //(*ATA)[tmi+5][tmi+11]  += acm;
                   (*P)[tmi+5][11]  += acm;
                 acm = t6*t7*w;
                 //(*ATA)[tmi+11][tmi+6]  += acm;
                   (*P)[tmi+11][6]  += acm;
                 //(*ATA)[tmi+6][tmi+11]  += acm;
                   (*P)[tmi+6][11]  += acm;
                 acm = t7*t7*w;
                 //(*ATA)[tmi+11][tmi+11] += acm;
                   (*P)[tmi+11][11] += acm;

              t8 = /* (*A)[e+2][tmi+7] = */  - fl*y*xr/(z*z);
                    AWT[tmi+7] += t8*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t8*w;
                 //(*ATA)[tmi+7][pnti+1] += acm;
                  (*Q)[tmi+7][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+7] += acm;
                 acm = t2*t8*w;
                 //(*ATA)[tmi+7][pnti+2] += acm;
                  (*Q)[tmi+7][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+7] += acm;
                 acm = t3*t8*w;
                 //(*ATA)[tmi+7][pnti+3] += acm;
                  (*Q)[tmi+7][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+7] += acm;
                 acm = t4*t8*w;
                 //(*ATA)[tmi+7][tmi+4]  += acm;
                   (*P)[tmi+7][4]  += acm;
                 //(*ATA)[tmi+4][tmi+7]  += acm;
                   (*P)[tmi+4][7]  += acm;
                 acm = t5*t8*w;
                // (*ATA)[tmi+7][tmi+5]  += acm;
                   (*P)[tmi+7][5]  += acm;
                 //(*ATA)[tmi+5][tmi+7]  += acm;
                   (*P)[tmi+5][7]  += acm;
                 acm = t6*t8*w;
                 //(*ATA)[tmi+7][tmi+6]  += acm;
                   (*P)[tmi+7][6]  += acm;
                 //(*ATA)[tmi+6][tmi+7]  += acm;
                   (*P)[tmi+6][7]  += acm;
                 acm = t7*t8*w;
                 //(*ATA)[tmi+7][tmi+11] += acm;
                   (*P)[tmi+7][11] += acm;
                 //(*ATA)[tmi+11][tmi+7] += acm;
                   (*P)[tmi+11][7] += acm;
                 acm = t8*t8*w;
                 //(*ATA)[tmi+7][tmi+7]  += acm;
                   (*P)[tmi+7][7]  += acm;

              t9 = /* (*A)[e+2][tmi+8] = */  - fl*y*yr/(z*z);
                    AWT[tmi+8] += t9*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t9*w;
                 //(*ATA)[tmi+8][pnti+1] += acm;
                  (*Q)[tmi+8][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+8] += acm;
                 acm = t2*t9*w;
                 //(*ATA)[tmi+8][pnti+2] += acm;
                  (*Q)[tmi+8][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+8] += acm;
                 acm = t3*t9*w;
                 //(*ATA)[tmi+8][pnti+3] += acm;
                  (*Q)[tmi+8][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+8] += acm;
                 acm = t4*t9*w;
                 //(*ATA)[tmi+8][tmi+4]  += acm;
                   (*P)[tmi+8][4]  += acm;
                 //(*ATA)[tmi+4][tmi+8]  += acm;
                   (*P)[tmi+4][8]  += acm;
                 acm = t5*t9*w;
                 //(*ATA)[tmi+8][tmi+5]  += acm;
                   (*P)[tmi+8][5]  += acm;
                 //(*ATA)[tmi+5][tmi+8]  += acm;
                   (*P)[tmi+5][8]  += acm;
                 acm = t6*t9*w;
                 //(*ATA)[tmi+8][tmi+6]  += acm;
                   (*P)[tmi+8][6]  += acm;
                 //(*ATA)[tmi+6][tmi+8]  += acm;
                   (*P)[tmi+6][8]  += acm;
                 acm = t7*t9*w;
                 //(*ATA)[tmi+8][tmi+11] += acm;
                   (*P)[tmi+8][11] += acm;
                 //(*ATA)[tmi+11][tmi+8] += acm;
                   (*P)[tmi+11][8] += acm;
                 acm = t8*t9*w;
                 //(*ATA)[tmi+8][tmi+7]  += acm;
                   (*P)[tmi+8][7]  += acm;
                 //(*ATA)[tmi+7][tmi+8]  += acm;
                   (*P)[tmi+7][8]  += acm;
                 acm = t9*t9*w;
                 //(*ATA)[tmi+8][tmi+8]  += acm;
                   (*P)[tmi+8][8]  += acm;

              t10 = /* (*A)[e+2][tmi+9] = */  - fl*y*zr/(z*z);
                    AWT[tmi+9] += t10*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t10*w;             
                 //(*ATA)[tmi+9][pnti+1] += acm;
                  (*Q)[tmi+9][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+9] += acm;
                 acm = t2*t10*w;             
                 //(*ATA)[tmi+9][pnti+2] += acm;
                  (*Q)[tmi+9][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+9] += acm;
                 acm = t3*t10*w;             
                 //(*ATA)[tmi+9][pnti+3] += acm;
                  (*Q)[tmi+9][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+9] += acm;
                 acm = t4*t10*w;             
                 //(*ATA)[tmi+9][tmi+4]  += acm;
                   (*P)[tmi+9][4]  += acm;
                 //(*ATA)[tmi+4][tmi+9]  += acm;
                   (*P)[tmi+4][9]  += acm;
                 acm = t5*t10*w;             
                 //(*ATA)[tmi+9][tmi+5]  += acm;
                   (*P)[tmi+9][5]  += acm;
                 //(*ATA)[tmi+5][tmi+9]  += acm;
                   (*P)[tmi+5][9]  += acm;
                 acm = t6*t10*w;             
                 //(*ATA)[tmi+9][tmi+6]  += acm;
                   (*P)[tmi+9][6]  += acm;
                 //(*ATA)[tmi+6][tmi+9]  += acm;
                   (*P)[tmi+6][9]  += acm;
                 acm = t7*t10*w;             
                 //(*ATA)[tmi+9][tmi+11] += acm;
                   (*P)[tmi+9][11] += acm;
                 //(*ATA)[tmi+11][tmi+9] += acm;
                   (*P)[tmi+11][9] += acm;
                 acm = t8*t10*w;             
                 //(*ATA)[tmi+9][tmi+7]  += acm;
                   (*P)[tmi+9][7]  += acm;
                 //(*ATA)[tmi+7][tmi+9]  += acm;
                   (*P)[tmi+7][9]  += acm;
                 acm = t9*t10*w;             
                 //(*ATA)[tmi+9][tmi+8]  += acm;
                   (*P)[tmi+9][8]  += acm;
                 //(*ATA)[tmi+8][tmi+9]  += acm;
                   (*P)[tmi+8][9]  += acm;
                 acm = t10*t10*w;             
                 //(*ATA)[tmi+9][tmi+9]  += acm;
                   (*P)[tmi+9][9]  += acm;

              t11 = /* (*A)[e+2][tmi+12] = */ - fl*y/(z*z);
                    AWT[tmi+12] += t11*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
                 acm = t1*t11*w;
                 //(*ATA)[tmi+12][pnti+1] += acm;
                  (*Q)[tmi+12][Qpnti+1] += acm;
                 //(*ATA)[pnti+1][tmi+12] += acm;
                 acm = t2*t11*w;
                 //(*ATA)[tmi+12][pnti+2] += acm;
                  (*Q)[tmi+12][Qpnti+2] += acm;
                 //(*ATA)[pnti+2][tmi+12] += acm;
                 acm = t3*t11*w;
                 //(*ATA)[tmi+12][pnti+3] += acm;
                  (*Q)[tmi+12][Qpnti+3] += acm;
                 //(*ATA)[pnti+3][tmi+12] += acm;
                 acm = t4*t11*w;
                 //(*ATA)[tmi+12][tmi+4]  += acm;
                   (*P)[tmi+12][4]  += acm;
                 //(*ATA)[tmi+4][tmi+12]  += acm;
                   (*P)[tmi+4][12]  += acm;
                 acm = t5*t11*w;
                 //(*ATA)[tmi+12][tmi+5]  += acm;
                   (*P)[tmi+12][5]  += acm;
                 //(*ATA)[tmi+5][tmi+12]  += acm;
                   (*P)[tmi+5][12]  += acm;
                 acm = t6*t11*w;
                 //(*ATA)[tmi+12][tmi+6]  += acm;
                   (*P)[tmi+12][6]  += acm;
                 //(*ATA)[tmi+6][tmi+12]  += acm;
                   (*P)[tmi+6][12]  += acm;
                 acm = t7*t11*w;
                 //(*ATA)[tmi+12][tmi+11] += acm;
                   (*P)[tmi+12][11] += acm;
                 //(*ATA)[tmi+11][tmi+12] += acm;
                   (*P)[tmi+11][12] += acm;
                 acm = t8*t11*w;
                 //(*ATA)[tmi+12][tmi+7]  += acm;
                   (*P)[tmi+12][7]  += acm;
                 //(*ATA)[tmi+7][tmi+12]  += acm;
                   (*P)[tmi+7][12]  += acm;
                 acm = t9*t11*w;
                 //(*ATA)[tmi+12][tmi+8]  += acm;
                   (*P)[tmi+12][8]  += acm;
                 //(*ATA)[tmi+8][tmi+12]  += acm;
                   (*P)[tmi+8][12]  += acm;
                 acm = t10*t11*w;
                 //(*ATA)[tmi+12][tmi+9]  += acm;
                   (*P)[tmi+12][9]  += acm;
                 //(*ATA)[tmi+9][tmi+12]  += acm;
                   (*P)[tmi+9][12]  += acm;
                 acm = t11*t11*w;
                 //(*ATA)[tmi+12][tmi+12] += acm;
                   (*P)[tmi+12][12] += acm;

              e += 2;
               
            
            }
            pnti += 3;
            Qpnti += 3;
            Spnti += 3;
          }
        } 
        tmi += 12;    
      }
    }



   //   awx += wx * photosum[inf];
   //   awy += wy * photosum[inf];
   //   wnp += photosum[inf];

//   awx = awx/wnp;
//   awy = awy/wnp;

//   printf(" avg wx = %e, avg wy = %e, dx = %e, dy = %e\n",awx,awy,awx*fl,awy*fl);

    int pnti = nuf*12;
    int Spnti = 0;
    for(inp=1;inp<=np;inp++)
    {
      if(pntsum[inp] > 0) // if pnt is usable
      {
        if( (*sp)[inp-1] != NULL) // if target point provided
        {
           sig[e+1] = sig[e+2] = sig[e+3] = 1.0/3.0;
           FLOAT w = 1.0/sig[e+2];
                 w*=w;

           L[e+1] = (*sp)[inp-1][1] - R[pnti+1];
           L[e+2] = (*sp)[inp-1][2] - R[pnti+2];
           L[e+3] = (*sp)[inp-1][3] - R[pnti+3];

//printf(" sp = %e,%e,%e    pnt = %e,%e,%e\n",(*sp)[inp-1][1],(*sp)[inp-1][2],
//                             (*sp)[inp-1][3],R[pnti+1],R[pnti+2],R[pnti+3]);

           FLOAT t1 = /* (*A)[e+1][pnti+1] = */ 1.0;
             AWT[pnti+1] += t1*L[e+1]*(1.0/(sig[e+1]*sig[e+1]));
           FLOAT acm = t1*t1*w;
             //(*ATA)[pnti+1][pnti+1] +=acm; 
             (*S)[Spnti+1][Spnti+1] +=acm;

           t1 = /* (*A)[e+2][pnti+2] = */ 1.0;
             AWT[pnti+2] += t1*L[e+2]*(1.0/(sig[e+2]*sig[e+2]));
             acm = t1*t1*w;
             //(*ATA)[pnti+2][pnti+2] +=acm; 
             (*S)[Spnti+2][Spnti+2] +=acm;

           t1 = /* (*A)[e+3][pnti+3] = */ 1.0;
             AWT[pnti+3] += t1*L[e+3]*(1.0/(sig[e+3]*sig[e+3]));
             acm = t1*t1*w;             
             //(*ATA)[pnti+3][pnti+3] +=acm; 
             (*S)[Spnti+3][Spnti+3] +=acm;
    
           e += 3;
        }
        pnti += 3;
        Spnti +=3; 
      }
    }


    FLOAT maxp = 0.0;
    FLOAT avgp = 0.0;

    for(int i= nuf*12+1;i< nuf*12+1+2*nup;i+=2)
    {
       FLOAT temp= sqrt(L[i]*L[i] + L[i+1]*L[i+1]);
       avgp += temp;
       if(temp>maxp)maxp=temp;
    }

    avgp = avgp/(nup); 

    FLOAT maxr = 0.0;
    FLOAT avgr = 0.0;
    for(i = nuf*12+1+2*nup;i<ne;i+=3)
    {
       FLOAT temp= sqrt(L[i]*L[i] + L[i+1]*L[i+1] + L[i+2]*L[i+2]);
       avgr += temp;
       if(temp>maxr)maxr=temp;
    }
    avgr = avgr/(nut); 

Output(0, "errors   film plane =%e avg, %e max;   target point = %e avg, %e max\n",avgp,maxp,avgr,maxr);

//printf("start lsq\n");
//    VECTOR D=lsq(A,L,nu,ne);
//printf("end lsq\n");

/******
{
PNTLST vtmp = matrix(1,nu,1,nu);
            
for(int ii=1;ii<=nu;ii++)
{
  for(int jj=1;jj<=nu;jj++)
  {
    (*vtmp)[ii][jj] = 0.0;
    for(int kk = 1;kk<=ne;kk++)
    {
      (*vtmp)[ii][jj] += (*A)[kk][ii]*(*A)[kk][jj];
    }
  }
}
printf("\n\n");
printf(" nu = %d,  ne = %d\n",nu,ne);
for( ii =1;ii<=ne;ii++)
{
  for( int jj=1; jj<=nu; jj++)
  {
    if( (*A)[ii][jj] != 0.0)
    {
      printf("_");
    }
    else
    {
      printf("0");
    }
  }
  printf("\n");
}
printf("\n\n");
printf(" nu = %d,  ne = %d\n",nu,ne);

for( ii =1;ii<=nu;ii++)
{
  for( int jj=1; jj<=nu; jj++)
  {
    if( (*vtmp)[ii][jj] != 0.0)
    {
      printf("_");
    }
    else
    {
      printf("0");
    }
  }
  printf("\n");
}
free_matrix(vtmp,1,nu,1,nu);



printf("new S\n");

for( ii =1;ii<=12*nf;ii++)
{
  for( int jj=1; jj<=nu-12*nf; jj++)
  {
    if( (*S)[ii][jj] != 0.0)
    {
      printf("_");
    }
    else
    {
      printf("0");
    }
  }
  printf("\n");
}
}


*****/

/*******

printf("new ATA\n");

for( ii =1;ii<=nu;ii++)
{
  for( int jj=1; jj<=nu; jj++)
  {
    if( (*ATA)[ii][jj] != 0.0)
    {
      printf("_");
    }
    else
    {
      printf("0");
    }
  }
  printf("\n");
}



*****/

//printf("vector L");
//print_vector(L,1,ne);
//getchar();
//void BBlsq(PNTLST A, VECTOR L,VECTOR s, VECTOR RR,int nu,int ne,int nf,int np,FLOAT *chisq)
/*****
    for(int ii=1;ii<=nu;ii++)
    {
      AWT[ii] = 0.0;
      for(int jj=1;jj<=ne;jj++) 
      {
         AWT[ii] += (*A)[jj][ii]*L[jj]*(1.0/(sig[jj]*sig[jj]));
      }
    }
*****/
//printf("AWT ");
//print_vector(AWT,1,nu);

  pchisq = 0.0;
  for(int ii=1;ii<=ne;ii++)
  {
    FLOAT c = L[ii]/sig[ii];
    pchisq += c*c;
  }

pchisq = pchisq/ne;
printf("pre chisq = %e\n",pchisq);

if(sflag)
{
  sflag=0;
  old_pchisq = 2.0*pchisq;
}

//if(fabs(old_pchisq - pchisq) < oldchisq*1.0e-7)pchisq = old_pchisq;


if(pchisq > old_pchisq)
{
   chisq = 2.0*oldchisq;
   for(ii=1;ii<=nu;ii++)
   {
     R[ii] -= a[ii]*CC*C;
   }
   CC=0.5*CC;
   if(CC <= 1.000e-2)CC = 0.0;
   printf("new cc = %e\n",CC);
}
else
{ 
    chisq = pchisq;
    old_pchisq = pchisq;
    BBlsq(NULL,P,Q,S,AWT,sig,a,nu,ne,nuf,nup,photosum,pntsum,fp,nf,np,&chisq);
    C=CC=1.0;


   // free_matrix(ATA,1,nu,1,nu);
   // svdfit(A,L,sig,ne,a,nu,u,v,w,&chisq,NULL);
  //printf("vector w");
  //print_vector(w,1,nu);

/*****
  FLOAT K1 = 0.0;
  FLOAT K2 = 0.0;
  chisq = 0.0;
  for(ii = 1;ii<=ne;ii++)
  {
    FLOAT tmp = 0.0;
    for(int jj=1;jj<=nu;jj++)
    {
       tmp += (*A)[ii][jj]*a[jj];
    }

    K1 += L[ii]*tmp/(sig[ii]*sig[ii]);
    K2 += tmp*tmp/(sig[ii]*sig[ii]);
    
  
    tmp = (tmp-L[ii])/sig[ii];
    chisq += tmp*tmp;
  }

  chisq = chisq/ne;
  C=K1/K2;
  printf("trial mult  K1 = %e,  K2 = %e, C = %e\n",K1,K2,K1/K2);
  printf("reg chisq = %e\n",chisq);  
******/

 }

/******
  chisq = 0.0;
  for(ii = 1;ii<=ne;ii++)
  {
    FLOAT tmp = 0.0;
    for(int jj=1;jj<=nu;jj++)
    {
       tmp += (*A)[ii][jj]*a[jj]*C*CC;
    }

   //K1 += L[ii]*tmp/(sig[ii]*sig[ii]);
   // K2 += tmp*tmp/(sig[ii]*sig[ii]);
    
  
    tmp = (tmp-L[ii])/sig[ii];
    chisq += tmp*tmp;
  }
chisq = chisq/ne;




printf("test chisq = %e\n",chisq);  
*******/

 for( i=1;i<=nu;i++)R[i] += C*CC*a[i];



  Output(1,"%d xxx chisq = %e\n",k,chisq);
 if(CC == 0.0)break;
    if(fabs(oldchisq - chisq) < oldchisq*1.0e-5)
    {
      bflag += 1;
    }
    else
    {
      bflag = 0;
    }

    if(bflag >= 2)break;
    oldchisq = old_pchisq;



    
    




      
  } // end main iteration loop


  // Hopefully a solution has been found

  // get tms from back into bd

  int tmi = 1;
  
  for( inf = 0; inf < nf; inf++) //loop over photos
  {
    if(photosum[inf+1] > 0)
    {
      //VECTOR mat = matmat(&R[tmi-1],&R[0]);
      copy_vector( (*tm)[inf],1,12,&R[tmi-1],1,12);
      //free_vector(mat,1,13);
      tmi += 12;
    }
  }   

  //get calculated points back into bd

  int cpi = tmi;

  for( inp = 0; inp < np; inp++) //loop over points
  {
    if(pntsum[inp+1] > 0) // if pnt is usable
    {
       //have a new unknown px,py,pz
       if((*cpx)[inp] == NULL)
       {
         (*cpx)[inp] = vector(1,3);
       } 
       copy_vector((*cpx)[inp],1,3, R,cpi,cpi+2);      
       cpi += 3;
            
    }        
  }        


//#############

  for(  inf = 1;inf <= nf;inf++)        // for each photo
  {

    VECTOR ftm  = (*tm)[inf-1];
    VECTOR fpnt = NULL;
    VECTOR tpnt = NULL;
    if(ftm != NULL)
    {
      for(inp=1;inp<=np;inp++)                 // for each point 
      {
        fpnt = (*(fp[inf-1]))[inp-1];

        if( fpnt != NULL )                     // if foto point in this photo
        {
          tpnt = (*sp)[inp-1]; 
          if(tpnt != NULL)                     // if target point;
          {

             FLOAT tx = ftm[1]*tpnt[1] + ftm[2]*tpnt[2] + ftm[3]*tpnt[3] + ftm[10];
             FLOAT ty = ftm[4]*tpnt[1] + ftm[5]*tpnt[2] + ftm[6]*tpnt[3] + ftm[11];
             FLOAT tz = ftm[7]*tpnt[1] + ftm[8]*tpnt[2] + ftm[9]*tpnt[3] + ftm[12];

                   tx *= fl/tz;
                   ty *= fl/tz;

            FLOAT  fx = fl*fpnt[1]/fpnt[3];                 
            FLOAT  fy = fl*fpnt[2]/fpnt[3];  

            FLOAT  fr = sqrt( fx*fx + fy*fy );
            FLOAT  tr = sqrt( tx*tx + ty*ty );

printf(" %d,%d tr = %e,  fr = %e;  dr = %e, dr/r3 = %e\n",inp-1,inf-1,tr,fr,fr-tr,(fr-tr)/(fr*fr*fr) );             

          }          
        }
      }
    }
  }




//###########

  free_vector(a,1,nu);
  free_vector(w,1,nu);
  free_vector(pntsum,1,np); // will contain count of photos each point is in
  free_vector(photosum,1,nf); // will contain count of usuable points for each photo
  free_vector(AWT,1,nu);
  free_matrix(Q,1,12*nf,1,nu-12*nf);
  free_matrix(S,1,nu-12*nf,1,nu-12*nf);
  free_matrix(P,1,12*nf,1,12/**nf*/);

    free_vector(R,1,nu);
    free_vector(L,1,ne);

    free_vector(sig,1,ne);






 // bundlecheck(fl,bd);



  return;

}







void newBB(BundleData &bd)
{

  PNTLST tm = bd.plTMatrix;
  PNTLST cpx = bd.plCalcPnts;
  PNTLST *fp = bd.plPixels;
  PNTLST sp = bd.plSurvPnts;
  int    nf = bd.nNumPhotos;
  int    np = bd.nNumPoints;

  int i;

  int ntf = 0;  // number of usuable photos ( some photos may not have matching points)
  int flag = 0;
  int tz = 12;  // number of unknowns due to each photo ( 12 for tm elements)
  int tf = 0;
 // int tf = 12;
  FLOAT sf = 1.0;



  // find number of photos with matching points, (ntf)

  for(int inf = 0; inf < nf; inf++) //loop over photos
  {
    flag = 0;
    for(int inp = 0; inp < np; inp++) //loop over points
    {
      if( (*(fp[inf]))[inp] != NULL) // test for point defined in this photog
      {
        for(i = 0; i < nf; i++) //see if point defined in photo other than this one
        {
          if(i != inf)
          {
            if( (*(fp[i]))[inp] != NULL) flag = 1;//matching point found
          }
        }
      }
      if(flag == 1)break; 
    }
    if(flag == 1)ntf++; // increment count of usable photos
  }   

 
   
  int nu = tz*ntf+tf; // number of unknowns, initially set to 12 per usable photo
  int ne = 9*ntf;     // number of equations, initial set to 
                      //    9 per usable photo (ortho equations);

  if(tf > 0)ne += 13;


  // find number of matching points and figure number of additional unknowns and equations

  for(int inp = 0; inp < np; inp++)      // loop over points
  {
    int count = 0;                       // count will become number of photos containing
                                         // this common point

    for(int inf = 0; inf < nf; inf++)    // loop over photos
    {
      if( (*(fp[inf]))[inp] != NULL)count++; // if point defined for this photo increment
                                             // count of photos containing this point
    }


    if(count > 1)  // have found at least a pair of photos with this point in common,
                   // this satisfies conditions for use of this point i
    {
      nu += 3;  //have a new unknown px,py,pz
      
      
      for( inf = 0; inf < nf; inf++) // loop over photos
      {
        if( (*(fp[inf]))[inp] != NULL) //point in this photo
        {
          ne += 3; // have 3 new equations (cross prodect of point and photo vector )
        }
      }
      if( (*sp)[inp] != NULL )  // have a surv. point for thos point
      {
         ne += 3; // have 3 new equations pnts = surv. pnts
      }
    }        
  }        
 




  
  // now have enough information to allocate required memory

  PNTLST A = matrix(1,ne,1,nu);
  VECTOR L = vector(1,ne);           
  VECTOR R = vector(1,nu); 

  // initialize R

  if(tf > 0.0)
  {
    R[1] = R[5] = R[9] = 1.0;
  }




  // get tms first -  must loop over pnts and photos again for at least matching pnt pairs

  int tmi = 1+tf;
  
  for( inf = 0; inf < nf; inf++) //loop over photos
  {
    flag = 0;
    for(int inp = 0; inp < np; inp++)// loop over pnts
    {
      if( (*(fp[inf]))[inp] != NULL)// if point defined
      {
        for(i = 0; i < nf; i++)
        {
          if(i != inf)
          {
            if( (*(fp[i]))[inp] != NULL) flag = 1;//matching point found
          }
        }
      }
      if(flag == 1)break; // break pnt loop, at least found
    }
    if(flag == 1) // indicates photo can be used
    {
      copy_vector( R,tmi,tmi+11, (*tm)[inf],1,12); // copy photos tmatrix into R
      if(tz == 13)R[tmi+tz-1] = 1.0;
      tmi += tz;
    }
  }   

  //get calculated points

    int cpi = tmi;

   for( inp = 0; inp < np; inp++) //loop over points
  {
    int count = 0;
    for(int inf = 0; inf < nf; inf++) // loop over photos
    {
      if( (*(fp[inf]))[inp] != NULL)count++;
    }
    if(count > 1)  // have found at least a pair of points
    {
       //have a new unknown px,py,pz
       if((*cpx)[inp] == NULL)
       {
          printf("****** error - null calculated point in newBB *****\n");
          getchar();
       } 
       copy_vector(R,cpi,cpi+2,(*cpx)[inp],1,3); //copy calculated point into R;      
       cpi += 3; // increment calculated point index
            
    }        
  }        
     FLOAT sum1 = 0.0;
     FLOAT sum2 = 0.0;
     FLOAT sum3 = 0.0;

     FLOAT sum1max = 0.0;
     FLOAT sum2max = 0.0;
     FLOAT sum3max = 0.0;

     int sum1cnt = 0;
     int sum2cnt = 0;
     int sum3cnt = 0;

     FLOAT wf1 = 100.0;
     FLOAT wf2 = 10.0;



  /*********** MAIN LOOP ******************/

  for(int loop =1; loop <= 200; loop++)
  {
     FLOAT temp = 0.0;
     sum1 = 0.0;
     sum2 = 0.0;
     sum3 = 0.0;

     sum1max = 0.0;
     sum2max = 0.0;
     sum3max = 0.0;

     sum1cnt = 0;
     sum2cnt = 0;
     sum3cnt = 0;

     tmi = tf+1;
     int e = 1;

     // setup tmatrix equations
 /*****
    if(tf > 0)
     {
       //forcesquare(L,R,*A);
       

       FLOAT w = 1.0;

       sf = (R[1] + R[5] + R[9])/( R[1]*R[1] + R[5]*R[5] + R[9]*R[9] );
     
       for(int ii = 1; ii<=12; ii++)R[ii] *= sf; 

       sf = 1.0 + 0.75*(sf-1.0);

       L[e] = w*1.0*( R[1] + R[5] + R[9] -R[1]*R[1] -R[5]*R[5] -R[9]*R[9] );
       (*A)[e][1] = w*10.0*(-1.0 + 2.0*R[1]);
       (*A)[e][5] = w*10.0*(-1.0 + 2.0*R[5]);
       (*A)[e][9] = w*10.0*(-1.0 + 2.0*R[9]);
       e++;

       L[e] = w*(1.0-R[1]);
       (*A)[e][1] = w*1.0;

       L[e+1] = -w*1,0*R[2];
       (*A)[e+1][2] = w*100*1.0;
 
       L[e+2] = -w*1.0*R[3];
       (*A)[e+2][3] = w*100*1.0;

       L[e+3] = -w*1.0*R[4];
       (*A)[e+3][4] = w*100*1.0;

       L[e+4] =  w*(1.0-R[5]);
       (*A)[e+4][5] = w*(1.0);


       L[e+5] = -w*1.0*R[6];
       (*A)[e+5][6] = w*100*1.0;

       L[e+6] = -w*1.0*R[7];
       (*A)[e+6][7] = w*100*1.0;

       L[e+7] = -w*1.0*R[8];
       (*A)[e+7][8] = w*100*1.0;

       L[e+8] = w*(1.0-R[9] );
       (*A)[e+8][9] = w*(1.0);


      L[e+9] = -w*1.0*R[10];
       (*A)[e+9][10] = w*100*1.0;

      L[e+10] = -w*1.0*R[11];
       (*A)[e+10][11] = w*100*1.0;

      L[e+11] = -w*1.0*R[12];
       (*A)[e+11][12] = w*100*1.0;



       e += 12;
     }
*****/

     for(i = 1; i <= ntf; i++) // loop over photos
     {
       //R[tmi+9] *= sf;
      // R[tmi+10] *= sf;
      // R[tmi+11] *= sf;

       PNTLST AA = submatrix( A,e,e+8,tmi,tmi+11,1,1);
       forcesquare( &(L[e-1]),&(R[tmi-1]), *AA );
       free_submatrix(AA,1,9,1,12);
       e += 9;
       tmi += tz;
     }

    //  setup point equations

     int cpi = (tf+tz*ntf)-3;

    for(int inp = 0; inp < np; inp++) //loop over points
    {
      int count = 0;
      for(int inf = 0; inf < nf; inf++) // loop over photos
      {
        if( (*(fp[inf]))[inp] != NULL)count++;
      }
      if(count > 1)  // have found at least a pair of points
      {
         //have a new unknown px,py,pz
         cpi += 3;
         //R[cpi+1] *= sf;
         //R[cpi+2] *= sf;
         //R[cpi+3] *= sf;
        
      
        int tmi = tf-tz;
        for( inf = 0; inf < nf; inf++)  // for each photo
        {
          tmi += tz;
          if( (*(fp[inf]))[inp] != NULL) 
          {
             // have 3 new equations (cross product of point and photo vector )

             // transfer world system point to photo system

             VECTOR tp = matpnt( &(R[tmi]), &(R[cpi]) );

             VECTOR p = (*(fp[inf]))[inp]; // get photo ray associated with this point
      

             //EQUATION 0.0 = | PxF |  each component of cross product vector of point 
                                     //and ray should be zero for ray to intersect point

             L[e]  = wf1*(-tp[2]*p[3] + tp[3]*p[2]);
             L[e+1]= wf1*(-tp[3]*p[1] + tp[1]*p[3]);
             L[e+2]= wf1*(-tp[1]*p[2] + tp[2]*p[1]);
 
             temp = L[e]/wf1;
             if(temp < 0.0)temp=-temp;
             if(temp   > sum1max) sum1max = temp;
             sum1 += temp;
             temp = L[e+1]/wf1;
             if(temp < 0.0)temp=-temp;
             if(temp   > sum1max) sum1max = temp;
             sum1 += temp;
             temp = L[e+2]/wf1;
             if(temp < 0.0)temp=-temp;
             if(temp   > sum1max) sum1max = temp;
             sum1 += temp;

             sum1cnt += 3;

       
            (*A)[e][tmi+4]  = wf1*p[3]*R[cpi+1];
            (*A)[e][tmi+5]  = wf1*p[3]*R[cpi+2];
            (*A)[e][tmi+6]  = wf1*p[3]*R[cpi+3];
            (*A)[e][tmi+11] = wf1*p[3];

            (*A)[e][tmi+7] = -wf1*p[2]*R[cpi+1];
            (*A)[e][tmi+8] = -wf1*p[2]*R[cpi+2];
            (*A)[e][tmi+9] = -wf1*p[2]*R[cpi+3];
            (*A)[e][tmi+12] = -wf1*p[2];

            (*A)[e][cpi+1] = wf1*(p[3]*R[tmi+4] - p[2]*R[tmi+7]);
            (*A)[e][cpi+2] = wf1*(p[3]*R[tmi+5] - p[2]*R[tmi+8]);
            (*A)[e][cpi+3] = wf1*(p[3]*R[tmi+6] - p[2]*R[tmi+9]);
 
           

            (*A)[e+1][tmi+7]  = wf1*p[1]*R[cpi+1];
            (*A)[e+1][tmi+8]  = wf1*p[1]*R[cpi+2];
            (*A)[e+1][tmi+9]  = wf1*p[1]*R[cpi+3];
            (*A)[e+1][tmi+12] = wf1*p[1];

            (*A)[e+1][tmi+1]  = -wf1*p[3]*R[cpi+1];
            (*A)[e+1][tmi+2]  = -wf1*p[3]*R[cpi+2];
            (*A)[e+1][tmi+3]  = -wf1*p[3]*R[cpi+3];
            (*A)[e+1][tmi+10] = -wf1*p[3];

            (*A)[e+1][cpi+1] = wf1*(p[1]*R[tmi+7] - p[3]*R[tmi+1]);
            (*A)[e+1][cpi+2] = wf1*(p[1]*R[tmi+8] - p[3]*R[tmi+2]);
            (*A)[e+1][cpi+3] = wf1*(p[1]*R[tmi+9] - p[3]*R[tmi+3]);
 


            (*A)[e+2][tmi+1]  = wf1*p[2]*R[cpi+1];
            (*A)[e+2][tmi+2]  = wf1*p[2]*R[cpi+2];
            (*A)[e+2][tmi+3]  = wf1*p[2]*R[cpi+3];
            (*A)[e+2][tmi+10] = wf1*p[2];

            (*A)[e+2][tmi+4]  = -wf1*p[1]*R[cpi+1];
            (*A)[e+2][tmi+5]  = -wf1*p[1]*R[cpi+2];
            (*A)[e+2][tmi+6]  = -wf1*p[1]*R[cpi+3];
            (*A)[e+2][tmi+11] = -wf1*p[1];


            (*A)[e+2][cpi+1] = wf1*(p[2]*R[tmi+1] - p[1]*R[tmi+4]);
            (*A)[e+2][cpi+2] = wf1*(p[2]*R[tmi+2] - p[1]*R[tmi+5]);
            (*A)[e+2][cpi+3] = wf1*(p[2]*R[tmi+3] - p[1]*R[tmi+6]);

            e += 3;

            free_vector(tp,1,3);
//  printf(" L after cal pnt equ");
// print_vector(L,1,ne);
// printf("%e,%e,%d,%e,%e,%d\n", sum1,sum1max,sum1cnt,sum2,sum2max ,sum2cnt);
// getchar(); 

          }
        }
        if( (*sp)[inp] != NULL )  // have a surv. point for this point
        {
           // have 3 new equations pnts = surv. pnts
           FLOAT wfx =10.0;
           
           L[e]   = wfx*((*sp)[inp][1] - R[cpi+1]);
           L[e+1] = wfx*((*sp)[inp][2] - R[cpi+2]);
           L[e+2] = wfx*((*sp)[inp][3] - R[cpi+3]);

           (*A)[e][cpi+1]   = wfx*1.0;
           (*A)[e+1][cpi+2] = wfx*1.0;
           (*A)[e+2][cpi+3] = wfx*1.0;

             temp = L[e]/wfx;
             if(temp < 0.0)temp=-temp;
             if(temp   > sum3max) sum3max = temp;
             sum3 += temp;
             temp = L[e+1]/wfx;
             if(temp < 0.0)temp=-temp;
             if(temp   > sum3max) sum3max = temp;
             sum3 += temp;
             temp = L[e+2]/wfx;
             if(temp < 0.0)temp=-temp;
             if(temp   > sum3max) sum3max = temp;
             sum3 += temp;

             sum3cnt += 3;


 

           e += 3;
/****

           L[e]   = wf2*( R[cpi+1] - (*sp)[inp][1]*R[1]
                                   - (*sp)[inp][2]*R[2]
                                   - (*sp)[inp][3]*R[3] - R[10] );

           //(*A)[e][cpi+1] = -wf2*1.0;
 
           (*A)[e][1] = wf2*(*sp)[inp][1];
           (*A)[e][2] = wf2*(*sp)[inp][2];
           (*A)[e][3] = wf2*(*sp)[inp][3];
           (*A)[e][10] = wf2*1.0;


           L[e+1]   = wf2*( R[cpi+2] - (*sp)[inp][1]*R[4]
                                     - (*sp)[inp][2]*R[5]
                                     - (*sp)[inp][3]*R[6] - R[11] );

           //(*A)[e+1][cpi+2] = -wf2*1.0;
 
           (*A)[e+1][4] = wf2*(*sp)[inp][1];
           (*A)[e+1][5] = wf2*(*sp)[inp][2];
           (*A)[e+1][6] = wf2*(*sp)[inp][3];
           (*A)[e+1][11] = wf2*1.0;



           L[e+2]   = wf2*( R[cpi+3] - (*sp)[inp][1]*R[7]
                                     - (*sp)[inp][2]*R[8]
                                     - (*sp)[inp][3]*R[9] - R[12] );

           //(*A)[e+2][cpi+3] = -wf2*1.0;
 
           (*A)[e+2][7] = wf2*(*sp)[inp][1];
           (*A)[e+2][8] = wf2*(*sp)[inp][2];
           (*A)[e+2][9] = wf2*(*sp)[inp][3];
           (*A)[e+2][12] = wf2*1.0;

 



             temp = L[e]/wf2;
             if(temp < 0.0)temp=-temp;
             if(temp   > sum2max) sum2max = temp;
             sum2 += temp;
             temp = L[e+1]/wf2;
             if(temp < 0.0)temp=-temp;
             if(temp   > sum2max) sum2max = temp;
             sum2 += temp;
             temp = L[e+2]/wf2;
             if(temp < 0.0)temp=-temp;
             if(temp   > sum2max) sum2max = temp;
             sum2 += temp;

             sum2cnt += 3;



           e += 3;

*****/
// printf(" L after real pnt  equation");
//print_vector(L,1,ne);
//printf("%e,%e,%d,%e,%e,%d\n", sum1,sum1max,sum1cnt,sum2,sum2max ,sum2cnt);
//getchar(); 
       }
      }        
    }        

    // equations have been setup   

//printf("%e,%e,%d,%e,%e,%d\n", sum1,sum1max,sum1cnt,sum3,sum3max ,sum3cnt);
  sum1 = sum1/sum1cnt; 
//  sum2 = sum2/sum2cnt; 
  sum3 = sum3/sum3cnt; 
   
  printf(" calulated pnts avg. = %e,  max = %e, real pnts avg = %e,  max = %e\n",
           sum1,sum1max,sum3,sum3max);
//  printf("to real no correction - avg = %e,  max = %e , sf = %e \n",sum3,sum3max,sf);
// printf("vector R\n");
// print_vector(R,1,nu);

// printf("\nvector L - \n");
// print_vector(L,1,ne);
/*
//printf("matrix A\n");
//print_matrix(A,1,ne,1,nu);
*/
    VECTOR D=lsq(A,L,nu,ne);
  //  add_vector(R,1,nu,D,1,nu);

    int flag = 0;
    for(i=1;i<=nu;i++)
    {
      R[i] += 0.7*D[i];
      if(D[i] < 0.0)D[i] = -D[i];
      if(D[i] > 1.0e-5)flag = 1;
    }

// printf("\nvector D\n");
// print_vector(D,1,nu);


    
   free_vector(D,1,nu);

   if(flag == 0)break;
  }



  // Hopefully a solution has been found

  // get tms from back into bd

  tmi = tf+1;
  
  for( inf = 0; inf < nf; inf++) //loop over photos
  {
    flag = 0;
    for(int inp = 0; inp < np; inp++)
    {
      if( (*(fp[inf]))[inp] != NULL)
      {
        for(i = 0; i < nf; i++)
        {
          if(i != inf)
          {
            if( (*(fp[i]))[inp] != NULL) flag = 1;//matching point found
          }
        }
      }
      if(flag == 1)break; 
    }
    if(flag == 1)
    {
      //VECTOR mat = matmat(&R[tmi-1],&R[0]);
      copy_vector( (*tm)[inf],1,12,&R[tmi-1],1,12);
      //free_vector(mat,1,13);
      tmi += tz;
    }
  }   



  //get calculated points back into bd

     cpi = tmi;

   for( inp = 0; inp < np; inp++) //loop over points
  {
    int count = 0;
    for(int inf = 0; inf < nf; inf++) // loop over photos
    {
      if( (*(fp[inf]))[inp] != NULL)count++;
    }
    if(count > 1)  // have found at least a pair of points
    {
       //have a new unknown px,py,pz
       if((*cpx)[inp] == NULL)
       {
          printf("****** error - null calculated point in newBB *****\n");
          getchar();
       } 
       copy_vector((*cpx)[inp],1,3, R,cpi,cpi+2);      
       cpi += 3;
            
    }        
  }        


// printf("vector R\n");
// print_vector(R,1,nu);

// printf("\nvector L - \n");
// print_vector(L,1,ne);



  free_vector(L,1,ne);
  free_vector(R,1,nu);
  free_matrix(A,1,ne,1,nu);


 
  return;
 
}







          



//void bndlxx(int nf,int npt,int *npntArray, int **ndxAdrrArray, 
//             PNTLST *pntAdrrArray, VECTOR *tmAdrrArray ,PNTLST xyz_p)  



void bndlxx(BundleData &bd,PNTLST cp)
{



  PNTLST tm = bd.plTMatrix;
  PNTLST cpx = bd.plCalcPnts;
  PNTLST *fp = bd.plPixels;
  PNTLST rp = bd.plSurvPnts;
  int    nf = bd.nNumPhotos;
  int    np = bd.nNumPoints;



   int i,j,k,it,flag;

  FLOAT c[7];
  int ck = 7;
  int ckk = 0;

  FLOAT temp,lmax;
  FLOAT c1=1.0e5;
  FLOAT c2=2.0e5;

 
  FLOAT *D;

  FLOAT *L;
  FLOAT *sLPT;

  FLOAT *R;
  FLOAT *RR;
  FLOAT *RPT;

  PNTLST A_p;
  MATRIX A;
  PNTLST AAT_p;
  MATRIX AAT;
  PNTLST APT_p;
  MATRIX APT;

  FLOAT dl2;

  PNTLST xyz = matrix_shell(1,1,1,3);
  
printf(" NUMBER OF FOTOS = %d\n",nf);
  int *npt_ft= (int *)myalloc(nf*sizeof(int));

  PNTLST *fps = (PNTLST *)myalloc( nf*sizeof(PNTLST));
  int **cps = (int **)myalloc( nf*sizeof(int*));

  for(i=0;i<nf;i++)npt_ft[i]=0;
  //**
  for(i=0;i<nf;i++)fps[i]=matrix_shell(1,1,1,3);
  //**
  for(i=0;i<nf;i++)cps[i]=(int *)myalloc(np*sizeof(int));
 
  int npt = 0;
  //**
  for(i=0;i<np;i++)
  {
    int count=0;
    for(j = 0;j< nf;j++)
    {
      if( (*(fp[j]))[i]!=NULL)
      {
        if( (*cp)[i] != NULL)count++;
      }
    }  	
    if(count>1)
    {
      npt++;
      if(npt > 1)add_matrix_shell_rows(xyz,1);
      (*xyz)[npt] = (*cp)[i];
 
      for(j=0;j<nf;j++)
      {
        if( (*(fp[j]))[i] != NULL)
	{
	  npt_ft[j]++;
	  if(npt_ft[j] > 1 )
	  {
	    add_matrix_shell_rows( fps[j],1);
	   // add_matrix_shell_rows( cps[j],1);
	  }  
	  (*(fps[j]))[npt_ft[j]] = (*(fp[j]))[i];
	  //(*(ps[j]))[npt_ft[j]] = (*xyz)[npt];
	  cps[j][npt_ft[j]-1]=npt;
	  
	}
      }	  
    }
  }  

 int nuf = 12;
  int nef = 9;
  int ps = nf*12+1;
 
  int nu = nuf*nf+ npt*3 - 12;

  int ne = nef*nf - 9;


  c[0]=0.9876;
  c[1]=1.0;
  c[2]=0.8666;
  c[3]=0.75;
  c[4]=0.9976;
  c[5]=0.8432;
  c[6]=0.975;

  for(i=0;i<nf;i++)
  { 
printf("num pnts for foto %d = %d\n",i,npt_ft[i]);
    ne += 3*npt_ft[i];
  }

  printf(" ne = %d\n",ne);

  for(i=0;i<nf;i++)
  {

    printf("tmatrix - %d\n\n",i);
    print_vector((*tm)[i],1,12);
  }


  L = vector(1,ne+1);
  sLPT = subvector(L,(nf*9)-8,ne+1,1);

  R = vector(1,nu+12);
  RR = subvector(R,13,nu+12,1);
  RPT = subvector(R,12*nf+1,nu+12,1);

  A_p = matrix(1,ne+1,1,nu);
  A = *A_p;
  AAT_p = submatrix(A_p,nf*9-8,ne, 1,nf*12-12, 1,1);
  AAT = *AAT_p;
  APT_p = submatrix(A_p,nf*9-8,ne, nf*12-11,nu, 1,1);
  APT = *APT_p;

//********************************************************


// calculate minimum x, y and z values for use as origin of xyz's

  VECTOR M = vector(1,3);
 for(i=1;i<=npt;i++)
 {
   for(j=1;j<=3;j++)
   {
     if(i<=1)
     {
       M[j] = (*xyz)[i][j]; 
     } 
     else
     {
       if((*xyz)[i][j]<M[j]) M[j]=(*xyz)[i][j];
     }
   }
 }



// adjust xyz's for calculation origin

 for(i=1;i<=npt;i++)
 {
   for(j=1;j<=3;j++)
   {
     (*xyz)[i][j] -= M[j];
   }
  }


// adjust camera locations for calculation origin
 for(i=0;i<nf;i++)
 {
   VECTOR ttm = (*tm)[i];
 
   VECTOR d = matpnt(ttm,M);

   (*tm)[i][10]=d[1];     
   (*tm)[i][11]=d[2];     
   (*tm)[i][12]=d[3];     

   free_vector(d,1,3);

  }






//*********************************************************

  dl2 = (*tm)[1][10]*(*tm)[1][10]
        + (*tm)[1][11]*(*tm)[1][11]
        + (*tm)[1][12]*(*tm)[1][12];

printf("******* D12 = %e *********\n",dl2);


//printf("ne = %d\n",ne);
//getchar();


//printf(" matrix xyz ");
//print_matrix(xyz,1,npt,1,3);


  /* intialize R */

   for(i=0;i<nf;i++)
   {
     FLOAT *r = subvector(R,12*i+1,12*i+12,1);      

     for(j=1;j<=12;j++)r[j]=(*tm)[i][j];

   //  printf("tm for photo %d\n",i);
   //  print_vector(r,1,12);

   }
   for(i=1;i<=npt;i++)
   {
     int s=(3*i)-2; 
     FLOAT *r = subvector(RPT,s,s+2,1);
   
     r[1] = (*xyz)[i][1];
     r[2] = (*xyz)[i][2];
     r[3] = (*xyz)[i][3];
   }

print_vector(R,1,nu);
    

  /* MAIN ITTERATION LOOP */
/****************************************************************/
  for(it = 0; it < 100; it++)
  {
    /* for each foto */
    int nps = 1;
    int npe = 0;
  
    for(i=0;i<nf;i++)
    {        
      int rs = i*12 +1 ;
      PNTLST AAF_p;
      FLOAT **AAF;
      PNTLST APF_p;
      FLOAT **APF;
      FLOAT *sLPF;

      FLOAT *RA = subvector(R,rs,rs+11,1);                      

      nps = npe + 1;   
      npe = nps + (3*npt_ft[i]) -1;
/*
printf("nps = %d, npe = %d\n",nps,npe);
*/
      if(i>0)
      {
        AAF_p = submatrix(AAT_p,nps,npe,(12*i)-11,(12*i),1,1);
        AAF = *AAF_p;
      }
      APF_p = submatrix(APT_p, nps,npe , 1,3*npt ,1,1);
      APF = *APF_p;
      sLPF = subvector(sLPT,nps,npe,1);

      if(i > 0)
      {
         int rs = i*9 -8;
         int cs = i*12-11;
         FLOAT *sL = subvector(L,rs,rs+8,1);
         FLOAT ***AAF_p = submatrix(A_p,rs,rs+8,cs,cs+8,1,1);  
         FLOAT **AAF = *AAF_p;

         forcesquare( sL, RA, AAF);
   
         free_submatrix(AAF_p,1,9,1,9);
      }


      if(i==1)
      {
        L[ne+1] = c1 * 
          (dl2 - RA[10]*RA[10]-RA[11]*RA[11]-RA[12]*RA[12]);
        A[ne+1][10] = c2*RA[10];
        A[ne+1][11] = c2*RA[11];
        A[ne+1][12] = c2*RA[12];
      }
/*   
printf("start pnt loop for %d points\n",npntArray[i]);
*/
      for(j=1;j<=npt_ft[i];j++)
      {
        PNT pnt = (*(fps[i]))[j];

        int rs = (cps[i][j-1])*3-2;
        FLOAT *RP = subvector(RPT,rs,rs+2,1);
         
        FLOAT *sL = subvector(sLPF,j*3-2,j*3,1);

        FLOAT ***AP_p = submatrix(APF_p,j*3-2,j*3,rs,rs+2,1,1);
        FLOAT **AP= *AP_p;
        FLOAT ***AA_p;
        FLOAT **AA;

        FLOAT x = RA[1]*RP[1] + RA[2]*RP[2] + RA[3]*RP[3]/* *RP[3]*/ + RA[10];
        FLOAT y = RA[4]*RP[1] + RA[5]*RP[2] + RA[6]*RP[3]/* *RP[3]*/ + RA[11];
        FLOAT z = RA[7]*RP[1] + RA[8]*RP[2] + RA[9]*RP[3]/* *RP[3]*/ + RA[12];
/*
printf("index rs = %d\n",rs);
*/
        if(i>0)
        {
          AA_p = submatrix(AAF_p,j*3-2,j*3,1,12,1,1);
          AA = *AA_p;
        }

        sL[1] = -pnt[2]*z + pnt[3]*y;
        sL[2] = -pnt[3]*x + pnt[1]*z;
        sL[3] = -pnt[1]*y + pnt[2]*x;
      
        if(i>0)
        {        
          AA[1][1] = 0.0;   
          AA[1][2] = 0.0;   
          AA[1][3] = 0.0;   
      

          AA[1][4] = -pnt[3]*RP[1];   
          AA[1][5] = -pnt[3]*RP[2];   
          AA[1][6] = -pnt[3]*RP[3] /**RP[3]*/;   

          AA[1][7] = pnt[2]*RP[1];   
          AA[1][8] = pnt[2]*RP[2];   
          AA[1][9] = +pnt[2]*RP[3] /**RP[3]*/;   

          AA[1][10] = 0.0;  
          AA[1][11] = -pnt[3];  
          AA[1][12] = pnt[2];  
        }
  
        AP[1][1] = -pnt[3]*RA[4] + pnt[2]*RA[7];
        AP[1][2] = -pnt[3]*RA[5] + pnt[2]*RA[8];
        AP[1][3] = (-pnt[3]*RA[6] + pnt[2]*RA[9]) /**(-2.0*RP[3])*/;


        if(i>0)
        {
          AA[2][1] = pnt[3]*RP[1];   
          AA[2][2] = pnt[3]*RP[2];   
          AA[2][3] = +pnt[3]*RP[3] /**RP[3]*/;   

          AA[2][4] = 0.0;   
          AA[2][5] = 0.0;   
          AA[2][6] = 0.0;   

          AA[2][7] = -pnt[1]*RP[1];   
          AA[2][8] = -pnt[1]*RP[2];   
          AA[2][9] = -pnt[1]*RP[3] /**RP[3]*/;   

          AA[2][10] = pnt[3];
          AA[2][11] = 0.0;
          AA[2][12] = -pnt[1];
        }

        AP[2][1] = pnt[3]*RA[1] - pnt[1]*RA[7];
        AP[2][2] = pnt[3]*RA[2] - pnt[1]*RA[8];
        AP[2][3] = (pnt[3]*RA[3] - pnt[1]*RA[9]) /* *(-2.0*RP[3])*/;


        if(i>0)
        {

          AA[3][1] = -pnt[2]*RP[1];   
          AA[3][2] = -pnt[2]*RP[2];   
          AA[3][3] = -pnt[2]*RP[3] /* *RP[3]*/;   

          AA[3][4] = pnt[1]*RP[1];   
          AA[3][5] = pnt[1]*RP[2];   
          AA[3][6] = +pnt[1]*RP[3] /* *RP[3]*/;   

          AA[3][7] = 0.0;
          AA[3][8] = 0.0;
          AA[3][9] = 0.0;

          AA[3][10] = -pnt[2];
          AA[3][11] = pnt[1];
          AA[3][12] = 0.0;
        }

        AP[3][1] = -pnt[2]*RA[1] + pnt[1]*RA[4];
        AP[3][2] = -pnt[2]*RA[2] + pnt[1]*RA[5];
        AP[3][3] = (-pnt[2]*RA[3] + pnt[1]*RA[6]) /* *(-2.0*RP[3])*/;
/*
print_matrix(AP,1,3,1,3);

printf("end pnt loop\n");
*/
        if(i>0)free_submatrix(AA_p,1,3,1,12);
        free_submatrix(AP_p,1,3,1,3);

      }     /* end point loop */
      if(i>0)free_submatrix(AAF_p,1,3*npt_ft[i],1,(nf-1)*12);
      free_submatrix(APF_p,1,3*npt_ft[i],1,3*npt);
    }   /* end foto loop */


/*
printf(" vector RR ");
print_vector(RR,1,nu);
    
printf(" vector L");
print_vector(L,1,ne+1);


printf("matrix A" );
print_matrix(A,1,ne+1,1,nu);
*/
   
    D=lsq(A_p,L,nu,ne+1);

/*
printf(" vector D ");
print_vector(D,1,nu);
*/
/*
for(i=1;i<=nu;i++)D[i] = 0.33*D[i];
    add_vector(RR,1,nu,D,1,nu);
*/
    for(i=1;i<=nu;i++)
    {

      FLOAT sf = 0.2;
      FLOAT temp =D[i];
      if(temp < 0.0)temp=-temp; 
      if(temp < 1.0e-3)sf=0.5;
      if(i==nu)
      {
        for(j=1;j<=nu;j++)
        {
          RR[j] +=  0.25*sf *c[ckk++]  *D[j];
          if(ckk>ck-1)ckk = 0;
        }
      }
    }


  lmax =0.0;
  temp = 0.0;
  for(i=nf*9-8;i<=ne;i++)
  {
    FLOAT tl;
    tl = L[i];
    if(L[i] < 0.0)tl = -tl;
    temp += tl;
    if(tl >lmax)lmax = tl;
  }
  temp =  temp/(i-(nf*9-8));

  printf(" AVerage L = %e, max = %e \n",temp,lmax);


    flag = 0;

    for(i=1;i<=nu;i++)
    {
      if(D[i]<0.0)D[i] = -D[i];
      if(D[i]>1.0e-7)
      {
        flag = 1;
      }
    }
    free_vector(D,1,npt);
    if(flag == 0)break;



  } /* end solution itteration */
/*******************************************************************/

//  for(i=0;i<=nf;i++)fps[i]=matrix_shell(1,1,1,3);
//  for(i=0;i<=nf;i++)cps[i]=(int *)myalloc(np*sizeof(int));
//**
    for(i=0;i<nf;i++)
    {
      free_matrix_shell(fps[i]);
      myfree((char *)cps[i]);
    }

    myfree((char *)fps);
    myfree((char *)cps);
    myfree((char *)npt_ft);


// PNTLST t=matrix(1,npt,1,3);

 for(i=1;i<=npt;i++)
 {
    int rs = i*3-2;
    FLOAT *RP = subvector(RPT,rs,rs+2,1);
    (*xyz)[i][1]=RP[1];
    (*xyz)[i][2]=RP[2];
    (*xyz)[i][3]=/* -RP[3]* */RP[3];
 }      



  for(i=0;i<nf;i++)
   {
     FLOAT *r = subvector(R,12*i+1,12*i+12,1);      

     for(j=1;j<=12;j++)(*tm)[i][j]=r[j];

    printf("tm for photo %d\n",i);
    print_vector(r,1,12);

   }
  




  free_vector(R,1,nu+12);
    
//printf(" vector L");
//print_vector(L,1,ne+1);

  free_vector(L,1,ne+1);

  free_matrix(A_p,1,ne+1,1,nu); 
  free_submatrix(AAT_p,1,1+ne-(nf*9-8),1,nf*12-12);
  free_submatrix(APT_p,1,1+ne-(nf*9-8),1,3*npt);

/*  srot(np, cp,xyz); */

//*************************************************************


 M[1]=-M[1];
 M[2]=-M[2];
 M[3]=-M[3];

 for(i=1;i<=npt;i++)
 {
   for(j=1;j<=3;j++)
   {
     (*xyz)[i][j] -= M[j];
   }
  }

 for(i=0;i<nf;i++)
 {
   VECTOR tmx = (*tm)[i];
 
   VECTOR d = matpnt(tmx,M);

   (*tm)[i][10]=d[1];     
   (*tm)[i][11]=d[2];     
   (*tm)[i][12]=d[3];     

   free_vector(d,1,3);

  }

  free_vector(M,1,3);
  free_matrix_shell(xyz);


printf("returnint from BNDLXX **************\n");
  return;

}



















FLOAT subbndl( int npp,PNTLST px2,PNTLST px1,PNTLST cp,VECTOR caa,int up)
{
   /* px1 and px2 are foto points */
   
   /* calculates Tmatrix from px1 or real world coordinate               */
   /* system to px2 system, returned through caa.                        */
   /* caa must be provided as input as approx. Tm for initial conditions */

   /* returns calculated 3d pnts in cp relative to px2 system */
   
   /* if up (use points flag) is set to 1, the points provided in cp will */
   /* be used to set initial conditions (points defined in px2 system)    */     

//printf("enter subbndl - %d\n",memcount(0));
   int np=npp;
 //  if(npp >= 6)np = 6;
 //  if(up)np=npp;

//printf("subbndl - caa\n");
//print_vector(caa,1,12);

//print_matrix(cp,1,np,1,3);

   int nu = 12;
   int ne = 13 + np;

   FLOAT fl = -65.124;
//   FLOAT fl = -65.4676;
   int bflag = 0;

   FLOAT    chisq = 0.0;
   FLOAT    oldchisq = 1.0e10;
   VECTOR   R =vector(1,12+4*npp);


   //COPY PROVIDED TRANSFORM INTO R
   copy_vector(R,1,12,caa,1,12);



   FLOAT   CD = 10.0*sqrt(R[10]*R[10] + R[11]*R[11] + R[12]*R[12]);


  if(up != 1)
  {
     PNTLST   A = matrix(1,ne,1,nu);
     VECTOR   L = vector(1,ne);

     VECTOR   a=vector(1,nu);
     VECTOR   w=vector(1,nu);
  
     PNTLST   u=matrix(1,ne,1,nu);
     PNTLST   v=matrix(1,nu,1,nu);

     VECTOR   sig = vector(1,ne);
     
     
/**
      int ti = 12;

      for(int i=1;i<=np;i++)
      {
        R[ti+1] = fl*(*px1)[i][1]/(*px1)[i][3];
        R[ti+2] = fl*(*px1)[i][2]/(*px1)[i][3];
        R[ti+3] = fl*(*px2)[i][1]/(*px2)[i][3];
        R[ti+4] = fl*(*px2)[i][2]/(*px2)[i][3];
       
        ti += 4;
      }
**/

    for( int kk=0;kk<40;kk++)
    {


//      forcesquare( L, R, *A,1.0);

        L[1] = R[1] - R[5]*R[9] + R[6]*R[8];
        L[2] = R[2] - R[6]*R[7] + R[4]*R[9];
        L[3] = R[3] - R[4]*R[8] + R[5]*R[7];


        L[10] = 1.0  - R[1]*R[1] - R[2]*R[2] - R[3]*R[3]; 
        L[11] = 1.0  - R[4]*R[4] - R[5]*R[5] - R[6]*R[6]; 
        L[12] = 1.0  - R[7]*R[7] - R[8]*R[8] - R[9]*R[9]; 

        (*A)[1][1] = -1.0;
        (*A)[1][5] =  R[9];
        (*A)[1][6] = -R[8];
        (*A)[1][8] =  R[6];
        (*A)[1][9] = -R[5];

        (*A)[2][2] = -1.0;
        (*A)[2][4] = -R[9];
        (*A)[2][6] =  R[7];
        (*A)[2][7] =  R[6];
        (*A)[2][9] = -R[4];

        (*A)[3][3] = -1.0;
        (*A)[3][4] =  R[8];
        (*A)[3][5] = -R[7];
        (*A)[3][7] = -R[5];
        (*A)[3][8] =  R[4];
     
        L[4] = R[4] - R[8]*R[3] + R[9]*R[2];
        L[5] = R[5] - R[9]*R[1] + R[7]*R[3];
        L[6] = R[6] - R[7]*R[2] + R[8]*R[1];

        (*A)[4][4] = -1.0;
        (*A)[4][2] = -R[9];
        (*A)[4][3] =  R[8];
        (*A)[4][8] =  R[3];
        (*A)[4][9] = -R[2];

        (*A)[5][5] = -1.0;
        (*A)[5][1] =  R[9];
        (*A)[5][3] = -R[7];
        (*A)[5][7] = -R[3];
        (*A)[5][9] =  R[1];

        (*A)[6][6] = -1.0;
        (*A)[6][1] = -R[8];
        (*A)[6][2] =  R[7];
        (*A)[6][7] =  R[2];
        (*A)[6][8] = -R[1];

        L[7] = R[7] - R[2]*R[6] + R[3]*R[5];
        L[8] = R[8] - R[3]*R[4] + R[1]*R[6];
        L[9] = R[9] - R[1]*R[5] + R[2]*R[4];

        (*A)[7][7] = -1.0;
        (*A)[7][2] =  R[6];
        (*A)[7][3] = -R[5];
        (*A)[7][5] = -R[3];
        (*A)[7][6] =  R[2];

        (*A)[8][8] = -1.0;
        (*A)[8][1] = -R[6];
        (*A)[8][3] =  R[4];
        (*A)[8][4] =  R[3];
        (*A)[8][6] = -R[1];
      
        (*A)[9][9] = -1.0;
        (*A)[9][1] =  R[5];
        (*A)[9][2] = -R[4];
        (*A)[9][4] = -R[2];
        (*A)[9][5] =  R[1];

        (*A)[10][1] = 2.0*R[1];
        (*A)[10][2] = 2.0*R[2];
        (*A)[10][3] = 2.0*R[3];

        (*A)[11][4] = 2.0*R[4];
        (*A)[11][5] = 2.0*R[5];
        (*A)[11][6] = 2.0*R[6];

        (*A)[12][7] = 2.0*R[7];
        (*A)[12][8] = 2.0*R[8];
        (*A)[12][9] = 2.0*R[9];

//      for(int i=1;i<=ne;i++) sig[i] = 0.1;
//      for( i=1;i<=12;i++) sig[i] = 75.0/(5.0*np); //.1/(5.0*np);
        for(int i=1;i<=13;i++)sig[i]=0.1/np;

//      sig[1]  = sig[2]  = sig[3]  = 0.01;
//      sig[4]  = sig[5]  = sig[6]  = 0.01;
//      sig[7]  = sig[8]  = sig[9]  = 0.01;
//      sig[10] = sig[11] = sig[12] = 0.01;


      FLOAT tmpsf = CD/sqrt(R[10]*R[10] + R[11]*R[11] + R[12]*R[12]);
      R[10] *= tmpsf;    
      R[11] *= tmpsf;    
      R[12] *= tmpsf;    



      L[13] = (CD * CD) - R[10]*R[10] - R[11]*R[11] - R[12]*R[12];


      (*A)[13][10] = 2.0*R[10];
      (*A)[13][11] = 2.0*R[11];
      (*A)[13][12] = 2.0*R[12];




      int e  = 14;
//          ti = 12;
      for( i=1;i<=np;i++)
      {

/**
        FLOAT px = R[1]*(*px1)[i][1] + R[2]*(*px1)[i][2] + R[3]*(*px1)[i][3];
        FLOAT py = R[4]*(*px1)[i][1] + R[5]*(*px1)[i][2] + R[6]*(*px1)[i][3];
        FLOAT pz = R[7]*(*px1)[i][1] + R[8]*(*px1)[i][2] + R[9]*(*px1)[i][3];

        FLOAT cx = (*px2)[i][2]*R[12] - (*px2)[i][3]*R[11];
        FLOAT cy = (*px2)[i][3]*R[10] - (*px2)[i][1]*R[12];
        FLOAT cz = (*px2)[i][1]*R[11] - (*px2)[i][2]*R[10];

        L[e] = px*cx + py*cy + pz*cz;

        (*A)[e][1] = -(*px1)[i][1]*cx;
        (*A)[e][2] = -(*px1)[i][2]*cx;
        (*A)[e][3] = -(*px1)[i][3]*cx;

        (*A)[e][4] = -(*px1)[i][1]*cy;
        (*A)[e][5] = -(*px1)[i][2]*cy;
        (*A)[e][6] = -(*px1)[i][3]*cy;
  
        (*A)[e][7] = -(*px1)[i][1]*cz;
        (*A)[e][8] = -(*px1)[i][2]*cz;
        (*A)[e][9] = -(*px1)[i][3]*cz;
  
        (*A)[e][10] = -(*px2)[i][3]*py + (*px2)[i][2]*pz;
        (*A)[e][11] = -(*px2)[i][1]*pz + (*px2)[i][3]*px;
        (*A)[e][12] = -(*px2)[i][2]*px + (*px2)[i][1]*py;

        e += 1;
**/



/**
        FLOAT px = R[1]*R[ti+1] + R[2]*R[ti+2] + R[3]*fl;
        FLOAT py = R[4]*R[ti+1] + R[5]*R[ti+2] + R[6]*fl;
        FLOAT pz = R[7]*R[ti+1] + R[8]*R[ti+2] + R[9]*fl;

        FLOAT cx = R[ti+4]*R[12] - fl*R[11];
        FLOAT cy = fl*R[10] - R[ti+3]*R[12];
        FLOAT cz = R[ti+3]*R[11] - R[ti+4]*R[10];

        L[e] = px*cx + py*cy + pz*cz;

        (*A)[e][1] = -R[ti+1]*cx;
        (*A)[e][2] = -R[ti+2]*cx;
        (*A)[e][3] = -fl*cx;
        (*A)[e][4] = -R[ti+1]*cy;
        (*A)[e][5] = -R[ti+2]*cy;
        (*A)[e][6] = -fl*cy;
        (*A)[e][7] = -R[ti+1]*cz;
        (*A)[e][8] = -R[ti+2]*cz;
        (*A)[e][9] = -fl*cz;

        (*A)[e][10] = -fl*py + R[ti+4]*pz;
        (*A)[e][11] = -R[ti+3]*pz + fl*px;
        (*A)[e][12] = -R[ti+4]*px + R[ti+3]*py;

        (*A)[e][ti+1] = -R[1]*cx - R[4]*cy - R[7]*cz;
        (*A)[e][ti+2] = -R[2]*cx - R[5]*cy - R[8]*cz;
        (*A)[e][ti+3] = -R[11]*pz + R[12]*py;
        (*A)[e][ti+4] = -R[12]*px + R[10]*pz;

        L[e+1] = fl*((*px1)[i][1]/(*px1)[i][3]) - R[ti+1];
        L[e+2] = fl*((*px1)[i][2]/(*px1)[i][3]) - R[ti+2];
        L[e+3] = fl*((*px2)[i][1]/(*px2)[i][3]) - R[ti+3];
        L[e+4] = fl*((*px2)[i][2]/(*px2)[i][3]) - R[ti+4];
  
        (*A)[e+1][ti+1] = 1.0;
        (*A)[e+2][ti+2] = 1.0;
        (*A)[e+3][ti+3] = 1.0;
        (*A)[e+4][ti+4] = 1.0;


        sig[e] = 0.0001/4.0; //4.0;
        sig[e+1] = sig[e+2] = sig[e+3] = sig[e+4] = 10.0;

        e += 5;
 
        ti += 4;
**/
        FLOAT x1 = (*px1)[i][1];
        FLOAT y1 = (*px1)[i][2];
        FLOAT z1 = (*px1)[i][3];
        FLOAT x2 = (*px2)[i][1];
        FLOAT y2 = (*px2)[i][2];
        FLOAT z2 = (*px2)[i][3];

        FLOAT px = R[1]*x1 + R[2]*y1 + R[3]*z1;
        FLOAT py = R[4]*x1 + R[5]*y1 + R[6]*z1;
        FLOAT pz = R[7]*x1 + R[8]*y1 + R[9]*z1;

        FLOAT cx = y2*R[12] - z2*R[11];
        FLOAT cy = z2*R[10] - x2*R[12];
        FLOAT cz = x2*R[11] - y2*R[10];

        L[e] = px*cx + py*cy + pz*cz;

        (*A)[e][1] = -x1*cx;
        (*A)[e][2] = -y1*cx;
        (*A)[e][3] = -z1*cx;
        (*A)[e][4] = -x1*cy;
        (*A)[e][5] = -y1*cy;
        (*A)[e][6] = -z1*cy;
        (*A)[e][7] = -x1*cz;
        (*A)[e][8] = -y1*cz;
        (*A)[e][9] = -z1*cz;

        (*A)[e][10] = -z2*py + y2*pz;
        (*A)[e][11] = -x2*pz + z2*px;
        (*A)[e][12] = -y2*px + x2*py;



        sig[e] =1.0;
       // sig[e] = 0.0001/4.0; //4.0;
        //sig[e+1] = sig[e+2] = sig[e+3] = sig[e+4] = 10.0;

        e += 1;
 
        //ti += 4;

      }

      svdfit(A,L,sig,ne,a,nu,u,v,w,&chisq,NULL);
  //printf("vector w");
  //print_vector(w,1,nu);
  //Output(0,"%d xxx chisq = %e\n",kk,chisq);
    if(fabs(oldchisq - chisq) < oldchisq*1.0e-7)
    {
      bflag += 1;
    }
    else
    {
      bflag = 0;
    }
    if(bflag >= 2)break;
    oldchisq = chisq;


      for(i=1;i<=nu;i++)R[i] += a[i];

    
    }

    //CHECK FOR RIGHT HAND SYSTEM
    FLOAT zx = R[2]*R[6] - R[3]*R[5];
    FLOAT zy = R[3]*R[4] - R[1]*R[6];
    FLOAT zz = R[1]*R[5] - R[2]*R[4];

    FLOAT iv = sqrt( (R[7]-zx)*(R[7]-zx) + (R[8]-zy)*(R[8]-zy) + (R[9]-zz)*(R[9]-zz) );

    //printf(" RH value = %e\n",iv);

    if(iv > 0.5)  //NOT A RIGHT HAND SYSTEM, SET CHISQ HIGH TO CAUSE REJECTION
    {
//printf("   ++++++ not square ++++\n");
       for(int i=1;i<=9;i++)R[i] = -R[i];
    }

    if(R[5] < 0.0)chisq = 1.0e5;

   // printf("vector Rfrom first subbndl");
   // print_vector(R,1,nu);
   // printf("solution - L\n");
   // print_vector(L,1,ne);


    free_vector(L,1,ne);
    free_vector(sig,1,ne);
    free_vector(a,1,nu);
    free_vector(w,1,nu);

    free_matrix(u,1,ne,1,nu);
    free_matrix(v,1,nu,1,nu);

    
  }












   np = npp;




    ne =10 + 4*np;
    nu = 12 + (3*np);

   //R =vector(1,nu);

    CD = 0.0;

   //chisq = 0.0;

//printf("bndl ne = %d, np = %d\n",ne,np);

    PNTLST A = matrix(1,ne,1,nu);
    VECTOR L = vector(1,ne);

    VECTOR a=vector(1,nu);
    VECTOR w=vector(1,nu);

    PNTLST u=matrix(1,ne,1,nu);
    PNTLST v=matrix(1,nu,1,nu);

    VECTOR sig = vector(1,ne);


   /* initial Results vector with Tmatrix provided in caa */
   /* Tmatrix - first 12 elements of R */
   //  copy_vector(R,1,12,caa,1,12);
//printf("\n caa");
//print_vector( caa,1,12);

    CD = sqrt(R[10]*R[10] + R[11]*R[11] + R[12]*R[12]);
//printf("scale factor for 1500 = %e\n",sf);



  
    if(up == 1) // use points flag - if set use points passed in cp for initial conditions
    { 
      
      int p;
      int kk=12; 
      for(p=1;p<= np;p++)
      {
        R[kk+1] = (*cp)[p][1];
        R[kk+2] = (*cp)[p][2];
        R[kk+3] = (*cp)[p][3];

        kk += 3;
      }

    }
    else
    {
       PNT pnt;
       PNTLST pnts=matrix(1,2,1,3);
       PNTLST tmxs=matrix(1,2,1,13);

       for(int ii = 1;ii<=12;ii++)
       {
         (*tmxs)[1][ii] = 0.0;
         (*tmxs)[2][ii] = R[ii];
         (*tmxs)[1][1] = 1.0;
         (*tmxs)[1][5] = 1.0;
         (*tmxs)[1][9] = 1.0;
       }

      int kk = 12;
      for(int p = 1;p<=np;p++)
      {
       
        for(int ii=1;ii<=3;ii++)
        {
           (*pnts)[1][ii] = (*px1)[p][ii];
           (*pnts)[2][ii] = (*px2)[p][ii];
        }
           
        // PNT get3dpnt_np(int nf,PNTLST pnts, FLOAT **tmxs )

        pnt = get3dpnt_np(2,pnts,*tmxs);
//printf("pnts = ");
//print_vector(pnt,1,3);
        R[kk+1] = (*cp)[p][1] = pnt[1];
        R[kk+2] = (*cp)[p][2] = pnt[2];
        R[kk+3] = (*cp)[p][3] = pnt[3];
        FLOAT tmpz =R[7]*pnt[1] + R[8]*pnt[2] + R[9]*pnt[3] + R[12];
//printf(" z = %e\n",tmpz);

        FLOAT s1=1;
        FLOAT s2=1;
        if(p==1)
        {
          if( pnt[3] > 0.0)s1= -1.0;
          if( tmpz > 0.0)s2= -1.0;
        }
        else
        {
          if(s1*pnt[3] > 0.0)chisq = 1.0e5;
          if(s1*tmpz > 0.0)chisq = 1.0e5;
        }
        kk += 3;
      
        

      }

      free_matrix(pnts,1,2,1,3);
      free_matrix(tmxs,1,2,1,13);
      free_vector(pnt,1,3);
printf(" chisq = %e\n",chisq); 

      for(p=1;p<=12;p++)caa[p] = R[p];
      return(chisq);
      
    }



//FLOAT ( int np, PNTLST p, PNTLST rp, VECTOR RR, int fs )
  for(int i = 1;i<=2;i++)
  {
printf("start refinement\n");
    VECTOR RR = subvector(R,1,12,1);
    PNTLST rp = matrix(1,np,1,3);
    for(int p=1;p<=np;p++)
    {
      (*rp)[p][1] = R[9 + 3*p +1];
      (*rp)[p][2] = R[9 + 3*p +2];
      (*rp)[p][3] = R[9 + 3*p +3];
    }

    getcamera3(np,px2,rp,RR,1);


    FLOAT tmpsf = CD/sqrt(R[10]*R[10] + R[11]*R[11] + R[12]*R[12]);
    R[10] *= tmpsf;    
    R[11] *= tmpsf;    
    R[12] *= tmpsf;    
    

       PNT pnt;
       PNTLST pnts=matrix(1,2,1,3);
       PNTLST tmxs=matrix(1,2,1,13);

       for(int ii = 1;ii<=12;ii++)
       {
         (*tmxs)[1][ii] = 0.0;
         (*tmxs)[2][ii] = R[ii];
         (*tmxs)[1][1] = 1.0;
         (*tmxs)[1][5] = 1.0;
         (*tmxs)[1][9] = 1.0;
       }

      int kk = 12;
      for( p = 1;p<=np;p++)
      {
       
        for(int ii=1;ii<=3;ii++)
        {
           (*pnts)[1][ii] = (*px1)[p][ii];
           (*pnts)[2][ii] = (*px2)[p][ii];
        }
           
        // PNT get3dpnt_np(int nf,PNTLST pnts, FLOAT **tmxs )
//printf("pnts = ");

        pnt = get3dpnt_np(2,pnts,*tmxs);
//print_vector(pnt,1,3);
        //R[kk+1] = ((*px1)[p][1]*pnt[1] + (*px1)[p][2]*pnt[2] + (*px1)[p][3]*pnt[3])*(*px1)[p][1];
        //R[kk+2] = ((*px1)[p][1]*pnt[1] + (*px1)[p][2]*pnt[2] + (*px1)[p][3]*pnt[3])*(*px1)[p][2];
        //R[kk+3] = ((*px1)[p][1]*pnt[1] + (*px1)[p][2]*pnt[2] + (*px1)[p][3]*pnt[3])*(*px1)[p][3];

        R[kk+1] = pnt[1];
        R[kk+2] = pnt[2];
        R[kk+3] = pnt[3];

        if(R[kk+3] > 0)
        {
          R[kk+1] *= -1;
          R[kk+2] *= -1;
          R[kk+3] *= -1;
printf("positive z coor.\n");
        }


        kk += 3;
      
      }  

      

      free_matrix(pnts,1,2,1,3);
      free_matrix(tmxs,1,2,1,13);
      free_vector(pnt,1,3);
printf("end refinement\n");
      
   } 




    FLOAT tmpsf = CD/sqrt(R[10]*R[10] + R[11]*R[11] + R[12]*R[12]);
    R[10] *= tmpsf;    
    R[11] *= tmpsf;    
    R[12] *= tmpsf;  

    for(i=13;i<=12+3*np;i++)R[i] *= tmpsf;  

  sig = vector(1,ne);
  FLOAT osig = 0.1;
 // if(fs) osig = 0.001;
  FLOAT osigs = 0.1;
 // if(fs)osigs = 0.001;



    
  //printf("second solutin\n");
/****
  for(int kk=0;kk<40;kk++)
  {

    // R[10],R[11],R[12] - vector from c1 orgin to c2 orgin  
    // this equation use to constrain distance between camera orgins 

    L[10] = (CD * CD) - R[10]*R[10] - R[11]*R[11] - R[12]*R[12];


    (*A)[10][10] = 2.0*R[10];
    (*A)[10][11] = 2.0*R[11];
    (*A)[10][12] = 2.0*R[12];

    int e =10;
    int ti = 12;


    forcesquare( L, R, *A,1.0);
    for(int i=1;i<=10;i++) sig[i] = .01/(4.0*np);
    sig[10] = 0.001/(4.0*np);
    //sig[1] = sig[2] = sig[3] = 0.001;
    //sig[4] = sig[5] = sig[6] = 0.001;


    for(int m=1;m<=np;m++)
    {

      L[e+1] = fl*( (*px1)[m][1]/(*px1)[m][3] ) - fl*( R[ti+1]/R[ti+3] );
      L[e+2] = fl*( (*px1)[m][2]/(*px1)[m][3] ) - fl*( R[ti+2]/R[ti+3] );

      (*A)[e+1][ti+1] = fl/R[ti+3];
      (*A)[e+1][ti+3] = -fl*(R[ti+1]/(R[ti+3]*R[ti+3]));

      (*A)[e+2][ti+2] = fl/R[ti+3];
      (*A)[e+2][ti+3] = -fl*(R[ti+2]/(R[ti+3]*R[ti+3]));



      sig[e+1] = sig[e+2] = sig[e+3] = sig[e+4] = 1.0; //0.0127/3.0;

      FLOAT x = R[1]*R[ti+1] + R[2]*R[ti+2] + R[3]*R[ti+3] + R[10];  
      FLOAT y = R[4]*R[ti+1] + R[5]*R[ti+2] + R[6]*R[ti+3] + R[11];  
      FLOAT z = R[7]*R[ti+1] + R[8]*R[ti+2] + R[9]*R[ti+3] + R[12];  

      L[e+3] = fl*((*px2)[m][1]/(*px2)[m][3]) - fl*(x/z);
      L[e+4] = fl*((*px2)[m][2]/(*px2)[m][3]) - fl*(y/z);

      (*A)[e+3][ti+1] = (fl/z)*R[1] - (fl*(x/(z*z)))*R[7];
      (*A)[e+3][ti+2] = (fl/z)*R[2] - (fl*(x/(z*z)))*R[8];
      (*A)[e+3][ti+3] = (fl/z)*R[3] - (fl*(x/(z*z)))*R[9];

      (*A)[e+3][1] = (fl/z)*R[ti+1];
      (*A)[e+3][2] = (fl/z)*R[ti+2];
      (*A)[e+3][3] = (fl/z)*R[ti+3];
      (*A)[e+3][10]= (fl/z);
      
      (*A)[e+3][7] = -(fl*(x/(z*z)))*R[ti+1];
      (*A)[e+3][8] = -(fl*(x/(z*z)))*R[ti+2];
      (*A)[e+3][9] = -(fl*(x/(z*z)))*R[ti+3];
      (*A)[e+3][12]= -(fl*(x/(z*z)));
      

      (*A)[e+4][ti+1] = (fl/z)*R[4] - (fl*(y/(z*z)))*R[7];
      (*A)[e+4][ti+2] = (fl/z)*R[5] - (fl*(y/(z*z)))*R[8];
      (*A)[e+4][ti+3] = (fl/z)*R[6] - (fl*(y/(z*z)))*R[9];

      (*A)[e+4][4] = (fl/z)*R[ti+1];
      (*A)[e+4][5] = (fl/z)*R[ti+2];
      (*A)[e+4][6] = (fl/z)*R[ti+3];
      (*A)[e+4][11]= (fl/z);


      (*A)[e+4][7] = -(fl*(y/(z*z)))*R[ti+1];
      (*A)[e+4][8] = -(fl*(y/(z*z)))*R[ti+2];
      (*A)[e+4][9] = -(fl*(y/(z*z)))*R[ti+3];
      (*A)[e+4][12]= -(fl*(y/(z*z)));
    
      e +=  4;
      ti += 3;

    }

//printf("vector L - getcamera\n");
//print_vector(L,1,ne);

//printf("matrix A");
//print_matrix(A,1,ne,1,nu);

//printf("+++++++++++++ okkkk =  ++++++++++++++\n");
//        printf("xxxxxx calling svdfit xxxxxxxx\n");

      
        svdfit(A,L,sig,ne,a,nu,u,v,w,&chisq,NULL);
//printf("vector w");
//print_vector(w,1,nu);
//printf("vector a");
//print_vector(a,1,nu);
Output(0,"%d 2nd solution running chisq = %e\n",kk,chisq);

    if(fabs(oldchisq - chisq) < oldchisq*1.0e-10)
    {
      bflag += 1;
    }
    else
    {
      bflag = 0;
    }
    if(bflag >= 2)break;
    oldchisq = chisq;


    for(i=1;i<=nu;i++)R[i] += a[i];


  }
*****/
  printf("chisq = %e\n",chisq);  
   // printf("vector Rfrom subbndl");
    //print_vector(R,1,nu);
    //printf("solution - L\n");
   // print_vector(L,1,ne);



      free_vector(L,1,ne);
      free_vector(sig,1,ne);
      free_vector(a,1,nu);
      free_vector(w,1,nu);

      free_matrix(u,1,ne,1,nu);
      free_matrix(v,1,nu,1,nu);








// ****** return Tmatrix in ca *************


//  for(i=1;i<=12;i++)(*caa)[i]=R[i];

    copy_vector(caa,1,12,R,1,12);




// printf(" matrix xyz from bndl \n");
//print_matrix(cp,1,np,1,3);
     




// printf("exit subbndl - %d\n",memcount(0));

  return(chisq);

}




void bndl( int np,PNTLST px1,PNTLST px2,PNTLST cpp,VECTOR caa, int up)
{

// printf("enter bndl - %d\n",memcount(0));

   if(up)    //use cpp and caa as initial parameters (
   {
     printf("    Ray Intersection  ( miss distance in mm's )\n");

     printf("              Average       Maximum\n");
     printf(" final   -  ");
      subbndl(np,px1,px2,cpp,caa,1);

    // printf("caa vector");
    // print_vector(caa,1,12);

       printf("\n\nTransfer Matrix - Master to Photo\n\n");
       display_tm(caa);

       return;
    }


    FLOAT l1,l2,l3,l4,l5,l6;
    PNTLST cp1=matrix(1,np,1,3);
    PNTLST cp2=matrix(1,np,1,3); 
    PNTLST cp3=matrix(1,np,1,3); 
    PNTLST cp4=matrix(1,np,1,3); 
    PNTLST cp5=matrix(1,np,1,3); 
    PNTLST cp6=matrix(1,np,1,3); 

    VECTOR r1 = vector(1,13);
    VECTOR r2 = vector(1,13);
    VECTOR r3 = vector(1,13);
    VECTOR r4 = vector(1,13);
    VECTOR r5 = vector(1,13);
    VECTOR r6 = vector(1,13);

    FLOAT sq2 = 1.0/sqrt(2.0);

    r1[1] = r2[1] = r3[1] = r4[1] = r5[1] = r6[1] = 1.0;
    r1[5] = r2[5] = r3[5] = r4[5] = r5[1] = r6[1] = 1.0;
    r1[9] = r2[9] = r3[9] = r4[9] = r5[1] = r6[1] = 1.0;

    r1[10] = 1500.0;
    r1[1] = r1[9] = 0;
    r1[3] = sq2;
    r1[7] = -sq2;
 
    r2[10] = -1500.0;
    r2[1] = r2[9] = 0;
    r2[3] = -sq2;
    r2[7] = sq2;


    r3[12] = 1500.0;

    r4[12] = -1500.0;

    r5[11] = 1500.0;

    r6[11] = -1500.0;


    printf("    Ray Intersection  ( miss distance in mm's )\n");

    int npp =np;
    //if(np >= 6)npp = 6;
    printf("              Average       Maximum\n");
    printf("  left   -  ");
    l1 = subbndl(npp,px1,px2,cp1,r1,0);
    printf("  right  -  ");
    l2 = subbndl(npp,px1,px2,cp2,r2,0);
    printf("  front  -  ");
    l3 = subbndl(npp,px1,px2,cp3,r3,0);
    printf("  back   -  ");
    l4 = subbndl(npp,px1,px2,cp4,r4,0);
    printf("  up     -  ");
    l5 = subbndl(npp,px1,px2,cp5,r5,0);
    printf("  down   -  ");
    l6 = subbndl(npp,px1,px2,cp6,r6,0);


//    printf(" left,right = %e,  %e \n",l1,l2);
//    printf("front,back  = %e,  %e \n",l3,l4);
//    printf("up,dpwn  = %e,  %e \n",l5,l6);

    if(l3 < l1)
    {
      l1 = l3;
      free_vector(r1,1,13);
      r1 = NULL;
      free_matrix(cp1,1,np,1,3);
      cp1 = NULL;
      l1 = l3;
      r1 = r3;
      cp1 = cp3;
    }
    else
    {
      free_vector(r3,1,3);
      
      free_matrix(cp3,1,np,1,3);
    }


    if(l5 < l1)
    {
      l1 = l5;
      free_vector(r1,1,13);
      r1 = NULL;
      free_matrix(cp1,1,np,1,3);
      cp1 = NULL;
      l1 = l5;
      r1 = r5;
      cp1 = cp5;
    }
    else
    {
      free_vector(r5,1,3);
      
      free_matrix(cp5,1,np,1,3);
    }





    if(l4 < l2)
    {
      l2 = l4;
      free_vector(r2,1,13);
      free_matrix(cp2,1,np,1,3);
      l2 = l4;
      r2 = r4;
      cp2 = cp4;
    }
    else
    {
      free_vector(r4,1,13);
      free_matrix(cp4,1,np,1,3);
    } 



    if(l6 < l2)
    {
      l2 = l6;
      free_vector(r2,1,13);
      free_matrix(cp2,1,np,1,3);
      l2 = l6;
      r2 = r6;
      cp2 = cp6;
    }
    else
    {
      free_vector(r6,1,13);
      free_matrix(cp6,1,np,1,3);
    } 





    if(l1 < l2)
    {
      //*caa = r1;
      copy_vector(caa,1,12,r1,1,12);
      copy_matrix(cpp,1,np,1,3,cp1,1,np,1,3);

    }
    else
    {
     // *caa = r2;
      copy_vector(caa,1,12,r2,1,12);
      copy_matrix(cpp,1,np,1,3,cp2,1,np,1,3);
    }
    free_matrix(cp1,1,np,1,3);
    free_matrix(cp2,1,np,1,3);
    free_vector(r1,1,13);
    free_vector(r2,1,13);

 
    printf("\n  final -  ");

  subbndl(np,px1,px2,cpp,caa,1);

// printf("caa vector");
// print_vector(caa,1,12);

printf("\n\nTransfer Matrix - Master to Photo\n\n");
display_tm(caa);

// printf("exit bndl - %d\n",memcount(0)); 

     
  return;
}




void loopbundle(BundleData *bd, int incflg) 
{
  int i,j,k;


  PNTLST ptm = bd->plTMatrix;
  PNTLST cp = bd->plCalcPnts;
  PNTLST *pfp = bd->plPixels;
  PNTLST rp = bd->plSurvPnts;
  int    nf = bd->nNumPhotos;
  int    npt = bd->nNumPoints;


  int maxloops = 1;

 for(int ii = 1; ii<=maxloops;ii++)
{
printf("----- start loopbundle %d --------%d \n",ii,memcount(0));
//getchar();
 // get calculated points by 
/***
      printf("\n\n     Get 3D Point Errors\n\n");
      printf("           ");
      for(int ik = 0;ik<nf;ik++)printf("     Photo #%d   ",ik);
      printf("\n\n");
***/

  for(i=0;i<npt;i++)  //for each point in bundle
  {
   
    PNTLST fp = matrix_shell(1,1,1,3);
    PNTLST tm = matrix_shell(1,1,1,12);
    VECTOR inphoto = vector(0,nf-1);

    int n=0;

    
    // get 2D points from each photo for this 3D point

    for(j=0;j<nf;j++)  // loop for each foto in bundle
    {
       inphoto[j] = 0.0;
       if( (*(pfp[j]))[i] != NULL)  // if a pixel point provide for this point
               
        if( (*ptm)[j] != NULL)
        {
          n++;
          inphoto[j] = 1.0;
          if(n > 1)
          {
            add_matrix_shell_rows(fp,1); 
            add_matrix_shell_rows(tm,1);
          }
      
          (*fp)[n] = (*(pfp[j]))[i];
          (*tm)[n] = (*ptm)[j];
        }
     }



     //printf(" getting 3d point for pnt %d, num photos = %d\n",i,n);
 
    if(n > 1) // must have 2d point from at least 2 photos to calculate 3d point
    {

      //print_matrix(fp,1,n,1,3);


      PNTLST error = matrix(1,n,1,3);
      VECTOR dp =get3dpnt_e( n, fp, *tm, error );  //calculate 3d point


      // put point in calculated point list
      if((*cp)[i] == NULL)
      {
        //(*trgpnts)[i] = clone_vector((*sp)[nsp],1,3,1,3);
        //   printf("create cp point\n");
        (*cp)[i] = vector(1,3);
      }
        // (*isp)[nsp] = (*cp)[i];
        // (*tp)[nsp] = (*cp)[i];
      copy_vector((*cp)[i],1,3,dp,1,3);
      free_vector(dp,1,3);
      // printf("new cp from getpoint = %e,%e,%e\n",dp[1],dp[2],dp[3]);

      // Display get3dpnt errors

/****      
      for(int ix = 1; ix<=3; ix++)
      {
        if(ix == 1)
        {
          printf("Pnt[%d]   ",i);
          if(i < 10)printf(" ");
          if(i < 100)printf(" ");
        }
        else
        {
          printf("           ");
        }
        int eix = 0;
        for(int inf = 0; inf < nf; inf++)
        {
          if(inphoto[inf] == 1.0)
          {
             if((*error)[++eix][ix] > 0.0)printf(" ");
             printf(" %e",(*error)[eix][ix]);

             if(fabsf((*error)[eix][ix]) >= 1.0)
             {
               printf("*");
             }
             else
             {
               printf(" ");
             }
             if(fabsf((*error)[eix][ix]) >= 10.0)
             {
               printf("*");
             }
             else
             {
               printf(" ");
             }
          }
          else
          {
             printf("    ............");
          }
        }
        printf("\n");
      }
      printf("\n");
****/
      free_matrix(error,1,n,1,3);
    }

    free_matrix_shell(fp);
    free_matrix_shell(tm);    
    free_vector(inphoto,0,nf-1);

  }


/*******************************************************




//VECTOR torealsys(int np, PNTLST cp_p, PNTLST mp_p, int npc, PNTLST xyz_p ,VECTOR tm)

  VECTOR ttm = torealsys(nsp, sp, tp, nsp, sp, NULL,itm);

//printf( " R,isp,sp\n");
//print_vector(ttm,1,13);
//  print_matrix(isp,1,nsp,1,3);
//  print_matrix(sp,1,nsp,1,3);

if(ttm != NULL)
{
  for(i = 0;i<nf;i++)
  {

    (*ptm)[i][10] *= ttm[13];
    (*ptm)[i][11] *= ttm[13];
    (*ptm)[i][12] *= ttm[13];

    VECTOR ntm = matmat((*ptm)[i],ttm);
//    free_vector((*ptm)[i],1,12);
//    (*ptm)[i] = ntm;

    copy_vector( (*ptm)[i],1,12,ntm,1,12);
    free_vector(ntm,1,12);
  }
  free_vector(ttm,1,13);
}

//##########
 PNTLST trcp = matrix(1,nsp,1,4);
 copy_matrix(trcp,1,nsp,1,3,sp,1,nsp,1,3);
  for(i=1;i<=nsp;i++)(*trcp)[i][4] = 1.0;



 PNTLST trwp = matrix(1,nsp,1,4);
 copy_matrix(trwp,1,nsp,1,3,tp,1,nsp,1,3);
 for(i=1;i<=nsp;i++)(*trwp)[i][4] = 1.0;

//PNTLST LUK(int nrk, int nck, int nre, PNTLST teq, PNTLST tkn)



  PNTLST mat = LUK(4,nsp,3,trcp,trwp);

//printf("calculated real world\n");
// print_matrix(trcp,1,ntp,1,3);
//printf("actual real world\n");
// print_matrix(trwp,1,ntp,1,3);

  if(mat != NULL)
  {



    VECTOR tmat = vector(1,13);

    tmat[1] = (*mat)[1][1];
    tmat[2] = (*mat)[1][2];
    tmat[3] = (*mat)[1][3];
    tmat[4] = (*mat)[2][1];
    tmat[5] = (*mat)[2][2];
    tmat[6] = (*mat)[2][3];
    tmat[7] = (*mat)[3][1];
    tmat[8] = (*mat)[3][2];
    tmat[9] = (*mat)[3][3];
    tmat[10] = (*mat)[1][4];
    tmat[11] = (*mat)[2][4];
    tmat[12] = (*mat)[3][4];
    tmat[13] = 1.0;

    for(i=0;i<nf;i++)
    {


      VECTOR t = matmat( (*ptm)[i],tmat);

      //copy_vector((*ptm)[i],1,12,t,1,12);

      free_vector(t,1,13);
    } 


  printf("LUK from realworld");
  print_matrix(mat,1,3,1,3);
 VECTOR  qu = findscale( tmat);
 printf("find scale vector\n");
  qu[10] = qu[10]/qu[12];
  qu[11] = qu[11]/qu[12];
print_vector(qu,1,12);
 free_vector(qu,1,12);

  free_vector(tmat,1,13);

  free_matrix(mat,1,3,1,4);

  }

//############


//PNTLST LUK(int nrk, int nck, int nre, PNTLST teq, PNTLST tkn)



   mat = LUK(4,nsp,3,isp,trcp);

//printf("calculated real world\n");
// print_matrix(trcp,1,ntp,1,3);
//printf("actual real world\n");
// print_matrix(trwp,1,ntp,1,3);

  if(mat != NULL)
  {



    VECTOR tmat = vector(1,13);

    tmat[1] = (*mat)[1][1];
    tmat[2] = (*mat)[1][2];
    tmat[3] = (*mat)[1][3];
    tmat[4] = (*mat)[2][1];
    tmat[5] = (*mat)[2][2];
    tmat[6] = (*mat)[2][3];
    tmat[7] = (*mat)[3][1];
    tmat[8] = (*mat)[3][2];
    tmat[9] = (*mat)[3][3];
    tmat[10] = (*mat)[1][4];
    tmat[11] = (*mat)[2][4];
    tmat[12] = (*mat)[3][4];
    tmat[13] = 1.0;



  for(i=1;i<= nsp;i++)
  {
    PNT pnt = matpnt(tmat,(*trcp)[i]);

 //   printf(" %e,  %e  %e \n",pnt[1],pnt[2],pnt[3]);  
 //   printf(" %e,  %e  %e \n",(*trwp)[i][1],(*trwp)[i][2],(*trwp)[i][3]); 
 //   copy_vector( (*sp)[i],1,3,pnt,1,3);
    free_vector(pnt,1,3); 
  
  }



  free_vector(tmat,1,13);

  free_matrix(mat,1,3,1,4);
  copy_matrix(isp,1,nsp,1,3,sp,1,nsp,1,3);

  }


 free_matrix(trcp,1,nsp,1,4); 
 free_matrix(trwp,1,nsp,1,4); 

**********************************************************/
 // copy_matrix(isp,1,nsp,1,3,sp,1,nsp,1,3);
 // free_matrix_shell(isp);
 // free_matrix_shell(tp);
 // free_matrix(sp,1,nsp,1,3);

//******************************************************************

//printf("test point 1\n");

if(incflg == 0 )
{
  if(ii < maxloops)
  {
//printf("ii = %d,  maxloops = %d\n",ii,maxloops);
    for(i = 0; i< nf; i++) // loop over photos
    {
//printf("i = %d, nf = %d\n",i,nf);

      PNTLST rp=matrix_shell(1,1,1,3);
      PNTLST pp=matrix_shell(1,1,1,3);
      int n = 0;

      for(j=0;j<npt;j++) // loop over points
      {
        if( (*cp)[j] != NULL)
        { 
          if( (*(pfp[i]))[j] != NULL)
          {
            n++;
            if(n > 1)
            {
              add_matrix_shell_rows(rp,1);
              add_matrix_shell_rows(pp,1);
            } 
  
            (*rp)[n] = (*cp)[j];
            (*pp)[n] = (*(pfp[i]))[j];
          
          }
        }
      }

  //printf("tm before getcamera\n");
  //print_vector((*ptm)[i],1,12);
    //print_matrix(pp,1,n,1,3);
  //print_matrix(rp,1,n,1,3);

   printf("\nResults getcamera - Transfer Matrix to Photo #%d,  %d points\n\n",i,n);
      getcamera( n , pp, rp, (*ptm)[i] );
      free_matrix_shell(rp);
      free_matrix_shell(pp);
    }
  }
 }
//printf("test point 2\n");

 }

// if(itm != NULL)free_vector(itm,1,13);
// itm = NULL;

//  free_matrix(trgpnts,1,npt,1,3);
  return;

}  // end loopbundle















/***** Single Photo Bundling *****/
int bundle_with_one(BundleData &bd )

{

  int i,j,k;
  int np  = 0;


  PNTLST tm  = bd.plTMatrix;   
  PNTLST cp  = bd.plCalcPnts;
  PNTLST *fp = bd.plPixels;
  PNTLST rp  = bd.plSurvPnts;
  int    nf  = bd.nNumPhotos;
  int    npt = bd.nNumPoints;

  np = 0;
  PNTLST fp1 = fp[0];

  PNTLST  pp = matrix_shell(1,npt,1,3);
  PNTLST  rpp = matrix_shell(1,npt,1,3);

// get photo points with matching measured point


  for(i = 0;i <npt; i++)
  {
    if( (*rp)[i] != NULL)
    {
      np++;                    // number of paired points
      (*pp)[np] =  (*fp1)[i];  // photo point
      (*rpp)[np] = (*rp)[i];   // real world point 
    }
  }


  // get centroid of measured points

//print_matrix(rpp,1,np,1,3);
  VECTOR cnt = vector(1,3);

  for(i =1; i<=np; i++)
  {
    add_vector(cnt,1,3,(*rpp)[i],1,3);
  }

  multvector(3,cnt,1.0/np);
//printf("cnt");
//print_vector(cnt,1,3);

  FLOAT maxlng = 0.0;
  VECTOR temp = vector(1,3);

  for(i = 1; i<=np; i++)
  {
    copy_vector(temp,1,3,(*rpp)[i],1,3);
    subtract_vector(temp,1,3,cnt,1,3);
    FLOAT lng = vector_length(3,temp);
    if(lng > maxlng)maxlng = lng;
  }
  
   maxlng *= 5.0;

   VECTOR tmr = vector(1,12);
    
    tmr[3] = -1.0;
    tmr[5] =  1.0;
    tmr[7] = 1.0;

  VECTOR tmx = vector(1,12);
  VECTOR tmo = vector(1,12);
  VECTOR tmf = vector(1,12);
  
  FLOAT min_error = 1.0e20;

//np = 4;
  if(np < 6)for(i=1; i<=4; i++)  // try camera in four quadrants
  {

    if(i==1)copy_vector(tmo,1,12,idmatrix(),1,12);

    tmo[10] = tmo[11] = tmo[12] = 0.0;

    VECTOR tmt = matmat(tmr,tmo);
    free_vector(tmo,1,12);
    tmo = tmt;

    //free_vector(tmx,1,12);

    copy_vector(tmx,1,12,tmo,1,12);
    
    PNT cp = matpnt(tmx,cnt);

    tmx[10] = -cp[1];
    tmx[11] = -cp[2];
    tmx[12] = -cp[3] - maxlng;
  
    free_vector(cp,1,3);

//printf("tmx");
//print_vector(tmx,1,12);
 

    //void getcamera( int np, PNTLST p, PNTLST rpp, VECTOR RR )
    FLOAT error = getcamera3(np,pp,rpp,tmx,0);

//printf("error = %e\n",error);
    if(error < min_error)
    {
//printf("new tmx");
      copy_vector(tmf,1,12,tmx,1,12);  //save results with min error
      min_error = error;
    }
  }
  free_vector(tmx,1,12);
  free_vector(tmr,1,12);

//  printf("tmx");
//  print_vector(tmf,1,12);



 // tmf = vector(1,12);

  for(i=1;i<=1;i++)getcamera3(np,pp,rpp,tmf,1);


  if( (*tm)[0] == NULL)(*tm)[0] = vector(1,13);
  copy_vector((*tm)[0],1,12,tmf,1,12);

/**
  PNTLST a = matrix(1,3,1,3);
  (*a)[1][1] = tmf[1];
  (*a)[1][2] = tmf[2];
  (*a)[1][3] = tmf[3];
  (*a)[2][1] = tmf[4];
  (*a)[2][2] = tmf[5];
  (*a)[2][3] = tmf[6];
  (*a)[3][1] = tmf[7];
  (*a)[3][2] = tmf[8];
  (*a)[3][3] = tmf[9];

  VECTOR w=vector(1,3);
  PNTLST v=matrix(1,3,1,3);

print_matrix(a,1,3,1,3);

  svdcmp(a,3,3,w,v);

printf("############################\n");
  printf(" results from svdcmp\n");
  print_vector(w,1,3);
  print_matrix(a,1,3,1,3);
  print_matrix(v,1,3,1,3);


  for(int nr=1;nr<=3;nr++)
  {
   for(int nc=1;nc<=3;nc++)
   {

     FLOAT vv = 0.0;
     for(int nnc=1;nnc<=3;nnc++)
     {
        vv+= ((*a)[nr][nnc])*(w[nnc])*((*v)[nc][nnc]);
     }
     printf("%d,%d,    %e\n",nr,nc,vv);
   }
  }





  FLOAT mag = 0.0;

  mag = (*v)[1][1]*(*v)[1][1]+(*v)[1][2]*(*v)[1][2]+(*v)[1][3]*(*v)[1][3];
  printf("row1 = %e\n",mag);
  mag = (*v)[2][1]*(*v)[2][1]+(*v)[2][2]*(*v)[2][2]+(*v)[2][3]*(*v)[2][3];
  printf("row2 = %e\n",mag);
  mag = (*v)[3][1]*(*v)[3][1]+(*v)[3][2]*(*v)[3][2]+(*v)[3][3]*(*v)[3][3];
  printf("row3 = %e\n",mag);
  mag = (*v)[1][1]*(*v)[1][1]+(*v)[2][1]*(*v)[2][1]+(*v)[3][1]*(*v)[3][1];
  printf("col1 = %e\n",mag);
  mag = (*v)[1][2]*(*v)[1][2]+(*v)[2][2]*(*v)[2][2]+(*v)[3][2]*(*v)[3][2];
  printf("col2 = %e\n",mag);
  mag = (*v)[1][3]*(*v)[1][3]+(*v)[2][3]*(*v)[2][3]+(*v)[3][3]*(*v)[3][3];
  printf("col3 = %e\n",mag);


PNTLST ww = matrix(1,3,1,3);
(*ww)[1][1] = w[1];
(*ww)[2][2] = w[2];
(*ww)[3][3] = w[3];
PNTLST vt=transpose(v,1,3,1,3);
PNTLST wvt = matrix_mult(ww,1,3,1,3,vt,1,3,1,3,1,3,1,3);
PNTLST awvt = matrix_mult(a,1,3,1,3,wvt,1,3,1,3,1,3,1,3);
printf("results matrix mult\n");
print_matrix(awvt,1,3,1,3);


  for(nr=1;nr<=3;nr++)
  {
    for(int nc=1;nc<=3;nc++)
    {
      (*a)[nr][nc] *= w[nc];
    }
  }

  for(nr=1;nr<=3;nr++)
  {
    for(int nc=1;nc<=3;nc++)
    {

      FLOAT vv=0.0;
      for(int nnc=1;nnc<=3;nnc++)
      {
         vv +=((*a)[nr][nnc])*((*v)[nnc][nc]);
      }
     printf("%d,%d,    %e\n",nr,nc,vv);
   }
  }      



  free_matrix(a,1,3,1,3);
  free_matrix(v,1,3,1,3);
  free_vector(w,1,3);
**/
  FLOAT fl = -65.124;

  //bundlecheck(fl,bd);

  return(1);
} 



/****  
int bundle_add(BundleData &bd )

{

  int i,j,k;
  int np  = 0;


  PNTLST tm = bd.plTMatrix;
  PNTLST cp = bd.plCalcPnts;
  PNTLST *fp = bd.plPixels;
  PNTLST rp = bd.plSurvPnts;
  int    nf = bd.nNumPhotos;
  int    npt = bd.nNumPoints;

****/


int bundle(BundleData &bd )

{

  int i,j,k;
  int np  = 0;
  int up  = 0;        // use previous data flag;
  int incflg = 1;     // set incremental bundle flag to true

  PNTLST  tm = bd.plTMatrix;
  PNTLST  cp = bd.plCalcPnts;
  PNTLST *fp = bd.plPixels;
  PNTLST  rp = bd.plSurvPnts;
  int     nf = bd.nNumPhotos;
  int    npt = bd.nNumPoints;
  extern jmp_buf env;

   if(setjmp(env) != 0)
  {
    //printf("returning 0 from bundle\n");
    return(0);
  }



  /* if there is only one photo, do single photo bundle */

  if( nf <2 ) 
  {
    return(bundle_with_one(bd));
  }


  PNTLST scp=NULL; 
  VECTOR mstm=NULL;


  PNTLST trwp = NULL;   // real world point with matching calculated point
  PNTLST trcp = NULL;   // calculated point with matching real world point


  PNTLST tcrwp = NULL;  // calculated rwp (sub_matrix of crwp)
  PNTLST crwp  = NULL;  // calculated rw - matches cp

  PNTLST ocrwp = NULL;    // old calculated rwp (sub matrix of cp)

  int    ntp = 0; // number of points in trwp,trcp
  int    nrc =0;  // number of points in tcrwp,ocrwp


//printf("enter bundle - %d\n",memcount(0));



  //clear all calculated points(delete allocation in PNTLIST)
  for(i=0;i<npt;i++)
  {
    if( (*cp)[i] != NULL)
    {
      free_vector( (*cp)[i],1,3);
      (*cp)[i] = NULL;
    }
  }

  //printf("count after null cp - %d\n",memcount(0) );

  if( (*tm)[0] != NULL)
  {
    for(i=1;i<nf;i++)
    {
      if( (*tm)[i] == NULL)
      {
        printf("\n ####  CAN NOT RUN INCREMENTAL BUNDLE  ####\n");
        printf(  "   ONE OR MORE PHOTOS HAVE A NULL T-MATRIX\n");
        return(0);
      }
    }
  }




if( (*tm)[0] == NULL )  // if t-matrix's not provided must do full bundle
{

  incflg =0;    // set incremental bundle flag to false;
  up = 1;
  if( (*tm)[0] == NULL )
  {
    up = 0;

    // NULL TM's
    for(i=0;i<nf;i++)
    {
      if( (*tm)[i] != NULL)
      {
        free_vector( ((*tm)[i]), 1,13);
       
      }
      ((*tm)[i]) = vector(1,13);
    }


   //printf("count after null tms - %d\n",memcount(0) );
    // NULL cp's


    // set first photo's tmatrix to the idenity matrix
    (*tm)[0] = vector(1,13);
     copy_vector((*tm)[0],1,12,idmatrix(),1,12);

    // printf("count after clone id - %d\n",memcount(0) );
  }
  else  // rebundle
  {

    // get each photos tm relative to master rather than real world

    VECTOR trm = matinv( (*tm)[0] );
//print_vector( trm,1,12);    

//print_vector( (*tm)[1],1,12);    

    for(int ff = 1; ff< nf; ff++ )
    {
       if( (*tm)[ff] != NULL)
       {
         VECTOR temp = matmat( (*tm)[ff],trm);
         copy_vector( (*tm)[ff],1,12,temp,1,12);
         free_vector(temp,1,12);
       }
    }
//print_vector( (*tm)[1],1,12);    
    FLOAT ts = (*tm)[1][10]*(*tm)[1][10] 
             + (*tm)[1][11]*(*tm)[1][11] + (*tm)[1][12]*(*tm)[1][12];

//printf("ts = %e\n",ts);
    ts = 1500.0/sqrt(ts);
//printf("ts = %e\n",ts);
    for(ff =1;ff<nf;ff++)
    {
      if( (*tm)[ff] != NULL )
      {
        (*tm)[ff][10] *= ts;
        (*tm)[ff][11] *= ts;
        (*tm)[ff][12] *= ts;
      }
    }

    scp = matrix(0,npt-1,1,3);
    for(int pp = 0; pp < npt; pp++)
    {
       if( (*cp)[pp] != NULL)
       {
         PNT temp = matpnt( (*tm)[0], (*cp)[pp] );
         copy_vector( (*scp)[pp],1,3,temp,1,3);
         free_vector( temp,1,3);

         (*scp)[pp][1] *= ts;
         (*scp)[pp][2] *= ts;
         (*scp)[pp][3] *= ts;

         free_vector( (*cp)[pp],1,3);
         (*cp)[pp] = NULL;
         

       }
       else
       {
         free_vector( (*scp)[pp],1,3);
         (*scp)[pp] = NULL;
       }
    }
    free_vector(trm,1,12);
    // set first photo's tmatrix to the idenity matrix
    (*tm)[0] = vector(1,13);
     copy_vector((*tm)[0],1,12,idmatrix(),1,12);

  }



/******/
// printf(" test point 1 - bundle - %d\n",memcount(0));


 // process photo pairs, 0-1,0-2, 0-3, etc

  // for each foto other that the first 
  for(i=1;i<nf;i++)
  {
    int np = 0;
    int nrp = 0;

    PNTLST p1 = matrix_shell(1,1,1,3);
    PNTLST p2 = matrix_shell(1,1,1,3);
    PNTLST tcp = matrix_shell(1,1,1,3);
    PNTLST otcp = matrix_shell(1,1,1,3);
    PNTLST tscp = NULL;
    if(scp != NULL) tscp = matrix_shell(1,1,1,3);

// printf(" test point 2 - bundle - %d\n",memcount(0));


    // get list of common points between pair of photo's 
    // for each point in first foto 
    for(j=0;j< npt;j++)
    { 
      if( (*(fp[0]))[j] != NULL) //if pnt in first photo of pair
      {
        if( (*(fp[i]))[j] != NULL) // if pnt in second photo of pair
        {

          np++; // increment count of points in photo pair

          if(np > 1) // if not first set of points in pair
          {
            add_matrix_shell_rows(p1,1);   //add space for  photo 1 pixel point
            add_matrix_shell_rows(p2,1);   // add space photo 2 pixel point
            add_matrix_shell_rows(tcp,1);  // add space for calculated point
            add_matrix_shell_rows(otcp,1); // add space for old calculated point
            if(tscp != NULL)add_matrix_shell_rows(tscp,1);
          }

          (*p1)[np]=(*(fp[0]))[j]; // add photo 1 pixel point
          (*p2)[np]=(*(fp[i]))[j]; // add photo 2 pixel point 

          (*otcp)[np]=(*cp)[j];// put old calculated point in pair list,get nulls
          if( (*cp)[j] == NULL)(*cp)[j] = vector(1,3); // if calculated point not
                                                       // provided, create space  
          (*tcp)[np]=(*cp)[j];// put calculated point in pair list
          if(tscp != NULL)
          {
            if( (*scp)[j] != NULL )
            {
              (*tscp)[np] = vector(1,3);
              copy_vector( (*tscp)[np],1,3,(*scp)[j],1,3);
            }
          }
        }
      }
    }  // end point matching 
       
     // create vector to hold t-matrix for subject foto 


// printf("XXXXXXXX calling bndl %d XXXXXXXXX\n",memcount(0));     

     // find t-matrix from pnt1 to pnt2, (master foto to subject foto) 



       //  void bndl( int np,PNTLST px1,PNTLST px2,PNTLST cp,VECTOR *caa,
       //             VECTOR tx2 ,int nrwp, PNTLST rwp)


    
   
       int ncrwp=0;
//       if(mrwp != NULL)ncrwp = np;
       PNTLST tempcp = matrix(1,np,1,3);     

printf("\n/////////////////////////////////////////////////////////////////\n");
printf(" LOCATING POSITION OF PHOTO #%d TO MASTER PHOTO (photo #0) \n",i);
printf("      Number of Points = %d\n\n",np);


        if(up)
        {
          if( (*tm)[i] == NULL)
          {
            (*tm)[i] = vector(1,12);
            bndl( np, p2, p1,tempcp,((*tm)[i]),0);
          }
          else
          {

             for(int kk = 1; kk <= np; kk++)
             {
                if((*tscp)[kk] == NULL)
                {
                  printf("calculate new point");

         //PNT get3dpnt_e(int nf,PNTLST pnts, FLOAT **tmxs ,PNTLST error)
                  PNTLST temp_ps = matrix_shell(1,2,1,3);
                  PNTLST temp_ms = matrix_shell(1,2,1,12);

                  (*temp_ms)[1] = (*tm)[0];
                  (*temp_ms)[2] = (*tm)[i];

                  (*temp_ps)[1] = (*p1)[kk];
                  (*temp_ps)[2] = (*p2)[kk];

                  (*tscp)[kk]=get3dpnt_np(2,temp_ps,*temp_ms);
           
                  free_matrix_shell(temp_ps);
                  free_matrix_shell(temp_ms);


                }
                copy_vector((*tempcp)[kk],1,3, (*tscp)[kk],1,3);
             }

             bndl( np, p2, p1,tempcp,((*tm)[i]),up);

          }
        }
        else
        {

           bndl( np, p2, p1,tempcp,((*tm)[i]),up);
        }
        // printf(" bndl - %d\n",jj);
     
        // getchar();
      
// printf("XXXXXXXX return from bndl %d XXXXXXXXX\n",memcount(0));     
        
      // scale points to master points if subject foto is the third
      //  or greater foto in the bundle, master foto and second foto determine
      //  bundle scale 
 
     if(i>1)
     {

       printf("  Scale Photo #0-#%d System to Master System ( Photo #0-#1)\n\n",i);
       printf("                   x              y              z\n");
       FLOAT sf= 0.0;
       int   nsf = 0;
       for(k=1;k<=np;k++)
       {
         if( (*otcp)[k] != NULL)
         {
           int m;
            printf("\n  Pnt[%d]  ",k);
            for(m=1;m<=3;m++)
            {
               FLOAT dsf = (*otcp)[k][m]/(*tempcp)[k][m];
               printf("   %e",dsf);
               sf +=dsf;
               nsf++;
// printf("\n  scale %d,  %e,  %e\n",nsf,dsf,sf);
            }
         }
      
       }
       printf("\n");       
       sf = sf/nsf;
       for(k=1;k<=np;k++)
       {
         int m;
         for(m=1;m<=3;m++)(*tempcp)[k][m] *= sf;

       }
       (*tm)[i][10] *= sf;
       (*tm)[i][11] *= sf;
       (*tm)[i][12] *= sf;
  printf("\n  scale factor = %e\n",sf);
//  getchar();

     } // end scaleing 

       for(k=1;k<=np;k++)
       {
//printf("copying");
//print_vector((*tcp)[k],1,3);
//print_vector((*tempcp)[k],1,3);
         copy_vector((*tcp)[k],1,3,(*tempcp)[k],1,3);

       }


       free_matrix(tempcp,1,np,1,3);
       tempcp = NULL;
       free_matrix_shell(tcp);
       tcp = NULL;
       free_matrix_shell(otcp);
       otcp = NULL;
       free_matrix_shell(p1);
       p1 = NULL; 
       free_matrix_shell(p2);
       p2 = NULL; 
       if(tscp != NULL)free_matrix(tscp,1,np,1,3);  
       tscp = NULL;
      
//       printf(" HIT ENTER KEY TO CONTINUE !!!\n");
//       getchar();

  }  // end processing for each photo 

//printf(" test point 3.1 - bundle - %d\n",memcount(0));
//getchar();

   if(scp != NULL)free_matrix(scp,0,npt-1,1,3);


 



  } // end of code performing  initial part of full bundle (i.e. part not done for inc. bundle)







  // get a bundled solution considering all photos 
 

  VECTOR TV;
  VECTOR itmx = NULL;
  for(int il = 0;il<1 ;il++)
  {
     loopbundle( &bd,incflg ); 

   trwp = matrix_shell(1,1,1,4); //real world point with matching calculated point
   trcp = matrix_shell(1,1,1,4); // calculated point with matching real world point

   ntp = 0; // number of points in trwp,trcp

    for(i=0;i<npt;i++)
    {
      if( (*cp)[i] != NULL)
      {
        if( (*rp)[i] != NULL) //check for matching real world (rw) point
        {
          ntp++;  // increment count of matching real world points
          if(ntp > 1)
          {
            add_matrix_shell_rows(trwp,1); // allocate space for rw point
            add_matrix_shell_rows(trcp,1);
          }
          (*trwp)[ntp] = (*rp)[i]; // put rw point and matching calculated point in lists
          (*trcp)[ntp] = (*cp)[i];
        }
      }
    }


    VECTOR *xcp = (*cp)-1;

     TV = torealsys(ntp,trcp,trwp, npt,&xcp,NULL,itmx);


     if(itmx == NULL)
     {
       itmx=vector(1,13);
       copy_vector(itmx,1,12,idmatrix(),1,12);
       itmx[13] = 1.0;
     }

    if(TV != NULL)
    {     

     for(i=0;i<nf;i++)
     {

       (*tm)[i][10] *= TV[13];
       (*tm)[i][11] *= TV[13];
       (*tm)[i][12] *= TV[13];

       VECTOR t = matmat( (*tm)[i],TV);

       copy_vector((*tm)[i],1,12,t,1,12);

       free_vector(t,1,13);
     } 

    }


 free_matrix_shell(trwp);
 free_matrix_shell(trcp);

  }

  free_vector(itmx,1,13);
  itmx = NULL;


// free_matrix_shell(trwp);
// free_matrix_shell(trcp);



// printf(" test point 11 - bundle - %d\n",memcount(0));

/****
for(i=0;i<nf;i++)
 // print_vector((*tm)[i],1,12);
{

  printf("  Transfer Matrix for Photo #%d\n\n",i);
  display_tm( (*tm)[i] );
}
****/  
//myallinfo();

//printf("exit bundle - %d\n",memcount(0));


if(TV != NULL)
{
  free_vector(TV,1,13);
  newerBB(bd);
}

/***
     loopbundle( &bd ); 

if(ntp > -2)
{
  newerBB(bd);
}
else
{


  for(int i=0;i<npt;i++)
  {
    if( (*cp)[i] != NULL)
    {
      PNT pnt = matpnt((*tm)[0],(*cp)[i]);

   //   printf(" %e,  %e  %e \n",pnt[1],pnt[2],pnt[3]);  
   //   printf(" %e,  %e  %e \n",(*trwp)[i][1],(*trwp)[i][2],(*trwp)[i][3]); 
      copy_vector( (*cp)[i],1,3,pnt,1,3);
      free_vector(pnt,1,3); 
    }
  }

   

   VECTOR adjmat = matinv((*tm)[0]);
   
   for( i = 0;i<nf;i++)
   {
      VECTOR temp = matmat( (*tm)[i], adjmat);
      copy_vector( (*tm)[i],1,13,temp,1,13);  
      free_vector(temp,1,13);
   }
   free_vector(adjmat,1,13);
}
***/  
//newBB2(bd);
printf("exit bundle - %d\n",memcount(0));





  return(1);
}


/**
int bundletest(BundleData &bd )
{
  for(int i = 0; i <= 100; i++) bundletest( bd );
  return(1);
}
**/







VECTOR rotx(int np,PNTLST p,PNTLST xyz)
{
  /* routine to calculate Tmatrix to take points xyz to p system */
printf("enter rotx - %ld\n",memcount(0));

  FLOAT max,sum;
  int flag;

  int i,j,k,l,m;


  int nd = 0;
  int and = np*np;

  PNTLST dp   = matrix(1,1,1,3);
  PNTLST dxyz = matrix(1,1,1,3);


  // Calculate all possible delta vectors between points in each system
  // The length of these delta vectors are independent of the coordinate systen
  // in which they are defined. Therefore the scale factor can be calculated independent
  // of the rotation transform.
 
  k=0;
  for(i = 1; i<=np; i++)
  {
    for(j=i;j<=np;j++)
    {
      if(i != j)
      {
        k++;

        if(k > 1)
        {
          add_matrix_rows(dp,1);
          add_matrix_rows(dxyz,1);
        }
        for(l=1;l<=3;l++)
        {
          (*dp)[k][l] = (*p)[j][l] - (*p)[i][l];
          (*dxyz)[k][l] = (*xyz)[j][l] - (*xyz)[i][l];
        }
      }
    }
  }

  nd = k;  // number of delta vectors

  int nu= 9;
  int ne = 9 + 3*nd;
  VECTOR R = vector(1,13);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR L = vector(1,ne);



  FLOAT sf=0;

  for(i=1;i<=nd;i++)
  {
    FLOAT lp = vector_length(3,(*dp)[i]);
    FLOAT lx = vector_length(3,(*dxyz)[i]);

   // printf(" rotx delta scale factor = %e\n", lx/lp);   

    multvector(3,(*dp)[i], (1000.0/lp) );     // scale each delta vector to a length of 1000.0
    multvector(3,(*dxyz)[i], (1000.0/lx) );

    sf += lx/lp; // sum scale factor for each delta vector for use in average calculations
  }



  sf = sf/nd; // get scale factor as average for each delta vector



  R[13] = sf;

  printf("rotx average scale factor = %e\n",sf);


  //xyz = *dxyz;
  //p   = *dp;

 // PNTLST LUK(int nrk, int nck, int nre, PNTLST eq, PNTLST kn)


  // VECTOR matinv(VECTOR m)


 //print_matrix(dp,1,3,1,3);
 if(nd = 3)
 {

   (*dp)[3][1] = (*dp)[1][2]*(*dp)[2][3] - (*dp)[1][3]*(*dp)[2][2];
   (*dp)[3][2] = (*dp)[1][3]*(*dp)[2][1] - (*dp)[1][1]*(*dp)[2][3];
   (*dp)[3][3] = (*dp)[1][1]*(*dp)[2][2] - (*dp)[1][2]*(*dp)[2][1];
 
   (*dxyz)[3][1] = (*dxyz)[1][2]*(*dxyz)[2][3] - (*dxyz)[1][3]*(*dxyz)[2][2];
   (*dxyz)[3][2] = (*dxyz)[1][3]*(*dxyz)[2][1] - (*dxyz)[1][1]*(*dxyz)[2][3];
   (*dxyz)[3][3] = (*dxyz)[1][1]*(*dxyz)[2][2] - (*dxyz)[1][2]*(*dxyz)[2][1];


   FLOAT det = (*dp)[1][1]*( (*dp)[2][2]*(*dp)[3][3] - (*dp)[3][2]*(*dp)[2][3] )
              -(*dp)[2][1]*( (*dp)[1][2]*(*dp)[3][3] - (*dp)[3][2]*(*dp)[1][3] )
              +(*dp)[3][1]*( (*dp)[1][2]*(*dp)[2][3] - (*dp)[2][2]*(*dp)[1][3] );


//  printf(" det = %e\n",det);



   VECTOR tmp = vector(1,13);

   tmp[1] = (*dp)[1][1];
   tmp[2] = (*dp)[2][1];
   tmp[3] = (*dp)[3][1];
   tmp[4] = (*dp)[1][2];
   tmp[5] = (*dp)[2][2];
   tmp[6] = (*dp)[3][2];
   tmp[7] = (*dp)[1][3];
   tmp[8] = (*dp)[2][3];
   tmp[9] = (*dp)[3][3];
   tmp[10] = 0.0;
   tmp[11] = 0.0;
   tmp[12] = 0.0;
   tmp[13] = 1.0;

//printf("calling matinv\n");
   VECTOR inv = matinv(tmp);
//print_vector(inv,1,13);
}
    
 PNTLST luk =LUK( 3, nd, 3, dp, dxyz);

//printf("@@@@@@@@@@@@ tm from LUK ###########\n");
//print_matrix(luk,1,3,1,3);

  R[1] = (*luk)[1][1];
  R[2] = (*luk)[1][2];
  R[3] = (*luk)[1][3];
  R[4] = (*luk)[2][1];
  R[5] = (*luk)[2][2];
  R[6] = (*luk)[2][3];
  R[7] = (*luk)[3][1];
  R[8] = (*luk)[3][2];
  R[9] = (*luk)[3][3];



free_matrix(luk,1,3,1,3);
  

  for(k=1;k<=500;k++)
  {
    
     forcesquare( L, R, *A);
    for(i=1;i<=nd;i++)
    {
      L[7+i*3] = (*dp)[i][1] - (*dxyz)[i][1]*R[1] -(*dxyz)[i][2]*R[2] -(*dxyz)[i][3]*R[3]
                          /*  - R[10] */;

      L[8+i*3] = (*dp)[i][2] - (*dxyz)[i][1]*R[4] -(*dxyz)[i][2]*R[5]- (*dxyz)[i][3]*R[6]
                          /*  - R[11]*/;

      L[9+i*3] = (*dp)[i][3] - (*dxyz)[i][1]*R[7] -(*dxyz)[i][2]*R[8] -(*dxyz)[i][3]*R[9]
                           /* - R[12]*/;

      (*A)[7+i*3][1] = (*dxyz)[i][1];
      (*A)[7+i*3][2] = (*dxyz)[i][2];
      (*A)[7+i*3][3] = (*dxyz)[i][3];
     /* (*A)[7+i*3][10] = 1;*/


      (*A)[8+i*3][4] = (*dxyz)[i][1];
      (*A)[8+i*3][5] = (*dxyz)[i][2];
      (*A)[8+i*3][6] = (*dxyz)[i][3];
    /*  A[8+i*3][11] = 1;*/


      (*A)[9+i*3][7] = (*dxyz)[i][1];
      (*A)[9+i*3][8] = (*dxyz)[i][2];
      (*A)[9+i*3][9] = (*dxyz)[i][3];
    /*  A[9+i*3][12] = 1;*/
    }  


// printf("\nvector L - \n");
// print_vector(L,1,ne);
/*
printf("matrix A\n");
print_matrix(A,1,ne,1,nu);
*/
    VECTOR D=lsq(A,L,nu,ne);
    add_vector(R,1,nu,D,1,nu);

// printf("\nvector D\n");
// print_vector(D,1,nu);


// printf("vector R\n");
// print_vector(R,1,nu);
    flag = 0;

    for(i=1;i<=nu;i++)
    {
      if(D[i]<0.0)D[i] = -D[i];
      if(D[i]>1.0e-12)
      {
        flag = 1;
      }
    }
    free_vector(D,1,nu);
    if(flag == 0)break;
  }
//  printf("rotx average error = %e, max error = %e\n",sum,max);

  max =sum=0.0;
  for(i =10;i<=ne;i++)
  {
      FLOAT error = L[i];
      if(error<0.0)error=-error; 
      sum += error;    
      if(error > max)max = error;
  }

  
  sum = sum/(np*3);
  printf("rotx average error = %e, max error = %e\n",sum,max);



// calculate approx distance between origins

  PNTLST pp = matrix(1,np,1,3);
  //pp = *pp_p;
  for(i=1;i<=np;i++)
  {
     // p[i][1] *= R[13];
     // p[i][2] *= R[13];
     // p[i][3] *= R[13];

      (*pp)[i][1] = R[1]*(*xyz)[i][1] + R[2]*(*xyz)[i][2] + R[3]*(*xyz)[i][3]/* + R[10] */;
      (*pp)[i][2] = R[4]*(*xyz)[i][1] + R[5]*(*xyz)[i][2] + R[6]*(*xyz)[i][3]/* + R[11] */;
      (*pp)[i][3] = R[7]*(*xyz)[i][1] + R[8]*(*xyz)[i][2] + R[9]*(*xyz)[i][3]/* + R[12] */;

     R[10] = (*p)[i][1]*R[13] - ((*pp)[i][1]/np);
     R[11] = (*p)[i][2]*R[13] - ((*pp)[i][2]/np);
     R[12] =(*p)[i][3]*R[13] - ((*pp)[i][3]/np);
  }


 free_matrix(pp,1,np,1,3);

  free_vector(L,1,ne);
   free_matrix(A,1,ne,1,nu);
   free_matrix(dp,1,nd,1,3);
   free_matrix(dxyz,1,nd,1,3);
//printf("\n R from rotx\n");
//print_vector(R,1,12);
//printf("rxit rotx - %ld\n",memcount(0));
  return(R);
}











VECTOR subsrotx(int np,PNTLST p,PNTLST xyz,VECTOR ir, FLOAT *rl, FLOAT *rm)
{
  /* routine to calculate Tmatrix to take points xyz to p system */
  /* and a scale facter to scale points p to transformed xyz points */
  FLOAT max,sum;
  FLOAT *D;
  int flag;
  int nu= 13;
  int ne = 9 + 3*np;

  int i,k,j;
//printf("enter subsrotx - %d\n",memcount(0));


  if(np< 3)
  {
    printf("Can not scale and rotate calculated points - less than 3 Measured Points\n");
    return(NULL);
  }


  VECTOR R = NULL;
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR L = vector(1,ne);

  PNTLST pp;
  PNTLST ps;
  FLOAT sf=0.0;



 if(ir == NULL)
 {


   for(int i = 1; i <= 1;i++)
   {
     if(R != NULL)free_vector(R,1,13);
     R = rotx(np,p,xyz);
     printf("return rotx - %d\n",i);
   }    
   sf =R[13];
 }
 else
 {
//printf("itm provided \n");
   R = clone_vector(ir,1,13,1,13);
 }

//  R[10] = ir[10];
//  R[11] = ir[11];
//  R[12] = ir[12];

//  free_matrix(dp_p,1,np,1,3);
//  free_matrix(dxyz_p,1,np,1,3);


//R[13] = sf;

//printf("enter srotx - %ld\n",memcount(0));

  for(k=1;k<=100;k++)
  {
    
     forcesquare( L, R, *A,1.0e4);
    for(i=1;i<=np;i++)
    {
      L[7+i*3] = R[13]*R[13]*(*p)[i][1] - (*xyz)[i][1]*R[1] -(*xyz)[i][2]*R[2] -(*xyz)[i][3]*R[3]
                            - R[10];

      L[8+i*3] = R[13]*R[13]*(*p)[i][2] - (*xyz)[i][1]*R[4] -(*xyz)[i][2]*R[5]- (*xyz)[i][3]*R[6]
                            - R[11];

      L[9+i*3] = R[13]*R[13]*(*p)[i][3] - (*xyz)[i][1]*R[7] -(*xyz)[i][2]*R[8] -(*xyz)[i][3]*R[9]
                            - R[12];

      (*A)[7+i*3][1] = (*xyz)[i][1];
      (*A)[7+i*3][2] = (*xyz)[i][2];
      (*A)[7+i*3][3] = (*xyz)[i][3];
      (*A)[7+i*3][10] = 1;
      (*A)[7+i*3][13] = -(*p)[i][1]*2.0*R[13];


      (*A)[8+i*3][4] = (*xyz)[i][1];
      (*A)[8+i*3][5] = (*xyz)[i][2];
      (*A)[8+i*3][6] = (*xyz)[i][3];
      (*A)[8+i*3][11] = 1;
      (*A)[8+i*3][13] = -(*p)[i][2]*2.0*R[13];


      (*A)[9+i*3][7] = (*xyz)[i][1];
      (*A)[9+i*3][8] = (*xyz)[i][2];
      (*A)[9+i*3][9] = (*xyz)[i][3];
      (*A)[9+i*3][12] = 1;
      (*A)[9+i*3][13] = -(*p)[i][3]*2.0*R[13];
    }  


//printf("vector L - sort\n");
//print_vector(L,1,ne);
/*
printf("matrix A\n");
print_matrix(A,1,ne,1,nu);
*/
    D=lsq(A,L,nu,ne);
    add_vector(R,1,nu,D,1,nu);

//printf("vector D\n");
//print_vector(D,1,nu);


// printf("vector R\n");
// print_vector(R,1,nu);

//getchar();
    if(R[13]<0.0)R[13]=-R[13];    
    flag = 0;

    for(i=1;i<=nu;i++)
    {
      if(D[i]<0.0)D[i] = -D[i];
      if(D[i]>1.0e-12)
      {
        flag = 1;
      }
    }
    free_vector(D,1,nu);

    max =sum=0.0;

    for(i =10;i<=ne;i++)
    {
        FLOAT error = L[i];
        if(error<0.0)error=-error; 
        sum += error;    
        if(error > max)max = error;
    }

  
    sum = sum/(np*3);
    if(sum > 1.0e50)flag=0;
// printf("srotx average error = %e, max error = %e\n",sum,max);

    if(flag == 0)break;

  }




  R[13]=R[13]*R[13];

printf("Scaling Results  sf = %e",R[13]);

 printf("  average error = %e, max error = %e\n\n",sum,max);

  *rl = sum;
  *rm = max;

/*
  pp = matrix(1,np,1,3);
  ps = matrix(1,np,1,3);

  for(i=1;i<=np;i++)
  {
      ps[i][1] = p[i][1] * R[13];
      ps[i][2] = p[i][2] * R[13];
      ps[i][3] = p[i][3] * R[13];

      pp[i][1] = R[1]*xyz[i][1] + R[2]*xyz[i][2] + R[3]*xyz[i][3] + R[10];
      pp[i][2] = R[4]*xyz[i][1] + R[5]*xyz[i][2] + R[6]*xyz[i][3] + R[11];
      pp[i][3] = R[7]*xyz[i][1] + R[8]*xyz[i][2] + R[9]*xyz[i][3] + R[12];
  }

  for(j=1;j<=5;j++)
  {
    FLOAT s = scale(np,ps,pp);
    R[13] *=s; 

    for(i=1;i<=np;i++)
    {
      ps[i][1] *= s;
      ps[i][2] *= s;
      ps[i][3] *= s;

    }
    if(R != NULL)free_vector(R,1,13);
    R = rotx(np,ps,xyz);

   for(i=1;i<=np;i++)
    {
      pp[i][1] = R[1]*xyz[i][1] + R[2]*xyz[i][2] + R[3]*xyz[i][3] + R[10];
      pp[i][2] = R[4]*xyz[i][1] + R[5]*xyz[i][2] + R[6]*xyz[i][3] + R[11];
      pp[i][3] = R[7]*xyz[i][1] + R[8]*xyz[i][2] + R[9]*xyz[i][3] + R[12];

    }


  }

  free_matrix(pp,1,np,1,3);
  free_matrix(ps,1,np,1,3);
*/

/*  free_vector(R,1,nu);*/
  free_vector(L,1,ne);
   free_matrix(A,1,ne,1,nu);


// printf(" R from subsrotx\n");
// print_vector(R,1,12);
//printf("exit subsrotx - %d\n",memcount(0));
  return(R);
}



VECTOR srotx(int np,PNTLST p,PNTLST xyz,VECTOR itm)
{
  int i,j;
  FLOAT rl,rm;
  VECTOR temp = NULL;
  
//    FLOAT ll,lm;
//printf(" enter srotx - %d\n",memcount(0));

 printf("\n   Scale and Rotate Calculated points to Measured Points\n\n");
 printf("            number of points = %d\n",np);

    //printf(" itm provided \n");
    temp = subsrotx(np,p,xyz,itm,&rl,&rm);
 //   printf("\n\n   average error = %e, max error = %e\n",rl,rm);
//printf(" exit srotx - %d\n",memcount(0));
    return(temp);
//  }

}   




VECTOR torealsys(int np, PNTLST cp, PNTLST mp, int npc, PNTLST xyz
        ,VECTOR tm, VECTOR itm)
{
  
  /* returns the tmatrix R to take mp's to scaled cp's (R[13]= scale) */

  /* xyz,s are converted to real world system if provided*/
  /* it is assumed xyz's are defined in same system as cp's */

  /* if tm provided, it is converted */
  /* it is assumed that tm is from cp's system to some arbitrary system. */
  /* The converted tm will be from the real world sys to to arbitrary system */

  VECTOR R;
 // MATRIX cp = *cp_p;
 // MATRIX mp = *mp_p;
 // MATRIX xyz =NULL;

//printf("enter torealsys %d\n",memcount(0));
  //if(npc > 0)xyz = *xyz_p;
  

  R=srotx(np,cp,mp,itm);

  if(R!= NULL)
  {
    int i;

    VECTOR inv = matinv(R);
    for(i=1;i<=npc;i++)
    {
      FLOAT p[3];
   
      if( (*xyz)[i] != NULL)
      {
//printf(" srotx before %e,%e,%e,",(*xyz)[i][1],(*xyz)[i][2],(*xyz)[i][3]);
  
        p[0] = ((*xyz)[i][1] * R[13]);
        p[1] = ((*xyz)[i][2] * R[13]);
        p[2] = ((*xyz)[i][3] * R[13]);

        (*xyz)[i][1] = inv[1]*p[0] + inv[2]*p[1] + inv[3]*p[2] + inv[10];
        (*xyz)[i][2] = inv[4]*p[0] + inv[5]*p[1] + inv[6]*p[2] + inv[11];
        (*xyz)[i][3] = inv[7]*p[0] + inv[8]*p[1] + inv[9]*p[2] + inv[12];
//printf(" after  %e,%e,%e\n",(*xyz)[i][1],(*xyz)[i][2],(*xyz)[i][3]);

      }

    
    } 

    free_vector(inv,1,12);

    if(tm != NULL)
    {
      VECTOR t;

      tm[10] *= R[13]; 
      tm[11] *= R[13]; 
      tm[12] *= R[13]; 

      t=clone_vector(tm,1,12,1,12);

      tm[1] = t[1]*R[1] + t[2]*R[4] + t[3]*R[7];
      tm[2] = t[1]*R[2] + t[2]*R[5] + t[3]*R[8];
      tm[3] = t[1]*R[3] + t[2]*R[6] + t[3]*R[9];

      tm[4] = t[4]*R[1] + t[5]*R[4] + t[6]*R[7];
      tm[5] = t[4]*R[2] + t[5]*R[5] + t[6]*R[8];
      tm[6] = t[4]*R[3] + t[5]*R[6] + t[6]*R[9];

      tm[7] = t[7]*R[1] + t[8]*R[4] + t[9]*R[7];
      tm[8] = t[7]*R[2] + t[8]*R[5] + t[9]*R[8];
      tm[9] = t[7]*R[3] + t[8]*R[6] + t[9]*R[9];
      
      tm[10] = t[1]*R[10] + t[2]*R[11] + t[3]*R[12] + t[10];
      tm[11] = t[4]*R[10] + t[5]*R[11] + t[6]*R[12] + t[11];
      tm[12] = t[7]*R[10] + t[8]*R[11] + t[9]*R[12] + t[12];

      free_vector(t,1,12);
    }
  }


//  if(itm != NULL)free_vector(itm,1,13);
//  itm = clone_vector(R,1,13,1,13);
//    if(itm != NULL)copy_vector(itm,1,13,R,1,13);

//printf("exit torealsys %d\n",memcount(0));
    return(R);
}




VECTOR MergeBundles( VECTOR masterImageTMatrix, VECTOR mergeImageTMatrix,
	int numCalcPnts, PNTLST masterCalcPnts, PNTLST mergeCalcPnts )
{

   VECTOR mat;
   PNTLST mp,cp;
  
   cp = submatrix(mergeCalcPnts,0,numCalcPnts-1,1,3,1,1);
   mp = submatrix(masterCalcPnts,0,numCalcPnts-1,1,3,1,1);

   print_matrix(mp,1,numCalcPnts,1,3);

   if(numCalcPnts > 2)
   {
     // VECTOR torealsys(int np, PNTLST cp, PNTLST mp, int npc, PNTLST xyz
     //   ,VECTOR tm, VECTOR itm)
     mat =  torealsys(numCalcPnts,cp,mp,NULL, NULL,
            NULL, NULL);
     printf(" matrix from master to merged\n");
     print_vector(mat,1,13);

   }
   else
   {
      
   
   
   }
  //free_submatrix(mp,1,numCalcPnts,1,3);
  //free_submatrix(cp,1,numCalcPnts,1,3);
  return(mat);
}  
     

 
