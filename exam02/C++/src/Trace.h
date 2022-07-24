/**
 *
 */

#include <fstream>

class Trace {
   public:
      Trace( std::string );
      ~Trace();
      bool hasNext();
      bool getNext( char *, long * );

   private:
      std::ifstream file;
      std::string line;

};

