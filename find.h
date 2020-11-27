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
#define thread_max 4

pthread_mutex_t lock;
pthread_t thread[thread_max]; 
int current_find_thread = 0; /**nb of current thread */


/**
 * Parametre for all find function
 * 
 */
typedef struct {
    char to_search[64]; /** what to search **/
    database_t* begin;  /**begin = where to search; **/
    database_t* end;    /** end = where to stock the result **/
    
} find_param;

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


void *find_in_id(void* val){
    find_param* param = (find_param*)val;
    int id = atoi(param->to_search);
    pthread_mutex_lock(&lock);
    int num = current_find_thread++; // all threads need that global variable 
    pthread_mutex_unlock(&lock);
    int max = (param->begin)->lsize;

    for(size_t i = num * (max/4); i < ((num+1) * (max/4));i++){

        if((param->begin->data[i]).id == id ){
                pthread_mutex_lock(&lock);
                db_add(param->end,&(param->begin->data[i]));
                pthread_mutex_unlock(&lock);
            }

    }
    return NULL;
}   


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
        
        //printf("le nom : %s ",to_search);
        //if(tmp[1] == 'u' && tmp[0] == 'Y' && tmp[2] == 'r'){printf("le nom : %s ",tmp);}
        /*if(!strcmp((start_db->data[i]).fname,to_search) ){
            db_add(end_db,&(start_db->data[i]));
        }*/
        
        
            
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

        /*if(!strcmp((start_db->data[i]).lname,to_search) ){
            db_add(end_db,&(start_db->data[i]));
        }
        break;*/

        
        //to_pass.begin = start_db; to_pass.end = end_db;to_pass.to_search = to_search;
        
        for (int i = 0; i < thread_max; i++) {
                
            if(pthread_create(&thread[i], NULL,find_in_lname, (void*)&to_pass) != 0){
                printf("*** PEUT PAS CREER LE THREAD ***\n");
            };  
        
        }

        for (int i = 0; i < thread_max; i++) {
            
            pthread_join(thread[i], NULL); 
        } 

        pthread_mutex_destroy(&lock);
        break;

    case 2: // field == id
        /*{int id = atoi(to_search);
        //printf("le nb : %d",id);
        if((start_db->data[i]).id == id ){
            db_add(end_db,&(start_db->data[i]));
        }}
        break;*/

        
        //to_pass.begin = start_db; to_pass.end = end_db;to_pass.to_search = to_search;
            
        for (int i = 0; i < thread_max; i++) { 
            pthread_create(&thread[i], NULL,find_in_id, (void*)&to_pass); 
        
        }

        for (int i = 0; i < thread_max; i++) { 
            pthread_join(thread[i], NULL); 
        } 

        break;

    case 3: // field == section
        /*if(!strcmp((start_db->data[i]).section,to_search) ){
            db_add(end_db,&(start_db->data[i]));
        }
        break;*/

        for (int i = 0; i < thread_max; i++) { 
            pthread_create(&thread[i], NULL,find_in_section, (void*)&to_pass); 
        
        }

        for (int i = 0; i < thread_max; i++) { 
            pthread_join(thread[i], NULL); 
        } 

        break;

    case 4: // field == birthdate
    /*if((start_db->data[i]).birthdate.tm_mday == datee.tm_mday &&
        (start_db->data[i]).birthdate.tm_mon == datee.tm_mon && 
        (start_db->data[i]).birthdate.tm_year == datee.tm_year ){
            db_add(end_db,&(start_db->data[i]));
        }
        break;*/

        
        //to_pass.begin = start_db; to_pass.end = end_db;strcpy(to_pass.to_search,to_search);
        
        for (int i = 0; i < thread_max; i++) { 
            pthread_create(&thread[i], NULL,find_in_birth, (void*)&to_pass); 
        
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


#endif