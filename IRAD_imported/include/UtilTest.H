///
/// @file
/// @ingroup irad_group
/// @brief Testing object for primitive utilities
///
#include "Testing.H"
//#include "primitive_utilities.H"

namespace IRAD {
  namespace Util {

    //  typedef IRTest::Dummy TPTestType;

    // The basic overaching test object for the primitive
    // utilities
    class TestObject : public Util::TestingObject<Util::TestResults>
    {
    private:
      std::string TestString1;
    public:
      void  Prologue(){
	std::ostringstream Ostr;
	Ostr << "#"       << std::endl
	     << " " << std::endl
	     << "\t" << std::endl
	     << "Test       # Comment" << std::endl
	     << "#         " << std::endl;
	TestString1.assign(Ostr.str());
      }
      // An actual test of the function called 
      // GetNextContent.   The name Test__XXXXX will
      // eventually help automated utilities with 
      // running tests by name.
      void Test__GetNextContent(Util::TestResults &result) {
	std::istringstream Istr(TestString1);
	std::string next_content(Util::GetNextContent(Istr));
	std::string line;
	std::getline(Istr,line);
	result.UpdateResult("GetNextContent:CommentsAndWhiteSpace",
			    next_content == "Test       ");
	result.UpdateResult("GetNextContent:StreamObject",
			    line == "#         ");  
      }

      // This runs all the tests
      void Process(Util::TestResults &result){
	Prologue();
	Test__GetNextContent(result);
	Epilogue();
      }

      // This runs the tests named in a list
      void ProcessTests(std::list<std::string> &test_names,Util::TestResults &result){
	Prologue();
	std::list<std::string>::iterator tni = test_names.begin();
	while(tni != test_names.end())
	  {
	    std::string testname(*tni++);
	    if(testname == "GetNextContent")
	      Test__GetNextContent(result);
	  }
	Epilogue();
      }
    };
  };
};
