/**
 *   Compile with: javac -classpath ./commons-cli-1.5.0.jar Main.java
 *
 *   Run with: java -cp commons-cli-1.5.0.jar:. Main
 *
 *   All program values are stored in long variables, change as needed
 */

import org.apache.commons.cli.*;

public class Main {

   public static void main(String[] args) {

      long lines = 4;
      long assoc = 1;
      long block = 16;
      long cyclesL1 = 1;	// Cycles to access level one
      long cyclesL2 = 8;	// Cycles to access level two
      long algo = 0;		// Default to FIFO

      Options options = new Options();
      Option optLines = Option.builder("l")
              .longOpt( "lines" )
              .hasArg()
              .valueSeparator(' ')
              .build();
      Option optSets = Option.builder("s")
              .longOpt( "sets" )
              .hasArg()
              .valueSeparator(' ')
              .build();
      Option optBlock = Option.builder( "b")
              .longOpt( "block" )
              .hasArg()
              .valueSeparator(' ')
              .build();

      Option optT1 = Option.builder( "t1")
              .longOpt( "time-level-1" )
              .hasArg()
              .valueSeparator(' ')
              .build();

      Option optT2 = Option.builder( "t2")
              .longOpt( "time-level-2" )
              .hasArg()
              .valueSeparator(' ')
              .build();

      Option optAlgo = Option.builder( "r")
              .longOpt( "algorithm" )
              .hasArg()
              .valueSeparator(' ')
              .build();

      options.addOption( optLines );	// Cache lines, referenced by index
      options.addOption( optSets );	// Line sets, cache associativity
      options.addOption( optBlock );	// Cache data block size, defaults to 16 bytes
      options.addOption( optT1 );	// Cycles to access level one
      options.addOption( optT2 );	// Cycles to access level two
      options.addOption( optAlgo );	// Cache replacement algorithm, 0 -> FIFO, 1 -> Random, 2 -> LRU

      CommandLineParser parser = new DefaultParser();
      try {
         CommandLine line = parser.parse(options, args);

// Cache lines value
         if ( null != line.getOptionValue( "lines" ) ) {
            lines = Long.parseLong( line.getOptionValue( "lines" ) );
         }

// Cache associativity
         if ( null != line.getOptionValue( "sets" ) ) {
            assoc = Long.parseLong( line.getOptionValue( "sets" ) );
         }

// Cache data block size
         if ( null != line.getOptionValue( "block" ) ) {
            block = Long.parseLong( line.getOptionValue( "block" ) );
         }

// Cycles to access cache level one
         if ( null != line.getOptionValue( "t1" ) ) {
            cyclesL1 = Long.parseLong( line.getOptionValue( "t1" ) );
         }

// Cycles to access cache level two
         if ( null != line.getOptionValue( "t2" ) ) {
            cyclesL2 = Long.parseLong( line.getOptionValue( "t2" ) );
         }

// Cache replacement algorithm
         if ( null != line.getOptionValue( "algorithm" ) ) {
            if ( line.getOptionValue( "algorithm").equals( "lru" ) ) {
               algo = 2;
            } else {
               if ( line.getOptionValue( "algorithm").equals( "random" ) ) {
                  algo = 1;
               }
            }
         }

      } catch ( ParseException e ) {
         System.err.println( "Parsing failed. Reason: " + e.getMessage() );
      }

      System.out.println( "Lines: " + lines );
      System.out.println( " Sets: " + assoc );
      System.out.println( "Block: " + block );
      System.out.println( "   T1: " + cyclesL1 );
      System.out.println( "   T2: " + cyclesL2 );
      System.out.println( " Algo: " + algo );

    }
}
