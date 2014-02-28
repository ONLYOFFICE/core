#include "precompiled_cpodf.h"

#include "xlsxconversioncontext.h"

#include "measuredigits.h"
#include "xlsx_package.h"

#include "xlsx_cell_format.h"
#include "../odf/calcs_styles.h"
#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/CPAssert.h>
#include "xlsx_utils.h"

#include <iostream>

namespace cpdoccore { 

namespace odf 
{
    class odf_document;
}

namespace oox {
    
namespace package
{
    class xlsx_document;
}

xlsx_conversion_context::
xlsx_conversion_context(::cpdoccore::oox::package::xlsx_document * outputDocument,
                        ::cpdoccore::odf::odf_document * odfDocument): output_document_(outputDocument),
	odf_document_(odfDocument),
	xlsx_text_context_(odf_document_->odf_context().styleContainer()),
	xlsx_table_context_(*this, xlsx_text_context_),
	
	maxDigitSize_ (std::pair<float,float>(-1.0, -1.0) ),
	default_style_( (std::numeric_limits<size_t>::max)() ),
	mediaitems_(odf_document_->get_folder()),
	xlsx_drawing_context_handle_(mediaitems_)
{
}
void xlsx_conversion_context::start_chart(std::wstring const & name)
{
	charts_.push_back(oox_chart_context::create(name));
	//добавл€ем новую форму дл€ диаграммы
	 //в ней будет информационна€ часть - и она пишетс€ каждый раз в свою xml (их - по числу диаграмм)
	//этот контекст нужно передавать в файл

}

void xlsx_conversion_context::end_chart()
{
	//current_chart().set_drawing_link(current_sheet().get_drawing_link());
	//излишн€€ инфа
}

void xlsx_conversion_context::start_document()
{
    odf::odf_read_context & odfContext = root()->odf_context();
    std::vector<const odf::style_instance *> instances;
    
	instances.push_back(odfContext.styleContainer().style_default_by_type(odf::style_family::TableCell));
	instances.push_back(odfContext.styleContainer().style_by_name(L"Default",odf::style_family::TableCell,false));

    odf::text_format_properties_content			textFormatProperties	= calc_text_properties_content(instances);
    odf::paragraph_format_properties			parFormatProperties		= calc_paragraph_properties_content(instances);
    odf::style_table_cell_properties_attlist	cellFormatProperties	= calc_table_cell_properties(instances);

    oox::xlsx_cell_format cellFormat;
   
	cellFormat.set_cell_type(XlsxCellType::s);
    cellFormat.set_num_format(oox::odf_string_to_build_in(L""));

    default_style_ = get_style_manager().xfId(&textFormatProperties, &parFormatProperties, &cellFormatProperties, &cellFormat, L"",true);

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

		chart->write_to(content->content());

		output_document_->get_xl_files().add_charts(content);
	
	}
    //workbook_content << L"<calcPr iterateCount=\"100\" refMode=\"A1\" iterate=\"false\" iterateDelta=\"0.0001\" />";

    {
        std::wstringstream strm;
        xlsx_text_context_.write_shared_strings(strm);
        output_document_->get_xl_files().set_sharedStrings( package::simple_element::create(L"sharedStrings.xml", strm.str()) );
    }

    {
        std::wstringstream strm;
        xlsx_style_.xlsx_serialize(strm);
        output_document_->get_xl_files().set_styles( package::simple_element::create(L"styles.xml", strm.str()) );
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

                ::cpdoccore::oox::xlsx_serialize(CP_XML_STREAM(), this->get_xlsx_defined_names());
            }
        }

        output_document_->get_xl_files().set_workbook( package::simple_element::create(L"workbook.xml", strm_workbook.str()) );
        output_document_->get_xl_files().set_media(get_mediaitems());

        package::xl_drawings_ptr drawings = package::xl_drawings::create(xlsx_drawing_context_handle_.content());
        output_document_->get_xl_files().set_drawings(drawings);
	
        package::xl_comments_ptr comments = package::xl_comments::create(xlsx_comments_context_handle_.content());
        output_document_->get_xl_files().set_comments(comments);
        
	}

}

void xlsx_conversion_context::start_body()
{}

void xlsx_conversion_context::end_body()
{}

oox_chart_context & xlsx_conversion_context::current_chart()
{
    if (!charts_.empty())
    {
        return *charts_.back().get();
    }
    else
    {
        CP_ASSERT(false);
        throw std::runtime_error("internal error");
    }
}
xlsx_xml_worksheet & xlsx_conversion_context::current_sheet()
{
    if (!sheets_.empty())
    {
        return *sheets_.back().get();
    }
    else
    {
        CP_ASSERT(false);
        throw std::runtime_error("internal error");
    }
}
void xlsx_conversion_context::create_new_sheet(std::wstring const & name)
{
    sheets_.push_back(xlsx_xml_worksheet::create(name));
}
bool xlsx_conversion_context::start_table(const std::wstring & tableName, const std::wstring & tableStyleName)
{
    // TODO : nested tables forbidden
    if (get_table_context().depth() > 0)
        return false;

    create_new_sheet(tableName);
    get_table_context().start_table(tableName, tableStyleName);

	current_sheet().cols() << L"<cols>";
    return true;
}

void xlsx_conversion_context::end_table()
{
    const double lastWidht = table_column_last_width();
    if (lastWidht > 0.0)
    {
        unsigned int cMin = get_table_context().columns_count() + 1;
        unsigned int cMax = (std::max)((unsigned int)1024, get_table_context().columns_count() + 100);
        CP_XML_WRITER(current_sheet().cols())
        {
            CP_XML_NODE(L"col")
            {
                CP_XML_ATTR(L"min", cMin);
                CP_XML_ATTR(L"max", cMax);
                CP_XML_ATTR(L"width", lastWidht);
                CP_XML_ATTR(L"customWidth", 1);
            }
        }
    }    
    current_sheet().cols() << L"</cols>";
    
	get_table_context().serialize_table_format(current_sheet().sheetFormat());

    get_table_context().serialize_merge_cells(current_sheet().mergeCells());
    get_table_context().serialize_hyperlinks(current_sheet().hyperlinks());
	get_table_context().dump_rels_hyperlinks(current_sheet().hyperlinks_rels());

	get_drawing_context().set_odf_packet_path(root()->get_folder());

    get_drawing_context().process_charts(get_table_metrics());
    get_drawing_context().process_images(get_table_metrics());
    get_drawing_context().process_shapes(get_table_metrics());

	if (!get_drawing_context().empty())
    {
        std::wstringstream strm;
        get_drawing_context().write_drawing(strm);
        
        const std::pair<std::wstring, std::wstring> drawingName 
            = xlsx_drawing_context_handle_.add_drawing_xml(strm.str(), get_drawing_context().get_drawings() );

        current_sheet().set_drawing_link(drawingName.first, drawingName.second);

        CP_XML_WRITER(current_sheet().drawing())
        {
            CP_XML_NODE(L"drawing")
            {
                CP_XML_ATTR(L"r:id", drawingName.second);
            }
        }

    }
	if (!get_comments_context().empty())
    {
        std::wstringstream strm;
        get_comments_context().write_comments(strm);
        
        std::wstringstream vml_strm;
        get_comments_context().write_comments_vml(vml_strm);
		
		const std::pair<std::wstring, std::wstring> commentsName 
            = xlsx_comments_context_handle_.add_comments_xml(strm.str(), vml_strm.str(),get_comments_context().get_comments() );

		const std::pair<std::wstring, std::wstring> vml_drawingName 
								=xlsx_comments_context_handle_.get_vml_drawing_xml();

        current_sheet().set_comments_link(commentsName.first, commentsName.second);
        current_sheet().set_vml_drawing_link(vml_drawingName.first, vml_drawingName.second);
    }    

    get_table_context().end_table();
}

void xlsx_conversion_context::dump_sheet()
{    
    if (!sheets_.empty())
    {
        sheets_.pop_back();
    }
}

void xlsx_conversion_context::start_table_column(unsigned int repeated, const std::wstring & defaultCellStyleName, int & cMin, int & cMax)
{
    cMin = get_table_context().columns_count();
    get_table_context().start_column(repeated, defaultCellStyleName);
    cMax = get_table_context().columns_count();
}

void xlsx_conversion_context::end_table_column()
{
}

void xlsx_conversion_context::set_table_row_group(int count, bool collapsed, int level)
{
	get_table_context().set_table_row_group(count, collapsed, level);
}

void xlsx_conversion_context::start_table_row(const std::wstring & styleName, const std::wstring & defaultCellStyleName)
{
    get_table_context().start_row(styleName, defaultCellStyleName);
}

void xlsx_conversion_context::non_empty_row()
{
    return get_table_context().non_empty_row();
}

bool xlsx_conversion_context::is_empty_row() const
{
    return get_table_context().is_empty_row();
}

void xlsx_conversion_context::end_table_row()
{
    get_table_context().end_row();
}

int xlsx_conversion_context::current_table_column() const
{
    return xlsx_table_context_.current_column();
}

int xlsx_conversion_context::current_table_row() const
{
    return xlsx_table_context_.current_row();
}

std::wstring xlsx_conversion_context::current_cell_address() const
{
    return oox::getCellAddress(current_table_column(), current_table_row());
}

void xlsx_conversion_context::start_office_spreadsheet(const odf::office_element * elm)
{
    spreadsheet_ = elm;
}

const odf::office_element * xlsx_conversion_context::get_spreadsheet()
{
    return spreadsheet_;
}

void xlsx_conversion_context::end_office_spreadsheet()
{}

void xlsx_conversion_context::start_paragraph(const std::wstring & styleName)
{
    xlsx_text_context_.start_paragraph(styleName);
}

void xlsx_conversion_context::end_paragraph()
{
    xlsx_text_context_.end_paragraph();
}

void xlsx_conversion_context::start_span(const std::wstring & styleName)
{
    xlsx_text_context_.start_span(styleName);
}

void xlsx_conversion_context::end_span()
{
    xlsx_text_context_.end_span();
}

void xlsx_conversion_context::start_table_cell(const std::wstring & formula, size_t columnsSpanned, size_t rowsSpanned)
{
    get_table_context().start_cell(formula, columnsSpanned, rowsSpanned);
}

void xlsx_conversion_context::end_table_cell()
{
    get_table_context().end_cell();
}

void xlsx_conversion_context::start_table_covered_cell()
{
    get_table_context().start_covered_cell();
}

void xlsx_conversion_context::end_table_covered_cell()
{
    get_table_context().end_covered_cell();
}

void xlsx_conversion_context::set_current_cell_style_id(unsigned int xfId)
{
    return get_table_context().set_current_cell_style_id(xfId);
}

int xlsx_conversion_context::get_current_cell_style_id()
{
    return get_table_context().get_current_cell_style_id();
}

std::pair<float,float> xlsx_conversion_context::getMaxDigitSize()
{
    if (maxDigitSize_.first <= 0.1)
    {
		std::wstring font_name;
		int font_size;

		std::vector<const odf::style_instance *> instances;
		
		odf::odf_read_context & odfContext = root()->odf_context();
		
		instances.push_back(odfContext.styleContainer().style_default_by_type(odf::style_family::TableCell));
		instances.push_back(odfContext.styleContainer().style_by_name(L"Default",odf::style_family::TableCell,false));

		odf::text_format_properties_content			textFormatProperties	= calc_text_properties_content(instances);

		if (textFormatProperties.style_font_name_)
			font_name = textFormatProperties.style_font_name_.get();
		else if (textFormatProperties.style_font_name_complex_)
			font_name = textFormatProperties.style_font_name_complex_.get();
		else if (textFormatProperties.style_font_name_asian_)
			font_name = textFormatProperties.style_font_name_asian_.get();
		else
			font_name = L"Arial";

		if ((textFormatProperties.fo_font_size_) && (textFormatProperties.fo_font_size_->get_type() == odf::font_size::Length))
			font_size = (int)(0.5 + textFormatProperties.fo_font_size_->get_length().get_value_unit(odf::length::pt));
		else
			font_size =10;
		
        maxDigitSize_ = utils::GetMaxDigitSizePixels(font_name.c_str(), font_size, getDefaultDpi());        
    }    
    return maxDigitSize_;
}

void xlsx_conversion_context::process_styles()
{
}

xlsx_table_metrics & xlsx_conversion_context::get_table_metrics()
{
    return get_table_context().get_table_metrics();
}

xlsx_drawing_context & xlsx_conversion_context::get_drawing_context()
{
    return get_table_context().get_drawing_context();
}

xlsx_drawing_context_handle & xlsx_conversion_context::get_drawing_context_handle()
{
    return xlsx_drawing_context_handle_;
}
xlsx_comments_context & xlsx_conversion_context::get_comments_context()
{
    return get_table_context().get_comments_context();
}

xlsx_comments_context_handle & xlsx_conversion_context::get_comments_context_handle()
{
    return xlsx_comments_context_handle_;
}

void xlsx_conversion_context::table_column_last_width(double w)
{
    return get_table_context().table_column_last_width(w);
}

double xlsx_conversion_context::table_column_last_width() const
{
    return get_table_context().table_column_last_width();
}

void xlsx_conversion_context::start_hyperlink(const std::wstring & styleName)
{	
	xlsx_text_context_.start_hyperlink();
	xlsx_text_context_.start_span(styleName);
	
	get_table_context().start_hyperlink();
}

void xlsx_conversion_context::end_hyperlink(std::wstring const & href)
{
	if (xlsx_text_context_.is_drawing_context() == false)
	{
		std::wstring content = xlsx_text_context_.end_span2();
		xlsx_text_context_.end_hyperlink(get_table_context().end_hyperlink(current_cell_address(), href, L""));
	}
	else
	{
		std::wstring hId = get_drawing_context().add_hyperlink(href,false);
		xlsx_text_context_.end_hyperlink(hId); 
		
		xlsx_text_context_.end_span2();
	}
}

}
}
