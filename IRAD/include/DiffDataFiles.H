///
/// @file 
/// @ingroup irad_group
/// @brief Implements a command-line interface for ensuring that two data
/// files are consistent.
///
#ifndef __DIFF_DATA_FILES_H__
#define __DIFF_DATA_FILES_H__

namespace IRAD 
{

  ///
  /// ComLine object for test running utility.
  ///
  class DDFComLine : public IRAD::Util::ComLineObject
  {
  public:
    DDFComLine()
      : IRAD::Util::ComLineObject()
    {};
    DDFComLine(const char *args[])
      : IRAD::Util::ComLineObject(args)
    {};
    void Initialize(){
      std::ostringstream Ostr;
      AddArgument("file1",1);
      AddArgHelp("file1","First file to read in for comparison against file2.");
      AddArgument("file2",1);
      AddArgHelp("file2","Second file to read in for comparison against file1.");
      AddOption('h',"help");
      AddHelp("help","Print out long version of help and exit.");
      AddOption('v',"verblevel",1,"level");
      AddHelp("verblevel","Set the verbosity level. (default = 1)");
      AddOption('o',"output",2,"filename");
      AddHelp("output","Set the output file to <filename>. (default = stdout)");
      AddOption('t',"tolerance",1,"tolerance");
      Ostr << "Set the numerical tolerance for comparing numbers to <tolerance>."
           << "\n\t\t(The default for the tolerance is 1.0e-12.)"
           << "\n\t\t(The default behavior without -t is to compare numbers as strings.)"
           << "\n\t\t(This flag will automatically force the -b flag to be used.)";
      AddHelp("tolerance",Ostr.str());
      AddOption('n',"numbers");
      Ostr.str("");
      Ostr << "Only compare the numbers in the two files."
           << "\n\t\t(This flag will automatically force the -t flag to be used.)";
      AddHelp("numbers",Ostr.str());
      AddOption('b',"blank");
      AddHelp("blank","Ignore blank space between words (or numbers).");
      Ostr.str("");
      Ostr << "Command-line interface for comparing data files.";
      _description.assign(Ostr.str());
    };
  };

  ///
  /// Compare two data files within a given tolerance.
  ///
  /// @param argc The number of command line arguments
  /// @param argv The command line arguments
  /// @returns 0 on success, 1 otherwise
  ///
  /// Command-line interface for diffdatafiles:
  ///
  ///            diffdatafiles [-h] [-v [level] ] <file1> <file2>
  ///
  ///            -h,--help
  ///                 Print out long version of help and exit.
  ///
  ///            -v,--verblevel [level]
  ///                 Set the verbosity level. (default = 1)
  ///
  /// This function is designed to compare two data files with 
  /// numerical solution data in mind. It will compare data within
  /// a given tolerence.
  /// 
  int DiffDataFiles(int argc,char *argv[]);

};
#endif
