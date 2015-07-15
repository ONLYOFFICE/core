#pragma once

#include <iosfwd>
#include <string>
#include <boost/scoped_ptr.hpp>



namespace oox {

class xlsx_cell_format;

/// \class  class xlsx_style_manager
class xlsx_style_manager
{
public:
    xlsx_style_manager();
    size_t size() const;
    size_t xfId(  const xlsx_cell_format * xlxsCellFormat,
        const std::wstring &num_format,  bool  default_set,bool & is_visible);

    size_t xfId(const xlsx_cell_format * xlxsCellFormat,
        const std::wstring &num_format, bool  default_set);


    void xlsx_serialize(std::wostream & _Wostream);
    
    ~xlsx_style_manager();

private:
    class Impl;
	boost::scoped_ptr<Impl> impl_;
};

}

