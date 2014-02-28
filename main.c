/**
 * Test main for the Diamond-Square island generator.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>

#include "island.h"

struct COLOR {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

static void printMapAsPgm ( int ***, const unsigned int, char * );
static void printMapAsPgm2 ( int ***, const unsigned int, char * );
static void printFloatMapAsPgm ( float ***, const unsigned int, char * );
static void printMapAsPpm ( int ***, const unsigned int, char * );
static struct COLOR getTerrainType( int );

int main ( int argc, char **argv ) {
  int              i, m, ** imap;
  unsigned int     n = 0;
  float **        map;

  if ( argc < 3 ) {
    fprintf ( stderr, "Usage: %s N <prefix>\nN is an integer greater than 0.\n", argv[0] );
    return EXIT_FAILURE;
  }

  m = atoi( argv[1] );

  if ( m <= 0 ) {
    fprintf ( stderr, "N must be an integer greater than zero: %d\n", m );
    return EXIT_FAILURE;
  }

  n = (unsigned int)( pow ( 2.0, m ) ) + 1;

  map = ( float ** ) malloc ( sizeof ( float * ) * n);
  for ( i = 0; i < n; ++i ) {
    map[ i ] = ( float * ) calloc ( n, sizeof ( float ) );
  }

  imap = ( int ** ) malloc ( sizeof ( int * ) * n);
  for ( i = 0; i < n; ++i ) {
    imap[ i ] = ( int * ) calloc ( n, sizeof ( int ) );
  }

  ds ( &map, n );
  island ( &imap, n );
  normInt ( &imap, n );
  printMapAsPgm2 ( &imap, n, argv[2] );
  norm ( &map, n );
  printFloatMapAsPgm ( &map, n, argv[2] );
  mult ( &map, &imap, n );
  smooth( &imap, n );
  normInt ( &imap, n );
  printMapAsPgm ( &imap, n, argv[2] );
  printMapAsPpm ( &imap, n, argv[2] );

  for ( i = 0; i < n; ++i ) {
    free ( map[ i ] );
  }
  free ( map );

  for ( i = 0; i < n; ++i ) {
    free ( imap[ i ] );
  }
  free ( imap );

  return EXIT_SUCCESS;
}

void printFloatMapAsPgm ( float *** map, const unsigned int MAP_SIZE, char * fn ) {
  int i, j, max = -1;
  FILE * f;
  char * filename = ( char * ) malloc ( ( strlen ( "ds_map.pgm" ) + strlen ( fn ) ) * sizeof ( char ) + 1);
  
  strcpy ( filename, fn );
  strcat ( filename, "ds_map.pgm" );

  f = fopen ( filename , "w" );

  fprintf ( f, "P2\n" );
  fprintf ( f, "%u %u\n", MAP_SIZE, MAP_SIZE );
  fprintf ( f, "255\n" );
  
  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      fprintf( f, "%d ", ( int )( ( * map )[ i ][ j ] * 255.0f ) );
    }
    fprintf( f, "\n" );
  }

  fclose ( f );
  free ( filename );
}

void printMapAsPgm ( int *** map, const unsigned int MAP_SIZE, char * fn ) {
  int i, j, max = -1;
  FILE * f;
  char * filename = ( char * ) malloc ( ( strlen ( "hmmap.pgm" ) + strlen ( fn ) ) * sizeof ( char ) + 1);
  
  strcpy ( filename, fn );
  strcat ( filename, "hmmap.pgm" );

  f = fopen ( filename , "w" );

  f = fopen ( filename, "w" );

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      max = max < ( * map )[ i ][ j ] ? ( * map )[ i ][ j ] : max;
    }
  }

  fprintf ( f, "P2\n" );
  fprintf ( f, "%u %u\n", MAP_SIZE, MAP_SIZE );
  fprintf ( f, "%d\n", max );
  
  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      fprintf( f, "%d ", ( * map )[ i ][ j ] );
    }
    fprintf( f, "\n" );
  }

  fclose ( f );
}

void printMapAsPgm2 ( int *** map, const unsigned int MAP_SIZE, char * fn ) {
  int i, j, max = -1;
  FILE * f;
  char * filename = ( char * ) malloc ( ( strlen ( "is.pgm" ) + strlen ( fn ) ) * sizeof ( char ) + 1);
  
  strcpy ( filename, fn );
  strcat ( filename, "is.pgm" );

  f = fopen ( filename , "w" );

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      max = max < ( * map )[ i ][ j ] ? ( * map )[ i ][ j ] : max;
    }
  }

  fprintf ( f, "P2\n" );
  fprintf ( f, "%u %u\n", MAP_SIZE, MAP_SIZE );
  fprintf ( f, "%d\n", max );
  
  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      fprintf( f, "%d ", ( * map )[ i ][ j ] );
    }
    fprintf( f, "\n" );
  }

  fclose ( f );
  free ( filename );
}

void printMapAsPpm ( int *** map, const unsigned int MAP_SIZE, char * fn ) {
  struct COLOR c;
  int i, j;
  FILE * f;
  char * filename = ( char * ) malloc ( ( strlen ( "map.ppm" ) + strlen ( fn ) ) * sizeof ( char ) + 1);
  
  strcpy ( filename, fn );
  strcat ( filename, "map.ppm" );

  f = fopen ( filename, "w" );

  fprintf ( f, "P3\n" );
  fprintf ( f, "%u %u\n", MAP_SIZE, MAP_SIZE );
  fprintf ( f, "255\n" );

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      c = getTerrainType( ( * map )[ i ][ j ] );
      fprintf( f, "%d %d %d\t", c.r, c.g, c.b );
    }
    fprintf( f, "\n" );
  }

  fclose ( f );
  free ( filename );
}

struct COLOR getTerrainType( int h) {
  struct COLOR c;

  if ( h < 30 ) {
    if ( h < 15 ) {
      c.r = 64; c.g = 64; c.b = 200;
      return c;
    } else {
      c.r = 128; c.g = 128; c.b = 255;
      return c;
    }
  } else {
    if ( h < 40 ) {
      c.r = 255; c.g = 234; c.b = 150;
      return c;
    } else if ( h >= 40 && h < 100 ) {
      c.r = 16; c.g = 180; c.b = 40;
      return c;
    } else if ( h >= 100 && h < 150 ) {
      c.r = 70; c.g = 164; c.b = 25;
      return c;
    } else if ( h >= 180 ) {
      c.r = 200; c.g = 200; c.b = 200;
      return c;
    } else {
      c.r = 143; c.g = 111; c.b = 81;
      return c;
    }
  }
}
