/*--------------------------------------------------------------------*/
/* symtable.h.                                                        */
/* Author: Zashaan Shaik                                              */
/*--------------------------------------------------------------------*/
#ifndef TABLE_INLUDED
#define TABLE_INCLUDED

#include <stddef.h>
/*--------------------------------------------------------------------*/

/* a SymTable_T is able to store and return the appropriate value for 
   a key */
typedef struct SymTable *SymTable_T;

/*--------------------------------------------------------------------*/

/* creates a new SymTable with no bindings and returns it */
SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/

/* frees the memory occupied by oSymTable and returns if it was 
   successful or not */
void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* returns the length of oSymTable */
size_t SymTable_getLength(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* first searches oSymTable and inserts the key pcKey with a value 
   pvValue if the key is not already in oSymTable. otherwise it returns
   the value at the key*/
int SymTable_put(SymTable_T oSymTable, 
    const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

/* searches oSymTable for the key pcKey and replaces the existing value
   with pvValue */
void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

/* checks oSymTable to see if there is an entry with the key pcKey and
   returns 1 if found and 0 if not */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* searches oSymTable to see if there is a key pcKey and if there is it
   returns its value */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* searches oSymTable for an entry with key pcKey and if there is it 
   removes it */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* goes through oSymTable and applies the pfApply function to all the 
   bindings and passes pvExtra as another parameter */
void SymTable_map(SymTable_T oSymTable, 
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra);

/*--------------------------------------------------------------------*/

#endif