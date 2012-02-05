#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include "hash.h"
#include "util.h"

#define CHUNK_SIZE 4096
#define TABLE_SIZE 1024

void print_help() {
    printf("wordstat\nusage:\n\twordstat <file>\t\t<file> is the path to the file which is to be parsed\n");
}

int main (int argc, const char * argv[])
{
    if(argc == 0 || strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }

    hash_table* table = new_hash_table(TABLE_SIZE);

    FILE *inFile;
    inFile = fopen(argv[1], "r");
    if(!inFile) {
        printf("couldn't open \"%s\"\nmaybe it doesn't exist\n", argv[1]);
        return 0;
    }

    fseek(inFile, 0, SEEK_END);
    int file_size = ftell(inFile), consumed = 0;
    rewind(inFile);

    char * buffer = calloc(CHUNK_SIZE, sizeof(char));
    char * leftover = calloc(CHUNK_SIZE, sizeof(char));
    int size_of_leftover = 0;
    if(buffer == NULL) fputs("could not allocate memory for buffer", stderr);

    while(consumed < file_size) {
        fread(buffer, sizeof(char), CHUNK_SIZE, inFile);
        if(buffer == NULL) {
            printf("buffer == NULL, consumed: %i, file_size: %i\n", consumed, file_size);
            return 1;
        }
        
        /* lets look for the last word*/
        int i = CHUNK_SIZE-1;
        while(buffer[i] >= '0' && buffer[i] <= '9' && 
           buffer[i] >= 'A' && buffer[i] <= 'Z' &&
           buffer[i] >= 'a' && buffer[i] <= 'z') i--;

        if(CHUNK_SIZE - size_of_leftover < i) {
            /* MORE LEFTOVER SPACE */
        }
        
        char *word, *brkt;
        char *sep = " ,./<?>;':\"[]{}\\|`-=~!@#$%^&*()_+";
        for(word = strtok_r(buffer, sep, &brkt); word; word = strtok_r(NULL, sep, &brkt)) {
            word = remove_numbers_from_front(word);
            
            /* because the tolower function is a queen */
            char *lowered = (char *)malloc(strlen(word) * sizeof(char) + 1);
            strcpy(lowered, word);
            strtolower(lowered);

            hash_element *tmp = hash_table_get(table, lowered);
            if(tmp) {
                hash_element_add_occurance(tmp, word);
            } else {
                hash_table_store(table, lowered, new_hash_element(word));
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
