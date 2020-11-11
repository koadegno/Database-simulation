#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdio.h>
#include "student.h"
#include <limits.h>

#define BUFFERSIZE 100

int open_file(const char* file_name,student_t* obj_student){
    FILE* file_data;
    file_data = fopen(file_name,"rb");
    
    
  
    // calculating the size of the file 
     
    

    if(!file_data) {fclose(file_data); return 0;} //false

    //long int a= fseek(file_data, -sizeof(student_t)*10, SEEK_END);
    fseek(file_data, 0L, SEEK_END); 
    long int size = ftell(file_data)/sizeof(student_t);
    fseek(file_data, sizeof(student_t), SEEK_SET);

    for(int i=0;i <size; i++)
    {
        //
        printf("\n");
        fread(obj_student,sizeof(*obj_student),1,file_data);
        printf("id : %u\nFirst name : %s\nLast name : %s\nSection : %s\nAnnif : %d/%d/%d\n",obj_student->id,obj_student->fname,obj_student->lname,obj_student->section,obj_student->birthdate.tm_mday,obj_student->birthdate.tm_mon,obj_student->birthdate.tm_year);
        
    };

    

    fclose(file_data);
    return 1; //true
}



int main(int argc, char const *argv[]) {

    const char* student_file;
    

    if(argc > 2) {printf("Trop de fichier !\n"); return 1;}; // no file

    if(argc==2) {
        student_file = argv[1]; printf("file : %s \n",student_file);
        student_t stud_struct;
        if (open_file(student_file,&stud_struct)){
            printf("open succed \n");
            
        }
        else{
            printf("ERREUR FILE \n");
            return 1;
        }
    }
    else{
        printf("Pas de fichier \n");
    }

    /*  read file with "<" redirection symbol
    char buffer[BUFFERSIZE];
    fgets(buffer, BUFFERSIZE , stdin);
    printf("Read: %s", buffer);*/


    



    return 0;
}
