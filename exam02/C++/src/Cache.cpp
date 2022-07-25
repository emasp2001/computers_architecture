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

void Cache::run() {
  print_configuration();
  read_trace();
  print_stats();
}

long Cache::get_set(long address, long *set) {
  *set = address / this->block;
  return *set;
}

long Cache::get_tag(long address, long *tag) {
  *tag = address / (this->block * this->assoc);
  return *tag;
}

long Cache::get_index(long address, long *index) {
  *index = (address % (this->block * this->assoc)) / this->block;
  return *index;
}

long Cache::get_block(long address, long *block) {
  *block = address % this->block;
  return *block;
}

long Cache::get_offset(long address, long *offset) {
  *offset = address % this->block;
  return *offset;
}

long Cache::get_address(long set, long tag, long block) {
  long address = (set * this->block) + block;
  return address;
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
        int set = this->get_set( address, NULL );
        int tag = this->get_tag( address, NULL );
        int index = this->get_index( address, NULL );
        int block = this->get_block( address, NULL );
        int offset = this->get_offset( address, NULL );
        long address = this->get_address( set, tag, block );
        printf( "  set: %d\n", set );
        printf( "  tag: %d\n", tag );
        printf( "  index: %d\n", index );
        printf( "  block: %d\n", block );
        printf( "  offset: %d\n", offset );
        printf( "  address: %ld\n", address );
        this->load( set, tag, block );
      } else if ( 'S' == operation ) {
        int set = this->get_set( address, NULL );
        int tag = this->get_tag( address, NULL );
        int index = this->get_index( address, NULL );
        int block = this->get_block( address, NULL );
        int offset = this->get_offset( address, NULL );
        long address = this->get_address( set, tag, block );
        printf( "  set: %d\n", set );
        printf( "  tag: %d\n", tag );
        printf( "  index: %d\n", index );
        printf( "  block: %d\n", block );
        printf( "  offset: %d\n", offset );
        printf( "  address: %ld\n", address );
        this->store( set, tag, block );
      }
    }
  }
}

// The load method is called when a load operation is issued.
void Cache::load(int set, int tag, int block ) {
  // Increment the total loads counter.
  this->total_loads++;
  // Get the set index.
  int index = this->get_index( set, NULL );
  // Get the tag.
  int tag_ = this->get_tag( tag, NULL );
  // Get the index.
  int index_ = this->get_index( index, NULL );
  // Get the block.
  int block_ = this->get_block( block, NULL );
  // Get the offset.
  int offset = this->get_offset( block, NULL );
  // Get the address.
  long address = this->get_address( set, tag, block );
  // Check if the address is in the cache.
  if ( this->is_in_cache( set, tag ) ) {
    // Increment the load hits counter.
    this->load_hits++;
  } else {
    // Increment the load misses counter.
    this->load_misses++;
    // Check if the cache is full.
    if ( this->is_full() ) {
      // Check if the cache is full and the replacement algorithm is FIFO.
      if ( 0 == this->algor ) {
        // Get the first block in the set.
        int first_block = this->get_first_block( set );
        // Remove the first block from the cache.
        this->remove_block( set, first_block );
      } else if ( 1 == this->algor ) {
        // Get a random block in the set.
        int random_block = this->get_random_block( set );
        // Remove the random block from the cache.
        this->remove_block( set, random_block );
      } else if ( 2 == this->algor ) {
        // Get the LRU block in the set.
        int lru_block = this->get_lru_block( set );
        // Remove the LRU block from the cache.
        this->remove_block( set, lru_block );
      }
    }
    // Add the block to the cache.
    this->add_block( set, tag, block );
  }
}

// The store method is called when a store operation is issued.
void Cache::store(int set, int tag, int block ) {
  // Increment the total stores counter.
  this->total_stores++;
  // Get the set index.
  int index = this->get_index( set, NULL );
  // Get the tag.
  int tag_ = this->get_tag( tag, NULL );
  // Get the index.
  int index_ = this->get_index( index, NULL );
  // Get the block.
  int block_ = this->get_block( block, NULL );
  // Get the offset.
  int offset = this->get_offset( block, NULL );
  // Get the address.
  long address = this->get_address( set, tag, block );
  // Check if the address is in the cache.
  if ( this->is_in_cache( set, tag ) ) {
    // Increment the store hits counter.
    this->store_hits++;
  } else {
    // Increment the store misses counter.
    this->store_misses++;
    // Check if the cache is full.
    if ( this->is_full() ) {
      // Check if the cache is full and the replacement algorithm is FIFO.
      if ( 0 == this->algor ) {
        // Get the first block in the set.
        int first_block = this->get_first_block( set );
        // Remove the first block from the cache.
        this->remove_block( set, first_block );
      } else if ( 1 == this->algor ) {
        // Get a random block in the set.
        int random_block = this->get_random_block( set );
        // Remove the random block from the cache.
        this->remove_block( set, random_block );
      } else if ( 2 == this->algor ) {
        // Get the LRU block in the set.
        int lru_block = this->get_lru_block( set );
        // Remove the LRU block from the cache.
        this->remove_block( set, lru_block );
      }
    }
    // Add the block to the cache.
    this->add_block( set, tag, block );
  }
}

bool Cache::is_in_cache(int set, int tag) {
  // Get the set index.
  int index = this->get_index( set, NULL );
  // Get the tag.
  int tag_ = this->get_tag( tag, NULL );
  // Get the index.
  int index_ = this->get_index( index, NULL );
  // Check if the tag is in the set.
  if ( this->sets[index_].tag == tag_ ) {
    // Return true.
    return true;
  } else {
    // Return false.
    return false;
  }
}

// The is_full method returns true if the cache is full.
bool Cache::is_full() {
  // Check if the number of blocks in the cache is equal to the number of blocks in the set.
  if ( this->block == this->assoc ) {
    // Return true.
    return true;
  } else {
    // Return false.
    return false;
  }
}

// The get_first_block method returns the first block in the set.
int Cache::get_first_block(int set) {
  // Get the set index.
  int index = this->get_index( set, NULL );
  // Get the first block in the set.
  int first_block = this->sets[index].first_block;
  // Return the first block.
  return first_block;
}

// The remove_block method removes a block from the cache.
void Cache::remove_block(int set, int block) {
  // Get the set index.
  int index = this->get_index( set, NULL );
  // Get the tag.
  int tag = this->get_tag( block, NULL );
  // Get the index.
  int index_ = this->get_index( index, NULL );
  // Get the block.
  int block_ = this->get_block( block, NULL );
  // Get the offset.
  int offset = this->get_offset( block, NULL );
  // Get the address.
  long address = this->get_address( set, tag, block );
  // Remove the block from the set.
  this->sets[index_].blocks[block_].tag = -1;
  this->sets[index_].blocks[block_].block = -1;
  this->sets[index_].blocks[block_].offset = -1;
  this->sets[index_].blocks[block_].address = -1;
  this->sets[index_].blocks[block_].lru = -1;
  this->sets[index_].blocks[block_].valid = false;
  // Update the first block.
  this->sets[index_].first_block = block_;
  // Update the LRU.
  this->sets[index_].lru = block_;
}

// The get_random_block method returns a random block in the set.
int Cache::get_random_block(int set) {
  // Get the set index.
  int index = this->get_index( set, NULL );
  // Get the random block in the set.
  int random_block = this->sets[index].random_block;
  // Return the random block.
  return random_block;
}

// The get_lru_block method returns the LRU block in the set.
int Cache::get_lru_block(int set) {
  // Get the set index.
  int index = this->get_index( set, NULL );
  // Get the LRU block in the set.
  int lru_block = this->sets[index].lru;
  // Return the LRU block.
  return lru_block;
}

// The add_block method adds a block to the cache.
void Cache::add_block(int set, int tag, int block) {
  // Get the set index.
  int index = this->get_index( set, NULL );
  // Get the tag.
  int tag_ = this->get_tag( tag, NULL );
  // Get the index.
  int index_ = this->get_index( index, NULL );
  // Get the block.
  int block_ = this->get_block( block, NULL );
  // Get the offset.
  int offset = this->get_offset( block, NULL );
  // Get the address.
  long address = this->get_address( set, tag, block );
  // Add the block to the set.
  this->sets[index_].blocks[block_].tag = tag_;
  this->sets[index_].blocks[block_].block = block_;
  this->sets[index_].blocks[block_].offset = offset;
  this->sets[index_].blocks[block_].address = address;
  this->sets[index_].blocks[block_].lru = block_;
  this->sets[index_].blocks[block_].valid = true;
  // Update the first block.
  this->sets[index_].first_block = block_;
  // Update the LRU.
  this->sets[index_].lru = block_;
  // Update the random block.
  this->sets[index_].random_block = block_;
}