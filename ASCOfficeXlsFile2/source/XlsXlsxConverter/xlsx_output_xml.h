#pragma once

#include <iosfwd>

#include "common.h"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "oox_rels.h"

namespace oox {

class xlsx_xml_worksheet;
typedef boost::shared_ptr<xlsx_xml_worksheet> xlsx_xml_worksheet_ptr;

/// \class  xlsx_xml_worksheet
class xlsx_xml_worksheet: boost::noncopyable
{
public:
    xlsx_xml_worksheet(std::wstring const & name);
    ~xlsx_xml_worksheet();
public:
    std::wstring name() const;
   
    std::wostream & dimension();
	std::wostream & sheetViews();
	std::wostream & cols();
    std::wostream & sheetFormat();
    std::wostream & sheetData();
    std::wostream & hyperlinks();
    std::wostream & mergeCells();
    std::wostream & drawing(); 
    std::wostream & comments(); 
	std::wostream & pageProperties();
	
	rels & hyperlinks_rels();

    void write_to(std::wostream & strm);

    void set_drawing_link(std::wstring const & fileName, std::wstring const & id);
    void set_vml_drawing_link(std::wstring const & fileName, std::wstring const & id);
    void set_comments_link(std::wstring const & fileName, std::wstring const & id);
	
	std::pair<std::wstring, std::wstring> get_drawing_link() const;
	std::pair<std::wstring, std::wstring> get_vml_drawing_link() const;
	std::pair<std::wstring, std::wstring> get_comments_link() const;

    static xlsx_xml_worksheet_ptr create(std::wstring const & name);

private:
    class Impl;
    boost::scoped_ptr<Impl> impl_;
};


}

