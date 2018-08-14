#include <iostream>
#include <limits>

#include "CheckResults.H"

namespace IRAD {
  namespace Util {
int CheckResults(int argc,char *argv[])
{
  CheckResultsComLine comline((const char **)argv);
  comline.Initialize();
  int clerr = comline.ProcessOptions();
  if(!comline.GetOption("help").empty()){
    std::cout << comline.LongUsage() << std::endl;
    return(0);
  }
  int verb_level = 1;
  std::string sverb = comline.GetOption("verbosity");
  std::istringstream Istr(sverb);
  Istr >> verb_level;
  if(verb_level < 0 || verb_level > 3)
    verb_level = 1;
  double lower_limit = -std::numeric_limits<double>::max();
  double upper_limit = std::numeric_limits<double>::max();
  std::string range = comline.GetOption("range");
  std::string ident = comline.GetOption("identifier");
  std::string name = comline.GetOption("name");
  bool passed = false;
  if(name.empty())
    name = ident;
  if(verb_level > 2)
    std::cerr << "Identifier: " << ident << std::endl;
  comline.ProcessRange<double>(lower_limit,upper_limit,range);
  if(verb_level > 2)
    std::cerr << "Processed range: (" << lower_limit << ":" << upper_limit 
              << ")" << std::endl;
  std::string line;
  while(std::getline(std::cin,line)){
    std::string::size_type x = line.find(ident);
    if(x != std::string::npos){
      x = line.find(":");
      if(x != std::string::npos){
        std::string snumbers = line.substr(x+1);
        std::istringstream NumIn(snumbers);
        double cnum = 0;
        NumIn >> cnum;
        passed = ((cnum >= lower_limit) && (cnum <= upper_limit));
        if(verb_level)
          std::cout << name << " = " << (passed ? 1 : 0) << std::endl;
        if((verb_level > 1) && !passed){
          std::cerr << "Test failed: " << cnum << " is not in range ("
                    << lower_limit << ":" << upper_limit << ")" << std::endl;
        }
        break;
      }
    }
  }
  if(!passed)
    return(1);
  return(0);
}
  } }

int main(int argc,char *argv[])
{
  return(IRAD::Util::CheckResults(argc,argv));
}
