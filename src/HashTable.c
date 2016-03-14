#ifndef  HASH_TABLE_C
#define  HASH_TABLE_C

#include <HashTable.h>

/*
 * This function is used to create the hash table for variable assignment of the
 * size given by the user.
 */
tHashTable * CreateHT( s32 Size_p )
{
  tHashTable * HashTable = NULL;
  s32 iElement;
  u8 Fault = 0;

  if ( Size_p < 0 )
  {
    Fault = 1;
  }

  if ( ! Fault )
  {
    HashTable = malloc( sizeof( tHashTable ) );
    if ( HashTable == NULL )
    {
      Fault = 1;
    }
  }

  if ( ! Fault )
  {
    HashTable->ppTable = malloc( sizeof( tTableEntry * ) * Size_p );
    if ( HashTable->ppTable == NULL )
    {
      Fault = 1;
      HashTable = NULL;
    }
    else
    {
      for( iElement = 0; iElement < Size_p; iElement++ )
      {
        HashTable->ppTable[ iElement ] = NULL;
      }
    }
  }

  if ( ! Fault )
  {
    HashTable->Size = Size_p;
  }

  return HashTable;
}

/*
 * This function is used to calculate the hash value.
 */
s32 Hash( tHashTable * pHashTable_p, c8 * pKey_p )
{
  ul32 Hashval;
  ul32 Hash;
  s32 iKey = 0;

  /* Convert our string to an integer */
  while( (Hashval < ULONG_MAX) && (iKey < strlen( pKey_p )) )
  {
    Hashval = Hashval << 8;
    Hashval += pKey_p[ iKey ];
    iKey++;
  }

  Hash = Hashval % pHashTable_p->Size;
  return Hash;
}

/*
 * This function is used to create a new hash value pair to be entered into hash table.
 */
tTableEntry * CreateHTEntry( c8 * pKey_p, c8 * pValue_p )
{
  u8 Fault = 0;
  tTableEntry * NewEntry = NULL;;

  if ( (pKey_p == NULL) || (pValue_p == NULL) )
  {
    Fault = 1;
    NewEntry = NULL;
  }
  if ( ! Fault )
  {
    NewEntry = malloc( sizeof( tTableEntry ) );
    if( NewEntry == NULL )
    {
      Fault = 1;
    }
  }

  if ( ! Fault )
  {
    NewEntry->pKey = strdup( pKey_p );
    if( NewEntry->pKey == NULL )
    {
      Fault = 1;
      NewEntry = NULL;
    }
  }

  if ( ! Fault )
  {
    NewEntry->pValue = strdup( pValue_p );
    if( NewEntry->pValue == NULL )
    {
      Fault = 1;
      NewEntry = NULL;
    }
    else
    {
      NewEntry->pNextEntry = NULL;
    }
  }

  return NewEntry;
}

/*
 * This function is used to enter a new key value pair into hash table.
 */
void SetHT( tHashTable * pHashTable_p, c8 * pKey_p, c8 * pValue_p )
{
  s32 Bin = 0;
  tTableEntry * NewEntry = NULL;
  tTableEntry * Next = NULL;
  tTableEntry * Last = NULL;

  Bin = Hash( pHashTable_p, pKey_p );

  Next = pHashTable_p->ppTable[ Bin ];

  while( (Next != NULL) && (Next->pKey != NULL) && (strcmp( pKey_p, Next->pKey ) > 0) )
  {
    Last = Next;
    Next = Next->pNextEntry;
  }

  /* There's already a pair.  Let's replace that string. */
  if( (Next != NULL) && (Next->pKey != NULL) && (strcmp( pKey_p, Next->pKey ) == 0) )
  {
    free( Next->pValue );
    Next->pValue = strdup( pValue_p );

    /* Nope, could't find it.  Time to grow a pair. */
  }
  else
  {
    NewEntry = CreateHTEntry( pKey_p, pValue_p );
    if ( NewEntry == NULL )
    {
      fprintf(stderr, "HashTable Create entry: allocation error\n");
      exit( cExitFailure );
    }

    /* We're at the start of the linked list in this bin. */
    if( Next == pHashTable_p->ppTable[ Bin ] )
    {
      NewEntry->pNextEntry = Next;
      pHashTable_p->ppTable[ Bin ] = NewEntry;

      /* We're at the end of the linked list in this bin. */
    }
    else if ( Next == NULL )
    {
      Last->pNextEntry = NewEntry;

      /* We're in the middle of the list. */
    }
    else
    {
      NewEntry->pNextEntry = Next;
      Last->pNextEntry = NewEntry;
    }
  }
}

/*
 * This function is used to get the value for a key from the hash table.
 */
c8 * GetHT( tHashTable * pHashTable_p, c8 * pKey_p )
{
  s32 Bin = 0;
  c8 * Result = NULL;
  tTableEntry * Pair;

  if ( (pHashTable_p == NULL) || (pKey_p == NULL) )
  {
    Result = NULL;
  }
  else
  {
    Bin = Hash( pHashTable_p, pKey_p );

    /* Step through the bin, looking for our value. */
    Pair = pHashTable_p->ppTable[ Bin ];
    while( (Pair != NULL) && (Pair->pKey != NULL) && (strcmp( pKey_p, Pair->pKey ) > 0) )
    {
      Pair = Pair->pNextEntry;
    }

    /* Did we actually find anything? */
    if( (Pair == NULL) || (Pair->pKey == NULL) || (strcmp( pKey_p, Pair->pKey ) != 0) )
    {
      Result = NULL;

    }
    else
    {
      Result = Pair->pValue;
    }
  }

  return Result;
}

#endif
