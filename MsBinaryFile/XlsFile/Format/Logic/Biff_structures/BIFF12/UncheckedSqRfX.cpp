/*
 * (c) Copyright Ascensio System SIA 2010-2021
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "UncheckedSqRfX.h"
#include <boost/algorithm/string.hpp>

using namespace XLS;

namespace XLSB
{

    UncheckedSqRfX::UncheckedSqRfX()
    {
    }

    UncheckedSqRfX::UncheckedSqRfX(XLS::CFRecord& record)
    {
        load(record);
    }

    UncheckedSqRfX::~UncheckedSqRfX()
    {
    }

    BiffStructurePtr UncheckedSqRfX::clone()
    {
        return BiffStructurePtr(new UncheckedSqRfX(*this));
    }

    void UncheckedSqRfX::load(XLS::CFRecord& record)
    {
        record >> crfx;
        UncheckedRfX rfx;
        for(size_t i = 0; i < crfx; i++)
        {
            record >> rfx;
            rgrfx.push_back(rfx);
            strValue += std::wstring (rfx.toString(false).c_str()) + ((i == crfx - 1) ? L"" : L" ");
        }
    }

	void UncheckedSqRfX::save(XLS::CFRecord& record)
	{
		std::vector<std::wstring> results;

		boost::algorithm::split(results, strValue, boost::is_any_of(L" "));
		crfx = results.size();

		record << crfx;

		for (auto& item : results)
		{
			UncheckedRfX rfx(item);
			record << rfx;
		}
	}

    const CellRef UncheckedSqRfX::getLocationFirstCell() const
    {
        std::vector<CellRangeRef> refs;

        AUX::str2refs(strValue, refs);

        if(!refs.size())
        {
            return CellRef();
        }
        else
        {
            return refs[0].getTopLeftCell();
        }
    }

} // namespace XLSB

