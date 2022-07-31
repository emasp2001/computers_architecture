/**
 *  Reads a trace file in simple format
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

#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "Trace.h"

Trace::Trace( std::string name ) {

   this->file.open( name, std::ios::in );
   if ( this->file.fail() ) {
   }
}


/**
 *
 */
Trace::~Trace() {

   this->file.close();

}


/**
 *
 */
bool Trace::hasNext() {
   bool next = false;

   if ( ! this->file.eof() ) {
      getline( this->file, this->line );
      next = ( this->line.size() > 0);
   }

   return next;

}


/**
 *
 */
long hexToLong( std::string hexText ) {
   long val = -1;
   std::istringstream iss( hexText );

   iss >> std::hex >> val;

   return val;

}



/**
 *
 */
bool Trace::getNext( char * operation, long * address) {
   std::string oper;
   std::string addr;
   std::stringstream ss( this->line );

   ss >> oper;
   ss >> addr;

   std::transform( oper.begin(), oper.end(), oper.begin(), ::toupper);
   * operation = oper[ 0 ];
   * address = hexToLong( addr );

   return true;
}

