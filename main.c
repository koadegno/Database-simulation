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

void select_commande(database_t* student_db)
{
    char field[64];
    char value[64];
    char input[64];
    char fname[64];
    char lname[64];
    char section[64];
    unsigned id;
    struct tm *annif = (struct tm *)malloc(sizeof(struct tm) * 1);
    char field_filter[64];
    char value_filter[64];
    char field_to_update[64];
    char update_value[64];
    char *commd_rest;
    char *commd;

    while (keepon == 0)
    {

        printf("> ");
        fgets(input, 100, stdin);
        printf("\n- commande : %s\n", input);
        commd_rest = input;
        commd = strtok_r(NULL, " ", &commd_rest);

        switch (commd[0])
        {
        database_t resultat;
        case 's':
            
            // if command not good
            if (!parse_selectors(commd_rest, field, value))
            {
                error("select");
                break;
            }
            
            // choose right field
            if(!strcmp(field,"fname")){
                
                db_init(&resultat);
                printf("fname : %s", value);
                
                db_search(student_db,&resultat,value,0);

            } 
            else if(!strcmp(field,"lname")){   
                
                db_init(&resultat);     
                printf("lname : %s",value);
                db_search(student_db,&resultat,value,1);

            } 
            else if(!strcmp(field,"id")){       
                
                db_init(&resultat);
                printf("id : %s",value);
                db_search(student_db,&resultat,value,2);

            } 
            else if(!strcmp(field,"section")){ 
                
                db_init(&resultat);      
                printf("section : %s",value);
                db_search(student_db,&resultat,value,3);

            } 
            else if(!strcmp(field,"birthdate")){ 
                
                db_init(&resultat);       
                printf("birthdate : %s",value);
                db_search(student_db,&resultat,value,4);
                
        
            } 
            else{error("");break;} 
            db_afficher(&resultat);;break;

        case 'i': //insert someone
            {

            if(!parse_insert(commd_rest,fname,lname,&id,section,annif)){
                error("insert");
                break;
            }
                        
            student_t new_stud; new_stud.id=id;strcpy(new_stud.fname,fname);
            strcpy(new_stud.lname,lname);strcpy(new_stud.section,section); new_stud.birthdate = *annif;
            db_add(student_db,&new_stud);
            {char* buff; student_to_str(buff,&new_stud); printf("Etudiant bien ajouter : %s\n",buff);}
            break;
            }
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
        db_init(&db_student);printf("\n*** INITIALISATION DE LA BASE DE DONNEES ***\n");
        db_load(&db_student, student_file);

        printf("---------------------------------------------------\n\n");

        select_commande(&db_student);
        printf("*** FINISH ***\n");
    }
    else
    {
        printf("Pas de fichier \n");
        database_t db_student;
        db_init(&db_student);printf("\n*** INITIALISATION DE LA BASE DE DONNEES ***\n");

        select_commande(&db_student);

        printf("\n\n*** FINISH ***\n");
    }

    /*  read file with "<" redirection symbol
    char buffer[BUFFERSIZE];
    fgets(buffer, BUFFERSIZE , stdin);
    printf("Read: %s", buffer);*/

    return 0;
}
