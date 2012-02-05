#include <stdio.h>
#include <string.h>
#include "util.h"

/*  func: strtolower(string)
 
 convert string into lower case.
 /!/
 string is modified! Malloc a new string.
 /!/
 */
char *strtolower(char* string) {
    int q;
    for(q = 0; q < strlen(string); q++) {
        string[q] = tolower(string[q]);
    }
    return string;
}

/*  func: remove_numbers_from_front(string)
 
 removes numbers from the front of words since 123alpha == alpha
 */
char * remove_numbers_from_front(char *string) {
    int i = 0;
    while(string[i] <= '9' && string[i] >= '0') i++;
    memcpy(string, string + i, sizeof(char) * (strlen(string) - i));
    int k;
    for(k = strlen(string) - i; k < strlen(string); k++) string[k] = '\0';
    return string;
}

void print_help(void) {
    printf("wordstat\nusage:\n\twordstat <file>\t\t<file> is the path to the file which is to be parsed\n");
}