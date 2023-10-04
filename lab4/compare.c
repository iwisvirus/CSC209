#include <stdio.h>
#include <string.h>

/*
    Write the main() function of a program that takes exactly two arguments,
    and prints one of the following:
        - "Same\n" if the arguments are the same.
        - "Different\n" if the arguments are different.
        - "Invalid\n" if the program is called with an incorrect number of
          arguments.

    NOTE: Cut and paste these strings into your printf statements to
    ensure that they will be exactly the strings above.

    Your main function should return 0, regardless of what's printed.
*/
int main(int argc, char **argv){

    char *firststring = argv[1];
    char *secondstring = argv[2];
    int compare;
    int i;

    // checking argument numbers 
    if(argc != 3){
        fprintf(stderr, "Invalid\n");
        return 1;
    }

    // checking longer string 
    if (strlen(firststring) > strlen(secondstring)){
        compare = strlen(firststring);
    }else{
        compare = strlen(secondstring);
    }
    
    for (i = 0; i < compare; i++) {
        if (firststring[i] != secondstring[i]){
            printf("Different\n");
            return 0;
        }
    }
    printf("same\n");

    // if(firststring == secondstring){
    //     printf("Same\n");
    // }else if(firststring != secondstring){
    //     printf("Different\n");
    // }

    return 0;

}