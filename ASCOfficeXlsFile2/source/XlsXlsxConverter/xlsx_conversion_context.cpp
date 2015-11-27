
#include "xlsx_conversion_context.h"

#include <iostream>

#include "simple_xml_writer.h"

#include "xlsx_package.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace oox {
    

xlsx_conversion_context::xlsx_conversion_context( package::xlsx_document * outputDocument): output_document_(outputDocument),
	xlsx_table_context_(*this), 
	xlsx_drawing_context_handle_(get_mediaitems())
{
}

xlsx_conversion_context::~xlsx_conversion_context()
{
}

xlsx_drawing_context & xlsx_conversion_context::get_drawing_context()
{
    return get_table_context().get_drawing_context();
}

void xlsx_conversion_context::start_document()
{
}

xlsx_xml_worksheet & xlsx_conversion_context::current_sheet()
{
    if (!sheets_.empty())
    {
        return *sheets_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
oox_chart_context & xlsx_conversion_context::current_chart()
{
    if (!charts_.empty())
    {
        return *charts_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
bool xlsx_conversion_context::start_table(const std::wstring & name)
{
    sheets_.push_back(xlsx_xml_worksheet::create(name));
    get_table_context().start_table(name);

	return true;
}

void xlsx_conversion_context::start_chart()
{
	charts_.push_back(oox_chart_context::create());
	//добавл€ем новую форму дл€ диаграммы
	 //в ней будет информационна€ часть - и она пишетс€ каждый раз в свою xml (их - по числу диаграмм)
	//этот контекст нужно передавать в файл

}

void xlsx_conversion_context::end_chart()
{
	//current_chart().set_drawing_link(current_sheet().get_drawing_link());
	//излишн€€ инфа
}

void xlsx_conversion_context::end_table()
{
    get_table_context().serialize_hyperlinks(current_sheet().hyperlinks());
	get_table_context().dump_rels_hyperlinks(current_sheet().hyperlinks_rels());

    get_table_context().end_table();
}

xlsx_drawing_context_handle & xlsx_conversion_context::get_drawing_context_handle()
{
    return xlsx_drawing_context_handle_;
}

void xlsx_conversion_context::end_document()
{
	std::wstringstream workbook_content;

    unsigned int count = 0;
    // добавл€ем таблицы
    BOOST_FOREACH(const xlsx_xml_worksheet_ptr& sheet, sheets_)
    {
        count++;
		const std::wstring id = std::wstring(L"sId") + boost::lexical_cast<std::wstring>(count);

        package::sheet_content_ptr content = package::sheet_content::create();
 ////////////////////////////////////////////////////////////////////////////////////////////       
		const std::pair<std::wstring, std::wstring> p1 = sheet->get_drawing_link();
        
		if (!p1.first.empty())
        {
            const std::wstring dId = p1.second;
            static const std::wstring kType = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing"; 
            const std::wstring dName = std::wstring(L"../drawings/" + p1.first);
            content->add_rel(relationship(dId, kType, dName));
        }
//////////////////////////////////////////////////////////////////////////////////////////////////
        content->add_rels(sheet->hyperlinks_rels());
/////////////////////////////////////////////////////////////////////////////////////////////////
		const std::pair<std::wstring, std::wstring> p2 = sheet->get_comments_link();        
		if (!p2.first.empty())
        {
            const std::wstring dId = p2.second;
            static const std::wstring kType = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments"; 
            const std::wstring dName = std::wstring(L"../" + p2.first);
            content->add_rel(relationship(dId, kType, dName));
        }

		const std::pair<std::wstring, std::wstring> p3 = sheet->get_vml_drawing_link();		
		if (!p3.first.empty())
        {
            const std::wstring dId = p3.second;
            static const std::wstring kType = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing"; 
            const std::wstring dName = std::wstring(L"../drawings/" + p3.first);
			content->add_rel(relationship(dId, kType, dName));
       }
/////////////////////////////////////////////////////////////////////////////////////////////////
		sheet->write_to(content->content());
        output_document_->get_xl_files().add_sheet(content);

	/////////////////////////////////////////////
        CP_XML_WRITER(workbook_content)
        {
            CP_XML_NODE(L"sheet")
            {
                CP_XML_ATTR(L"name", sheet->name());
                CP_XML_ATTR(L"sheetId", count);
                CP_XML_ATTR(L"state", L"visible");
                CP_XML_ATTR(L"r:id", id);            
            }
        }

    }
	//добавл€ем диаграммы

	count = 0;
    BOOST_FOREACH(const oox_chart_context_ptr& chart, charts_)
    {
		count++;
		package::chart_content_ptr content = package::chart_content::create();

		chart->dump_rels(content->rels());
		chart->write_to(content->content());

		output_document_->get_xl_files().add_charts(content);
	
	}
    //workbook_content << L"<calcPr iterateCount=\"100\" refMode=\"A1\" iterate=\"false\" iterateDelta=\"0.0001\" />";

	{
        output_document_->get_xl_files().set_sharedStrings( package::simple_element::create(L"sharedStrings.xml", xlsx_shared_strings_.str()) );
    }


    {
        std::wstringstream strm_workbook;

        CP_XML_WRITER(strm_workbook)
        {
            CP_XML_NODE(L"workbook")
            {
                CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
                CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");

                CP_XML_NODE(L"sheets")
                {
                    CP_XML_STREAM() << workbook_content.str();
                }

                CP_XML_NODE(L"definedNames")
                {
					CP_XML_STREAM() << xlsx_defined_names_.str();
				}
            }
        }

        output_document_->get_xl_files().set_workbook( package::simple_element::create(L"workbook.xml", strm_workbook.str()) );

		output_document_->content_type().set_media(get_mediaitems());
        output_document_->get_xl_files().set_media(get_mediaitems());

        package::xl_drawings_ptr drawings = package::xl_drawings::create(xlsx_drawing_context_handle_.content());
        output_document_->get_xl_files().set_drawings(drawings);
	
        //package::xl_comments_ptr comments = package::xl_comments::create(xlsx_comments_context_handle_.content());
        //output_document_->get_xl_files().set_comments(comments);
        
	}

}

}