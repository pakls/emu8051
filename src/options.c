/* options.c -- functions for processing command-line options and arguments
   Copyright (C) 2003 Hugo Villeneuve */


#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include "common.h"
#include "options.h"


char *hex_file;


char *
GetHexFileName( void )
{
  return hex_file;
}


/*******************************************************************************
 * Display the help message and exit
 ******************************************************************************/
static void
DisplayUsage( void )
{
  printf( "Usage: %s [OPTION]... [FILENAME]\n", PACKAGE );
  printf( "Simulator/emulator for 8051 family microcontrollers.\n\n" );
  printf( "  -h                        display this help and exit\n" );
  printf( "  -version                  display version information and exit\n");
  printf( "\n" );
}


/*******************************************************************************
 * Display version information and exit
 ******************************************************************************/
static void
DisplayVersion( void )
{
  printf( "\n" );
  printf( "  %s, version %s\n", PACKAGE, VERSION );
  printf( "  Written by Hugo Villeneuve, Jonathan St-Andr� and Pascal Fecteau.\n\n" );
}


static void
InvalidOption( const char *message, /*@null@*/ const char *string )
{
  if( string == NULL ) {
    fprintf(stderr, "%s: %s\n", PACKAGE, message );
  }
  else {
    fprintf(stderr, "%s: %s %s\n", PACKAGE, message, string );
  }

  fprintf(stderr, "Try `%s -h' for more information.\n", PACKAGE );

  exit( EXIT_FAILURE );
}


/*******************************************************************************
 * Initializes the different options passed as arguments on the command line.
 ******************************************************************************/
void
ParseCommandLineOptions( int argc, char *argv[] )
{
  int i;
  char *token;

  for( i = 1; i < argc; i++ ) {
    token = argv[i];
    switch( token[0] ) {
    case '-':
      /* Processing options names */
      switch( token[1] ) {
      case 'h':
	if( strlen( &token[1] ) == 1 ) {
	  DisplayUsage();
	  exit( EXIT_SUCCESS );
	}
	InvalidOption( "invalid option", token );
	break;
      case 'v' :
	if( STREQ( "version", &token[1] ) ) {
	  DisplayVersion();
	  exit( EXIT_SUCCESS );
	}
	else {
	  InvalidOption( "invalid option", token );
	}
	break;
      default:
	InvalidOption( "invalid option", token );
	break;
      } /* end switch( token[1] ) */
      break;
    default:
      /* Processing options arguments */
      /* Must be the filename... */
      hex_file = token;
      break;
    } /* end switch( token[0] ) */
  } /* end for */
  
}
