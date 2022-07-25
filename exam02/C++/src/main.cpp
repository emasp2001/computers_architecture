/**
 *   Parsing program options in command line
 *
 *   Example
 *
 *     ./cacheSim.out -l 256 -s 4 -b 16 -t1 1 -t2 5 -y write-allocate -w write-back -r lru -f archivoTraza
 *
 *   Set integer variables with cache configuration parameters
 *
 */

#include <algorithm>
#include <string>
#include <iostream>
#include "Memory.cpp"

char* getCmdOption(char ** begin, char ** end, const std::string & option) {
  char ** itr = std::find( begin, end, option );
  if ( itr != end && ++itr != end ) {
      return *itr;
  }
  return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string & option) {
  return std::find( begin, end, option ) != end;
}

int main( int argc, char * argv[] ) {
  long lines = 4;
  long assoc = 1;
  long block = 16;
  long timeLevel1 = 1;
  long timeLevel2 = 7;
  long alloc = 0;	// 0 = No-write-allocate, 1 = Write-allocate
  long write = 0;	// 0 = write-back, 1 = write-through
  long algor = 0;	// 0 = FIFO, 1 = Random, 2 = LRU
  
  std::string strRead;

  if ( cmdOptionExists( argv, argv + argc, "-l" ) ) {
    strRead = std::string( getCmdOption( argv, argv + argc, "-l" ) );
    lines = std::stol( strRead );
  }

  if ( cmdOptionExists( argv, argv + argc, "-s" ) ) {
    strRead = std::string( getCmdOption( argv, argv + argc, "-s" ) );
    assoc = std::stol( strRead );
  }

  if ( cmdOptionExists( argv, argv + argc, "-b" ) ) {
    strRead = std::string( getCmdOption( argv, argv + argc, "-b" ) );
    assoc = std::stol( strRead );
  }

  if ( cmdOptionExists( argv, argv + argc, "-t1" ) ) {
    strRead = std::string( getCmdOption( argv, argv + argc, "-t1" ) );
    timeLevel1 = std::stol( strRead );
  }

  if ( cmdOptionExists( argv, argv + argc, "-t2" ) ) {
    strRead = std::string( getCmdOption( argv, argv + argc, "-t2" ) );
    timeLevel2 = std::stol( strRead );
  }

  if ( cmdOptionExists( argv, argv + argc, "-y" ) ) {
    strRead = std::string( getCmdOption( argv, argv + argc, "-y" ) );
    alloc = ( 0 == strRead.compare( "write-allocate" ) );	// Default to "no-write-allocate" if parameter is differnt from "write-allocate"
  }

  if ( cmdOptionExists( argv, argv + argc, "-w" ) ) {
    strRead = std::string( getCmdOption( argv, argv + argc, "-w" ) );
    write = ( 0 == strRead.compare( "write-back" ) );		// Default to "write-back" if parameter is different from "write-back"
  }

  if ( cmdOptionExists( argv, argv + argc, "-r" ) ) {
    strRead = std::string( getCmdOption( argv, argv + argc, "-r" ) );
    algor = 0;	// Default to "FIFO"
    if ( ! strRead.compare( "random" ) ) {
        algor = 1;
    } else {
      if ( ! strRead.compare( "lru" ) ) {
          algor = 2;
      }
    }
  }

  char * filename = getCmdOption(argv, argv + argc, "-f");
  if (!filename) {
    std::string file = "../data/Traza-1.txt";
    filename = new char[file.length() + 1];
    strcpy(filename, file.c_str());
  }

  Cache * cache = new Cache(lines, assoc, block, timeLevel1, timeLevel2, alloc, write, algor, filename);
  cache->run();
  
  return 0;
}

