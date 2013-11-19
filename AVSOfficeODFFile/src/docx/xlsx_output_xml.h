#ifndef _CPDOCCORE_XLSX_OUTPUT_XML_fe64931a
#define _CPDOCCORE_XLSX_OUTPUT_XML_fe64931a
#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPNoncopyable.h>

#include "docx_rels.h"

namespace cpdoccore {
namespace oox {

class xlsx_xml_worksheet;
typedef _CP_PTR(xlsx_xml_worksheet) xlsx_xml_worksheet_ptr;

/// \class  xlsx_xml_worksheet
class xlsx_xml_worksheet: noncopyable
{
public:
    xlsx_xml_worksheet(std::wstring const & name);
    ~xlsx_xml_worksheet();
public:
    std::wstring name() const;
   
	std::wostream & cols();
    std::wostream & sheetFormat();
    std::wostream & sheetData();
    std::wostream & hyperlinks();
    std::wostream & mergeCells();
    std::wostream & drawing(); 
    std::wostream & comments(); 
	
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
    _CP_SCOPED_PTR(Impl) impl_;
};

class xlsx_xml_workbook;
typedef _CP_PTR(xlsx_xml_workbook) xlsx_xml_workbook_ptr;

/// \class  xlsx_xml_workbook
class xlsx_xml_workbook : noncopyable
{
public:
    xlsx_xml_workbook();
    ~xlsx_xml_workbook();
public:
    std::wostream & sheets();

public:
    void write_to(std::wostream & strm);

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}

#endif

