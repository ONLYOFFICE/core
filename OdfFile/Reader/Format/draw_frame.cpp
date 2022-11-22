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

#include <ostream>
#include <sstream>
#include <string>

#include <boost/regex.hpp>

#include <xml/xmlchar.h>
#include "odf_document.h"

#include "draw_common.h"

#include "serialize_elements.h"
#include "style_graphic_properties.h"
#include "odfcontext.h"

#include "../../Reader/Converter/xlsx_package.h"
#include "../../Reader/Converter/docx_package.h"
#include "../../Reader/Converter/pptx_package.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../Common/OfficeFileFormatChecker.h"
#include "../../../Common/3dParty/pole/pole.h"

namespace cpdoccore { 
namespace odf_reader {

// draw-image-attlist
void draw_image_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:filter-name", draw_filter_name_);
}

/// draw-frame-attlist
void draw_frame_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:copy-of", draw_copy_of_);
    CP_APPLY_ATTR(L"fo:min-width", fo_min_width_);
    CP_APPLY_ATTR(L"fo:min-height", fo_min_height_);
}
/// draw-gframe-attlist
void draw_g_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
/// draw-chart-attlist

void draw_chart_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    //CP_APPLY_ATTR(L"draw:filter-name", draw_filter_name_);
}
// draw:image
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_image::ns = L"draw";
const wchar_t * draw_image::name = L"image";

void draw_image::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	office_element*  last_element = getContext()->get_last_element();
	if ((!last_element) || (last_element->get_type() != typeDrawFrame))
	{
		draw_frame_ptr = office_element_creator::get()->create(L"draw", L"frame", getContext(), false);
		draw_frame_ptr->add_attributes(Attributes);
	}
	
	draw_image_attlist_.add_attributes(Attributes);
    xlink_attlist_.add_attributes(Attributes);
}

void draw_image::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"office", L"binary-data")
    {
        CP_CREATE_ELEMENT(office_binary_data_);
    }
    else
    {
        CP_CREATE_ELEMENT(content_);
        //CP_NOT_APPLICABLE_ELM();
    }
}
std::wostream & draw_image::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return _Wostream;
}
// draw:chart
////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_chart::ns = L"draw";
const wchar_t * draw_chart::name = L"chart";

std::wostream & draw_chart::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return _Wostream;
}

void draw_chart::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_chart_attlist_.add_attributes(Attributes);
    xlink_attlist_.add_attributes(Attributes);
}

void draw_chart::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    //if CP_CHECK_NAME(L"office", L"title")
    //{
    //    CP_CREATE_ELEMENT(title_);
    //}
    //else 
    //{
        CP_CREATE_ELEMENT(content_);
    //}
}


// draw:g
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_g::ns = L"draw";
const wchar_t * draw_g::name = L"g";
void draw_g::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_draw_attlists_.shape_with_text_and_styles_.add_attributes(Attributes);
    common_draw_attlists_.position_.add_attributes(Attributes);
    common_draw_attlists_.rel_size_.add_attributes(Attributes);
    
	draw_g_attlist_.add_attributes(Attributes);
}
void draw_g::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);

	if (content_.empty()) return;

	draw_g		*group = dynamic_cast<draw_g*>		(content_.back().get());
	draw_frame	*frame = dynamic_cast<draw_frame*>	(content_.back().get());
	draw_shape	*shape = dynamic_cast<draw_shape*>	(content_.back().get());

	if (group)
	{		
		int x = 0, y = 0, cx = 0, cy = 0;
		if (group->common_draw_attlists_.position_.svg_x_ && group->common_draw_attlists_.position_.svg_y_)
		{
			x = get_value_emu(group->common_draw_attlists_.position_.svg_x_);
			y = get_value_emu(group->common_draw_attlists_.position_.svg_y_);
		}
		else
		{
			x = group->position_child_x1;
			y = group->position_child_y1;
		}

		if (group->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ && 
			group->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_)
		{
			cx = get_value_emu(group->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_);
			cy = get_value_emu(group->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_);
		}
		else
		{
			cx = group->position_child_x2 - group->position_child_x1;
			cy = group->position_child_y2 - group->position_child_y1;
		}
		
		if (position_child_x1 > x || position_child_x1 == 0x7fffffff) position_child_x1 = x;
		if (position_child_y1 > y || position_child_y1 == 0x7fffffff) position_child_y1 = y;
		
		if (position_child_x2 < x + cx || position_child_x2 == 0x7fffffff) position_child_x2 = x + cx;
		if (position_child_y2 < y + cy || position_child_y2 == 0x7fffffff) position_child_y2 = y + cy;
	}
	else if (frame)
	{
		int x = 0, y = 0, cx = 0, cy = 0;
		x = get_value_emu(frame->common_draw_attlists_.position_.svg_x_);
		y = get_value_emu(frame->common_draw_attlists_.position_.svg_y_);

		cx = get_value_emu(frame->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_);
		cy = get_value_emu(frame->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_);

		if (position_child_x1 > x || position_child_x1 == 0x7fffffff) position_child_x1 = x;
		if (position_child_y1 > y || position_child_y1 == 0x7fffffff) position_child_y1 = y;
		
		if (position_child_x2 < x + cx || position_child_x2 == 0x7fffffff) position_child_x2 = x + cx;
		if (position_child_y2 < y + cy || position_child_y2 == 0x7fffffff) position_child_y2 = y + cy;

		if (frame->is_object_)
		{
			object_index = content_.size() - 1;
		}
	}
	else if (shape)
	{
		int x = 0, y = 0, cx = 0, cy = 0;
		x = get_value_emu(shape->common_draw_attlists_.position_.svg_x_);
		y = get_value_emu(shape->common_draw_attlists_.position_.svg_y_);

		cx = get_value_emu(shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_);
		cy = get_value_emu(shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_);

		if (position_child_x1 > x || position_child_x1 == 0x7fffffff) position_child_x1 = x;
		if (position_child_y1 > y || position_child_y1 == 0x7fffffff) position_child_y1 = y;
		
		if (position_child_x2 < x + cx || position_child_x2 == 0x7fffffff) position_child_x2 = x + cx;
		if (position_child_y2 < y + cy || position_child_y2 == 0x7fffffff) position_child_y2 = y + cy;
	}
}

std::wostream & draw_g::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(content_, bXmlEncode);
    return _Wostream;
}

// draw:frame
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_frame::ns = L"draw";
const wchar_t * draw_frame::name = L"frame";

std::wostream & draw_frame::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(content_, bXmlEncode);
    return _Wostream;
}

void draw_frame::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
 	idx_in_owner = -1;

	common_draw_attlists_.shape_with_text_and_styles_.add_attributes(Attributes);
    common_draw_attlists_.position_.add_attributes(Attributes);
    common_draw_attlists_.rel_size_.add_attributes(Attributes);
    
	draw_frame_attlist_.add_attributes(Attributes);
}

void draw_frame::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (CP_CHECK_NAME(L"draw", L"text-box") ||
        CP_CHECK_NAME(L"draw", L"image")	||//копия объекта в виде картинки ну.. или просто картинка
        CP_CHECK_NAME(L"table", L"table")	||
        CP_CHECK_NAME(L"draw", L"object-ole")||
        CP_CHECK_NAME(L"draw", L"applet")	||
        CP_CHECK_NAME(L"draw", L"floating-frame")||
        CP_CHECK_NAME(L"draw", L"plugin")
		)	
    {
        CP_CREATE_ELEMENT(content_);
    }
	else if CP_CHECK_NAME(L"draw", L"object")	//embedded объект
    {
        CP_CREATE_ELEMENT(content_);
		is_object_ = true;
    }
    else if CP_CHECK_NAME(L"office", L"event-listeners")
    {
        CP_CREATE_ELEMENT(office_event_listeners_);                    
    }
    else if CP_CHECK_NAME(L"draw", L"glue-point")
    {
        CP_CREATE_ELEMENT(draw_glue_point_);
    }
    else if CP_CHECK_NAME(L"draw", L"image-map")
    {
        CP_CREATE_ELEMENT(draw_image_map_);
    }
    else if (   CP_CHECK_NAME(L"draw", L"contour-polygon") ||
                CP_CHECK_NAME(L"draw", L"contour-path") )
    {
        CP_CREATE_ELEMENT(draw_contour_);        
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}


///////////////////////

void draw_text_box_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:chain-next-name", draw_chain_next_name_);
    CP_APPLY_ATTR(L"draw:corner-radius", draw_corner_radius_);
    CP_APPLY_ATTR(L"fo:min-width", fo_min_width_);
    CP_APPLY_ATTR(L"fo:min-height", fo_min_height_);
    CP_APPLY_ATTR(L"fo:max-width", fo_max_width_);
    CP_APPLY_ATTR(L"fo:max-height", fo_max_height_);
}

// draw:text-box
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_text_box::ns = L"draw";
const wchar_t * draw_text_box::name = L"text-box";

std::wostream & draw_text_box::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(content_, bXmlEncode);
    return _Wostream;
}

void draw_text_box::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	office_element*  last_element = getContext()->get_last_element();
	if ((!last_element) || (last_element->get_type() != typeDrawFrame))
	{
		draw_frame_ptr = office_element_creator::get()->create(L"draw", L"frame", getContext(), false);
		draw_frame_ptr->add_attributes(Attributes);
	}
    draw_text_box_attlist_.add_attributes(Attributes);
}

void draw_text_box::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_); 
}

// draw:object
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_object::ns = L"draw";
const wchar_t * draw_object::name = L"object";

void draw_object::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	office_element*  last_element = getContext()->get_last_element();
	if ((!last_element) || (last_element->get_type() != typeDrawFrame))
	{
		draw_frame_ptr = office_element_creator::get()->create(L"draw", L"frame", getContext(), false);
		draw_frame_ptr->add_attributes(Attributes);
	}
	
	draw_object_attlist_.add_attributes(Attributes);
    xlink_attlist_.add_attributes(Attributes);
}

void draw_object::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"office", L"document")
    {
		//embedded
		odf_document_ = odf_document_ptr( new odf_document(Reader, NSDirectory::GetTempPath()));
    }
}

// draw:object-ole
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_object_ole::ns = L"draw";
const wchar_t * draw_object_ole::name = L"object-ole";

void draw_object_ole::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:class-id", draw_class_id_);

    xlink_attlist_.add_attributes(Attributes);
}

void draw_object_ole::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM(); 
}

void draw_object_ole::detectObject(const std::wstring &fileName, std::wstring &prog, std::wstring &extension, oox::_rels_type &rels)
{
	extension = L".bin";

	POLE::Storage *storage = new POLE::Storage(fileName.c_str());
	if ((storage) && (storage->open(false, false) == true))
	{
		rels = oox::typeOleObject;

		std::wstring prog;
		POLE::Stream* pStream = new POLE::Stream(storage, L"CompObj");
		if ((pStream) && (pStream->size() > 28))
		{
			//skip the CompObjHeader
			pStream->seek(28);

			int sz_obj = (int)pStream->size() - 28;

			std::vector<std::string> str;
			
			while (sz_obj > 4)
			{
				_UINT32 sz = 0;			
				pStream->read((unsigned char*)&sz, 4); sz_obj-= 4;
				
				if (sz > sz_obj) 
					break;
				unsigned char *data  = new unsigned char[sz];
				pStream->read(data, sz);

				str.push_back(std::string((char*)data, sz));
				delete []data;

				sz_obj-= sz;
			}
			if (!str.empty())
			{
				prog = std::wstring (str.back().begin(), str.back().end());
			}
			delete pStream;
		}
		delete storage;
	}
	else
	{
		COfficeFileFormatChecker checker(fileName);
		switch(checker.nFileType)
		{
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC:		extension = L".doc"; prog = L"Word"; rels = oox::typeOleObject; break;
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:		extension = L".docx"; prog = L"Word"; rels = oox::typeMsObject; break; 

		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS:		extension = L".xls"; prog = L"Excel"; rels = oox::typeOleObject; break;
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:	extension = L".xlsx"; prog = L"Excel"; rels = oox::typeMsObject; break;

		case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT:	extension = L".ppt"; prog = L"PowerPoint"; rels = oox::typeOleObject; break;
		case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:	extension = L".pptx"; prog = L"PowerPoint"; rels = oox::typeMsObject; break;

		}
	}
}


std::wstring draw_object::office_convert(odf_document_ptr odfDocument, int type)
{
	if (!odfDocument) return L"";

	std::wstring href_result;
	std::wstring folderPath		= odfDocument->get_folder();	
    std::wstring objectOutPath	= NSDirectory::CreateDirectoryWithUniqueName(folderPath);
	
	if (objectOutPath.empty()) return L"";

	if (type == 1)
	{
		oox::package::docx_document	outputDocx;
		oox::docx_conversion_context conversionDocxContext ( odfDocument.get());
	   
		conversionDocxContext.set_output_document (&outputDocx);
		//conversionContext.set_font_directory	(fontsPath);
		
		if (odfDocument->docx_convert(conversionDocxContext))
		{	    
			outputDocx.write(objectOutPath);

			href_result = xlink_attlist_.href_.get_value_or(L"Object");
			int pos = href_result.find(L"./");
			if (pos >= 0) href_result = href_result.substr(2);
			
			href_result = L"docx" +  href_result + L".docx";
		}
	}
	if (type == 2)
	{
		oox::package::xlsx_document	outputXlsx;
		oox::xlsx_conversion_context conversionXlsxContext ( odfDocument.get());
	   
		conversionXlsxContext.set_output_document (&outputXlsx);
		//conversionContext.set_font_directory	(fontsPath);
		
		if (odfDocument->xlsx_convert(conversionXlsxContext))
		{	    
			outputXlsx.write(objectOutPath);

			href_result = xlink_attlist_.href_.get_value_or(L"Object");
			int pos = href_result.find(L"./");
			if (pos >= 0) href_result = href_result.substr(2);
			
			href_result = L"xlsx" +  href_result + L".xlsx";
		}
	}
	if (!href_result.empty())
	{
		std::wstring temp_file = folderPath + FILE_SEPARATOR_STR + href_result;

		COfficeUtils oCOfficeUtils(NULL);
		oCOfficeUtils.CompressFileOrDirectory(objectOutPath, temp_file, true);
	}	
    NSDirectory::DeleteDirectory(objectOutPath);
	
	return href_result;
}
// draw:param
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_param::ns = L"draw";
const wchar_t * draw_param::name = L"param";

void draw_param::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:name", draw_name_);
	CP_APPLY_ATTR(L"draw:value", draw_value_);
}

void draw_param::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM(); 
}
// draw:plugin
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_plugin::ns = L"draw";
const wchar_t * draw_plugin::name = L"plugin";

void draw_plugin::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:mime-type", draw_mime_type_);

    xlink_attlist_.add_attributes(Attributes);
}

void draw_plugin::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_); 
}

}
}
