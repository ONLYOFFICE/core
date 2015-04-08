#include "../odf/precompiled_cpodf.h"
#include "xlsx_output_xml.h"
#include <boost/make_shared.hpp>
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

/// \class  xlsx_xml_worksheet::Impl
class xlsx_xml_worksheet::Impl
{
public:
    Impl(std::wstring const & name) : name_(name){}
    std::wstring name_;
  
	std::wstringstream  cols_;
    std::wstringstream  sheetFormat_;
    std::wstringstream  sheetData_;
    std::wstringstream  mergeCells_;
    std::wstringstream  drawing_;
    std::wstringstream  hyperlinks_;
    std::wstringstream  comments_;

	rels hyperlinks_rels_;

    std::wstring drawingName_;
    std::wstring drawingId_;

    std::wstring commentsName_;
    std::wstring commentsId_;

	std::wstring vml_drawingName_;
	std::wstring vml_drawingId_;
};

std::wstring xlsx_xml_worksheet::name() const
{
    return impl_->name_;
}

xlsx_xml_worksheet_ptr xlsx_xml_worksheet::create(std::wstring const & name)
{
    return boost::make_shared<xlsx_xml_worksheet>(name);
}

xlsx_xml_worksheet::xlsx_xml_worksheet(std::wstring const & name)
 : impl_(new xlsx_xml_worksheet::Impl(name))
{
}

xlsx_xml_worksheet::~xlsx_xml_worksheet()
{
}

std::wostream & xlsx_xml_worksheet::cols()
{
    return impl_->cols_;
}
std::wostream & xlsx_xml_worksheet::sheetFormat()
{
    return impl_->sheetFormat_;
}

std::wostream & xlsx_xml_worksheet::sheetData()
{
    return impl_->sheetData_;
}

std::wostream & xlsx_xml_worksheet::mergeCells()
{
    return impl_->mergeCells_;
}

std::wostream & xlsx_xml_worksheet::drawing()
{
    return impl_->drawing_;
}
std::wostream & xlsx_xml_worksheet::comments()
{
    return impl_->comments_;
}
std::wostream & xlsx_xml_worksheet::hyperlinks()
{
    return impl_->hyperlinks_;
}
rels & xlsx_xml_worksheet::hyperlinks_rels()
{
    return impl_->hyperlinks_rels_;
}
void xlsx_xml_worksheet::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"worksheet")
        {
            CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");        
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
            CP_XML_ATTR(L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006");
            CP_XML_ATTR(L"mc:Ignorable", L"x14ac");
            CP_XML_ATTR(L"xmlns:x14ac", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac");

			CP_XML_STREAM() << impl_->sheetFormat_.str();

            CP_XML_STREAM() << impl_->cols_.str();

            CP_XML_NODE(L"sheetData")
            {
                CP_XML_STREAM() << impl_->sheetData_.str();
            }
			//оказываетс€ пор€док нахождени€ элементов важен !!! (дл€ office 2010)
			//объединенные €чейки раньше чем гиперлинки !!!
           
			CP_XML_STREAM() << impl_->mergeCells_.str();

            if (!impl_->hyperlinks_.str().empty())
            {
                CP_XML_NODE(L"hyperlinks")
                {
                    CP_XML_STREAM() << impl_->hyperlinks_.str();
                }
            }

            CP_XML_STREAM() << impl_->drawing_.str();
			if (impl_->commentsId_.length()>0)
			{
				CP_XML_NODE(L"legacyDrawing")
				{
					CP_XML_ATTR(L"r:id",impl_->vml_drawingId_);
				}
			}

			//CP_XML_NODE(L"headerFooter){}

			//CP_XML_NODE(L"rowBreaks){}

			//CP_XML_NODE(L"colBreaks){}
		}
    }
}

void xlsx_xml_worksheet::set_drawing_link(std::wstring const & fileName, std::wstring const & id)
{
    impl_->drawingName_ = fileName;
    impl_->drawingId_ = id;      
}
void xlsx_xml_worksheet::set_comments_link(std::wstring const & fileName, std::wstring const & id)
{
    impl_->commentsName_ = fileName;
    impl_->commentsId_ = id;      
}
void xlsx_xml_worksheet::set_vml_drawing_link(std::wstring const & fileName, std::wstring const & id)
{
    impl_->vml_drawingName_ = fileName;
    impl_->vml_drawingId_ = id;      
}
std::pair<std::wstring, std::wstring> xlsx_xml_worksheet::get_drawing_link() const
{
    return std::pair<std::wstring, std::wstring>(impl_->drawingName_, impl_->drawingId_);
}
std::pair<std::wstring, std::wstring> xlsx_xml_worksheet::get_comments_link() const
{
    return std::pair<std::wstring, std::wstring>(impl_->commentsName_, impl_->commentsId_);
}
std::pair<std::wstring, std::wstring> xlsx_xml_worksheet::get_vml_drawing_link() const
{
    return std::pair<std::wstring, std::wstring>(impl_->vml_drawingName_, impl_->vml_drawingId_);
}

//class xlsx_xml_workbook::Impl: noncopyable
//{
//public:
//    std::wstringstream sheets_;
//};
//
//xlsx_xml_workbook::xlsx_xml_workbook() : impl_(new xlsx_xml_workbook::Impl)
//{
//}
//
//xlsx_xml_workbook::~xlsx_xml_workbook()
//{
//}
//
//std::wostream & xlsx_xml_workbook::sheets()
//{
//    return impl_->sheets_;
//}
//
//void xlsx_xml_workbook::write_to(std::wostream & strm)
//{
//    CP_XML_WRITER(strm)
//    {
//        CP_XML_NODE(L"workbook")
//        {
//            CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
//            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
//
//            CP_XML_NODE(L"sheets")
//            {
//                CP_XML_STREAM() << impl_->sheets_.str();            
//            }
//        }    
//    }
//}
//


}
}
