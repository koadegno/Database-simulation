#ifndef _FIND_H_
#define _FIND_H_

#include "student.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "parsing.h"
#include "db.h"
#include <stdbool.h>
#define thread_max 4

pthread_mutex_t lock;           /** mutex to managa **/
pthread_t thread[thread_max];   /** thread to manage **/
int current_find_thread = 0;    /**nb of current thread **/


/**
 * Parametre for all find function
 * 
 */
typedef struct {
    char to_search[64]; /** what to search **/
    database_t* begin;  /**begin = where to search; **/
    database_t* end;    /** end = where to stock the result **/
    
} find_param;

/**
 * Thread function to find fname inside
 * the db
 * param :  find_param
 */
void *find_in_fname(void* val){
    find_param* param = (find_param*)val;

    pthread_mutex_lock(&lock);
    int num = current_find_thread++; // all threads need that global variable 
    pthread_mutex_unlock(&lock);

    size_t max = (param->begin)->lsize;
    size_t stop = ((num+1) * (max/4));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;printf("Taille max du dernier thread : %ld avec tid : %ld\n",stop,pthread_self());}
    
    for(size_t i = num * ( max/4); i < stop;i++){
        //printf("le nom = %s  ",(param->to_search));
        
        if(!strcmp((param->begin->data[i]).fname,param->to_search) ){
                
                pthread_mutex_lock(&lock);
                db_add(param->end,&(param->begin->data[i]));
                pthread_mutex_unlock(&lock);
                
            }
    }
    return NULL;
}

/**
 * Thread function to find lname inside
 * the db
 * param :  find_param
 */
void *find_in_lname(void* val){
    find_param* param = (find_param*)val;

    pthread_mutex_lock(&lock);
    int num = current_find_thread++; // all threads need that global variable 
    pthread_mutex_unlock(&lock);
    size_t max = (param->begin)->lsize;
    size_t stop = ((num+1) * (max/4));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;printf("Taille max du dernier thread : %ld avec tid : %ld\n",stop,pthread_self());}
    for(size_t i = num * (max/4); i < stop;i++){
        
        if(!strcmp( (param->begin->data[i]).lname,param->to_search) ){
                
                pthread_mutex_lock(&lock);
                db_add(param->end,&(param->begin->data[i]));
                pthread_mutex_unlock(&lock);
            }
        
    }
    
    return ;
}

/**
 * Thread function to find id inside
 * the db
 * param :  find_param
 */
void *find_in_id(void* val){
    find_param* param = (find_param*)val;
    int id = atoi(param->to_search);

    pthread_mutex_lock(&lock);
    int num = current_find_thread++; // all threads need that global variable 
    pthread_mutex_unlock(&lock);

    size_t max = (param->begin)->lsize;
    size_t stop = ((num+1) * (max/4));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;printf("Taille max du dernier thread : %ld avec tid : %ld\n",stop,pthread_self());}
    
    for(size_t i = num * (max/4); i < stop;i++){

        if((param->begin->data[i]).id == id ){
                pthread_mutex_lock(&lock);
                db_add(param->end,&(param->begin->data[i]));
                pthread_mutex_unlock(&lock);
            }

    }
    return NULL;
}   

/**
 * Thread function to find birth inside
 * the db
 * param :  find_param
 */
void *find_in_birth(void* val){
    find_param* param = (find_param*)val;
    
    pthread_mutex_lock(&lock);
    int num = current_find_thread++; // all threads need that global variable 
    pthread_mutex_unlock(&lock);

    struct tm datee;strptime(param->to_search, "%d/%m/%Y", &datee);
    int max = (param->begin)->lsize;
    size_t stop = ((num+1) * (max/4));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;printf("Taille max du dernier thread : %ld avec tid : %ld\n",stop,pthread_self());}
    
    for(size_t i = num * (max/4); i < stop;i++)
    {

        if((param->begin->data[i]).birthdate.tm_mday == datee.tm_mday &&
            (param->begin->data[i]).birthdate.tm_mon == datee.tm_mon && 
            (param->begin->data[i]).birthdate.tm_year ==datee.tm_year ){
                pthread_mutex_lock(&lock);
                db_add(param->end,&(param->begin->data[i]));
                pthread_mutex_unlock(&lock);

            }

    }
    return NULL;
}

/**
 * Thread function to find section inside
 * the db
 * param :  find_param.
 */
void *find_in_section(void* val){
    find_param* param = (find_param*)val;

    int num = current_find_thread++;
    int max = (param->begin)->lsize;
    size_t stop = ((num+1) * (max/4));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;printf("Taille max du dernier thread : %ld avec tid : %ld\n",stop,pthread_self());}
    
    for(size_t i = num * (max/4); i < stop;i++){

        if(!strcmp((param->begin->data[i]).section,param->to_search) ){
                pthread_mutex_lock(&lock);
                db_add(param->end,&(param->begin->data[i]));
                pthread_mutex_unlock(&lock);
            }

    }
    return NULL;
}



void db_search(database_t* start_db, database_t* end_db,char to_search[], int field){

    to_search[strlen(to_search) -1] = '\0';
    
    find_param to_pass; // struct just for thread param
    to_pass.begin = start_db; to_pass.end = end_db;
    strcpy(to_pass.to_search,to_search);

    if (pthread_mutex_init(&lock, NULL)!=0){ 
        printf("\n*** ERREUR AVEC LA CREATION DU MUTEX ***\n");
        return;
    }
        
    switch (field){
    case 0: // field == fname
            
        for (int i = 0; i < thread_max; i++) { 
            if(pthread_create(&thread[i], NULL,find_in_fname, (void*)&to_pass) != 0){
                printf("\n*** PEUT PAS CREER LE THREAD ***\n");
            }; 
        
        }

        for (int i = 0; i < thread_max; i++) { 
            if (!pthread_join(thread[i], NULL)){printf("Le %d a finis : %ld\n",i,thread[i]);}; 
        } 

        break;
    case 1: // field == lname

        
        for (int i = 0; i < thread_max; i++) {
                
            if(pthread_create(&thread[i], NULL,find_in_lname, (void*)&to_pass) != 0){
                printf("*** PEUT PAS CREER LE THREAD ***\n");
            };  
        
        }

        for (int i = 0; i < thread_max; i++) {
            
            pthread_join(thread[i], NULL); 
        } 

        
        break;

    case 2: // field == id
                    
        for (int i = 0; i < thread_max; i++) { 
            if(pthread_create(&thread[i], NULL,find_in_id, (void*)&to_pass) != 0){
                printf("*** PEUT PAS CREER LE THREAD ***\n");
            };
        }

        for (int i = 0; i < thread_max; i++) { 
            pthread_join(thread[i], NULL); 
        } 

        break;

    case 3: // field == section
        
        for (int i = 0; i < thread_max; i++) { 
            if(pthread_create(&thread[i], NULL,find_in_section, (void*)&to_pass) != 0){
                printf("*** PEUT PAS CREER LE THREAD ***\n");
            };
        }

        for (int i = 0; i < thread_max; i++) { 
            pthread_join(thread[i], NULL); 
        } 

        break;

    case 4: // field == birthdate

        for (int i = 0; i < thread_max; i++) { 
            if(pthread_create(&thread[i], NULL,find_in_birth, (void*)&to_pass) != 0){
                printf("*** PEUT PAS CREER LE THREAD ***\n");
            };
        }

        for (int i = 0; i < thread_max; i++) { 
            pthread_join(thread[i], NULL); 
        } 

        break;
    default:
        break;
    }
    current_find_thread = 0;
    pthread_mutex_destroy(&lock);
}

void error(char *err)
{
    printf("\n**Querie Error %s**\nTry again\n", err);
}

bool choose_right_field_to_search(char* field,char* value,database_t* student_db,database_t* resultat){

    // choose right field
    bool ret = true;
    if(!strcmp(field,"fname")){
        
        db_init(resultat);
        printf("fname : %s", value);
        
        db_search(student_db,resultat,value,0);

    } 
    else if(!strcmp(field,"lname")){   
        
        db_init(resultat);     
        printf("lname : %s",value);
        db_search(student_db,resultat,value,1);

    } 
    else if(!strcmp(field,"id")){  

        
        db_init(resultat);
        printf("ok\n");
        printf("id : %s",value);
        db_search(student_db,resultat,value,2);

    } 
    else if(!strcmp(field,"section")){ 
        
        db_init(resultat);      
        printf("section : %s",value);
        db_search(student_db,resultat,value,3);

    } 
    else if(!strcmp(field,"birthdate")){ 
        
        db_init(resultat);       
        printf("birthdate : %s",value);
        db_search(student_db,resultat,value,4);
        

    } 
    else{error("");ret = false;
    }
    return ret;
}


#endif