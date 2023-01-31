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
#pragma once

#include <vector>
#include <string>

namespace cpdoccore { 
namespace odf_writer {

class rels;

class _mediaitems
{
public:
    enum Type { typeUnknown = 0, typeImage, typeMedia, typeOleObject, typeObjectReplacement};

    struct item 
    {
		item(const std::wstring & _oox_ref, Type _type, const std::wstring & _odf_ref);

        Type type = typeUnknown;
        
		std::wstring oox_ref;
        std::wstring odf_ref;
    };
    _mediaitems()
    {
 		count_image			= 0;
 		count_media			= 0;
 		count_object		= 0;
 		count_image_object	= 0;
 		count_control		= 0;
	}
	
	size_t count_image;
	size_t count_media;
	size_t count_object;
	size_t count_image_object;
	size_t count_control;

    void add_or_find(const std::wstring & oox_ref, Type type, std::wstring & odf_ref);
    
	void dump_rels(rels & Rels, std::wstring local_path = L"");
    std::vector<item> & items() { return items_; }

private:
    std::vector<item> items_;

};

}
}

