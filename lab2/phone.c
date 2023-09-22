#include <stdio.h>
#include <stdlib.h>
/*
 * Sample usage:
 * $ gcc -Wall -std=gnu99 -g -o phone phone.c
 * $ ./phone 4161234567 3
 * 1
 * $ ./phone 4161234567 -1
 * 4161234567
 * $ ./phone 4161234567 9
 * 7
 * $ ./phone 4161234567 10 
 * ERROR
 */
int main(int argc, char **argv) {
    

    int integer;
    char phone[11];

    scanf("\n%s %d", phone, &integer);
    // getchar();
    if (integer == -1){
        printf("%s\n", phone);
        return 0;
    }else if(integer >= 0 && integer <= 9){
        printf("%c\n", phone[integer]);
        return 0;
    }else if (integer <= -1 | integer >= 9){
        printf("ERROR\n");
        return 1;

    }

    
    // // Read in the command-line arguments and convert the strings to doubles
    // double principal = strtod(argv[1], NULL);
    // double rate = strtod(argv[2], NULL);

    // // Call invest to make you more money
    // invest(&principal, rate);

    // printf("%.2f\n", principal);
    return 0;

}

