#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include "hash.h"
#include "util.h"

#define CHUNK_SIZE 1024
#define TABLE_SIZE 1024
#define XCODE_TEST

int main (int argc, const char * argv[])
{
#ifndef XCODE_TEST
    if(argc == 0 || strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }
#endif

    hash_table* table = new_hash_table(TABLE_SIZE);

    FILE *inFile;
#ifndef XCODE_TEST
    inFile = fopen(argv[1], "r");
#else
    inFile = fopen("/Users/artem/code/c/p1/p1/test.txt", "r");
#endif
    if(!inFile) {
        printf("couldn't open \"%s\"\nmaybe it doesn't exist\n", argv[1]);
        return 0;
    }

    fseek(inFile, 0, SEEK_END);
    int file_size = ftell(inFile), consumed = 0;
    rewind(inFile);

    char * buffer = (char *)calloc(CHUNK_SIZE, sizeof(char));
    char * leftover = (char *)calloc(CHUNK_SIZE * 2, sizeof(char));
    char * leftover_start = leftover;
    int leftover_size = CHUNK_SIZE * 2 * sizeof(char);
    char *last_word;
    if(buffer == NULL) fputs("could not allocate memory for buffer", stderr);
    
    regex_t regex;
	regmatch_t pm;
	int error;
    
    error = regcomp(&regex, "([[:alpha:]][[:alnum:]]*)", REG_EXTENDED);
	if( error ){ fprintf(stderr, "Could not compile regex\n"); return 1; }
    

    while(consumed < file_size) {
        fread(buffer, sizeof(char), CHUNK_SIZE, inFile);
        if(buffer == NULL) {
            printf("buffer == NULL, consumed: %i, file_size: %i\n", consumed, file_size);
            return 1;
        }
        
        strcat(leftover, buffer);
        
        error = regexec(&regex, leftover, 1, &pm, 0);
        while( error == 0 ) {
            char *word = (char *)malloc( sizeof(char) * ((int)pm.rm_eo - (int)pm.rm_so + 2) );
            strlcpy(word, leftover + pm.rm_so, sizeof(char) * ((int)pm.rm_eo - (int)pm.rm_so + 1));
            
            char *lowered = (char *)malloc(strlen(word) * sizeof(char) + 1);
            strcpy(lowered, word);
            strtolower(lowered);
            
            hash_element *tmp = hash_table_get(table, lowered);
            if(tmp) {
                hash_element_add_occurance(tmp, word);
            } else {
                hash_table_store(table, lowered, new_hash_element(word));
            }
            
            error = regexec(&regex, leftover += pm.rm_eo , 1, &pm, REG_NOTBOL);
            last_word = word;
            free(lowered);
            free(word);
        }
        
        leftover = leftover_start;
        strlcpy(leftover, last_word, sizeof(char) * (strlen(last_word) + 1));
        
        consumed += sizeof(char) * CHUNK_SIZE;
    }
    regfree(&regex);
    free(buffer);
    free(leftover);
    fclose(inFile);

    print_hash_keys_in_lexicongraphical_order(table);

    hash_table_free(table);

    return 0;
}
