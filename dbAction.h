#ifndef _DBACTION_H_
#define _DBACTION_H_

#include "student.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "parsing.h"
#include "db.h"
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "writelog.h"
#define thread_max 4

pthread_mutex_t lock;           /** mutex to manage critique section**/
pthread_t t1; pthread_t t2; pthread_t t3; pthread_t t4;
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
    size_t stop = ((num+1) * (max/thread_max));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;}
    
    for(size_t i = num * ( max/thread_max); i < stop;i++){
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
    size_t stop = ((num+1) * (max/thread_max));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;}
    for(size_t i = num * (max/thread_max); i < stop;i++){
        
        if(!strcmp( (param->begin->data[i]).lname,param->to_search) ){
                
                pthread_mutex_lock(&lock);
                db_add(param->end,&(param->begin->data[i]));
                pthread_mutex_unlock(&lock);
            }
    }
    return NULL;
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
    size_t stop = ((num+1) * (max/thread_max));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;}
    
    for(size_t i = num * (max/thread_max); i < stop;i++){
        
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
    size_t stop = ((num+1) * (max/thread_max));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;}
    
    for(size_t i = num * (max/thread_max); i < stop;i++)
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
    size_t stop = ((num+1) * (max/thread_max));

    // if max and nb of thread aren't Greatest common divisor (pgcd)
    if (thread[3] == pthread_self()){stop+=max-stop;}
    
    for(size_t i = num * (max/thread_max); i < stop;i++){

        if(!strcmp((param->begin->data[i]).section,param->to_search) ){
                pthread_mutex_lock(&lock);
                db_add(param->end,&(param->begin->data[i]));
                pthread_mutex_unlock(&lock);
            }

    }
    return NULL;
}


/**
 * search inside de db depending of the field
 * using thread (thread_max)
 */
void db_search(database_t* start_db, database_t* end_db,const char to_search[], int field){

    //printf("1 My va   lue : %s\n",to_search);
        
    find_param to_pass; // struct just for thread param
    to_pass.begin = start_db; to_pass.end = end_db;
    strcpy(to_pass.to_search,to_search);

    // start mutex for thread
    if (pthread_mutex_init(&lock, NULL)!=0){ 
        printf("\n*** ERREUR AVEC LA CREATION DU MUTEX ***\n");
        return;
    }

    // add thread to the array
    thread[0] =t1; thread[1] =t2;  thread[2] =t3;  thread[3] =t4; 

    switch (field){
    case 0: // field == fname
            
        for (int i = 0; i < thread_max; i++) { 
            if(pthread_create(&thread[i], NULL,find_in_fname, (void*)&to_pass) != 0){
                printf("\n*** PEUT PAS CREER LE THREAD ***\n");
            };
        }

        for (int i = 0; i < thread_max; i++) { 
            pthread_join(thread[i], NULL); 
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

/**
 * Execute select query for database 999999 elements
 * 
 */
void query_select_normal(database_t *source,database_t* dest,const char* field,const char* value){

    
    for(size_t j =0; j< source->lsize; j++){
        student_t *tmp = &(source->data[j]);
        switch (field[0])
        {
        case 'f': //fname
            
            if (!strcmp((source->data[j]).fname,value)){
                db_add(dest,tmp);
            }
            break;
        case 'l': //lname
           
            if (!strcmp((source->data[j]).lname,value)){
                db_add(dest,tmp);
            }
            break;
        case 'i': //id
            {
            int id = atoi(value);
            if ((source->data[j]).id == id){
                
                db_add(dest,tmp);
            }
            }
            break;
        case 's': //section
            if (!strcmp((source->data[j]).section,value)){
                db_add(dest,tmp);
            }
            break;
        case 'b': //birthdate
        {
            
            struct tm date;strptime(value, "%d/%m/%Y", &date);
            if((source->data[j]).birthdate.tm_mday == date.tm_mday &&
            (source->data[j]).birthdate.tm_mon == date.tm_mon && 
            (source->data[j]).birthdate.tm_year ==date.tm_year ){
            db_add(dest,&(source->data[j]));}
            break;
        }
        default:
            break;
        }   
    }


}

/**
 * Show error 
 */
void error()
{
    printf("\n** Querie Error **\nTry again\n");
}

/**
 * Choose the right field to lauch the search
 * 
 */
bool choose_right_field_to_work(const char* field,char* value,database_t* student_db,database_t* resultat,LogPath* log){
    clock_t start, end;
    double cpu_time_used;

    if (value[strlen(value)-1] == '\n'){
        value[strlen(value) -1] = '\0';
    
    }
    // choose right field
    bool ret = true;db_init(resultat);

    char job_to_do[5][11] = {"fname","lname","id","section","birthdate"};
    
    if (student_db->lsize >= 999999){
        for(int i = 0;i <5;i++){
            
            if(!strcmp(field,job_to_do[i])){
                
                start = clock();
                db_search(student_db,resultat,value,i);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                WriteLog(log,resultat,cpu_time_used);
                ret = true;
                break;
                
            }
            else{
                ret=false;
            }
        }
    }else{
        start = clock();
        query_select_normal(student_db,resultat,field,value);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        WriteLog(log,resultat,cpu_time_used);
    }
   
    return ret;
}

/**
 * delete in termes of which field is select and the string search_for
 *  
 */
void delete(database_t *source, char* search_for,int field,LogPath* log,database_t *res){
    size_t max = source->lsize;
    int reduc = 0;
    int i = (source->lsize)-1;
    clock_t start, end;
    double cpu_time_used;
    if (search_for[strlen(search_for)-1] == '\n'){
        search_for[strlen(search_for) -1] = '\0';
    }
    
    start = clock();

    // loop for swap with the last one
    for(;i >= 0;i--){
        //printf("ok i=%d  ",i);
        //sleep(10);
        switch (field)
        {
        case 0: // fname
            if(!strcmp((source->data[i]).fname,search_for) ){
                db_add(res,&(source->data[i]));
                student_t tmp = (source->data[max-1-reduc]); // last
                source->data[max-1-reduc] = source->data[i];
                source->data[i] = tmp;reduc++;

            }
            break;
        case 1: // lname
            if(!strcmp((source->data[i]).lname,search_for) ){
                db_add(res,&(source->data[i]));
                student_t tmp = (source->data[max-1-reduc]);
                source->data[max-1-reduc] = source->data[i];
                source->data[i] = tmp;reduc++;
            }
            break;
        case 2: // id
        {   
            int stud_id = atoi(search_for);
            
            if((source->data[i]).id == stud_id ){    
                db_add(res,&(source->data[i]));  
                student_t tmp = (source->data[max-1-reduc]);          
                source->data[max-1-reduc] = source->data[i];
                source->data[i] = tmp;reduc++;
            }
        }
            break;
        case 3: // section
            if(!strcmp((source->data[i]).section,search_for) ){
                db_add(res,&(source->data[i]));
                student_t tmp = (source->data[max-1-reduc]);
                source->data[max-1-reduc] = source->data[i];
                source->data[i] = tmp;
                reduc++;

            }
            break;
        case 4: // birth
            {
            
            struct tm datee;strptime(search_for, "%d/%m/%Y", &datee);
            if( (source->data[i]).birthdate.tm_mday == datee.tm_mday &&
            // Ajoute des elements a sup a la partir de le fin 
            (source->data[i]).birthdate.tm_mon == datee.tm_mon && 
            (source->data[i]).birthdate.tm_year ==datee.tm_year  ){
                //swap
                db_add(res,&(source->data[i]));
                student_t tmp = (source->data[max-1-reduc]);
                source->data[max-1-reduc] = source->data[i];
                source->data[i] = tmp;
                reduc++;
            }
            }
            break;
        default:
            break;
        }
    
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    WriteLog(log,res,cpu_time_used);
    printf("\nIl y a %d qui correspond\n",reduc);
    source->lsize = source->lsize - reduc;


}

/**
 * set source->data[index] with the right field to value
 */
void do_modification(database_t *source,size_t index,char* field_to_mod,char* value){


    if (value[strlen(value)-1] == '\n'){
        value[strlen(value) -1] = '\0';
    
    }
    switch (field_to_mod[0])
        {
        case 'f': //fname
            
            
            strcpy((source->data[index]).fname,value);
            
            break;
        case 'l': //lname
            
            strcpy((source->data[index]).lname,value);
            
            break;
        case 'i': //id
            {
            int id = atoi(value);
            (source->data[index]).id = id;
            
            }
            break;
        case 's': //section
            
            strcpy((source->data[index]).section,value);
            
            break;
        case 'b': //birthdate
        {
            struct tm datee;strptime(value, "%d/%m/%Y", &datee);
            (source->data[index]).birthdate = datee;
            break;
        }
        default:
            break;
        }
}

/**
 * Just update the db source
 */
void update_db(database_t *source,char *field_fielter,char* field_to_up, char* value,char* last_value,LogPath* log, database_t* res){
    clock_t start, end;
    double cpu_time_used;
    
    start =clock();
    for(size_t j =0; j< source->lsize; j++){
        switch (field_fielter[0])
        {
        case 'f': //fname
            if (!strcmp((source->data[j]).fname,last_value)){
                do_modification(source,j,field_to_up,value);
                db_add(res,&(source->data[j]));
            }
            break;
        case 'l': //lname
            if (!strcmp((source->data[j]).lname,last_value)){
                do_modification(source,j,field_to_up,value);
                db_add(res,&(source->data[j]));
            }
            break;
        case 'i': //id
            {
            int last_id = atoi(last_value);
            if ((source->data[j]).id == last_id){
                
                do_modification(source,j,field_to_up,value);
                db_add(res,&(source->data[j]));
            }
            }
            break;
        case 's': //section
            if (!strcmp((source->data[j]).section,last_value)){
                do_modification(source,j,field_to_up,value);
                db_add(res,&(source->data[j]));
            }
            break;
        case 'b': //birthdate
        {
            
            struct tm last_datee;strptime(last_value, "%d/%m/%Y", &last_datee);
            if((source->data[j]).birthdate.tm_mday == last_datee.tm_mday &&
            (source->data[j]).birthdate.tm_mon == last_datee.tm_mon && 
            (source->data[j]).birthdate.tm_year ==last_datee.tm_year ){
            do_modification(source,j,field_to_up,value);
            db_add(res,&(source->data[j]));
            }
            break;
        }
        default:
            break;
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    WriteLog(log,res,cpu_time_used);
}

#endif