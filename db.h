#ifndef _DB_H
#define _DB_H


#include "student.h"
#include "dbAction.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#include <unistd.h>



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

    for(size_t i =0; i < db->lsize; i++){
        fwrite(&(db->data[i]),sizeof(db->data[i]),1,ptr_file);

    }
    //db_afficher(db);
    fclose(ptr_file);
    printf("SAVE OK\n");


}


void db_afficher(database_t* db) {
    char buff[256];
    printf("%zd eléments dans la DB.\n\n", db->lsize);
    for (size_t i = 0; i < db->lsize; i++) {
        printf("%ld",i+1);
        student_to_str(buff, &( db->data[i] ));
        printf("%s\n",buff);
        //sleep(1);
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


#endif