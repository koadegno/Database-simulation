#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "parsing.h"
#include "dbAction.h"
#include "db.h"


const char save_file[256] = "database.bin";
database_t* DATABASE;
int querie_running = 0;// 1 running and 0 not running

static void stop_signal(int sign){
    
    if(querie_running == 2){
        printf("Fermeture ");
        db_save(DATABASE,save_file);
        exit(0);}
    printf("\n*** WAIT UNTIL THE AND OF THE QUERY ***\n\n");
    querie_running = 0;
    return;
}

void select_commande(database_t* student_db)
{
    char field[64], value[64], input[64], fname[64], lname[64], section[64], field_filter[64], value_filter[64], field_to_update[64], update_value[64], *commd_rest, *commd;
    unsigned id;
    struct tm *annif = (struct tm *)malloc(sizeof(struct tm) * 1);
        
    printf("Entrer une commande :\n>> ");
    
    while (fgets(input, 64, stdin)){
            database_t resultat;
            querie_running = 1;
            printf("\n- commande : %s\n", input);
            commd_rest = input;
            commd = strtok_r(NULL, " ", &commd_rest);
            
            // stop action ?
            /*if(querie_running==0){
            db_save(student_db,save_file);
            printf("\n**Sauvegarde de la Base de données **\n\n");
            exit(0);}*/
            
            switch (commd[0])
            {
            
            
            case 's':
                
                // if command not good
                
                if (!parse_selectors(commd_rest, field, value))
                {
                    error();
                    break;
                }
                
                // choose right field
                
                if (!choose_right_field_to_work(field,value,student_db,&resultat)){
                    break;}
                    
                db_afficher(&resultat);break;

            case 'i': //insert someone
                {
                char buff[256];
                if(!parse_insert(commd_rest,fname,lname,&id,section,annif)){
                    error();
                    break;
                }
                // create new student            
                student_t new_stud; new_stud.id=id;strcpy(new_stud.fname,fname);
                strcpy(new_stud.lname,lname);strcpy(new_stud.section,section); new_stud.birthdate = *annif;
                db_add(student_db,&new_stud);

                student_to_str(buff,&new_stud); 
                printf("Ajout : %s\n",buff);
                break;
                }
            case 'd':
                
                if (!parse_selectors(commd_rest, field, value))
                {
                    error();
                    break;
                }
            
                switch (field[0])
                {
                case 'f':
                    delete(student_db,value,0);
                    break;
                case 'l':
                    delete(student_db,value,1);
                    break;
                case 'i':
                    delete(student_db,value,2);
                    break;
                case 's':
                    delete(student_db,value,3);
                    break;
                case 'b':
                    
                    delete(student_db,value,4);
                    break;
                default:
                    break;
                }
                
                break;

            case 'u':

                if(!parse_update(commd_rest,field_filter,value_filter,field_to_update,update_value)){
                    error();
                    break;
                }
                update_db(student_db,field_to_update,update_value,value_filter);
                break;
            default:
                printf("\nQuery not correct \n");
                break;
                

            };
        printf("---------------------------------------------------\n\n");
        printf("Entrer une commande : \n>> ");
    
        if(querie_running==0){
            db_save(student_db,save_file);
            //printf("\n**Sauvegarde de la Base de données **\n\n");
            exit(0);}
        querie_running = 2;
    }
}



int main(int argc, char const *argv[])
{
    // handeling CTRL + C and CTRL + D signal
    struct sigaction stop_procc;
    stop_procc.sa_handler = stop_signal;
    stop_procc.sa_flags = SA_RESTART;

    sigaction(SIGINT,&stop_procc,NULL); //  CTRL + C
    sigaction(SIGQUIT,&stop_procc,NULL); //  CTRL + D

    
    const char *student_file;

    if (argc > 2)
    {
        printf("Trop de parametre !\n");
        return 1;
    }; // no file

    if (argc == 2)
    {
        student_file = argv[1];
        database_t db_student;
        db_init(&db_student);printf("\n\n*** INITIALISATION DE LA BASE DE DONNEES ***\n\n");
        db_load(&db_student, student_file);
        DATABASE = &db_student;
        strcmp(save_file,student_file);
        select_commande(&db_student);
                
    }
    else
    {
        database_t db_student;
        db_init(&db_student);printf("\n*** INITIALISATION DE LA BASE DE DONNEES ***\n");
        DATABASE = &db_student;
        select_commande(&db_student);
    }
    db_save(DATABASE,save_file);
    printf("TERMINAISON ...\n");
    
    return 0;
}
