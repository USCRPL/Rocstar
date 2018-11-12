///
/// @file 
/// @ingroup irad_group
/// @brief Implements a command-line interface for runnning tests.
///
#include <sstream>
#include <fstream>

#include "UnixUtils.H"
#include "ComLine.H"
#include "FDUtils.H"

#include "RunTest.H"

namespace IRAD
{
  int RunTest(int argc,char *argv[])
  {
    
    RTSComLine comline((const char **)(argv));
    // The call to comline.Initialize() reads the command line arguments
    // from the array passed in the previous line.
    comline.Initialize();
    // The ProcessOptions() call does detailed examination of the command
    // line arguments to check for user errors or other problems. This call
    // will return non-zero if there were errors on the commandline.
    int clerr = comline.ProcessOptions();
    // Check if the user just wanted to get the help and exit
    if(!comline.GetOption("help").empty()){
      // Print out the "long usage" (i.e. help) message to stdout
      std::cout << comline.LongUsage() << std::endl;
      return(0);
    }
    if(clerr){
      std::cout << comline.ErrorReport() << std::endl
                << std::endl << comline.ShortUsage() << std::endl;
      return(1);
    }
    // These outstreams allow the output to file to be set up and separated
    // from the stdout.
    //    std::ofstream Ouf;
    //    std::ostream *Out = &std::cout;

    // The next few lines populate some strings based on the 
    // users input from the commandline.
    std::string OutFileName(comline.GetOption("output"));
    //    std::string TestName(comline.GetOption("name"));
    std::string ListName(comline.GetOption("list"));
    std::string FileName(comline.GetOption("file"));
    std::string sverb(comline.GetOption("verblevel"));
    std::string HostName(comline.GetOption("hostname"));
    std::string PlatformsName(comline.GetOption("platforms"));
    std::string SourceDirectory(comline.GetOption("srcdir"));
    std::string BinaryDirectory(comline.GetOption("bindir"));
    std::string ScriptArgument(comline.GetOption("argument"));

    if(PlatformsName.empty() && ListName.empty() && FileName.empty()){
      std::cout  << comline.ShortUsage() << std::endl
                 << "IRAD::RunTest> Error: One of --list or --platform or --file"
                 << " must be specified. Exiting (fail)." << std::endl;
      return(1);
    }
    if((!PlatformsName.empty() && !ListName.empty())||
       (!FileName.empty() && (!PlatformsName.empty() || !ListName.empty()))){
      std::cout << comline.ShortUsage() << std::endl;
      std::cout << "IRAD::RunTest> Error: Ambigous settings. --list, --platform, and"
                << " --file are mutually exclusive. Exiting (fail)." << std::endl;
      return(1);
    }
    if(!HostName.empty() && PlatformsName.empty()){
      std::cout << "IRAD::RunTest> Warning: Hostname option is meaningless without platforms file."
                << std::endl;
    }
    // The following block parses and sets the verbosity level
    int verblevel = 1;
    if(!sverb.empty()){
      verblevel = 1;
      if(sverb != ".true."){
        std::istringstream Istr(sverb);
        Istr >> verblevel;
        if(verblevel < 0)
          verblevel = 1;
      }
    }
    if(verblevel > 2)
      std::cout << "IRAD::RunTest> Starting up with the following environment:" << std::endl
                << "IRAD::RunTest> OutFileName = " << OutFileName << std::endl
                << "IRAD::RunTest> ListName = " << ListName << std::endl
                << "IRAD::RunTest> FileName = " << FileName << std::endl
                << "IRAD::RunTest> HostName = " << HostName << std::endl
                << "IRAD::RunTest> PlatformsName = " << PlatformsName << std::endl
                << "IRAD::RunTest> SourceDirectory = " << SourceDirectory << std::endl
                << "IRAD::RunTest> BinaryDirectory = " << BinaryDirectory << std::endl;
    
    // This block sets up the output file if the user specified one
    bool keep_outfile = false;
    if(!OutFileName.empty()){
      keep_outfile = true;
      //      Ouf.open(OutFileName.c_str());
      //      if(!Ouf){
      //        std::cout << "IRAD::RunTests> Error: Could not open output file, " 
      //                  <<  OutFileName << " for test output. Exiting (fail)." << std::endl;
      //        return(1);
      //      }
      //      Out = &Ouf;
    }
    else{
      std::string name_stub("test_script.o");
      IRAD::Sys::TempFileName(name_stub);
      OutFileName = name_stub;
    }

    if(HostName.empty())
      HostName = IRAD::Sys::Hostname();
    //    if(PlatformsName.empty())
    //      PlatformsName = "./share/platforms/parallel_platforms";

    std::string BasePath;
    if(!PlatformsName.empty()){
      // Open up the platforms file and find the host
      std::ifstream PlatInf;
      std::string::size_type x = PlatformsName.find_last_of("/");
      if(x != std::string::npos)
        BasePath.assign(PlatformsName.substr(0,x+1));
      PlatInf.open(PlatformsName.c_str());
      if(!PlatInf){
        std::cout << "IRAD::RunTest> Error: Could not open platforms file, "
                  << PlatformsName << " for resolving platform-specific tests for "
                  << HostName << ". Exiting (fail)." << std::endl;
        return(1);
      }
      //      std::string PlatformFileName;
      std::string splatform;
      while(std::getline(PlatInf,splatform) && ListName.empty()){
        std::istringstream Istr(splatform);
        std::string platform_name;
        std::string running_name;
        Istr >> platform_name >> running_name;
        if(HostName == platform_name)
          ListName.assign(running_name);
      }
      if(ListName.empty()){
        std::cout << "IRAD::RunTest> Error: Could not find a platform file "
                  << "for " << HostName << ". Exiting (fail)." << std::endl;
        return(1);
      }
    }

    // Now we either have a list, or a single named test.
    std::vector<std::string> TestFileNames;
    if(!ListName.empty()){
      // Support both relative and absolute path
      std::ifstream ListInf;
      ListInf.open(ListName.c_str());
      if(!ListInf){
        if(!BasePath.empty()){
          std::string RelName(BasePath+ListName);
          ListInf.open(RelName.c_str());
        }
        if(!ListInf){
          std::cout << "IRAD::RunTest> Error: Could not open list file, "
                    << ListName << ". Exiting (fail)." << std::endl;
          return(1);
        }
      }
      if(BasePath.empty()){
        std::string::size_type x = ListName.find_last_of("/");
        if(x != std::string::npos)
          BasePath.assign(ListName.substr(0,x+1)); 
      }
      std::string raw_test_name;
      while(std::getline(ListInf,raw_test_name)){
        if(!IRAD::Sys::FILEEXISTS(raw_test_name)){
          std::string RelName(BasePath+raw_test_name);
          if(!IRAD::Sys::FILEEXISTS(RelName)){
            std::cout << "IRAD::RunTest> Warning: Could not find test " 
                      << raw_test_name << " from " << ListName << "." 
                      << std::endl;
          }
          else{
            TestFileNames.push_back(RelName);
          }
        }
        else{
          TestFileNames.push_back(raw_test_name);
        }
      }
      ListInf.close();
    }
    else { // An explicit filename must have been given
      if(!IRAD::Sys::FILEEXISTS(FileName)){
        std::cout << "IRAD::RunTest> Error: Could not find specified test, "
                  << FileName << ". Exiting (fail)." << std::endl;
        return(1);
      }
      TestFileNames.push_back(FileName);
    }
    
    // Now we have a list of full paths to tests to run
    int err = 0;
    std::vector<std::string>::iterator ti = TestFileNames.begin();
    while(ti != TestFileNames.end()){
      // The following block simply makes a link to the testing
      // script in the current directory for running.
      std::string TestPath(*ti++);
      std::string TestName(TestPath);
      std::string::size_type x = TestName.find_last_of("/");
      if(x != std::string::npos)
        TestName = TestName.substr(x+1);
      std::string LocalTest("./"+TestName);
      if(verblevel > 1)
        std::cout << "IRAD::RunTest> Linking " << TestPath << " to " << LocalTest << std::endl;
      if(IRAD::Sys::SymLink(TestPath,LocalTest)){
        err++;
        std::cout << "IRAD::RunTest> Warning: Could not create symbolic link "
                  << "to " << TestPath << " at " << LocalTest << "." 
                  << std::endl;
      }
      if(verblevel > 1)
        std::cout << "IRAD::RunTest> Running " << TestName << "." << std::endl;
      std::string TmpComOut(IRAD::Sys::TempFileName(OutFileName));
      std::string Command(LocalTest+" "+TmpComOut);
      if(!SourceDirectory.empty())
        Command = Command + " " + SourceDirectory;
      if(!BinaryDirectory.empty())
        Command = Command + " " + BinaryDirectory;
      if(!ScriptArgument.empty())
        Command = Command + " " + ScriptArgument;
      if(verblevel > 1)
        std::cout << "IRAD::RunTest> Running command: " << Command << std::endl;
      IRAD::Sys::InProcess TestProcess(Command);
      std::string testoutline;
      while(std::getline(TestProcess,testoutline)){
        if(verblevel)
          std::cout << testoutline << std::endl;
      }
      IRAD::Sys::Remove(LocalTest);
      std::ifstream ComInf(TmpComOut.c_str());
      if(!ComInf){
        std::cout << "IRAD::RunTest> Warning: Cannot access test results from "
                  << TestName << ". Continuing." << std::endl;
        err++;
      }
      else {
        std::ofstream Ouf;
        Ouf.open(OutFileName.c_str(),std::ios::app);
        if(!Ouf){
          std::cout << "IRAD::RunTest> Error: Cannot open outfile, " << OutFileName
                    << ". Exiting (fail)." << std::endl;
          return(1);
        }
        std::string comline;
        while(std::getline(ComInf,comline))
          Ouf << comline << std::endl;
        ComInf.close();
        Ouf.close();
        IRAD::Sys::Remove(TmpComOut);
      }
    }
    if(err && (verblevel > 1))
      std::cout << "IRAD::RunTest> Warning: " << err << " errors occurred during testing."
                << std::endl;
    if(!keep_outfile){
      std::ifstream Inf;
      Inf.open(OutFileName.c_str());
      if(!Inf){
        std::cout << "IRAD::RunTest> Error: Could not open test output"
                  << ". Test must have gone wrong.  Exiting (fail)."
                  << std::endl;
        return(1);
      }
      std::string line;
      while(std::getline(Inf,line))
        std::cout << line << std::endl;
      Inf.close();
      IRAD::Sys::Remove(OutFileName);
    }
    return(err);
  }
};


int main(int argc,char *argv[])
{
  return(IRAD::RunTest(argc,argv));
}
