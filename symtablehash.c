/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Zashaan Shaik                                              */
/*--------------------------------------------------------------------*/
#include "symtable.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
/*--------------------------------------------------------------------*/
/* the code below is the hash table implementation for the SymTable   */
/*--------------------------------------------------------------------*/

/* creates the binding and struct code */
struct Binding {
    char *key;
    const void *value;
    struct Binding *next;
};
struct SymTable {
    struct Binding **buckets;
    size_t numBuckets;
    size_t length;
};

/*--------------------------------------------------------------------*/

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}

/*--------------------------------------------------------------------*/

/* checks the current length of oSymTable and resizes the table if 
   needed else does nothing*/
static void SymTable_resize(SymTable_T oSymTable) {

    /* assert that the table is not null */
    assert(oSymTable != NULL);


}

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void) {

    /* create the table and allocate memory to it*/
    SymTable_T newTable;
    newTable = calloc(509, sizeof(*newTable->buckets));

    /* if no memory is allocated then return null */
    if (newTable == NULL) return NULL;

    /* set values for the numBuckets and length variables */
    newTable->numBuckets = 509;
    newTable->length = 0;

    /* returns the new table */
    return newTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable) {

    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;
    struct Binding *nextBinding;
    size_t bucketNumber;

    /* assert that the table is not null */
    assert(oSymTable != NULL);

    /* outer loop to go through each bucket */
    while (bucketNumber < oSymTable->numBuckets) {
        /* assigns current binding as the first binding in the fresh 
           bucket */
        currentBinding = oSymTable->buckets[bucketNumber];

        /* loops through the bucket and frees memory as it goes */
        while (currentBinding != NULL) {
            nextBinding = currentBinding->next;
            free(currentBinding->key);
            free(currentBinding);
            currentBinding = nextBinding;
        }

        /* moves onto the next bucket */
        bucketNumber++;
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
    size_t bucketNumber;

    /* assert that the table, key, and value is not null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    assert(pvValue != NULL);

    /* calculates the bucket number for the key */
    bucketNumber = SymTable_hash(pcKey, oSymTable->numBuckets);

    /* assigns the current binding */
    currentBinding = oSymTable->buckets[bucketNumber];

    /* loops through bucket and first checks if the key is in 
        the table */
    while (currentBinding != NULL) {
        currentKey = currentBinding->key;
        
        if (strcmp(currentKey, pcKey) == 0) return 0;

        currentBinding = currentBinding->next;
    }

    /* now that it is not in the bucket already, add it */
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
    newBinding->next = oSymTable->buckets[bucketNumber];
    oSymTable->buckets[bucketNumber] = newBinding;

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
    size_t bucketNumber;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* calculates the bucket number for the key */
    bucketNumber = SymTable_hash(pcKey, oSymTable->numBuckets);

    /* assigns the current binding */
    currentBinding = oSymTable->buckets[bucketNumber];

    /* loops through the bucket to find the key replace the value */
    while (currentBinding != NULL) {
        currentKey = currentBinding->key;
        
        if (strcmp(currentKey, pcKey) == 0) {
            const void *currentValue = currentBinding->value;
            currentBinding->value = pvValue;
            return (void *)currentValue;
        }

        currentBinding = currentBinding->next;
    }

    /* if been through the bucket and key not found return NULL */
    return NULL;
    }

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {

    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;
    char *currentKey;
    size_t bucketNumber;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* calculates the bucket number for the key */
    bucketNumber = SymTable_hash(pcKey, oSymTable->numBuckets);

    /* assigns the current binding */
    currentBinding = oSymTable->buckets[bucketNumber];

    /* loops through the bucket to search for the key and return the 
       value */
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
    size_t bucketNumber;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* calculates the bucket number for the key */
    bucketNumber = SymTable_hash(pcKey, oSymTable->numBuckets);

    /* assigns the current binding */
    currentBinding = oSymTable->buckets[bucketNumber];

    /* loops through the bucket to find the key and return the value */
    while (currentBinding != NULL) {
        currentKey = currentBinding->key;
        if (strcmp(currentKey, pcKey) == 0) {
            return (void *)currentBinding->value;
        }

        currentBinding = currentBinding->next;
    }

    /* if been through the bucket and key not found return NULL */
    return NULL;
}

/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {

    /* creates the variables needed to go through the loop */
    struct Binding *previousBinding;
    struct Binding *currentBinding;
    char *currentKey;
    const void *currentBindingValue;
    size_t bucketNumber;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* calculates the bucket number for the key */
    bucketNumber = SymTable_hash(pcKey, oSymTable->numBuckets);

    /* assigns the previous binding */
    previousBinding = oSymTable->buckets[bucketNumber];

    /* check to see that bucket is not empty */
    if (previousBinding == NULL) return NULL;

    /* assigns current binding if table not empty */
    currentBinding = previousBinding->next;

    /* assigns the correct value for current key */
    currentKey = previousBinding->key;

    /* handles the case where the first element is removed */
    if (strcmp(currentKey, pcKey) == 0) {
        oSymTable->buckets[bucketNumber] = currentBinding;
        currentBindingValue = previousBinding->value;
        free(previousBinding->key);
        free(previousBinding);
        oSymTable->length--;
        return (void *)currentBindingValue;
    }

    /* loops through the bucket to find the key and remove it */
    while (currentBinding != NULL) {
        currentKey = currentBinding->key;
        if (strcmp(currentKey, pcKey) == 0) {
            previousBinding->next = currentBinding->next;
            currentBindingValue = currentBinding->value;
            free(currentBinding->key);
            free(currentBinding);
            oSymTable->length--;
            return (void *)currentBindingValue;
        }

        previousBinding = currentBinding;
        currentBinding = currentBinding->next;
    }

    /* if been through the bucket and key not found return NULL */
    return NULL;
}

/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable, 
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {

    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;
    size_t bucketNumber;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    /* outer loop to go through each bucket */
    while (bucketNumber < oSymTable->numBuckets) {
        /* assigns current binding as the first binding in the fresh 
           bucket */
        currentBinding = oSymTable->buckets[bucketNumber];

        /* loops through and does the function passed in for each binding 
        in the current bucket */
        while (currentBinding != NULL) {
            (*pfApply)(currentBinding->key, 
                (void *) currentBinding->value, (void *)pvExtra);

            currentBinding = currentBinding->next;
        }

        /* moves onto the next bucket */
        bucketNumber++;
    }  
}

/*--------------------------------------------------------------------*/
