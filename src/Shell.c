#ifndef  SHELL_C
#define  SHELL_C

#include"Shell.h"

/**
 * @mainpage dsh Shell
 * 					 This shell is a custom shell and is named dsh. this shell is created
 * 					 by Jatin Jain.
 */
/* ========================================================================= */
/**
 * This function will take command typed on console by user and will save it in
 * a line for further parsing.
 *
 * @return c8  Returns pointer to a line.
 */
static c8 * GetLine( void );

/**
 * This function will parse the line taken as input from user. It will replace
 * the variables with their original value and will split it into multiple arguments.
 *
 * @param   Line_p       Pointer to a line to be parsed.
 * @param   HashTable_p  Hash table of shell variables.
 * @return  tArgs        Pointer to a structure #tArgs containing arguments.
 */
static tArgs * SplitLine( c8 * Line_p, tHashTable * HashTable_p );

/**
 * This function will take the arguments and will launch the process such as vi, ls,
 * echo, etc.
 *
 * @param   Args_p        Pointer to a structure #tArgs containing arguments.
 * @return  s32          Return value.
 */
static s32 RunProcess( tArgs * Args_p );

/**
 * This function will take the arguments and will do the task as requested by the
 * user. It will perform all the task including shell built in.
 *
 * @param   Args_p       Pointer to a structure #tArgs containing arguments.
 * @param   HashTable_p  Hash table of shell variables.
 * @return  s32          Return value.
 */
static s32 RunCommand( tArgs * Args_p, tHashTable * HashTable_p );

/**
 * This function is the function which is called inside main and will execute
 * the shell.
 */
static void ExecuteDSH( void );

/**
 * This function is used to fetch the value of variable. First the value is
 * checked in the hash table if not found in it then the value is searched in
 * environment as variable can be an environment variable.
 *
 * @param   Variable_p   name of variable
 * @param   HashTable_p  Hash table of shell variables.
 * @return  c8           Pointer to a value of variable. Returns NULL if value not
 *                       found.
 */
static c8 * GetVariableValue( c8 * Variable_p, tHashTable * HashTable_p );

/**
 * This function is used execute the shell built in command "cd". It is used to change
 * the working directory. Used can give absolute as well as relative path to change the
 * directory.
 *
 * @param   Args_p       Pointer to a structure #tArgs containing arguments.
 * @param   HashTable_p  Hash table of shell variables.
 * @return  s32          Return value.
 */
static s32 BuiltIn_cd( tArgs * Args_p, tHashTable * HashTable_p );

/**
 * This function is used execute the shell built in command "help". This will print the
 * information related to shell and will print the list of shell built in commands.
 *
 * @param   Args_p       Pointer to a structure #tArgs containing arguments.
 * @param   HashTable_p  Hash table of shell variables.
 * @return  s32          Return value.
 */
static s32 BuiltIn_help( tArgs * Args_p, tHashTable * HashTable_p );

/**
 * This function is used execute the shell built in command "exit". This command is used
 * to exit from the shell.
 *
 * @param   Args_p       Pointer to a structure #tArgs containing arguments.
 * @param   HashTable_p  Hash table of shell variables.
 * @return  s32          Return value.
 */
static s32 BuiltIn_exit( tArgs * Args_p, tHashTable * HashTable_p );

/**
 * This function is used execute the shell built in command "set". This command is used
 * to set the shell variables. For example "set a = 90" spaces are mandatory.
 *
 * @param   Args_p       Pointer to a structure #tArgs containing arguments.
 * @param   HashTable_p  Hash table of shell variables.
 * @return  s32          Return value.
 */
static s32 BuiltIn_set( tArgs * Args_p, tHashTable * HashTable_p );

/**
 * This function is used execute the shell built in command "export". This command is same
 * as that of bash shell. it is used to export the shell variable so that they can be used
 * is subprocesses too.
 *
 * @param   Args_p       Pointer to a structure #tArgs containing arguments.
 * @param   HashTable_p  Hash table of shell variables.
 * @return  s32          Return value.
 */
static s32 BuiltIn_export( tArgs * Args_p, tHashTable * HashTable_p );

/* ******************************************************************************************* */
c8 * DSHBuiltin[ ] = {
    "cd",
    "help",
    "exit",
    "set",
    "export"
};

s32 (* FuncBuiltin[ ]) ( tArgs * Args_p, tHashTable * HashTable_p ) = {
		&BuiltIn_cd,
		&BuiltIn_help,
		&BuiltIn_exit,
		&BuiltIn_set,
		&BuiltIn_export
};

/* ******************************************************************************************* */
/*
 * This function will take command typed on console by user and will save it in
 * a line for further parsing.
 */
static c8 * GetLine( void )
{
  c8 * Line;
  u16 iLine = 0;
  c8 InputChar;
  u8 Read = 1;
  s32 nBytes = cBufferSize;

  Line = malloc( nBytes );
  while( Read )
  {
    InputChar = getchar( );
    if ( (InputChar == EOF) || (InputChar == '\n') )
    {
      Line[ iLine ] = '\0';
      Read = 0;
    }
    else
    {
      Line[ iLine ] = InputChar;
    }

    iLine++;

    if ( Read )
    {
      if ( iLine >= nBytes )
      {
        nBytes += cBufferSize;
        Line = realloc( Line, nBytes );
        if ( ! Line )
        {
          fprintf(stderr, "GetLine: allocation error\n");
          exit( cExitFailure );
        }
      }
    }
  }

  return Line;
}
/*
 * This function will take the arguments and will launch the process such as vi, ls,
 * echo, etc.
 */
static tArgs * SplitLine( c8 * Line, tHashTable * HashTable_p )
{
  tArgs * Result;
  c8 * Token;
  c8 * TempToken;
  u16 nElements = cElements;
  u16 iArgument = 0;

  Result = malloc( sizeof( tArgs ) );
  if ( ! Result )
  {
    fprintf(stderr, "SplitLine: allocation error\n");
    exit( cExitFailure );
  }
  Result->nArguments = 0;
  Result->ppArguments = malloc( nElements * sizeof( c8 * ) );
  if ( ! Result->ppArguments )
  {
    fprintf(stderr, "SplitLine: allocation error\n");
    exit( cExitFailure );
  }

  Token = strtok( Line, cDelimiter );
  while ( Token != NULL )
  {
    Result->nArguments++;
    Result->ppArguments[ iArgument ] = Token;
    iArgument++;

    if ( iArgument >= cElements )
    {
      nElements += cElements;
      Result->ppArguments = realloc( Result->ppArguments, nElements * sizeof( c8 * ) );
      if ( ! Result->ppArguments )
      {
        fprintf(stderr, "SplitLine: allocation error\n");
        exit( cExitFailure );
      }
    }

    TempToken = strtok( NULL, cDelimiter );
    if ( (TempToken !=NULL) && (TempToken[ 0 ] == '$') )
    {
      Token = GetVariableValue( &TempToken[ 1 ], HashTable_p );
      if ( Token == NULL)
      {
        Token = TempToken;
      }
    }
    else
    {
      Token = TempToken;
    }
  }

  return Result;
}

/*
 * This function will take the arguments and will launch the process such as vi, ls,
 * echo, etc.
 */
s32 RunProcess( tArgs * Args_p )
{
  pid_t Pid;
  pid_t Wpid;
  s32 Status;
  s32 InputFileHandle;
  s32 OutputFileHandle;
  u8 Direction = 0;

  if ( Args_p->nArguments > 2 )
  {
    if ( Args_p->ppArguments[ Args_p->nArguments - 2 ][ 0 ] == '>' )
    {
      Direction = cOut;
//      printf( "direction is out\n" );
      Args_p->ppArguments[ Args_p->nArguments - 2 ] = NULL;
    }
    else if ( Args_p->ppArguments[ Args_p->nArguments - 2 ][ 0 ] == '<' )
    {
      Direction = cIn;
//      printf( "direction is in\n" );
      Args_p->ppArguments[ Args_p->nArguments - 2 ] = NULL;
    }
    else
    {
      //Mista C
    }
  }

  Pid = fork( );
  if ( Pid == 0 )
  {
    // Child process
    if ( Direction == cOut )
    {
      OutputFileHandle = open( Args_p->ppArguments[ Args_p->nArguments - 1 ], O_WRONLY | O_CREAT );
      if ( OutputFileHandle == -1 )
      {
        fprintf( stderr, "dsh error: unable to open file for writing\n" );
      }
      dup2( OutputFileHandle, STDOUT_FILENO );
      Args_p->ppArguments[ Args_p->nArguments - 1 ] = NULL;
      close( OutputFileHandle );
    }
    if ( Direction == cIn )
    {
      InputFileHandle = open( Args_p->ppArguments[ Args_p->nArguments - 1 ], O_RDONLY );
      if ( InputFileHandle == -1 )
      {
        fprintf( stderr, "dsh error: unable to open file for reading\n" );
      }
      dup2( InputFileHandle, STDIN_FILENO );
      Args_p->ppArguments[ Args_p->nArguments - 1 ] = NULL;
      close( InputFileHandle );
    }
    if ( execvp(Args_p->ppArguments[ 0 ], Args_p->ppArguments) == -1 )
    {
      perror( "dsh" );
    }
    exit( cExitFailure );
  }
  else if ( Pid < 0 )
  {
    // Error forking
    perror( "dsh" );
  }
  else
  {
    do
    {
      Wpid = waitpid( Pid, &Status, WUNTRACED );
    } while ( (! WIFEXITED( Status ) ) && (! WIFSIGNALED( Status )) );
  }

  return 1;

//  execvp( Args_p->Arguments[ 0 ], &Args_p->Arguments[ 1 ] );
}

/*
 * This function will take the arguments and will do the task as requested by the
 * user. It will perform all the task including shell built in.
 */
static s32 RunCommand( tArgs * Args_p, tHashTable * HashTable_p )
{
  u16 iBuiltin;
  u8 Fault = 0;
  u8 Builtin;
  s32 Status;
  s32 Result = 1;

  if ( Args_p->ppArguments[ 0 ] == NULL )
  {
    Fault = 1;
    Result = 1;
  }

  if ( ! Fault )
  {
    for( iBuiltin = 0; iBuiltin < cBuiltinNumber; iBuiltin++ )
    {
      Status = strcmp( Args_p->ppArguments[ 0 ], DSHBuiltin[ iBuiltin ] );
      if ( Status == 0 )
      {
        Builtin = 1;
        Result = (* FuncBuiltin[ iBuiltin ])( Args_p, HashTable_p );
        iBuiltin = cBuiltinNumber;
      }
      else
      {
        Builtin = 0;
      }
    }
  }

  if ( ! Builtin )
  {
    RunProcess( Args_p );
  }

  return Result;
}

/*
 * This function is the function which is called inside main and will execute
 * the shell.
 */
void ExecuteDSH( void )
{
  c8 * Line;
  tArgs * Args;
  c8 aPwd[ cDirSize ];
  s32 Status = 1;;
  tHashTable * HashTable = NULL;

  HashTable = CreateHT( cHashSize );
  if ( HashTable == NULL )
  {
    fprintf(stderr, "HashTable Create: allocation error\n");
    exit( cExitFailure );
  }

  do
  {
    memset( aPwd, 0, cDirSize );
    getcwd( aPwd, cDirSize );
    fprintf( stdout, "%s->~", aPwd );

    Line = GetLine( );
    Args = SplitLine( Line, HashTable );
    Status = RunCommand( Args, HashTable );

    free( Line );
  } while( Status );
}

/*
 * This function is used to fetch the value of variable. First the value is
 * checked in the hash table if not found in it then the value is searched in
 * environment as variable can be an environment variable.
 */
static c8 * GetVariableValue( c8 * Variable_p, tHashTable * HashTable_p )
{
  c8 * Result = NULL;

  if ( Variable_p == NULL )
  {
    fprintf( stderr, "DSH internal: invalid argument to getVariable\n" );
  }
  else
  {
    Result = GetHT( HashTable_p, Variable_p );
    if ( Result == NULL )
    {
      Result = getenv( Variable_p );
    }
  }

  return Result;
}

/*
 * This function is used execute the shell built in command "cd". It is used to change
 * the working directory. Used can give absolute as well as relative path to change the
 * directory.
 */
static s32 BuiltIn_cd( tArgs * Args_p, tHashTable * HashTable_p )
{
	s32 Status;

	if ( Args_p->ppArguments[ 1 ] == NULL )
	{
		fprintf( stderr, "dsh: expected argument to \"cd\"\n" );
	}
	else
	{
		Status = chdir( Args_p->ppArguments[ 1 ] );
		if ( Status != 0)
		{
			perror( "dsh" );
		}
	}

	return 1;
}

/*
 * This function is used execute the shell built in command "help". This will print the
 * information related to shell and will print the list of shell built in commands.
 */
static s32 BuiltIn_help( tArgs * Args_p, tHashTable * HashTable_p )
{
  s32 iBuiltin;

  printf( "\n********************************************************************\n");
  printf( "Jatin Jain's DSH\n" );
  printf( "Version 1.0.0\n" );
  printf( "For usage just type program names and arguments and then hit enter.\n");
  printf( "Buit in commands are as follows:\n\n" );

  for ( iBuiltin = 0; iBuiltin < cBuiltinNumber; iBuiltin++ )
  {
    printf("%s\n", DSHBuiltin[ iBuiltin ]);
  }
  printf( "********************************************************************\n\n");

  return 1;
}

static s32 BuiltIn_exit( tArgs * Args_p, tHashTable * HashTable_p )
{
	return 0;
}

/*
 * This function is used execute the shell built in command "exit". This command is used
 * to exit from the shell.
 */
static s32 BuiltIn_set( tArgs * Args_p, tHashTable * HashTable_p )
{
  s32 Status;
  s32 Result = 1;
  c8 * Value;
  u8 IsEnv = 0;

  if ( Args_p->ppArguments[ 1 ] != NULL )
  {
    Value = getenv ( Args_p->ppArguments[ 1 ] );
    if ( Value != NULL )
    {
      IsEnv = 1;
    }
  }

  if ( (Args_p->ppArguments[ 2 ] != NULL) && (Args_p->ppArguments[ 1 ] != NULL) && (Args_p->ppArguments[ 3 ] != NULL) )
  {
    Status = strcmp( Args_p->ppArguments[ 2 ], "=" );
    if ( Status == 0 )
    {
      SetHT( HashTable_p, Args_p->ppArguments[ 1 ], Args_p->ppArguments[ 3 ] );
      printf( "%s=%s\n", Args_p->ppArguments[ 1 ], Args_p->ppArguments[ 3 ] );
      if ( IsEnv )
      {
        setenv( Args_p->ppArguments[ 1 ], Args_p->ppArguments[ 3 ], 1 );
      }
    }
    else
    {
      fprintf( stderr, "Invalid argumets for shell inbuilt command \"set\"\n" );
    }
  }
  else
  {
    fprintf( stderr, "Invalid argumets for shell inbuilt command \"set\"\n" );
  }

  return Result;
}

/*
 * This function is used execute the shell built in command "export". This command is same
 * as that of bash shell. it is used to export the shell variable so that they can be used
 * is subprocesses too.
 */
static s32 BuiltIn_export( tArgs * Args_p, tHashTable * HashTable_p )
{
  s32 Status = 1;
  s32 Result = 1;
  c8 * Value;

//  printf( "Number of arguments = %d\n", Args_p->nArguments );
  if ( Args_p->nArguments >= 4 )
  {
    Status = strcmp( Args_p->ppArguments[ 2 ], "=" );
  }

  if ( Status == 0 )
  {
    setenv( Args_p->ppArguments[ 1 ], Args_p->ppArguments[ 3 ], 1 );
    printf( "%s=%s\n", Args_p->ppArguments[ 1 ], Args_p->ppArguments[ 3 ] );
  }
  else if ( Args_p->nArguments >= 2 )
  {
    Value = GetHT( HashTable_p, Args_p->ppArguments[ 1 ] );
    if ( Value != NULL)
    {
      setenv( Args_p->ppArguments[ 1 ], Value, 1 );
    }
    else
    {
      fprintf( stderr, "dsh export error: Variable cannot be exported as it is not initialized\n" );
    }
  }
  else
  {
    fprintf( stderr, "dsh export error: Improper arguments\n" );
  }

  return Result;
}

/* ******************************************************************************************* */

s32 main( s32 argc ,c8 ** argv )
{
  ExecuteDSH( );
  return 0;
}

#endif
