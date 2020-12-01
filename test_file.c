#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>

char* message = "Message important ne pas interrompre \n";

static void action(int sig){
    write(1,"Reception du signal SIGINT\n",28);
}

int main(){

    signal(SIGINT,action);
    int count=0;
    printf("Debut:\n");

    sigset_t set;
    sigemptyset(&set);
    sigprocmask(SIG_BLOCK,&set, NULL);

    do{
        write(2,&message[count],1);
        sleep(1);
        count=count+1;
    }while (count <strlen(message)+1);

    sigemptyset(&set);
    sigpending(&set);
    if(sigismember(&set,SIGINT)){
        write(1,"SIGINT est en attente\n", strlen("SIGINT est en attente\n")+1);};
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_UNBLOCK,&set,NULL);
    return 0;
}