/*FF*************************************************************************
*									    *
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

/*ele_math.c++ */
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include "mathsubs.h"
#include "photog_msg.h"
#include "output.h"
#include "photog_math.h"
#include "ele_math.h"
#include "..\triton\triton.h"
#include "drawutils.h"
#include <setjmp.h>
extern jmp_buf env;
extern int envi;

VECTOR ccc(int np, PNTLST pnts);
VECTOR cccp(int np, PNTLST pnts,VECTOR u);
void make_cyl_by_endpnts(PHOTO_FLOAT radius,PNT pnt1,PNT pnt2,struct Cylinder &cyl,struct _transform &trans);



void rotpnt(VECTOR m , PNT p)
{

  PNT pp = vector(1,3);

  pp[1] = m[1]*p[1] +m[2]*p[2] + m[3]*p[3];
  pp[2] = m[4]*p[1] +m[5]*p[2] + m[6]*p[3];
  pp[3] = m[7]*p[1] +m[8]*p[2] + m[9]*p[3];

  copy_vector(p,1,3,pp,1,3);

  free_vector(pp,1,3);

  return;
}


VECTOR prj_pnt_onto_line( VECTOR pnt, VECTOR org, VECTOR dir)
{
  PHOTO_FLOAT s = (pnt[1]-org[1])*dir[1] + (pnt[2]-org[2])*dir[2] +(pnt[3]-org[3])*dir[3];
  VECTOR pp = vector(1,3);

  pp[1] = org[1] + s*dir[1];
  pp[2] = org[2] + s*dir[2];
  pp[3] = org[3] + s*dir[3];

  return(pp);
}
  

PHOTO_FLOAT dist_pnt_to_line(VECTOR pnt, PNTLST line)
{

  VECTOR d = clone_vector(pnt,1,3,1,3);
  subtract_vector(d,1,3, (*line)[2],1,3);

  VECTOR c = cross_vector(d, (*line)[1] );

  PHOTO_FLOAT l = vector_length(3,c);

  free_vector(d,1,3);
  free_vector(c,1,3);

  return(l);
}



PHOTO_FLOAT dist_pnt_to_line(VECTOR pnt, VECTOR org, VECTOR dir)
{

  VECTOR d = clone_vector(pnt,1,3,1,3);
  subtract_vector(d,1,3, org,1,3);

  VECTOR c = cross_vector(d,dir );

  PHOTO_FLOAT l = vector_length(3,c);

  free_vector(d,1,3);
  free_vector(c,1,3);

  return(l);
}



PHOTO_FLOAT intersect_lines_dir( VECTOR o1, VECTOR d1, VECTOR o2,
                         VECTOR d2, PHOTO_FLOAT *s1, PHOTO_FLOAT *s2 )
{

  VECTOR L = vector(1,3);
  PNTLST A = matrix(1,3,1,2);

  for(int i = 1; i<=3; i++)
  {
    L[i] = o1[i]-o2[i];      
    (*A)[i][1] = -d1[i];
    (*A)[i][2] = d2[i];
  }

  VECTOR R = lsq(A,L,2,3);

  *s1 = R[1];
  *s2 = R[2];

  free_vector(L,1,3);
  free_vector(R,1,2);
  free_matrix(A,1,3,1,2);


  PHOTO_FLOAT x = o1[1] + (*s1)*d1[1] - o2[1] - (*s2)*d2[1];
  PHOTO_FLOAT y = o1[2] + (*s1)*d1[2] - o2[2] - (*s2)*d2[2];
  PHOTO_FLOAT z = o1[3] + (*s1)*d1[3] - o2[3] - (*s2)*d2[3];


  PHOTO_FLOAT l = sqrt( x*x + y*y + z*z );
printf("l from intersect = %e  s1 = %e,  s2 = %e\n",l,*s1,*s2);
  return(l);
}




VECTOR prj_pnt_onto_plane(VECTOR pnt, VECTOR pl_org, VECTOR pl_dir)
{
  /*
        Project a point onto a plane

        pnt    - point to project
        pl_org - point in plane
        pl_dir - vector normal to plane(unit);

  */

  //project pnt onto line with direction of pl_dir through pl_org
  VECTOR pnt1 = prj_pnt_onto_line( pnt, pl_org, pl_dir);
        
  // get vector from projected point to point plane origin
  VECTOR d = clone_vector(pl_org,1,3,1,3);
  subtract_vector(d,1,3, pnt1,1,3);

  //adding this delta vector to input point gives the projected point
  VECTOR prj_pnt = clone_vector(pnt,1,3,1,3);
  add_vector(prj_pnt,1,3,d,1,3);

  free_vector(pnt1,1,3);
  free_vector(d,1,3);

  return(prj_pnt);
}



VECTOR intersect_line_plane(VECTOR pl_org, VECTOR pl_dir, VECTOR ln_org, VECTOR ln_dir)
{
  /*
       Find intersection point of a line and a plane

        pl_org  - point in plane
        pl_dir  - vector normal to plane
        ln_org  - point on Line 
        ln_dir  - direction vector for line


        a NULL vector is returned if line parallel to plane;
  */

  // project line origin onto plane 
  VECTOR pnt1 = prj_pnt_onto_plane( ln_org, pl_org, pl_dir);

  //get vector from line origin to plane normal to plane
  subtract_vector(pnt1,1,3,ln_org,1,3);

  //get |pnt1|*|ln_dir|*cos(theta) - dot product of vectors pnt1 and ln_dir
  PHOTO_FLOAT uv =  dot_vector( 3,pnt1,ln_dir);  
  
  //if uv == 0 line and plane are parallel
  if(uv < 1.0e-10 ) return(NULL);

  // we are looking for some scaler s of ln_dir such that s*|ln_org|cos(theta) = |pnt1|
  //     s*|pnt1|*|ln_dir|*cos(theta) = |pnt1|*|pnt1|
  //     s*uv = |pnt1|*|pnt1|
  //     s = (|pnt1|*|pnt1|)/uv where |pnt1|*|pnt1| = dot product of pnt1 with itself

  PHOTO_FLOAT s = (dot_vector( 3,pnt1,pnt1))/uv;
  
  //scale ln by s and add line origin to get results
  VECTOR pp = clone_vector(ln_dir,1,3,1,3);
  multvector(3,pp,s);
  add_vector(pp,1,3,ln_org,1,3);
   

  free_vector(pnt1,1,3);

  return(pp);
}




VECTOR intersect_line_beamSide(VECTOR cl_org, VECTOR cl_dir, VECTOR sd_dir, PHOTO_FLOAT dist, PHOTO_FLOAT width,
                               VECTOR ln_org, VECTOR ln_dir )
{
  /*
       Find intersection point of a line and side of beam of infinite length

        cl_org  - point on center line of beam
        cl_dir  - direction vector for center line
        sd_dir  - vector normal to side, points from center line towards side
        dist    - distance from center line to plane
        widtn   - width of side , edge to edge 

        ln_org  - point on intersecting line
        ln_dir  - direction vector for intersecting line

        a NULL vector is returned if no intersection;
  */

  //get point in plane of beam side
  VECTOR sd_org  = unit_vector(3,sd_dir);
  multvector(3,sd_org,dist);
  add_vector(sd_org,1,3,cl_org,1,3);

  // intersect line with plane contaning beam side
  VECTOR pnt = intersect_line_plane( sd_org, sd_dir,ln_org,ln_dir);
  free_vector(sd_org,1,3);

  // check to see if line papallel to side
  if(pnt == NULL)
  {

    printf("LINE PARALLEL TO SIDE !!!\n");
    return(pnt);
  }
  //check to see if intersect point is between sides
  PHOTO_FLOAT x = dist_pnt_to_line(pnt,cl_org,cl_dir);
  
  if( (x*x) <= ((width*width/4.0)+(dist*dist)))return(pnt);
  free_vector(pnt,1,3);
  printf("POINT NOT BETWEEN SIDES !!\n");

  return(NULL);
}  



PNTLST intersect_line_sphere(PHOTO_FLOAT r, VECTOR center, VECTOR l_org, VECTOR l_dir)
{

printf("enter inter sect line sphere %e, \n",r);
  
  VECTOR cp = prj_pnt_onto_line( center, l_org, l_dir);
  
  PHOTO_FLOAT d = dist_pnt_to_line( center, l_org, l_dir);

  printf("distance from center to line - %e\n",d);


  PHOTO_FLOAT dd = sqrt( r*r - d*d);

  PNTLST pnts = matrix_shell(1,2,1,3);

  (*pnts)[1] = clone_vector(l_dir,1,3,1,3);
  (*pnts)[2] = clone_vector(l_dir,1,3,1,3);

  multvector(3,(*pnts)[1], -dd);
  multvector(3,(*pnts)[2],  dd);
  
  add_vector((*pnts)[1],1,3,cp,1,3);
  add_vector((*pnts)[2],1,3,cp,1,3);
  free_vector(cp,1,3);
  return(pnts);
}



PNTLST intersect_line_cyl(PHOTO_FLOAT r, VECTOR c_org, VECTOR c_dir, VECTOR l_org, VECTOR l_dir)
{
  PHOTO_FLOAT s1 = 0;
  PHOTO_FLOAT s2 = 0;

  VECTOR v1 = cross_vector(l_dir,c_dir);
  VECTOR v2 = cross_vector(c_dir,v1);
  VECTOR v  = unit_vector(3,v2);

  free_vector(v1,1,3);
  free_vector(v2,1,3);


  intersect_lines_dir( c_org, c_dir , l_org,
                         v,  &s1, &s2 );
  VECTOR center = clone_vector(c_dir,1,3,1,3);
  multvector(3,center,s1);
  add_vector(center,1,3,c_org,1,3);

 PNTLST ints = intersect_line_sphere( r, center, l_org, v);

 intersect_lines_dir(l_org,l_dir, (*ints)[1], c_dir,&s1,&s2);

 copy_vector( (*ints)[1],1,3, l_dir, 1,3);
 multvector(3, (*ints)[1], s1 );
 add_vector((*ints)[1],1,3,l_org,1,3);

 intersect_lines_dir(l_org,l_dir, (*ints)[2], c_dir,&s1,&s2);

 copy_vector( (*ints)[2],1,3, l_dir, 1,3);
 multvector(3, (*ints)[2], s1 );
 add_vector((*ints)[2],1,3,l_org,1,3);

 free_vector(v,1,3);
 free_vector(center,1,3);

 return(ints);
}






PNTLST intersect_line_cone(PHOTO_FLOAT k, VECTOR c_org, VECTOR c_dir, VECTOR l_org, VECTOR l_dir)
{
  PHOTO_FLOAT s1 = 0;
  PHOTO_FLOAT s2 = 0;

printf("   K = %e\n",k);

printf("c_org");
print_vector(c_org,1,3);

printf("c_dir");
print_vector(c_dir,1,3);

printf("l_org");
print_vector(l_org,1,3);


printf("l_dir");
print_vector(l_dir,1,3);


  VECTOR v0 = clone_vector(c_org,1,3,1,3);
print_vector(v0,1,3);
    subtract_vector(v0,1,3,l_org,1,3);
print_vector(v0,1,3);
  VECTOR v1 = cross_vector(l_dir,v0);
print_vector(v1,1,3);

  VECTOR v2 = cross_vector(c_dir,v1);
print_vector(v2,1,3);

  VECTOR v  = unit_vector(3,v2);
print_vector(v,1,3);

  free_vector(v0,1,3);
  free_vector(v1,1,3);
  free_vector(v2,1,3);


  intersect_lines_dir( c_org, c_dir , l_org,
                         v,  &s1, &s2 );

  VECTOR center = clone_vector(c_dir,1,3,1,3);
  multvector(3,center,s1);
  add_vector(center,1,3,c_org,1,3);



 PHOTO_FLOAT r = k*s1;

 if(r < 0.0)r = -r;
  
 PNTLST ints = intersect_line_sphere( r, center, l_org, v);

 intersect_lines_dir(l_org,l_dir, (*ints)[1], c_dir,&s1,&s2);

 copy_vector( (*ints)[1],1,3, l_dir, 1,3);
 multvector(3, (*ints)[1], s1 );
 add_vector((*ints)[1],1,3,l_org,1,3);

 intersect_lines_dir(l_org,l_dir, (*ints)[2], c_dir,&s1,&s2);

 copy_vector( (*ints)[2],1,3, l_dir, 1,3);
 multvector(3, (*ints)[2], s1 );
 add_vector((*ints)[2],1,3,l_org,1,3);

 free_vector(v,1,3);
 free_vector(center,1,3);

 return(ints);
}






VECTOR centroid( int np, PNTLST pnts, PNTLST tms)
{


  int nu = 3 + np;
  int ne = 3*np;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);

  int e = 1;
  int p = 4;


  // solve 
  for(int i = 0; i< np; i++)
  {

    VECTOR pnt = (*pnts)[i];
    VECTOR tm  = (*tms)[i];

    L[e]   = -tm[10];
    L[e+1] = -tm[11];
    L[e+2] = -tm[12];

    (*A)[e][1]   = -1.0;
    (*A)[e+1][2] = -1.0;
    (*A)[e+2][3] = -1.0;

    (*A)[e][p]   = pnt[1];
    (*A)[e+1][p] = pnt[2];
    (*A)[e+2][p] = pnt[3];

    e += 3;
    p++;
  }

   VECTOR R = lsq(A,L,nu,ne);

   printf(" R fron centrod ");
   print_vector(R,1,nu);
  //getchar();

  // VECTOR C = clone_vector(R,1,3,1,3);

 //  free_vector(R,1,3);

   return(R);
}






VECTOR cvec(int np, PNTLST pnts )

{

   int nu = 4;
   int ne = 1+np;

   VECTOR R = vector(1,nu);
   PNTLST A = matrix(1,ne,1,nu);
   VECTOR L = vector(1,ne);

  R[1] = 1.0;
  R[2] = 1.0;
  R[3] = 1.0;

  for(int loop =0; loop < 50; loop++)
  {
    int e = 1;

    L[e] = 1.0*(1.0 - R[1]*R[1] - R[2]*R[2] - R[3]*R[3]);

    (*A)[e][1] = 2.0*R[1];
    (*A)[e][2] = 2.0*R[2];
    (*A)[e][3] = 2.0*R[3];
   
    e++;

   for(int i = 1; i <= np; i++)
   {
     L[e] = R[4] - R[1]*(*pnts)[i-1][1] 
           - R[2]*(*pnts)[i-1][2] - R[3]*(*pnts)[i-1][3];
 
     (*A)[e][4] = -1.0;

     (*A)[e][1] = (*pnts)[i-1][1];
     (*A)[e][2] = (*pnts)[i-1][2];
     (*A)[e][3] = (*pnts)[i-1][3];
    
     e++;
   }

    printf("vector R");
    print_vector(R,1,nu);

    printf("vector L");
    print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

    for(i=1; i<= nu; i++) R[i] += D[i];

    printf("vector D");
    print_vector(D,1,nu);

    printf("end loop %d\n",loop);
    //getchar();

  
    free_vector(D,1,nu);

  }

  free_vector(L,1,ne);
  free_matrix(A,1,ne,1,nu);

  return(R);
}     







void norm_circ(int np1, PNTLST pnts1, VECTOR tm1,
               int np2, PNTLST pnts2, VECTOR tm2)
{ 

  printf(" enter norm_circle ");

  PHOTO_FLOAT big = 1.0e5;
  PHOTO_FLOAT little = 1.0e-3;
  PHOTO_FLOAT radsqr = 10.0;
  int i;
  int tnc = 2;
  int tnp = np1+np2;




 // find approx origin

  PHOTO_FLOAT s1 = 0.0;
  PHOTO_FLOAT s2 = 0.0;

  VECTOR w1 = cvec( np1,  pnts1 );

  VECTOR w2 = cvec( np2,  pnts2 );
  

  intersect_lines_dir( &(tm1[9]),w1,&(tm2[9]),w2,&s1,&s2);

  VECTOR org = vector(1,3);



  org[1] = 0.5*( tm1[10] + s1*w1[1] + tm2[10] + s2*w2[1]);
  org[2] = 0.5*( tm1[11] + s1*w1[2] + tm2[11] + s2*w2[2]);
  org[3] = 0.5*( tm1[12] + s1*w1[3] + tm2[12] + s2*w2[3]);


printf(" ORIGIN \n");
print_vector(org,1,3);














  int nu = 3 + 3*tnc + tnc + 3*tnc + tnp + 3*tnp;
  int ne = tnc + 3*tnc + tnp + tnp +3*tnp + 1;
  
  VECTOR L = vector(1,ne);
  VECTOR R = vector(1,nu);
  PNTLST A = matrix(1,ne,1,nu);

printf(" nu = %d, ne = %d \n",nu,ne);
  for(int loop = 0; loop < 100; loop++)
  {
    
     
 
     int e = 1;
     

     int ci = 1;
     int ui = ci + 3;

     int si = ui + 3*tnc;
     int wi = si + tnc;
     
     int gi = wi + 3*tnc;
     int vi = gi + tnp;
     



     if(loop == 0)
     {
        R[ci] =   org[1];
        R[ci+1] = org[2];
        R[ci+2] = org[3];

        PHOTO_FLOAT ll = w1[1]*w1[1] + w1[2]*w1[2] + w1[3]*w1[3];
        ll = sqrt(ll);
        R[ui] =   w1[1]/ll;
        R[ui+1] = w1[2]/ll;
        R[ui+2] = w1[3]/ll;



        ll = w2[1]*w2[1] + w2[2]*w2[2] + w2[3]*w2[3];
        ll = sqrt(ll);

        R[ui+3] = w2[1]/ll;
        R[ui+4] = w2[2]/ll;
        R[ui+5] = w2[3]/ll;

        for(i=si;i<wi;i++)R[i] = 1.0;

        for(i=wi;i<gi;i += 6)
        {
          R[i]   = w1[1];
          R[i+1] = w1[2];
          R[i+2] = w1[3];
          R[i+3] = w2[1];
          R[i+4] = w2[2];
          R[i+5] = w2[3];
        }

        for(i=gi;i<= nu;i++)R[i] = 50.0;

     }






     for(int ncc = 0; ncc < tnc; ncc++)  // loop over circles
     {

        PNTLST pnts = pnts1;
        VECTOR tm   = tm1;
        int np = np1;
        if( ncc > 0)
        {
          pnts = pnts2;
          tm = tm2;
          np = np2;
        }


       // constraint U.U = 1;

       PHOTO_FLOAT lng =  R[ui]*R[ui] + R[ui+1]*R[ui+1] + R[ui+2]*R[ui+2];
       lng = sqrt(lng);
       R[ui]   = R[ui]/lng;
       R[ui+1] = R[ui+1]/lng;
       R[ui+2] = R[ui+2]/lng;

       L[e] = big*( R[ui]*R[ui] + R[ui+1]*R[ui+1] + R[ui+2]*R[ui+2] - 1.0 );
   
       (*A)[e][ui]   = big*(-2.0*R[ui]);
       (*A)[e][ui+1] = big*(-2.0*R[ui+1]);
       (*A)[e][ui+2] = big*(-2.0*R[ui+2]);

       e++;





        

        //constraint F + s*W = C

        if(R[si] < 0.0 ) R[si] *= -1.0;

        L[e]   = little*(tm[10] + R[si]*R[si]/*R[wi]*/   - R[ci]);
        L[e+1] = little*(tm[11] + R[si]*R[si]/*R[wi+1]*/ - R[ci+1]);
        L[e+2] = little*(tm[11] + R[si]*R[si]/*R[wi+2]*/ - R[ci+2]);

        
        (*A)[e][si]   = little*(-2.0*R[si]/*R[wi]*/);
        (*A)[e+1][si] = little*(-2.0*R[si]/*R[wi+1]*/);
        (*A)[e+2][si] = little*(-2.0*R[si]/*R[wi+2]*/);
/*
        (*A)[e][wi]      = little*(-R[si]*R[si]);
        (*A)[e+1][wi+1]  = little*(-R[si]*R[si]);
        (*A)[e+2][wi+2]  = little*(-R[si]*R[si]);
*/
        (*A)[e][ci]     = little*(1.0);
        (*A)[e+1][ci+1] = little*(1.0);
        (*A)[e+2][ci+2] = little*(1.0);


        e += 3;

        for(int npc = 0; npc < np; npc++)
        {
           VECTOR pnt = (*pnts)[npc];







           //constraint V.V = rad*rad

           lng =  R[vi]*R[vi] + R[vi+1]*R[vi+1] + R[vi+2]*R[vi+2];
           lng = sqrt(lng)/sqrt(radsqr);
           R[vi]   = R[vi]/lng;
           R[vi+1] = R[vi+1]/lng;
           R[vi+2] = R[vi+2]/lng;





           L[e] = (R[vi]*R[vi] + R[vi+1]*R[vi+1] + R[vi+2]*R[vi+2] -radsqr);


          (*A)[e][vi]   = -2.0*R[vi];
          (*A)[e][vi+1] = -2.0*R[vi+1];
          (*A)[e][vi+2] = -2.0*R[vi+2];

      
          e++;

 
          //constraint V.U = 0.0 

          L[e] = big*(R[vi]*R[ui] + R[vi+1]*R[ui+1] + R[vi+2]*R[ui+2]);
   
           (*A)[e][ui]   = big*(-R[vi]);
           (*A)[e][ui+1] = big*(-R[vi+1]);
           (*A)[e][ui+2] = big*(-R[vi+2]);

           (*A)[e][vi]   = big*(-R[ui]);
           (*A)[e][vi+1] = big*(-R[ui+1]);
           (*A)[e][vi+2] = big*(-R[ui+2]);
 
          e++;

        
          //constraint W + V = g*f;

          if(R[gi] < 0.0)R[gi] *= -1.0;

          L[e]   = R[wi]   + R[vi]   - R[gi]*R[gi]*pnt[1];
          L[e+1] = R[wi+1] + R[vi+1] - R[gi]*R[gi]*pnt[2];
          L[e+2] = R[wi+2] + R[vi+2] - R[gi]*R[gi]*pnt[3];


          (*A)[e][wi]     = -1.0;
          (*A)[e+1][wi+1] = -1.0;
          (*A)[e+2][wi+2] = -1.0;

 
          (*A)[e][vi]     = -1.0;
          (*A)[e+1][vi+1] = -1.0;
          (*A)[e+2][vi+2] = -1.0;
    
          (*A)[e][gi]   = 2.0*R[gi]; 
          (*A)[e+1][gi] = 2.0*R[gi]; 
          (*A)[e+2][gi] = 2.0*R[gi]; 

          e += 3;

          gi++;
          vi += 3;


        }  //end loop over points
 
        si++; 
        ui += 3;
        wi += 3;

      } // end loop over circles;


      // constraint U1.U2 = 1.0

      ci = 1;
      ui = ci + 3;

      si = ui + 3*tnc;
      wi = si + tnc;
     
      gi = wi + 3*tnc;
      vi = gi + tnp;

      L[e] = little*(1.0 - R[ui]*R[ui+3] - R[ui+1]*R[ui+4] - R[ui+2]*R[ui+5]);

      (*A)[e][ui] = little*R[ui+3];
      (*A)[e][ui+1] = little*R[ui+4];
      (*A)[e][ui+2] = little*R[ui+5];
      (*A)[e][ui+3] = little*R[ui];
      (*A)[e][ui+4] = little*R[ui+1];
      (*A)[e][ui+5] = little*R[ui+2];

      e++;



printf("vector R");
print_vector(R,1,nu);


printf("vector L");
print_vector(L,1,ne);
    VECTOR D = lsq(A,L,nu,ne);

printf("vector D");
print_vector(D,1,nu);

//getchar();
    for(i = 1; i<= nu; i++) R[i] += D[i];
    
     free_vector(D,1,nu);

    } // end iteration loop

  free_vector(R,1,nu);
  free_vector(L,1,ne); 
  free_matrix(A,1,ne,1,nu);

  free_vector(w1,1,4);
  free_vector(w2,1,4);
  free_vector(org,1,3);

  return;
}



void init_circ( int np1, PNTLST pnts1, VECTOR tm1, 
                int np2, PNTLST pnts2, VECTOR tm2)
{
printf(" enter INIT_CIRC \n");

  PHOTO_FLOAT big = 1.0e5;
  PHOTO_FLOAT s1 = 0.0;
  PHOTO_FLOAT s2 = 0.0;
  int i = 0;
  int tnp = np1+np2;
  
  PHOTO_FLOAT RR = 0.0;

  VECTOR w1 = cvec( np1,  pnts1 );

  VECTOR w2 = cvec( np2,  pnts2 );
  

  intersect_lines_dir( &(tm1[9]),w1,&(tm2[9]),w2,&s1,&s2);

  VECTOR org = vector(1,3);



  org[1] = 0.5*( tm1[10] + s1*w1[1] + tm2[10] + s2*w2[1]);
  org[2] = 0.5*( tm1[11] + s1*w1[2] + tm2[11] + s2*w2[2]);
  org[3] = 0.5*( tm1[12] + s1*w1[3] + tm2[12] + s2*w2[3]);


printf(" ORIGIN \n");
print_vector(org,1,3);


  int nu = 3  + tnp + 3*tnp; 
  int ne = 1 + 5*tnp;

  VECTOR R = vector(1,nu);
  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);

  for(int loop = 0; loop < 100; loop++)
  {
    int e = 1;
    int ui = 1;
    //int ri = ui + 3;
    int gi = ui + 3;
    int vi = gi + tnp;
    

    if(loop == 0)
    {

      for(i = 0; i < np1; i++)
      {
        PHOTO_FLOAT cos = w1[1]*(*pnts1)[i][1] 
                  + w1[2]*(*pnts1)[i][2]
                  + w1[3]*(*pnts1)[i][3];

        if(cos < 0.0 ) 
        {
           w1[1] = -w1[1];
           w1[2] = -w1[2];
           w1[3] = -w1[3];
          
           cos = -cos;
        }

        PHOTO_FLOAT Lw = sqrt ( ((org[1] - tm1[10])*(org[1] - tm1[10]))    
                         +((org[2] - tm1[11])*(org[2] - tm1[11]))
                         +((org[3] - tm1[12])*(org[3] - tm1[12])) );
        

        R[gi] = sqrt( Lw/cos );

        R[vi]  = ((*pnts1)[i][1])*R[gi]*R[gi] - w1[1]*Lw;
        R[vi+1] = ((*pnts1)[i][2])*R[gi]*R[gi] - w1[2]*Lw;
        R[vi+2] = ((*pnts1)[i][3])*R[gi]*R[gi] - w1[3]*Lw;

        RR += sqrt( R[vi]*R[vi] + R[vi+1]*R[vi+1] + R[vi+2]*R[vi+2] );

        gi++;
        vi += 3;
      }


      for(i = 0; i < np2; i++)
      {
        PHOTO_FLOAT cos = w2[1]*(*pnts2)[i][1] 
                  + w2[2]*(*pnts2)[i][2]
                  + w2[3]*(*pnts2)[i][3];

        if(cos < 0.0 ) 
        {
           w2[1] = -w2[1];
           w2[2] = -w2[2];
           w2[3] = -w2[3];
          
           cos = -cos;
        }


        PHOTO_FLOAT Lw = sqrt ( ((org[1] - tm2[10])*(org[1] - tm2[10]))    
                         +((org[2] - tm2[11])*(org[2] - tm2[11]))
                         +((org[3] - tm2[12])*(org[3] - tm2[12])) );
        

        R[gi] =sqrt( Lw/cos );

        R[vi]  = ((*pnts2)[i][1])*R[gi]*R[gi] - w2[1]*Lw;
        R[vi+1] = ((*pnts2)[i][2])*R[gi]*R[gi] - w2[2]*Lw;
        R[vi+2] = ((*pnts2)[i][3])*R[gi]*R[gi] - w2[3]*Lw;

        RR += sqrt( R[vi]*R[vi] + R[vi+1]*R[vi+1] + R[vi+2]*R[vi+2] );

        gi++;
        vi += 3;
      }


       RR = RR/tnp;


      R[ui]   = 0.5*(w1[1]+w2[1]);
      R[ui+1] = 0.5*(w1[2]+w2[2]);
      R[ui+2] = 0.5*(w1[3]+w2[3]);

      PHOTO_FLOAT l = 1.0/sqrt( R[ui]*R[ui] + R[ui+1]*R[ui+1] + R[ui+2]*R[ui+2] );
      R[ui]   = R[ui]*l;
      R[ui+1] = R[ui+1]*l;
      R[ui+2] = R[ui+2]*l;

       ui = 1;
       //ri = ui + 3;
       gi = ui + 3;
       vi = gi + tnp;
    
    }

    // constraint U.U = 1;

    L[e] = big*(1.0 - R[ui]*R[ui] - R[ui+1]*R[ui+1]  - R[ui+2]*R[ui+2]);

    (*A)[e][ui]   = big*2.0*R[ui];
    (*A)[e][ui+1] = big*2.0*R[ui+1];
    (*A)[e][ui+2] = big*2.0*R[ui+2];

    e++;

    for(i = 0; i < tnp; i++)
    {
      // constraint V.V = r*r

      L[e] = /*RR*RR*/ - R[vi]*R[vi] - R[vi+1]*R[vi+1] - R[vi+2]*R[vi+2];

//      (*A)[e][ri] = -2.0*R[ri];

      (*A)[e][vi] =  2.0*R[vi];
      (*A)[e][vi+1] = 2.0*R[vi+1];
      (*A)[e][vi+2] = 2.0*R[vi+2];

      e++;
       

      //constraint U.V = 0.0;

      L[e] = big*(R[ui]*R[vi] + R[ui+1]*R[vi+2] + R[ui+1]*R[vi+2]);

      (*A)[e][ui]   = -big*R[vi];
      (*A)[e][ui+1] = -big*R[vi+1];
      (*A)[e][ui+2] = -big*R[vi+2];

      (*A)[e][vi]   = -big*R[ui];
      (*A)[e][vi+1] = -big*R[ui+1];
      (*A)[e][vi+2] = -big*R[ui+2];

      e++;

      VECTOR pnt = (*pnts1)[i];
      VECTOR tm = tm1;
  
      if(i >= np1)
      {
        pnt = (*pnts2)[i-np1];
        tm = tm2;
      }

      // constraint org + V = g*f + F;

      L[e]   = org[1] + R[vi]   - R[gi]*R[gi]*pnt[1] - tm[10];
      L[e+1] = org[2] + R[vi+1] - R[gi]*R[gi]*pnt[2] - tm[11];
      L[e+2] = org[3] + R[vi+2] - R[gi]*R[gi]*pnt[3] - tm[12];

      (*A)[e][vi]     = -1.0; 
      (*A)[e+1][vi+1] = -1.0; 
      (*A)[e+2][vi+2] = -1.0; 

      (*A)[e][gi]   = 2.0*R[gi]*pnt[1];
      (*A)[e+1][gi] = 2.0*R[gi]*pnt[2];
      (*A)[e+2][gi] = 2.0*R[gi]*pnt[3];

      e += 3;

      gi++;
      vi += 3;
    }
printf("vector R");
print_vector(R,1,nu);


printf("vector L");
print_vector(L,1,ne);
    VECTOR D = lsq(A,L,nu,ne);

printf("vector D");
print_vector(D,1,nu);

    for(i = 1; i<= nu; i++) R[i] += D[i];


//getchar();
   }




printf("exit init circ\n");
  return;
}


PNTLST find_ellipse(int np,PNTLST pts)
{
  PHOTO_FLOAT big = 1.0e2;

printf("find ellipse input pnts");
print_matrix(pts,0,np-1,1,3);

  PNTLST p = clone_matrix(pts,0,np-1,1,3,0,np-1,1,3);
/***
  for(int i = 0;i<np;i++)
  {

    PHOTO_FLOAT zf = -65.0/(*p)[i][3];
    (*p)[i][1] = (*p)[i][1] * zf;
    (*p)[i][2] = (*p)[i][2] * zf;
    (*p)[i][3] = -65.0;
  }
***/
  VECTOR RR = cccp(np,pts,NULL);  // get cyl info 
  print_vector(RR,1,7);
 // getchar();

  free_matrix(p,0,np-1,1,3);

  //PHOTO_FLOAT zf = (-65.0 - RR[3])/RR[6];


  PNTLST rr = matrix(1,3,1,3);

  (*rr)[1][1] = RR[1];
  (*rr)[1][2] = RR[2];
  (*rr)[1][3] = RR[3];


  PHOTO_FLOAT vl = sqrt((*rr)[1][1]*(*rr)[1][1] 
                + (*rr)[1][2]*(*rr)[1][2] 
                + (*rr)[1][3]*(*rr)[1][3]);

  (*rr)[1][1] = (*rr)[1][1]/vl;
  (*rr)[1][2] = (*rr)[1][2]/vl;
  (*rr)[1][3] = (*rr)[1][3]/vl;



  (*rr)[2][1] = RR[4];
  (*rr)[2][2] = RR[5];
  (*rr)[2][3] = RR[6];


        vl = sqrt((*rr)[2][1]*(*rr)[2][1] 
                + (*rr)[2][2]*(*rr)[2][2] 
                + (*rr)[2][3]*(*rr)[2][3]);

  (*rr)[2][1] = (*rr)[2][1]/vl;
  (*rr)[2][2] = (*rr)[2][2]/vl;
  (*rr)[2][3] = (*rr)[2][3]/vl;


  vl = (*rr)[1][1]*(*rr)[2][1] + (*rr)[1][2]*(*rr)[2][2] + (*rr)[1][3]*(*rr)[2][3];

  (*rr)[3][1] = vl*(*rr)[1][1];
  (*rr)[3][2] = vl*(*rr)[1][2];
  (*rr)[3][3] = vl*(*rr)[1][3];

  (*rr)[3][1] = 2.0*(*rr)[3][1] - (*rr)[2][1];
  (*rr)[3][2] = 2.0*(*rr)[3][2] - (*rr)[2][2];
  (*rr)[3][3] = 2.0*(*rr)[3][3] - (*rr)[2][3];


  vl = (*rr)[3][1]*(*rr)[3][1] + (*rr)[3][2]*(*rr)[3][2] + (*rr)[3][3]*(*rr)[3][3];

  (*rr)[3][1] = vl*(*rr)[3][1];
  (*rr)[3][2] = vl*(*rr)[3][2];
  (*rr)[3][3] = vl*(*rr)[3][3];

  free_vector(RR,1,7);
//print_vector((*rr)[1],1,3);

//getchar();


  RR = cccp(np,pts,NULL);  // get cyl info 

  print_vector(RR,1,7);
//  getchar();

  return(rr);
}



PNTLST xfind_ellipse(int np,PNTLST pts)
{

  PNTLST p = clone_matrix(pts,0,np-1,1,3,1,np,1,3);

  for(int i = 1;i<=np;i++)
  {

    PHOTO_FLOAT zf = -65.0/(*p)[i][3];
    (*p)[i][1] = (*p)[i][1] * zf;
    (*p)[i][2] = (*p)[i][2] * zf;
  }


  int nu = 6+2*(np-1);
  int ne = 1 + 3*(np-1);

  VECTOR R = vector(1,nu);
  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);

    int ri = 1;
    int si = ri+1;
    int ci = si+1;
    int ui = ci+2;
    int xi = ui+2;
  
  R[ri] = 1.0/sqrt( ((*p)[1][1] - (*p)[np][1])*((*p)[1][1] - (*p)[np][1])
               +((*p)[1][2] - (*p)[np][2])*((*p)[1][2] - (*p)[np][2]) );
 // R[bi] = R[ai];
 // R[ci] = 0.5*((*p)[1][1]+(*p)[np][1]);
 // R[ci+1] = 0.5*((*p)[1][2]+(*p)[np][2]);
  R[ci] = (*p)[np][1];
  R[ci+1] = (*p)[np][2];
  np--;

  R[ui] = 1.0;
  R[ui+1] = 0.1;

  PHOTO_FLOAT lmax = 0.0;
  PHOTO_FLOAT lmin = 0.0;
  int maxi = 1;
  int mini = 1;

  for(i = 1; i<= np; i++)
  {
    PHOTO_FLOAT dx = (*p)[i][1] - (*p)[np+1][1];
    PHOTO_FLOAT dy = (*p)[i][2] - (*p)[np+1][2];
    PHOTO_FLOAT lng = sqrt(dx*dx + dy*dy);
    if(i==1)
    {
      lmax = lng;
      maxi = i;
      lmin = lng;
      mini = i;
    }
    else
    {
      if(lng > lmax)
      {
        lmax = lng;
        maxi = i;
      }
      if(lng < lmin)
      {
        lmin = lng;
        mini = i;
      }
    }
  } 

  R[ri] = lmax;
  R[ui+1] = ( (*p)[maxi][1] - (*p)[np+1][1] )/lmax;
  R[ui]   = ( (*p)[maxi][2] - (*p)[np+1][2] )/lmax;

  PHOTO_FLOAT dx = ( (*p)[mini][1] - (*p)[np+1][1]);
  PHOTO_FLOAT dy = ( (*p)[mini][2] - (*p)[np+1][2]);

  R[si] = sqrt(dx*dx + dy*dy)/R[ri];


  for(i = 1; i<=np; i++)
  {

    PHOTO_FLOAT dx = (*p)[i][1] - (*p)[np+1][1];
    PHOTO_FLOAT dy = (*p)[i][2] - (*p)[np+1][2];
 
    R[xi]   = (R[ui]*dx - R[ui+1]*dy)/R[si];
    R[xi+1] = R[ui+1]*dx + R[ui]*dy;

    xi += 2;
  }

  //R[si] = 1.0;
 
  R[si] = sqrt(R[si]); 

  PHOTO_FLOAT big = 1.0e2;
  for(int loop = 0; loop <= 100; loop++)
  {


   int e = 1;
    ri = 1;
    si = ri+1;
    ci = si+1;
    ui = ci+2;
    xi = ui+2;



   // constraint U.U = 1.0;

   PHOTO_FLOAT vl = sqrt(R[ui]*R[ui] + R[ui+1]*R[ui+1]);
   R[ui] = R[ui]/vl;
   R[ui+1] = R[ui+1]/vl;

   L[e] = big*(R[ui]*R[ui] + R[ui+1]*R[ui+1] -1.0);

   (*A)[e][ui] =   -2.0*big*R[ui]; 
   (*A)[e][ui+1] = -2.0*big*R[ui+1]; 

   e++;


    if(R[ri] < 0.0)R[ri] = -R[ri];

    for(i = 1;i<=np;i++)
    {
    
      
      L[e]   = big*(R[ri]*R[ri] - R[xi]*R[xi] - R[xi+1]*R[xi+1]);

      (*A)[e][ri]   = -2.0*big*R[ri];
      (*A)[e][xi]   =  2.0*big*R[xi]; 
      (*A)[e][xi+1] =  2.0*big*R[xi+1]; 

      e++;

      L[e]   = (*p)[i][1] - R[si]*R[si]*R[ui]  *R[xi] - R[ui+1]*R[xi+1] - R[ci];
      L[e+1] = (*p)[i][2] + R[si]*R[si]*R[ui+1]*R[xi] - R[ui]  *R[xi+1] - R[ci+1];
   

      (*A)[e][si]   =  2.0*R[si]*R[ui]  *R[xi];
      (*A)[e+1][si] = -2.0*R[si]*R[ui+1]*R[xi];

      (*A)[e][ui]   = R[si]*R[si]*R[xi];
      (*A)[e+1][ui] = R[xi];

      (*A)[e][ui+1]   = R[xi+1];
      (*A)[e+1][ui+1] = -R[si]*R[si]*R[xi];


      (*A)[e][ci]   = 1.0;

      (*A)[e+1][ci+1] = 1.0;


      (*A)[e][xi]   = R[si]*R[si]*R[ui];
      (*A)[e][xi+1] = R[ui+1];
    
      (*A)[e+1][xi]   = -R[si]*R[si]*R[ui+1];
      (*A)[e+1][xi+1] =  R[ui];


      e++;
      e++;

      xi +=2;
    }

printf("vector R");
print_vector(R,1,nu);
printf(" vector L");
print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

     for(int j =1;j<=nu;j++) R[j] += 0.7*D[j];

//     R[ci] -= 0.6*D[ci];
//     R[ci+1] -= 0.6*D[ci+1];

 printf("vector D");
print_vector(D,1,nu);

 printf(" loop - %d\n", loop);
    free_vector(D,1,nu);
 

  }// end loop for iteration


  free_vector(L,1,ne);
  free_matrix(A,1,ne,1,nu);
  free_matrix(p,1,np,1,3);

  PNTLST rr = matrix(1,3,1,3);


  (*rr)[1][1] = R[ci]/sqrt(R[ci]*R[ci]+R[ci+1]*R[ci+1] + 65.0*65.0);
  (*rr)[1][2] = R[ci+1]/sqrt(R[ci]*R[ci]+R[ci+1]*R[ci+1] + 65.0*65.0);
  (*rr)[1][3] = -65.0/sqrt(R[ci]*R[ci]+R[ci+1]*R[ci+1] + 65.0*65.0);

print_vector((*rr)[1],1,3);
//getchar();

  free_vector(R,1,nu);
  return(rr);
}





VECTOR circle_by_edge(int np, PNTLST pnts, PNTLST tms, VECTOR center,
                       VECTOR uv )
{
  PHOTO_FLOAT big = 1.0e3;
printf("ENTER CIRCLE BY EDGE\n");
//getchar();

   int nu = 6 + np;
   int ne = 1+ np + np*np - np;

  int i;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);



  int ci =1;
  int ui = ci +3;
  int ti = ui +3;


  R[ci]   = center[1];
  R[ci+1] = center[2];
  R[ci+2] = center[3];



  for(int j = 0; j< np; j++)
  {
    R[ti+j] = 1.0 + j*0.01;
    R[ui]   = ((*pnts)[j][1])/np;
    R[ui+1] = ((*pnts)[j][2])/np;
    R[ui+2] = ((*pnts)[j][3])/np;

  }


  R[ui]   = uv[1];
  R[ui+1] = uv[2];
  R[ui+2] = uv[3];

  // U  = unit vector normal to circle plane;

  for(int loop = 0; loop < 100; loop++)
  {

    int e = 1;
    ti = ui+3;

    // U.U = 1.0  insure unit length normal vector
    PHOTO_FLOAT l = sqrt( R[ui]*R[ui] + R[ui+1]*R[ui+1] + R[ui+2]*R[ui+2] );

    R[ui] = R[ui]/l;
    R[ui+1] = R[ui+1]/l;
    R[ui+2] = R[ui+2]/l;

    L[e] = 1000.0*(R[ui]*R[ui] + R[ui+1]*R[ui+1] + R[ui+2]*R[ui+2] - 1.0);

    (*A)[e][ui] = -2000.0*R[ui];
    (*A)[e][ui+1] = -2000.0*R[ui+1];
    (*A)[e][ui+2] = -2000.0*R[ui+2];

    e++;

    for(int i = 0; i<np; i++)
    {
      PHOTO_FLOAT temp = (R[ci]  -((*tms)[i][10]) )*R[ui]
                  +(R[ci+1]-((*tms)[i][11]) )*R[ui+1]
                  +(R[ci+2]-((*tms)[i][12]) )*R[ui+2];

      temp = temp/( ((*pnts)[i][1])*R[ui] + ((*pnts)[i][2])*R[ui+1] + ((*pnts)[i][3])*R[ui+2] );

      R[ti+i] = sqrt(temp - 100.0);
    }
  

    for( i = 0;i<np;i++)
    {

        // C = vector to circle center
        // Q = vector to point on circle
        // Q = F + f where F is vector to photo origin and f is point on photo ray;
        //     f = fp*(100+t*t) where fp is ray unit vector and t is scalar to calculate distance
        //          distance along vector;

       // U.(Q-C) = 0.0
       
       L[e] = 1000.0*( R[ui]  *((*tms)[i][10]) + R[ui]  *(100+R[ti]*R[ti])*((*pnts)[i][1]) - R[ui]*  R[ci]
                     + R[ui+1]*((*tms)[i][11]) + R[ui+1]*(100+R[ti]*R[ti])*((*pnts)[i][2]) - R[ui+1]*R[ci+1]
                     + R[ui+2]*((*tms)[i][12]) + R[ui+2]*(100+R[ti]*R[ti])*((*pnts)[i][3]) - R[ui+2]*R[ci+2] );


      (*A)[e][ui]   = 1000.0*( -((*tms)[i][10]) - (100+R[ti]*R[ti])*((*pnts)[i][1]) +  R[ci]   );
      (*A)[e][ui+1] = 1000.0*( -((*tms)[i][11]) - (100+R[ti]*R[ti])*((*pnts)[i][2]) +  R[ci+1] );
      (*A)[e][ui+2] = 1000.0*( -((*tms)[i][12]) - (100+R[ti]*R[ti])*((*pnts)[i][3]) +  R[ci+2] );

      (*A)[e][ti] = -2000.0*R[ti]*( R[ui]  *((*pnts)[i][1])
                                  + R[ui+1]*((*pnts)[i][2])
                                  + R[ui+2]*((*pnts)[i][3]) );


      (*A)[e][ci]   = 1000.0*R[ui];
      (*A)[e][ci+1] = 1000.0*R[ui+1];
      (*A)[e][ci+2] = 1000.0*R[ui+2];

      e++;

      

      PHOTO_FLOAT rxi = ((*tms)[i][10]) +  (100+R[ti]*R[ti])*((*pnts)[i][1]) - R[ci];
      PHOTO_FLOAT ryi = ((*tms)[i][11]) +  (100+R[ti]*R[ti])*((*pnts)[i][2]) - R[ci+1];
      PHOTO_FLOAT rzi = ((*tms)[i][12]) +  (100+R[ti]*R[ti])*((*pnts)[i][3]) - R[ci+2];

      PHOTO_FLOAT rri = rxi*rxi + ryi*ryi + rzi*rzi;

      int tk = ui+3;

      for(int k=0;k<np;k++)
      {
        if(k != i)
        {
     
          PHOTO_FLOAT rxk = ((*tms)[k][10]) +  (100+R[tk]*R[tk])*((*pnts)[k][1]) - R[ci];
          PHOTO_FLOAT ryk = ((*tms)[k][11]) +  (100+R[tk]*R[tk])*((*pnts)[k][2]) - R[ci+1];
          PHOTO_FLOAT rzk = ((*tms)[k][12]) +  (100+R[tk]*R[tk])*((*pnts)[k][3]) - R[ci+2];

          PHOTO_FLOAT rrk = rxk*rxk + ryk*ryk + rzk*rzk;


          L[e] = sqrt(rri) - sqrt(rrk);

          PHOTO_FLOAT wfi = 1.0/(2.0*sqrt(rri));
          PHOTO_FLOAT wfk = 1.0/(2.0*sqrt(rrk));

          (*A)[e][ci]   = wfi*2.0*rxi - wfk*2.0*rxk;
          (*A)[e][ci+1] = wfi*2.0*ryi - wfk*2.0*ryk;
          (*A)[e][ci+2] = wfi*2.0*rzi - wfk*2.0*rzk;

          (*A)[e][ti]   = wfi*(-4.0*rxi*R[ti]*((*pnts)[i][1])
                               -4.0*ryi*R[ti]*((*pnts)[i][2])
                               -4.0*rzi*R[ti]*((*pnts)[i][3]) );

          (*A)[e][tk]   = wfk*(4.0*rxk*R[tk]*((*pnts)[k][1])
                             + 4.0*ryk*R[tk]*((*pnts)[k][2])
                             + 4.0*rzk*R[tk]*((*pnts)[k][3]) );

          e++;

        }
        tk++;

      }

      ti++;
      
    }//end loop over points

//printf("vector R");
//print_vector(R,1,nu);
//printf(" vector L");
//print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);
     
     PHOTO_FLOAT dsum = 0.0;
     PHOTO_FLOAT al = 0.0;

     PHOTO_FLOAT fk = 0.5;
     if(loop >= 50)fk = 0.25;
     for(int j =1;j<=nu;j++) R[j] += fk*D[j];
     for( j =1;j<=nu;j++) dsum += D[j]*D[j];

     for( j =1;j<=ne;j++) al += L[j]*L[j];

// printf("vector D");
//print_vector(D,1,nu);

 printf(" loop - %d   avgd = %e, %e\n", loop, sqrt(dsum/nu),sqrt(al/ne));
    free_vector(D,1,nu);

  }
//printf("vector R");
//print_vector(R,1,nu);
//printf(" vector L");
//print_vector(L,1,ne);


  free_vector(L,1,ne);
  free_matrix(A,1,ne,1,nu);
  /*****  contents of C
    int ui = 1;
    int ri = ui + 3;
    int ci = ri+1;
    int gi = ci+3;
    int vi = gi+np;

  ****/

   ci =1;
   ui = ci +3;
   ti = ui +3;





  VECTOR C = vector(1,7+np);

  C[1] = R[4];
  C[2] = R[5];
  C[3] = R[6];

  C[5] = R[1];
  C[6] = R[2];
  C[7] = R[3];
  
  for( i = 0;i<np;i++)
  {
    PHOTO_FLOAT rxi = ((*tms)[i][10]) +  (100+R[ti]*R[ti])*((*pnts)[i][1]) - R[ci];
    PHOTO_FLOAT ryi = ((*tms)[i][11]) +  (100+R[ti]*R[ti])*((*pnts)[i][2]) - R[ci+1];
    PHOTO_FLOAT rzi = ((*tms)[i][12]) +  (100+R[ti]*R[ti])*((*pnts)[i][3]) - R[ci+2];

    PHOTO_FLOAT rri = rxi*rxi + ryi*ryi + rzi*rzi;

    C[4] += sqrt(rri)/np;

    C[8+i] = 100+R[ti]*R[ti];

    ti++;
  }

  free_vector(R,1,nu);

printf("VECTOR C");

print_vector(C,1,7+np);;

 return(C);
}






VECTOR xxcircle_by_edge(int np, PNTLST pnts, PNTLST tms, VECTOR center )
{
  PHOTO_FLOAT big = 1.0e3;

  
   int nu = 6 + 3*np;
   int ne = 1+5*np + np*np - np;

  int i;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);

printf("ENTER CIRCLE BY EDGE\n");
//getchar();
  VECTOR C = centroid( np, pnts, tms);

    PHOTO_FLOAT l = sqrt( C[1]*C[1] + C[2]*C[2] + C[3]*C[3] );

    int ui = 1;
   // int ri = ui + 3;
    int ci = ui + 1;
    int pi = ci + 3;

    ci--;

    R[ci+1] = center[1];
    R[ci+2] = center[2];
    R[ci+3] = center[3];
  
    ui--;

    R[ui+1] = C[1]/l;
    R[ui+2] = C[2]/l;
    R[ui+3] = C[3]/l;

    pi--;

  // R[ri] = 0.0;
   for(i = 0;i<np;i++)
   {

     PHOTO_FLOAT x = C[1] - (*tms)[i][10];
     PHOTO_FLOAT y = C[2] - (*tms)[i][11];
     PHOTO_FLOAT z = C[3] - (*tms)[i][12];
     PHOTO_FLOAT ll = sqrt(x*x + y*y + z*z);
     
     R[pi+1] = (*pnts)[i][1]*ll + (*tms)[i][10];
     R[pi+2] = (*pnts)[i][2]*ll + (*tms)[i][11];
     R[pi+3] = (*pnts)[i][3]*ll + (*tms)[i][12];

     x = R[pi+1] - R[ci+1];
     y = R[pi+2] - R[ci+2];
     z = R[pi+3] - R[ci+3];

    // R[ri] += sqrt(x*x +y*y + z*z)/np;


     pi += 3;
  }

  PHOTO_FLOAT coef = 1.0;
  for(int loop = 0; loop < 200; loop++)
  {

    if(loop > 50)coef = 1.999 - (0.999/50.0)*loop;

    int ui = 1;
    // int ri = ui + 3;
    int ci = ui + 3;
    int pi = ci + 3;

    ui--;
    ci--;
    pi--;
    
    int e = 1; 



    // constraint U.U = 1.0;

     PHOTO_FLOAT sf = sqrt(R[ui+1]*R[ui+1] + R[ui+2]*R[ui+2] + R[ui+3]*R[ui+3]);

    R[ui+1]= R[ui+1]*sf;
    R[ui+2]= R[ui+2]*sf;
    R[ui+3]= R[ui+3]*sf;

    L[e] = big*big*(1.0 - R[ui+1]*R[ui+1] - R[ui+2]*R[ui+2] - R[ui+3]*R[ui+3]);

    (*A)[e][ui+1] = big*big*2.0*R[ui+1];
    (*A)[e][ui+2] = big*big*2.0*R[ui+2];
    (*A)[e][ui+3] = big*big*2.0*R[ui+3];

    e++;



    for(i = 0; i < np; i++)
    {

      VECTOR tm = (*tms)[i];
      VECTOR f  = (*pnts)[i];


      // constraint (P - C).U = 0.0;

      L[e] = - big*(R[pi+1] - R[ci+1])*R[ui+1]
             - big*(R[pi+2] - R[ci+2])*R[ui+2]
             - big*(R[pi+3] - R[ci+3])*R[ui+3];

      (*A)[e][pi+1] = big*R[ui+1];
      (*A)[e][pi+2] = big*R[ui+2];
      (*A)[e][pi+3] = big*R[ui+3];

      (*A)[e][ci+1] = -big*R[ui+1];
      (*A)[e][ci+2] = -big*R[ui+2];
      (*A)[e][ci+3] = -big*R[ui+3];
        
      (*A)[e][ui+1] = big*(R[pi+1] - R[ci+1]);
      (*A)[e][ui+2] = big*(R[pi+2] - R[ci+2]);
      (*A)[e][ui+3] = big*(R[pi+3] - R[ci+3]);

      e++;

      

      
      //constraint (P-F)xf = 0.0;

      L[e]   = big*((R[pi+2]-tm[11])*f[3] - (R[pi+3]-tm[12])*f[2]);
      L[e+1] = big*((R[pi+3]-tm[12])*f[1] - (R[pi+1]-tm[10])*f[3]);
      L[e+2] = big*((R[pi+1]-tm[10])*f[2] - (R[pi+2]-tm[11])*f[1]);

      (*A)[e][pi+2] = -big*f[3];
      (*A)[e][pi+3] =  big*f[2];

      (*A)[e+1][pi+3] = -big*f[1];
      (*A)[e+1][pi+1] =  big*f[3];

      (*A)[e+2][pi+1] = -big*f[2];
      (*A)[e+2][pi+2] =  big*f[1];

      e += 3;


      // constraint (P - C).(P - C) = (Pk - Ck).(Pk - Ck);


      int pk = ci+3;
      pk--;
      for(int kk = 0; kk < np; kk++)
      {
       if(kk != i)
       {
        L[e] = (R[pk+1]-R[ci+1])*(R[pk+1]-R[ci+1])
              +(R[pk+2]-R[ci+2])*(R[pk+2]-R[ci+2]) 
              +(R[pk+3]-R[ci+3])*(R[pk+3]-R[ci+3])
                            - (R[pi+1]-R[ci+1])*(R[pi+1]-R[ci+1])
                            - (R[pi+2]-R[ci+2])*(R[pi+2]-R[ci+2])    
                            - (R[pi+3]-R[ci+3])*(R[pi+3]-R[ci+3]);


       // (*A)[e][ri] = -2.0*R[ri];


        (*A)[e][pi+1] = 2.0*(R[pi+1]-R[ci+1]);
        (*A)[e][pi+2] = 2.0*(R[pi+2]-R[ci+2]);
        (*A)[e][pi+3] = 2.0*(R[pi+3]-R[ci+3]);

        (*A)[e][ci+1] = -2.0*(R[pi+1]-R[ci+1]); 
        (*A)[e][ci+2] = -2.0*(R[pi+2]-R[ci+2]); 
        (*A)[e][ci+3] = -2.0*(R[pi+3]-R[ci+3]);


        (*A)[e][pk+1] = -2.0*(R[pk+1]-R[ci+1]);
        (*A)[e][pk+2] = -2.0*(R[pk+2]-R[ci+2]);
        (*A)[e][pk+3] = -2.0*(R[pk+3]-R[ci+3]);

        (*A)[e][ci+1] += 2.0*(R[pk+1]-R[ci+1]); 
        (*A)[e][ci+2] += 2.0*(R[pk+2]-R[ci+2]); 
        (*A)[e][ci+3] += 2.0*(R[pk+3]-R[ci+3]);


        e++;

      }
       pk++;
      }

      // constraint (P-C).(C-F) = 0.0;

      L[e] = coef*( (R[pi+1]-R[ci+1])*(R[ci+1]-tm[10])
            +(R[pi+2]-R[ci+2])*(R[ci+2]-tm[11])
            +(R[pi+3]-R[ci+3])*(R[ci+3]-tm[12]));

      (*A)[e][pi+1] = - coef*(R[ci+1]-tm[10]);
      (*A)[e][pi+2] = - coef*(R[ci+2]-tm[11]);
      (*A)[e][pi+3] = - coef*(R[ci+3]-tm[12]);

      (*A)[e][ci+1] =  coef*((R[ci+1]-tm[10]) - (R[pi+1]-R[ci+1]));
      (*A)[e][ci+2] =  coef*((R[ci+2]-tm[11]) - (R[pi+2]-R[ci+2]));
      (*A)[e][ci+3] =  coef*((R[ci+3]-tm[12]) - (R[pi+3]-R[ci+3]));

      e++;


      pi += 3;

    } // end loop for each point

printf("vector R");
print_vector(R,1,nu);
printf(" vector L");
print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

     for(int j =1;j<=nu;j++) R[j] += D[j];

 printf("vector D");
print_vector(D,1,nu);

 printf(" loop - %d\n", loop);
    free_vector(D,1,nu);
 

  }// end loop for iteration

free_vector(L,1,ne);
free_matrix(A,1,ne,1,nu);
    
return(R);

}



VECTOR xcircle_by_edge(int np, PNTLST pnts, PNTLST tms )
{
  int i = 0;
  PHOTO_FLOAT big = 1.0e5;
/***
 // convert to real world system 
 
  for(i=0;i<np;i++) // for each line
  {
//     printf("tm vector\n");
//print_vector((*tms)[i],1,12);

    VECTOR inv = matinv((*tms)[i]); 
    copy_vector((*tms)[i],1,12,inv,1,12);
     

      rotpnt(inv, (*pnts)[i] );
printf("converted point");
 print_vector( (*pnts)[i],1,3);


    free_vector(inv,1,12);
 
  }
***/ 
  VECTOR C = centroid( np, pnts, tms);



  int nu = 7 + 4*np;
  int ne = 1 + 6*np;


  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);

  for(int loop = 0; loop < 100; loop++)
  {
    int e = 1;
    int ui = 1;      // unit vector normal to plane of circle
    int ri = ui + 3; // radius of circle
    int ci = ri+1;   // center of circle
    int gi = ci+3;
    int vi = gi+np;




    if(loop == 0)
    {

      R[ui] = 1.0;
      R[ui+1] = 1.0;
      R[ui+2] = 1.0;

      copy_vector(R,5,7+np,C,1,3+np);

      for(i=gi;i<vi;i++)R[i] = sqrt(R[i]);

      R[ri] = 0.0;  
      for(i = 0; i < np; i++)
      {
        VECTOR tm = (*tms)[i];
        VECTOR pnt = (*pnts)[i];

        R[ui] += pnt[1];
        R[ui+1] += pnt[2];
        R[ui+2] += pnt[3];
    
        PHOTO_FLOAT x = tm[10] + R[gi]*R[gi]*pnt[1] - R[ci];
        PHOTO_FLOAT y = tm[11] + R[gi]*R[gi]*pnt[2] - R[ci+1];
        PHOTO_FLOAT z = tm[12] + R[gi]*R[gi]*pnt[3] - R[ci+2];
        
        PHOTO_FLOAT r = sqrt(x*x + y*y + z*z);
       
        R[ri] += r/np;       

        R[vi]   = x/r; 
        R[vi+1] = y/r; 
        R[vi+2] = z/r; 


        vi += 3;
        gi++;
      }
      
      PHOTO_FLOAT ll = sqrt(R[ui]*R[ui] + R[ui+1]*R[ui+1] + R[ui+2]*R[ui+2]);
      R[ui] = R[ui]/ll;
      R[ui+1] = R[ui+1]/ll;
      R[ui+2] = R[ui+2]/ll;

      R[ri] = sqrt(R[i]);
      free_vector(C,1,3);
   
      gi = ci+3;
      vi = gi+np;


    }
 

/***
     for(i = 0; i < np; i++)
      {
        VECTOR tm = (*tms)[i];
        VECTOR pnt = (*pnts)[i];
    
        PHOTO_FLOAT x = tm[10] + R[gi]*R[gi]*pnt[1] - R[ci];
        PHOTO_FLOAT y = tm[11] + R[gi]*R[gi]*pnt[2] - R[ci+1];
        PHOTO_FLOAT z = tm[12] + R[gi]*R[gi]*pnt[3] - R[ci+2];
        
        //R[ri] += sqrt(x*x + y*y + z*z)/np;

        R[vi]   = x; 
        R[vi+1] = y; 
        R[vi+2] = z; 


       // vi += 3;
        gi++;
      }
***/  
      gi = ci+3;
      vi = gi+np;


/***
    // constraint 0 = r*r;

    L[e] = -R[ri]*R[ri];

    (*A)[e][ri] = 2.0*R[ri];

    e++;
***/

    // constraint u.u = 1
/***
   PHOTO_FLOAT ll = sqrt(R[ui]*R[ui] + R[ui+1]*R[ui+1] + R[ui+2]*R[ui+2]);

   R[ui] = R[ui]/ll;
   R[ui+1] = R[ui+1]/ll;
   R[ui+2] = R[ui+2]/ll;
***/

   L[e] = big*(1.0 -R[ui]*R[ui] - R[ui+1]*R[ui+1] - R[ui+2]*R[ui+2]);

    (*A)[e][ui]   = 2.0*big*R[ui];
    (*A)[e][ui+1] = 2.0*big*R[ui+1];
    (*A)[e][ui+2] = 2.0*big*R[ui+2];

    e++;
  
    for(i=0;i<np;i++) //for each point 
    {

      VECTOR tm = (*tms)[i];
      VECTOR pnt = (*pnts)[i];

 

      // constraint V.V = 1.0;

        PHOTO_FLOAT med = 1.0e3;
/***
       R[vi]   = tm[10] + R[gi]*R[gi]*pnt[1] - R[ci];
       R[vi+1] = tm[11] + R[gi]*R[gi]*pnt[2] - R[ci+1];
       R[vi+2] = tm[13] + R[gi]*R[gi]*pnt[3] - R[ci+2];
      
      PHOTO_FLOAT ll = sqrt(R[vi]*R[vi] + R[vi+1]*R[vi+1] + R[vi+2]*R[vi+2]);

      R[vi]   = R[vi]/ll;
      R[vi+1] = R[vi+1]/ll;
      R[vi+2] = R[vi+2]/ll;

***/


      L[e] = med*( 1.0 - R[vi]*R[vi] - R[vi+1]*R[vi+1] - R[vi+2]*R[vi+2]);

      //(*A)[e][ri] = -med*2.0*R[ri];    

      (*A)[e][vi]   = med*2.0*R[vi];    
      (*A)[e][vi+1] = med*2.0*R[vi+1];    
      (*A)[e][vi+2] = med*2.0*R[vi+2];    

      e++;




      // constraint V.U = 0

       med = 1.0e5;
      L[e] = med*( -R[vi]*R[ui] - R[vi+1]*R[ui+1] - R[vi+2]*R[ui+2]);

      (*A)[e][ui]   = med*R[vi];
      (*A)[e][ui+1] = med*R[vi+1];
      (*A)[e][ui+2] = med*R[vi+2];

      (*A)[e][vi]   = med*R[ui];
      (*A)[e][vi+1] = med*R[ui+1];
      (*A)[e][vi+2] = med*R[ui+2];

      e++ ;    


      // constraint (C + r*v - F - g*f)*f = 0.0
  
      med = 1.0e3;

      L[e] =med*( (R[ci]   + R[ri]*R[ri]*R[vi]   - tm[10] - R[gi]*R[gi]*pnt[1])*pnt[1]
           + (R[ci+1] + R[ri]*R[ri]*R[vi+1] - tm[11] - R[gi]*R[gi]*pnt[2])*pnt[2]
           + (R[ci+2] + R[ri]*R[ri]*R[vi+2] - tm[12] - R[gi]*R[gi]*pnt[3])*pnt[3] );

      (*A)[e][ci]   = -med*pnt[1];
      (*A)[e][ci+1] = -med*pnt[2];
      (*A)[e][ci+2] = -med*pnt[3];

      (*A)[e][ri] = -med*2.0*R[ri]*( R[vi]*pnt[1] + R[vi+1]*pnt[2] + R[vi+2]*pnt[3]);

      (*A)[e][gi] = med*2.0*R[gi]*( pnt[1]*pnt[1] + pnt[2]*pnt[2]  + pnt[3]*pnt[3]);

      (*A)[e][vi]   = -med*R[ri]*R[ri]*pnt[1];
      (*A)[e][vi+1] = -med*R[ri]*R[ri]*pnt[2];
      (*A)[e][vi+2] = -med*R[ri]*R[ri]*pnt[3];

      e++;


       // constraint (C + r*v - F - g*f) = 0.0
     
      
      L[e]   = R[ci]   + R[ri]*R[ri]*R[vi]   - tm[10] - R[gi]*R[gi]*pnt[1]; 
      L[e+1] = R[ci+1] + R[ri]*R[ri]*R[vi+1] - tm[11] - R[gi]*R[gi]*pnt[2]; 
      L[e+2] = R[ci+2] + R[ri]*R[ri]*R[vi+2] - tm[12] - R[gi]*R[gi]*pnt[3]; 


      (*A)[e][ci]     = -1.0;
      (*A)[e+1][ci+1] = -1.0;
      (*A)[e+2][ci+2] = -1.0;

      (*A)[e][ri]   = -2.0*R[ri]*R[vi];
      (*A)[e+1][ri] = -2.0*R[ri]*R[vi+1];
      (*A)[e+2][ri] = -2.0*R[ri]*R[vi+2];

      (*A)[e][vi]     = -R[ri]*R[ri];
      (*A)[e+1][vi+1] = -R[ri]*R[ri];
      (*A)[e+2][vi+2] = -R[ri]*R[ri];

      (*A)[e][gi]    = 2.0*R[gi]*pnt[1];
      (*A)[e+1][gi]  = 2.0*R[gi]*pnt[2];
      (*A)[e+2][gi]  = 2.0*R[gi]*pnt[3];

      e += 3;
      
  


      gi++;

      vi +=3;

    } // end loop for each point

printf("vector R");
print_vector(R,1,nu);
printf(" vector L");
print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

     ui = 1;
     ri = ui + 3;
     ci = ri+1;
     gi = ci+3;
     vi = gi+np;
 

      R[ui]   += D[ui];
      R[ui+1] += D[ui+1];
      R[ui+2] += D[ui+2];


      R[ci]   += D[ci];
      R[ci+1] += D[ci+1];
      R[ci+2] += D[ci+2];


    R[ri] += D[ri];

    if( R[ri] < 0 )R[ri] *= -1.0;

    for(i=gi;i<vi;i++)
    {
      R[i] += D[i];
      if(R[i] < 0.0) R[i] = -R[i];
    }

   for(i=vi;i<= nu;i++)R[i] += D[i];

printf("vector D");
print_vector(D,1,nu);

 printf(" loop - %d\n", loop);
    free_vector(D,1,nu);
 

  }// end loop for iteration

  free_matrix(A,1,ne,1,nu);

  for(i = 5;i < nu; i += 6)
  {
    printf(" r = %e\n",sqrt(L[i]*L[i] +L[i+1]*L[i+1] + L[i+2]*L[1+2] ) );
  } 
//getchar();

  free_vector(L,1,ne);

 // VECTOR sph = clone_vector(R,1,4,1,4);

 // free_vector(R,1,nu);

  return(R);
} 






VECTOR con_circles(int nl, int *nll,int *npl, PNTLST *pnts,PNTLST tms,PNTLST centers)
{

  int tnc = 0;   // total number of 2d circles;
  int tnp = 0;   // total number of points;

  int k = 0;
  int i = 0;
  int j = 0;
  int m = 0;
  int e = 0;

   PHOTO_FLOAT big = 1.0e5;
  

 // nl =2;

// convert to real world system 
  k=-1;
  for(i=0;i<nl;i++) // for each 3d line
  {
    for(j=0;j< nll[i]; j++) // for each 2d line defining 3d line
    {
      tnc++;
      k++;
      printf("tm vector\n");
print_vector((*tms)[k],1,12);

      VECTOR inv = matinv((*tms)[k]); 
      copy_vector((*tms)[k],1,12,inv,1,12);
     
      for(m=0;m<npl[k];m++)
      {
        tnp++;
        rotpnt(inv, (*(pnts[k]))[m] );
printf("converted point");
 print_vector( (*(pnts[k]))[m],1,3);
     }

      free_vector(inv,1,12);
    }

  }

  // setup to call circle by edge for each circle to use as initial conditions
  k = -1;
  m = 0;
  PNTLST C = matrix_shell(0,nl-1,1,10);
  PNTLST tt = matrix_shell( 0,tnp-1,1,12);
  PNTLST pp = matrix_shell( 0,tnp-1,1,3);
  int ii = 0;
  for(i=0;i<nl;i++)
  {
    m = 0;
    PNTLST p = NULL;
    for(j=0;j<nll[i];j++)
    {
      k++; //2d line index

      p = pnts[k];
   
      for(ii = 0;ii < npl[k];ii++)
      { 
        (*tt)[m] = (*tms)[k];
        (*pp)[m++]= (*p)[ii];
      }
    }

	 (*C)[i] = circle_by_edge( m, pp, tt ,(*centers)[i+1],(*centers)[i+1]);

  /*****  contents of C
    int ui = 1;
    int ri = ui + 3;
    int ci = ri+1;
    int gi = ci+3;
    int vi = gi+np;
  ****/


  }

//  VECTOR circle_by_edge( m, PNTLST pnts, PNTLST tms )
 



//printf("calling norm_circles \n");
//  norm_circ(npl[0],pnts[0],(*tms)[0],npl[1],pnts[1],(*tms)[1]);


  int nu = 3 + 3 + nl + nl + 3*tnp + tnp;
  int ne = 2 + 5*tnp;

printf(" tnc = %d, tnp = %d, nu = %d, ne = %d\n",tnc,tnp,nu,ne);


  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);
//    VECTOR w1 = NULL;
//    VECTOR w2 = NULL;

  int cp = 0;
  for(int loop = 0; loop < 200; loop++)
  {


    cp = 0;
    e =1;
    int ui = 1;          // normal vector;
    int ci = ui + 3;     // center point variable, one 3d point
    int si = ci + 3;     // distance along normal to circle centers, one for each circle except first
    int ri = si + nl;    // circle radius, one per circle;
    int vi = ri + nl;    // vector from center to radial point, one per 2d point
    int gi = vi +3*tnp;  // distance along view line to radial point, one per 2d point;

//    int ni = gi + tnp;
//    int wi = ni + tnc;
//    int qi = wi + 3*tnc;

//    int pi = qi + tnc;
//    int mi = pi + 3*tnp;

 


   if(loop == 0 )
    {

      int ui = 1;          // normal vector;
      int ci = ui + 3;     // center point variable, one 3d point
      int si = ci + 3;     // distance along normal to circle centers, one for each circle except first
      int ri = si + nl;    // circle radius, one per circle;
      int vi = ri + nl;    // vector from center to radial point, one per 2d point
      int gi = vi +3*tnp;  // distance along view line to radial point, one per 2d point;

      for(i=1; i<= nu; i++)R[i] = 1.0 + i/100.0;

      R[ui]   = (*C)[0][1];
      R[ui+1] = (*C)[0][2];
      R[ui+2] = (*C)[0][3];

      R[ci]   = (*C)[0][5];
      R[ci+1] = (*C)[0][6];
      R[ci+2] = (*C)[0][7];

      for(i=si;i<ri;i++)R[i] = 1.0;

      for(i= ri; i< vi; i++) R[i] = (*C)[0][4];

      int iv = vi;
      int ig = gi;
      int cp = 0;      

      for(int ln = 0; ln < nl; ln++) // loop over 3d circles;
      {
        int igk = 0;
        for(int nv = 0; nv < nll[ln]; nv++) // loop over 2d circles per 3d circle
        {

          PNTLST cpnts = pnts[cp];  // list of points for this circle/view
          VECTOR tm = (*tms)[cp];   // tmatrix for this circle/view

          for(int pn = 0; pn < npl[cp]; pn++) // loop over points per this 2d circle
          {
             VECTOR pnt = (*cpnts)[pn];

             R[ig] = sqrt( (*C)[ln][8+(igk++)] );
             R[iv] = tm[10] + R[ig]*R[ig]*pnt[1]  - R[ci];
             R[iv+1] = tm[11] + R[ig]*R[ig]*pnt[2]  - R[ci];
             R[iv+2] = tm[12] + R[ig]*R[ig]*pnt[3]  - R[ci];

             iv += 3;
             ig++;

      //for(i=vi;i< gi;i++)R[i] = 1.0;

      //for(i=gi; i <= nu;i++)R[i] = sqrt( (*C)[0][8+i-gi] );
          }
          cp++;
        }

      }

     //for(i=vi;i< gi;i++)R[i] = 1.0;

      //   free_vector(tmp,1,4);



      //   free_vector(tmp,1,4);


    }

    for(i=ri;i<vi;i++)
    {
      if(R[i] < 0.0)R[i] = -R[i];
    }

    // constraint U.U = 1;

    PHOTO_FLOAT temp = sqrt(R[ui]*R[ui] + R[ui+1]*R[ui+1] + R[ui+2]*R[ui+2]);

    R[ui]   = R[ui]/temp;
    R[ui+1] = R[ui+1]/temp;
    R[ui+2] = R[ui+2]/temp;
   
    L[e] = big*(1.0 - R[ui]*R[ui] - R[ui+1]*R[ui+1] - R[ui+2]*R[ui+2]);  

    (*A)[e][ui]   = big*(2.0*R[ui]);
    (*A)[e][ui+1] = big*(2.0*R[ui+1]);
    (*A)[e][ui+2] = big*(2.0*R[ui+2]);
  
    e++;
 

    cp =0;
    for(int ln = 0; ln < nl; ln++) // loop over 3d circles;
    {
      if(ln == 0)
      {
         //constraint s for first circle = 0.0;

         R[si] = 0.0;

         L[e] = big*R[si];
   
         (*A)[e][si] = -big*1.0;

          e++;
      }
      
    
 

      for(int nv = 0; nv < nll[ln]; nv++) // loop over 2d circles per 3d circle
      {

        PNTLST cpnts = pnts[cp];  // list of points for this circle/view
        VECTOR tm = (*tms)[cp];   // tmatrix for this circle/view



        for(int pn = 0; pn < npl[cp]; pn++) // loop over points per this 2d circle
        {
          VECTOR pnt = (*cpnts)[pn];



         // constraint V.U = 0


         PHOTO_FLOAT temp = R[vi]*R[ui] + R[vi+1]*R[ui+1] + R[vi+2]*R[ui+2];
         R[vi]   = R[vi]   - temp*R[ui];
         R[vi+1] = R[vi+1] - temp*R[ui+1];
         R[vi+2] = R[vi+2] - temp*R[ui+2];

         PHOTO_FLOAT med = 1.0e5;
         L[e] = med*( -R[vi]*R[ui] - R[vi+1]*R[ui+1] - R[vi+2]*R[ui+2]);

         (*A)[e][ui]   = med*R[vi];
         (*A)[e][ui+1] = med*R[vi+1];
         (*A)[e][ui+2] = med*R[vi+2];

         (*A)[e][vi]   = med*R[ui];
         (*A)[e][vi+1] = med*R[ui+1];
         (*A)[e][vi+2] = med*R[ui+2];

         e++ ;    


          // constraint V.V = 1.0;

          med = 1.0e5;

          temp = sqrt(R[vi]*R[vi] + R[vi+1]*R[vi+1] + R[vi+2]*R[vi+2]);
   
          R[vi]   = R[vi]/temp;
          R[vi+1] = R[vi+1]/temp;
          R[vi+2] = R[vi+2]/temp;

          L[e] = med*( 1.0 - R[vi]*R[vi] - R[vi+1]*R[vi+1] - R[vi+2]*R[vi+2]);

    
         (*A)[e][vi]   = med*2.0*R[vi];    
         (*A)[e][vi+1] = med*2.0*R[vi+1];    
         (*A)[e][vi+2] = med*2.0*R[vi+2];    

         e++;




        // constraint (C +s*U + r*v - F - g*f) = 0.0
     
    
         L[e]   = R[ci]   + R[si]*R[ui]   + R[ri]*R[ri]*R[vi]   - tm[10] - R[gi]*R[gi]*pnt[1]; 
         L[e+1] = R[ci+1] + R[si]*R[ui+1] + R[ri]*R[ri]*R[vi+1] - tm[11] - R[gi]*R[gi]*pnt[2]; 
         L[e+2] = R[ci+2] + R[si]*R[ui+2] + R[ri]*R[ri]*R[vi+2] - tm[12] - R[gi]*R[gi]*pnt[3]; 

         (*A)[e][ci]     = -1.0;
         (*A)[e+1][ci+1] = -1.0;
         (*A)[e+2][ci+2] = -1.0;

         (*A)[e][si]   = -R[ui];
         (*A)[e+1][si] = -R[ui+1];
         (*A)[e+2][si] = -R[ui+2];

         (*A)[e][ui]   = -R[si];
         (*A)[e][ui+1] = -R[si];
         (*A)[e][ui+2] = -R[si];

         (*A)[e][ri]   = -2.0*R[ri]*R[vi];
         (*A)[e+1][ri] = -2.0*R[ri]*R[vi+1];
         (*A)[e+2][ri] = -2.0*R[ri]*R[vi+2];

         (*A)[e][vi]     = -R[ri]*R[ri];
         (*A)[e+1][vi+1] = -R[ri]*R[ri];
         (*A)[e+2][vi+2] = -R[ri]*R[ri];

         (*A)[e][gi]    = 2.0*R[gi]*pnt[1];
         (*A)[e+1][gi]  = 2.0*R[gi]*pnt[2];
         (*A)[e+2][gi]  = 2.0*R[gi]*pnt[3];

          e += 3;
      

         gi++;

         vi +=3;


       } // end loop over pnts in 2d circle

        cp++;
      } // end loop over 2d circles

      ri++;
      si++;
    }  // end loop over 3d circles


    printf("vector R");
    print_vector(R,1,nu);

    printf("vector L");
    print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

    for(i=1; i<= nu; i++) R[i] += 0.5*D[i];

    printf("vector D");
    print_vector(D,1,nu);

    printf("end loop %d\n",loop);
    //getchar();


  }// end loop for iteration

  free_matrix(A,1,ne,1,nu);
  free_vector(L,1,ne);
    int ui = 1;          // normal vector;
    int ci = ui + 3;     // center point variable, one 3d point
    int si = ci + 3;     // distance along normal to circle centers, one for each circle except first
    int ri = si + nl;  // circle radius, one per circle;
    int vi = ri + nl;    // vector from center to radial point, one per 2d point
    int gi = vi +3*tnp;  // distance along view line to radial point, one per 2d point;

  //  int ni = gi + tnp;
  //  int wi = ni + tnc;
  //  int qi = wi + 3*tnc;

  //  int pi = qi + tnc;
  //  int mi = pi + 3*tnp;


  VECTOR circles = clone_vector(R,1,vi-1,1,vi-1);

  free_vector(R,1,nu);

  return(circles);
}












VECTOR sphere_by_edge(int np, PNTLST pnts, PNTLST tms )
{
  int i = 0;
  PHOTO_FLOAT big = 1.0e5;

 // convert to real world system 
 
  for(i=0;i<np;i++) // for each line
  {
//     printf("tm vector\n");
//print_vector((*tms)[i],1,12);

    VECTOR inv = matinv((*tms)[i]); 
    copy_vector((*tms)[i],1,12,inv,1,12);
     

      rotpnt(inv, (*pnts)[i] );
printf("converted point");
 print_vector( (*pnts)[i],1,3);


    free_vector(inv,1,12);
 
  }
 
  VECTOR C = centroid( np, pnts, tms);



  int nu = 4 + 4*np;
  int ne = 5*np;


  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);

  for(int loop = 0; loop < 100; loop++)
  {
    int e = 1;
    int ri = 1;
    int ci = ri+1;
    int gi = ci+3;
    int vi = gi+np;

    if(loop == 0)
    {

      copy_vector(R,2,4+np,C,1,3+np);

      for(i=gi;i<vi;i++)R[i] = sqrt(R[i]);

      R[ri] = 0.0;  
      for(i = 0; i < np; i++)
      {
        VECTOR tm = (*tms)[i];
        VECTOR pnt = (*pnts)[i];
    
        PHOTO_FLOAT x = tm[10] + R[gi]*R[gi]*pnt[1] - R[ci];
        PHOTO_FLOAT y = tm[11] + R[gi]*R[gi]*pnt[2] - R[ci+1];
        PHOTO_FLOAT z = tm[12] + R[gi]*R[gi]*pnt[3] - R[ci+2];
        
        R[ri] += sqrt(x*x + y*y + z*z)/np;

        R[vi]   = x; 
        R[vi+1] = y; 
        R[vi+2] = z; 

        vi += 3;
        gi++;
      }
      

      free_vector(C,1,3);
   
      gi = ci+3;
      vi = gi+np;


    }
 
    
  
    for(i=0;i<np;i++) //for each point 
    {

      VECTOR tm = (*tms)[i];
      VECTOR pnt = (*pnts)[i];

      // constraint F+g*f = C + V 

      L[e]   = tm[10] + R[gi]*R[gi]*pnt[1] - R[vi] - R[ci];
      L[e+1] = tm[11] + R[gi]*R[gi]*pnt[2] - R[vi+1] - R[ci+1];
      L[e+2] = tm[12] + R[gi]*R[gi]*pnt[3] - R[vi+2] - R[ci+2];

      (*A)[e][ci]     =  1.0;
      (*A)[e+1][ci+1] =  1.0;
      (*A)[e+2][ci+2] =  1.0;

      (*A)[e][gi]   = -2.0*R[gi]*pnt[1];      
      (*A)[e+1][gi] = -2.0*R[gi]*pnt[2];      
      (*A)[e+2][gi] = -2.0*R[gi]*pnt[3];      

      (*A)[e][vi]     = 1.0;       
      (*A)[e+1][vi+1] = 1.0;       
      (*A)[e+2][vi+2] = 1.0;


      e+=3;

      // constraint V.V = r*r;

      L[e] = R[ri]*R[ri] - R[vi]*R[vi] - R[vi+1]*R[vi+1] - R[vi+2]*R[vi+2];

      (*A)[e][ri] = -2.0*R[ri];    

      (*A)[e][vi]   = 2.0*R[vi];    
      (*A)[e][vi+1] = 2.0*R[vi+1];    
      (*A)[e][vi+2] = 2.0*R[vi+2];    

      e++;

      // constraint v.f = 0

      L[e] = big*( -R[vi]*pnt[1] - R[vi+1]*pnt[2] - R[vi+2]*pnt[3]);

      (*A)[e][vi]   = big*pnt[1];
      (*A)[e][vi+1] = big*pnt[2];
      (*A)[e][vi+2] = big*pnt[3];

      e++;
      
      gi++;

      vi +=3;

    } // end loop for each point

printf("vector R");
print_vector(R,1,nu);
printf(" vector L");
print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);


     ri = 1;
     ci = ri+1;
     gi = ci+3;
     vi = gi+np;

 
    R[ci]   += D[ci];
    R[ci+1] +=  D[ci+1];
    R[ci+2] +=  D[ci+2];
    
    R[ri] += D[ri];

    if( R[ri] < 0 )R[ri] *= -1.0;

    for(i=gi;i<vi;i++)
    {
      R[i] += D[i];
      if(R[i] < 0.0) R[i] = -R[i];
    }

    for(i=vi;i<= nu;i++)R[i] += D[i];

printf("vector D");
print_vector(D,1,nu);

//getchar();
    free_vector(D,1,nu);
 

  }// end loop for iteration

  free_matrix(A,1,ne,1,nu);
  free_vector(L,1,ne);
  


  VECTOR sph = clone_vector(R,1,4,1,4);

  free_vector(R,1,nu);

  return(sph);
} 






int make_sphere_by_edge(int nNumPnts, PNTLST plPnts,
    PNTLST plTMatrices, struct Sphere &sph, struct _transform &trans )	    
{

   if(setjmp(env) != 0)
  {
   return(1);
  }

   VECTOR s = sphere_by_edge( nNumPnts-2, plPnts, plTMatrices );
   sph.diameter = 2.0*s[1];



  trans.pozition.pos_x = s[2];
  trans.pozition.pos_y = s[3];
  trans.pozition.pos_z = s[4];

  trans.rotation.r[0]= 1.0;
  trans.rotation.r[1]= 0.0;
  trans.rotation.r[2]= 0.0;

  trans.rotation.r[3]= 0.0;

  trans.rotation.r[4]= 0.0;
  trans.rotation.r[5]= 1.0;
  trans.rotation.r[6]= 0.0;

  trans.rotation.r[7]= 0.0;


  trans.rotation.r[8]= 0.0;
  trans.rotation.r[9]= 0.0;
  trans.rotation.r[10]= 1.0;

  trans.rotation.r[11]= 0.0;


  trans.rotation.r[12]= 0.0;
  trans.rotation.r[13]= 0.0;
  trans.rotation.r[14]= 0.0;

  trans.rotation.r[15]= 1.0;

  free_vector(s,1,4);


  return(0);
}





int MakeCirc( int                 nl,    //number of 2d arcs ( number of photos)
	       int                *npl,   //number of points per each 2d arc,
               PNTLST             *pnts,  // list of 2d points 
	       PNTLST              tms,   // Tmatrix, one per photo;
               double            &radius,    
               struct _transform &trans )


{

   if(setjmp(env) != 0)
  {
   return(1);
  }

  int k = 0;

  struct Cylinder cyl;

  PNTLST  center = matrix(1,1,1,3);

  PNTLST cnt = matrix(1,nl,1,3);
  PNTLST mtx = matrix(1,nl,1,13);
  PNT t1 = vector(1,3);
  PNT t2 = vector(1,3);
  VECTOR uv = vector(1,3);

  for(int i = 1; i<=nl; i++)
  {
    PNTLST R = find_ellipse( npl[k], pnts[k]);
    (*cnt)[i][1] = (*R)[1][1];
    (*cnt)[i][2] = (*R)[1][2];
    (*cnt)[i][3] = (*R)[1][3];

    copy_vector((*mtx)[i],1,13,(*tms)[i-1],1,13);

    VECTOR inv = matinv((*mtx)[i]);

    if(i==1)
    {
      copy_vector(t1,1,3,(*R)[2],1,3);
      rotpnt(inv,t1);
   
      copy_vector(t2,1,3,(*R)[3],1,3);
      rotpnt(inv,t2);

    }
    
    if(i==2)
    {

      PNT tt1 = vector(1,3);
      PNT tt2 = vector(1,3);

      copy_vector(tt1,1,3,(*R)[2],1,3);
      rotpnt(inv,tt1);
   
      copy_vector(tt2,1,3,(*R)[3],1,3);
      rotpnt(inv,tt2);

      PHOTO_FLOAT tmp = t1[1]*tt1[1] + t1[2]*tt1[2] + t1[3]*tt1[3];
      if(tmp<0.0)tmp=-tmp;

        uv[1] = (t1[1]+tt1[1])/2.0;
        uv[2] = (t1[2]+tt1[2])/2.0;
        uv[3] = (t1[3]+tt1[3])/2.0;


printf("dotprod = %e\n",tmp);    
      PHOTO_FLOAT tmp2 = t1[1]*tt2[1] + t1[2]*tt2[2] + t1[3]*tt2[3];
      if(tmp2<0.0)tmp2=-tmp2;
printf("dotprod = %e\n",tmp2);    
      if(tmp2 > tmp)
      {
        tmp = tmp2;
        uv[1] = (t1[1]+tt2[1])/2.0;
        uv[2] = (t1[2]+tt2[2])/2.0;
        uv[3] = (t1[3]+tt2[3])/2.0;
      }

       tmp2 = t2[1]*tt1[1] + t2[2]*tt1[2] + t2[3]*tt1[3];
       if(tmp2<0.0)tmp2=-tmp2;
printf("dotprod = %e\n",tmp2);    
      if(tmp2 > tmp)
      {
        tmp = tmp2;
        uv[1] = (t2[1]+tt1[1])/2.0;
        uv[2] = (t2[2]+tt1[2])/2.0;
        uv[3] = (t2[3]+tt1[3])/2.0;
      }

       tmp2 = t2[1]*tt2[1] + t2[2]*tt2[2] + t2[3]*tt2[3];
      if(tmp2<0.0)tmp2=-tmp2;
printf("dotprod = %e\n",tmp2);    
      if(tmp2 > tmp)
      {
        tmp = tmp2;
        uv[1] = (t2[1]+tt2[1])/2.0;
        uv[2] = (t2[2]+tt2[2])/2.0;
        uv[3] = (t2[3]+tt2[3])/2.0;
      }

      free_vector(tt1,1,3);
      free_vector(tt2,1,3);
    }
 
    if(i>2)
    {
      PNT tt1 = vector(1,3);
      PNT tt2 = vector(1,3);

      copy_vector(tt1,1,3,(*R)[2],1,3);
      rotpnt(inv,tt1);
   
      copy_vector(tt2,1,3,(*R)[3],1,3);
      rotpnt(inv,tt2);

      
      PHOTO_FLOAT tmp = uv[1]*tt1[1] + uv[2]*tt1[2] + uv[3]*tt1[3];
      if(tmp <0.0)tmp=-tmp;
printf("dotprod = %e\n",tmp);    
    
      PHOTO_FLOAT tmp2 = uv[1]*tt2[1] + uv[2]*tt2[2] + uv[3]*tt2[3];
      if(tmp2<0.0)tmp2=-tmp2;
printf("dotprod = %e\n",tmp2);    

      if(tmp > tmp2)
      {
        uv[1] = (uv[1]+tt1[1])/2.0;
        uv[2] = (uv[2]+tt1[2])/2.0;
        uv[3] = (uv[3]+tt1[3])/2.0;
      }
      else
      {
        uv[1] = (uv[1]+tt2[1])/2.0;
        uv[2] = (uv[2]+tt2[2])/2.0;
        uv[3] = (uv[3]+tt2[3])/2.0;
      }
      free_vector(tt1,1,3);
      free_vector(tt2,1,3);


   }
    
        

    free_vector(inv,1,12);
    free_matrix(R,1,3,1,3);
    k++;
  }

  free_vector(t1,1,3);
  free_vector(t2,1,3);

  (*center)[1] =  get3dpnt(nl,cnt,*mtx);
print_matrix(center,1,1,1,3);
//getchar();


// convert to real world system 


  int tnp = 0;
  k=-1;
  for(int j=0;j< nl; j++) // for each 2d line defining 3d line
  {
    //tnc++;
    k++;
    printf("tm vector\n");
//print_vector((*tms)[k],1,12);

    VECTOR inv = matinv((*tms)[k]); 
    copy_vector((*tms)[k],1,12,inv,1,12);
     
    for(int m=0;m<npl[k];m++)
    {
      tnp++;
      rotpnt(inv, (*(pnts[k]))[m] );
//printf("converted point");
// print_vector( (*(pnts[k]))[m],1,3);
   }

    free_vector(inv,1,12);
  }

  

  // setup to call circle by edge for each circle to use as initial conditions
  k = -1;
  int m = 0;
 // PNTLST C = matrix_shell(0,nl-1,1,10);
  PNTLST tt = matrix_shell( 0,tnp-1,1,12);
  PNTLST pp = matrix_shell( 0,tnp-1,1,3);
  int ii = 0;
  m = 0;
  PNTLST p = NULL;
  for(j=0;j<nl;j++)
  {
    k++; //2d line index

    p = pnts[k];
   
    for(ii = 0;ii < npl[k];ii++)
    { 
      (*tt)[m] = (*tms)[k];
      (*pp)[m++]= (*p)[ii];
    }
  }

  VECTOR C = circle_by_edge( m, pp, tt ,(*center)[1],uv);
  free_vector(uv,1,3);

  /*****  contents of C
    int ui = 1;
    int ri = ui + 3;
    int ci = ri+1;
    int gi = ci+3;
    int vi = gi+np;
  ****/


 // void make_cyl_by_endpnts(PHOTO_FLOAT radius,PNT pnt1,PNT pnt2,struct tCylinder &cyl,struct _transform &trans)


  PNT pt1 = vector(1,3);
  PNT pt2 = vector(1,3);


  pt1[1] = C[5] + C[1];
  pt1[2] = C[6] + C[2];
  pt1[3] = C[7] + C[3];


  pt2[1] = C[5] - C[1];
  pt2[2] = C[6] - C[2];
  pt2[3] = C[7] - C[3];


  radius = C[4];

Output( 0, "MakeCirc() - radius = %lf\n", radius );
Output( 0, "MakeCirc() - pt1 = %lf, %lf, %lf\n", pt1[1], pt1[2], pt1[3] );
Output( 0, "MakeCirc() - pt2 = %lf, %lf, %lf\n", pt2[1], pt2[2], pt2[3] );
  make_cyl_by_endpnts(radius,pt1,pt2,cyl,trans);

    //VECTOR con_circles(int nl, int *nll,int *npl, PNTLST *pnts,PNTLST tms,PNTLST centers)

   // VECTOR cirs = con_circles(1,&nl,npl, pnts, tms, center);

//printf("end end end end  RADIUS = %e\n",radius);
//getchar();



  return(0);
}




int make_rtorus_by_edge(int *nll,int *npl, PNTLST *pnts,PNTLST tms,
                        PNTLST sep, PNTLST setm , struct rect_torus &rtor,
                        struct _transform &trans)
{

   if(setjmp(env) != 0)
  {
   return(1);
  }

  PNTLST  centers = matrix(1,3,1,3);



  int k = 0;
  for(int j =0;j<3;j++)
  {
    PNTLST cnt = matrix(1,nll[j],1,3);
    PNTLST mtx = matrix(1,nll[j],1,13);
    for(int i = 1; i<=nll[j]; i++)
    {
      PNTLST R = find_ellipse( npl[k], pnts[k]);
      (*cnt)[i][1] = (*R)[1][1];
      (*cnt)[i][2] = (*R)[1][2];
      (*cnt)[i][3] = (*R)[1][3];

      copy_vector((*mtx)[i],1,13,(*tms)[i-1],1,13);
      free_matrix(R,1,3,1,3);
      k++;
    }

     (*centers)[j+1] =  get3dpnt(nll[j],cnt,*mtx);
print_vector((*centers)[j+1],1,3);
//getchar();

  }


  //  VECTOR C = circle_by_edge(int np, PNTLST pnts, PNTLST tms )

 //   VECTOR cirs = con_circles(3,nll,npl, pnts, tms ,centers);

printf("end end end end \n");
//getchar();

  int tnl = 0;
  for(int i = 0; i< 3; i++)
  {
    
    VECTOR cirs = con_circles(3,&nll[i],&npl[tnl], &pnts[tnl], &tms[tnl], centers);
printf("end end end end \n");
//getchar();
    tnl += nll[i];
  }

  return(0);
}











int make_snout_by_circle(int *nll,int *npl, PNTLST *pnts,PNTLST tms, struct tSnout &snt,
                        struct _transform &trans)
{
   if(setjmp(env) != 0)
  {
   return(1);
  }
  return(0);
}









PNTLST torus_by_edge( int np, PNTLST pnts, PNTLST tms)
{

  int i = 0;
  int loop = 0;
 PHOTO_FLOAT sq3 = sqrt(3.0); 
  PHOTO_FLOAT sq2 = sqrt(2.0);
  PHOTO_FLOAT CR = 0.0;
  int mflag = 0;
  int mdelta = 50;
  int mnext = 50;

  int nps[100];
  for(i=0;i<100;i++)nps[i]=0;
  int nv = 0; // number of views

  for(i=0;i<np;i++)
  {
    PHOTO_FLOAT tt;
    if(i==0)tt = (*tms)[i][10];

    if(tt != (*tms)[i][10])
    {
      tt = (*tms)[i][10];
      nv++;
    }
    nps[nv]++;
  }
  nv++;

printf("number of views = %d,  %d,%d\n", nv,nps[0],nps[1]);

  PNTLST  centers = matrix(1,3,1,3);



   int k = 0;
  for(int j =0;j<1;j++)
  {
    PNTLST cnt = matrix(1,nv,1,3);
    PNTLST mtx = matrix(1,nv,1,13);
    for(int i = 1; i<=nv; i++)
    {
      PNTLST spnts = submatrix(pnts,k,k+nps[k],1,3,0,1);
      PNTLST R = find_ellipse( nps[k], spnts);
      free_submatrix(spnts,0,nps[k],1,3);
      (*cnt)[i][1] = (*R)[1][1];
      (*cnt)[i][2] = (*R)[1][2];
      (*cnt)[i][3] = (*R)[1][3];

      copy_vector((*mtx)[i],1,13,(*tms)[i-1],1,13);
      free_matrix(R,1,3,1,3);
      k++;
    }

     (*centers)[j+1] =  get3dpnt(nv,cnt,*mtx);
print_vector((*centers)[j+1],1,3);
//getchar();

  }









// VECTOR circle_by_edge(int np, PNTLST pnts, PNTLST tms )

 VECTOR C = circle_by_edge( np, pnts,  tms , (*tms)[1],(*tms)[1] );
/**
  cir 
    int ui = 1;
    int ri = ui + 3;
    int ci = ri+1;
    int gi = ci+3;
    int vi = gi+np;
***/




   int nu = 7 + 4*np;
   int ne = 5 + 5*np;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);

  for( loop = 0; loop < 200; loop++)
  {

     int e = 1;

    int ci = 1;
    int ui = ci + 3;
    int ri = ui + 3;
    int vi = ri + 1;
    int gi = vi + 3*np;

    if(loop == 0)
    {
      for(i=ci;i<ui;i++)R[i] = C[i];
      for(i=ui;i<ri;i++)R[i] = 1.0-i/100.0;
      R[ri] = 1;
      for(i=vi;i<gi;i++)R[i] = 1.0-i/100.0;
      for(i=gi;i<=nu;i++)R[i] = 1.0;


    }

   // constraint U.U = 1.0 i.e U is a unit vector

    
    L[e] = 1.0e5*(1.0 - R[ui]*R[ui] - R[ui+1]*R[ui+1] - R[ui+2]*R[ui+2]);

    (*A)[e][ui]   = 1.0e5*2.0*R[ui];
    (*A)[e][ui+1] = 1.0e5*2.0*R[ui+1];
    (*A)[e][ui+2] = 1.0e5*2.0*R[ui+2];

    e++;

    // constraint lock center point for first 50 iterations


    PHOTO_FLOAT ck = 1000.0;
    if(mflag == 1)ck = 0.1;

    L[e] =  ck*( C[1] - R[ci]);
    L[e+1] = ck*(C[2] - R[ci+1]);
    L[e+2] = ck*(C[3] - R[ci+2]);

    (*A)[e][ci]  = ck;
    (*A)[e+1][ci+1] = ck;
    (*A)[e+2][ci+2] = ck;

    e += 3;


    // constraint r = 0.0;

    PHOTO_FLOAT ckr = 0.001;
    if(mflag == 1)ckr = 1000.0;
   
    
    L[e] = ckr*(R[ri]*R[ri] - CR);

    (*A)[e][ri] = ckr*(-2.0*R[ri]);

    e++;


    for(i=0;i<np;i++) //for each point 
    {
      VECTOR tm = (*tms)[i];
      VECTOR pnt = (*pnts)[i];


      // constraint V.V = 1.0;

      L[e] = 1.0e5*(1.0 - R[vi]*R[vi] - R[vi+1]*R[vi+1] - R[vi+2]*R[vi+2]);

      (*A)[e][vi]   = 1.0e5*2.0*R[vi];
      (*A)[e][vi+1] = 1.0e5*2.0*R[vi+1];
      (*A)[e][vi+2] = 1.0e5*2.0*R[vi+2];

 
      e++;

      // constraint V.U = 0;
 
       L[e] = 1.0e5*(R[vi]*R[ui] + R[vi+1]*R[ui+1] + R[vi+2]*R[ui+2]);

       (*A)[e][vi]   = 1.0e5*(-R[ui]);
       (*A)[e][vi+1] = 1.0e5*(-R[ui+1]);
       (*A)[e][vi+2] = 1.0e5*(-R[ui+2]);

       (*A)[e][ui]   = 1.0e5*(-R[vi]);
       (*A)[e][ui+1] = 1.0e5*(-R[vi+1]);
       (*A)[e][ui+2] = 1.0e5*(-R[vi+2]);

       e++;
      
       // constraint C + r*V = F + g*f

       L[e]   = R[ci]   + R[ri]*R[ri]*R[vi]   - tm[10] - R[gi]*R[gi]*pnt[1];
       L[e+1] = R[ci+1] + R[ri]*R[ri]*R[vi+1] - tm[11] - R[gi]*R[gi]*pnt[2];
       L[e+2] = R[ci+2] + R[ri]*R[ri]*R[vi+2] - tm[12] - R[gi]*R[gi]*pnt[3];

       (*A)[e][ci]     = -1.0;
       (*A)[e+1][ci+1] = -1.0;
       (*A)[e+2][ci+2] = -1.0;

       (*A)[e][vi]     = -R[ri]*R[ri];
       (*A)[e+1][vi+1] = -R[ri]*R[ri];
       (*A)[e+2][vi+2] = -R[ri]*R[ri];

       (*A)[e][gi] = 2.0*R[gi]*pnt[1];
       (*A)[e+1][gi] = 2.0*R[gi]*pnt[2];
       (*A)[e+2][gi] = 2.0*R[gi]*pnt[3];

       (*A)[e][ri] = -2.0*R[ri]*R[vi];
       (*A)[e+1][ri] = -2.0*R[ri]*R[vi+1];
       (*A)[e+2][ri] = -2.0*R[ri]*R[vi+2];

       e += 3;
       

      vi += 3;
      gi++;

    
    } // end loop each point

printf("vector R");
print_vector(R,1,nu);

printf("vector L");
print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

printf("vector D");
print_vector(D,1,nu);

printf("end loop %d\n",loop);
//getchar();


     for(i=1;i<=nu;i++)R[i] += D[i];
     if( R[ri] < 0.0)R[ri] = -R[ri];
    free_vector(D,1,nu);
 
  if(loop > mnext)
  {
    mnext += mdelta;
    if(mflag == 1)
    {
      mflag = 0;
    }
    else
    {
      mflag = 1;
    }
printf("mflag = %d,  mnext = %d\n",mflag,mnext);

  }


  }// end loop for iteration

  free_matrix(A,1,ne,1,nu);
  free_vector(L,1,ne);


  for(i=5;i<ne;i += 5)
  {
    PHOTO_FLOAT rr = sqrt( L[i]*L[i] + L[i+1]*L[i+1]+ L[i+2]*L[i+2]);
    printf(" r = %e\n",rr);
  }


printf(" end circle check\n");

//getchar();



free_vector(C,1,3);

  C = R;
 




   nu = 8 + 6*np;
   ne = 1 + 7*np;
 


   L = vector(1,ne);
   A = matrix(1,ne,1,nu);
   R = vector(1,nu);

  for( loop = 0; loop < 50; loop++)
  {

     int e = 1;

    int ci = 1;
    int ui = ci + 3;
    int ri = ui + 3;
    int vi = ri + 2;
    int ai = vi + 3*np;
    int bi = ai + np;
    int gi = bi + np;

    if(loop == 0)
    {
      // initialize center
      for(i=ci;i< ui;i++)R[i] = C[i];
    
      // initialize normal vector;
      for(i=ui;i< ri;i+=3)R[i] = C[i];
/** 
     {
        R[i] = 1.0/sq3;
        R[i+1] = 1.0/sq3;
        R[i+2] = 1.0/sq3;
      }
**/
      // initialize radius
      R[ri] = C[7];
      R[ri+1] = 1.0;

      // initialize radius 1 unit vectors
      for(i=vi;i< ai;i+=3)R[i] = C[i-1];
/** 
     {
        R[i] = -1.0/sq3;
        R[i+1] = 1.0/sq3;
        R[i+2] = -1.0/sq3;
      }
**/
      // initialize radius 2 unit vectors
      for(i=ai;i< bi;i++)
      {
        R[i] = 1.0/sq2;
      }

      for(i=bi;i< gi;i++)
      {
        R[i] = 1.0/sq2;
      }

      // initialize radius 1 unit vectors
      for(i=gi;i<= nu;i++)R[i] = 10.0;

  
    }
 
 
    // constraint U.U = 1.0 i.e U is a unit vector

    
    L[e] = 1.0e5*(1.0 - R[ui]*R[ui] - R[ui+1]*R[ui+1] - R[ui+2]*R[ui+2]);

    (*A)[e][ui]   = 1.0e5*2.0*R[ui];
    (*A)[e][ui+1] = 1.0e5*2.0*R[ui+1];
    (*A)[e][ui+2] = 1.0e5*2.0*R[ui+2];

    e++;
 
    for(i=0;i<np;i++) //for each point 
    {

      //  F + g*pnt = C + r1*V + a*V + b*u;
 
      VECTOR tm = (*tms)[i];
      VECTOR pnt = (*pnts)[i];

      L[e]   = tm[10] + R[gi]*R[gi]*pnt[1] - (R[ri]*R[ri]+R[i+1]*R[i+1]+R[ai])*R[vi]   - R[bi]*R[ui] - R[ci]; 
      L[e+1] = tm[11] + R[gi]*R[gi]*pnt[2] - (R[ri]*R[ri]+R[i+1]*R[i+1]+R[ai])*R[vi+1] - R[bi]*R[ui+1]-R[ci+1]; 
      L[e+2] = tm[12] + R[gi]*R[gi]*pnt[3] - (R[ri]*R[ri]+R[i+1]*R[i+1]+R[ai])*R[vi+2] - R[bi]*R[ui+2]-R[ci+2]; 
      
   
      (*A)[e][ci]     = 1.0;
      (*A)[e+1][ci+1] = 1.0;
      (*A)[e+2][ci+2] = 1.0;
  
      (*A)[e][vi]     = R[ri]*R[ri]+R[i+1]*R[i+1]+R[ai];
      (*A)[e+1][vi+1] = R[ri]*R[ri]+R[i+1]*R[i+1]+R[ai];
      (*A)[e+2][vi+2] = R[ri]*R[ri]+R[i+1]*R[i+1]+R[ai];

      (*A)[e][ui]     = R[bi];
      (*A)[e+1][ui+1] = R[bi];
      (*A)[e+2][ui+2] = R[bi];

      (*A)[e][ri]   = 2.0*R[ri]*R[vi];
      (*A)[e+1][ri] = 2.0*R[ri]*R[vi+1];
      (*A)[e+2][ri] = 2.0*R[ri]*R[vi+2];

      (*A)[e][ri+1]   = 2.0*R[ri+1]*R[vi];
      (*A)[e+1][ri+1] = 2.0*R[ri+1]*R[vi+1];
      (*A)[e+2][ri+1] = 2.0*R[ri+1]*R[vi+2];


      (*A)[e][ai] = R[vi];
      (*A)[e+1][ai] = R[vi+1];
      (*A)[e+2][ai] = R[vi+2];

      (*A)[e][bi] = R[ui];
      (*A)[e+1][bi] = R[ui+1];
      (*A)[e+2][bi] = R[ui+2];

      (*A)[e][gi]   = -2.0*R[gi]*pnt[1];
      (*A)[e+1][gi] = -2.0*R[gi]*pnt[2];
      (*A)[e+2][gi] = -2.0*R[gi]*pnt[3];

      e += 3;

      // constraint a*a + b*b = r2*r2;

      L[e] = (R[ai]*R[ai] + R[bi]*R[bi]  - R[ri+1]*R[ri+1]*R[ri+1]*R[ri+1]);

      (*A)[e][ai]   = (-2.0*R[ai]);
      (*A)[e][bi]   = (-2.0*R[bi]);
      (*A)[e][ri+1] = (4.0*R[ri+1]*R[ri+1]*R[ri+1]);

      e++;

      // constraint V.V = 1;

      L[e] = 1.0e5*(1.0 - R[vi]*R[vi] - R[vi+1]*R[vi+1] - R[vi+2]*R[vi+2]);

      (*A)[e][vi]   = 1.0e5*2.0*R[vi];
      (*A)[e][vi+1] = 1.0e5*2.0*R[vi+1];
      (*A)[e][vi+2] = 1.0e5*2.0*R[vi+2];

      e++;


      // constraint (a*V + b*U).pnt = 0;

      L[e] = 1.0*( (R[ai]*R[vi]   + R[bi]*R[ui])*pnt[1] 
                     + (R[ai]*R[vi+1] + R[bi]*R[ui+1])*pnt[2]
                     + (R[ai]*R[vi+2] + R[bi]*R[ui+2])*pnt[3]);

      (*A)[e][vi] =  1.0*( -R[ai]*pnt[1]);
      (*A)[e][vi+1] = 1.0*(-R[ai]*pnt[2]);
      (*A)[e][vi+2] = 1.0*(-R[ai]*pnt[3]);

      (*A)[e][ui] =  1.0*( -R[bi]*pnt[1]);
      (*A)[e][ui+1] = 1.0*(-R[bi]*pnt[2]);
      (*A)[e][ui+2] = 1.0*(-R[bi]*pnt[3]);

      (*A)[e][ai] = 1.0*(-R[vi]*pnt[1] - R[vi+1]*pnt[2] - R[vi+2]*pnt[3]);

      (*A)[e][bi] = 1.0*(-R[ui]*pnt[1] - R[ui+1]*pnt[2] - R[ui+2]*pnt[3]);

      e++;

      // constraint V.U = 0;
 
       L[e] = 1.0e5*(R[vi]*R[ui] + R[vi+1]*R[ui+1] + R[vi+2]*R[ui+2]);

       (*A)[e][vi]   = 1.0e5*(-R[ui]);
       (*A)[e][vi+1] = 1.0e5*(-R[ui+1]);
       (*A)[e][vi+2] = 1.0e5*(-R[ui+2]);

       (*A)[e][ui]   = 1.0e5*(-R[vi]);
       (*A)[e][ui+1] = 1.0e5*(-R[vi+1]);
       (*A)[e][ui+2] = 1.0e5*(-R[vi+2]);

       e++;

      vi += 3;
      ai++;
      bi++;
      gi++;

    
    } // end loop each point

printf("vector R");
print_vector(R,1,nu);

printf("vector L");
print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

printf("vector D");
print_vector(D,1,nu);
//getchar();
     ci = 1;
     ui = ci + 3;
     ri = ui + 3;
     vi = ri + 2;
     ai = vi + 3*np;
     bi = ai + np;
     gi = bi + np;


     for(i=ci;i<ui;i++) R[i] += D[i];
     for(i=ui;i<ri;i++) R[i] += D[i];
     for(i=ri;i<vi;i++)
     {
       R[i] += D[i];
      // if(R[i] < 0.0)R[i] = -R[i];
     }
     for(i=vi;i<ai;i++) R[i] += D[i];
     for(i=ai;i<bi;i++) R[i] += D[i];
     for(i=bi;i<gi;i++) R[i] += D[i];
     for(i=gi;i<=nu;i++) 
     {
       R[i] += D[i];
       if(R[i] < 0.0 )R[i] = -R[i];
     }


     free_vector(D,1,nu);

  }// end loop for iteration

  free_matrix(A,1,ne,1,nu);
  free_vector(L,1,ne);

  PNTLST tors = matrix_shell(1,3,1,3);
  (*tors)[1] = clone_vector(R,1,3,1,3);
  (*tors)[2] = clone_vector(R,4,6,1,3);
  (*tors)[3] = clone_vector(R,7,9,1,3);

  (*tors)[3][2] = (*tors)[3][2]*(*tors)[3][2];
  (*tors)[3][1] = (*tors)[3][1]*(*tors)[3][1] + (*tors)[3][2];

  free_vector(R,1,nu);

  return(tors);
}





int make_ctorus_by_edge(int np, PNTLST pnts, PNTLST tms,
                           struct CirTorus &ct, struct _transform &trans )	    
{
   if(setjmp(env) != 0)
  {
   return(1);
  }

  PNTLST torus =  torus_by_edge( np, pnts, tms);


  ct.innerRadius = (*torus)[3][1];
  ct.outerRadius =(*torus)[3][2] ;
  ct.angle = 180.0;



  VECTOR zaxis = vector(1,3);
  zaxis[1] =  -(*torus)[2][1];
  zaxis[2] =  -(*torus)[2][2];
  zaxis[3] =  -(*torus)[2][3];
  
  VECTOR temp = vector(1,3);
  temp[1] =  zaxis[3];
  temp[2] =  -zaxis[1];
  temp[3] =  zaxis[2];

  VECTOR temp2 = cross_vector(temp,zaxis);
  VECTOR yaxis = unit_vector(3,temp2);
  VECTOR xaxis = cross_vector(yaxis,zaxis);

  free_vector(temp,1,3);
  free_vector(temp2,1,3);




  trans.pozition.pos_x = (*torus)[1][1];
  trans.pozition.pos_y = (*torus)[1][2];
  trans.pozition.pos_z = (*torus)[1][3];

  trans.rotation.r[0]= xaxis[1];
  trans.rotation.r[1]= xaxis[2];
  trans.rotation.r[2]= xaxis[3];

  trans.rotation.r[3]= 0.0;

  trans.rotation.r[4]= yaxis[1];
  trans.rotation.r[5]= yaxis[2];
  trans.rotation.r[6]= yaxis[3];

  trans.rotation.r[7]= 0.0;


  trans.rotation.r[8]= zaxis[1];
  trans.rotation.r[9]= zaxis[2];
  trans.rotation.r[10]= zaxis[3];

  trans.rotation.r[11]= 0.0;


  trans.rotation.r[12]= 0.0;
  trans.rotation.r[13]= 0.0;
  trans.rotation.r[14]= 0.0;

  trans.rotation.r[15]= 1.0;

  free_matrix(torus,1,3,1,3);
//  free_matrix(start,1,2,1,3);
//  free_matrix(end,1,2,1,3);
  free_vector(xaxis,1,3);
  free_vector(yaxis,1,3);
  free_vector(zaxis,1,3);



  return(0);
}



PNTLST cone_by_edge(int nl,int *npl, PNTLST *pnts,PNTLST tms) 
{


// CONE equations


  int i = 0;
  int ne = 0;
  int nu = 0;
 // int tnl = 0;
  int tnp = 0;
  int j = 0;
  int k = 0;
  int m = 0;  


// convert to real world system 
  k=-1;
  for(i=0;i<nl;i++) // for each line
  {
     printf("tm vector\n");
print_vector((*tms)[i],1,12);

    VECTOR inv = matinv((*tms)[i]); 
    copy_vector((*tms)[i],1,12,inv,1,12);
     
    for(m=0;m<npl[i];m++)
    {
      rotpnt(inv, (*(pnts[i]))[m] );
printf("converted point");
 print_vector( (*(pnts[i]))[m],1,3);
    }

    free_vector(inv,1,12);
 
  }




  //calculate initial values

 
  nu = 3;
  ne = 3;

  VECTOR L = vector(1,3);
  PNTLST A = matrix(1,3,1,3);
  PNTLST V = matrix(1,nl,1,3);

  for(i =1;i<=nl;i++)
  {
    VECTOR tmp = cross_vector( (*(pnts[i-1]))[0],(*(pnts[i-1]))[1]);
    (*V)[i] = unit_vector(3,tmp);
    free_vector(tmp,1,3);
  }   

  L[1] = (*V)[1][1]*( (*tms)[0][10] ) + (*V)[1][2]*( (*tms)[0][1l] ) + (*V)[1][3]*( (*tms)[0][12] );
  L[2] = (*V)[2][1]*( (*tms)[1][10] ) + (*V)[2][2]*( (*tms)[1][1l] ) + (*V)[2][3]*( (*tms)[1][12] );
  L[3] = (*V)[3][1]*( (*tms)[2][10] ) + (*V)[3][2]*( (*tms)[2][1l] ) + (*V)[3][3]*( (*tms)[2][12] );

  (*A)[1][1] = (*V)[1][1];
  (*A)[1][2] = (*V)[1][2]; 
  (*A)[1][3] = (*V)[1][3];

  (*A)[2][1] = (*V)[2][1];
  (*A)[2][2] = (*V)[2][2];
  (*A)[2][3] = (*V)[2][3];

  (*A)[3][1] = (*V)[3][1];
  (*A)[3][2] = (*V)[3][2];
  (*A)[3][3] = (*V)[3][3];


   VECTOR P = solve(A,L,nu); // cone apex

  free_vector(L,1,3);
  free_matrix(A,1,3,1,3);

 
  nu = 4;
  ne = 4;

  L = vector(1,4);
  A = matrix(1,4,1,4);

  L[1] = 0.0;
  L[2] = 0.0;
  L[3] = 0.0;

  L[4] = 1.0;

  (*A)[1][1] = (*V)[1][1];  
  (*A)[1][2] = (*V)[1][2];  
  (*A)[1][3] = (*V)[1][3]; 

  (*A)[1][4] = -1.0;  
 
  (*A)[2][1] = (*V)[2][1];  
  (*A)[2][2] = (*V)[2][2];  
  (*A)[2][3] = (*V)[2][3];  

  (*A)[2][4] = -1.0;  

  (*A)[3][1] = (*V)[3][1];  
  (*A)[3][2] = (*V)[3][2];  
  (*A)[3][3] = (*V)[3][3];  

  (*A)[3][4] = -1.0;  

 
  (*A)[4][1] = 1.0; 
  (*A)[4][2] = 1.0; 
  (*A)[4][3] = 1.0; 


   VECTOR tmp = solve(A,L,nu); // cone direction vector
   VECTOR U = unit_vector(3,tmp);

  free_vector(tmp,1,3);
  free_vector(L,1,4);
  free_matrix(A,1,4,1,4); 
   

 printf("vector P");
 print_vector(P,1,3);

 printf("vector U");
 print_vector(U,1,3);











// get total number 2d points
  for(i=0;i<nl;i++)
  {
   tnp += npl[i];
  } 
     
printf("total number of points = %d\n",tnp);
  nu = 7 + 3*nl + 2*tnp;
  ne = 1 + 2*nl + 4*tnp;

   L = vector(1,ne);
   A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);





  int gi= 0;
  for(int loop = 0; loop < 50; loop++)
  {
    int j = 0;
    int k = 1;
    
    int pi = 1;
    int ui = pi+3;
    int ki = ui+3;
    int vi = ki+1;
    int si = vi+3*nl;
    gi = si+ tnp;
    int e = 1; 

printf("enter LOOP\n");

    if(loop == 0)
    {
      for(i=pi; i < ui; i++) R[i] =P[i];
      for(i=ui; i < ki; i++)R[i] = U[i-ui+1];
      R[ki] = 0.5;
      for(i=vi; i < si; i += 3)for(j = 1; j<=3; j++)R[i+j-1] = (*V)[1+((i-vi)/3)][j];
      for(i=si; i < gi; i++) R[i] = 1.0;
      for(i=gi; i <= nu; i++) R[i] = 100.0;
print_vector(R,1,nu);

    }

    j = -1;


    // constraint u is unit vector;


     L[e] = 10000.0*(1.0 - R[ui]*R[ui] - R[ui+1]*R[ui+1] - R[ui+2]*R[ui+2]);

     (*A)[e][ui]   = 10000.0*2.0*R[ui];
     (*A)[e][ui+1] = 10000.0*2.0*R[ui+1];
     (*A)[e][ui+2] = 10000.0*2.0*R[ui+2];


     e++;


 
    for(int l = 0; l< nl; l++)  // for each edge line
    {
      
        j++;
      VECTOR tm = (*tms)[l];
      PNTLST lpnts =  pnts[l];
  

    

      // constraint v is unit vector

       L[e] = 10000.0*(1.0 - R[vi]*R[vi] - R[vi+1]*R[vi+1] - R[vi+2]*R[vi+2]);

       (*A)[e][vi]   = 10000.0*2.0*R[vi];
       (*A)[e][vi+1] = 10000.0*2.0*R[vi+1];
       (*A)[e][vi+2] = 10000.0*2.0*R[vi+2];
        
       e++;

      //constraint v is normal to line from camera origin to cone apex

       L[e] = 10000.0*( R[vi]*R[pi]     - R[vi]*tm[10] 
                      + R[vi+1]*R[pi+1] - R[vi+1]*tm[11]
                      + R[vi+2]*R[pi+2] - R[vi+2]*tm[12] );

       (*A)[e][vi]   = 10000.0*(-R[pi]   + tm[10]);
       (*A)[e][vi+1] = 10000.0*(-R[pi+1] + tm[11]);
       (*A)[e][vi+2] = 10000.0*(-R[pi+2] + tm[12]);
   
       (*A)[e][pi]   = 10000.0*(-R[vi]);
       (*A)[e][pi+1] = 10000.0*(-R[vi+1]);
       (*A)[e][pi+2] = 10000.0*(-R[vi+2]);

       e++;

      for(k = 1;k <= npl[l];k++) // for each pnt in edge line
      {
// printf("ui,pi,gi,si,e = %d,%d,%d,%d,%d\n",ui,pi,gi,si,e);
           VECTOR f = (*lpnts)[k-1];

           
           L[e]     = R[pi]   + R[si]*R[ui]   + R[ki]*R[si]*R[vi] 
                      - tm[10] - R[gi]*R[gi]*f[1];

           L[e+1]   = R[pi+1] + R[si]*R[ui+1] + R[ki]*R[si]*R[vi+1]
                      - tm[11] - R[gi]*R[gi]*f[2];

           L[e+2]   = R[pi+2] + R[si]*R[ui+2] + R[ki]*R[si]*R[vi+2]
                      - tm[12] - R[gi]*R[gi]*f[3];

           (*A)[e][pi] = -1;
           (*A)[e+1][pi+1] = -1;
           (*A)[e+2][pi+2] = -1;

           (*A)[e][si]   =  (-R[ui]   - R[ki]*R[vi]);
           (*A)[e+1][si] =  (-R[ui+1] - R[ki]*R[vi+1]);
           (*A)[e+2][si] =  (-R[ui+2] - R[ki]*R[vi+2]);

           (*A)[e][ui]     = -R[si];
           (*A)[e+1][ui+1] = -R[si];
           (*A)[e+2][ui+2] = -R[si];

           (*A)[e][ki]   = -R[si]*R[vi];
           (*A)[e+1][ki] = -R[si]*R[vi+1];
           (*A)[e+2][ki] = -R[si]*R[vi+2];

           (*A)[e][vi]     = -R[ki]*R[si];
           (*A)[e+1][vi+1] = -R[ki]*R[si];
           (*A)[e+2][vi+2] = -R[ki]*R[si];

           (*A)[e][gi]   = 2.0*R[gi]*f[1];
           (*A)[e+1][gi] = 2.0*R[gi]*f[2];
           (*A)[e+2][gi] = 2.0*R[gi]*f[3];

     
           e += 3;

      // constraint v is normal to f, ie v.f = 0
     
          L[e] = 10000.0*(-R[vi]*f[1] - R[vi+1]*f[2] - R[vi+2]*f[3]);

          (*A)[e][vi]   = 10000.0*f[1];
          (*A)[e][vi+1] = 10000.0*f[2];
          (*A)[e][vi+2] = 10000.0*f[3];

          e++;
 
          gi++;
          si++;

       }  // end loop for each point in edge line 


        vi +=3;
     
     }  // end loop for each edge line
  

printf("vector L");
print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

 
    pi = 1;
    ui = pi+3;
    ki = ui+3;
    vi = ki+1;
    si = vi+3*nl;
    gi = si+ tnp;

    
     for(i=pi;i< ui;i++) R[i] += D[i];
      
     for(i=ui;i<ki;i++)R[i] += D[i];
     R[ki] += D[ki];
     for(i=vi;i<si;i++)R[i] += D[i];
     for(i=si;i<gi; i++)R[i] += D[i];
      for(i=gi;i<= nu; i++)
     {
       R[i] += D[i];
       if(R[i] <0.0)R[i] = -R[i];
     }

printf("vector D");
print_vector(D,1,nu);
printf("vector R");
print_vector(R,1,nu);

     free_vector(D,1,nu);

  } // end itterattion loop

  for(i=gi;i<=gi+tnp-1;i++)R[i] = R[i]*R[i];

  PNTLST lines = matrix(1,3,1,3);
  k=1;
  for(i =1;i<=3;i++)
  {
    for(j=1;j<=3;j++)
    {
      (*lines)[i][j] = R[k++];
    }
  }

  free_vector(R,1,nu);
  free_vector(L,1,ne);
  free_matrix(A,1,ne,1,nu); 


  free_matrix(V,1,nl,1,3);
  free_vector(P,1,3);
  free_vector(U,1,3);

print_matrix(lines,1,3,1,3);
  return(lines);
}








int make_cone_by_lines(int nl,int *npl, PNTLST *pnts,PNTLST tms,
                        PNTLST sep, PNTLST setm , struct Cone &cone,
                        struct _transform &trans)

{
   if(setjmp(env) != 0)
  {
   return(1);
  }

  int i = 0;

  PNTLST cone1 = cone_by_edge( nl,npl,pnts,tms);

//  printf(" setm,s ");
//print_vector((*setm)[0],1,12);
//print_vector((*setm)[1],1,12);

  //convert start and end points to real world
  for(i=0;i< 2;i++)
  {
     VECTOR inv = matinv((*setm)[i]); 
     copy_vector((*setm)[i],1,12,inv,1,12);

      rotpnt(inv, (*sep)[i] );

     free_vector(inv,1,12);
  }


  PHOTO_FLOAT k = (*cone1)[3][1];

  k = k/sqrt(1-k*k);

//  printf(" setm,s ");
//printf_vector((*setm)[0],1,12);
//printf_vector((*setm)[1],1,12);

  PNTLST start = intersect_line_cone( k,(*cone1)[1],(*cone1)[2],&((*setm)[0][9]),(*sep)[0]);
  PNTLST end   = intersect_line_cone( k,(*cone1)[1],(*cone1)[2],&((*setm)[1][9]),(*sep)[1]);

  subtract_vector( (*start)[1],1,3,(*cone1)[1],1,3);
  subtract_vector( (*end)[1],1,3,(*cone1)[1],1,3);
 
 PHOTO_FLOAT sd = dot_vector( 3, (*start)[1], (*cone1)[2] );
 PHOTO_FLOAT ed = dot_vector( 3, (*end)[1], (*cone1)[2] );

 if(ed > sd )
 {
   PHOTO_FLOAT t = ed;
   ed =sd;
   sd = t;
 }

 cone.bottomRadius = k*sd;
 cone.topRadius = k*ed;
 cone.xOffset = 0.0;
 cone.yOffset = 0.0;
 cone.height = sd-ed;



  VECTOR zaxis = vector(1,3);
  zaxis[1] =  -(*cone1)[2][1];
  zaxis[2] =  -(*cone1)[2][2];
  zaxis[3] =  -(*cone1)[2][3];
  
  VECTOR temp = vector(1,3);
  temp[1] =  zaxis[3];
  temp[2] =  -zaxis[1];
  temp[3] =  zaxis[2];

  VECTOR temp2 = cross_vector(temp,zaxis);
  VECTOR yaxis = unit_vector(3,temp2);
  VECTOR xaxis = cross_vector(yaxis,zaxis);

  free_vector(temp,1,3);
  free_vector(temp2,1,3);




  trans.pozition.pos_x = (*cone1)[1][1] + 0.5*(sd+ed)*( (*cone1)[2][1]);
  trans.pozition.pos_y = (*cone1)[1][2] + 0.5*(sd+ed)*( (*cone1)[2][2]) ;
  trans.pozition.pos_z = (*cone1)[1][3] + 0.5*(sd+ed)*( (*cone1)[2][3]);

  trans.rotation.r[0]= xaxis[1];
  trans.rotation.r[1]= xaxis[2];
  trans.rotation.r[2]= xaxis[3];

  trans.rotation.r[3]= 0.0;

  trans.rotation.r[4]= yaxis[1];
  trans.rotation.r[5]= yaxis[2];
  trans.rotation.r[6]= yaxis[3];

  trans.rotation.r[7]= 0.0;


  trans.rotation.r[8]= zaxis[1];
  trans.rotation.r[9]= zaxis[2];
  trans.rotation.r[10]= zaxis[3];

  trans.rotation.r[11]= 0.0;


  trans.rotation.r[12]= 0.0;
  trans.rotation.r[13]= 0.0;
  trans.rotation.r[14]= 0.0;

  trans.rotation.r[15]= 1.0;

  free_matrix(cone1,1,3,1,3);
  free_matrix(start,1,2,1,3);
  free_matrix(end,1,2,1,3);
  free_vector(xaxis,1,3);
  free_vector(yaxis,1,3);
  free_vector(zaxis,1,3);

  return(0);


}  









PNTLST multipl( int nl, int *llary, int *npl, PNTLST *pnts, PNTLST tms) 
{
  int i = 0;
  int ne = 0;
  int nu = 0;
  int tnl = 0;
  int tnp = 0;
  int j = 0;
  int k = 0;
  int m = 0;  

  VECTOR p1 = NULL;
  VECTOR p2 = NULL;
  VECTOR p3 = NULL;
  VECTOR p4 = NULL;

// get total number of 2d lines and total number of 2d points
  for(i=0;i<nl;i++)
  {
    tnl += llary[i];
    for(j = 0; j < llary[i]; j++)
    {
      tnp += npl[k++];
    }
  } 
     

  nu = 3 + 3*nl + 2*tnp - nl;
  ne = 3*tnp + 1;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);

  PNTLST nv = matrix(1,tnl,1,3);
  PNTLST nvo = matrix(1,tnl,1,3);
  

  PHOTO_FLOAT l1 = 0.0;
  PHOTO_FLOAT l2 = 0.0;
  PHOTO_FLOAT l = 0.0;
// convert to real world system 

  k=-1;
  for(i=0;i<nl;i++) // for each 3d line
  {
    for(j=0;j< llary[i]; j++) // for each 2d line defining 3d line
    {
      k++;
      printf("tm vector\n");
print_vector((*tms)[k],1,12);

      VECTOR inv = matinv((*tms)[k]);
      VECTOR temp = vector(1,3);
      
      (*nvo)[k+1][1] = inv[10]; // save camera origin as plane vector origin
      (*nvo)[k+1][2] = inv[11]; 
      (*nvo)[k+1][3] = inv[12]; 

      copy_vector((*tms)[k],1,12,inv,1,12);

      l1 =0.0;     
      for(m=0;m<npl[k];m++)  // for each point defining this 2d line
      {
        rotpnt(inv, (*(pnts[k]))[m] );
printf("converted point");
 print_vector( (*(pnts[k]))[m],1,3);

        if( (m == 0) )  // first point on 2d line 
        {
          p1 = (*(pnts[k]))[m];
        }
        else
        {
          temp[1] = (*(pnts[k]))[m][1];
          temp[2] = (*(pnts[k]))[m][2];
          temp[3] = (*(pnts[k]))[m][3];
          if(l1 < (l = vector_length(3,temp)))
          {
            p2 = (*(pnts[k]))[m]; // point on first 2d line of first 3d line max distance from p1
            l1 = l;
          }
        }

      }

      free_vector(inv,1,12);
      free_vector(temp,1,3);

      VECTOR tmp = cross_vector(p1,p2);
      VECTOR cp1 = unit_vector(3,tmp); // photo 1 plane (origin,p1,p2) containing line
      (*nv)[k+1][1] = cp1[1];
      (*nv)[k+1][2] = cp1[2];
      (*nv)[k+1][3] = cp1[3];

      free_vector(tmp,1,3);
      free_vector(cp1,1,3);
   
    }

  }

  int breakflag = 0;
  PHOTO_FLOAT oldavgl = 1.0e15;

  int gi= 0;
  for(int loop = 0; loop < 50; loop++)
  {
    int j = 0;
    int k = 1;
    int ui = 1;     // first variables in R array are vector components of line direction vector

    int pi = 4;     // next variables in R array are starting points for each 3d line
 
    gi = pi+ 3*nl;  // next variables are scale factors of 2d points to intersection of 3d line

    int si = gi + tnp; // next variables are scale factors of 3d vectors to intersections 0f 2d vectors
    int e = 1; 

    if(loop == 0) // calculate initial direction vector of parallel lines
    {

      VECTOR cp1 = cross_vector((*nv)[1],(*nv)[2]);
      VECTOR uv = unit_vector(3,cp1); // photo 1 plane (origin,p1,p2) containing line


      R[ui]   = uv[1];
      R[ui+1] = uv[2];
      R[ui+2] = uv[3];

      free_vector(cp1,1,3);
      free_vector(uv,1,3);

      //for(i=ui;i<= ui+2;i++)R[i] = 1.0;
      for(i=pi;i<= pi + 3*nl -1;i++) R[i] =1.0;
      for(i=gi;i<= gi + tnp -1; i++) R[i] = 100.0;
      for(i=si;i<= si + tnp -nl-1; i++) R[i] = 1.0;
    }

    j = -1;
    for(int l = 0; l< nl; l++)  // for each parallel line
    {
      
      for(int ll = 0; ll < llary[l]; ll++) // for each 2d line for this line
      {
        j++;
        int pl1ndx;
         
        if(ll == 0)
        {
          pl1ndx = j+1;
        }
     
        VECTOR tm = (*tms)[j];
        PNTLST lpnts =  pnts[j];

        PHOTO_FLOAT med = 1.0;

        for(k = 1;k <= npl[j];k++) // for each pnt in 2d line
        {
// printf("ui,pi,gi,si,e = %d,%d,%d,%d,%d\n",ui,pi,gi,si,e);
           VECTOR f = (*lpnts)[k-1];

           if(loop==0) // if first pass initialize 
           {

             VECTOR pl = (*nv)[pl1ndx];
             VECTOR po = (*nvo)[pl1ndx];
             if(ll == 0)
             {
               pl = (*nv)[pl1ndx+1];
               po = (*nvo)[pl1ndx+1];
             }

             PHOTO_FLOAT t = ( (po[1]-tm[10])*pl[1]
                       + (po[2]-tm[11])*pl[2]
                       + (po[3]-tm[12])*pl[3])/(f[1]*pl[1] + f[2]*pl[2] + f[3]*pl[3]);

             if((ll == 0) && (k == 1)) // if first point of first 2d line  
             {
               R[gi] = sqrt(t);
               R[pi]   = tm[10] + R[gi]*R[gi]*f[1];
               R[pi+1] = tm[11] + R[gi]*R[gi]*f[2];
               R[pi+2] = tm[12] + R[gi]*R[gi]*f[3];
             }
             else
             {
               R[gi] = sqrt(t);
               PHOTO_FLOAT t1 = tm[10] + R[gi]*R[gi]*f[1];
               PHOTO_FLOAT t2 = tm[11] + R[gi]*R[gi]*f[2];
               PHOTO_FLOAT t3 = tm[12] + R[gi]*R[gi]*f[3];

               t1 = (t1-R[pi  ])/R[ui  ];
               t2 = (t2-R[pi+1])/R[ui+1];
               t3 = (t3-R[pi+2])/R[ui+2];

               R[si] = (t1+t2+t3)/3.0;
             }
           }
                
           med = 1.0/(R[gi]*R[gi]);
           if((ll == 0) && (k == 1)) // if first point of first 2d line  
           {
             L[e]   = med*(tm[10] +R[gi]*R[gi]*f[1] - R[pi]);
             L[e+1] = med*(tm[11] +R[gi]*R[gi]*f[2] - R[pi+1]);
             L[e+2] = med*(tm[12] +R[gi]*R[gi]*f[3] - R[pi+2]);
           }
           else
           {
             L[e]   = med*(tm[10] - R[si]*R[ui]   - R[pi]   +R[gi]*R[gi]*f[1]);
             L[e+1] = med*(tm[11] - R[si]*R[ui+1] - R[pi+1] +R[gi]*R[gi]*f[2]);
             L[e+2] = med*(tm[12] - R[si]*R[ui+2] - R[pi+2] +R[gi]*R[gi]*f[3]);
           }

           (*A)[e][pi] = med*1.0;
           (*A)[e+1][pi+1] =  med*1.0;
           (*A)[e+2][pi+2] =  med*1.0;

           (*A)[e][gi] = - med*2.0*R[gi]*f[1];
           (*A)[e+1][gi] = - med*2.0*R[gi]*f[2];
           (*A)[e+2][gi] = - med*2.0*R[gi]*f[3];

           if( (ll==0) && (k==1) ) 
           {
           }
           else  
           {
             (*A)[e][ui] =  med*R[si];
             (*A)[e+1][ui+1] =  med*R[si];
             (*A)[e+2][ui+2] =  med*R[si];
         
             (*A)[e][si] =  med*R[ui];
             (*A)[e+1][si] =  med*R[ui+1];
             (*A)[e+2][si] =  med*R[ui+2];
             si++; 
           }
           e += 3; 

/**
           med = 1.0;

           if((ll == 0) && (k == 1))
           {
             L[e] =med*( (tm[10] + R[gi]*R[gi]*f[1] - R[pi]   )*R[ui]
                       + (tm[11] + R[gi]*R[gi]*f[2] - R[pi+1] )*R[ui+1]
                       + (tm[12] + R[gi]*R[gi]*f[3] - R[pi+2] )*R[ui+2]);

           }
           else
           {
             L[e] = med*(( tm[10] + R[gi]*R[gi]*f[1] - R[pi]   - R[si]*R[ui])  *R[ui]
                        + (tm[11] + R[gi]*R[gi]*f[2] - R[pi+1] - R[si]*R[ui+1])*R[ui+1]
                        + (tm[12] + R[gi]*R[gi]*f[3] - R[pi+2] - R[si]*R[ui+2])*R[ui+2]);
           }

           (*A)[e][gi] = -med*2.0*R[gi]*( f[1]*R[ui] + f[2]*R[ui+1] + f[3]*R[ui+2] );

           (*A)[e][pi]   = med*R[ui]; 
           (*A)[e][pi+1] = med*R[ui+1]; 
           (*A)[e][pi+2] = med*R[ui+2];

           if((ll == 0) && (k == 1))
           {
             (*A)[e][ui]   = -med*(tm[10] + R[gi]*R[gi]*f[1] - R[pi]   );
             (*A)[e][ui+1] = -med*(tm[11] + R[gi]*R[gi]*f[2] - R[pi+1] );
             (*A)[e][ui+2] = -med*(tm[12] + R[gi]*R[gi]*f[3] - R[pi+2] );
           }
           else
           {

              (*A)[e][si] = med*(R[ui]*R[ui] + R[ui+1]*R[ui+1] + R[ui+2]*R[ui+1]);

             (*A)[e][ui]   = -med*(tm[10] + R[gi]*R[gi]*f[1] - R[pi]   ) + med*2.0*R[ui]*R[si];
             (*A)[e][ui+1] = -med*(tm[11] + R[gi]*R[gi]*f[2] - R[pi+1] ) + med*2.0*R[ui+1]*R[si];
             (*A)[e][ui+2] = -med*(tm[12] + R[gi]*R[gi]*f[3] - R[pi+2] ) + med*2.0*R[ui+2]*R[si];
           }
      
           e++;   


           if((ll == 0) && (k == 1))
           {
             L[e] = med*(tm[10] + R[gi]*R[gi]*f[1] - R[pi]   )*f[1]
                  + (tm[11] + R[gi]*R[gi]*f[2] - R[pi+1] )*f[2]
                  + (tm[12] + R[gi]*R[gi]*f[3] - R[pi+2] )*f[3];

           }
           else
           {
             L[e] = med*(tm[10] + R[gi]*R[gi]*f[1] - R[pi]   - R[si]*R[ui])  *f[1]
                  + (tm[11] + R[gi]*R[gi]*f[2] - R[pi+1] - R[si]*R[ui+1])*f[2]
                  + (tm[12] + R[gi]*R[gi]*f[3] - R[pi+2] - R[si]*R[ui+2])*f[3];
           }

           (*A)[e][gi] = -med*2.0*R[gi]*( f[1]*f[1] + f[2]*f[2] + f[3]*f[3] );

           (*A)[e][pi]   = med*f[1]; 
           (*A)[e][pi+1] = med*f[2]; 
           (*A)[e][pi+2] = med*f[3];

           if((ll == 0) && (k == 1))
           {

           }
           else
           {

              (*A)[e][si] = med*(R[ui]*f[1] + R[ui+1]*f[2] + R[ui+2]*f[3]);

             (*A)[e][ui]   = med*f[1]*R[si];
             (*A)[e][ui+1] = med*f[2]*R[si];
             (*A)[e][ui+2] = med*f[3]*R[si];
             si++;
           }
      
           e++;   
**/





           gi++;
      
         } // end loop for each pnt in 2d line

       }  // end loop for each 2d line defining 3d line 

       pi += 3;








     }  // end loop for each 3d line
  
     L[e] = 10000*(1-R[1]*R[1] - R[2]*R[2] - R[3]*R[3]);
     (*A)[e][1] = 2.0*10000.0*R[1];
     (*A)[e][2] = 2.0*10000.0*R[2];
     (*A)[e][3] = 2.0*10000.0*R[3];


     PHOTO_FLOAT avgl = 0.0;
     for(i=1;i<=ne;i++)avgl += L[i];


//printf("vector L");
//print_vector(L,1,ne);
//print_matrix(A,1,ne,1,nu);

     VECTOR D = lsq(A,L,nu,ne);

     ui = 1;
     pi = 4;
     gi = pi+ 3*nl;
     si = gi + tnp;
     
      
     for(i=ui;i<= ui+2;i++)R[i] += 1.0*D[i];
     for(i=pi;i<= pi + 3*nl -1;i++) R[i] += 1.0*D[i];
     for(i=gi;i<= gi + tnp -1; i++)
     {
       R[i] += 1.0*D[i];
       if(R[i] <0.0)R[i] = -R[i];
     }
     for(i=si;i<= si + tnp -nl-1; i++) R[i] += 1.0*D[i];


     free_vector(D,1,nu);
//printf("avgl = %e\n",avgl);
     avgl = fabs(avgl/(PHOTO_FLOAT)ne);
     if( fabs(avgl-oldavgl) < avgl*1.0e-5)
     {
        breakflag += 1;
     }
     else
     {
       breakflag = 0;
     }
     if(breakflag > 2)break;
     oldavgl = avgl;

Output(2," itteration - %d,  avgl = %e\n",loop, oldavgl);



  } // end itterattion loop


//printf("vector D");
//print_vector(D,1,nu);
//printf("vector R");
//print_vector(R,1,nu);


  for(i=gi;i<=gi+tnp-1;i++)R[i] = R[i]*R[i];

  PNTLST lines = matrix(1,nl+1,1,3);
  k=1;
  for(i =1;i<=nl+1;i++)
  {
    for(j=1;j<=3;j++)
    {
      (*lines)[i][j] = R[k++];
    }
  }

  free_vector(R,1,nu);
  free_vector(L,1,ne);
  free_matrix(A,1,ne,1,nu); 

//print_matrix(lines,1,nl+1,1,3);
  return(lines);
}

/*FF*************************************************************************
*									    *
*   NAME								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   INPUTS								    *
*									    *
*   OUTPUTS								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	00/00/93 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "MakeLineBy2DLines"
int MakeLineBy2DLines( int *np2DLinesPer3DLine, 
	int *npPntsPer2DLine, PNTLST *plp2DLinePnts, 
	PNTLST plp2DLineTMatrix, PNTLST plEndPnts,
	PNTLST plEndPntsTMatrix, struct Line2p & line2p )
{
    PNTLST pl3DLine;
    VECTOR vInverse;
    double dfDelta;
    double dfScale1, dfScale2;
    
   if(setjmp(env) != 0)
   {
    return(1);
   }

    /* find point on line and unit vector along line */
    pl3DLine = multipl( 1, np2DLinesPer3DLine, npPntsPer2DLine,
	    plp2DLinePnts, plp2DLineTMatrix );

    printf( "Here's the 3D line\n" );		
    print_matrix( pl3DLine, 1, 2, 1, 3 );

    /* convert end points to real world system */
    for( int i = 0; i < 2; i++ )
    {
	vInverse = matinv( (*plEndPntsTMatrix)[i] );
	copy_vector( (*plEndPntsTMatrix)[i], 1, 12, vInverse, 1, 12 );
	rotpnt( vInverse, (*plEndPnts)[i] );
	free_vector( vInverse, 1, 12 );
    }
    
    dfDelta = intersect_lines_dir( &((*plEndPntsTMatrix)[0][9]), 
	    (*plEndPnts)[0], (*pl3DLine)[2], (*pl3DLine)[1], 
	    &dfScale1, &dfScale2 );

    Output( 4, "First end point\n" );	    
    Output( 4, "  dfDelta = %G\n", dfDelta );
    Output( 4, "  dfScale1 = %G\n", dfScale1 );
    Output( 4, "  dfScale2 = %G\n", dfScale2 );

    line2p.pt1.x = (*pl3DLine)[2][1] + dfScale2 * (*pl3DLine)[1][1];
    line2p.pt1.y = (*pl3DLine)[2][2] + dfScale2 * (*pl3DLine)[1][2];
    line2p.pt1.z = (*pl3DLine)[2][3] + dfScale2 * (*pl3DLine)[1][3];

    dfDelta = intersect_lines_dir( &((*plEndPntsTMatrix)[1][9]),
	    (*plEndPnts)[1], (*pl3DLine)[2], (*pl3DLine)[1], 
	    &dfScale1, &dfScale2 );

    Output( 4, "Second end point\n" );	    
    Output( 4, "  dfDelta = %G\n", dfDelta );
    Output( 4, "  dfScale1 = %G\n", dfScale1 );
    Output( 4, "  dfScale2 = %G\n", dfScale2 );
	    
    line2p.pt2.x = (*pl3DLine)[2][1] + dfScale2 * (*pl3DLine)[1][1];
    line2p.pt2.y = (*pl3DLine)[2][2] + dfScale2 * (*pl3DLine)[1][2];
    line2p.pt2.z = (*pl3DLine)[2][3] + dfScale2 * (*pl3DLine)[1][3];

    free_matrix( pl3DLine, 1, 2, 1, 3 );
	return 0;
}
#undef FUNC


/*FF*************************************************************************
*									    *
*   NAME								    *
*									    *
*   DESCRIPTION								    *
*									    *
*   INPUTS								    *
*									    *
*   OUTPUTS								    *
*									    *
*   NOTES								    *
*									    *
*   BUGS								    *
*									    *
*   REVISION HISTORY							    *
*	00/00/93 - jwb - Version 1.0.0					    *
*									    *
****************************************************************************/
#define FUNC "make_box_by_perp_lines"
int make_box_by_perp_lines(int *nll,int *npl, PNTLST *pnts,PNTLST tms,
                        PNTLST sep, PNTLST setm , struct Box &box,
                        struct _transform &trans)
{
    int e, i, j, k, m;
    int tnl = 0;		        /* total number of 2D lines */
    int tnp = 0;		        /* total number of 2D points */
    int nu = 0;			        /* total number of unknowns */
    int ne = 0;			        /* total number of equations */
    VECTOR vInverse;
    VECTOR L, R, D;
    PNTLST A;
    VECTOR tm;
    int ci, vi, di, gi;
    double s[3];
    #define BIGNUM (1.0E5)

   if(setjmp(env) != 0)
   {
    return(1);
   }
    
/* solve for 3 perpendicular lines */
    PNTLST  lines[3];

    /* find out total number of 2D lines and 2D points */
    k = 0;
    int pindx = 0;
    for( i = 0; i < 3; i++ )
    {
	tnl += nll[i];
//PNTLST multipl( int nl, int *llary, int *npl, PNTLST *pnts, PNTLST tms)
         PNTLST tmx = submatrix(tms,k,k+nll[i],1,3,0,1); 
         lines[i] = multipl(1,&(nll[i]),&(npl[k]),&(pnts[k]),tmx);
         free_submatrix(tmx,0,nll[i],1,3);

	for( j = 0; j < nll[i]; j++ )
        {
            
	    tnp += npl[k++];
        }
    }





    DebugOutput( 2, "total number of 2D lines = %d\n", tnl );
    DebugOutput( 2, "total number of 2D points = %d\n", tnp );

    /* find out how many equations and unknowns */
    nu = 3*3 + 3 + 2 * tnp;	        /* 3 unknowns per perpendicular 3D line,
					 * 3 unknowns for vertex of 3 lines, 
					 * 2 additional unknowns per 2D point
					 */
    ne = 3 + 3 + 3*tnp;		        /* 3 equations from unit vector definitions,
					 * 3 equations from perpendicular vectors, 
					 * 3 equations per 2D point
					 */

    DebugOutput( 2, "number of unknowns = %d\n", nu );
    DebugOutput( 2, "number of equations = %d\n", ne );

    /* Allocate matrices for solution */
    L = vector( 1, ne );
    A = matrix( 1, ne, 1, nu );
    R = vector( 1, nu );

    /* convert all 2D points along lines to real world system */
/**
    k = 0;
    for( i = 0; i < 3; i++ )
    {
	for( j = 0; j < nll[i]; j++ )
	{
	    vInverse = matinv( (*tms)[k] );
	    copy_vector( (*tms)[k], 1, 12, vInverse, 1, 12 );
	    
	    for( m = 0; m < npl[k]; m++ )
	    {
		rotpnt( vInverse, (*(pnts[k]))[m] );
	    }
	    free_vector( vInverse, 1, 12 );
	    
	    k++;
	}
    }
**/
    
    /* do iterative least square solution */

    PHOTO_FLOAT oldavgd = 0.0;
    int bcount = 0;
    for( int loop = 0; loop < 50; loop++ )
    {
	ci = 1;		        /* index for center point */
	vi = ci + 3;	        /* index for 3 vectors */
	di = vi + 3*3;	        /* index for distance along v vectors */
	gi = di + tnp;	        /* index for distance along camera ray vector */

	/* initialize R first time through */
	if( loop == 0 )
	{
           PHOTO_FLOAT s1,s2;

//PHOTO_FLOAT intersect_lines_dir( VECTOR o1, VECTOR d1, VECTOR o2,
//                         VECTOR d2, PHOTO_FLOAT *s1, PHOTO_FLOAT *s2 )

            intersect_lines_dir( (*(lines[0]))[2],(*(lines[0]))[1],
                                 (*(lines[1]))[2],(*(lines[1]))[1],&s1,&s2);
	    DebugOutput( 2, "ci = %d\n", ci );
	    DebugOutput( 2, "vi = %d\n", vi );
	    DebugOutput( 2, "di = %d\n", di );
	    DebugOutput( 2, "gi = %d\n", gi );

	    for( i = ci; i < vi; i++ )
                R[i] = (*(lines[0]))[2][i-ci+1] + (*(lines[0]))[1][i-ci+1]*s1;
           
            k=0;
	    for( i = 0; i < 3; i++ )
            {
	      for( j = 0; j < 3; j++ )
   
		R[vi + k++] = (*(lines[i]))[1][j+1];
            }


		
	}
	
	e = 1;
	
	for( i = 0; i < 3; i++ )
	{
	    L[e] = BIGNUM * (R[vi]*R[vi] + R[vi+1]*R[vi+1] + R[vi+2]*R[vi+2]-1.0);
	     
	    (*A)[e][vi]   = BIGNUM*(-2.0*R[vi]); 
	    (*A)[e][vi+1] = BIGNUM*(-2.0*R[vi+1]);
	    (*A)[e][vi+2] = BIGNUM*(-2.0*R[vi+2]);

	    e++;
	    vi+=3;
	}
	
	vi = ci + 3;

	L[e] = BIGNUM*(R[vi]*R[vi+3] + R[vi+1]*R[vi+4] + R[vi+2]*R[vi+5]);
	(*A)[e][vi]   = BIGNUM*(-R[vi+3]);
	(*A)[e][vi+1] = BIGNUM*(-R[vi+4]);
	(*A)[e][vi+2] = BIGNUM*(-R[vi+5]);
	(*A)[e][vi+3] = BIGNUM*(-R[vi]);
	(*A)[e][vi+4] = BIGNUM*(-R[vi+1]);
	(*A)[e][vi+5] = BIGNUM*(-R[vi+2]);
	e++;

	L[e] = BIGNUM*(R[vi]*R[vi+6] + R[vi+1]*R[vi+7] + R[vi+2]*R[vi+8]);
	(*A)[e][vi]   = BIGNUM*(-R[vi+6]);
	(*A)[e][vi+1] = BIGNUM*(-R[vi+7]);
	(*A)[e][vi+2] = BIGNUM*(-R[vi+8]);
	(*A)[e][vi+6] = BIGNUM*(-R[vi]);
	(*A)[e][vi+7] = BIGNUM*(-R[vi+1]);
	(*A)[e][vi+8] = BIGNUM*(-R[vi+2]);
	e++;
	
	L[e] = BIGNUM*(R[vi+3]*R[vi+6] + R[vi+4]*R[vi+7] + R[vi+5]*R[vi+8]);
	(*A)[e][vi+3] = BIGNUM*(-R[vi+6]);
	(*A)[e][vi+4] = BIGNUM*(-R[vi+7]);
	(*A)[e][vi+5] = BIGNUM*(-R[vi+8]);
	(*A)[e][vi+6] = BIGNUM*(-R[vi+3]);
	(*A)[e][vi+7] = BIGNUM*(-R[vi+4]);
	(*A)[e][vi+8] = BIGNUM*(-R[vi+5]);
	e++;
	
	k = 0;
	for( int l = 0; l < 3; l++ )
	{
	    for( int ll = 0; ll < nll[l]; ll++ )
	    {
		tm = (*tms)[k];
		PNTLST lpnts = pnts[k];
		
		for( int llp = 1; llp <= npl[k]; llp++ )
		{


		    VECTOR f = (*lpnts)[llp-1];

                    if(loop == 0)
                    {
                      intersect_lines_dir( &R[ci-1],&R[vi-1],
                                           &tm[10-1],&f[0],&R[di],&R[gi]);
                      R[gi] = sqrt(R[gi]);
                    }
                 
                    PHOTO_FLOAT sf = 100.0/(R[gi]*R[gi]);
		    L[e]   = sf*(R[ci]   + R[di]*R[vi]   - tm[10] - R[gi]*R[gi]*f[1]);
		    L[e+1] = sf*(R[ci+1] + R[di]*R[vi+1] - tm[11] - R[gi]*R[gi]*f[2]);
		    L[e+2] = sf*(R[ci+2] + R[di]*R[vi+2] - tm[12] - R[gi]*R[gi]*f[3]);
		    
		    (*A)[e][ci] = -sf;
		    (*A)[e+1][ci+1] = -sf;
		    (*A)[e+2][ci+2] = -sf;
		    
		    (*A)[e][vi] = -sf*R[di];
		    (*A)[e+1][vi+1] = -sf*R[di];
		    (*A)[e+2][vi+2] = -sf*R[di];
		    
		    (*A)[e][di]   = -sf*R[vi];
		    (*A)[e+1][di] = -sf*R[vi+1];
		    (*A)[e+2][di] = -sf*R[vi+2];
		    
		    (*A)[e][gi]   = sf*(2.0 * R[gi]*f[1]);
		    (*A)[e+1][gi] = sf*(2.0 * R[gi]*f[2]);
		    (*A)[e+2][gi] = sf*(2.0 * R[gi]*f[3]);
		    
		    di++;
		    gi++;
		    e+=3;
		}
	    
		k++;
	    }
	    vi+=3;
	}

	DebugOutput( 2, "Vector L:\n" );
	print_vector( L, 1, ne );
	DebugOutput( 2, "Vector R:\n" );
	print_vector( R, 1, nu );

	Output( 1, "solving iteration #%d\n", loop );

	D = lsq( A, L, nu, ne );
  
        PHOTO_FLOAT avgd = 0.0;
        PHOTO_FLOAT avgr = 0.0;

	for( i = ci; i <= nu; i++ )
        {
	    R[i] += D[i]; 
            avgd += fabs(D[i]);
            avgr += fabs(R[i]);
        }



        avgd = avgd/nu;
  
        if( fabs(oldavgd - avgd) < fabs(avgr*1.0e-8))
        {
          bcount += 1;
        }
        else
        {
          bcount = 0;
        }
  

	DebugOutput( 2, "Vector D:\n" );
	print_vector( D, 1, nu );

        if(bcount >=3)break;
        oldavgd = avgd;
    }

    DebugOutput( 2, "*****Final results*****\n" );

    DebugOutput( 2, "Vector L:\n" );
    print_vector( L, 1, ne );
    DebugOutput( 2, "Vector R:\n" );
    print_vector( R, 1, nu );
    DebugOutput( 2, "Vector D:\n" );
    print_vector( D, 1, nu );

/* set lengths of each line */

    ci = 1;
    vi = ci + 3;
    di = vi + 3*3;
    gi = di + tnp;

    /* convert 2D end points to real system */ 
    for( i = 0; i < 3; i++ )
    {
	vInverse = matinv( (*setm)[i] );
	copy_vector( (*setm)[i], 1, 12, vInverse, 1, 12 );
	
	rotpnt( vInverse, (*sep)[i] );
	
	free_vector( vInverse, 1, 12 );
    }
    
    double dfScale1, dfScale2, dfDelta;
    
    for( i = 0; i < 3; i++ )
    {
	dfDelta = intersect_lines_dir( &((*setm)[i][9]), (*sep)[i],
		R, &(R[vi + i*3 - 1]), &dfScale1, &s[i] );
    }
    
    VECTOR cp;
    
    cp = cross_vector( &(R[vi-1]), &(R[vi+2]) );
    
    if( dot_vector( 3, cp, &(R[vi+5]) ) < 0 )
    {
	R[vi+6] *= -1.0;
	R[vi+7] *= -1.0;
	R[vi+8] *= -1.0;
	s[2] *= -1.0;
	
	DebugOutput( 2, "********** We reversed the Z vector **********\n" );
    }
    
    free_vector( cp, 1, 3 );
    
    
/* convert data to structures */
    memset( &box, 0, sizeof(struct Box) );
    memset( &trans, 0, sizeof(struct _transform) );

    box.xlen = fabs(s[0]);
    box.zlen = fabs(s[1]);
    box.ylen = fabs(s[2]);
    
    trans.pozition.pos_x = R[1] + (s[0]*R[vi] + s[1]*R[vi+3] + s[2]*R[vi+6])/2.0;
    trans.pozition.pos_y = R[2] + (s[0]*R[vi+1] + s[1]*R[vi+4] + s[2]*R[vi+7])/2.0;
    trans.pozition.pos_z = R[3] + (s[0]*R[vi+2] + s[1]*R[vi+5] + s[2]*R[vi+8])/2.0;

    trans.rotation.r[0] = R[vi];
    trans.rotation.r[1] = R[vi+1];
    trans.rotation.r[2] = R[vi+2];
    trans.rotation.r[3] = 0.0;
    
    trans.rotation.r[4] = R[vi+3];
    trans.rotation.r[5] = R[vi+4];
    trans.rotation.r[6] = R[vi+5];
    trans.rotation.r[7] = 0.0;
    
    trans.rotation.r[8] = R[vi+6];
    trans.rotation.r[9] = R[vi+7];
    trans.rotation.r[10] = R[vi+8];
    trans.rotation.r[11] = 0.0;
    
    trans.rotation.r[12] = 0.0;
    trans.rotation.r[13] = 0.0;
    trans.rotation.r[14] = 0.0;
    trans.rotation.r[15] = 1.0;
    
    trans.scale.sca_x = 1.0;
    trans.scale.sca_y = 1.0;
    trans.scale.sca_z = 1.0;
    
    trans.orientation.ori_x = 0.0;
    trans.orientation.ori_y = 0.0;
    trans.orientation.ori_z = 1.0;
    trans.orientation.ori_beta = 0.0;
    
    trans.center.cen_x = 0.0;
    trans.center.cen_y = 0.0;
    trans.center.cen_z = 0.0;

    free_vector( L, 1, ne );
    free_vector( R, 1, nu );
    free_vector( D, 1, nu );
    free_matrix( A, 1, ne, 1, nu );

    return(0);
}






int make_box_by_lines(int *nll,int *npl, PNTLST *pnts,PNTLST tms,
                        PNTLST sep, PNTLST setm , struct Box &box,
                        struct _transform &trans)

{
   if(setjmp(env) != 0)
  {
   return(1);
  }

  PHOTO_FLOAT width = 0.0;
  PHOTO_FLOAT depth = 0.0;
  int i = 0;

 VECTOR d = vector(1,3); 
  
 PNTLST lines = multipl( 3,nll,npl,pnts,tms);

// convert start end pnts to real world system 
 
  for(i=0;i<2;i++)
  {

      VECTOR inv = matinv((*setm)[i]); 
      copy_vector((*setm)[i],1,12,inv,1,12);
     
      rotpnt(inv, (*sep)[i] );

      free_vector(inv,1,12);
  

  }








  for(i=3;i<=4;i++)
  {
    
    d[i-2] = dist_pnt_to_line( (*lines)[i], lines);
  }

  PNTLST temp = matrix_shell(1,2,1,3);

  (*temp)[1] = (*lines)[1];
  (*temp)[2] = (*lines)[4];

  d[3] = dist_pnt_to_line((*lines)[3],temp);
  

  free_matrix_shell(temp);

  
  PHOTO_FLOAT max = d[1];
  int ck = 1;

  if(d[2] >max)
  {
    max = d[2];
    ck = 2;
  }

  if(d[3] > max)ck =3;

print_vector(d,1,3);
  if(ck == 1)
  {
    width = d[2]/2.0;
    depth = d[3]/2.0;

printf("width,depth = %e,%e\n",width,depth);
    VECTOR tp = clone_vector((*lines)[4],1,3,1,3);
    copy_vector( (*lines)[4],1,3,(*lines)[3],1,3);
    copy_vector( (*lines)[3],1,3,tp,1,3);
    free_vector(tp,1,3);
  }

  if(ck == 3)
  {
    width = d[1]/2.0;
    depth = d[2]/2.0;
printf("width,depth = %e,%e\n",width,depth);

    VECTOR tp = clone_vector((*lines)[2],1,3,1,3);
    copy_vector( (*lines)[2],1,3,(*lines)[3],1,3);
    copy_vector( (*lines)[3],1,3,tp,1,3);
    free_vector(tp,1,3);
  }


   VECTOR sp = vector(1,3);
// get start point  
   int ln;
   PHOTO_FLOAT ss= 0;
   PHOTO_FLOAT maxd = 0;

  for(i=1;i<=3;i++)
  {
    PHOTO_FLOAT d = 0;
    PHOTO_FLOAT s1 = 0;
    PHOTO_FLOAT s2 = 0;

   // intersect_lines_dir( VECTOR o1, VECTOR d1, VECTOR o2,
   //                      VECTOR d2, PHOTO_FLOAT *s1, PHOTO_FLOAT *s2 )
    
   d = intersect_lines_dir( &((*setm)[0][9]), (*sep)[0],
                            (*lines)[i+1],(*lines)[1],&s1,&s2);
   if(i ==1)
   {
     ln = 1;
     maxd = d;
     ss = s2;
   }
   else
   {
     if(d < maxd)
     {
       maxd = d;
       ln = i;
       ss = s2;
     }
   }


  }

  sp[1] = (*lines)[ln+1][1] + ss*((*lines)[1][1]);
  sp[2] = (*lines)[ln+1][2] + ss*((*lines)[1][2]);
  sp[3] = (*lines)[ln+1][3] + ss*((*lines)[1][3]);
printf(" start points- ss = %e ",ss);
print_vector(sp,1,3);

//VECTOR prj_pnt_onto_line( VECTOR pnt, VECTOR org, VECTOR dir)
  VECTOR pa = prj_pnt_onto_line(sp,(*lines)[2],(*lines)[1]);
  VECTOR pb = prj_pnt_onto_line(sp,(*lines)[3],(*lines)[1]);
  VECTOR pc = prj_pnt_onto_line(sp,(*lines)[4],(*lines)[1]);

printf(" start points ");
print_vector(pa,1,3);
print_vector(pb,1,3);
print_vector(pc,1,3);

// get end points
 for(i=1;i<=3;i++)
  {
    PHOTO_FLOAT d = 0;
    PHOTO_FLOAT s1 = 0;
    PHOTO_FLOAT s2 = 0;

   // intersect_lines_dir( VECTOR o1, VECTOR d1, VECTOR o2,
   //                      VECTOR d2, PHOTO_FLOAT *s1, PHOTO_FLOAT *s2 )
    
   d = intersect_lines_dir( &((*setm)[1][9]), (*sep)[1],
                            (*lines)[i+1],(*lines)[1],&s1,&s2);
   if(i ==1)
   {
     ln = 1;
     maxd = d;
     ss = s2;
   }
   else
   {
     if(d < maxd)
     {
       maxd = d;
       ln = i;
       ss = s2;
     }
   }


  }

  sp[1] = (*lines)[ln+1][1] + ss*((*lines)[1][1]);
  sp[2] = (*lines)[ln+1][2] + ss*((*lines)[1][2]);
  sp[3] = (*lines)[ln+1][3] + ss*((*lines)[1][3]);


printf(" start points ");
print_vector(sp,1,3);

//VECTOR prj_pnt_onto_line( VECTOR pnt, VECTOR org, VECTOR dir)
  VECTOR ea = prj_pnt_onto_line(sp,(*lines)[2],(*lines)[1]);
  VECTOR eb = prj_pnt_onto_line(sp,(*lines)[3],(*lines)[1]);
  VECTOR ec = prj_pnt_onto_line(sp,(*lines)[4],(*lines)[1]);



printf(" start points ");
print_vector(ea,1,3);
print_vector(eb,1,3);
print_vector(ec,1,3);

  VECTOR sc = vector(1,3);
 
  sc[1] = pa[1] + 0.5*(pb[1]-pa[1]) + 0.5*(pc[1]-pb[1]) + 0.5*(ea[1]-pa[1]);
  sc[2] = pa[2] + 0.5*(pb[2]-pa[2]) + 0.5*(pc[2]-pb[2]) + 0.5*(ea[2]-pa[2]);
  sc[3] = pa[3] + 0.5*(pb[3]-pa[3]) + 0.5*(pc[3]-pb[3]) + 0.5*(ea[3]-pa[3]);

  VECTOR tmp = vector(1,3);

  tmp[1] = pb[1]-pa[1];
  tmp[2] = pb[2]-pa[2];
  tmp[3] = pb[3]-pa[3];

printf(" x axis ");
print_vector(tmp,1,3);

  width = vector_length(3,tmp);
  VECTOR xaxis = unit_vector(3,tmp);
print_vector(xaxis,1,3);

  tmp[1] = ea[1] - pa[1];
  tmp[2] = ea[2] - pa[2];
  tmp[3] = ea[3] - pa[3];

printf(" y axis ");
print_vector(tmp,1,3);
  PHOTO_FLOAT height = vector_length(3,tmp);

  VECTOR yaxis = unit_vector(3,(*lines)[1]);
print_vector(yaxis,1,3);


  tmp[1] = pb[1]-pc[1];
  tmp[2] = pb[2]-pc[2];
  tmp[3] = pb[3]-pc[3];

printf(" z axis ");
print_vector(tmp,1,3);
  
  depth = vector_length(3,tmp);

  free_vector(tmp,1,3);
  tmp = cross_vector(xaxis,yaxis);
  VECTOR zaxis = unit_vector(3,tmp);

print_vector(zaxis,1,3);
  free_vector(tmp,1,3);

  tmp = cross_vector(yaxis,zaxis);
  free_vector(xaxis,1,3);
  xaxis = unit_vector(3,tmp);

  box.zlen = height;
  box.xlen = width;
  box.ylen = depth;
  
  trans.pozition.pos_x = sc[1];
  trans.pozition.pos_y = sc[2] ;
  trans.pozition.pos_z = sc[3];

  trans.rotation.r[0]= xaxis[1];
  trans.rotation.r[1]= xaxis[2];
  trans.rotation.r[2]= xaxis[3];

  trans.rotation.r[3]= 0.0;

  trans.rotation.r[4]= yaxis[1];
  trans.rotation.r[5]= yaxis[2];
  trans.rotation.r[6]= yaxis[3];

  trans.rotation.r[7]= 0.0;


  trans.rotation.r[8]= zaxis[1];
  trans.rotation.r[9]= zaxis[2];
  trans.rotation.r[10]= zaxis[3];

  trans.rotation.r[11]= 0.0;


  trans.rotation.r[12]= 0.0;
  trans.rotation.r[13]= 0.0;
  trans.rotation.r[14]= 0.0;

  trans.rotation.r[15]= 1.0;



  for(i=0;i<16;i++)printf("matrix[%d] = %e\n", i,trans.rotation.r[i]);
  free_vector(xaxis,1,3);
  free_vector(yaxis,1,3);
  free_vector(zaxis,1,3);


  trans.scale.sca_x = 1.0;
  trans.scale.sca_y = 1.0;
  trans.scale.sca_z = 1.0;

  trans.orientation.ori_x = 0.0;
  trans.orientation.ori_y = 0.0;
  trans.orientation.ori_z = 1.0;
  trans.orientation.ori_beta = 0.0;
  

  trans.center.cen_x = 0.0;
  trans.center.cen_y = 0.0;
  trans.center.cen_z = 0.0;

  free_vector(tmp,1,3);
  free_vector(sc,1,3);
  free_vector(pa,1,3);
  free_vector(pb,1,3);
  free_vector(pc,1,3);
  free_vector(ea,1,3);
  free_vector(eb,1,3);
  free_vector(ec,1,3);

  return(0);
   
}


void make_cyl_by_endpnts(PHOTO_FLOAT radius,PNT pnt1,PNT pnt2,
                         struct Cylinder &cyl,struct _transform &trans)
{

//    subtract_vector( (*sep)[1], 1, 3, (*sep)[0], 1, 3 );

  VECTOR cl = vector(1,3);

  cl[1] = pnt2[1] -pnt1[1];
  cl[2] = pnt2[2] -pnt1[2];
  cl[3] = pnt2[3] -pnt1[3];
    
    cyl.height = vector_length( 3, cl );

  //  fprintf( stderr, "cylinder height = %e\n", cyl.cyl_height );

/***** End of Start/End Points based on surface intersection *****/
    
 
  // get cyl tm

  VECTOR yaxis = unit_vector(3,cl);
//  yaxis[1] = cl[1];
//  yaxis[2] = cl[2];
//  yaxis[3] = cl[3];
  
  VECTOR temp = vector(1,3);
  temp[1] =  yaxis[3];
  temp[2] =  yaxis[1];
  temp[3] =  yaxis[2];

  VECTOR temp2 = cross_vector(temp,yaxis);
  VECTOR xaxis = unit_vector(3,temp2);
  VECTOR zaxis = cross_vector(xaxis,yaxis);

  free_vector(temp,1,3);
  free_vector(temp2,1,3);

 
  
  
  cyl.radius = radius;

//  free_vector(R,1,4);

/***** part of Start/End Points based on centerline intersection *****/
#if FALSE
  subtract_vector( (*sep)[1],1,3,(*sep)[0],1,3);
  
  cyl.cyl_height = vector_length(3,(*sep)[1]);
#endif
/***** End part of Start/End Points based on centerline intersection *****/


  //if(s2 < s1)multvector(3,cl,-1.0);

  trans.pozition.pos_x = pnt1[1] + 0.5*( cl[1]);
  trans.pozition.pos_y = pnt1[2] + 0.5*( cl[2]) ;
  trans.pozition.pos_z = pnt1[3] + 0.5*( cl[3]);

  trans.rotation.r[0]= xaxis[1];
  trans.rotation.r[1]= xaxis[2];
  trans.rotation.r[2]= xaxis[3];

  trans.rotation.r[3]= 0.0;

  trans.rotation.r[4]= yaxis[1];
  trans.rotation.r[5]= yaxis[2];
  trans.rotation.r[6]= yaxis[3];

  trans.rotation.r[7]= 0.0;


  trans.rotation.r[8]= zaxis[1];
  trans.rotation.r[9]= zaxis[2];
  trans.rotation.r[10]= zaxis[3];

  trans.rotation.r[11]= 0.0;


  trans.rotation.r[12]= 0.0;
  trans.rotation.r[13]= 0.0;
  trans.rotation.r[14]= 0.0;

  trans.rotation.r[15]= 1.0;



  for(int i=0;i<16;i++)printf("matrix[%d] = %e\n", i,trans.rotation.r[i]);
  free_vector(xaxis,1,3);
  free_vector(yaxis,1,3);
  free_vector(zaxis,1,3);


  trans.scale.sca_x = 1.0;
  trans.scale.sca_y = 1.0;
  trans.scale.sca_z = 1.0;

  trans.orientation.ori_x = 0.0;
  trans.orientation.ori_y = 0.0;
  trans.orientation.ori_z = 1.0;
  trans.orientation.ori_beta = 0.0;
  

  trans.center.cen_x = 0.0;
  trans.center.cen_y = 0.0;
  trans.center.cen_z = 0.0;

  free_vector(cl,1,3);
  return;

}





int make_cyl_by_lines(int nl,int *npl, PNTLST *pnts,PNTLST tms, PNTLST drp,
                        PNTLST sep, PNTLST setm , struct Cylinder &cyl,
                        struct _transform &trans)
{

   if(setjmp(env) != 0)
  {
   return(1);
  }

  int i,j;
  PNTLST norm = matrix_shell(1,nl,1,3);


// convert to real world system 

  for(i=0;i<nl;i++)
  {

   printf("tm vector\n");
print_vector((*tms)[i],1,12);

     VECTOR inv = matinv((*tms)[i]); 
     copy_vector((*tms)[i],1,12,inv,1,12);

     

     for(j=0;j<npl[i];j++)
     {
       rotpnt(inv, (*(pnts[i]))[j] );

     }

     rotpnt(inv,(*drp)[i] );

printf("drp's");
print_vector( (*drp)[i] ,1,3);

     free_vector(inv,1,12);
  }

//getchar();
print_matrix(sep,0,1,1,3); 
  for(i=0;i< 2;i++)
  {
     VECTOR inv = matinv((*setm)[i]); 
     copy_vector((*setm)[i],1,12,inv,1,12);

      rotpnt(inv, (*sep)[i] );

     free_vector(inv,1,12);
  }



  // get tangent plane normals for each line
 
  for( i = 0; i< nl; i++)
  {
   
    int np = npl[i];
    PNTLST p = pnts[i];
    VECTOR tm = (*tms)[i];

    VECTOR  L = vector(1,np+1); 
   printf("tm vector\n");
print_vector((*tms)[i],1,12);
    PNTLST  A = matrix(1,np+1,1,3);

    for(j=0;j< np;j++)
    {
      L[j+1] = 0.0; 
 
      (*A)[j+1][1] = (*p)[j][1];   
      (*A)[j+1][2] = (*p)[j][2]; 
      (*A)[j+1][3] = (*p)[j][3]; 

    }

    L[np+1] = 1.0;
    (*A)[np+1][1] = 1.0;
    (*A)[np+1][2] = 1.0;
    (*A)[np+1][3] = 1.0;

    VECTOR R = lsq(A,L,3,np+1);
printf("norm vectors\n");
print_vector(R,1,3);

    (*norm)[i+1] = unit_vector(3,R);
print_vector((*norm)[i+1] ,1,3);
    free_vector(R,1,3);
    free_vector(L,1,np);
    free_matrix(A,1,np,1,3);

  }



  // get center line unit vector , cl   

   VECTOR L = vector(1,nl+1);
   PNTLST A = matrix(1,nl+1,1,3);

   for(i=1;i<=nl;i++)
   {
     L[i] = 0.0;

     (*A)[i][1] = (*norm)[i][1];
     (*A)[i][2] = (*norm)[i][2];
     (*A)[i][3] = (*norm)[i][3];
   }

   L[nl+1] = 1.0;
   (*A)[nl+1][1] = 1.0;
   (*A)[nl+1][2] = 1.0;
   (*A)[nl+1][3] = 1.0;

  VECTOR R = lsq(A,L,3,nl+1);
printf("cl vectors\n");
print_vector(R,1,3);

  VECTOR cl = unit_vector(3,R);
print_vector(cl,1,3);

  free_vector(L,1,nl+1);
  free_vector(R,1,3);
  free_matrix(A,1,nl+1,1,3);



for(i=1;i<=nl;i++)printf("normal check  line %d,  %e\n",i,dot_vector(3,cl,(*norm)[i] ));


// adjust tangent plane normals for direction points


  for(i=0;i<nl;i++)
  {
    if(dot_vector(3, (*norm)[i+1], (*drp)[i] )  < 0) multvector(3,(*norm)[i+1],-1.0 );
print_vector((*norm)[i+1] ,1,3);


  }


// calculate center line origin 

  R = vector(1,7);
  L = vector(1,nl+4);
  A = matrix(1,nl+4,1,7);

  R[1] = 1.0;
  R[2] = (100.0*(*drp)[0][1]) +((*tms)[0][10]);
  R[3] = (100.0*(*drp)[0][2]) +((*tms)[0][11]);
  R[4] = (100.0*(*drp)[0][3]) +((*tms)[0][12]);

  for(int k = 1; k <= 100; k++)
  {
    for(i=1;i<nl;i++)
    {

//print_vector((*tms)[i],1,12);
      L[i] = -dot_vector(3,&((*tms)[i][9]),(*norm)[i+1]);
      L[i] -= R[1]*R[1];
      L[i] += (*norm)[i+1][1]*R[2];
      L[i] += (*norm)[i+1][2]*R[3];
      L[i] += (*norm)[i+1][3]*R[4];
      (*A)[i][1] = 2.0*R[1];
      (*A)[i][2] = -(*norm)[i+1][1]; 
      (*A)[i][3] = -(*norm)[i+1][2]; 
      (*A)[i][4] = -(*norm)[i+1][3]; 
    }

    L[nl] = 10000.0 *( (*tms)[0][10]-R[2] + R[5]+ R[1]*R[1]*(*norm)[1][1]);
    L[nl+1] =10000.0*( (*tms)[0][11]-R[3] + R[6]+ R[1]*R[1]*(*norm)[1][2]);
    L[nl+2] =10000.0*( (*tms)[0][12]-R[4] + R[7]+ R[1]*R[1]*(*norm)[1][3]);

    (*A)[nl][2] = 1.0*10000.0;
    (*A)[nl][5] = -1.0*10000.0;
    (*A)[nl][1] = -2.0*(*norm)[1][1]*10000.0;

    (*A)[nl+1][3] = 1.0*10000.0;
    (*A)[nl+1][6] = -1.0*10000.0;
    (*A)[nl+1][1] = -2.0*(*norm)[1][2]*10000.0;

    (*A)[nl+2][4] = 1.0*10000.0;
    (*A)[nl+2][7] = -1.0*10000.0;
    (*A)[nl+2][1] = -2.0*(*norm)[1][3]*10000.0;

    L[nl+3] = (R[5]*(*norm)[1][1]+R[6]*(*norm)[1][2]+R[7]*(*norm)[1][3])*10000.0;
    (*A)[nl+3][5] = -(*norm)[1][1]*10000.0;
    (*A)[nl+3][6] = -(*norm)[1][2]*10000.0;
    (*A)[nl+3][7] = -(*norm)[1][3]*10000.0;

   L[nl+4] = (R[5]*cl[1]+R[6]*cl[2]+R[7]*cl[3])*10000.0;
    (*A)[nl+4][5] = -cl[1]*10000.0;
    (*A)[nl+4][6] = -cl[2]*10000.0;
    (*A)[nl+4][7] = -cl[3]*10000.0;
  //  L[nl+1]=0.0;
  //  (*A)[nl+1][2] = 1000,0*cl[1];
  //  (*A)[nl+1][3] = 1000.0*cl[2];
  //  (*A)[nl+1][4] = 1000.0*cl[3];


    VECTOR D =lsq(A,L,7,nl+4);
    R[1] += D[1];
    R[2] += D[2];
    R[3] += D[3];
    R[4] += D[4];
    R[5] += D[5];
    R[6] += D[6];
    R[7] += D[7];
   
print_vector(R,1,7);
    if(R[1] <0.0)R[1]= -R[1];
print_vector(L,1,nl+4);
print_vector(D,1,7);
    free_vector(D,1,7);
  }
  
  R[1] = R[1]*R[1];


  free_vector(L,1,nl+4);
  free_matrix(A,1,nl+4,1,7);


// check center point

  for(i = 1; i<= nl; i++)
  {

    PNT d = clone_vector( &( R[1]),1,3,1,3);
    subtract_vector(d,1,3,&((*tms)[i-1][9]),1,3);
    printf("Radius = %e\n",dot_vector(3,d,(*norm)[i]));
    free_vector(d,1,3);
  }

print_vector(R,1,7);


/***** Start/End Points based on centerline intersection *****/

#if FALSE
  // get start and end points

  PHOTO_FLOAT s1,s2;
 
  for(i=1;i<=2;i++)
  {
  
    intersect_lines_dir(&(R[1]),cl, &((*setm)[i-1][9]), (*sep)[i-1],&s1,&s2);
    
    for(j=1;j<=3;j++)
    {
      (*sep)[i-1][j] =  R[j+1]+ s1*cl[j];
    }
  }   
#endif

/***** End of Start/End Points based on centerline intersection *****/

/***** Start/End Points based on surface intersection *****/  

    PNTLST pl;
    VECTOR v1;
    for( i = 1; i <= 2; i++ )
    {
	pl = intersect_line_cyl( R[1], &(R[1]), cl, &((*setm)[i-1][9]), (*sep)[i-1] );

	v1 = prj_pnt_onto_line( (*pl)[1], &(R[1]), cl );
	
	copy_vector( (*sep)[i-1], 1, 3, v1, 1, 3 );
	
	free_matrix( pl, 1, 2, 1, 3 );
	free_vector( v1, 1, 3 );
    }
    
    fprintf( stderr, "After calls to intersect_line_cyl\n" );
    

   make_cyl_by_endpnts(R[1],(*sep)[1],(*sep)[0],cyl,trans);

   free_vector(R,1,4);
   free_matrix(norm,1,nl,1,3);
   free_vector(cl,1,3);
   return(0);


    subtract_vector( (*sep)[1], 1, 3, (*sep)[0], 1, 3 );
    
    cyl.height = vector_length( 3, (*sep)[1] );

    fprintf( stderr, "cylinder height = %lf\n", cyl.height );

/***** End of Start/End Points based on surface intersection *****/
    
 
  // get cyl tm

  VECTOR yaxis = vector(1,3);
  yaxis[1] = cl[1];
  yaxis[2] = cl[2];
  yaxis[3] = cl[3];
  
  VECTOR temp = vector(1,3);
  temp[1] =  yaxis[3];
  temp[2] =  yaxis[1];
  temp[3] =  yaxis[2];

  VECTOR temp2 = cross_vector(temp,yaxis);
  VECTOR xaxis = unit_vector(3,temp2);
  VECTOR zaxis = cross_vector(xaxis,yaxis);

  free_vector(temp,1,3);
  free_vector(temp2,1,3);

 
  
  
  cyl.radius = R[1];

  free_vector(R,1,4);

/***** part of Start/End Points based on centerline intersection *****/
#if FALSE
  subtract_vector( (*sep)[1],1,3,(*sep)[0],1,3);
  
  cyl.cyl_height = vector_length(3,(*sep)[1]);
#endif
/***** End part of Start/End Points based on centerline intersection *****/


  //if(s2 < s1)multvector(3,cl,-1.0);

  trans.pozition.pos_x = (*sep)[0][1] + 0.5*( (*sep)[1][1]);
  trans.pozition.pos_y = (*sep)[0][2] + 0.5*( (*sep)[1][2]) ;
  trans.pozition.pos_z = (*sep)[0][3] + 0.5*( (*sep)[1][3]);

  trans.rotation.r[0]= xaxis[1];
  trans.rotation.r[1]= xaxis[2];
  trans.rotation.r[2]= xaxis[3];

  trans.rotation.r[3]= 0.0;

  trans.rotation.r[4]= yaxis[1];
  trans.rotation.r[5]= yaxis[2];
  trans.rotation.r[6]= yaxis[3];

  trans.rotation.r[7]= 0.0;


  trans.rotation.r[8]= zaxis[1];
  trans.rotation.r[9]= zaxis[2];
  trans.rotation.r[10]= zaxis[3];

  trans.rotation.r[11]= 0.0;


  trans.rotation.r[12]= 0.0;
  trans.rotation.r[13]= 0.0;
  trans.rotation.r[14]= 0.0;

  trans.rotation.r[15]= 1.0;



  for(i=0;i<16;i++)printf("matrix[%d] = %e\n", i,trans.rotation.r[i]);
  free_vector(xaxis,1,3);
  free_vector(yaxis,1,3);
  free_vector(zaxis,1,3);


  trans.scale.sca_x = 1.0;
  trans.scale.sca_y = 1.0;
  trans.scale.sca_z = 1.0;

  trans.orientation.ori_x = 0.0;
  trans.orientation.ori_y = 0.0;
  trans.orientation.ori_z = 1.0;
  trans.orientation.ori_beta = 0.0;
  

  trans.center.cen_x = 0.0;
  trans.center.cen_y = 0.0;
  trans.center.cen_z = 0.0;

  return(0);

}
/////////////////////////////////////////////////////////////////////
// Make infinite length cylinder and return just the radius
int make_cyl_by_lines(int nl,int *npl, PNTLST *pnts,PNTLST tms, PNTLST drp,
                        struct Cylinder &cyl, struct _transform &trans)
{

   if(setjmp(env) != 0)
  {
   return(1);
  }

  int i,j;
  PNTLST norm = matrix_shell(1,nl,1,3);


// convert to real world system 

  for(i=0;i<nl;i++)
  {

   printf("tm vector\n");
print_vector((*tms)[i],1,12);

     VECTOR inv = matinv((*tms)[i]); 
     copy_vector((*tms)[i],1,12,inv,1,12);

     

     for(j=0;j<npl[i];j++)
     {
       rotpnt(inv, (*(pnts[i]))[j] );

     }

     rotpnt(inv,(*drp)[i] );

printf("drp's");
print_vector( (*drp)[i] ,1,3);

     free_vector(inv,1,12);
  }

  // get tangent plane normals for each line
 
  for( i = 0; i< nl; i++)
  {
   
    int np = npl[i];
    PNTLST p = pnts[i];
    VECTOR tm = (*tms)[i];

    VECTOR  L = vector(1,np+1); 
   printf("tm vector\n");
print_vector((*tms)[i],1,12);
    PNTLST  A = matrix(1,np+1,1,3);

    for(j=0;j< np;j++)
    {
      L[j+1] = 0.0; 
 
      (*A)[j+1][1] = (*p)[j][1];   
      (*A)[j+1][2] = (*p)[j][2]; 
      (*A)[j+1][3] = (*p)[j][3]; 

    }

    L[np+1] = 1.0;
    (*A)[np+1][1] = 1.0;
    (*A)[np+1][2] = 1.0;
    (*A)[np+1][3] = 1.0;

    VECTOR R = lsq(A,L,3,np+1);
printf("norm vectors\n");
print_vector(R,1,3);

    (*norm)[i+1] = unit_vector(3,R);
print_vector((*norm)[i+1] ,1,3);
    free_vector(R,1,3);
    free_vector(L,1,np);
    free_matrix(A,1,np,1,3);

  }



  // get center line unit vector , cl   

   VECTOR L = vector(1,nl+1);
   PNTLST A = matrix(1,nl+1,1,3);

   for(i=1;i<=nl;i++)
   {
     L[i] = 0.0;

     (*A)[i][1] = (*norm)[i][1];
     (*A)[i][2] = (*norm)[i][2];
     (*A)[i][3] = (*norm)[i][3];
   }

   L[nl+1] = 1.0;
   (*A)[nl+1][1] = 1.0;
   (*A)[nl+1][2] = 1.0;
   (*A)[nl+1][3] = 1.0;

  VECTOR R = lsq(A,L,3,nl+1);
printf("cl vectors\n");
print_vector(R,1,3);

  VECTOR cl = unit_vector(3,R);
print_vector(cl,1,3);

  free_vector(L,1,nl+1);
  free_vector(R,1,3);
  free_matrix(A,1,nl+1,1,3);



for(i=1;i<=nl;i++)printf("normal check  line %d,  %e\n",i,dot_vector(3,cl,(*norm)[i] ));


// adjust tangent plane normals for direction points


  for(i=0;i<nl;i++)
  {
    if(dot_vector(3, (*norm)[i+1], (*drp)[i] )  < 0) multvector(3,(*norm)[i+1],-1.0 );
print_vector((*norm)[i+1] ,1,3);


  }


// calculate center line origin 

  R = vector(1,7);
  L = vector(1,nl+4);
  A = matrix(1,nl+4,1,7);

  R[1] = 1.0;
  R[2] = (100.0*(*drp)[0][1]) +((*tms)[0][10]);
  R[3] = (100.0*(*drp)[0][2]) +((*tms)[0][11]);
  R[4] = (100.0*(*drp)[0][3]) +((*tms)[0][12]);

  for(int k = 1; k <= 100; k++)
  {
    for(i=1;i<nl;i++)
    {

//print_vector((*tms)[i],1,12);
      L[i] = -dot_vector(3,&((*tms)[i][9]),(*norm)[i+1]);
      L[i] -= R[1]*R[1];
      L[i] += (*norm)[i+1][1]*R[2];
      L[i] += (*norm)[i+1][2]*R[3];
      L[i] += (*norm)[i+1][3]*R[4];
      (*A)[i][1] = 2.0*R[1];
      (*A)[i][2] = -(*norm)[i+1][1]; 
      (*A)[i][3] = -(*norm)[i+1][2]; 
      (*A)[i][4] = -(*norm)[i+1][3]; 
    }

    L[nl] = 10000.0 *( (*tms)[0][10]-R[2] + R[5]+ R[1]*R[1]*(*norm)[1][1]);
    L[nl+1] =10000.0*( (*tms)[0][11]-R[3] + R[6]+ R[1]*R[1]*(*norm)[1][2]);
    L[nl+2] =10000.0*( (*tms)[0][12]-R[4] + R[7]+ R[1]*R[1]*(*norm)[1][3]);

    (*A)[nl][2] = 1.0*10000.0;
    (*A)[nl][5] = -1.0*10000.0;
    (*A)[nl][1] = -2.0*(*norm)[1][1]*10000.0;

    (*A)[nl+1][3] = 1.0*10000.0;
    (*A)[nl+1][6] = -1.0*10000.0;
    (*A)[nl+1][1] = -2.0*(*norm)[1][2]*10000.0;

    (*A)[nl+2][4] = 1.0*10000.0;
    (*A)[nl+2][7] = -1.0*10000.0;
    (*A)[nl+2][1] = -2.0*(*norm)[1][3]*10000.0;

    L[nl+3] = (R[5]*(*norm)[1][1]+R[6]*(*norm)[1][2]+R[7]*(*norm)[1][3])*10000.0;
    (*A)[nl+3][5] = -(*norm)[1][1]*10000.0;
    (*A)[nl+3][6] = -(*norm)[1][2]*10000.0;
    (*A)[nl+3][7] = -(*norm)[1][3]*10000.0;

   L[nl+4] = (R[5]*cl[1]+R[6]*cl[2]+R[7]*cl[3])*10000.0;
    (*A)[nl+4][5] = -cl[1]*10000.0;
    (*A)[nl+4][6] = -cl[2]*10000.0;
    (*A)[nl+4][7] = -cl[3]*10000.0;
  //  L[nl+1]=0.0;
  //  (*A)[nl+1][2] = 1000,0*cl[1];
  //  (*A)[nl+1][3] = 1000.0*cl[2];
  //  (*A)[nl+1][4] = 1000.0*cl[3];


    VECTOR D =lsq(A,L,7,nl+4);
    R[1] += D[1];
    R[2] += D[2];
    R[3] += D[3];
    R[4] += D[4];
    R[5] += D[5];
    R[6] += D[6];
    R[7] += D[7];
   
print_vector(R,1,7);
    if(R[1] <0.0)R[1]= -R[1];
print_vector(L,1,nl+4);
print_vector(D,1,7);
    free_vector(D,1,7);
  }
  
  R[1] = R[1]*R[1];


  free_vector(L,1,nl+4);
  free_matrix(A,1,nl+4,1,7);


// check center point

  for(i = 1; i<= nl; i++)
  {

    PNT d = clone_vector( &( R[1]),1,3,1,3);
    subtract_vector(d,1,3,&((*tms)[i-1][9]),1,3);
    TRACE("Radius = %e\n",dot_vector(3,d,(*norm)[i]));
    free_vector(d,1,3);
  }
TRACE("RADIUS R1 = %e\n",R[1] );
	cyl.radius = R[1];
 
print_vector(R,1,7);


  return(0);

}

/////////////////////////////////////////////////////////////////////


void xmake_cyl_by_3dpnts(int np,PNTLST pnts,PNTLST sep, PNTLST setm , struct Cylinder &cyl,
                        struct _transform &trans)
{


  for(int i=0;i< 2;i++)
  {
     VECTOR inv = matinv((*setm)[i]); 
     copy_vector((*setm)[i],1,12,inv,1,12);

      rotpnt(inv, (*sep)[i] );

     free_vector(inv,1,12);
  }




  int nu = 7;
  int ne = 3 + np;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);
  PHOTO_FLOAT RAS = 0.0;



  R[1] = 1.0;
  R[6] = 1.0;

  for( int k = 0; k< np; k++)
  {
    R[1] =((*pnts)[k][1])/np;
    R[2] =((*pnts)[k][2])/np;
    R[3] =((*pnts)[k][3])/np;
  }

  R[7] = sqrt(R[1]*R[1] + R[2]*R[2] + R[3]*R[3]);


  for(int loop = 0; loop < 200; loop++)
  {

    for( int j = 1; j <= 4; j += 3)
    {
      PHOTO_FLOAT temp = sqrt( R[j]*R[j] + R[j+1]*R[j+1] + R[j+2]*R[j+2]);
      R[j] =   R[j]/temp;
      R[j+1] = R[j+1]/temp;
      R[j+2] = R[j+2]/temp;
    }


  
//    forcesquare( L, R, *A,1.0e5);

    int e = 1;

    L[e] = 100000.0*(1.0 - R[1]*R[1] - R[2]*R[2] - R[3]*R[3] );

    (*A)[e][1] =100000.0*2.0*R[1];
    (*A)[e][2] =100000.0*2.0*R[2];
    (*A)[e][3] =100000.0*2.0*R[3];

    e++;


    L[e] = 100000.0*(1.0 - R[4]*R[4] - R[5]*R[5] - R[6]*R[6] );

    (*A)[e][4] =100000.0*2.0*R[4];
    (*A)[e][5] =100000.0*2.0*R[5];
    (*A)[e][6] =100000.0*2.0*R[6];

    e++;

    L[e] = 100000.0*( - R[1]*R[4] - R[2]*R[5] - R[3]*R[6] );

    (*A)[e][1] =100000.0*R[4];
    (*A)[e][2] =100000.0*R[5];
    (*A)[e][3] =100000.0*R[6];
    (*A)[e][4] =100000.0*R[1];
    (*A)[e][5] =100000.0*R[2];
    (*A)[e][6] =100000.0*R[3];

    e++;


   // int e = 10;

    for(int i = 0; i < np; i++)
    {
       RAS = 0.0;

      (*A)[e][1] = 0.0;    
      (*A)[e][2] = 0.0;
      (*A)[e][3] = 0.0;
      (*A)[e][4] = 0.0;    
      (*A)[e][5] = 0.0;    
      (*A)[e][6] = 0.0;    
    
      (*A)[e][7] =0.0;    

      for(int k = 0; k<np; k++)
      {
  
        PHOTO_FLOAT x = R[1]*((*pnts)[k][1]) + R[2]*((*pnts)[k][2]) + R[3]*((*pnts)[k][3]) - R[7];
        PHOTO_FLOAT y = R[4]*((*pnts)[k][1]) + R[5]*((*pnts)[k][2]) + R[6]*((*pnts)[k][3]);

        RAS += (x*x + y*y)/np;

        (*A)[e][1] -= (2.0/np)*x*((*pnts)[k][1]);    
        (*A)[e][2] -= (2.0/np)*x*((*pnts)[k][2]);
        (*A)[e][3] -= (2.0/np)*x*((*pnts)[k][3]);
        (*A)[e][4] -= (2.0/np)*y*((*pnts)[k][1]);    
        (*A)[e][5] -= (2.0/np)*y*((*pnts)[k][2]);    
        (*A)[e][6] -= (2.0/np)*y*((*pnts)[k][3]);    
    
        (*A)[e][7] -= -(2.0/np)*x;    
      }
    
printf(" ras = %e, %e\n",RAS,sqrt(RAS));
      PHOTO_FLOAT x = R[1]*((*pnts)[i][1]) + R[2]*((*pnts)[i][2]) + R[3]*((*pnts)[i][3]) - R[7];
      PHOTO_FLOAT y = R[4]*((*pnts)[i][1]) + R[5]*((*pnts)[i][2]) + R[6]*((*pnts)[i][3]);

printf("rad = %e,%e\n",(x*x + y*y),sqrt((x*x + y*y) ) );
      L[e] = RAS - (x*x + y*y);

      (*A)[e][1] += 2.0*x*((*pnts)[i][1]);
      (*A)[e][2] += 2.0*x*((*pnts)[i][2]);
      (*A)[e][3] += 2.0*x*((*pnts)[i][3]);
      (*A)[e][4] += 2.0*y*((*pnts)[i][1]);
      (*A)[e][5] += 2.0*y*((*pnts)[i][2]);
      (*A)[e][6] += 2.0*y*((*pnts)[i][3]);

      (*A)[e][7] += -2.0*x;
   
      e++;

    }


   printf("vector R");
    print_vector(R,1,nu);

printf("vector L");
print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

   for(i=1;i<= nu;i++)R[i] += 0.5*D[i];

printf("vector D");
print_vector(D,1,nu);

 printf(" loop - %d\n", loop);
    free_vector(D,1,nu);
 

  }// end loop for iteration


 free_matrix(A,1,ne,1,nu);

  VECTOR c = cross_vector(R, &(R[4]) );
  VECTOR cl  = unit_vector(3,c);
  free_vector(c,1,3);

printf("z vector + %e,%e,%e\n",cl[1],cl[2],cl[3]);

  R[1] = R[1]*R[7];
  R[2] = R[2]*R[7];
  R[3] = R[3]*R[7];

  RAS = sqrt(RAS);

  // get end points of cylinder

 
    PNTLST pl;
    VECTOR v1;
    for( i = 1; i <= 2; i++ )
    {
        //PNTLST intersect_line_cyl(PHOTO_FLOAT r, VECTOR c_org, VECTOR c_dir, VECTOR l_org, VECTOR l_dir)
	pl = intersect_line_cyl( RAS, R, cl, &((*setm)[i-1][9]), (*sep)[i-1] );

//VECTOR prj_pnt_onto_line( VECTOR pnt, VECTOR org, VECTOR dir)
	v1 = prj_pnt_onto_line( (*pl)[1], R, cl );
	
	copy_vector( (*sep)[i-1], 1, 3, v1, 1, 3 );
	
	free_matrix( pl, 1, 2, 1, 3 );
	free_vector( v1, 1, 3 );
    }
 
   make_cyl_by_endpnts(RAS,(*sep)[1],(*sep)[0],cyl,trans);

 free_vector(R,1,nu);
 free_vector(cl,1,3);


  return;
}

VECTOR ccc(int np, PNTLST pnts)
{

  // given np points on the surface of a cylinder, this routine calculates the
  // the center line of the cylinder ( RR[1] thru RR[3] = point on center line,
  // RR[4] thru RR[6] = direction unit vector) and radius( RR[7] );


  int nu = 7;
  int ne = 3 + np*np -np;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);
  PHOTO_FLOAT RAS = 0.0;



  R[1] = 1.0;
  R[6] = 1.0;

  for( int k = 0; k< np; k++)
  {
    R[1] =((*pnts)[k][1])/np;
    R[2] =((*pnts)[k][2])/np;
    R[3] =((*pnts)[k][3])/np;
  }

  R[7] = sqrt(R[1]*R[1] + R[2]*R[2] + R[3]*R[3]);


  for(int loop = 0; loop < 200; loop++)
  {

    for( int j = 1; j <= 4; j += 3)
    {
      PHOTO_FLOAT temp = sqrt( R[j]*R[j] + R[j+1]*R[j+1] + R[j+2]*R[j+2]);
      R[j] =   R[j]/temp;
      R[j+1] = R[j+1]/temp;
      R[j+2] = R[j+2]/temp;
    }


    VECTOR z = cross_vector(R, &(R[3]) );
    VECTOR yy = cross_vector(z, R);
    VECTOR yu  = unit_vector(3,yy);
      R[4] = yu[1];
      R[5] = yu[2];
      R[6] = yu[3];

  
//    forcesquare( L, R, *A,1.0e5);

    int e = 1;

    L[e] = 100000.0*(1.0 - R[1]*R[1] - R[2]*R[2] - R[3]*R[3] );

    (*A)[e][1] =100000.0*2.0*R[1];
    (*A)[e][2] =100000.0*2.0*R[2];
    (*A)[e][3] =100000.0*2.0*R[3];

    e++;


    L[e] = 100000.0*(1.0 - R[4]*R[4] - R[5]*R[5] - R[6]*R[6] );

    (*A)[e][4] =100000.0*2.0*R[4];
    (*A)[e][5] =100000.0*2.0*R[5];
    (*A)[e][6] =100000.0*2.0*R[6];

    e++;

    L[e] = 1.0e7*( - R[1]*R[4] - R[2]*R[5] - R[3]*R[6] );

    (*A)[e][1] =1.0e7*R[4];
    (*A)[e][2] =1.0e7*R[5];
    (*A)[e][3] =1.0e7*R[6];
    (*A)[e][4] =1.0e7*R[1];
    (*A)[e][5] =1.0e7*R[2];
    (*A)[e][6] =1.0e7*R[3];

    e++;


   // int e = 10;

    for(int i = 0; i < np; i++)
    {

      for(int k = 0; k<np; k++)
      {
        PHOTO_FLOAT xk = R[1]*((*pnts)[k][1]) + R[2]*((*pnts)[k][2]) + R[3]*((*pnts)[k][3]) - R[7];
        PHOTO_FLOAT yk = R[4]*((*pnts)[k][1]) + R[5]*((*pnts)[k][2]) + R[6]*((*pnts)[k][3]);
       // PHOTO_FLOAT rk = sqrt(xk*xk + yk*yk);

//printf("radius = %e\n",rk);

        if(i != k )
        {
  
          PHOTO_FLOAT xi = R[1]*((*pnts)[i][1]) + R[2]*((*pnts)[i][2]) + R[3]*((*pnts)[i][3]) - R[7];
          PHOTO_FLOAT yi = R[4]*((*pnts)[i][1]) + R[5]*((*pnts)[i][2]) + R[6]*((*pnts)[i][3]);
          PHOTO_FLOAT ri = sqrt(xi*xi + yi*yi);
          
          L[e] =(xk*xk + yk*yk) - (xi*xi + yi*yi);

          (*A)[e][1] = -2.0*xk*((*pnts)[k][1]) + 2.0*xi*((*pnts)[i][1]);    
          (*A)[e][2] = -2.0*xk*((*pnts)[k][2]) + 2.0*xi*((*pnts)[i][2]);
          (*A)[e][3] = -2.0*xk*((*pnts)[k][3]) + 2.0*xi*((*pnts)[i][3]);
          (*A)[e][4] = -2.0*yk*((*pnts)[k][1]) + 2.0*yi*((*pnts)[i][1]);    
          (*A)[e][5] = -2.0*yk*((*pnts)[k][2]) + 2.0*yi*((*pnts)[i][2]);    
          (*A)[e][6] = -2.0*yk*((*pnts)[k][3]) + 2.0*yi*((*pnts)[i][3]);    
    
          (*A)[e][7] = 2.0*xk - 2.0*xi;
        

          e++; 

        }   
      }
    
   

    }


    for(int k = 0; k<np; k++)
    {
      PHOTO_FLOAT xk = R[1]*((*pnts)[k][1]) + R[2]*((*pnts)[k][2]) + R[3]*((*pnts)[k][3]) - R[7];
      PHOTO_FLOAT yk = R[4]*((*pnts)[k][1]) + R[5]*((*pnts)[k][2]) + R[6]*((*pnts)[k][3]);
      PHOTO_FLOAT rk = sqrt(xk*xk + yk*yk);

printf("radius %d = %e\n",k,rk);
    }

   printf("vector R");
    print_vector(R,1,nu);

printf("vector L");
print_vector(L,1,ne);

     VECTOR D = lsq(A,L,nu,ne);

   for(i=1;i<= nu;i++)R[i] += 0.333*D[i];

printf("vector D");
print_vector(D,1,nu);

 printf(" loop - %d\n", loop);
    free_vector(D,1,nu);
 

  }// end loop for iteration


 free_matrix(A,1,ne,1,nu);

  VECTOR c = cross_vector(R, &(R[3]) );
  VECTOR cl  = unit_vector(3,c);
  free_vector(c,1,3);

printf("z vector + %e,%e,%e\n",cl[1],cl[2],cl[3]);


  for(k=0;k<np;k++)
  {
    PHOTO_FLOAT x = R[1]*((*pnts)[k][1]) + R[2]*((*pnts)[k][2]) + R[3]*((*pnts)[k][3]) - R[7];
    PHOTO_FLOAT y = R[4]*((*pnts)[k][1]) + R[5]*((*pnts)[k][2]) + R[6]*((*pnts)[k][3]);

    RAS += sqrt(x*x + y*y);
  }

  RAS =RAS/np;

printf(" RAS = %e\n",RAS);


  VECTOR RR = vector(1,7);

  RR[1] = R[1]*R[7];
  RR[2] = R[2]*R[7];
  RR[3] = R[3]*R[7];

  RR[4] = cl[1];
  RR[5] = cl[2];
  RR[6] = cl[3];

  RR[7] = RAS;

  free_vector(R,1,ne);
  free_vector(cl,1,3);

  return(RR);
}









VECTOR cccp(int np, PNTLST pnts, VECTOR initu)
{

  // given np points on the surface of a cylinder, this routine calculates the
  // the center line of the cylinder ( RR[1] thru RR[3] = point on center line,
  // RR[4] thru RR[6] = direction unit vector) and radius( RR[7] );


  int nu = 6 + np;
  int ne = 2 + np + np*np - np;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);

  int ui = 1;
  int ci = ui + 3;
  int ti = ci + 3;

  //R[1] = R[2] = R[3] = R[4] = R[5] = R[6] = 1.0;

   if(initu == NULL)
   {
     R[3] = -1.0;
   }
   else
   {
     R[1] = initu[1];
     R[2] = initu[2];
     R[3] = initu[3];
   } 
    
    //R[6] = -100.0;

  PHOTO_FLOAT cx = 0.0;
  PHOTO_FLOAT cy = 0.0;
  PHOTO_FLOAT cz = 0.0;

  for(int i = 0; i <np; i++)
  {
    R[ti+i] = 100.0/(*pnts)[i][3];
    cx += R[ti+i]*(*pnts)[i][1];
    cy += R[ti+i]*(*pnts)[i][2];
    cz += R[ti+i]*(*pnts)[i][3];
  }
    

  R[4] =  cx/np;
  R[5] =  cy/np;
  R[6] =  cz/np;

  


  for(int loop = 0; loop < 200; loop++)
  {

    ui = 1;
    ci = ui + 3;
    ti = ci + 3;
 
    int e = 1;


    // U is a unit vector defining a plane 100 units from the system origin

    // U.U = 1.0

    PHOTO_FLOAT temp = sqrt(R[1]*R[1] + R[2]*R[2] + R[3]*R[3]);

    R[1] = R[1]/temp;
    R[2] = R[2]/temp;
    R[3] = R[3]/temp;

    L[e] = 100000.0*(1.0 - R[1]*R[1] - R[2]*R[2] - R[3]*R[3] );

    (*A)[e][1] =100000.0*2.0*R[1];
    (*A)[e][2] =100000.0*2.0*R[2];
    (*A)[e][3] =100000.0*2.0*R[3];

    e++;

    // C is the sought circle center in the plane defined by U
    // C.U = 100.0  constrain C to plane

    temp =  100.0/(R[1]*R[4] + R[2]*R[5] + R[3]*R[6]);

    R[4] *= temp;
    R[5] *= temp;
    R[6] *= temp;

    L[e] = 100000.0*(100.0 - R[1]*R[4] - R[2]*R[5] - R[3]*R[6] );

    (*A)[e][1] =100000.0*R[4];
    (*A)[e][2] =100000.0*R[5];
    (*A)[e][3] =100000.0*R[6];

    (*A)[e][4] =100000.0*R[1];
    (*A)[e][5] =100000.0*R[2];
    (*A)[e][6] =100000.0*R[3];

    e++;

    for( i = 0; i < np; i++)
    {

      // point on photo ray define by scalar ( P = t*F, F being a unit vector)
      // force point to plane ( t*P.U = 100)

      R[ti+i] = sqrt(100.0/(R[1]*((*pnts)[i][1]) + R[2]*((*pnts)[i][2]) + R[3]*((*pnts)[i][3]) ));

      PHOTO_FLOAT rad = (R[ti+i]*R[ti+i]*((*pnts)[i][1]) - R[4])*(R[ti+i]*R[ti]*((*pnts)[i][1]) - R[4] )
                 +(R[ti+i]*R[ti+i]*((*pnts)[i][2]) - R[5])*(R[ti+i]*R[ti]*((*pnts)[i][2]) - R[5] )
                 +(R[ti+i]*R[ti+i]*((*pnts)[i][3]) - R[6])*(R[ti+i]*R[ti]*((*pnts)[i][3]) - R[6] );
 
     // printf(" radius[%d] = %e\n",i,sqrt(rad) );


    }


    for( i = 0; i < np; i++)
    {
      L[e] = 100.0 - R[ti]*R[ti]*(R[1]*((*pnts)[i][1])
                                + R[2]*((*pnts)[i][2]) 
                                + R[3]*((*pnts)[i][3]) );

      (*A)[e][1] = R[ti]*R[ti]*((*pnts)[i][1]);
      (*A)[e][2] = R[ti]*R[ti]*((*pnts)[i][2]);
      (*A)[e][3] = R[ti]*R[ti]*((*pnts)[i][3]);
    
      (*A)[e][ti] = 2.0*R[ti]*(R[1]*((*pnts)[i][1]) + R[2]*((*pnts)[i][2]) + R[3]*((*pnts)[i][3]) );

      e++;

      int tk = ci + 3;
      for(int k = 0; k < np; k++)
      {
        PHOTO_FLOAT li,lk;

        if(k != i)
        {
          li = (R[ti]*R[ti]*((*pnts)[i][1]) - R[4])*(R[ti]*R[ti]*((*pnts)[i][1]) - R[4] )
                +(R[ti]*R[ti]*((*pnts)[i][2]) - R[5])*(R[ti]*R[ti]*((*pnts)[i][2]) - R[5] )
                +(R[ti]*R[ti]*((*pnts)[i][3]) - R[6])*(R[ti]*R[ti]*((*pnts)[i][3]) - R[6] );

           lk = (R[tk]*R[tk]*((*pnts)[k][1]) - R[4])*(R[tk]*R[tk]*((*pnts)[k][1]) - R[4] )
                +(R[tk]*R[tk]*((*pnts)[k][2]) - R[5])*(R[tk]*R[tk]*((*pnts)[k][2]) - R[5] )
                +(R[tk]*R[tk]*((*pnts)[k][3]) - R[6])*(R[tk]*R[tk]*((*pnts)[k][3]) - R[6] );
          
          li = sqrt(li);
          lk = sqrt(lk);
 
          L[e] = li - lk;

          li = 1.0/(2.0*li);
          lk = 1.0/(2.0*lk);

          (*A)[e][ti] =li*(
                           -4.0*(R[ti]*R[ti]*((*pnts)[i][1]) - R[4])*R[ti]*((*pnts)[i][1])
                           -4.0*(R[ti]*R[ti]*((*pnts)[i][2]) - R[5])*R[ti]*((*pnts)[i][2])
                           -4.0*(R[ti]*R[ti]*((*pnts)[i][3]) - R[6])*R[ti]*((*pnts)[i][3])
                          );

          

          (*A)[e][tk] = lk*(
                            2.0*(R[tk]*R[tk]*((*pnts)[k][1]) - R[4])*R[tk]*((*pnts)[k][1])
                           +2.0*(R[tk]*R[tk]*((*pnts)[k][2]) - R[5])*R[tk]*((*pnts)[k][2])
                           +2.0*(R[tk]*R[tk]*((*pnts)[k][3]) - R[6])*R[tk]*((*pnts)[k][3])
                          );

          (*A)[e][4] =  li*(2.0*(R[ti]*R[ti]*((*pnts)[i][1]) - R[4]))
                       -lk*(2.0*(R[tk]*R[tk]*((*pnts)[k][1]) - R[4]));

          (*A)[e][5] =  li*(2.0*(R[ti]*R[ti]*((*pnts)[i][2]) - R[5]))
                       -lk*(2.0*(R[tk]*R[tk]*((*pnts)[k][2]) - R[5]));

          (*A)[e][6] =  li*(2.0*(R[ti]*R[ti]*((*pnts)[i][3]) - R[6]))
                       -lk*(2.0*(R[tk]*R[tk]*((*pnts)[k][3]) - R[6]));
 
          e++;
        }
   
        tk++;
      }

      ti++;

    }

/***
    for(int k = 0; k<np; k++)
    {
      PHOTO_FLOAT xk = R[1]*((*pnts)[k][1]) + R[2]*((*pnts)[k][2]) + R[3]*((*pnts)[k][3]) - R[11];
      PHOTO_FLOAT yk = R[4]*((*pnts)[k][1]) + R[5]*((*pnts)[k][2]) + R[6]*((*pnts)[k][3]);
      PHOTO_FLOAT rk = sqrt(xk*xk + yk*yk);

printf("radius %d = %e\n",k,rk);
    }
***/
//   printf("vector R");
//    print_vector(R,1,nu);

//printf("vector L");
//print_vector(L,1,ne);

PHOTO_FLOAT avgl = 0.0;
for(i=1;i<=ne;i++)avgl += L[i]*L[i];
avgl = sqrt(avgl/ne);

     VECTOR D = lsq(A,L,nu,ne);

   PHOTO_FLOAT wf = 1.0;
   if(loop > 100)wf = 0.75;
   if(loop > 150)wf = 0.5;
   if(loop > 175)wf = 0.25;

   for(i=1;i<= nu;i++)R[i] += wf*D[i];

PHOTO_FLOAT avgd = 0.0;
for(i=1;i<=nu;i++)avgd += D[i]*D[i];
avgd=sqrt(avgd/nu);

printf("%d  avgl = %e  avgd = %e\n",loop,avgl,avgd);

//printf("vector D");
//print_vector(D,1,nu);

// printf(" loop - %d\n", loop);
    free_vector(D,1,nu);
 

  }// end loop for iteration


 free_matrix(A,1,ne,1,nu);
/****

  VECTOR c = cross_vector(R, &(R[3]) );
  VECTOR cl  = unit_vector(3,c);
  free_vector(c,1,3);

printf("z vector + %e,%e,%e\n",cl[1],cl[2],cl[3]);


  for(int k=0;k<np;k++)
  {
    PHOTO_FLOAT x = R[1]*((*pnts)[k][1]) + R[2]*((*pnts)[k][2]) + R[3]*((*pnts)[k][3]) - R[10];
    PHOTO_FLOAT y = R[4]*((*pnts)[k][1]) + R[5]*((*pnts)[k][2]) + R[6]*((*pnts)[k][3]);
    PHOTO_FLOAT z = R[7]*((*pnts)[k][1]) + R[8]*((*pnts)[k][2]) + R[9]*((*pnts)[k][3]);

    RAS += sqrt(x*x + y*y);
  }

  RAS =RAS/np;

//printf(" RAS = %e\n",RAS);

*****/

  VECTOR RR = vector(1,7);

  // center
  RR[1] = R[4];
  RR[2] = R[5];
  RR[3] = R[6];

  //plane
  RR[4] = R[7];
  RR[5] = R[8];
  RR[6] = R[8];

//  RR[7] = RAS;

  free_vector(R,1,ne);
//  free_vector(cl,1,3);

  return(RR);
}









int make_cyl_by_3dpnts(int np,PNTLST pnts,PNTLST sep, PNTLST setm , struct Cylinder &cyl,
                       struct _transform &trans)
//int make_circle_by_3dpnts(int np,PNTLST pnts,double & radius,struct _transform & trans )
{
   if(setjmp(env) != 0)
  {
   return(1);
  }


  for(int i=0;i< 2;i++)
  {
     VECTOR inv = matinv((*setm)[i]); 
     copy_vector((*setm)[i],1,12,inv,1,12);

      rotpnt(inv, (*sep)[i] );

     free_vector(inv,1,12);
  }

  VECTOR R = ccc(np,pnts);


  // get end points of cylinder

 
    PNTLST pl;
    VECTOR v1;
    for( i = 1; i <= 2; i++ )
    {
        //PNTLST intersect_line_cyl(PHOTO_FLOAT r, VECTOR c_org, VECTOR c_dir, VECTOR l_org, VECTOR l_dir)
	pl = intersect_line_cyl( R[7], R, &R[3], &((*setm)[i-1][9]), (*sep)[i-1] );

//VECTOR prj_pnt_onto_line( VECTOR pnt, VECTOR org, VECTOR dir)
	v1 = prj_pnt_onto_line( (*pl)[1], R, &R[3] );
	
	copy_vector( (*sep)[i-1], 1, 3, v1, 1, 3 );
	
	free_matrix( pl, 1, 2, 1, 3 );
	free_vector( v1, 1, 3 );
    }
 
   make_cyl_by_endpnts(R[7],(*sep)[1],(*sep)[0],cyl,trans);

 free_vector(R,1,7);


  return(0);
}


void get_plane_of_pnts(int np, PNTLST pnts, VECTOR org, VECTOR p)
{
  int nu = 3;
  int ne = np;

printf("enter get plane of points\n");
print_matrix(pnts,0,np-1,1,3);
  PHOTO_FLOAT chisq;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);
  VECTOR sig = vector(1,ne);
  VECTOR w = vector(1,nu);

  PNTLST   u=matrix(1,ne,1,nu);
  PNTLST   v=matrix(1,nu,1,nu);

  for(int i = 0;i<np;i++)
  {
    (*A)[i+1][1] = (*pnts)[i][1];
    (*A)[i+1][2] = (*pnts)[i][2];
    (*A)[i+1][3] = (*pnts)[i][3];
    L[i+1] = 1.0;
    sig[i+1]=1.0;
  }


  svdfit(A,L,sig,ne,R,nu,u,v,w,&chisq,NULL);

printf("R vector");
print_vector(R,1,nu);

  PHOTO_FLOAT a12 = 1.0/sqrt(R[1]*R[1]+R[2]*R[2]+R[3]*R[3]);

  p[1]=a12*R[1];
  p[2]=a12*R[2];
  p[3]=a12*R[3];

  org[1]=a12*p[1];
  org[2]=a12*p[2];
  org[3]=a12*p[3];

printf("vector p");
print_vector(p,1,3);
printf("vector org");
print_vector(org,1,3);

  free_vector(L,1,ne);
  free_vector(R,1,nu);
  free_vector(sig,1,ne);
  free_vector(w,1,nu);

  free_matrix(A,1,ne,1,nu);
  free_matrix(u,1,ne,1,nu);
  free_matrix(v,1,nu,1,nu);

printf("returning from get plane of points\n");
  return;
}



int make_circle_by_3dpnts(int np,PNTLST pnts,double & radius,struct _transform & trans )
{

  printf("enter make circle by 3d points \n");

  VECTOR pl    = vector(1,3);
  VECTOR zaxis = vector(1,3);

  get_plane_of_pnts(np,pnts,pl,zaxis);

  VECTOR xaxis = vector(1,3);
  xaxis[1] = (*pnts)[0][1]-pl[1];
  xaxis[2] = (*pnts)[0][2]-pl[2];
  xaxis[3] = (*pnts)[0][3]-pl[3];
 
  VECTOR yaxis = cross_vector(zaxis,xaxis);
  PHOTO_FLOAT lng = vector_length(3,yaxis);
  multvector(3,yaxis,1.0/lng);

  free_vector(xaxis,1,3);

  xaxis = cross_vector(yaxis,zaxis);
  lng = vector_length(3,xaxis);
  multvector(3,xaxis,1.0/lng);
  
// printf("xaxis");
// print_vector(xaxis,1,3);
// printf("yaxis");
// print_vector(yaxis,1,3);
// printf("zaxis");
// print_vector(zaxis,1,3);

  PNTLST pts = matrix(1,np,1,2);

  for( int i = 0;i<np;i++)
  {
    PHOTO_FLOAT x = (*pnts)[i][1] - pl[1]; 
    PHOTO_FLOAT y = (*pnts)[i][2] - pl[2]; 
    PHOTO_FLOAT z = (*pnts)[i][3] - pl[3]; 

    (*pts)[i+1][1] = x*xaxis[1] + y*xaxis[2] + z*xaxis[3];
    (*pts)[i+1][2] = x*yaxis[1] + y*yaxis[2] + z*yaxis[3];
    PHOTO_FLOAT tempz = x*zaxis[1] + y*zaxis[2] + z*zaxis[3];
//printf(" 2d points %e,%e,%e\n",(*pts)[i+1][1],(*pts)[i+1][2],tempz);
  }

  
  int nu = 3;
  int ne = np;

  PHOTO_FLOAT chisq;

  VECTOR L = vector(1,ne);
  PNTLST A = matrix(1,ne,1,nu);
  VECTOR R = vector(1,nu);
  VECTOR a = vector(1,nu);
  VECTOR sig = vector(1,ne);
  VECTOR w = vector(1,nu);

  PNTLST   u=matrix(1,ne,1,nu);
  PNTLST   v=matrix(1,nu,1,nu);

  
  R[1] = R[2] = R[3] = 0.0;
  for(i=1;i<=np;i++)          // get centroid of pnts for initial center;
  {
     R[2] += (*pts)[i][1]/np;
     R[3] += (*pts)[i][2]/np;
  }
  
  for(i=1;i<=np;i++)          // average distance from center to pnts for initial radius
  {
     PHOTO_FLOAT x = (*pts)[i][1] - R[2];
     PHOTO_FLOAT y = (*pts)[i][2] - R[3];
     R[1] += (sqrt( x*x + y*y))/np;
  }


  for(int k=1;k<=10;k++)
  {
    for(i=1;i<=np;i++)
    {

      PHOTO_FLOAT x = (*pts)[i][1] - R[2]; 
      PHOTO_FLOAT y = (*pts)[i][2] - R[3]; 
      PHOTO_FLOAT lng = sqrt(x*x + y*y);
      sig[i] = 1.0;

      L[i] = -R[1] + lng;

      (*A)[i][1] = 1.0;
      (*A)[i][2] = x/lng;
      (*A)[i][3] = y/lng;

    }

//printf("vector R");
//print_vector(R,1,3); 
//printf("vector L");
//print_vector(L,1,ne); 
    svdfit(A,L,sig,ne,a,nu,u,v,w,&chisq,NULL);

    for(i=1;i<=3;i++)R[i]+=a[i];
printf("%d chisq = %e\n",k,chisq/np);
  }

//printf("vector R");
//print_vector(R,1,3);



  

  trans.pozition.pos_x = R[2]*xaxis[1] + R[3]*yaxis[1] + pl[1];
  trans.pozition.pos_y = R[2]*xaxis[2] + R[3]*yaxis[2] + pl[2];
  trans.pozition.pos_z = R[2]*xaxis[3] + R[3]*yaxis[3] + pl[3];

printf("origin = %e,%e,%e\n",trans.pozition.pos_x,trans.pozition.pos_y,trans.pozition.pos_z);

  trans.rotation.r[0]= yaxis[1];
  trans.rotation.r[1]= yaxis[2];
  trans.rotation.r[2]= yaxis[3];

  trans.rotation.r[3]= 0.0;

  trans.rotation.r[4]= zaxis[1];
  trans.rotation.r[5]= zaxis[2];
  trans.rotation.r[6]= zaxis[3];

  trans.rotation.r[7]= 0.0;


  trans.rotation.r[8]= xaxis[1];
  trans.rotation.r[9]= xaxis[2];
  trans.rotation.r[10]= xaxis[3];

  trans.rotation.r[11]= 0.0;


  trans.rotation.r[12]= 0.0;
  trans.rotation.r[13]= 0.0;
  trans.rotation.r[14]= 0.0;

  trans.rotation.r[15]= 1.0;



  //for(i=0;i<16;i++)printf("matrix[%d] = %e\n", i,trans.rotation.r[i]);
  free_vector(xaxis,1,3);
  free_vector(yaxis,1,3);
  free_vector(zaxis,1,3);


  trans.scale.sca_x = 1.0;
  trans.scale.sca_y = 1.0;
  trans.scale.sca_z = 1.0;

  trans.orientation.ori_x = 0.0;
  trans.orientation.ori_y = 0.0;
  trans.orientation.ori_z = 1.0;
  trans.orientation.ori_beta = 0.0;
  

  trans.center.cen_x = 0.0;
  trans.center.cen_y = 0.0;
  trans.center.cen_z = 0.0;


  radius = R[1];



  free_vector(L,1,ne);
  free_vector(R,1,nu);
  free_vector(a,1,nu);
  free_vector(sig,1,ne);
  free_vector(w,1,nu);
  free_matrix(A,1,ne,1,nu);

  free_matrix(u,1,ne,1,nu);
  free_matrix(v,1,nu,1,nu);
  free_vector(pl,1,3);
//  free_vector(xaxis,1,3);
//  free_vector(yaxis,1,3);
//  free_vector(zaxis,1,3);


   return(0);
}
  







