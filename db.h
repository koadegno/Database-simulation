#ifndef _DB_H
#define _DB_H

#include "student.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * Database structure type.
 */
typedef struct {
    student_t *data; /** The list of students **/
    size_t lsize;    /** The logical size of the list **/
    size_t psize;    /** The physical size of the list **/
} database_t;


/**
 * 
 * Extend the place inside the db
 * 
 **/
void db_etendre_taille(database_t *db){

     if (db->lsize >= db->psize) {
        db->psize *= 2;
        db->data = realloc(db->data, sizeof(student_t) * db->psize);
        if (!db->data) {
            perror("Erreur de malloc!\n");
            exit(1);
        }
    }
}



/** 
 *  Add a student to the database.
 * TODO: implement this function.
 **/
void db_add(database_t *db, student_t* s){

    db_etendre_taille(db);
    db->data[db->lsize] = *s;
    db->lsize++;
}




/**
 * Delete a student from the database.
 * TODO: implement this function.
 **/
void db_delete(database_t *db, student_t *s){

    bool flag = false;
    // On va parcourir la db en ecrasant la data i par la data i+1 pour ne pas avoir de "trou" dans le tableau.
    for (size_t i = 0; i < db->lsize; i++) {
        if (student_equals(&(db->data)[i],s) || flag){
            db->data[i] = db->data[i + 1];
            flag = true;
        }  
    }
    db->lsize--;
}


/**
 * Save the content of a database_t to the specified file.
 * TODO: implement this function
 **/
void db_save(database_t *db, const char *path){
    
    FILE* ptr_file = fopen(path,"wb");

    if (!ptr_file){ printf("\nERREUR FILE\n"); return;}

    student_t var = db->data[db->lsize-1];
    char t[256];
    student_to_str(t,&var);
    printf("etudiant : %s\n",t);

    //fwrite(&var,sizeof(student_t),1,ptr_file);
    for(size_t i =0; i < db->lsize; i++){
        printf("the idx : %ld ",i );
        fwrite(&(db->data[i]),sizeof(db->data[i]),1,ptr_file);

     } 
    //db_afficher(db);
    fclose(ptr_file);
    printf("\nSAVE RUNNING\n");


}


void db_afficher(database_t* db) {
    char buff[256];
    printf("%zd eléments dans la DB.\n\n", db->lsize);
    for (size_t i = 0; i < db->lsize; i++) {
        printf("%ld",i);
        student_to_str(buff, &( db->data[i] ));
        printf("%s\n",buff);
        sleep(2);
    }
    
    
}


/**
 * Load the content of a database of students from a file.
 * TODO: implement this function.
 **/
void db_load(database_t *db, const char *path){

    
    FILE* file_data;
    file_data = fopen(path,"rb");
    
    if(!file_data) {fclose(file_data);return;} // Don't get acces to file
    
    student_t obj_student;

    // get nb of line in the file
    /*fseek(file_data, 0L, SEEK_END); 
    long int file_size = ftell(file_data)/sizeof(student_t);
    fseek(file_data, sizeof(student_t), SEEK_SET);*/
    
    while(fread(&obj_student,sizeof(obj_student),1,file_data))
    {
        db_add(db,&obj_student);
        //printf("id : %u\nFirst name : %s\nLast name : %s\nSection : %s\nAnnif : %d/%d/%d\n",obj_student->id,obj_student->fname,obj_student->lname,obj_student->section,obj_student->birthdate.tm_mday,obj_student->birthdate.tm_mon,obj_student->birthdate.tm_year);
        
    };
    fclose(file_data);
    
}



/**
 * Initialise a database_t structure.
 * Typical use:
 * ```
 * database_t db;
 * db_init(&db);
 * ```
 * TODO: implement this function.
 **/
void db_init(database_t *db){
    
    db->psize = 100000;
    db->lsize = 0;
    db->data = (student_t*)malloc(sizeof(student_t) * db->psize);
    if (!db->data) {
        perror("Erreur lors de l'allocation de la memoire!\n");
        exit(1);
    }

    
}

/**
 * Function to search into the db
 * field is a number for representing each field 
 * 
 **/
void db_search(database_t* start_db, database_t* end_db,char to_search[], int field){

    to_search[strlen(to_search) -1] = '\0';struct tm datee;strptime(to_search, "%d/%m/%Y", &datee);
    for(int i =0;i < start_db->lsize;i++){
        
//select fname=Yury

        switch (field)
        {
        
        
        case 0: // field == fname
            
            //printf("le nom : %s ",to_search);
            //if(tmp[1] == 'u' && tmp[0] == 'Y' && tmp[2] == 'r'){printf("le nom : %s ",tmp);}
            if(!strcmp((start_db->data[i]).fname,to_search) ){
                db_add(end_db,&(start_db->data[i]));
            }
            break;
        case 1:

            if(!strcmp((start_db->data[i]).lname,to_search) ){
                db_add(end_db,&(start_db->data[i]));
            }
            break;
        case 2:
            {int id = atoi(to_search);
            //printf("le nb : %d",id);
            if((start_db->data[i]).id == id ){
                db_add(end_db,&(start_db->data[i]));
            }}
            break;
        case 3:
            if(!strcmp((start_db->data[i]).section,to_search) ){
                db_add(end_db,&(start_db->data[i]));
            }
            break;
        case 4:
        if((start_db->data[i]).birthdate.tm_mday == datee.tm_mday &&
            (start_db->data[i]).birthdate.tm_mon == datee.tm_mon && 
            (start_db->data[i]).birthdate.tm_year == datee.tm_year ){
                db_add(end_db,&(start_db->data[i]));
            }
            break;
        default:
            break;
        }
    }

}


#endif