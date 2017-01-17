/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "draw_frame.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/odf/odf_document.h>

#include "serialize_elements.h"
#include "style_graphic_properties.h"
#include "odfcontext.h"

#include "../docx/xlsx_package.h"
#include "../docx/docx_package.h"
#include "../docx/pptx_package.h"

#include "datatypes/length.h"
#include "datatypes/borderstyle.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"
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
    draw_image_attlist_.add_attributes(Attributes);
    common_xlink_attlist_.add_attributes(Attributes);
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

// draw:chart
////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_chart::ns = L"draw";
const wchar_t * draw_chart::name = L"chart";

std::wostream & draw_chart::text_to_stream(std::wostream & _Wostream) const
{
    return _Wostream;
}

void draw_chart::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_chart_attlist_.add_attributes(Attributes);
    common_xlink_attlist_.add_attributes(Attributes);
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
}
// draw:frame
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_frame::ns = L"draw";
const wchar_t * draw_frame::name = L"frame";

std::wostream & draw_frame::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(content_);
    return _Wostream;
}

void draw_frame::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
 	idx_in_owner = -1;

	common_presentation_attlist_.add_attributes(Attributes);
    
	common_draw_attlists_.shape_with_text_and_styles_.add_attributes(Attributes);
    common_draw_attlists_.position_.add_attributes(Attributes);
    common_draw_attlists_.rel_size_.add_attributes(Attributes);
    
	presentation_shape_attlist_.add_attributes(Attributes);
    
	draw_frame_attlist_.add_attributes(Attributes);
}

void draw_frame::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (CP_CHECK_NAME(L"draw", L"text-box") ||
        CP_CHECK_NAME(L"draw", L"image") ||//копия объекта в виде картинки ну.. или просто картинка
        CP_CHECK_NAME(L"draw", L"object") ||//месторасположение embedded объекта
        CP_CHECK_NAME(L"draw", L"object-ole") ||
        CP_CHECK_NAME(L"draw", L"applet") ||
        CP_CHECK_NAME(L"draw", L"floating-frame") ||
        CP_CHECK_NAME(L"draw", L"plugin") ||
        CP_CHECK_NAME(L"table", L"table") 
        )
    {
        CP_CREATE_ELEMENT(content_);
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

std::wostream & draw_text_box::text_to_stream(std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(content_);
    return _Wostream;
}

void draw_text_box::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_text_box_attlist_.add_attributes(Attributes);
}

void draw_text_box::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_); 
}

void draw_text_box::add_text(const std::wstring & Text)
{
}

// draw:object
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_object::ns = L"draw";
const wchar_t * draw_object::name = L"object";

void draw_object::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_object_attlist_.add_attributes(Attributes);
    common_xlink_attlist_.add_attributes(Attributes);
}

void draw_object::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM(); // TODO
}

// draw:object
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_object_ole::ns = L"draw";
const wchar_t * draw_object_ole::name = L"object-ole";

void draw_object_ole::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:class-id", draw_class_id_);

    common_xlink_attlist_.add_attributes(Attributes);
}

void draw_object_ole::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM(); 
}

std::wstring draw_object_ole::detectObject(const std::wstring &fileName)
{
	POLE::Storage *storage = new POLE::Storage(fileName.c_str());
	if (storage == NULL) return L"";

	if (storage->open(false, false) == false)
	{
		delete storage;
		return L"";
	}
	std::wstring prog;
	POLE::Stream* pStream = new POLE::Stream(storage, "CompObj");
	if ((pStream) && (pStream->size() > 28))
	{
		//skip the CompObjHeader
		pStream->seek(28);

		int sz_obj = pStream->size() - 28;

		std::vector<std::string> str;
		
		while (sz_obj > 0)
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
	return prog;
}


std::wstring draw_object::office_convert(odf_document * odfDocument, int type)
{
	std::wstring href_result;
	std::wstring folderPath		= odfDocument->get_folder();	
    std::wstring objectOutPath	= NSDirectory::CreateDirectoryWithUniqueName(folderPath);
	
	if (type == 1)
	{
		oox::package::docx_document	outputDocx;
		oox::docx_conversion_context conversionDocxContext ( odfDocument);
	   
		conversionDocxContext.set_output_document (&outputDocx);
		//conversionContext.set_font_directory	(fontsPath);
		
		if (odfDocument->docx_convert(conversionDocxContext))
		{	    
			outputDocx.write(objectOutPath);

			href_result = common_xlink_attlist_.href_.get_value_or(L"Object");
			int pos = href_result.find(L"./");
			if (pos >= 0) href_result = href_result.substr(2);
			
			href_result = L"docx" +  href_result + L".docx";
		}
	}
	if (type == 2)
	{
		oox::package::xlsx_document	outputXlsx;
		oox::xlsx_conversion_context conversionXlsxContext ( odfDocument);
	   
		conversionXlsxContext.set_output_document (&outputXlsx);
		//conversionContext.set_font_directory	(fontsPath);
		
		if (odfDocument->xlsx_convert(conversionXlsxContext))
		{	    
			outputXlsx.write(objectOutPath);

			href_result = common_xlink_attlist_.href_.get_value_or(L"Object");
			int pos = href_result.find(L"./");
			if (pos >= 0) href_result = href_result.substr(2);
			
			href_result = L"xlsx" +  href_result + L".xlsx";
		}
	}
	if (!href_result.empty())
	{
		std::wstring temp_file = folderPath + FILE_SEPARATOR_STR + href_result;

		COfficeUtils oCOfficeUtils(NULL);
		oCOfficeUtils.CompressFileOrDirectory(objectOutPath.c_str(), temp_file.c_str(), -1);
	}	
    NSDirectory::DeleteDirectory(objectOutPath);
	
	return href_result;
}

}
}
