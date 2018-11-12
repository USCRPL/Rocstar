///
/// @file 
/// @ingroup irad_group
/// @brief Implements a command-line interface for runnning tests.
///
#include <sstream>
#include <fstream>
#include <iomanip>
#include <math.h>

#include "UnixUtils.H"
#include "ComLine.H"
#include "FDUtils.H"
#include "DiffDataFiles.H"

namespace IRAD
{
  bool isNumber(std::string sCheck){
    if(sCheck == "")
      return false;
    for(int i=0; i < sCheck.size();i++){
      if(!isdigit(sCheck[i]) && sCheck[i] != 'e' && sCheck[i] != 'E'
         && sCheck[i] != '.' && sCheck[i] != '+' && sCheck[i] != '-'){
        return false;
      }
    }
    return true;
  }

  int DiffDataFiles(int argc,char *argv[])
  {
    
    DDFComLine comline((const char **)(argv));
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
    std::ofstream Ouf;
    std::ostream *Out = &std::cout;

    // The next few lines populate some strings based on the 
    // users input from the commandline.
    std::string OutFileName(comline.GetOption("output"));
    std::vector<std::string> FileNames = comline.GetArgs();
    std::string sverb(comline.GetOption("verblevel"));
    std::string sTolerance(comline.GetOption("tolerance"));
    std::string sNoBlank(comline.GetOption("blank"));

    int retval = 0;

    // The following block parses and sets the verbosity level
    int verblevel = 1;
    if(!sverb.empty()){
      if(sverb != ".true."){
        std::istringstream Istr(sverb);
        Istr >> verblevel;
        if(verblevel < 0)
          verblevel = 1;
      }
    }

    // If the user only wants to compare numbers in the file set the bool for that
    bool numbers = false;
    if(!comline.GetOption("numbers").empty()){
      numbers = true;
    }

    // If there is a tolerance set the bool and convert the tolerance to a double
    bool useTol = false;
    double tolerance = 1.0e-12;
    if(!sTolerance.empty()){
      useTol = true;
      if(sTolerance != ".true."){
        std::istringstream Istr(sTolerance);
        Istr >> tolerance;
      }
    }
    //The tolerance flag must be used if the numbers flag is used
    else if(numbers){
      useTol = true;
    }
 
    // If the flag to ignore blank space is used set the bool
    bool noBlank = false;
    if(!sNoBlank.empty())
      noBlank = true;

    if(verblevel > 2){
      std::cout << "IRAD::DiffDataFiles> Starting up with the following environment:" << std::endl
                << "IRAD::DiffDataFiles> FileName1 = " << FileNames[0] << std::endl
                << "IRAD::DiffDataFiles> FileName2 = " << FileNames[1] << std::endl
                << "IRAD::DiffDataFiles> Verbosity = " << verblevel << std::endl
                << "IRAD::DiffDataFiles> OutFileName = " << OutFileName << std::endl
                << "IRAD::DiffDataFiles> IgnoreBlank = " << noBlank << std::endl
                << "IRAD::DiffDataFiles> NumbersOnly = " << numbers << std::endl
                << "IRAD::DiffDataFiles> Tolerance = "; 
      if(useTol)
        std::cout << tolerance;
      std::cout << std::endl; 
    }    

    // This block sets up the output file if the user specified one
    bool keep_outfile = false;
    if(!OutFileName.empty()){
      keep_outfile = true;
      Ouf.open(OutFileName.c_str());
      if(!Ouf){
        std::cout << "IRAD::DiffDataFiles> Error: Could not open output file, " 
                  <<  OutFileName << " for output. Exiting (fail)." << std::endl;
        return(1);
      }
      Out = &Ouf;
    }

    // Open the two files for reading
    std::ifstream InFile1;
    std::ifstream InFile2;
   
    InFile1.open(FileNames[0].c_str());
    if(!InFile1){
      std::cout << "IRAD::DiffDataFiles> Error: Could not open input file, " 
                <<  FileNames[0] << " for reading. Exiting (fail)." << std::endl;
      return(1);
    }
    InFile2.open(FileNames[1].c_str());
    if(!InFile2){
      std::cout << "IRAD::DiffDataFiles> Error: Could not open input file, " 
                <<  FileNames[1] << " for reading. Exiting (fail)." << std::endl;
      return(1);
    }
 
    // Read in the two files and compare
    std::string line1,line2, string1, string2, printString1="", printString2="";
    std::stringstream ss1, ss2, ssPrint1, ssPrint2;
    int lineNo = 0;
    bool lineDiff = false, numDiff = false;
    *Out << "File     line:              content" << std::endl;
    while(!InFile1.eof() || !InFile2.eof()){
      lineNo++;
      if(!InFile1.eof())
        std::getline(InFile1,line1);
      if(!InFile2.eof())
        std::getline(InFile2,line2);
      //compare line by line as strings(including white space)
      if(!noBlank && !useTol){
        if(line1 != line2){
            ssPrint1 << line1;
            ssPrint2 << line2;
            lineDiff = true;
            retval = 1;
        }
      }
      //compare each individual word in the file 
      else{
        ss1 << line1;
        ss2 << line2;
        while(ss1 >> string1){
          ss2 >> string2;
          if(string1 != string2){
            if(useTol){
              if(!isNumber(string1) || !isNumber(string2)){
                if(!numbers)
                  numDiff = true;
              }
              else{
                std::stringstream convert;
                double val1, val2;
                convert << string1;
                convert >> val1;
                convert.clear();
                convert.str("");
                convert << string2;
                convert >> val2;
                if(fabs(val1 - val2) > tolerance)
                  numDiff = true;
                else{
                  ssPrint1 << std::scientific << std::setw(20) << val1 << " ";
                  ssPrint2 << std::scientific << std::setw(20) << val2 << " ";
                }
              }
            }
            if(!useTol || numDiff){
              lineDiff = true;
              retval = 1;
              printString1 += '[' + string1 + "]";
              printString2 += '[' + string2 + "]";
              ssPrint1 << std::setw(20) << printString1 << " ";
              ssPrint2 << std::setw(20) << printString2 << " ";
            }
          }
          else{
            ssPrint1 << std::setw(20) << string1 << " ";
            ssPrint2 << std::setw(20) << string2 << " ";
          }
          string1 = string2 = "";
          printString1 = printString2 = "";
          numDiff = false;
        }
        while(ss2 >> string2){
          if(isNumber(string2) || !numbers){
            lineDiff = true;
            printString1 += '[' + string1 + "]";
            printString2 += '[' + string2 + "]";
            ssPrint1 << std::setw(20) << printString1 << " ";
            ssPrint2 << std::setw(20) << printString2 << " ";
            retval = 1;
          }
        }  
        printString1 = printString2 = string2 = "";
      }
      if(lineDiff){
        *Out << std::setw(4) << std::right << "1" << " " << std::setw(8) 
             << lineNo << ": " << ssPrint1.str() << std::endl
             << std::setw(4) << std::right << "2" << " " << std::setw(8) 
             << lineNo << ": " << ssPrint2.str() << std::endl;
      }
      ss1.clear();
      ss2.clear();
      ss1.str("");
      ss2.str("");
      ssPrint1.clear();
      ssPrint2.clear();
      ssPrint1.str("");
      ssPrint2.str("");
      lineDiff = false;
    }
    if(InFile1.eof() != InFile2.eof()){
      retval = 1;
    }

    if(keep_outfile)
      Ouf.close();

    return(retval);
  }
};


int main(int argc,char *argv[])
{
  return(IRAD::DiffDataFiles(argc,argv));
}
