#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

#define CHUNK_SIZE 1024

char* strtolower(char* string) {
    int q;
    for(q = 0; q < strlen(string); q++) {
        string[q] = tolower(string[q]);
    }
    return string;
}

int main (int argc, const char * argv[])
{    
    /*if(argc == 0 || strcmp(argv[0], "-h")) {
        print_help();
        return 0;
    }*/
    
    hash_table* table = new_hash_table(512);
    
    FILE *inFile;
    inFile = fopen("/Users/artem/code/c/p1/p1/test.txt", "r");
    if(!inFile) {
        printf("couldn't open \"%s\"\nmaybe it doesn't exist\n", argv[0]);
        return 0;
    }
    
    fseek(inFile, 0, SEEK_END);
    int file_size = ftell(inFile), consumed = 0;
    rewind(inFile);
    
    char * buffer = calloc(CHUNK_SIZE, sizeof(char));
    char * leftover = calloc(CHUNK_SIZE, sizeof(char));
    if(buffer == NULL) fputs("could not allocate memory for buffer", stderr);
    
    while(consumed < file_size) {
        fread(buffer, sizeof(char), CHUNK_SIZE, inFile);
        if(buffer == NULL) {
            printf("buffer == NULL, consumed: %i, file_size: %i\n", consumed, file_size);
            return 1;
        }

        char *word, *brkt;
        char *sep = " ,./<?>;':\"[]{}\\|`-=~!@#$%^&*()_+";
        for(word = strtok_r(buffer, sep, &brkt); word; word = strtok_r(NULL, sep, &brkt)) {
            /* because the tolower function is a queen */
            char *lowered = (char *)malloc(strlen(word) * sizeof(char) + 1);
            strcpy(lowered, word);
            strtolower(lowered);
            
            hash_element *tmp = hash_table_get(table, lowered);
            if(tmp) {
                hash_element_add_occurance(tmp, word);
            } else {
                tmp = new_hash_element(word);
                hash_table_store(table, lowered, tmp);
                free(tmp);
            }
            free(lowered);
        }        
        consumed += sizeof(char) * CHUNK_SIZE;
    }
    free(buffer);
    fclose(inFile);
    
    print_hash_keys_in_lexicongraphical_order(table);
    
    hash_table_free(table);
    
    return 0;
}
