#include <stdio.h>
#include <stdlib.h>
/*
 * Sample usage:
 * $ gcc -Wall -std=gnu99 -g -o phone_loop phone_loop.c
 * $ ./phone_loop 
 * 4147891234
 * 3
 * 7
 * 6
 * 1
 * -1
 * 4147891234
 * 0
 * 4
 * 11
 * ERROR
 */
int main(int argc, char **argv) {
    

    int integer = 0;
    char phone[11];

    scanf("\n%s", phone);
    while (scanf("\n%d", &integer) != EOF){
    // scanf("\n%d", &integer);
    if (integer == -1){
        printf("%s\n", phone);
    }else if((integer >= 0) && (integer <= 9)){
        printf("%c\n", phone[integer]);
    }else if ((integer <= -1 | integer >= 9)){
        printf("ERROR\n");
        return 1;
    }
    }
    
    // // Read in the command-line arguments and convert the strings to doubles
    // double principal = strtod(argv[1], NULL);
    // double rate = strtod(argv[2], NULL);

    // // Call invest to make you more money
    // invest(&principal, rate);

    // printf("%.2f\n", principal);
    return 0;

}

