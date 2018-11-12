///
/// @file
/// @brief Performs various checks on numerical test results.
/// @ingroup irad_group
///

#ifndef __CHECK_RESULTS_H__
#define __CHECK_RESULTS_H__
#include <sstream>
#include "ComLine.H"
namespace IRAD 
{
  ///
  /// Check numerical results utility.
  ///
  /// @param argc integer indicating how many words are in the command line.
  /// @param argv string words of the command line.
  /// @returns 0 if test was successful, 1 if not.
  ///
  /// This utility numerically checks the numbers on a line
  /// that matches a specified string.  The lines in the file 
  /// are assumed to have the following format: 
  ///
  /// UniqueString : <numerical entries>
  /// 
  /// This utility will perform the indicated test(s) on the
  /// numerical entires associated with the "UniqueString"
  /// identifier given as input.  The result is a binary 
  /// YES(1) or NO(0) which is output to stdout as:
  /// 
  /// TestName = 1||0
  ///
  /// If the test passed(=YES), this utility returns a 0, otherwise it returns 1.
  ///
  int CheckResults(int argc,char *argv[]);

  ///
  /// The ComLineObject for the example programs.
  ///
  /// This comline object implements a basic command-line interface
  /// handler for the example programs.  These command-line objects
  /// handle all of the generation of help messages, command line 
  /// argument validation, and encapsulation.
  ///
  class CheckResultsComLine : public IRAD::Util::ComLineObject
  {
  public:
    CheckResultsComLine()
      : IRAD::Util::ComLineObject()
    {};
    CheckResultsComLine(const char *args[])
      : IRAD::Util::ComLineObject(args)
    {};
    ///
    /// This is where the actual options and arguments are described.
    ///
    /// For details about what can be done in this function, see the
    /// documentation for the IRAD::Util::ComLineObject.
    ///
    void Initialize(){
      AddOption('h',"help");
      AddOption('v',"verbosity",1);
      AddOption('i',"identifier",3,"string");
      AddOption('n',"name",2,"testname");
      AddOption('r',"range",3,"lower:upper");
      AddHelp("help","Prints this long version of help.");
      AddHelp("verbosity","Sets the verbosity level to 1 or the specified level.");
      std::ostringstream Ostr;
      Ostr << "Sets the unique identifying string to <string>. The utility "
           << "\n\t\tlooks for \"string\" to identify the input line containing"
           << "\n\t\tthe result that should be checked. The numerical result is"
           << "\n\t\texpected to be delimited by a colon (:).  In other words..."
           << "\n\t\tthe expected line format is as follows:"
           << "\n\n\t\t\t string : <numerical entries>\n";
      AddHelp("identifier",Ostr.str());
      AddHelp("name","Sets the <testname> to be used for results output.");
      AddHelp("range","Specifies the range to test against.");
      Ostr.str("");
      Ostr.clear();
      // The following commented string formation is kept around to 
      // illustrate the required tabs and newlines to make the output 
      // look proper:
      //
      //    Ostr << "Use fixed problem size in scalability analysis.  Only makes"
      //         << "\n\t\tsense when scalability mode is enabled.";
      //    Ostr.str("");
      Ostr.str("");
      Ostr << "This utility reads a file off stdin, searches for the line with the indicated string"
           << " and performs a numerical comparison of the numbers in that line with the specified"
           << " range.  The result is reported to stdout and the compliment is returned.";
      _description.assign(Ostr.str());
    };
  };


};
#endif
