#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "util.h"

#define cmp(a, b) printf("[%s:%s] %i\n", a, b, strcmp(a, b));

/*
    HASH ELEMENT HELPERS
 */

/*  func: new_hash_element(str)

    a helper function which takes in a string, and returns a pointer to
    a hash_element struct. SUPER convenient.
 */

hash_element* new_hash_element(char *str) {
    hash_element *tmp = (hash_element *)malloc(sizeof(hash_element));
    tmp->str = (char *)malloc(strlen(str) * sizeof(char) + 1);
    char *lowered_str = (char *)malloc(strlen(str) * sizeof(char) + 1);
    strcpy(lowered_str, str);
    strtolower(lowered_str);
    
    strcpy(tmp->str, lowered_str);
    tmp->count = 1;
    tmp->case_sensitive_occurances = 1;
    tmp->occurances = (char_node *)malloc(sizeof(char_node));
    tmp->occurances->next = NULL;
    tmp->occurances->data = (char *)malloc(strlen(str) * sizeof(char) + 1);
    strcpy(tmp->occurances->data, str);
    if(!tmp->occurances) {
        fprintf(stderr, "no memory while allocating occurances[]\n");
        return NULL;
    }
    return tmp;
}

void free_hash_element(hash_element *element) {
    char_node *head = element->occurances;

    while(head != NULL) {
        char_node *tmp = head->next;
        free(head->data);
        free(head);
        head = tmp;
    }
    free(element);
}

/*  func: hash_element_add_occurance(element, str)

    if str == element->str then it's just another occurance since
    str is already in the case-sensitive occurances** array. Otherwise
    an existing copy of str is sought in occurances** and if it's not
    found, it is added. count of the str is incrimented in all cases.
 */
int hash_element_add_occurance(hash_element *element, char *str) {
    if(element->str == str) {
        element->count += 1;
        return 1;
    } else {
        char_node *tmp = element->occurances;
        while(tmp != NULL) {
            if(strcmp(tmp->data, str) == 0) {
                element->count += 1;
                tmp = NULL;
                return 1;
            }
            tmp = tmp->next;
        }
        /* this is a new case-sensitive version of str */
        element->count += 1;
        element->case_sensitive_occurances += 1;

        char_node *head = (char_node *)malloc(sizeof(char_node));
        head->next = element->occurances;
        head->data = (char *)malloc(strlen(str) * sizeof(char) + 1);
        strcpy(head->data, str);
        element->occurances = head;

        return 1;
    }
    return 0;
}

/*
    HASH TABLE HELPERS
 */

/*  func: new_hash_table(size)

    helper function which allocates and initializes a hash_table for
    future use. Returns a pointer to that hash_table.
 */
hash_table* new_hash_table(int size) {
    hash_table *table = (hash_table *)malloc(sizeof(hash_table));

    if(!table) {
        fprintf(stderr, "no memory while allocating hash_table\n");
        return NULL;
    }

    table->key_num = size;
    table->key_alloc = 0;
    table->storage = (node **)calloc(1, size * sizeof(node *));

    if(table->storage == NULL) {
        fprintf(stderr, "no memory while allocating table->store\n");
        free(table);
        return NULL;
    }

    return table;
}

/*  func: sort_strings(arr, size)

    sort <size> strings in <arr> in lexicongraphical order.
 */
void sort_strings(char ** arr, int size) {
    int i;
    for(i = 0; i < size; i++) {
        int lowest = i;

        int k;
        for(k = i; k < size; k++)
            if(strcmp(arr[k], arr[lowest])<0) lowest = k;    

        char * tmp = arr[i];
        arr[i] = arr[lowest];
        arr[lowest] = tmp;
    }
}

/*  func: print_hash_keys_in_lexicongraphical_order(table)

    print out all of the hash_elements in "alphabetical" order and display
    their number of occurances, and number of case-sensitive versions;
 */
void print_hash_keys_in_lexicongraphical_order(hash_table *table){
    char **arr = hash_table_get_all_keys(table);
    sort_strings(arr, table->key_alloc);

    int i;
    for(i = 0; i < table->key_alloc; i++) {
        hash_element *tmp = hash_table_get(table, arr[i]);
        fprintf(stdout,"%s", tmp->str);
        int k;
        const int num = 6 - strlen(tmp->str) / 4;
        for(k = 0; k < num; k++) fprintf(stdout,"\t");

        fprintf(stdout,"%i\t%i\n", tmp->count, tmp->case_sensitive_occurances);
        tmp = NULL;;
    }

    for(i = 0; i < table->key_alloc; i++)
        free(arr[i]);
    free(arr);
}

/*
    HASH TABLE 
 */

/*  func: lua_hash(str)

    function takes in a string and returns a positive int hash.
 */
int lua_hash(char *str) {
    int l = strlen(str);
    int i, step = ((l >> 5) + 1);
    int h = l + (l >= 4?*(int*)str:0);
    for( i=l; i >= step; i -= step) {
        h = h^(( h<<5 ) + (h >> 2) + ((unsigned char *)str)[i-1]);
    }

    /* abs(h) is a modification on the original lua hash
       to make it work better with my method of determining a
       table index using lua_hash%table->key_num */
    return abs(h);
}


/*  func: hash_table_store(table, str, element)

    function stores a hash_element in the hash_table table under the key str.
    if there are collisions under the key, it will append the element
    to the linked list at that location.

        Example:
            hash_table *table = new_hash_table(16);
            hash_element *tmp = new_hash_element("example");
            hash_table_store(table, tmp->str, tmp);
 */
void hash_table_store(hash_table* table, char *str, hash_element* val) {
    int hash = lua_hash(str);

    /* create a linked list node, point it to the head of the index */
    node * head = (node *)malloc(sizeof(node));
    head->data = val;
    if(table->storage[hash%table->key_num] != NULL) {
        head->next = table->storage[hash%table->key_num];
    } else
        head->next = NULL;

    table->key_alloc += 1;
    table->storage[hash%table->key_num] = head;
}


/*  func: hash_table_get(table, str)

    function takes a hash_table pointer and a string and returns
    a pointer to the hash_element in the table. It will *always* 
    return a pointer to the right hash_element, even if there are
    multiple collisions under the hash of str.

    suitable for modifying hash_elements;
 */
hash_element* hash_table_get(hash_table* table, char *str) {
    int hash = lua_hash(str);
    if(table->storage[hash%table->key_num] == NULL) {
        return NULL;
    } else {
        /* get the first node*/
        node *tmp = (node *)table->storage[hash%table->key_num];
        if(!tmp->next) /* if it has no followers, it must be the only node in the list */
            return tmp->data;

        /* oh boy, there are more than 1 nodes in this linked list, one of them must be it, since this is a hash index with collisions on str */
        while((tmp = tmp->next) != NULL)
            if(strcmp(table->storage[hash%table->key_num]->data->str, str)==0)
                return table->storage[hash%table->key_num]->data;
    }
    return NULL;
}

int hash_table_remove(hash_table *table, char *str) {
    int hash = lua_hash(str);
    if(table->storage[hash%table->key_num]) {
        node *head = table->storage[hash%table->key_num], *prev = NULL;
        while(head != NULL) {
            if(strcmp(head->data->str, str) == 0) {
                if(head->next == NULL) {
                    free_hash_element(head->data);
                    free(head);
                } else if(prev) {
                    prev->next = head->next;
                    free_hash_element(head->data);
                    free(head);
                    table->storage[hash%table->key_num] = prev;
                } else {
                    free_hash_element(head->data);
                    free(head);
                    table->storage[hash%table->key_num] = NULL;
                }
                return 0;
            }
            prev = head;
            head = head->next;
        }
        return 0;
    } else {
        return 1;
    }
}

/*  func: hash_table_get_all_keys(table)

    function takes a hash_table pointer and returns an array of
    hash_element pointers (hash_element **). 
 */
char ** hash_table_get_all_keys(hash_table *table) {
    char ** arr = (char **)calloc(1, table->key_alloc * sizeof(char *));

    int i, arr_i = -1;
    for(i = 0; i < table->key_num; i++) {
        if(table->storage[i]) {
            node *curr_node = table->storage[i];
            arr_i += 1;
            arr[arr_i] = curr_node->data->str;
            curr_node = curr_node->next;

            while(curr_node != NULL) {
                arr_i += 1;
                arr[arr_i] = curr_node->data->str;
                curr_node = curr_node->next;
            }
        }
    }

    return arr;
}

void hash_table_free(hash_table *table) {
    int i;
    for(i = 0; i < table->key_num; i++) {
        if(table->storage[i] != NULL) {
            node *head = table->storage[i];
            while(head != NULL) {
                node *next_node = head->next;
                free_hash_element(head->data);
                free(head);
                head = next_node;
            }
        }
    }
    free(table->storage);
    free(table);
}
