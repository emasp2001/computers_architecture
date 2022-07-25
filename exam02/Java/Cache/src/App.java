import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class App {
  public static void main(String[] args) {
    cache.print_statistics();
    Cache cache = read_cache_parameters("/Users/emma/Documents/university/Arquitectura_Computadoras/github_repo-arqui_computadores/exam02/Java/src/config.txt");
    cache.simulate("/Users/emma/Documents/university/Arquitectura_Computadoras/github_repo-arqui_computadores/exam02/Java/src/trace.txt");
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
      return my_cache;
    } catch (FileNotFoundException e) {
      System.out.println("hubo un error");
      e.printStackTrace();
      return null;
    }
  }
}
