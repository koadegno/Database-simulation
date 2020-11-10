#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdio.h>

int main(int argc, char const *argv[]) {

    char* student_file;

    if(argc > 3) return 1;

    if(argc>2) student_file = agrv[2];


    printf("file %s : ",student_file)






    return 0;
}
