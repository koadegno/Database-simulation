#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdio.h>
#include "student.h"
#include "db.h"


int main(int argc, char const *argv[]) {

    const char* student_file;
    

    if(argc > 2) {printf("Trop de fichier !\n"); return 1;}; // no file

    if(argc==2) {
        student_file = argv[1]; printf("file : %s \n",student_file);
        database_t db_student;
        db_init(&db_student);
        printf("ok\n");
        db_load(&db_student,student_file);
        
    }
    else{
        printf("Pas de fichier \n");
    }

    /*  read file with "<" redirection symbol
    char buffer[BUFFERSIZE];
    fgets(buffer, BUFFERSIZE , stdin);
    printf("Read: %s", buffer);*/


    



    return 0;
}
