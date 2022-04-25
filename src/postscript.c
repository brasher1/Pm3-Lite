#include <stdio.h>
#include "dvi2ps.h"
#include "commands.h"
#include "postscript.h"
#include "pxlfile.h"


#ifdef BUDGET
#define fontcost(p) 2372
#define charcost(ptr) (((ptr->width + 7) >> 3) * ptr->height+70)
#define BIGCHAR(ptr) (charcost(ptr)>budget/100)
int budget = BUDGET;
#endif

char    *digit = "0123456789ABCDEF";
int   h;			/* current horizontal position              */
int   hh;			/* idem in pixels		              */
int   hhh = UNDEFPOS;           /* current h on device */
int   v;			/* current vertical position                */
int   vv;			/* idem in pixels		              */
int   vvv = UNDEFPOS;           /* current v on device */

int     ChkOpt();               /* Check PostScript option for validity */
void    DoSpecial();
void    EmitBitmap();
void    EmitChar();
char   *GetKeyStr();
int     GetKeyVal();
void	Init_Page();
void	MakePSFont();
void	MoveDown();
void	MoveOver();
void	MoveRight();
int	PixRound();
#ifdef BUILTIN
char    *PSDef();
#endif
void    PutInt();
void    PutOct();
int	RuleRound();
void	SetChar();
void	SetFntNum();
void    SetPosn();
void	SetRule();
void    SetString();

/**********************************************************************/
/*************************  External Procedures  **********************/
/**********************************************************************/

float	ActualFactor();
int     IsSame();
void    CopyFile();
void	Fatal();
int     NoSignExtend();         /* see cautionary note in code, re arithmetic vs logical shifts */
void	Warning();
extern FILE *outfp;
extern struct font_entry *fontptr, *prevfont, *hfontptr;
#ifdef STATS
extern int Stats;
extern int   Stats;              /* are we reporting stats ?                 */
extern int   Snbpxl;             /* # of bytes of pixel data                 */
extern int   Sonbpx;            /* "optimal" number of bytes of pixel data  */
extern int   Sndc;               /* # of different characters typeset        */
extern int   Stnc;               /* total # of chars typeset                 */
extern int   Snbpx0, Sndc0, Stnc0;   /* used for printing incremental changes per dvi page */
#endif
extern int mag, num, den;
extern FILE* dvifp;

/*-->ChkOpt*/   /* check a user supplied option for validity */
/*********************************************************************/
/****************************** ChkOpt *******************************/
/*********************************************************************/

#define ISOPT(s) if( EQ(str,s) ) return( TRUE )

int
ChkOpt( str )
char    *str;
{
/*        lcase(str);  doesn't work */

        ISOPT("note");          /* its a shame to build this into the program */
        ISOPT("letter");
	ISOPT("a4");
        ISOPT("legal");
        ISOPT("landscape");
        ISOPT("manualfeed");
        return( FALSE );
}


/*-->DoSpecial*/
/*********************************************************************/
/*****************************  DoSpecial  ***************************/
/*********************************************************************/

typedef enum {None, String, Integer, Number, Dimension} ValTyp;

typedef struct {
        char    *Key;           /* the keyword string */
        char    *Val;           /* the value string */
        ValTyp  vt;             /* the value type */
        union {                 /* the decoded value */
            int  i;
            float n;
            } v;
        } KeyWord;

typedef struct {
        char    *Entry;
        ValTyp  Type;
        } KeyDesc;

#define PSFILE 0
#define PSTEXT 7
KeyDesc KeyTab[] = {{"psfile", String},
                    {"hsize", Dimension},
                    {"vsize", Dimension},
                    {"hoffset", Dimension},
                    {"voffset", Dimension},
                    {"hscale", Number},
                    {"vscale", Number},
		    {"pstext", String}};

#define NKEYS (sizeof(KeyTab)/sizeof(KeyTab[0]))

void
DoSpecial( str, n )          /* interpret a \special command, made up of keyword=value pairs */
char    *str;
int n;
{ 
        char spbuf[STRSIZE]; 
        char *sf = NULL;
        KeyWord k;
        int i;

        str[n] = '\0';
        spbuf[0] = '\0';

        SetPosn(hh, vv);
        EMITS("@beginspecial\n");

        while( (str=GetKeyStr(str,&k)) != NULL ) {      /* get all keyword-value pairs */
                              /* for compatibility, single words are taken as file names */
                if( k.vt == None && access(k.Key,0) == 0) {
                        if( sf ) Warning("  More than one \\special file name given. %s ignored", sf );
                        strcpy(spbuf, k.Key);
                        sf = spbuf;
                        }
                else if( GetKeyVal( &k, KeyTab, NKEYS, &i ) && i != -1 ) {
                        if( i == PSFILE ) {
                                if( sf ) Warning("  More than one \\special file name given. %s ignored", sf );
                                strcpy(spbuf, k.Val);
                                sf = spbuf;
                                }
			else if(i == PSTEXT) {
				EMITS(k.Val);
				EMITC('\n');
				}
                        else            /* the keywords are simply output as PS procedure calls */
                                EMIT(outfp, "%f @%s\n", k.v.n, KeyTab[i].Entry);
                        }
                else Warning("  Invalid keyword or value in \\special - \"%s\" ignored", k.Key );
                }

        EMITS("@setspecial\n");

        if( sf )
                CopyFile( sf );
        else
                Warning("  No special file name provided.");

        EMITS("@endspecial\n");
}


/*-->EmitBitmap*/
/**********************************************************************/
/****************************  EmitBitmap  ****************************/
/**********************************************************************/

void
EmitBitmap(c, ce, command)              /* output a character */
int c;
char * command;
struct char_entry *ce;
{
		/* Output in PostScript coord system (y +ive up, x +ive right)
		   (0,0) of char bitmap at lower left.  Output scan lines
                           from bottom to top */
        int i;
        register int j;
        register unsigned char *sl;
        register int cc;
        int nbpl;

        EMITS("[<");
        cc = 2;
        nbpl = (ce->width + 7) >> 3;
        for(i = ce->height-1;  i >= 0;  i--) {
                sl = (unsigned char *)(ce->where.address.pixptr + i*nbpl);
                for(j = 0;  j < nbpl;  j++, sl++) {
                        if( cc > 100 ) {
                                EMITS("\n  ");   cc = 2; }
                        EMITH(*sl);
                        cc += 2;
                        }
                }
        EMIT(outfp,"> %d %d %d %d %d] %d %s\n", 
             ce->width, ce->height, ce->xOffset,
	     (((int)ce->height)-ce->yOffset)-1, ce->dx, c, command);

#ifdef STATS
        Snbpxl += nbpl*ce->height;
        fontptr->nbpxl += nbpl*ce->height;
        Sonbpx += (ce->width*ce->height + 7) >> 3;
        Sndc ++;
#endif
}

/*-->EmitChar*/
/**********************************************************************/
/****************************  EmitChar  ******************************/
/**********************************************************************/

void
EmitChar(c, ce)              /* output a character */
int c;
struct char_entry *ce;
{

#ifdef BUDGET
	/* don't load character permanently if it's too big */
	if (BIGCHAR(ce)) return;
#endif
	if( fontptr->ncdl == 0 ) {    /* open font dict before first char */
#ifdef BUDGET
	    if (fontcost(fontptr) >= budget) return;
	    budget -= fontcost(fontptr);
#endif
	    EMIT(outfp,"/%s @newfont\n", fontptr->psname );
	}
 
	if( fontptr != prevfont ) {   /* because this isn't done on pass 0 */
		EMIT(outfp,"%s @sf\n", fontptr->psname);
		prevfont = fontptr;
		}

	EmitBitmap(c, ce, "@dc");
        fontptr->ncdl ++;

	ce->where.isloaded = PERMANENT;
#ifdef BUDGET
	budget -= charcost(ce);
#endif

}


/*-->GetKeyStr*/
/**********************************************************************/
/*****************************  GetKeyStr  ****************************/
/**********************************************************************/

        /* extract first keyword-value pair from string (value part may be null)
         * return pointer to remainder of string
         * return NULL if none found
         */

char    KeyStr[STRSIZE];
char    ValStr[STRSIZE];

char *GetKeyStr( str, kw )
char    *str;
KeyWord *kw;
{
        char *s, *k, *v, t;

        if( !str ) return( NULL );

        for( s=str; *s == ' '; s++ ) ;                  /* skip over blanks */
        if( *s == '\0' ) return( NULL );

        for( k=KeyStr;                          /* extract keyword portion */
             *s != ' ' && *s != '\0' && *s != '='; 
             *k++ = *s++ ) ;
        *k = '\0';
        kw->Key = KeyStr;
        kw->Val = v = NULL;
        kw->vt = None;

        for( ; *s == ' '; s++ ) ;                       /* skip over blanks */
        if( *s != '=' )                         /* look for "=" */
                return( s );

        for( s++ ; *s == ' '; s++ ) ;                   /* skip over blanks */
        if( *s == '\'' || *s == '\"' )          /* get string delimiter */
                t = *s++;
        else
                t = ' ';
        for( v=ValStr;                          /* copy value portion up to delim */
             *s != t && *s != '\0';
             *v++ = *s++ ) ;
        if( t != ' ' && *s == t ) s++;
        *v = '\0';
        kw->Val = ValStr;
        kw->vt = String;

        return( s );
}


/*-->GetKeyVal*/
/**********************************************************************/
/*****************************  GetKeyVal  ****************************/
/**********************************************************************/

        /* get next keyword-value pair
         * decode value according to table entry
         */

int GetKeyVal( kw, tab, nt, tno)
KeyWord *kw; 
KeyDesc tab[];
int     nt;
int     *tno;
{
        int i;
        char c = '\0';

        *tno = -1;

        for(i=0; i<nt; i++)
                if( IsSame(kw->Key, tab[i].Entry) ) {
                        *tno = i;
                        switch( tab[i].Type ) {
                                case None: 
                                        if( kw->vt != None ) return( FALSE );
                                        break;
                                case String:
                                        if( kw->vt != String ) return( FALSE );
                                        break;
                                case Integer:
                                        if( kw->vt != String ) return( FALSE );
                                        if( sscanf(kw->Val,"%d%c", &(kw->v.i), &c) != 1
                                            || c != '\0' ) return( FALSE );
                                        break;
                                case Number:
                                case Dimension:
                                        if( kw->vt != String ) return( FALSE );
                                        if( sscanf(kw->Val,"%f%c", &(kw->v.n), &c) != 1
                                            || c != '\0' ) return( FALSE );
                                        break;
                                }
                        kw->vt = tab[i].Type;
                        return( TRUE );
                        }

        return( TRUE );
}

/*--Init_Page*/
/**********************************************************************/
/****************************  Init_Page  ******************************/
/**********************************************************************/

void
Init_Page()
{
	    h = v = 0;
            hhh = vvv = UNDEFPOS;
	    hh = vv = 0;
	    fontptr = NULL;
            prevfont = NULL;
}

/*-->MakePSFont*/
/**********************************************************************/
/****************************  MakePSFont  ******************************/
/**********************************************************************/

void
MakePSFont (fontptr)
struct font_entry *fontptr;
{
	register int i;

		    /**********************************************************
		      Font Size is ActualFactor(mag) * s/d * Normal Size
		      Normal Size is	d		(DVI UNITS)
		      * num/den	(10^-7 metres)
		      / 10^-5		(centimetres)
		      / 2.54		(inches)
		      * 300		DOTS
		     *********************************************************/

	EMIT(outfp, "/%s %f %s\n", 
	     fontptr->psname,
	     ActualFactor(mag) * fontptr->s * num/den/254000.0 * RESOLUTION,
	     PSDef(fontptr->n));
			 /* note: PSDef may output a file as side effect */
	for (i = FIRSTPXLCHAR; i <= LASTPXLCHAR; i++)
	  fontptr->ch[i].where.isloaded = PERMANENT;
	fontptr->ncdl++;
#ifdef BUDGET
	budget -= fontcost(fontptr);
#endif
}

/*-->MoveDown*/
/**********************************************************************/
/****************************  MoveDown  ******************************/
/**********************************************************************/

void
MoveDown(a)
int a;
{
    int vtemp;
    vtemp = PixRound (v += a, vconv);
    if (fontptr && abs(a)<5*fontptr->font_space) {
	vv += PixRound(a, vconv);
	if (vtemp>vv+max_drift) vv = vtemp-max_drift;
	else if (vtemp<vv-max_drift) vv = vtemp+max_drift;
    }
    else vv = vtemp;
}


/*-->MoveOver*/
/**********************************************************************/
/****************************  MoveOver  ******************************/
/**********************************************************************/

void
MoveOver(b)
int b;
{
    int htemp;
    htemp = PixRound (h += b, hconv);
    if (fontptr && b<fontptr->font_space && b>-4*fontptr->font_space) {
	hh += PixRound(b, hconv);
	if (htemp>hh+max_drift) hh = htemp-max_drift;
	else if (htemp<hh-max_drift) hh = htemp+max_drift;
    }
    else hh = htemp;
}

/*-->MoveRight*/
/**********************************************************************/
/****************************  MoveRight  ******************************/
/**********************************************************************/

void
MoveRight(b)
int b;
{
    int htemp;
    htemp = PixRound (h += b, hconv);
    if (htemp>hh+max_drift) hh = htemp-max_drift;
    else if (htemp<hh-max_drift) hh = htemp+max_drift;
}


/*-->PixRound*/
/**********************************************************************/
/*****************************  PixRound  *****************************/
/**********************************************************************/

int
PixRound(x, conv)	/* return rounded number of pixels */
register int x;		/* in DVI units     */
int conv;		/* conversion factor */
{
    return((int)((x + (conv >> 1)) / conv));
}

/*-->PutInt*/
/**********************************************************************/
/*****************************  PutInt  *******************************/
/**********************************************************************/

void
PutInt(n)               /* output an integer followed by a space */
register int n;
{
    char buf[10];
    register char *b;

    if( n == 0 )
        EMITC('0'); 
    else {
        if( n < 0 ) {
            EMITC('-');
            n = -n;
            }
    
        for(b=buf;  n>0;  ) {
            *b++ = digit[n%10];
            n /= 10;
            }
    
        for( ; b>buf; )
            EMITC(*--b);
        }

    EMITC(' ');
}


/*-->PutOct*/
/**********************************************************************/
/*****************************  PutOct  *******************************/
/**********************************************************************/

void
PutOct(n)               /* output an 3 digit octal number preceded by a "\" */
register int n;
{                  
    EMITC( '\\' ); 
    EMITC( digit[(n&0300)>>6] );
    EMITC( digit[(n&0070)>>3] ); 
    EMITC( digit[n&0007] );
}

/*-->RuleRound*/
/**********************************************************************/
/*****************************  RuleRound  ****************************/
/**********************************************************************/

int
RuleRound(x, conv)	/* return rounded number of pixels */
register int x;		/* in DVI units     */
int conv;		/* conversion factor */
{
    return((int)((x + conv - 1) / conv));
}


/*-->SetChar*/
/**********************************************************************/
/*****************************  SetChar  ******************************/
/**********************************************************************/

void
SetChar(c, command, PassNo)
int c, command, PassNo;
{
    register struct char_entry *ptr;  /* temporary char_entry pointer */
    int k;

    ptr = &(fontptr->ch[c]);
    if (ptr->where.isloaded==NOTLOADED) LoadAChar(c, ptr);
    if (PassNo==0) return;

    SetPosn(hh,vv);
    if (fontptr->font_file_id != NO_FILE) {     /* ignore missing fonts */
#ifdef BUDGET
	if (ptr->where.isloaded!=PERMANENT) {	/* and missing characters */
	    if (ptr->where.address.pixptr != NULL) {
		EMITS("save\n");
		EmitBitmap(c, ptr, "@pc restore");
	    }
	} else {
#endif
	    if( fontptr != prevfont ) {
		    EMIT(outfp,"%s @sf\n", fontptr->psname);
		    prevfont = fontptr;
		    }
	    EMITN(c); EMITS("c\n");
	    hhh += ptr->dx;
#ifdef BUDGET
	}
#endif
    }

    if (command <= SET4) {
        hh += ptr->dx;
	MoveRight(ptr->tfmw);
    }

#ifdef STATS
    Stnc ++;
    fontptr->ncts ++;
#endif
}

/*-->SetFntNum*/
/**********************************************************************/
/****************************  SetFntNum  *****************************/
/**********************************************************************/

void
SetFntNum(k, Emitting)
int k, Emitting;

/*  this routine is used to specify the font to be used in printing future
    characters */

{
    fontptr = hfontptr;
    while ((fontptr!=NULL) && (fontptr->k!=k))
	fontptr = fontptr->next;
    if (fontptr == NULL)
	Fatal("font %d undefined", k);
/*
    if (Emitting && (fontptr->font_file_id != NO_FILE) )
        EMIT(outfp,"%s @sf\n", fontptr->psname);
*/
}


void
SetPosn(x, y)           /* output a positioning command */
int x, y;
{
        int rx,ry;
#ifdef USERELPOS
        if (y == vvv) {  /* use relative movement if just moving horizontally */
            if ( x != hhh ) {
                EMITN(rx=x-hhh);
                EMITS("r ");
                }
            }
        else {
#endif
            EMITN(x);
            EMITN(y);
            EMITS("p ");
#ifdef USERELPOS
            }
#endif
	hhh = x;
	vvv = y;
}


/*-->SetRule*/
/**********************************************************************/
/*****************************  SetRule  ******************************/
/**********************************************************************/

void
SetRule(a, b, Set)
int a, b;
BOOLEAN Set;

{	    /*	 this routine will draw a rule */
    int q;	/* b, rounded to pixels */

    if( a > 0 && b > 0 ) {
        SetPosn(hh,vv);                   /* lower left corner */
        q = RuleRound(b,hconv);      	/* width */
        EMITN( q );       		/* width */
        EMITN(RuleRound(a,vconv));       /* height */
        EMITS("ru\n");
        }
    if (Set) { hh += q; MoveRight(b); }
}


/*-->SetString*/
/**********************************************************************/
/*****************************  SetString  ****************************/
/**********************************************************************/

void
SetString(firstch, PassNo)              /* read and set a consecutive string of chars */
int firstch, PassNo;
{
#define SLEN 256
    char string[SLEN];
    register char *sp;
    register int  c;
    register struct char_entry *ptr;
    int len, h1, hh1;
    int special;	/* 1 if we couldn't load the last character */

    /* read entire string of chars */

    special = 0;
    for(c = firstch, sp = string; c >= SETC_000 && c <= SETC_127 && sp < &string[SLEN]; ) {
      *sp++ = c;
      ptr = &(fontptr->ch[c]);
      if (ptr->where.isloaded==NOTLOADED ) LoadAChar(c, ptr);
      if (PassNo != 0 && ptr->where.isloaded!=PERMANENT)
	{ special = 1; break; }
      c = NoSignExtend(dvifp, 1);
    }
    if (!special) ungetc(c, dvifp);           /* backup one character */
    
    if( PassNo == 0 ) return;
    len = sp - string - special;        	/* NULL's are valid chars, */
    /* so cant use for string termination */
    if( len > 0 &&
       fontptr->font_file_id != NO_FILE ) {     /* ignore missing fonts */
	 sp = string;
	 if( fontptr != prevfont ) {
	   EMIT(outfp,"%s @sf\n", fontptr->psname);
	   prevfont = fontptr;
	 }
	 while (sp < string+len) {
	   if (sp == string+len-1) SetChar (*sp++, SET1, PassNo);
	   else {
	     SetPosn(hh, vv);
	     EMITC('(');
	     h1 = h ;
	     while (sp < string+len) {
	       ptr = &(fontptr->ch[*sp]);
	       h1 += ptr->tfmw;
	       hh += ptr->dx;
	       if( *sp < ' ' || *sp >= 0177 )
		 EMITO(*sp);
	       else if( *sp == '(' || *sp == ')' || *sp == '\\') {
		 EMITC('\\'); 
		 EMITC(*sp); 
	       }
	       else
		 EMITC(*sp);
	       ++sp;
	       hh1 = PixRound (h1, hconv) - hh;
	       if ( abs (hh1) > max_drift) break;
	     }
	     EMITS(") s\n");
	     hhh = hh;
	     MoveRight(h1-h);
	   }
	 }
       }	 
    if (special) SetChar (string[len], SET1, PassNo);

#ifdef STATS
    Stnc += len;
    fontptr->ncts += len;
#endif
}

