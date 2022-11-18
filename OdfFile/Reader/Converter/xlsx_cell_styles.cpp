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
#include <vector>

#include <xml/simple_xml_writer.h>

#include "xlsx_cell_styles.h"
#include "xlsx_cell_style.h"

namespace cpdoccore {
namespace oox {

class xlsx_cell_styles::Impl
{
public:
    std::vector<xlsx_cell_style> cell_styles_;    
};

xlsx_cell_styles::xlsx_cell_styles() : impl_(new xlsx_cell_styles::Impl())
{
    xlsx_cell_style default_style;
    
	default_style.xfId		= 0;
    default_style.builtinId = 0;
    default_style.name		= L"Default";

    impl_->cell_styles_.push_back(default_style);
}

xlsx_cell_styles::~xlsx_cell_styles()
{}

void xlsx_cell_styles::serialize(std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"cellStyles")
        {
            CP_XML_ATTR(L"count", impl_->cell_styles_.size());

   			for (size_t i = 0; i < impl_->cell_styles_.size(); i++)
			{
                oox::xlsx_serialize(CP_XML_STREAM(), impl_->cell_styles_[i]);                    
            }
        }
    }
}


}
}
