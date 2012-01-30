#ifndef p1_hash_h
#define p1_hash_h

#include "hash.h"

typedef struct char_node {
    char *data;
    struct char_node * next;
} char_node;

typedef struct hash_element {
    char *str;
    int count;
    int case_sensitive_occurances;
    char_node * occurances;
} hash_element;

typedef struct node {
    hash_element *data;
    struct node * next;
} node;

typedef struct hash_table {
    int size;
    int key_num;
    int key_alloc;
    node ** storage;
} hash_table;

/* hash_element helpers */
hash_element* new_hash_element(char *str);
int hash_element_add_occurance(hash_element *element, char *str);

/* hash table helpers */
void sort_strings(char ** arr, int size);
void print_hash_keys_in_lexicongraphical_order(hash_table *table);
void hash_table_free(hash_table * table);

/* hash table functions */
int lua_hash(char *str);
hash_table* new_hash_table(int size);
void hash_table_store(hash_table* table, char *str, hash_element* val);
hash_element* hash_table_get(hash_table* table, char *str);
char** hash_table_get_all_keys(hash_table *table);

#endif
