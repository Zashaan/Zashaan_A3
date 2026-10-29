/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Zashaan Shaik                                              */
/*--------------------------------------------------------------------*/
#include "symtable.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
/*--------------------------------------------------------------------*/
/* the code below is the linked list implementation for the SymTable  */
/*--------------------------------------------------------------------*/

/* creates the binding and struct code */
struct Binding {
    char *key;
    const void *value;
    struct Binding *next;
};
struct SymTable {
    struct Binding *first;
    size_t length;
};

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void) {

    /* create the table and allocate memory to it*/
    SymTable_T newTable;
    newTable = malloc(sizeof(*newTable));

    /* if no memory is allocated then return null */
    if (newTable == NULL) return NULL;

    /* initializes the new table values */
    newTable->first = NULL;
    newTable->length = 0;

    /* returns the new table */
    return newTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable) {

    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;
    struct Binding *nextBinding;

    /* assert that the table is not null */
    assert(oSymTable != NULL);

    /* assigns the current binding */
    currentBinding = oSymTable->first;

    /* loops through oSymTable and frees memory as it goes */
    while (currentBinding != NULL) {
        nextBinding = currentBinding->next;
        free(currentBinding->key);
        free(currentBinding);
        currentBinding = nextBinding;
    }

    /* frees the table itself */
    free(oSymTable);
}

/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable) {

    /* assert that oSymTable is not empty */
    assert(oSymTable != NULL);

    /* returns the length of the table passed in */
    return oSymTable->length;
}

/*--------------------------------------------------------------------*/

int SymTable_put(SymTable_T oSymTable, 
    const char *pcKey, const void *pvValue) {

        /* creates the variables needed to go through put function */
        struct Binding *currentBinding;
        struct Binding *newBinding;
        char *currentKey;

        /* assert that the table and the key is not null */
        assert(oSymTable != NULL);
        assert(pcKey != NULL);

        /* assigns the current binding */
        currentBinding = oSymTable->first;

        /* loops through oSymTable and first checks if the key is in 
           the table */
        while (currentBinding != NULL) {
            currentKey = currentBinding->key;
            
            if (strcmp(currentKey, pcKey) == 0) return 0;

            currentBinding = currentBinding->next;
        }

        /* now that it is not in the table already, add it */
        /* allocate memory */
        newBinding = malloc(sizeof(*newBinding));

        /* check if null memory then return 0 */
        if (newBinding == NULL) return 0;

        /* assign memory for the copy of the key */
        currentKey = malloc(strlen(pcKey) + 1);

        /* check if actually assigned or free memory and return 0 */
        if (currentKey == NULL) {
            free(newBinding);
            return 0;
        }

        /* copy over the key passes in into current key */
        strcpy(currentKey, pcKey);

        /* actually does the proper assigns to update the new binding */
        newBinding->key = currentKey;
        newBinding->value = pvValue;

        /* adds the new binding to the start of the table */
        newBinding->next = oSymTable->first;
        oSymTable->first = newBinding;

        /* updates the length */
        oSymTable->length++;

        /* returns 1 to say that it has been added */
        return 1;
    }

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue) {

    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;
    char *currentKey;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* assigns the current binding */
    currentBinding = oSymTable->first;

    /* loops through oSymTable to find the key replace the value */
    while (currentBinding != NULL) {
        currentKey = currentBinding->key;
        if (strcmp(currentKey, pcKey) == 0) {
            size_t currentValue = currentBinding->value;
            currentBinding->value = pvValue;
            return (void *)currentValue;
        }

        currentBinding = currentBinding->next;
    }

    /* if been through the table and key not found return NULL */
    return NULL;
     }

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {

    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;
    char *currentKey;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* assigns the current binding */
    currentBinding = oSymTable->first;

    /* loops through oSymTable and first checks if the key is in 
       the table */
    while (currentBinding != NULL) {
        currentKey = currentBinding->key;
        if (strcmp(currentKey, pcKey) == 0) return 1;

        currentBinding = currentBinding->next;
    }

    /* if been through the table and key not found return 0 */
    return 0;
}

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    
    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;
    char *currentKey;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* assigns the current binding */
    currentBinding = oSymTable->first;

    /* loops through oSymTable to find the key and return the value */
    while (currentBinding != NULL) {
        currentKey = currentBinding->key;
        if (strcmp(currentKey, pcKey) == 0) {
            return (void *)currentBinding->value;
        }

        currentBinding = currentBinding->next;
    }

    /* if been through the table and key not found return NULL */
    return NULL;
}

/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    
    /* creates the variables needed to go through the loop */
    struct Binding *previousBinding;
    struct Binding *currentBinding;
    char *currentKey;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* assigns the current binding */
    previousBinding = oSymTable->first;
    currentBinding = previousBinding->next;

    /* assigns the correct value for current key */
    currentKey = previousBinding->key;

    /* handles the case where the first element is removed */
    if (strcmp(currentKey, pcKey) == 0) {
        oSymTable->first = currentBinding;
        size_t previousBindingValue = previousBinding->value;
        free(previousBinding->key);
        free(previousBinding);
        return previousBindingValue;
    }

    /* loops through oSymTable to find the key and remove it */
    while (currentBinding != NULL) {
        currentKey = currentBinding->key;
        if (strcmp(currentKey, pcKey) == 0) {
            previousBinding->next = currentBinding->next;
            size_t currentBindingValue = currentBinding->value;
            free(currentBinding->key);
            free(currentBinding);
            return currentBindingValue;
        }

        previousBinding = currentBinding;
        currentBinding = currentBinding->next;
    }

    /* if been through the table and key not found return NULL */
    return NULL;
}

/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable, 
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {
    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    /* sets the value for current binding */
    currentBinding = oSymTable->first;

    /* loops through and does the function passed in for each binding */
    while (currentBinding != NULL) {
        (*pfApply)(currentBinding->key, 
            (void *) currentBinding->value, (void *)pvExtra);

        currentBinding = currentBinding->next;
    }
}

/*--------------------------------------------------------------------*/
