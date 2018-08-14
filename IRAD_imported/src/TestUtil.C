///
/// @file 
/// @ingroup irad_group
/// @brief Implementation of the IRAD::Util namespace tests
///
#include "UtilTest.H"

namespace IRAD {
  namespace Util {

    ///
    /// Drives the IRAD::Util namespace's test object.
    /// 
    /// @param argc number of string command line tokens
    /// @param argv string command line tokens
    /// @returns 0 if successful, 1 otherwise
    /// @ingroup irad_group
    ///
    int UtilTest(int argc,char *argv[])
    {
      Util::TestObject  test;
      Util::TestResults results;
      test.Process(results);
      std::ofstream Ouf;
      std::ostream *Out = &std::cout;
      if(argc > 1){
	Ouf.open(argv[1]);
	if(!Ouf){
	  std::cout << "Error: Could not open output file, " << argv[1] << "." 
		    << std::endl;
	  return(1);
	}
	Out = &Ouf;
      }
      *Out << results << std::endl;
      if(Ouf)
	Ouf.close();
      return(0);
    }
  };
};

int main(int argc,char *argv[])
{
  return(IRAD::Util::UtilTest(argc,argv));
}
