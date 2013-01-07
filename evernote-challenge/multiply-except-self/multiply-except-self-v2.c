// quick-n-dirty approach to this problem
#include <stdlib.h>
#include <stdio.h>

int main( int argc, char **argv )
{
  int numLines;
  scanf( "%i", &numLines );

  int* buf = (int *)malloc( numLines * sizeof( int ) );
  int i;
  for( i = 0; i < numLines; ++i )
  {
    int tmp;
    scanf( "%i", &tmp );
    buf[i] = tmp;
  }

  for( i = 0; i < numLines; ++i )
  {
    long long int product = 1;
    int j;
    for( j = 0; j < numLines; ++j )
    {
      if( j != i )
      {
        product *= buf[j];
      }
    }
    printf( "%lli\n", product );
  }

  free( buf );
  return 0;
}
