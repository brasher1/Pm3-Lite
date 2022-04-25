#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( int argc, char *argv[] )
{
    FILE *out;
    int year, month, day;
    struct tm tim;    
    struct Tim { time_t t1,t2,t3; } tim2;
    
    if( argc != 4 )
        return 0;

  	memset(&tim,0,sizeof(tim));

    tim.tm_year = atoi(argv[3]);
    tim.tm_mon = atoi(argv[1]) - 1;
    tim.tm_mday = atoi(argv[2]);
  
    time(&(tim2.t1));
    tim2.t3 = tim2.t1;
    tim2.t2 = mktime(&tim);
    out = fopen("datadump.bin","w");

 	printf("t1=%ld, t2=%ld, t3=%ld", tim2.t1, tim2.t2, tim2.t3);

    fwrite(&tim2,1,12,out);
    fclose(out);
}
