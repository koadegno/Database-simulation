#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include <string.h>
#include "parsing.c"
#include "db.h"
#define BUFFERSIZE 100
#define keepon 0
#define querie_running 1 

void error(char* err){
    printf("Querie Error %s\n",err);
}

void select_commande(){
    char field[100]; char value[100];
    char input[100];
    char fname[64]; char lname[64]; char section[64];
    unsigned id; struct tm* annif = (struct tm*)malloc(sizeof(struct tm)*1);
    char field_filter[100];char value_filter[100];
    char field_to_update[100];char update_value[100];
    while(keepon == 0){
        
        printf("> ");
        fgets(input,100,stdin);

        switch (input[0])
        {
        case 's':
            
            printf("%s",input);

            if (!parse_selectors(input,field,value)){
                error("select");break;
            }

            printf("\nselect \n");
            break;
        case 'i':
            
            printf("%s",input);
            if(!parse_insert(input,fname,lname,&id,section,annif)){
                error("insert");break;
            }
            printf("\ninsert \n");
            break;

        case 'd':
            printf("\ndelete \n");

            break;

        case 'u':
            printf("%s",input);
            if(!parse_update(input,field_filter,value_filter,field_to_update,update_value)){
                error("update");break;
            }
            printf("\nupdate \n");
            break;


        default:
            printf("\nQuery not correct \n");
            break;
        };



    }
}


int main(int argc, char const *argv[]) {

    const char* student_file;
    

    if(argc > 2) {printf("Trop de fichier !\n"); return 1;}; // no file

    if(argc==2) {
        student_file = argv[1]; printf("file : %s \n",student_file);
        database_t db_student;
        db_init(&db_student);
        db_load(&db_student,student_file);
        
        printf("---------------------------------------------------\n\n");

        db_afficher(&db_student);
        printf("*** FINISH ***\n");

    }
    else{
        printf("Pas de fichier \n");
        database_t db_student;
        db_init(&db_student);
        
        select_commande();


        printf("\n\n*** FINISH ***\n");
    }

    /*  read file with "<" redirection symbol
    char buffer[BUFFERSIZE];
    fgets(buffer, BUFFERSIZE , stdin);
    printf("Read: %s", buffer);*/


    



    return 0;
}
