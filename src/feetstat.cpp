// feetstat.cpp
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "DbaseToTildeDelim.h"

#ifndef FALSE
#define FALSE 0
#define TRUE (!FALSE)
#endif

/* States */
typedef enum {
	haveNothing     = 0,
    haveSomething   = 1, 
    gotFeet  	    = 2, 
    haveSomething2  = 3, 
    wholeInches     = 4, 
    numDone         = 5,
	num				= 6,
    denom	        = 7,  
	endOfLine       = 8,
    invalidState    = 0xffff
} State;

#define NUM_STATES  10

/* Inputs */
#define QUOTE	'"'
#define TICK	'\''
#define SPACE	' '
#define SLASH	'/'
#define DASH 	'-'
#define PLUS	'+'
#define DECIMAL	'.'

#define NCHRS 256
#define NUMERIC	(NCHRS + 1)
#define ALPHA	(NCHRS + 2)
#define ANY	(NCHRS + 3)

typedef struct {
    State   ft_state;		        /* current state */
    short   ft_char;		        /* input character */
    State   ft_next;		        /* next state */
    void    (*ft_action)( char );	/* action to take */
} StateTransTable;

typedef State ** StateExpandedTable;

int StateMachineInit(
	StateTransTable transTable[],
	int numTransTableEntries,
	State **expandedTable,
	int numStates,
	int numInputs );

StateExpandedTable createExpandedTable( int numStates, int numInputs );
void destroyExpandedTable( StateExpandedTable theTable, int numStates );
State getNextState( StateTransTable initTable[], StateExpandedTable expandedTable, State currState, int input );

char tempField[2][1024];
char field[4096];
int numFields = 0;

char temp[128];
int sign = 1;
double feet, whole_inches, numerator, denominator;
/*
int main(int argc, char *argv[] )
{
        FILE *in;
        char buffer[1024];

        if( argc != 2 )
                exit(0);
        in = fopen(argv[1], "r" );
        if( in == NULL )
                exit(0);
        while( fgets( buffer, sizeof(buffer), in ) != NULL )
        {
                if( buffer[strlen(buffer)-1] == '\n' )
                        buffer[strlen(buffer)-1] = '\0';
                printf( "%s\n", DbaseToTildeDelim(buffer) );
        }
        fclose(in);
        return 0;
}
*/
char *charcat( char *str, char c )
{
    char *ptr = str;
    
    if( ptr == NULL )
	return( ptr );

    while( *ptr != '\0' )
	ptr++;
    *ptr = c;
    *(ptr+1) = '\0';
    
    return( str );
}

void appendField( char *newField )
{
//    if( strlen( field ) > 0 )
    if( numFields > 0 )	
	charcat( field, '~' );
    strcat( field, newField );
    
    numFields++;
    
//    fprintf( stderr, "field[%d] = |%s|\n", numFields, newField );
}


#define FUNC "anyHNtoIS"
void anyHStoIS( char c )
{
	feet = whole_inches = numerator = 0;
	denominator = 1;
	sign = 1;
 	
//    fprintf( stderr, "%s - Entering invalid state.\n", FUNC );
}
#undef FUNC

void char_temp( char c )
{
	charcat( temp, c );
}

void temp_feet( char c )
{
//	char_cat( temp, c );
	feet = atof(temp);
	memset(temp,0,sizeof(temp));
}

void temp_win( char c )
{
	whole_inches = atof(temp);
	memset(temp,0,sizeof(temp));
}

void temp_num( char c )
{
	numerator = atof(temp);
	memset(temp,0,sizeof(temp));
}

void temp_denom( char c )
{
	denominator = atof(temp);
	memset(temp,0,sizeof(temp));
}

void save_sign( char c )
{
	if( c == '-' )
		sign = -1;
	else
		sign = 1;
}

void no_temp( char c )
{
}

StateTransTable initTable[] = {
    { haveNothing,	NUMERIC,    haveSomething,  char_temp }, 
{ haveNothing,	DECIMAL,	haveSomething,	char_temp },
	{ haveNothing,	DASH,		haveNothing,	save_sign },
	{ haveNothing,	PLUS,		haveNothing,	save_sign },
    { haveNothing,	ANY,	    invalidState,   anyHStoIS }, 
	
	{ haveSomething,	NUMERIC,	haveSomething,	char_temp },
{ haveSomething,	DECIMAL,	haveSomething,	char_temp },
    { haveSomething,	TICK,	    gotFeet,		temp_feet }, 
    { haveSomething,	QUOTE,	    endOfLine,	    temp_win }, 	 
    { haveSomething,	SPACE,	    wholeInches,   	temp_win },
    { haveSomething,	SLASH,	    numDone,   		temp_num },
	{ haveSomething,	NULL,		endOfLine,		temp_win },
    { haveSomething,	ANY,	    invalidState,	anyHStoIS },

    { gotFeet,		SPACE,	    gotFeet,  		char_temp }, 
    { gotFeet,		NUMERIC,	haveSomething2, char_temp }, 
{ gotFeet,		DECIMAL,	haveSomething2,	char_temp },
    { gotFeet,		NULL,	    endOfLine,    	NULL }, 
    { gotFeet,		ANY,	    invalidState,  	anyHStoIS }, 

    { haveSomething2,	NUMERIC,	haveSomething2,	char_temp },
{ haveSomething2,	DECIMAL,	haveSomething2,	char_temp },
    { haveSomething2,	QUOTE,	    endOfLine,  	temp_win },  
    { haveSomething2,	SPACE,	    wholeInches,	temp_win }, 
    { haveSomething2,	SLASH,	    numDone,		temp_num }, 
    { haveSomething2,	NULL,	    endOfLine,		temp_win }, 
    { haveSomething2,	ANY,	    invalidState,	anyHStoIS }, 

    { wholeInches,		SPACE,	    wholeInches,    NULL }, 
    { wholeInches,		NUMERIC,	num,		    char_temp }, 
{ wholeInches,		DECIMAL,	num,			char_temp },
    { wholeInches,		NULL,	    endOfLine,    	NULL }, 
    { wholeInches,		ANY,		invalidState,   anyHStoIS }, 

    { num,			NUMERIC,    num,	   		char_temp }, 
{ num,	DECIMAL,	num,	char_temp },
    { num,			SLASH,	    numDone,		temp_num }, 
    { num,			ANY,    	invalidState,  	anyHStoIS }, 
    
    { numDone,		NUMERIC,	    denom,		  char_temp }, 
{ numDone,		DECIMAL,		denom,		  char_temp },
    { numDone,		ANY,    		invalidState,  	anyHStoIS }, 
    
    { denom,		NUMERIC,	    denom,			char_temp }, 
{ denom,		DECIMAL,		denom,			char_temp },
    { denom,		QUOTE,		    endOfLine,		temp_denom }, 
    { denom,		NULL,		    endOfLine,		temp_denom }, 
    { denom,		ANY,    		invalidState,  	anyHStoIS }, 

// Don't enter state transition info for invalidState, it is automatically eaten.    
//	{ invalidState,	ANY,		invalidState,		NULL },

    { endOfLine,	ANY,	    endOfLine,	    NULL }
};

double getInches( char *inputStr )
{
    int i, j;
    State currState;
    char *currPos;
    int currLineNo = 0;
    
	feet = whole_inches = numerator = 0.0;
	denominator = 1.0;
	sign = 1;
	memset(temp,0,sizeof(temp));

    StateExpandedTable expandedTable;

    expandedTable = createExpandedTable( NUM_STATES, NCHRS );

    StateMachineInit( initTable, (sizeof(initTable)/sizeof(initTable[0])), expandedTable, NUM_STATES, NCHRS );

    memset( field, 0, sizeof(field) );
    numFields = 0;
    currLineNo++;
    
    currState = haveNothing;
    currPos = inputStr;
    int moreStuffToDo = TRUE;
    while( moreStuffToDo )
    {
		State lastState = currState;
		void (*actionFunc)( char );
	
		currState = getNextState( initTable, expandedTable, currState, *currPos );

		if( lastState != invalidState )
		{
			actionFunc = (initTable[expandedTable[lastState][*currPos]]).ft_action;
			if( actionFunc != NULL )
			    actionFunc( *currPos );
		}
	//	    printf( "currState = %d, nextState = %d, currChar = %c, tempField[0] = |%s|, tempField[1] = |%s|\n",
	//		    lastState, currState, *currPos, tempField[0], tempField[1] );

		if( *currPos == '\0' )
		    moreStuffToDo = FALSE;
	    
		currPos++;
    }

    destroyExpandedTable( expandedTable, NUM_STATES );

//    strcpy( inputStr, field );
//    return( inputStr );
	return (double)sign * ((feet * 12 ) + whole_inches + (numerator/denominator));
}

StateExpandedTable createExpandedTable( int numStates, int numInputs )
{
    StateExpandedTable theTable;
    
    theTable = (StateExpandedTable)malloc( numStates*sizeof(State *) );
    for( int i = 0; i < numStates; i++ )
		theTable[i] = (State *)malloc( numInputs*sizeof(State) );

    return( theTable );
}

void destroyExpandedTable( StateExpandedTable theTable, int numStates )
{
    for( int i = 0; i < numStates; i++ )
		free( theTable[i] );
    free( theTable );
}

int StateMachineInit( StateTransTable transTable[], int numTransTableEntries, State **expandedTable, int numStates, int numInputs )
{
    int currState, currInput;
    int currTransTableEntry;

    /* Initialize expandedTable to contain all invalid transition indices */
    for( currInput = 0; currInput < numInputs; currInput++ )
    {
		for( currState = 0; currState < numStates; currState++ )
		{
		    expandedTable[currState][currInput] = invalidState;
		}
    }

    for( currTransTableEntry = 0; currTransTableEntry < numTransTableEntries; currTransTableEntry++ )
    {
		StateTransTable *ptrTableEntry = &transTable[currTransTableEntry];
		switch( ptrTableEntry->ft_char )
		{
		    case ANY:
				for( currInput = 0; currInput < numInputs; currInput++ )
				{
				    if( expandedTable[ptrTableEntry->ft_state][currInput] == invalidState )
						expandedTable[ptrTableEntry->ft_state][currInput] = (State)currTransTableEntry;
				}
				break;
	    
		    case NUMERIC:
				for( currInput = '0'; currInput <= '9'; currInput++ )
				{
				    if( expandedTable[ptrTableEntry->ft_state][currInput] == invalidState )
						expandedTable[ptrTableEntry->ft_state][currInput] = (State)currTransTableEntry;
				}
				break;
		
		    case ALPHA:
				for( currInput = 'A'; currInput <= 'Z'; currInput++ )
				{
				    if( expandedTable[ptrTableEntry->ft_state][currInput] == invalidState )
						expandedTable[ptrTableEntry->ft_state][currInput] = (State)currTransTableEntry;
				}

				for( currInput = 'a'; currInput <= 'z'; currInput++ )
				{
				    if( expandedTable[ptrTableEntry->ft_state][currInput] == invalidState )
						expandedTable[ptrTableEntry->ft_state][currInput] = (State)currTransTableEntry;
				}
				break;
	    
		    default:
				expandedTable[ptrTableEntry->ft_state][ptrTableEntry->ft_char] = (State)currTransTableEntry;
				break;
		}
    }

    for( currInput = 0; currInput < numInputs; currInput++ )
    {
	for( currState = 0; currState < numStates; currState++ )
	{
	    if( expandedTable[currState][currInput] == invalidState )
		expandedTable[currState][currInput] = (State)currState;
	}
    }

    return( 0 );
}

State getNextState( StateTransTable initTable[], StateExpandedTable expandedTable, State currState, int input )
{
	if( currState == invalidState )
		return invalidState;
    int currTransEntry = expandedTable[currState][input];
    return( initTable[currTransEntry].ft_next );
}
