# include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
char *name = "No one";

void sing (int code){
    fprintf(stderr, "Signal number %d\n", code);
    fprintf(stderr, "Happy birthday to you\n");
    fprintf(stderr, "Happy birthdy to you\n");
    sleep(7);
    fprintf(stderr, "Happy birthday dear %s\n", name);
    fprintf(stderr, "Happy birthday to you\n");
}

int main(){

    struct sigaction sa;
    sa.sa_handler = sing;
    sa.sa_flags = 0;
    if(sigemptyset(&sa.sa_mask) == -1){
        perror("sigemptyset");

    }

    if (sigaction(SIGUSR1, &sa, NULL) == -1){
        perror("sigaction");
        exit(1);
    }

    int i = 1;
    while(i != 0){
        printf("%d\n", i);
        i++;
    }
}