///
/// @file
/// @brief Test results utility header.
/// @ingroup irad_group
///

#ifndef __TEST_RESULTS_H__
#define __TEST_RESULTS_H__

namespace IRAD 
{
  ///
  /// Test results utility.
  ///
  /// @param argc integer indicating how many words are in the command line.
  /// @param argv string words of the command line.
  /// @returns 0 if test was successful, 1 if not.
  ///
  /// Usage:
  /// 
  /// > testresults \<Name of Test\> \<File test results\>
  ///
  /// This utility gets the results of a named test
  /// from a file.  The file is assumed to have the 
  /// results from the named tests in the following 
  /// format:
  /// 
  ///          NameOfTest1=1 # passed test
  ///          NameOfTest2=0 # failed test
  ///          NameOfTest3 = 1 # also passed
  ///
  /// If the test passed, this utility returns a 0, otherwise it returns 1.
  ///
  int TestResults(int argc,char *argv[]);
};
#endif
