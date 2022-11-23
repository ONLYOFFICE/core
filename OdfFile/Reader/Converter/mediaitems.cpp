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

#include "mediaitems.h"

#include <boost/regex.hpp>

#include <xml/utils.h>

#include "../../../OOXML/Base/Base.h"
#include "../../../OOXML/SystemUtility/File.h"

#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"

namespace cpdoccore { 
namespace oox {

bool is_internal(const std::wstring & uri, const std::wstring & packetRoot)
{
	if (uri.empty())return false;

    std::wstring mediaPath = boost::regex_search(uri.begin(), uri.end(), boost::wregex(L"^/[A-Za-z]:")) 
        ? std::wstring(uri.begin() + 1, uri.end()) 
        : uri;

	std::wstring  resultPath = packetRoot + FILE_SEPARATOR_STR + mediaPath;

	return NSFile::CFileBinary::Exists(resultPath) || NSDirectory::Exists(mediaPath);
}

mediaitems::item::item(std::wstring const & _href,_rels_type _type, std::wstring const & _outputName,
						bool _mediaInternal, std::wstring const & _Id, _rels_type_place type_place_)
           : href(_href), type(_type), outputName(_outputName), mediaInternal(_mediaInternal), Id(_Id), valid(true), type_place(type_place_)
		   //вообще говоря даже если файл покоцанный то мы все равно обязаны перенести "объект"
{    
	count_add = 1;
	count_used = 0;
}
mediaitems::mediaitems(const std::wstring & odfPacket) : odf_packet_(odfPacket)
{
	count_charts	= 0;
	count_shape		= 0;
	count_image		= 0;
	count_tables	= 0;
	count_media		= 0;
	count_object	= 0;
	count_audio		= 0;
	count_video		= 0;
	count_slide		= 0;
	count_activeX	= 0;	
	count_control	= 0;	

	applicationFonts_	= NSFonts::NSApplication::Create();
}
mediaitems::~mediaitems()
{
    if (applicationFonts_)
        delete applicationFonts_;
}
void mediaitems::set_font_directory(std::wstring pathFonts)
{
    if (applicationFonts_)
        applicationFonts_->InitializeFromFolder(pathFonts);
}

std::wstring mediaitems::add_or_find(const std::wstring & href, _rels_type type, bool & isInternal, _rels_type_place type_place)
{
    std::wstring ref;
    return add_or_find(href, type, isInternal, ref, type_place);
}

std::wstring static get_default_file_name(_rels_type type)
{
    switch (type)
    {
    case typeImage:
        return L"image";
    case typeChart:
        return L"chart";
    case typeMsObject:
       return L"msObject";
	case typeOleObject:
       return L"oleObject";
    case typeMedia:
        return L"media";
	case typeSlide:
        return L"slide";
	case typeVideo:
        return L"video"; 
	case typeAudio:
        return L"audio";
	case typeControl:
        return L"control";
	case typeControlProps:
        return L"controlProps";
	default:
        return L"";
    }
}
std::wstring mediaitems::create_file_name(const std::wstring & uri, _rels_type type, bool & isInternal, size_t Num)
{
	if (uri.empty()) return L"";

	std::wstring sExt;
	std::wstring f_name = odf_packet_ + FILE_SEPARATOR_STR + uri;

	if (type == typeImage)
	{
		sExt = detectImageFileExtension(f_name);	//4EA0AA6E-479D-4002-A6AA-6D6C88EC6D65.odt - image - "opentbs_added_1.phpxvkeg" = png
	}
	else
	{
	}

	if (sExt.empty())
	{
		size_t n = uri.find(L"ObjectReplacements");
		if (n != std::wstring::npos)
		{
			if (!isInternal) return L"";

			f_name = odf_packet_ + uri.substr(1, uri.length() - 1);
			sExt = detectImageFileExtension(f_name);
		}
		
		if (sExt.empty())
		{
			//то что есть .. 
			size_t n = uri.rfind(L".");
			if (n != std::wstring::npos) 
				sExt = XmlUtils::GetLower(uri.substr(n));
		}
	}

	if (type == typeOleObject && sExt.empty())
		sExt = L".bin";
	else if ( type == typeChart)
		sExt = L".xml";
   
	return get_default_file_name(type) + std::to_wstring(Num) + sExt;
}
std::wstring mediaitems::detectImageFileExtension(const std::wstring &fileName)
{
	NSFile::CFileBinary file;

	std::wstring sExt;
	if (true == file.OpenFile(fileName))
	{
		BYTE buffer[128];
		int buffer_size = 128;
		
		file.ReadFile(buffer, buffer_size);
		file.CloseFile();
		
		CImageFileFormatChecker image_checker;
		sExt = image_checker.DetectFormatByData(buffer, buffer_size);

		if (sExt.empty())
		{
			size_t n = fileName.rfind(L".");
			if (n != std::wstring::npos) 
				sExt = XmlUtils::GetLower(fileName.substr(n));

		}
		else sExt = std::wstring(L".") + sExt;
	}
	return XmlUtils::GetLower(sExt);
}

std::wstring mediaitems::add_or_find(const std::wstring & href, _rels_type type, bool & isInternal, std::wstring & ref, _rels_type_place type_place)
{
	bool isMediaInternal = true;
	std::wstring sub_path = L"media/";
	
	std::wstring inputFileName;
	if ( type == typeChart)
	{
		sub_path = L"charts/";
	}
	else if ( type == typeMsObject || type == typeOleObject)
	{
		sub_path = L"embeddings/";
	}
	else if ( type == typeControlProps)
	{
		sub_path = L"ctrlProps/";
	}
	else 
	{
		isMediaInternal = is_internal(href, odf_packet_);
		if (href.empty() && type == typeImage)
			return L"";
	}
	
	int number=0;
	
		 if ( type == typeChart)		number = count_charts	+ 1;
	else if ( type == typeImage)		number = count_image	+ 1;
	else if ( type == typeShape)		number = count_shape	+ 1;
	else if ( type == typeMedia)		number = count_media	+ 1;
	else if ( type == typeAudio)		number = count_audio	+ 1;
	else if ( type == typeVideo)		number = count_video	+ 1;
	else if ( type == typeSlide)		number = count_slide	+ 1;
	else if ( type == typeMsObject ||
			  type == typeOleObject)	number = count_object	+ 1;
	else if ( type == typeControl)		number = count_control	+ 1;
	else
		number = items_.size() + 1;
	
	inputFileName = create_file_name(href, type, isMediaInternal, number);
	
    std::wstring inputPath	= isMediaInternal ? odf_packet_ + FILE_SEPARATOR_STR + href : href;
	std::wstring outputPath	= isMediaInternal ? ( sub_path + inputFileName)		 : href;

	std::wstring id;
    for (size_t i = 0 ; i < items_.size(); i++)
    {
		if (items_[i].type_place != type_place) continue;

		if ((items_[i].href == inputPath && !inputPath.empty()) || (items_[i].type == type && inputPath.empty()))
		{
			id			= items_[i].Id;
			outputPath  = items_[i].outputName;

			items_[i].count_add++;
			break;
		}
	}

  	ref = outputPath;
    isInternal = isMediaInternal;

	if (id.empty())
	{
		if ( type == typeChart)
		{
			id = std::wstring(L"chId") + std::to_wstring(count_charts + 1);
			count_charts++;
		}
		else if ( type == typeImage)
		{
            size_t n_svm = outputPath.rfind (L".svm");
			if ( n_svm != std::wstring::npos )
			{
				outputPath = outputPath.substr(0, n_svm) + L".png"; 
			}
//------------------------------------------------
			//if (inputFileName.empty()) return L"";  - Book 27.ods - пустые линки на картинки

			id = std::wstring(L"picId") + std::to_wstring(count_image + 1);
			count_image++;
		}
		else if ( type == typeMsObject || type == typeOleObject)
		{
			id = std::wstring(L"objId") + std::to_wstring(count_object + 1);
			count_object++;
		}
		else if ( type == typeAudio)
		{
			id = std::wstring(L"aId") + std::to_wstring(count_audio + 1);
			count_audio++;
		}
		else if ( type == typeVideo)
		{
			id = std::wstring(L"vId") + std::to_wstring(count_video + 1);
			count_video++;
		}
		else if ( type == typeMedia)
		{
			id = std::wstring(L"mId") + std::to_wstring(count_media + 1);
			count_media++;
		}
		else
		{
			id = std::wstring(L"rId") + std::to_wstring(count_shape + 1);	
			count_shape++;
		}
		
		items_.push_back( item(inputPath, type, XmlUtils::EncodeXmlString(outputPath), isMediaInternal, id, type_place) );
	}

  	ref = outputPath;
    isInternal = isMediaInternal;
	return id;
}
void mediaitems::add_rels(bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type, _rels_type_place type_place)
{
	items_.push_back( item(L"", type, ref, isInternal, rid, type_place) );
}

std::wstring mediaitems::add_control_props(std::wstring & oox_target)
{
    const bool isMediaInternal = true;
  
	count_control++;  
	
	std::wstring rId = std::wstring(L"ctrlId") + std::to_wstring(count_control);

	oox_target = std::wstring(L"ctrlProp") + std::to_wstring(count_control) + L".xml";
		
	items_.push_back( item(L"", typeControlProps, oox_target, isMediaInternal, rId, oox::document_place) );
    return rId;
}
void mediaitems::dump_rels(rels & Rels, _rels_type_place type_place)
{
    for (size_t i = 0; i < items_.size(); i++)
    {
		if ( items_[i].type == typeUnknown ) continue; 
		if ( items_[i].type == typeShape ) continue; 
		if ( items_[i].type == typeGroupShape ) continue; 

		if (items_[i].type_place != type_place) continue;
		
		if (items_[i].count_used >= items_[i].count_add) continue; // уже использовали этот релс выше(колонтитул ....)

		Rels.add( relationship(
                items_[i].Id, 
                get_rel_type (items_[i].type), 
                items_[i].valid			? items_[i].outputName	: L"NULL", 
                items_[i].mediaInternal	? L""					: L"External" )
                );
		items_[i].count_used++;
	}        
}


}
}
