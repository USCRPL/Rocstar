///
/// \file
/// \ingroup irad_group
/// \brief Text menu system for user interfaces
///
#ifndef __MENU_H__
#define __MENU_H__

#include "Configuration.H"
#include <iomanip>

namespace IRAD { 

  namespace Util {

    typedef std::string::size_type strsize;

    template<typename ConfigType = Util::ConfigurationObject,typename ParamType = Util::ConfigParameters> 
    class MenuObject
    {
    public:
      MenuObject() : _external_border("*"), _vertical_rule("|"), _horizontal_rule("-"), 
		     _field_separator(" = "),_menu_width(80),_column_length(12) {};
      std::string LastSection() const { return(_last_section); }; 
      std::string hRule()
      {
	std::ostringstream Ostr;
	strsize menuspace = _menu_width - _external_border.size()*2 - 2;
	int hsize = _horizontal_rule.size();
	if(hsize == 0) hsize=1;
	Ostr << _external_border;
	for(unsigned int npchar = 0;npchar < (menuspace+2);npchar+=hsize)
	  Ostr << std::setw(hsize) << _horizontal_rule;
	Ostr << _external_border;
	return(Ostr.str());
      };
      std::string centerString(const std::string &message)
      {
	std::ostringstream Ostr;
	strsize border_size = _external_border.size() + 1;
	strsize menuspace   = _menu_width-(border_size*2);
	int nspaces = (menuspace-message.size())/2;
	int spaces = 0;
	Ostr << _external_border << " ";
	while(spaces++ < nspaces)
	  Ostr << " ";
	Ostr << message;
	//      spaces = 0;
	//      while(spaces++ < nspaces)
	//	Ostr << " ";
	int space_left = _menu_width - (Ostr.str().size() + 2);
	while(space_left--)
	  Ostr << " ";
	Ostr << " " << _external_border;
	return(Ostr.str());
      };
      virtual std::string menuString(ConfigType &config,const std::string &section)
      {
	_last_section.assign(section);
	std::ostringstream Ostr;

	// Calculate some sizes for output formatting
	strsize border_size = _external_border.size() + 1;
	strsize vrule_size  = _vertical_rule.size() + 2;
	strsize menuspace   = _menu_width-(border_size*2);
	ParamType &params = config.Section(section);
	int npp = params.size();
	int ncol = 1;   
	while((_column_length*ncol) < npp)
	  ncol++;
	int lines_per_col = (npp + ncol)/ncol;
	std::vector<int> npercol(ncol,0);
	std::vector<int>::iterator npci = npercol.begin();
	while(npci != npercol.end()){
	  *npci = (npp < lines_per_col ? npp : lines_per_col);
	  npp -= *npci++;
	} 
	int rule_size   = vrule_size * (ncol - 1);
	int button_size = 4; // "xx) "
	int field_separator_size = _field_separator.size(); // " = "
	int column_size = (menuspace - rule_size)/ncol;
	int field_space = column_size - button_size - field_separator_size;
	unsigned int field_size  = field_space/2;
	int total_size  = rule_size + (ncol * (button_size+(2*field_size)+field_separator_size));
	int xtraspace   = (menuspace-total_size);
	int fieldspace  = xtraspace/(ncol*2);

	// If there is space left over, flow it into the field size and
	// track the overflow.  This is needed to make sure all the
	// borders line up when printing the menu. 
	if(fieldspace > 0){
	  field_size += fieldspace;
	  xtraspace -= (fieldspace*ncol*2);
	}

	// Print the header and the section name to the buffer
	// followed by a horizontal rule.
	Ostr << _header << std::endl
	     << centerString(section) << std::endl << hRule() << std::endl
	     << _external_border << " ";
	// Print the menu lines
	int line_count = 1;
	for(line_count = 1; line_count <= npercol[0];line_count++){
	  int extraspace = xtraspace;
	  for(int cc = 0;cc < ncol;cc++){
	    unsigned int param_index = line_count-1;
	    for(int ccount = 0;ccount < cc;ccount++)
	      param_index += npercol[ccount];
	    std::string key;
	    std::string value;
	    if(param_index < params.size()){
	      key   = params[param_index].Key();
	      value = params[param_index].Value();
	      if(key.size() > field_size){
		key = std::string("*") + key.substr(key.size() - field_size + 1);
	      }
	      if(value.size() > field_size){
		value = std::string("*")+ value.substr(value.size() - field_size + 1);
	      }
	    }
	    if(param_index < params.size())
	      Ostr << std::setw(2) << std::setiosflags(std::ios::left) <<  param_index+1 << std::setw(2) << ") ";
	    else
	      Ostr << std::setw(4) << "";
	    Ostr << std::setw(field_size) << std::setiosflags(std::ios::left) 
		 << (param_index < params.size() ? key : "") 
		 << std::setw(3) << (param_index < params.size() ? " = " : "") 
		 << std::setw(field_size) 
		 << (param_index < params.size() ? value : "") << ((extraspace--) > 0 ? " " : "")
		 << ((cc == (ncol-1)) ? "" : (std::string(std::string(" ") + _vertical_rule + std::string(" "))));	
	  }
	  while(extraspace-- > 0)
	    Ostr << " ";
	  Ostr << " " << _external_border << std::endl;
	  if(line_count < npercol[0])
	    Ostr << _external_border << " ";
	}
	// Print a horizontal rule between the menu lines and the navigation section
	Ostr << hRule();
	return(Ostr.str());
      };

      std::string navigationString(ConfigType &config,const std::string &section){
	std::ostringstream Ostr;
	int section_index   = config.SectionIndex(section);
	ParamType &params   = config.Section(section);
	strsize border_size = _external_border.size() + 1;
	strsize vrule_size  = _vertical_rule.size() + 2;
	strsize menuspace   = _menu_width-(border_size*2);
	int npp             = params.size();
	int ncol            = 1;   
	while((_column_length*ncol) < npp)
	  ncol++;
	int lines_per_col   = (npp + ncol)/ncol;
	std::vector<int> npercol(ncol,0);
	std::vector<int>::iterator npci = npercol.begin();
	while(npci != npercol.end()){
	  *npci = (npp < lines_per_col ? npp : lines_per_col);
	  npp -= *npci++;
	} 
	int rule_size   = vrule_size * (ncol - 1);
	int button_size = 4; // default: "xx) "
	int field_separator_size = _field_separator.size(); // default: " = "
	int column_size = (menuspace - rule_size)/ncol;
	int field_space = column_size - button_size - field_separator_size;
	unsigned int field_size  = field_space/2;
	int total_size  = rule_size + (ncol * (button_size+(2*field_size)+field_separator_size));
	int xtraspace   = (menuspace-total_size);
	int menuitem = params.size()+1;
	int navsize = button_size + field_size + 1;
	int nnavperline = menuspace/navsize;
	xtraspace = menuspace - navsize*nnavperline + 1;

	std::string topName(config.Name());
	if(topName.size() > menuspace){
	  topName = std::string("*") + topName.substr(topName.size() - menuspace + 1);
	}

	Ostr << centerString("Navigation") << std::endl << hRule() << std::endl;
	std::ostringstream OnavStr;
	OnavStr << _external_border << " "
		<< 0 << ")" << std::setiosflags(std::ios::left) 
		<< std::setw(topName.size()) << topName;
	std::string navSectionString(config.NavigationSections()[section_index]);
	std::istringstream Istr(navSectionString);
	std::string navString;
	bool first_on_line = false;
	xtraspace = _menu_width - (OnavStr.str().size() + _external_border.size()+1);
	while(Istr >> navString){
	  // Trim the string if it is larger than the max size
	  if(navString.size() > menuspace){
	    navString = std::string("*") + navString.substr(navString.size() - menuspace +1);
	  }
	  if((int)(button_size + navString.size()+3) > xtraspace){
	    while(xtraspace--)
	      OnavStr << " ";
	    OnavStr << " " << _external_border << std::endl;
	    Ostr << OnavStr.str();
	    OnavStr.str("");
	    OnavStr << _external_border << " ";
	    first_on_line = true;
	  }
	  OnavStr << (first_on_line ? "" : " | ") << menuitem++ << ")" << std::setw(std::ios::left) 
		  << std::setw(navString.size()) << navString;
	  xtraspace = _menu_width - (OnavStr.str().size()+_external_border.size()+1);
	}
	if(xtraspace > 0){
	  xtraspace = _menu_width - (OnavStr.str().size()+_external_border.size()+1);
	  while(xtraspace--)
	    OnavStr << " ";
	  OnavStr << " " << _external_border;
	}
	Ostr << OnavStr.str() << std::endl << hRule();
	return(Ostr.str());
      };
      void SetExternalBorder(const std::string &eb){
	_external_border.assign(eb);
      };
      std::string Border() const {
	return(_external_border);
      };
      void SetHeader(const std::string &hdr){
	_header.assign(hdr);
      };
      std::string Header() const {
	return(_header);
      };
      void SetHRule(const std::string &hrule){
	_horizontal_rule.assign(hrule);
      };
      void SetVRule(const std::string &vrule){
	_vertical_rule.assign(vrule);
      };
      void SetFieldSeparator(const std::string &sep){
	_field_separator.assign(sep);
      };
      void SetMenuWidth(unsigned int wid){
	_menu_width = wid;
      };
      void SetColumnLength(unsigned int colen){
	_column_length = colen;
      };
    private:
      std::string _header;
      std::string _external_border;
      std::string _vertical_rule;
      std::string _horizontal_rule;
      std::string _field_separator;
      strsize     _menu_width;
      int         _column_length;
      std::string _last_section;
      std::string _next_section;
      std::string _up_section;
    };
  };
};
#endif



