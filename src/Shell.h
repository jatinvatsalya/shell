#ifndef  SHELL_H
#define  SHELL_H
/* ========================================================================= */
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "TypeDefs.h"
#include "HashTable.h"
/* ========================================================================= */
/**
 * Used for success return.
 */
#define cExitSuccess               0
/**
 * Used for failure return.
 */
#define cExitFailure               1
/**
 * Used to give a size to a buffer.
 */
#define cBufferSize             1024
/**
 * Defines size to a directory.
 */
#define cDirSize                1024
/**
 * Defines initial number of arguments.
 */
#define cElements                 10
/**
 * Defines Delimiters used for spliting line into arguments.
 */
#define cDelimiter       " \t\r\n\a"
/**
 * Defines number of shell built in commands.
 */
#define cBuiltinNumber             5
/**
 * Defines size of hash table to store variables.
 */
#define cHashSize               2048
/**
 * Defines input redirection.
 */
#define cIn                        1
/**
 * Defines output redirection.
 */
#define cOut                       2
/* ------------------------------------------------------------------------- */
/**
 * This structure contains the information of the arguments taken as input from user.
 */
struct Args_
{
  c8 ** ppArguments;
  /**<List of all the arguments. */
  u16 nArguments;
  /**<Number of arguments. */
};

/**
 * This typedef is used to assign alternative name to the structure #Args_.
 */
typedef struct Args_ tArgs;
/* ========================================================================= */
#endif
