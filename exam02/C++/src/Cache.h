#include <string>

class Cache {
  public:
    Cache(long lines, long assoc, long block, long timeLevel1, long timeLevel2, long alloc, long write, long algor, std::string traceFile="../data/Traza-1.txt");
    ~Cache();
    void run();
    void print_configuration();
    void print_stats();
    
    // Configuration
    long lines;
    long assoc;
    long block;
    long timeLevel1;
    long timeLevel2;
    long alloc; // 0 = No-write-allocate, 1 = Write-allocate
    long write; // 0 = write-back, 1 = write-through
    long algor; // 0 = FIFO, 1 = Random, 2 = LRU
    std::string traceFile;

    // Statistics
    int total_loads;
    int total_stores;
    int load_hits;
    int load_misses;
    int store_hits;
    int store_misses;
    int total_cpu_cycles;

  private:
    // Replacement algorithms
    void replace();
    void replace_lru();
    void replace_random();
    void replace_fifo();

    // Cache memory
    long** cache;

    // Cache functions
    void store( long, long );
    void load( long, long );

    // Simulation
    void read_trace();
};