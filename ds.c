#ifndef __cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include <string.h>

#else

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cfloat>
#include <cmath>
#include <cstring>

#endif

#include "ds.h"

static const float SEED = 1000.0;

static float randomFloat ();

/**
 * Port of the java implementation outlined here http://stackoverflow.com/a/2773032
 */
void ds ( float *** map, const unsigned int DATA_SIZE ) {
  float    avg, r, h = SEED / 2.0;
  int       sideLength, halfSide, x, y;

  srand( time( NULL ) );

  ( * map )[ 0 ][ 0 ] = ( * map )[ 0 ][ DATA_SIZE - 1 ] = ( * map )[ DATA_SIZE - 1 ][ 0 ] = ( * map )[ DATA_SIZE - 1 ][ DATA_SIZE - 1 ] = h;

  for ( sideLength = DATA_SIZE - 1; sideLength >= 2; sideLength /= 2, h /= 2.0 ) {
    halfSide = sideLength / 2;

    for ( x = 0; x < DATA_SIZE -1; x += sideLength ) {
      for ( y = 0; y < DATA_SIZE - 1; y += sideLength ) {
	avg = ( * map )[ x ][ y ] +
	  ( * map )[ x + sideLength ][ y ] +
	  ( * map )[ x ][ y + sideLength ] +
	  ( * map )[ x + sideLength ][ y + sideLength ];
	avg /= 4.0;

	r = randomFloat();

	( * map )[ x + halfSide ][ y + halfSide ] = avg + ( r * 2.0 * h ) - h;
      }
    }

    for ( x = 0; x < DATA_SIZE - 1; x += halfSide ) {
      for ( y =  ( x + halfSide ) % sideLength; y < DATA_SIZE - 1; y += sideLength ) {
	avg = ( * map )[ ( x - halfSide + DATA_SIZE ) % DATA_SIZE ][ y ] +
	  ( * map )[ ( x + halfSide ) % DATA_SIZE ][ y ] +
	  ( * map )[ x ][ ( y + halfSide ) % DATA_SIZE ] +
	  ( * map )[ x ][ ( y - halfSide + DATA_SIZE ) % DATA_SIZE ];
	avg /= 4.0;

	r = randomFloat();

	avg = avg + ( r * 2.0 * h ) - h;

	( * map )[ x ][ y ] = avg;

	if ( x == 0 ) ( * map )[ DATA_SIZE - 1 ][ y ] = avg;
	if ( y == 0 ) ( * map )[ x ][ DATA_SIZE - 1 ] = avg;
      }
    }
  }
}

void island ( int ***imap, unsigned int MAP_SIZE ) {
  const int part = ( MAP_SIZE * 3000 ) / 513;
  const int life = ( MAP_SIZE * 5000 ) / 513;
  int x, y, dx, dy, i, c, n, nx, ny, l;

  srand( time( NULL ) );

  for ( i = 0; i < part; ++i ) {
    x = ( MAP_SIZE / 4 ) + ( random() % ( MAP_SIZE / 2 ) );
    y = ( MAP_SIZE / 4 ) + ( random() % ( MAP_SIZE / 2 ) );

    for ( l = life; l >= 0; l-- ) {
      dx = ( random() % 3 ) - 1;
      dy = ( random() % 3 ) - 1;

      ( * imap )[ x ][ y ] += 1;

      c = ( * imap )[ x ][ y ];

      nx = ( x + dx );
      ny = ( y + dy );
    
      if ( nx < 0 || nx >= MAP_SIZE || ny < 0 || ny >= MAP_SIZE ) {
	l--;
	continue;
      }

      if ( ( * imap )[ nx ][ ny ] <= c ) {
	  ( * imap )[ nx ][ ny ] += 1;
      }

      x = nx;
      y = ny;
    }
  }
}

void norm ( float *** fmap, unsigned int MAP_SIZE ) {
  int i, j;
  float max = -FLT_MAX;

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      max = abs( ( * fmap )[ i ][ j ] ) > max ? abs ( ( * fmap )[ i ][ j ] ) : max;
    }
  }

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      ( * fmap )[ i ][ j ] += max;
    }
  }

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      max = abs( ( * fmap )[ i ][ j ] ) > max ? abs ( ( * fmap )[ i ][ j ] ) : max;
    }
    }

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      ( * fmap )[ i ][ j ] /= max;
    }
  }
}

void normInt ( int *** map, unsigned int MAP_SIZE ) {
  int i, j, max = -1;

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      max = max < ( * map )[ i ][ j ] ? ( * map )[ i ][ j ] : max;
    }
  }

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      ( * map )[ i ][ j ] = ( int ) ( ( ( float ) ( * map )[ i ][ j ] / ( float ) max ) * 255.0 );
    }
  }
}

extern void mult ( float *** fmap, int *** imap, unsigned int MAP_SIZE ) {
  int i, j;
  
  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      /*fprintf ( stderr, "%f %d\n", ( * fmap )[ i ][ j ], ( * imap )[ i ][ j ] );*/
      ( * imap )[ i ][ j ] = ( int )( ( ( * fmap )[ i ][ j ] * ( ( float ) ( * imap )[ i ][ j ] / 255.0f ) ) * 250.0f );
    }
  }

}

void smooth ( int *** map, unsigned int MAP_SIZE) {
  int i, j, avg, **temp;
  
  temp = ( int ** ) malloc ( sizeof ( int * ) * MAP_SIZE );
  for ( i = 0; i < MAP_SIZE; ++i ){
    temp[ i ] = ( int * ) malloc ( sizeof ( int ) * MAP_SIZE );
  }

  for ( i = 0; i < MAP_SIZE; ++i ) {
    for ( j = 0; j < MAP_SIZE; ++j ) {
      avg = 0;

      /* I C H
       * E A D
       * G B F */

      /* I */ avg += ( * map )[ ( i - 1 ) < 0 ? MAP_SIZE - 1 : i - 1 ][ ( j  - 1 ) < 0 ? MAP_SIZE - 1 : j - 1 ];
      /* C */ avg += ( * map )[ ( i - 1 ) < 0 ? MAP_SIZE - 1 : i - 1 ][ j                                     ];
      /* H */ avg += ( * map )[ ( i - 1 ) < 0 ? MAP_SIZE - 1 : i - 1 ][ ( j + 1 ) % MAP_SIZE                  ];
      /* E */ avg += ( * map )[ i                                    ][ ( j  - 1 ) < 0 ? MAP_SIZE - 1 : j - 1 ];
      /* A */ avg += ( * map )[ i                                    ][ j                                     ];
      /* D */ avg += ( * map )[ i                                    ][ ( j + 1 ) % MAP_SIZE                  ];
      /* G */ avg += ( * map )[ ( i + 1 ) % MAP_SIZE                 ][ ( j  - 1 ) < 0 ? MAP_SIZE - 1 : j - 1 ];
      /* B */ avg += ( * map )[ ( i + 1 ) % MAP_SIZE                 ][ j                                     ];
      /* F */ avg += ( * map )[ ( i + 1 ) % MAP_SIZE                 ][ ( j + 1 ) % MAP_SIZE                  ];

      avg /= 9;

      temp[ i ][ j ] = avg;
    }
  }

  for ( i = 0; i < MAP_SIZE; ++i ){
    memcpy ( ( * map )[ i ], temp[ i ], MAP_SIZE * sizeof ( int ) );
    free ( temp[ i ] );
  }

  free ( temp );
}

static float randomFloat() {
  float r =  ( float ) random() / ( float ) RAND_MAX;
  r = ( r * 2.0 ) - 1.0;
  return r;
}
