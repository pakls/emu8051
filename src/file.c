/* file.c -- functions for loading an Intel HEX file
   Copyright (C) 2004 Hugo Villeneuve */


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


/* Convert an ascii string to an hexadecimal number. */
static unsigned int
Ascii2Hex( char *istring, int length )
{
  unsigned int result = 0;
  int i, ascii_code;

  if ( !length || ( length > (int) strlen(istring) ) ) {
    length = strlen(istring);
  }
  
  for ( i = 0; i < length; i++ ) {
    ascii_code = istring[ i ];
    if ( ascii_code > 0x39 ) {
      ascii_code &= 0xDF;
    }
    if ( ( ascii_code >= 0x30 && ascii_code <= 0x39 ) ||
	 ( ascii_code >= 0x41 && ascii_code <= 0x46 ) ) {
      ascii_code -= 0x30;
      if ( ascii_code > 9 ) {
	ascii_code -= 7;
      }
      result <<= 4;
      result += ascii_code;
    }
    else {
      printf( "%s: In Ascii2Hex(), syntax error.\n", PACKAGE );
    }
  }
  return result;
}


void
LoadHexFile( const char *filename, void (* cpu_write_pgm)( unsigned int Address, unsigned char Value ) )
{
  int i, j, RecLength, LoadOffset, RecType, Data, Checksum;
  
#define LINE_BUFFER_LEN 256
  FILE *fp;
  int status;
  char line[LINE_BUFFER_LEN];

  if( filename != NULL ) {
    /* Trying to open the file. */
    fp = fopen( filename, "r" );
    if( fp == NULL ) {
      perror( PACKAGE );
      /*ErrorLocation( __FILE__, __LINE__ );*/
      exit( EXIT_FAILURE );
    }
  }
  
  /* Reading one line of data from the configuration file. */
  /* char *fgets(char *s, int size, FILE *stream);
     Reading stops after an EOF or a newline.  If a newline is read, it is
     stored into the buffer.  A '\0'  is  stored after the last character in
     the buffer. */
  while( fgets( line, LINE_BUFFER_LEN, fp ) != NULL ) {
    i = 0;
    Checksum = 0;

    if ( line[ i++ ] != ':' ) {
      printf( "%s: line not beginning with \":\"\n", PACKAGE );
      goto close_file;
    }
      
    RecLength = Ascii2Hex( &line[ i ], 2 );
    i += 2;
    Checksum += RecLength;
      
    LoadOffset = Ascii2Hex( &line[i], 4 );
    Checksum += LoadOffset / 256;
    Checksum += LoadOffset % 256;
    i += 4;
    
    RecType = Ascii2Hex( &line[i],2);
    i += 2;
    Checksum += RecType;
    
    if ( RecType == 1 ) {
      Checksum += Ascii2Hex( &line[ i ], 2 );
      if ( Checksum &= 0x000000FF ) {
	/* Error. */
	printf( "%s: Invalid format\n", PACKAGE );
	goto close_file;
      }
      else {
	/* OK */
	goto close_file;
      }
    }   
    
    for ( j = 0; j < RecLength; j++ ) {
      Data = Ascii2Hex( &line[ i ], 2 );
      (*cpu_write_pgm)( (unsigned int)(LoadOffset + j), (unsigned char)Data );
      i += 2;
      Checksum += Data;
    }
    
    RecType = Ascii2Hex( &line[ i ], 2 );
    Checksum += RecType;      
    
    if ( Checksum &= 0x000000FF ) {
      printf( "%s: Invalid format\n", PACKAGE );
      goto close_file;
    }
  }
  
 close_file:
  status = fclose( fp );
  if( status != EXIT_SUCCESS ) {
    fprintf( stderr, "%s: Error closing configuration file.\n", PACKAGE );
    /*ErrorLocation( __FILE__, __LINE__ );*/
    exit( EXIT_FAILURE );
  }
}
