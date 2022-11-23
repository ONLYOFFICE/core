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

#include "EXTCONNECTION.h"
#include "../Biff12_records/BeginExtConnection.h"
#include "../Biff12_unions/ACUID.h"
#include "../Biff12_unions/ECDBPROPS.h"
#include "../Biff12_unions/ECOLAPPROPS.h"
#include "../Biff12_unions/ECPARAMS.h"
#include "../Biff12_unions/ECWEBPROPS.h"
#include "../Biff12_unions/ECTXTWIZ.h"
#include "../Biff12_unions/FRTEXTCONNECTIONS.h"
#include "../Biff12_records/EndExtConnection.h"

using namespace XLS;

namespace XLSB
{

    EXTCONNECTION::EXTCONNECTION()
    {
    }

    EXTCONNECTION::~EXTCONNECTION()
    {
    }

    BaseObjectPtr EXTCONNECTION::clone()
    {
        return BaseObjectPtr(new EXTCONNECTION(*this));
    }

    // EXTCONNECTION = BrtBeginExtConnection [ACUID]
    //      [(ECDBPROPS [ECOLAPPROPS / ECPARAMS]) / (ECWEBPROPS [ECPARAMS]) / ECTXTWIZ]
    //          FRTEXTCONNECTIONS BrtEndExtConnection
    const bool EXTCONNECTION::loadContent(BinProcessor& proc)
    {
        if (proc.optional<ACUID>())
        {
            //m_ACUID = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginExtConnection>())
        {
            m_BrtBeginExtConnection = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<ACUID>())
        {
            //m_ACUID = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<ECDBPROPS>())
        {
            m_ECDBPROPS = elements_.back();
            elements_.pop_back();

            if (proc.optional<ECOLAPPROPS>())
            {
                m_ECOLAPPROPS = elements_.back();
                elements_.pop_back();
            }
            else if (proc.optional<ECPARAMS>())
            {
                m_ECPARAMS = elements_.back();
                elements_.pop_back();
            }
        }
        else if (proc.optional<ECWEBPROPS>())
        {
            m_ECWEBPROPS = elements_.back();
            elements_.pop_back();

            if (proc.optional<ECPARAMS>())
            {
                m_ECPARAMS = elements_.back();
                elements_.pop_back();
            }
        }
        else if (proc.optional<ECTXTWIZ>())
        {
            m_ECTXTWIZ = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<FRTEXTCONNECTIONS>())
        {
            m_FRTEXTCONNECTIONS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndExtConnection>())
        {
            m_BrtEndExtConnection = elements_.back();
            elements_.pop_back();
        }

        return m_BrtBeginExtConnection && m_BrtEndExtConnection;
    }

} // namespace XLSB

