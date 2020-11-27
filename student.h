#ifndef _STUDENT_H
#define _STUDENT_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * Student structure type.
 **/
typedef struct {
    unsigned int id;         /** Unique ID **/
    char fname[64];      /** Firstname **/
    char lname[64];      /** Lastname **/
    char section[64];    /** Section **/
    struct tm birthdate; /** Birth date **/
} student_t;

/**
 * Convert a student to a human-readlable string.
 **/
void student_to_str(char* buffer, student_t* s){

    
    char test[] = "- ID : %d\n  Nom,Prenom : %s %s\n  Section : %s\n  Birthdate : %d/%d/%u\n";
    sprintf(buffer,test,s->id,s->fname,s->lname,s->section,s->birthdate.tm_mday,s->birthdate.tm_mon+1,s->birthdate.tm_year+1900);
    
}

/**
 * Return whether two students are equal or not.
 * Two students are equal when all their fields are equal.
 **/
int student_equals(student_t* s1, student_t* s2){

    if ( !memcmp(s1->fname,s2->fname, 64*sizeof(char)) &&
     !memcmp(s1->lname,s2->lname, 64*sizeof(char)) && 
     !memcmp(s1->section,s2->section, 64*sizeof(char)) && 
     s1->id == s2->id && 
    s1->birthdate.tm_mday == s2->birthdate.tm_mday &&
    s1->birthdate.tm_mon == s2->birthdate.tm_mon && 
    s1->birthdate.tm_year == s2->birthdate.tm_year ){ return 1;}

     return 0;

}

#endif