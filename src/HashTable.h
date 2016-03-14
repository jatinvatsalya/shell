#ifndef  HASH_TABLE_H
#define  HASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "TypeDefs.h"
/* ========================================================================= */
/**
 * This structure contains the information of a particular key value pair of a
 * hash table and a pointer to a next pair in the table.
 */
struct TableEntry_
{
  c8 * pKey;
  /**<Pointer to a hash key. */
  c8 * pValue;
  /**<Pointer to a hash value. */
  struct TableEntry_ * pNextEntry;
  /**<Pointer to next entry in hash table. */
};

/**
 * This typedef is used to assign alternative name to the structure #TableEntry_.
 */
typedef struct TableEntry_ tTableEntry;

/**
 * This structure contains the hash table and the size of that hash table.
 */
struct HashTable_
{
  s32 Size;
  /**<Size of hash table. */
  tTableEntry ** ppTable;
  /**<Pointer to a hash table. */
};

/**
 * This typedef is used to assign alternative name to the structure #HashTable_.
 */
typedef struct HashTable_ tHashTable;
/* ------------------------------------------------------------------------- */
/**
 * Used for success return.
 */
#define cExitSuccess                 0
/**
 * Used for failure return.
 */
#define cExitFailure                 1
/* ========================================================================= */
/**
 * This function is used to create the hash table for variable assignment of the
 * size given by the user.
 *
 * @param   Size_p       Size of hash table.
 * @return  tHashTable   Pointer to a hash table.
 */
tHashTable * CreateHT( s32 Size_p );

/**
 * This function is used to calculate the hash value.
 *
 * @param   pHashTable_p Pointer to a hash table.
 * @param   pKey_p       Pointer to a hash key for which hash value need to be calculated.
 * @return  s32          Pointer to a hash value.
 */
s32 Hash( tHashTable * pHashTable_p, c8 * pKey_p );

/**
 * This function is used to create a new hash value pair to be entered into hash table.
 *
 * @param   pKey_p       Pointer hash key for key value pair.
 * @param   pValue_p     Pointer to a hash value for key value pair.
 * @return  tTableEntry  pointer structure #tTableEntry containing the hash value pair.
 */
tTableEntry * CreateHTEntry( c8 * pKey_p, c8 * pValue_p );

/**
 * This function is used to enter a new key value pair into hash table.
 *
 * @param   pHashTable_p Pointer to a hash table structure #tHashTable.
 * @param   pKey_p       Pointer to a hash key.
 * @param   pValue_p     Pointer to a hash value.
 */
void SetHT( tHashTable * pHashTable_p, c8 * pKey_p, c8 * pValue_p );

/**
 * This function is used to get the value for a key from the hash table.
 *
 * @param   pHashTable_p Pointer to a hash table structure #tHashTable.
 * @param   pKey_p       Pointer to a hash key.
 * @return  c8           Pointer to a hash value.
 */
c8 * GetHT( tHashTable * pHashTable_p, c8 * pKey_p );
/* ========================================================================= */
#endif
