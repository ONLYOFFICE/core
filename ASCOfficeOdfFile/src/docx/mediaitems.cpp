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

#include "mediaitems.h"

#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>

#include <cpdoccore/xml/utils.h>

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"

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

mediaitems::item::item(	std::wstring const & _href,
                       RelsType _type,
                       std::wstring const & _outputName,
						bool _mediaInternal,
						std::wstring const & _Id
					   )
           : href(_href),
           type(_type),
           outputName(_outputName),
           mediaInternal(_mediaInternal),
		   Id(_Id),
           valid(true) //вообще говоря даже если файл покоцанный то мы все равно обязаны перенести "объект"
{    
	count_add = 1;
	count_used = 0;
}

std::wstring mediaitems::add_or_find(const std::wstring & href, RelsType type, bool & isInternal)
{
    std::wstring ref;
    return add_or_find(href, type, isInternal, ref);
}

std::wstring static get_default_file_name(RelsType type)
{
    switch (type)
    {
    case typeImage:
        return L"image";
    case typeChart:
        return L"chart";
    case typeMedia:
        return L"media";
    case typeMsObject:
       return L"msObject";
	case typeOleObject:
       return L"oleObject";
	default:
        return L"";
    }
}
std::wstring mediaitems::create_file_name(const std::wstring & uri, RelsType type, bool & isInternal, size_t Num)
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
		int n = uri.find(L"ObjectReplacements");
		if (n >= 0)
		{
			if (!isInternal) return L"";

			f_name = odf_packet_ + uri.substr(1, uri.length() - 1);
			sExt = detectImageFileExtension(f_name);
		}
		
		if (sExt.empty())
		{
			//то что есть .. 
			int n = uri.rfind(L".");
			if (n > 0) 
				sExt = uri.substr(n);
		}
	}

	if (type == typeOleObject && sExt.empty())
		sExt = L".bin";
   
	return get_default_file_name(type) + boost::lexical_cast<std::wstring>(Num) + sExt;
}

std::wstring mediaitems::detectImageFileExtension(std::wstring &fileName)
{
	CFile file;

	std::wstring sExt;
	if (file.OpenFile(fileName) == S_OK)
	{
		BYTE buffer[128];
		int buffer_size = 128;
		
		file.ReadFile(buffer, buffer_size);
		file.CloseFile();
		
		CImageFileFormatChecker image_checker;
		sExt = image_checker.DetectFormatByData(buffer, buffer_size);

		if (!sExt.empty()) sExt = std::wstring(L".") + sExt;
	}
	return sExt;
}

std::wstring mediaitems::add_or_find(const std::wstring & href, RelsType type, bool & isInternal, std::wstring & ref)
{
    bool isMediaInternal = is_internal(href, odf_packet_);
  
	std::wstring sub_path = L"media/";
	
	std::wstring inputFileName;
	if ( type == typeChart)
	{
		sub_path = L"charts/";
	}
	if ( type == typeMsObject || type == typeOleObject)
	{
		sub_path = L"embeddings/";
	}
	int number=0;
	
		 if ( type == typeChart)		number = count_charts	+ 1;
	else if ( type == typeImage)		number = count_image	+ 1;
	else if ( type == typeShape)		number = count_shape	+ 1;
	else if ( type == typeMedia)		number = count_media	+ 1;
	else if ( type == typeMsObject ||
			  type == typeOleObject)	number = count_object	+ 1;
	else
		number = items_.size() + 1;
	
	inputFileName = create_file_name(href, type, isMediaInternal, number);
	
    std::wstring inputPath	= isMediaInternal ? odf_packet_ + FILE_SEPARATOR_STR + href : href;
	std::wstring outputPath	= isMediaInternal ? ( sub_path + inputFileName)		 : href;
	
	if ( type == typeChart) outputPath = outputPath + L".xml";

	std::wstring id;
    for (int i = 0 ; i < items_.size(); i++)
    {
		if (items_[i].href == inputPath)
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
			id = std::wstring(L"chId") + boost::lexical_cast<std::wstring>(count_charts + 1);
			count_charts++;
		}
		else if ( type == typeImage)
		{
            int n_svm = outputPath.rfind (L".svm");
			if ( n_svm >= 0 )
			{
				outputPath = outputPath.substr(0, n_svm) + L".png"; 
			}
//------------------------------------------------
			if (inputFileName.empty()) return L"";

			id = std::wstring(L"picId") + boost::lexical_cast<std::wstring>(count_image + 1);
			count_image++;
		}
		else if ( type == typeMsObject || type == typeOleObject)
		{
			id = std::wstring(L"objId") + boost::lexical_cast<std::wstring>(count_object + 1);
			count_object++;
		}
		else
		{
			id = std::wstring(L"rId") + boost::lexical_cast<std::wstring>(count_shape + 1);	
			count_shape++;
		}
		
		items_.push_back( item(inputPath, type, xml::utils::replace_text_to_xml(outputPath), isMediaInternal, id) );
	}

  	ref = outputPath;
    isInternal = isMediaInternal;
	return id;
}

void mediaitems::dump_rels(rels & Rels)
{
    size_t i = 0;
    for (int i = 0; i < items_.size(); i++)
    {
		if (items_[i].count_used > items_[i].count_add) continue; // уже использовали этот релс выше(колонтитул ....)
        
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
