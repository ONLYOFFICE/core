/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include <xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

//  xlsx_xml_worksheet::Impl
class xlsx_xml_worksheet::Impl
{
public:
    Impl(std::wstring const & name, bool hidden) : name_(name), hidden_(hidden) {}
    std::wstring name_;
	bool hidden_;
  
	void clear()
	{
		cols_.clear();
		sheetFormat_.clear();
		sheetData_.clear();
		mergeCells_.clear();
		hyperlinks_.clear();
		comments_.clear();
		sort_.clear();
		tableParts_.clear();
		autofilter_.clear();
		conditionalFormatting_.clear();
		picture_background_.clear();
		dataValidations_.clear();
		dataValidationsX14_.clear();
		ole_objects_.clear();
		page_props_.clear();
		header_footer_.clear();
		controls_.clear();
		protection_.clear();
		breaks_.clear();
	}

	std::wstringstream  cols_;
    std::wstringstream  sheetFormat_;
    std::wstringstream  sheetData_;
    std::wstringstream  mergeCells_;
    std::wstringstream  hyperlinks_;
    std::wstringstream  comments_;
    std::wstringstream	sort_;
	std::wstringstream	tableParts_;
    std::wstringstream	autofilter_;
	std::wstringstream	conditionalFormatting_;
	std::wstringstream  picture_background_;
	std::wstringstream  dataValidations_;
	std::wstringstream  dataValidationsX14_;
	std::wstringstream	ole_objects_;
	std::wstringstream	page_props_;
	std::wstringstream	header_footer_;
	std::wstringstream	controls_;
	std::wstringstream	protection_;
	std::wstringstream	breaks_;

	rels sheet_rels_;

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
bool xlsx_xml_worksheet::hidden() const
{
    return impl_->hidden_;
}

xlsx_xml_worksheet_ptr xlsx_xml_worksheet::create(std::wstring const & name, bool hidden)
{
    return boost::make_shared<xlsx_xml_worksheet>(name, hidden);
}

xlsx_xml_worksheet::xlsx_xml_worksheet(std::wstring const & name, bool hidden)
 : impl_(new xlsx_xml_worksheet::Impl(name, hidden))
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
std::wostream & xlsx_xml_worksheet::tableParts()
{
    return impl_->tableParts_;
}
std::wostream & xlsx_xml_worksheet::conditionalFormatting()
{
    return impl_->conditionalFormatting_;
}
std::wostream & xlsx_xml_worksheet::sort()
{
    return impl_->sort_;
}
std::wostream & xlsx_xml_worksheet::autofilter()
{
    return impl_->autofilter_;
}
std::wostream & xlsx_xml_worksheet::comments()
{
    return impl_->comments_;
}
std::wostream & xlsx_xml_worksheet::hyperlinks()
{
    return impl_->hyperlinks_;
}
std::wostream & xlsx_xml_worksheet::ole_objects()
{
    return impl_->ole_objects_;
}
std::wostream & xlsx_xml_worksheet::controls()
{
    return impl_->controls_;
}
std::wostream & xlsx_xml_worksheet::page_properties()
{
    return impl_->page_props_;
}
std::wostream & xlsx_xml_worksheet::header_footer()
{
    return impl_->header_footer_;
}
std::wostream & xlsx_xml_worksheet::picture_background()
{
    return impl_->picture_background_;
}
std::wostream & xlsx_xml_worksheet::dataValidations()
{
    return impl_->dataValidations_;
}
std::wostream & xlsx_xml_worksheet::dataValidationsX14()
{
	return impl_->dataValidationsX14_;
}
std::wostream & xlsx_xml_worksheet::protection()
{
    return impl_->protection_;
}
std::wostream & xlsx_xml_worksheet::breaks()
{
    return impl_->breaks_;
}
//---------------------------------------------------------------------------------------
rels & xlsx_xml_worksheet::sheet_rels()
{
    return impl_->sheet_rels_;
}

void xlsx_xml_worksheet::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"worksheet")
        {
            CP_XML_ATTR(L"xmlns",		L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");        
            CP_XML_ATTR(L"xmlns:r",		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:xdr",	L"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing");
			CP_XML_ATTR(L"xmlns:x14",	L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");
            CP_XML_ATTR(L"xmlns:mc",	L"http://schemas.openxmlformats.org/markup-compatibility/2006");
            CP_XML_ATTR(L"mc:Ignorable",L"x14ac");
            CP_XML_ATTR(L"xmlns:x14ac", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac");

			CP_XML_STREAM() << impl_->sheetFormat_.str();

            CP_XML_STREAM() << impl_->cols_.str();

            CP_XML_NODE(L"sheetData")
            {
				if (impl_->sheetData_.rdbuf()->in_avail() != 0)
				{
					impl_->sheetData_.flush();
					CP_XML_STREAM() << impl_->sheetData_.rdbuf();
				}
            }
			std::wstring protect = impl_->protection_.str();
			if (false == protect.empty())
            {
				CP_XML_STREAM() << protect;
			}
			//оказывается порядок нахождения элементов важен !!! (для office 2010)
			//объединенные ячейки раньше чем гиперлинки !!!
			
			CP_XML_STREAM() << impl_->autofilter_.str(); //автофильтры перед merge !!!
			
			CP_XML_STREAM() << impl_->mergeCells_.str();
			
			CP_XML_STREAM() << impl_->sort_.str();

			CP_XML_STREAM() << impl_->conditionalFormatting_.str();

			CP_XML_STREAM() << impl_->dataValidations_.str();
			
			std::wstring hyperlinks = impl_->hyperlinks_.str();
			if (false == hyperlinks.empty())
            {
                CP_XML_NODE(L"hyperlinks")
                {
                    CP_XML_STREAM() << hyperlinks;
                }
            }
			CP_XML_STREAM() << impl_->page_props_.str();
			//props выше legacyDrawing !!

			CP_XML_STREAM() << impl_->header_footer_.str();
			
			CP_XML_STREAM() << impl_->breaks_.str();
			
			if (false == impl_->drawingId_.empty())
			{
				CP_XML_NODE(L"drawing")
				{
					CP_XML_ATTR(L"r:id", impl_->drawingId_);
				}
			} 			
			if (false == impl_->vml_drawingId_.empty())
			{
				CP_XML_NODE(L"legacyDrawing")
				{
					CP_XML_ATTR(L"r:id", impl_->vml_drawingId_);
				}
			}
			std::wstring oleObjects = impl_->ole_objects_.str();
			if (false == oleObjects.empty())
            {
                CP_XML_NODE(L"oleObjects")
                {
					CP_XML_STREAM() << oleObjects;
                }
            }
			std::wstring controls = impl_->controls_.str();
			if (false == impl_->controls_.str().empty())
            {
                CP_XML_NODE(L"controls")
                {
					CP_XML_STREAM() << controls;
                }
            }
			std::wstring tableParts = impl_->tableParts_.str();
			if (false == tableParts.empty())
            {
                CP_XML_NODE(L"tableParts")
                {
					CP_XML_STREAM() << tableParts;
				}
			}
			CP_XML_STREAM() << impl_->picture_background_.str();

			std::wstring dataValidations14 = impl_->dataValidationsX14_.str();
			if (false == dataValidations14.empty())
			{
				CP_XML_NODE(L"extLst")
				{
					CP_XML_NODE(L"ext")
					{
						CP_XML_ATTR(L"uri", L"{CCE6A557-97BC-4b89-ADB6-D9C93CAAB3DF}");
						CP_XML_ATTR(L"xmlns:x14", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");

						CP_XML_STREAM() << dataValidations14;
					}
				}
			}
		}
    }
	impl_->clear();
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
}
