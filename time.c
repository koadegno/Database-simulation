# include <stdio.h>
# include <stdlib.h>
# include <time.h>

# define TIME_SIZE 40


void time_to_string(char* buff){
    
    struct timespec tms;
    clock_gettime(CLOCK_REALTIME,&tms);
    long nanos = tms.tv_nsec + 1e9 * tms.tv_sec;
    sprintf(buff,"%ld",nanos);
}


int main(){

    char nanos[TIME_SIZE];
    time_to_string(nanos);
    
    printf("Nanosecondes: %s\n",nanos);
    

}