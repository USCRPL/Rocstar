///
/// @file
/// @brief Test results utility implementation
/// @ingroup irad_group
///
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TestResults.H"

namespace IRAD {

  int TestResults(int argc,char *argv[])
  {
    bool verbose = false;
    std::istream *Ins = &std::cin;
    std::ifstream Inf;
    if(argc < 2){
      std::cout << "Usage:\n\n"
		<< argv[0] << " [name of test] <testing results file>" 
		<< std::endl;
      return(1);
    }
    std::string name_of_test(argv[1]);
    std::string name_of_results_file(argv[2]);
    if(argc > 2){
      Inf.open(argv[2]);
      if(!Inf){
	std::cout << "Error: Could not open indicated results file, " 
		  << argv[2] << "." << std::endl;
	return(1);
      }
      Ins = &Inf;
    }
    if(argc > 3)
      verbose = true;
    std::string line;
    if(verbose)
      std::cout << "Searching for " << name_of_test << " in " 
                << name_of_results_file << std::endl;
    while(std::getline(*Ins,line)){
      std::string::size_type x = line.find("=");
      if(x != std::string::npos){
	std::string testname(line.substr(0,x));
	std::string testresult(line.substr(x+1));
	testname = testname.substr(0,name_of_test.size());
	if(testname == name_of_test){
          if(verbose)
            std::cout << "Found " << name_of_test << ", reading result from "
                      << testresult << std::endl;
	  std::istringstream Istr(testresult);
	  int result = -1;
	  Istr >> result;
	  if(result == 1)
	    return(0);
	  else
	    return(1);
	}
      }
    }
    return(1);
  }
};

int main(int argc,char *argv[])
{
  return(IRAD::TestResults(argc,argv));
}
