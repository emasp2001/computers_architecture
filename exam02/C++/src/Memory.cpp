// A simple Studen class:
// Language: cpp
// Path: C++/src/Student.cpp

#include <string>
#include <iostream>
#include "Trace.cc"

class Cache {
  public:
  // Configuration
  int sets;
  int block_set;
  int bytes_block;
  int timeLevel1;
  int timeLevel2;
  int alloc; // 0 = No-write-allocate, 1 = Write-allocate
  int write; // 0 = write-back, 1 = write-through
  int algor; // 0 = FIFO, 1 = Random, 2 = LRU
  std::string traceFile;
  int** cache;

  // Statistics
  int total_loads;
  int total_stores;
  int load_hits;
  int load_misses;
  int store_hits;
  int store_misses;
  int total_cpu_cycles;

  Cache(int sets, int block_set, int bytes_block, int timeLevel1, int timeLevel2, int alloc, int write, int algor, std::string traceFile="../data/Traza-1.txt") {
    this->sets = sets;
    this->block_set = block_set;
    this->bytes_block = bytes_block;
    this->timeLevel1 = timeLevel1;
    this->timeLevel2 = timeLevel2;
    this->alloc = alloc;
    this->write = write;
    this->algor = algor;
    this->traceFile = traceFile;
    this->cache = new int*[sets];
    for (int i = 0; i < sets; i++) {
      this->cache[i] = new int[block_set];
      for (int j = 0; j < block_set; j++) {
        this->cache[i][j] = -1;
      }
    }
    this->total_loads = 0;
    this->total_stores = 0;
    this->load_hits = 0;
    this->load_misses = 0;
    this->store_hits = 0;
    this->store_misses = 0;
    this->total_cpu_cycles = 0;
  }

  ~Cache() {
    for (int i = 0; i < sets; i++) {
      delete[] this->cache[i];
    }
    delete[] this->cache;
  }

  void print_configuration() {
    printf( "Configuración de la memoria cache\n" );
    printf( "       Líneas: %d\n", this->sets );
    printf( "Asociatividad: %d\n", this->block_set );
    printf( "  Tam. bloque: %d\n", this->bytes_block );
    printf( "Tiempo niv. 1: %d\n", this->timeLevel1 );
    printf( "Tiempo niv. 2: %d\n", this->timeLevel2 );
    printf( " Asigna Escr.: %d\n", this->alloc );
    printf( "Estrat. Escr.: %d\n", this->write );
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

  void print_stats() {
    printf( "Estadísticas de la memoria cache\n" );
    printf( "Total Loads:  %d\n", this->total_loads );
    printf( "Total Stores: %d\n", this->total_stores );
    printf( "Load hits:    %d\n", this->load_hits );
    printf( "Load misses:  %d\n", this->load_misses );
    printf( "Store hits:   %d\n", this->store_hits );
    printf( "Store misses: %d\n", this->store_misses );
    printf( "Total CPU cycles: %d\n", this->total_cpu_cycles );
  }

  int get_set(int address) {
    return address / (this->bytes_block * this->block_set);
  }

  int get_tag(int address) {
    return address / (this->bytes_block * this->block_set);
  }

  int get_offset(int address) {
    return address % (this->bytes_block);
  }

  int get_block(int address) {
    return address / this->bytes_block;
  }

  int get_block_address(int address) {
    return address - (address % this->bytes_block);
  }

  int get_block_tag(int address) {
    return address / this->bytes_block;
  }

  int get_block_offset(int address) {
    return address % this->bytes_block;
  }

  int get_block_set(int address) {
    return address / (this->bytes_block * this->block_set);
  }

  int get_block_set_address(int address) {
    return address - (address % (this->bytes_block * this->block_set));
  }

  bool is_in_cache(int address) {
    int set = this->get_set(address);
    int tag = this->get_tag(address);
    for (int i = 0; i < this->block_set; i++) {
      if (this->cache[set][i] == tag) {
        return true;
      }
    }
    return false;
  }
  
  bool is_full() {
    for (int i = 0; i < this->sets; i++) {
      for (int j = 0; j < this->block_set; j++) {
        if (this->cache[i][j] == -1) {
          return false;
        }
      }
    }
    return true;
  }

  int get_first_block(int set) {
    for (int i = 0; i < this->block_set; i++) {
      if (this->cache[set][i] != -1) {
        return this->cache[set][i];
      }
    }
    return -1;
  }

  void remove_block(int set, int block) {
    for (int i = 0; i < this->block_set; i++) {
      if (this->cache[set][i] == block) {
        this->cache[set][i] = -1;
        return;
      }
    }
  }

  int get_random_block(int set) {
    int random_block = -1;
    if (this->is_full()) {
      random_block = this->get_first_block(set);
    } else {
      int random_index = rand() % this->block_set;
      random_block = this->cache[set][random_index];
    }
    return random_block;
  }

  int get_lru_block(int set) {
    int lru_block = -1;
    int lru_index = -1;
    for (int i = 0; i < this->block_set; i++) {
      if (this->cache[set][i] != -1) {
        if (lru_index == -1) {
          lru_index = i;
          lru_block = this->cache[set][i];
        } else {
          if (this->cache[set][i] < lru_block) {
            lru_index = i;
            lru_block = this->cache[set][i];
          }
        }
      }
    }
    return lru_block;
  }

  void add_block(int set, int block) {
    if (this->is_full()) {
      this->remove_block(set, this->get_lru_block(set));
    }
    this->cache[set][this->get_first_block(set)] = block;
  }

  void replace_block_fifo(int set, int block) {
    this->remove_block(set, this->get_first_block(set));
    this->add_block(set, block);
  }

  void replace_block_random(int set, int block) {
    this->remove_block(set, this->get_random_block(set));
    this->add_block(set, block);
  }

  void replace_block_lru(int set, int block) {
    this->remove_block(set, this->get_lru_block(set));
    this->add_block(set, block);
  }

  void replace_block(int set, int block) {
    switch (this->algor) {
    case 0:
      this->replace_block_fifo(set, block);
      break;

    case 1:
      this->replace_block_random(set, block);
      break;

    case 2:
      this->replace_block_lru(set, block);
      break;

    default:
      this->replace_block_fifo(set, block);
      break;
    }
  }

  // A load misses is when the address is not in the cache.
  // A store misses is when the address is not in the cache and the cache is full.
  // A load hit is when the address is in the cache.
  // A store hit is when the address is in the cache and the cache is full.
  void load(int address) {
    this->total_loads++;
    int set = this->get_set(address);
    int tag = this->get_tag(address);
    if (this->is_in_cache(address)) {
      this->load_hits++;
    } else {
      this->load_misses++;
      if (this->is_full()) {
        this->store_misses++;
      }
      this->replace_block(set, tag);
    }
  }

  void store(int address) {
    this->total_stores++;
    int set = this->get_set(address);
    int tag = this->get_tag(address);
    if (this->is_in_cache(address)) {
      this->store_hits++;
    } else {
      this->store_misses++;
      if (this->is_full()) {
        this->store_misses++;
      }
      this->replace_block(set, tag);
    }
  }

  void read_trace() {
    // std::ifstream trace_file;
    // trace_file.open(this->traceFile);
    // if (!trace_file.is_open()) {
    //   std::cout << "Error opening trace file" << std::endl;
    //   exit(1);
    // }
    // std::string line;
    // while (std::getline(trace_file, line)) {
    //   if (line.substr(0, 1) == "L") {
    //     this->load(std::stoi(line.substr(2)));
    //   } else if (line.substr(0, 1) == "S") {
    //     this->store(std::stoi(line.substr(2)));
    //   }
    // }
    // trace_file.close();

  Trace * trace = new Trace( this->traceFile );
  char operation;
  long address;

  printf("Contenidos del Archivo Trace\n");
  while ( trace->hasNext() ) {
    trace->getNext( &operation, &address );
    if ( '/' != operation ) {
      printf( "%c %ld\n", operation, address );
      if ( 'L' == operation ) {
        this->load( address );
      } else if ( 'S' == operation ) {
        this->store( address );
      }
    }
  }
  }

  void run() {
    this->read_trace();
    print_stats();
  }
};