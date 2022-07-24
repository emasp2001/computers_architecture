#include "Cache.h"
#include "Trace.cc"

Cache::Cache(long lines, long assoc, long block, long timeLevel1, long timeLevel2, long alloc, long write, long algor, std::string traceFile) {
  // Cache Configuration.
  this->lines = lines;
  this->assoc = assoc;
  this->block = block;
  this->timeLevel1 = timeLevel1;
  this->timeLevel2 = timeLevel2;
  this->alloc = alloc;
  this->write = write;
  this->algor = algor;
  this->traceFile = traceFile;

  // Cache memory
  this->cache = new long*[lines];
  for (long i = 0; i < lines; i++) {
    this->cache[i] = new long[assoc];
  }

  // Cache Statistics.
  this->total_loads = 0;
  this->total_stores = 0;
  this->load_hits = 0;
  this->load_misses = 0;
  this->store_hits = 0;
  this->store_misses = 0;
  this->total_cpu_cycles = 0;
}

Cache::~Cache() {
}

void Cache::print_configuration() {
  printf( "Configuración de la memoria cache\n" );
  printf( "       Líneas: %ld\n", this->lines );
  printf( "Asociatividad: %ld\n", this->assoc );
  printf( "  Tam. bloque: %ld\n", this->block );
  printf( "Tiempo niv. 1: %ld\n", this->timeLevel1 );
  printf( "Tiempo niv. 2: %ld\n", this->timeLevel2 );
  printf( " Asigna Escr.: %ld\n", this->alloc );
  printf( "Estrat. Escr.: %ld\n", this->write );
  switch (this->algor) {
  case 0:
    printf( "Algor. Reempl: %s\n", "FIFO" );
    break;

  case 1:
    printf( "Algor. Reempl: %s\n", "Random" );
    break;

  case 2:
    printf( "Algor. Reempl: %s\n", "LRU" );
    break;

  default:
    printf( "Algor. Reempl: %s\n", "FIFO" );
    break;
  }
  printf("Archivo traza: %s\n", this->traceFile.c_str());
}

void Cache::print_stats() {
  printf( "Estadísticas de la memoria cache\n" );
  printf( "Total Loads:  %d\n", this->total_loads );
  printf( "Total Stores: %d\n", this->total_stores );
  printf( "Load hits:    %d\n", this->load_hits );
  printf( "Load misses:  %d\n", this->load_misses );
  printf( "Store hits:   %d\n", this->store_hits );
  printf( "Store misses: %d\n", this->store_misses );
  printf( "Total CPU cycles: %d\n", this->total_cpu_cycles );
}

void Cache::read_trace() {
  Trace * trace = new Trace( this->traceFile );
  char operation;
  long address;

  printf("Contenidos del Archivo Trace\n");
  while ( trace->hasNext() ) {
    trace->getNext( &operation, &address );
    if ( '/' != operation ) {
      printf( "%c %ld\n", operation, address );
      if ( 'L' == operation ) {
        load( address, 0 );
      } else if ( 'S' == operation ) {
        store( address, 0 );
      }
    }
  }
}

void Cache::run() {
  print_configuration();
  read_trace();
  print_stats();
}

