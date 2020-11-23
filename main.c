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

void error(char *err)
{
    printf("\n**Querie Error %s**\nTry again\n", err);
}

void select_commande()
{
    char field[100];
    char value[100];
    char input[100];
    char fname[64];
    char lname[64];
    char section[64];
    unsigned id;
    struct tm *annif = (struct tm *)malloc(sizeof(struct tm) * 1);
    char field_filter[100];
    char value_filter[100];
    char field_to_update[100];
    char update_value[100];
    char *commd_rest;
    char *commd;

    while (keepon == 0)
    {

        printf("> ");
        fgets(input, 100, stdin);
        printf(" - commande : %s\n", input);
        commd_rest = input;
        commd = strtok_r(NULL, " ", &commd_rest);

        switch (commd[0])
        {
        case 's':

            if (!parse_selectors(commd_rest, field, value))
            {
                error("select");
                break;
            }
            //printf("field : %s\nvalue : %s\n",field,value);

            if(!strcmp(field,"fname")){
                printf("fname : %s", value);
            }
            else if(!strcmp(field,"lname")){        
                printf("lname : %s",value);
            }
            else if(!strcmp(field,"id")){       
                printf("id : %s",value);
            }
            else if(!strcmp(field,"section")){       
                printf("section : %s",value);
            }
            else if(!strcmp(field,"birthdate")){        
                printf("birthdate : %s",value);
        
            }
            else error(""); break;


            
            
            break;
        case 'i':
            

            if(!parse_insert(commd_rest,fname,lname,&id,section,annif)){
                error("insert");
                break;
            }
            printf("\nINSERT GOOOD \n\n");
            printf("fname : %s\nlname :%s\nid : %u\nsection : %s\n",fname,lname,id,section);
            break;

        case 'd':
            printf("\ndelete \n");

            break;

        case 'u':

            
            if(!parse_update(commd_rest,field_filter,value_filter,field_to_update,update_value)){
                error("update");
                break;
            }
            printf("\nUPDATE GOOD\n\n");
            printf("field_fielter : %s\nvalue_filter : %s\nfield_to_update :%s\nupdate_value : %s\n",field_filter, value_filter,field_to_update,update_value);
            break;


        default:
            printf("\nQuery not correct \n");
            break;
        };
    }
}

int main(int argc, char const *argv[])
{

    const char *student_file;

    if (argc > 2)
    {
        printf("Trop de fichier !\n");
        return 1;
    }; // no file

    if (argc == 2)
    {
        student_file = argv[1];
        printf("file : %s \n", student_file);
        database_t db_student;
        db_init(&db_student);
        db_load(&db_student, student_file);

        printf("---------------------------------------------------\n\n");

        db_afficher(&db_student);
        printf("*** FINISH ***\n");
    }
    else
    {
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
