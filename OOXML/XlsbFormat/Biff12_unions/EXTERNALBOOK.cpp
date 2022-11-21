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

#include "EXTERNALBOOK.h"
#include "../Biff12_records/SupTabs.h"
#include "../Biff12_unions/EXTERNNAME.h"
#include "../Biff12_unions/EXTERNTABLE.h"

using namespace XLS;

namespace XLSB
{

    EXTERNALBOOK::EXTERNALBOOK(ExternalReferenceType type) : sbt(type)
    {
    }

    EXTERNALBOOK::~EXTERNALBOOK()
    {
    }

    BaseObjectPtr EXTERNALBOOK::clone()
    {
        return BaseObjectPtr(new EXTERNALBOOK(*this));
    }

    //EXTERNALBOOK = BrtSupTabs *EXTERNNAME *EXTERNTABLE
    const bool EXTERNALBOOK::loadContent(BinProcessor& proc)
    {
        if (proc.optional<SupTabs>())
        {
            m_BrtSupTabs = elements_.back();
            elements_.pop_back();
        }

        EXTERNNAME externname(sbt);
        int countEXTERNNAME = proc.repeated(externname, 0, 0);

        while(countEXTERNNAME > 0)
        {
            m_arEXTERNNAME.insert(m_arEXTERNNAME.begin(), elements_.back());
            elements_.pop_back();
            countEXTERNNAME--;
        }

        int countEXTERNTABLE = proc.repeated<EXTERNTABLE>(0, 0);

        while(countEXTERNTABLE > 0)
        {
            m_arEXTERNTABLE.insert(m_arEXTERNTABLE.begin(), elements_.back());
            elements_.pop_back();
            countEXTERNTABLE--;
        }

        return m_BrtSupTabs != nullptr;
    }

} // namespace XLSB

