/**
 *  Process a trace file in simple format
 *
 *  Lines in file could contain a comment, starting with "//"
 *  Trace lines must contain an operation ('L' for load or 'S' for store; maybe 'R' for read, and 'W' for write
 *  and an hexadecimal address and optionally a write value
 *  Example
 *  L 0		// Load from memory address 0
 *  S abcdef	// Store to memory address abcdef
 *  S fedcba 7	// Store 7 to memory address fedcba
 *
 */


#include "Trace.h"

int main( int parametros, char ** valores ) {
   char operation;
   long address;
   Trace * trace = new Trace( "Traza-1.txt" );
   
   while ( trace->hasNext() ) {
      trace->getNext( &operation, &address );
      if ( '/' != operation ) {
         printf( "%c %ld\n", operation, address );
      }
   }

}

