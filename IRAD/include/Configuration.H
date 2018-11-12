///
/// \file
/// \ingroup irad_group
/// \brief Configuration object interface (for config files, etc).
///
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <cstdlib>
#include "Parameters.H"

namespace IRAD {
  namespace Util {
    
    class ConfigParameters : public Util::Parameters
    {
      //    friend std::ostream &operator<<(std::ostream &Ostr,const ConfigParameters &params);
      //    friend std::istream &operator>>(std::istream &Istr,ConfigParameters &params);
    private:
      std::string _Name;
    public:
      ConfigParameters(): _Name("") {};
      ConfigParameters(const std::string &name): _Name(name) {}; 
      const std::string &Name() const { return(_Name); };
      void Name(const std::string &name) { _Name.assign(name); };
    };

    class ConfigurationObject : public Util::ConfigParameters
    {
      friend std::ostream &operator<<(std::ostream &Ostr,const Util::ConfigurationObject &cob);
      friend std::istream &operator>>(std::istream &Istr,Util::ConfigurationObject &cob);
    private:
      std::vector<Util::ConfigParameters> _parameters;
      std::vector<std::string>            _sections;
    public:
      std::vector<Util::ConfigParameters> &GetParameters() { return(_parameters); };
      Util::ConfigParameters &Section(const std::string &section_name);
      void AddSection(Util::ConfigParameters &params) { _parameters.push_back(params); _sections.push_back("");};
      unsigned int SectionIndex (const std::string &section_name) const;
      std::string &NavigationSection(const std::string &section_name);
      std::string NavigationSection(const std::string &section_name) const;
      std::vector<std::string> &NavigationSections() { return (_sections); };
      virtual std::string AdvanceToNextSection(std::istream &Inf);
      virtual std::string ReadSection(std::istream &Inf);
      virtual std::string ExtractSection(const std::string &section_name,std::istream &Inf);
    };
    std::ostream &operator<<(std::ostream &Ostr,const Util::ConfigurationObject &cob);
    std::istream &operator>>(std::istream &Istr,Util::ConfigurationObject &cob);
  };
};

#endif
