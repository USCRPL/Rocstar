///
/// @file
/// @ingroup gridconversion_group
/// @brief Testing utilities for GridConversion.
///
#ifndef __TEST_GRIDCONVERSION_H__
#define __TEST_GRIDCONVERSION_H__

#include "GridConversionTest.H"

namespace GridConversion{
  
  ///
  /// ComLineObject for GridConversion testing command-line interface.
  ///
  class TestComLine : public IRAD::Util::ComLineObject
  {
  public:
    TestComLine()
      : IRAD::Util::ComLineObject()
    {};
    TestComLine(const char *args[])
      : IRAD::Util::ComLineObject(args)
    {};
    void Initialize(){
      AddOption('h',"help");
      AddHelp("help","Print out long version of help and exit.");
      AddOption('v',"verblevel",1,"level");
      AddHelp("verblevel","Set the verbosity level. (default = 0)");
      AddOption('o',"output",2,"filename");
      AddHelp("output","Set the output file to <filename>. (default = stdout)");
      AddOption('l',"list",2,"filename");
      std::ostringstream Ostr;
      Ostr << "Set the list file name to <filename>. (no default). "
           << "The list file should be a text file with one test name per line.";
      AddHelp("list",Ostr.str());
      AddOption('n',"name",2,"TestName");
      AddHelp("name","Run test by name. (no default)");
      Ostr.str("");
      Ostr << "Command-line interface for the test of the GridConversion.";
      _description.assign(Ostr.str());
    };
  };
};
#endif
