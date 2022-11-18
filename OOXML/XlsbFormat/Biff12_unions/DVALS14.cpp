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

#include "DVALS14.h"
#include "../Biff12_records/BeginDVals14.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_unions/ACDVALLIST.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/FRTEnd.h"
#include "../Biff12_records/EndDVals14.h"

using namespace XLS;

namespace XLSB
{

    DVALS14::DVALS14()
    {
    }

    DVALS14::~DVALS14()
    {
    }

    class Parenthesis_DVALS14: public XLS::CompositeObject
        {
            BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DVALS14)
        public:
            BaseObjectPtr clone()
            {
                return BaseObjectPtr(new Parenthesis_DVALS14(*this));
            }

            const bool loadContent(XLS::BinProcessor& proc)
            {               
                if (proc.optional<ACDVALLIST>())
                {
                    //m_ACDVALLIST = elements_.back();
                    elements_.pop_back();
                }
                if(proc.optional<DVal14>())
                {
                    m_BrtDVal14 = elements_.back();
                    elements_.pop_back();
                }
                else return false;

                return true;
            };

            //BaseObjectPtr   m_ACDVALLIST;
            BaseObjectPtr	m_BrtDVal14;
        };

    BaseObjectPtr DVALS14::clone()
    {
        return BaseObjectPtr(new DVALS14(*this));
    }

    // DVALS14 = BrtFRTBegin BrtBeginDVals14 1*65534([ACDVALLIST] BrtDVal14) BrtEndDVals14 BrtFRTEnd
    const bool DVALS14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginDVals14>())
        {
            m_BrtBeginDVals14 = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<Parenthesis_DVALS14>(0, 65534);
        while(count > 0)
        {
            m_arBrtDVal14.insert(m_arBrtDVal14.begin(), static_cast<Parenthesis_DVALS14*>(elements_.back().get())->m_BrtDVal14);
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndDVals14>())
        {
            m_BrtEndDVals14 = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<FRTEnd>())
        {
            m_BrtFRTEnd = elements_.back();
            elements_.pop_back();
        }

        return m_BrtBeginDVals14 && m_BrtEndDVals14 && m_BrtFRTEnd;
    }

} // namespace XLSB

