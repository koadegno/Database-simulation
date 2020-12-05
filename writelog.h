
#ifndef _WRITELOG_H_
#define _WRITELOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "db.h"
#include "student.h"


# define TIME_SIZE 40


/**
 * represente the Log path for a query
 */
typedef struct 
{
    char file_path[64]; /** path of the file */
    char query[64];     /** the query runung */
} LogPath;


long time_to_string(){
    
    struct timespec tms;
    clock_gettime(CLOCK_REALTIME,&tms);
    long nanos = tms.tv_nsec + 1e9 * tms.tv_sec;
    return nanos;
    
}

/**
 * create a log file and path for a query
 * param : 
 *        - index : int represente query number 
 *                  0 = select, 1 = insert, 2 = delete, 3 = update
 */
void InitLogPath(LogPath* file_struct,char* commande,int index){

    char file_format[256];
    char queryname[4][10] = {"select","insert","delete","update"};
    sprintf(file_format,"logs/%ld-%s.txt",time_to_string(),queryname[index]);
    strcpy(file_struct->file_path,file_format);
    strcpy(file_struct->query,commande);
    
    

}

void WriteLog(LogPath* file_struct,database_t* res,double mytime){

    char buff[128];
    FILE *ptr_file;
    ptr_file = fopen(file_struct->file_path,"w");
    fprintf(ptr_file,"Query \"%s\" completed in %fms with %ld: \n",file_struct->query,mytime,res->lsize);

    for(size_t i = 0; i < res->lsize; i++){
        student_to_str(buff,&(res->data[i]));
        fprintf(ptr_file,"%s\n",buff);
        

    }

    fclose(ptr_file);

}

#endif