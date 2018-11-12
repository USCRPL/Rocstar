///
/// @file
/// @ingroup irad_group
/// @brief Testing constructs for unit testing
///
#ifndef __TESTING_H__
#define __TESTING_H___

#include "Parameters.H"

namespace IRAD {

  namespace Util {

    ///
    /// Encapsulating class for collections of test results.
    ///
    class TestResults : public Util::Parameters
    {
      //    friend std::ostream &operator<<(std::ostream &oSt,
      //				    const IRTest::Results &results);
      //    friend std::istream &operator>>(std::istream &iSt,
      //				    IRTest::Results &results);
    public:
    
      ///
      /// Add a named result to the test results
      ///
      /// @param name Constant std::string reference to the test name.
      /// @param result Constant reference to a <ValueType> value.
      ///
      /// This method puts a named test result into the 
      /// collection of results. The "value" is converted to 
      /// a string. In general, a 0 is a false or negative result
      /// while a 1 is true or a positive result.
      ///
      template <typename ValueType>
      void AddResult(const std::string &name,const ValueType &result)
      {
	Util::ParamType p;
	p.Key(name);
	std::ostringstream Ostr;
	Ostr << result;
	p.Value(Ostr.str());
	this->push_back(p);
      }
    
      ///
      /// Updates an existing test result.
      /// 
      /// @param name Constant reference to a std::string containing the test name.
      /// @param result Constant reference to the <ValueType> data containing the test result.
      ///
      /// This method is very much like AddResult except this one only
      /// replaces the value of an existing test. If the named test is not
      /// found in the results already, it is automatically added.
      ///
      template<typename ValueType>
      void UpdateResult(const std::string &name,const ValueType &result)
      {
	std::ostringstream Ostr;
	Ostr << result;
	Util::TestResults::iterator _thisit = this->begin();
	while(_thisit != this->end()){
	  if(_thisit->first == name){
	    _thisit->second = Ostr.str();
	    return;
	  }
	  _thisit++;
	}
	return(AddResult<ValueType>(name,result));
      } 
    };
  
    ///
    /// Interface for a general testing object.
    ///
    /// This object is meant to be the base class for 
    /// testing objects that can easily be integrated into
    /// composite software packages with integrated testing
    /// facilities.
    /// 
    /// The general intention is that developers would 
    /// implement all tests as member methods of classes
    /// inheriting from this class. 
    /// 
    template<typename ResultsType = Util::TestResults>
    class TestingObject
    {
    public:
      /// Set up the tests and any test fixture constructs
      virtual void Prologue(){};
      /// Process all tests and populate a "results" object
      virtual void Process(ResultsType &results){};
      /// Process named tests from a list and populate a "results" object
      virtual void ProcessTests(std::list<std::string> &names,ResultsType &results){};
      /// Run a particular test
      virtual void RunTest(const std::string &name,ResultsType &results){};
      /// Clean up any test fixtures that need cleaning.
      virtual void Epilogue(){};
      virtual ~TestingObject(){};
    };
  };
};
#endif
