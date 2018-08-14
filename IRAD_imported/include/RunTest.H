///
/// @file 
/// @ingroup irad_group
/// @brief Implements a command-line interface for runnning tests.
///
#ifndef __RUN_TEST_H__
#define __RUN_TEST_H__

namespace IRAD 
{

  ///
  /// ComLine object for test running utility.
  ///
  class RTSComLine : public IRAD::Util::ComLineObject
  {
  public:
    RTSComLine()
      : IRAD::Util::ComLineObject()
    {};
    RTSComLine(const char *args[])
      : IRAD::Util::ComLineObject(args)
    {};
    void Initialize(){
      AddOption('h',"help");
      AddHelp("help","Print out long version of help and exit.");
      AddOption('v',"verblevel",1,"level");
      AddHelp("verblevel","Set the verbosity level. (default = 1)");
      AddOption('o',"output",2,"filename");
      AddHelp("output","Set the output file to <filename>. (default = stdout)");
      AddOption('l',"list",2,"filename");
      AddOption('b',"bindir",2,"path");
      AddHelp("bindir","Set a binary path to <path>. (no default)");
      AddOption('s',"srcdir",2,"path");
      AddHelp("srcdir","Set a source path to <path>. (no default)");
      std::ostringstream Ostr;
      Ostr << "Set the list file name to <filename>."
           << "The list file should be a text file with one test name per line.";
      AddHelp("list",Ostr.str());
      AddOption('f',"file",2,"filename");
      AddHelp("file","Run the particular test specified by <filename>.");
      AddOption('a',"argument",2,"argument");
      Ostr.str("");
      Ostr << "Provide an extra argument in <argument> for runtest to pass"
           << " to the testing script(s).";
      AddHelp("argument",Ostr.str());
      //      AddOption('n',"name",2,"TestName");
      //      AddHelp("name","Run test by name. (no default) [disabled!]");
      AddOption('H',"hostname",2,"HostName");
      AddHelp("host","Set the host to <HostName>. (no default)");
      AddOption('p',"platforms",2,"filename");
      AddHelp("platforms","Set the platforms file to <filename>. (default = ./share/Platforms/platforms)");
      Ostr.str("");
      Ostr << "Command-line interface for running tests..";
      _description.assign(Ostr.str());
    };
  };

  ///
  /// Runs test programs and scripts through a standard interface.
  ///
  /// @param argc The number of command line arguments
  /// @param argv The command line arguments
  /// @returns 0 on success, 1 otherwise
  ///
  /// Command-line interface for runtest:
  ///
  ///            runtest [-h] [-v [level] -o <filename> -l <filename> -f <filename> -H <HostName> -p <filename> ] 
  ///
  ///            -h,--help
  ///                 Print out long version of help and exit.
  ///
  ///            -v,--verblevel [level]
  ///                 Set the verbosity level. (default = 1)
  ///
  ///            -o,--output <filename>
  ///                 Set the output file to <filename>. (default = stdout)
  ///
  ///            -l,--list <filename>
  ///                 Set the list file name to <filename>.The list file should be a text file with one test name per line.
  ///
  ///            -f,--file <filename>
  ///                 Run the particular test specified by <filename>.
  ///
  ///            -H,--hostname <HostName>
  ///           
  ///            -p,--platforms <filename>
  ///                 Set the platforms file to <filename>. (default = ./share/Platforms/platforms)
  ///
  ///
  /// This function is designed to run complex tests by invoking
  /// test programs and scripts.  
  /// 
  int RunTest(int argc,char *argv[]);

};
#endif
