#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <UnixUtils.H>

int main(int argc,char *argv[])
{
  if(argc < 2)
    return 1;
  std::string numproc_s(argv[1]);
  std::istringstream Istr(numproc_s);
  std::ofstream RocinSurfFile;
  std::ofstream RocinVolFile;
  std::ifstream OrigSurfFile;
  std::ifstream OrigVolFile;
  OrigSurfFile.open("ifluid_in_00.000000.txt");
  OrigVolFile.open("fluid_in_00.000000.txt");
  std::string surfpanes;
  std::string surffiles;
  std::string volpanes;
  std::string volfiles;
  std::string junk;
  OrigSurfFile >> junk >> junk >> junk 
	       >> surffiles >> junk;
  std::getline(OrigSurfFile,surfpanes);
  OrigVolFile >> junk >> junk >> junk
	      >> volfiles >> junk;
  std::getline(OrigVolFile,volpanes);
  OrigSurfFile.close();
  OrigVolFile.close();
  rename("ifluid_in_00.000000.txt","ifluid_in_00.000000.txt.orig");
  rename("fluid_in_00.000000.txt","fluid_in_00.000000.txt.orig");
  RocinSurfFile.open("ifluid_in_00.000000.txt");
  RocinVolFile.open("fluid_in_00.000000.txt");
  int np = 0;
  Istr >> np;
  for(int i = 0; i < np; i++){
    RocinSurfFile << "@Proc: " << i << std::endl;
    RocinVolFile <<  "@Proc: " << i << std::endl;
    int fileid = i+1;
    std::ostringstream Ostr;
    std::ostringstream Ostr2;
    Ostr2 << i;
    int nchar = Ostr2.str().length();
    nchar = 4 - nchar;
    while(nchar--)
      Ostr << "0";
    Ostr << Ostr2.str();
    RocinSurfFile << "@Files: " << Ostr.str() << "/" << surffiles << std::endl
		  << "@Panes: " << surfpanes << std::endl << std::endl;
    RocinVolFile  << "@Files: " << Ostr.str() << "/" << volfiles << std::endl
		  << "@Panes: " << volpanes << std::endl << std::endl;
    IRAD::Sys::ChDir(Ostr.str());
    IRAD::Sys::ChDir(Ostr.str());
    Ostr2.clear();
    Ostr2.str("");
    Ostr2 << fileid;
    nchar = 5 - Ostr2.str().length();
    Ostr.clear();
    Ostr.str("");
    while(nchar--)
      Ostr << "0";
    Ostr << fileid;
    std::ostringstream FNout;
    std::ostringstream FNout2;
    FNout << "fluid_" << Ostr.str() << ".hdf";
    FNout2 << "../fluid_" << Ostr.str() << ".hdf";
    IRAD::Sys::SymLink(FNout2.str().c_str(),FNout.str().c_str());
    FNout.clear();
    FNout2.clear();
    FNout.str("");
    FNout2.str("");
    FNout << "ifluid_" << Ostr.str() << ".hdf";
    FNout2 << "../ifluid_" << Ostr.str() << ".hdf";
    if(IRAD::Sys::FILEEXISTS(FNout2.str()))
      IRAD::Sys::SymLink(FNout2.str().c_str(),FNout.str().c_str());
    IRAD::Sys::ChDir(std::string(".."));
  }
  RocinSurfFile.close();
  RocinVolFile.close();
  return 0;
}
