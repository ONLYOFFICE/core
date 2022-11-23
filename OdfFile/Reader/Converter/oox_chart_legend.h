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

#include <iosfwd>
#include <CPOptional.h>
#include <CPScopedPtr.h>

#include "oox_layout.h"
            
namespace cpdoccore {
namespace oox {

class oox_chart_legend : boost::noncopyable
{
public:
	oox_chart_legend(){}
	~oox_chart_legend(){}
 
    void oox_serialize(std::wostream & _Wostream);

	odf_reader::chart::legend content_;  
private:
	
	cpdoccore::oox::oox_layout layout_;	//layout (Layout) §21.2.2.88

	//extLst (Chart Extensibility) §21.2.2.64
	//legendEntry (Legend Entry) §21.2.2.94
	//legendPos (Legend Position) §21.2.2.95
	//overlay (Overlay) §21.2.2.132
	//spPr (Shape Properties) §21.2.2.197
	//txPr (Text Properties) §21.2.2.216
};

}
}
