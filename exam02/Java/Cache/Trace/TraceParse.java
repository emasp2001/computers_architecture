class TraceParse {

   final static char LOAD = 'L';
   final static char STORE= 'S';

   public TraceParse() {
   } 

   public void ParseFile() {
      String operation;
      long address;
	String parseString;
	String[] strings;
	byte c;


      try {
         while (( c = (byte)System.in.read() ) != -1 ) {
            parseString = "";

	    while( c != '\n' ) {	      // Read until a newline
              parseString += (char) c;
              c = (byte)System.in.read();
            }

            strings = parseString.split("\\s");

            // Read the first character of the line.
            // It determines whether to read or write to the cache.
            if ( ! strings[ 0 ].equals( "//" ) ) {	// Comment skip
               if ( strings.length < 2 ) {
                  break;
               }
               operation = strings[ 0 ];

               // Read the address (as a hex number)
               address = hex_to_long( strings[ 1 ] );

               System.out.println( (operation.equals( "L" )? "Load " : "Store ") + " at " + address + "[" + strings[1] + " ]" );
            }
    
         }
      } catch ( Exception e ) {
         e.printStackTrace();
      }

   }
   
   public String long_to_hex(long input ) {
      String hex_string;
      hex_string=Long.toHexString( input );
      while ( hex_string.length() < 8 ) {
         hex_string = "0" + hex_string;
      }
	
      return hex_string;

   }
 
   public static void printCacheAndMemory() {

	
	// Use this code to format and print your output
	/*
	System.out.println("STATISTICS");
	System.out.println("Misses:");
	System.out.println("Total: " + total_misses + " DataReads: " + readmisses + " DataWrites: " + writemisses);
	System.out.println("Miss rate:");
	System.out.println("Total: " + totalmr + " DataReads: " + readmr + " DataWrites: " + writemr);
	System.out.println("Number of Dirty Blocks Evicted from the Cache: " + numevicts);
	System.out.println("CACHE CONTENTS");
	System.out.println("Set   V   Tag      D     Word0     Word1     Word2     Word3     Word4     Word5     Word6     Word7");
	System.out.println(set + "  "+ v + "  " + int_to_hex(tag) + "  " + d + "  " + int_to_hex(word0) + "  " + int_to_hex(word1) + "  "  + int_to_hex(word2)
			   + "  " + int_to_hex(word3) + "  " + int_to_hex(word4) + "  " +  int_to_hex(word5) + "  " + int_to_hex(word6) + "  " + int_to_hex(word7));
	System.out.println();
	System.out.println("MAIN MEMORY:");
	System.out.println("Address   Words");
	System.out.println(int_to_hex(address) + "  " + int_to_hex(memword0) + "  " + int_to_hex(memword1) + "  "  + int_to_hex(memword2) + "  " + int_to_hex(memword3) + "  " + int_to_hex(memword4) + "  " +  int_to_hex(memword5) + "  " + int_to_hex(memword6) + "  " + int_to_hex(memword7));
	*/
   }

   public long hex_to_long( String s ) {
      String digits = "0123456789ABCDEF";
      s = s.toUpperCase();
      long val = 0;
      for ( int i = 0; i < s.length(); i++ ) {
         char c = s.charAt( i );
         long d = digits.indexOf( c );
         val = val << 4 + d;
      }

      return val;

   }

   public static void main(String[] args) throws java.io.IOException {
      TraceParse trace = new TraceParse();

      trace.ParseFile();

    }

}

