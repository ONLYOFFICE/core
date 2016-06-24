/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "xlsx_output_xml.h"
#include <boost/make_shared.hpp>

#include "simple_xml_writer.h"

namespace oox {

/// \class  xlsx_xml_worksheet::Impl
class xlsx_xml_worksheet::Impl
{
public:
    Impl(std::wstring const & name) : name_(name)
	{
		state_ = L"visible";
	}
    
	std::wstring name_;
	std::wstring state_;
  
	std::wstringstream  cols_;
	std::wstringstream  sheetPr_;
	std::wstringstream  sheetFormatPr_;
    std::wstringstream  sheetData_;
    std::wstringstream  mergeCells_;
    std::wstringstream  drawing_;
    std::wstringstream  hyperlinks_;
    std::wstringstream  comments_;
    std::wstringstream  dimension_;
    std::wstringstream  sheetViews_;
	std::wstringstream  pageProperties_;
	std::wstringstream  sortAndFilters_;
	std::wstringstream  customViews_;
	std::wstringstream  conditionalFormatting_;
	std::wstringstream  picture_;

	rels rels_;

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
std::wstring xlsx_xml_worksheet::state() const
{
    return impl_->state_;
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
std::wostream & xlsx_xml_worksheet::dimension()
{
    return impl_->dimension_;
}
std::wostream & xlsx_xml_worksheet::sheetViews()
{
    return impl_->sheetViews_;
}
std::wostream & xlsx_xml_worksheet::sheetFormat()
{
    return impl_->sheetFormatPr_;
}
std::wostream & xlsx_xml_worksheet::sheetProperties()
{
    return impl_->sheetPr_;
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
std::wostream & xlsx_xml_worksheet::pageProperties()
{
	return impl_->pageProperties_;
}
std::wostream & xlsx_xml_worksheet::comments()
{
    return impl_->comments_;
}
std::wostream & xlsx_xml_worksheet::hyperlinks()
{
    return impl_->hyperlinks_;
}
std::wostream & xlsx_xml_worksheet::sheetSortAndFilters()
{
	return impl_->sortAndFilters_;
}
std::wostream & xlsx_xml_worksheet::customViews()
{
    return impl_->customViews_;
}
std::wostream & xlsx_xml_worksheet::conditionalFormatting()
{
    return impl_->conditionalFormatting_;
}
std::wostream & xlsx_xml_worksheet::picture()
{
    return impl_->picture_;
}

//-----------------------------------------------------------------
rels & xlsx_xml_worksheet::sheet_rels()
{
    return impl_->rels_;
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

			CP_XML_STREAM() << impl_->sheetPr_.str();
			CP_XML_STREAM() << impl_->dimension_.str();
			CP_XML_STREAM() << impl_->sheetViews_.str();
			CP_XML_STREAM() << impl_->sheetFormatPr_.str();

            CP_XML_STREAM() << impl_->cols_.str();

            CP_XML_NODE(L"sheetData")
            {
                CP_XML_STREAM() << impl_->sheetData_.str();
            }
			//оказывается порядок нахождения элементов важен !!! (для office 2010)
			//объединенные ячейки раньше чем гиперлинки !!!

			CP_XML_STREAM() << impl_->sortAndFilters_.str();
           
			CP_XML_STREAM() << impl_->customViews_.str();
			
			CP_XML_STREAM() << impl_->mergeCells_.str();
			
			CP_XML_STREAM() << impl_->conditionalFormatting_.str();

			CP_XML_STREAM() << impl_->hyperlinks_.str();
  	
			CP_XML_STREAM() << impl_->pageProperties_.str();

            CP_XML_STREAM() << impl_->drawing_.str();
		
			if (impl_->commentsId_.length()>0)
			{
				CP_XML_NODE(L"legacyDrawing")
				{
					CP_XML_ATTR(L"r:id",impl_->vml_drawingId_);
				}
			}

			CP_XML_STREAM() << impl_->picture_.str();

			//CP_XML_NODE(L"rowBreaks){}

			//CP_XML_NODE(L"colBreaks){}
		}
    }
}
void xlsx_xml_worksheet::set_state (std::wstring const & state)
{
	impl_->state_ = state;
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

}

