///
/// @file
/// @ingroup irad_group
/// @brief Configuration object implementation (for config files, etc)
///
#include "Configuration.H"
namespace IRAD {
  namespace Util {

    ///
    /// Extract parameters for particular section.
    ///
    Util::ConfigParameters &Util::ConfigurationObject::Section(const std::string &section_name){
      if(section_name == Name())
	return(*this);
      std::vector<Util::ConfigParameters>::iterator pi = _parameters.begin();
      while(pi != _parameters.end()){
	if(pi->Name() == section_name)
	  return(_parameters[pi-_parameters.begin()]);
	pi++;
      }
      std::cerr << "Util::ConfigurationObject::GetSectionParameters::Fatal Error - Section "
		<< section_name << " does not exist. Exiting." << std::endl;
      Util::Error err = 1;
      throw err;
      exit(1);
      return(_parameters[0]);
    };

    // returns index = _parameters.size() if not found
    unsigned int Util::ConfigurationObject::SectionIndex(const std::string &section_name) const {
      std::vector<Util::ConfigParameters>::const_iterator pi = _parameters.begin();
      unsigned int index = 0;
      if(section_name != Name()){
	while(pi != _parameters.end()){
	  if(pi->Name() == section_name)
	    return (index+1);
	  pi++;
	  index++;
	} 
      }
      return(index);
    };

    std::string &Util::ConfigurationObject::NavigationSection(const std::string &section_name)
    {
      return(_sections[SectionIndex(section_name)]);
    };
    std::string Util::ConfigurationObject::NavigationSection(const std::string &section_name) const
    {
      return(_sections[SectionIndex(section_name)]);    
    };
    std::string Util::ConfigurationObject::ExtractSection(const std::string &section_name,std::istream &Inf)
    {
      while(Inf)
	{
	  std::string line;
	  std::getline(Inf,line);
	  if(!line.empty()){
	    if(line[0] == '#'){
	      std::vector<std::string> tokens;
	      Util::TokenizeString(tokens,line);
	      if(tokens[0] == "#Section"){
		if(tokens.size() > 1){
		  if(tokens[1] == section_name){
		    return (this->ReadSection(Inf));
		  }
		
		}
	      }
	    }
	  }
	}
      return(std::string(""));
    }

    std::string Util::ConfigurationObject::AdvanceToNextSection(std::istream &Inf)
    {
      std::string empty_retval;
      while(Inf)
	{
	  std::string line;
	  std::getline(Inf,line);
	  if(!line.empty()){
	    if(line[0] == '#'){
	      std::vector<std::string> tokens;
	      Util::TokenizeString(tokens,line);
	      if(tokens[0] == "#Section")
		if(tokens.size() > 1)
		  return(tokens[1]);
	    }
	  }
	}
      return(empty_retval);
    }

    std::string Util::ConfigurationObject::ReadSection(std::istream &Inf)
    {
      std::ostringstream Ostr;
      while(Inf)
	{
	  int open_sections = 1;
	  std::string line;
	  std::getline(Inf,line);
	  if(line[0] == '#'){
	    std::vector<std::string> tokens;
	    Util::TokenizeString(tokens,line);
	    if(tokens[0] == "#EndSection") {
	      open_sections = open_sections - 1;
	      if(open_sections == 0)
		return(Ostr.str());
	    }
	    else if(tokens[0] == "#Section") {
	      open_sections++;
	      Ostr << line;
	    }
	  }
	  Ostr << line << std::endl;
	}
      return(Ostr.str());
    }
    ///
    /// Stream output operator for Util::ConfigurationObject.
    ///
    std::ostream &operator<<(std::ostream &Ostr,const Util::ConfigurationObject &cob){
      // Output the configuration of the configuration object itself
      Ostr << "#" << std::endl
	   << "#Section Configuration" << std::endl
	   << "#" << std::endl
	   << static_cast<Util::Parameters>(cob) << std::endl
	   << "#Sections " << cob._sections[0] << std::endl
	   << "#EndSection" << std::endl;
      // Now output the parameters that configure the application to which 
      // this configuration object applies.
      std::vector<Util::ConfigParameters>::const_iterator pi = cob._parameters.begin();
      while(pi != cob._parameters.end()){
	Ostr << "#" << std::endl
	     << "#Section " << pi->Name() << std::endl
	     << "#" << std::endl
	     << static_cast<Util::Parameters>(*pi) << std::endl
	     << "#Sections " << cob._sections[pi-cob._parameters.begin()+1] << std::endl
	     << "#EndSection" << std::endl;
	pi++;
      }
      return(Ostr); 
    }
  
    /// 
    /// Stream input operator for Util::ConfigurationObject.
    ///
    std::istream &operator>>(std::istream &Istr,Util::ConfigurationObject &cob){
      // Read in the configuration for the configuration object itself
      std::string configsection = cob.ExtractSection("Configuration",Istr);
      cob.Name("Configuration");
      std::istringstream ConfigIstr(configsection);
      ConfigIstr >> static_cast<Util::Parameters &>(cob);
      // Now read in the one token name for the application which this 
      // object configures and stuff it into the _sections vector.
      ConfigIstr.clear();
      ConfigIstr.str(configsection);
      std::string line;
      cob._sections.resize(0);
      while(std::getline(ConfigIstr,line) && cob._sections.empty()){
	if(!line.empty()){
	  std::vector<std::string> tokens;
	  Util::TokenizeString(tokens,line);
	  std::vector<std::string>::iterator ti = tokens.begin();
	  if(*ti++ == "#Sections"){
	    std::ostringstream Ostr;
	    while(ti != tokens.end())
	      Ostr << *ti++ << " ";
	    cob._sections.push_back(Ostr.str());
	  }
	}
      }
      // Advance to the next section - if found, then read the 
      // section from the input stream and stuff it into the
      // _parameters vector.
      std::string sectionname = cob.AdvanceToNextSection(Istr);
      while(!sectionname.empty()){
	Util::ConfigParameters params(sectionname);
	std::string section = cob.ReadSection(Istr);
	std::istringstream SectionIstr(section);
	SectionIstr >> static_cast<Util::Parameters &>(params);
	cob._parameters.push_back(params);
	// Re-read the section and extract the sections to
	// which the currently read section is linked.  This
	// is how we track the relationships between sections
	// as an aid for automatically generating user interfaces
	// to the configuration object.
	SectionIstr.clear();
	SectionIstr.str(section);
	std::string line;
	bool done = false;
	while(std::getline(SectionIstr,line) && !done){
	  if(!line.empty()){
	    std::vector<std::string> tokens;
	    Util::TokenizeString(tokens,line);
	    std::vector<std::string>::iterator ti = tokens.begin();
	    if(*ti++ == "#Sections"){
	      std::ostringstream Ostr;
	      while(ti != tokens.end())
		Ostr << *ti++ << " ";
	      cob._sections.push_back(Ostr.str());
	      done = true;
	    }
	  }
	}
	sectionname = cob.AdvanceToNextSection(Istr);
      }
      return(Istr);
    }
  };
};
