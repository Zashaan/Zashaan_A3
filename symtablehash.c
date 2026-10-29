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

/* creates the struct binding that will be used to actually store each 
   key and value pair in each binding with a pointer to the next binding
   as well */
struct Binding {

    /* creates the variable that stores the pointer to the key */
    char *key;

    /* creates the variable with a pointer to the value */
    const void *value;

    /* creates the variable for the pointer to the next binding */
    struct Binding *next;
};

/* creates the struct SymTable that has the actual table, its number of
   buckets, length, and also all of the buckets */
struct SymTable {

    /* variable to store the array of all the buckets */
    struct Binding **buckets;

    /* variable to store how many buckets there are */
    size_t numBuckets;

    /* variable to store the length of the table */
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
   needed else does nothing */
static void SymTable_resize(SymTable_T oSymTable) {

    /* creates the array to be used to look at sizes */
    static const size_t bucketSizes[] = {509, 1021, 2039, 4093, 
        8191, 16381, 32749, 65521};
    
    /* stores the array length */
    const size_t sizeNumber = 
        sizeof(bucketSizes) / sizeof(bucketSizes[0]);

    /* variables to keep track of locations and counts */
    size_t oldBucket;
    size_t newBucket;
    size_t newBucketCount;
    size_t i;

    /* creates the new bucket array and variables to populate */
    struct Binding **newBuckets;
    struct Binding *currentBinding;
    struct Binding *nextBinding;

    /* assert that the table is not null */
    assert(oSymTable != NULL);

    /* return if resizing is not needed */
    if (oSymTable->length <= oSymTable->numBuckets) return;

    /* find how many buckets there are right now */
    for (i = 0; i < sizeNumber; i++) {
        if (bucketSizes[i] == oSymTable->numBuckets) break;
    }

    /* check if the max is already reached */
    if (i + 1 == sizeNumber) return;

    /* assigns the new count for buckets */
    newBucketCount = bucketSizes[i];

    /* give memory to the new buckets array */
    newBuckets = calloc(newBucketCount, sizeof(*newBuckets));

    /* check if actually assigned */
    if (newBuckets == NULL) return;

    /* loop through all the old buckets and copy each binding in 
       newBuckets */
    for (oldBucket = 0; oldBucket < oSymTable->numBuckets; 
        oldBucket++) {

            /* starts with the first binding in each bucket */
            currentBinding = oSymTable->buckets[oldBucket];

            /* loops through the bucket and moves the bindings */
            while (currentBinding != NULL) {
                /* store the next pointer */
                nextBinding = currentBinding->next;

                /* calculate which bucket it should go into */
                newBucket = SymTable_hash(currentBinding->key, 
                    newBucketCount);
                
                /* update the next for this binding and put it into the
                   correct new bucket */
                currentBinding->next = newBuckets[newBucket];
                newBuckets[newBucket] = currentBinding;

                /* go to the next binding in the bucket */
                currentBinding = nextBinding;
            }
        };

    /* free the old bucket array */
    free(oSymTable->buckets);

    /* update the table with the new buckets */
    oSymTable->buckets = newBuckets;
    oSymTable->numBuckets = newBucketCount;
}

/*--------------------------------------------------------------------*/

/* hash table implementation for creating a new SymTable_T */
SymTable_T SymTable_new(void) {

    /* create the table and allocate memory to it*/
    SymTable_T newTable;
    newTable = malloc(sizeof(*newTable));

    /* if no memory is allocated then return null */
    if (newTable == NULL) return NULL;
    
    /* give memory to the bucket array */
    newTable->buckets = calloc(509, sizeof(*newTable->buckets));

    /* check if memory given to bucket array */
    if (newTable->buckets == NULL) {
        free(newTable);
        return NULL;
    }
    
    /* set values for the numBuckets and length variables */
    newTable->numBuckets = 509;
    newTable->length = 0;

    /* returns the new table */
    return newTable;
}

/*--------------------------------------------------------------------*/

/* hash table implementation for freeing the table */
void SymTable_free(SymTable_T oSymTable) {

    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;
    struct Binding *nextBinding;
    size_t bucketNumber;

    /* assert that the table is not null */
    assert(oSymTable != NULL);

    /* starts bucket number at 0 */
    bucketNumber = 0;

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

    /* frees the bucket array and table itself */
    free(oSymTable->buckets);
    free(oSymTable);
}

/*--------------------------------------------------------------------*/

/* hash table implementation for getting the length of the table */
size_t SymTable_getLength(SymTable_T oSymTable) {

    /* assert that oSymTable is not empty */
    assert(oSymTable != NULL);

    /* returns the length of the table passed in */
    return oSymTable->length;
}

/*--------------------------------------------------------------------*/

/* hash table implementation for putting a pair in the table */
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

    /* resizes the table if needed */
    if (oSymTable->length > oSymTable->numBuckets) {
        SymTable_resize(oSymTable);
    }

    /* returns 1 to say that it has been added */
    return 1;
    }

/*--------------------------------------------------------------------*/

/* hash table implementation for replacing a pair in the table */
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

/* hash table implementation for checking if the key is in the table */
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

/* hash table implementation for getting the value of the key in the 
   table */
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

/* hash table implementation for removing a pair from the table */
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

/* hash table implementation for applying the function to the table */
void SymTable_map(SymTable_T oSymTable, 
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {

    /* creates the variables needed to go through the loop */
    struct Binding *currentBinding;
    size_t bucketNumber;

    /* assert that the table and the key is not null */
    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    /* starts bucket number at 0 */
    bucketNumber = 0;

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
