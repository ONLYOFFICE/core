
#include "xlsx_conversion_context.h"

#include <iostream>

#include "simple_xml_writer.h"

//#include "measuredigits.h"
#include "xlsx_package.h"
//#include "xlsx_utils.h"
//#include "xlsx_cell_format.h"
//#include "../odf/calcs_styles.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace oox {
    

xlsx_conversion_context::xlsx_conversion_context( package::xlsx_document * outputDocument): output_document_(outputDocument),
	xlsx_table_context_(*this, xlsx_text_context_)
	//
	//maxDigitSize_ (std::pair<float,float>(-1.0, -1.0) ),
	//default_style_( (std::numeric_limits<size_t>::max)() ),
	//xlsx_drawing_context_handle_(mediaitems_)
{
}

xlsx_conversion_context::~xlsx_conversion_context()
{
}

std::wostream & xlsx_conversion_context::current_stream()
{
	if (!sheets_.empty())
		return current_sheet().sheetData();
	else
		return other_stream;
}

void xlsx_conversion_context::start_document()
{
 //   std::vector<const odf_reader::style_instance *> instances;
 //   
	//instances.push_back(odfContext.styleContainer().style_default_by_type(odf_types::style_family::TableCell));
	//instances.push_back(odfContext.styleContainer().style_by_name(L"Default",odf_types::style_family::TableCell,false));

 //   odf_reader::text_format_properties_content		textFormatProperties	= calc_text_properties_content(instances);
 //   odf_reader::paragraph_format_properties			parFormatProperties		= calc_paragraph_properties_content(instances);
 //   odf_reader::style_table_cell_properties_attlist	cellFormatProperties	= calc_table_cell_properties(instances);

 //   oox::xlsx_cell_format cellFormat;
 //  
	//cellFormat.set_cell_type(XlsxCellType::s);
 //   cellFormat.set_num_format(oox::odf_string_to_build_in(0));

 //   default_style_ = get_style_manager().xfId(&textFormatProperties, &parFormatProperties, &cellFormatProperties, &cellFormat, L"",true);

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
bool xlsx_conversion_context::start_table(const std::wstring & name)
{
    //if (get_table_context().depth() > 0)
    //    return false;

    sheets_.push_back(xlsx_xml_worksheet::create(name));
    get_table_context().start_table(name);

	//current_sheet().cols() << L"<cols>";
    return true;
}

void xlsx_conversion_context::end_table()
{
    //const double lastWidht = table_column_last_width();
    //if (lastWidht > 0.0)
    //{
    //    unsigned int cMin = get_table_context().columns_count() + 1;
    //    unsigned int cMax = (std::max)((unsigned int)1024, get_table_context().columns_count() + 100);
    //    CP_XML_WRITER(current_sheet().cols())
    //    {
    //        CP_XML_NODE(L"col")
    //        {
				////CP_XML_ATTR(L"collapsed", L"false");            
				////CP_XML_ATTR(L"hidden", L"false");            
    //            CP_XML_ATTR(L"max", cMax);
    //            CP_XML_ATTR(L"min", cMin);
    //            //CP_XML_ATTR(L"style", 0);
    //            CP_XML_ATTR(L"width", lastWidht);
    //            CP_XML_ATTR(L"customWidth", 0);
    //        }
    //    }
    //}    
    //current_sheet().cols() << L"</cols>";
    
	//get_table_context().serialize_table_format(current_sheet().sheetFormat());

 //   get_table_context().serialize_merge_cells(current_sheet().mergeCells());
 //   get_table_context().serialize_hyperlinks(current_sheet().hyperlinks());
	//get_table_context().dump_rels_hyperlinks(current_sheet().hyperlinks_rels());

	//get_drawing_context().set_odf_packet_path(root()->get_folder());

    //get_drawing_context().process_charts(get_table_metrics());
    //get_drawing_context().process_images(get_table_metrics());
    //get_drawing_context().process_shapes(get_table_metrics());

	//if (!get_drawing_context().empty())
 //   {
 //       std::wstringstream strm;
 //       get_drawing_context().write_drawing(strm);
 //       
 //       const std::pair<std::wstring, std::wstring> drawingName 
 //           = xlsx_drawing_context_handle_.add_drawing_xml(strm.str(), get_drawing_context().get_drawings() );

 //       current_sheet().set_drawing_link(drawingName.first, drawingName.second);

 //       CP_XML_WRITER(current_sheet().drawing())
 //       {
 //           CP_XML_NODE(L"drawing")
 //           {
 //               CP_XML_ATTR(L"r:id", drawingName.second);
 //           }
 //       }

 //   }
	//if (!get_comments_context().empty())
 //   {
 //       std::wstringstream strm;
 //       get_comments_context().write_comments(strm);
 //       
 //       std::wstringstream vml_strm;
 //       get_comments_context().write_comments_vml(vml_strm);
	//	
	//	const std::pair<std::wstring, std::wstring> commentsName 
 //           = xlsx_comments_context_handle_.add_comments_xml(strm.str(), vml_strm.str(),get_comments_context().get_comments() );

	//	const std::pair<std::wstring, std::wstring> vml_drawingName 
	//							=xlsx_comments_context_handle_.get_vml_drawing_xml();

 //       current_sheet().set_comments_link(commentsName.first, commentsName.second);
 //       current_sheet().set_vml_drawing_link(vml_drawingName.first, vml_drawingName.second);
 //   }    

    get_table_context().end_table();
}

void xlsx_conversion_context::end_document()
{
	std::wstringstream workbook_content;

    unsigned int count = 0;
    // добавляем таблицы
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
	//добавляем диаграммы

	//count = 0;
 //   BOOST_FOREACH(const oox_chart_context_ptr& chart, charts_)
 //   {
	//	count++;
	//	package::chart_content_ptr content = package::chart_content::create();

	//	chart->write_to(content->content());

	//	output_document_->get_xl_files().add_charts(content);
	//
	//}
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

                //get_xlsx_defined_names().xlsx_serialize(CP_XML_STREAM());
            }
        }

        output_document_->get_xl_files().set_workbook( package::simple_element::create(L"workbook.xml", strm_workbook.str()) );

		output_document_->content_type().set_media(get_mediaitems());
        output_document_->get_xl_files().set_media(get_mediaitems());

        //package::xl_drawings_ptr drawings = package::xl_drawings::create(xlsx_drawing_context_handle_.content());
        //output_document_->get_xl_files().set_drawings(drawings);
	
        //package::xl_comments_ptr comments = package::xl_comments::create(xlsx_comments_context_handle_.content());
        //output_document_->get_xl_files().set_comments(comments);
        
	}

}

}