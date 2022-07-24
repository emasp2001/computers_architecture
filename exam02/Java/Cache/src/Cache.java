import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Random;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public class Cache {
  // Memory attributes.
  String name;
  int cache[];
  int cache_sets;
  int blocks_sets;
  int bytes_per_block;
  Boolean write_allocate;
  Boolean no_write_allocate;
  Boolean write_back;
  Boolean write_through;
  String replace_algorithm;
  String time_first_access;
  String time_n_access;
  
  // Statistics attributes.
  int total_loads;
  int total_stores;
  int total_load_hits;
  int total_load_misses;
  int total_store_hits;
  int total_store_misses;
  int total_cpu_cycles;

  // A constructor for this class that initializes the cache parameters.
  public Cache(int cache_sets, int blocks_sets, int bytes_per_block, Boolean write_allocate, Boolean no_write_allocate, Boolean write_back, Boolean write_through, String replace_algorithm, String time_first_access, String time_n_access, String name) {
    // Memory attributes.
    this.cache_sets = cache_sets;
    this.blocks_sets = blocks_sets;
    this.bytes_per_block = bytes_per_block;
    this.write_allocate = write_allocate;
    this.no_write_allocate = no_write_allocate;
    this.write_back = write_back;
    this.write_through = write_through;
    this.replace_algorithm = replace_algorithm;
    this.time_first_access = time_first_access;
    this.time_n_access = time_n_access;
    this.name = name;
    this.cache = new int[cache_sets * blocks_sets * bytes_per_block];

    // Statistics attributes.
    this.total_loads = 0;
    this.total_stores = 0;
    this.total_load_hits = 0;
    this.total_load_misses = 0;
    this.total_store_hits = 0;
    this.total_store_misses = 0;
    this.total_cpu_cycles = 0;
  }

  // A simulate method that simulates the cache.
  public void simulate() {
    // Read from command line the file name
    String file_name = "config.txt";
    Cache cache = read_cache_parameters(file_name);
    // Read from command line the file name
    file_name = "trace.txt";
    cache.read_trace(file_name);
    // Print the statistics.
    cache.print_statistics();
  }

  // A method to read the parameters for the cache from a file: cache_parameters.txt
  public static Cache read_cache_parameters(String file_name) {
    Cache my_cache = null;
    try {
      File file = new File(file_name);
      Scanner scanner = new Scanner(file);
      int cache_sets = scanner.nextInt();
      int blocks_sets = scanner.nextInt();
      int bytes_per_block = scanner.nextInt();
      Boolean write_allocate = scanner.nextBoolean();
      Boolean no_write_allocate = scanner.nextBoolean();
      Boolean write_back = scanner.nextBoolean();
      Boolean write_through = scanner.nextBoolean();
      String replace_algorithm = scanner.next();
      String time_first_access = scanner.next();
      String time_n_access = scanner.next();
      String name = scanner.next();
      scanner.close();
      my_cache = new Cache(cache_sets, blocks_sets, bytes_per_block, write_allocate, no_write_allocate, write_back, write_through, replace_algorithm, time_first_access, time_n_access, name);
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    }
    return my_cache;
  }

  // A method to read the trace from a file: trace.txt
  public void read_trace(String file_name) {
    try {
      File file = new File(file_name);
      Scanner scanner = new Scanner(file);
      while (scanner.hasNext()) {
        String instruction = scanner.next();
        if (instruction.equals("L")) {
          int address = scanner.nextInt();
          int set = get_set(address);
          int tag = get_tag(address);
          int block = get_block(address);
          int byte_offset = get_byte_offset(address);
          int cache_index = get_cache_index(set, block);
          int cache_tag = get_cache_tag(set, tag);
          int cache_byte_offset = get_cache_byte_offset(set, byte_offset);
          int cache_data = get_cache_data(cache_index, cache_tag, cache_byte_offset);
          if (cache_data == -1) {
            total_load_misses++;
            total_cpu_cycles += get_cpu_cycles(time_first_access);
            total_cpu_cycles += get_cpu_cycles(time_n_access);
          } else {
            total_load_hits++;
            total_cpu_cycles += get_cpu_cycles(time_first_access);
          }
          total_loads++;
        } else if (instruction.equals("S")) {
          int address = scanner.nextInt();
          int set = get_set(address);
          int tag = get_tag(address);
          int block = get_block(address);
          int byte_offset = get_byte_offset(address);
          int cache_index = get_cache_index(set, block);
          int cache_tag = get_cache_tag(set, tag);
          int cache_byte_offset = get_cache_byte_offset(set, byte_offset);
          int cache_data = get_cache_data(cache_index, cache_tag, cache_byte_offset);
          if (cache_data == -1) {
            total_store_misses++;
            total_cpu_cycles += get_cpu_cycles(time_first_access);
            total_cpu_cycles += get_cpu_cycles(time_n_access);
          } else {
            total_store_hits++;
            total_cpu_cycles += get_cpu_cycles(time_first_access);
          }
          total_stores++;
        }
      }
      scanner.close();
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    }
  }

  // A method to print the statistics.
  public void print_statistics() {
    System.out.println("Cache name: " + name);
    System.out.println("Total loads: " + total_loads);
    System.out.println("Total stores: " + total_stores);
    System.out.println("Total load hits: " + total_load_hits);
    System.out.println("Total load misses: " + total_load_misses);
    System.out.println("Total store hits: " + total_store_hits);
    System.out.println("Total store misses: " + total_store_misses);
    System.out.println("Total CPU cycles: " + total_cpu_cycles);
  }

  // A method to get the set number from the address.
  public int get_set(int address) {
    return (address / (blocks_sets * bytes_per_block)) % cache_sets;
  }

  // A method to get the tag number from the address.
  public int get_tag(int address) {
    return (address / (blocks_sets * bytes_per_block)) / cache_sets;
  }

  // A method to get the block number from the address.
  public int get_block(int address) {
    return (address / bytes_per_block) % blocks_sets;
  }

  // A method to get the byte offset from the address.
  public int get_byte_offset(int address) {
    return address % bytes_per_block;
  }

  // A method to get the cache index from the set and block numbers.
  public int get_cache_index(int set, int block) {
    return set * blocks_sets + block;
  }

  // A method to get the cache tag from the set and tag numbers.
  public int get_cache_tag(int set, int tag) {
    return set * cache_sets + tag;
  }

  // A method to get the cache byte offset from the set and byte offset numbers.
  public int get_cache_byte_offset(int set, int byte_offset) {
    return set * bytes_per_block + byte_offset;
  }

  // A method to get the cache data from the cache index, cache tag and cache byte offset numbers.
  public int get_cache_data(int cache_index, int cache_tag, int cache_byte_offset) {
    // return cache[cache_index][cache_tag][cache_byte_offset];
    return cache[cache_index * cache_tag * cache_byte_offset];
  }

  // A method to get the CPU cycles from the time first access and time n access.
  public int get_cpu_cycles(String time_first_access) {
    int cpu_cycles = Integer.parseInt(time_first_access);
    return cpu_cycles;
  }

  // A method to set the cache data from the cache index, cache tag and cache byte offset numbers.
  public void set_cache_data(int cache_index, int cache_tag, int cache_byte_offset, int cache_data) {
    // cache[cache_index][cache_tag][cache_byte_offset] = cache_data;
    cache[cache_index * cache_tag * cache_byte_offset] = cache_data;
  }

  // The replacement algorithm.
  public void replace_algorithm(int cache_index, int cache_tag, int cache_byte_offset, int cache_data) {
    if (replace_algorithm.equals("LRU")) {
      replace_lru(cache_index, cache_tag, cache_byte_offset, cache_data);
    } else if (replace_algorithm.equals("FIFO")) {
      replace_fifo(cache_index, cache_tag, cache_byte_offset, cache_data);
    } else if (replace_algorithm.equals("Random")) {
      replace_random(cache_index, cache_tag, cache_byte_offset, cache_data);
    }
  }

  // The LRU replacement algorithm.
  public void replace_lru(int cache_index, int cache_tag, int cache_byte_offset, int cache_data) {
    int lru_index = 0;
    int lru_tag = 0;
    int lru_byte_offset = 0;
    int lru_data = 0;
    for (int i = 0; i < blocks_sets; i++) {
      for (int j = 0; j < cache_sets; j++) {
        for (int k = 0; k < bytes_per_block; k++) {
          if (cache[cache_index * cache_tag * cache_byte_offset] == -1) {
            lru_index = cache_index;
            lru_tag = cache_tag;
            lru_byte_offset = cache_byte_offset;
            lru_data = cache_data;
          }
          cache_index++;
        }
      }
    }
    cache_index = lru_index;
    cache_tag = lru_tag;
    cache_byte_offset = lru_byte_offset;
    cache_data = lru_data;
  }

  // The FIFO replacement algorithm.
  public void replace_fifo(int cache_index, int cache_tag, int cache_byte_offset, int cache_data) {
    int fifo_index = 0;
    int fifo_tag = 0;
    int fifo_byte_offset = 0;
    int fifo_data = 0;
    for (int i = 0; i < blocks_sets; i++) {
      for (int j = 0; j < cache_sets; j++) {
        for (int k = 0; k < bytes_per_block; k++) {
          if (cache[cache_index * cache_tag * cache_byte_offset] == -1) {
            fifo_index = cache_index;
            fifo_tag = cache_tag;
            fifo_byte_offset = cache_byte_offset;
            fifo_data = cache_data;
          }
          cache_index++;
        }
      }
    }
    cache_index = fifo_index;
    cache_tag = fifo_tag;
    cache_byte_offset = fifo_byte_offset;
    cache_data = fifo_data;
  }

  // The Random replacement algorithm.
  public void replace_random(int cache_index, int cache_tag, int cache_byte_offset, int cache_data) {
    int random_index = 0;
    int random_tag = 0;
    int random_byte_offset = 0;
    int random_data = 0;
    for (int i = 0; i < blocks_sets; i++) {
      for (int j = 0; j < cache_sets; j++) {
        for (int k = 0; k < bytes_per_block; k++) {
          if (cache[cache_index * cache_tag * cache_byte_offset] == -1) {
            random_index = cache_index;
            random_tag = cache_tag;
            random_byte_offset = cache_byte_offset;
            random_data = cache_data;
          }
          cache_index++;
        }
      }
    }
    cache_index = random_index;
    cache_tag = random_tag;
    cache_byte_offset = random_byte_offset;
    cache_data = random_data;
  }

}

// Create a based configuration file for the cache.
// Name: cache.txt
// Format: cache_sets, cache_blocks, cache_bytes_per_block, cache_write_allocate, cache_no_write_allocate, cache_write_back, cache_write_through, cache_replace_algorithm, cache_time_first_access, cache_time_n_access, cache_name
// Example: cache_sets, cache_blocks, cache_bytes_per_block, cache_write_allocate, cache_no_write_allocate, cache_write_back, cache_write_through, cache_replace_algorithm, cache_time_first_access, cache_time_n_access, cache_name
// Example: 4, 4, 4, write_allocate, no_write_allocate, write_back, write_through, fifo, 1, 1, cache_name

