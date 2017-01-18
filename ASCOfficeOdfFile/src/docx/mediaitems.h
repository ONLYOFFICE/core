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
#pragma once

#include <vector>
#include <string>

#include "oox_rels.h"

namespace cpdoccore { 
namespace oox {

class mediaitems
{
public:
    mediaitems(const std::wstring & odfPacket) : odf_packet_(odfPacket)
    {
		count_charts	= 0;
 		count_shape		= 0;
 		count_image		= 0;
 		count_tables	= 0;
 		count_media		= 0;
		count_object	= 0;
	}

    struct item 
    {
        item(	std::wstring const &	_href,
				RelsType				_type,
				std::wstring const &	_outputName,
				bool					_mediaInternal,
				std::wstring const &	_Id);

        std::wstring	href;
        RelsType		type;
        std::wstring	outputName;
        bool			mediaInternal;
        bool			valid;
		std::wstring	Id;
		int				count_used;
		int				count_add;
    };
    typedef std::vector< item > items_array;
	
	size_t count_charts;
	size_t count_image;
	size_t count_media;
	size_t count_shape;
	size_t count_tables;
	size_t count_object;

    std::wstring add_or_find(const std::wstring & href, RelsType type, bool & isInternal);//возможны ссылки на один и тот же объект
    std::wstring add_or_find(const std::wstring & href, RelsType type, bool & isInternal, std::wstring & ref);
    
	void dump_rels(rels & Rels);
    items_array & items() { return items_; }

	static std::wstring get_rel_type(RelsType type)
	{
		switch (type)
		{
		case typeImage:		return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";
		case typeChart:		return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart";
		case typeMsObject:	return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package";
		case typeOleObject:	return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject";
		case typeHyperlink:	return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink";
		default:
			return L"";
		}
	}

private:
	std::wstring create_file_name			(const std::wstring & uri, RelsType type, bool & isInternal, size_t Num);
	std::wstring detectImageFileExtension	(std::wstring &fileName);
    
	items_array		items_;
    std::wstring	odf_packet_;

};

}
}

