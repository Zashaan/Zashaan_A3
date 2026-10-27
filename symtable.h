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

/* frees the memory occupied by oSymTable */
void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* retunrs the length of the SymTable */
size_t SymTable_getLength(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* */
int SymTable_put(SymTable_T oSymTable, 
    const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/


void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/


int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable, 
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra);

/*--------------------------------------------------------------------*/

#endif