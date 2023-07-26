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

#include "SLICERCACHE.h"
#include "../Biff12_records/BeginSlicerCache.h"
#include "../Biff12_unions/ACUID.h"
#include "../Biff12_records/BeginSlicerCacheDef.h"
#include "../Biff12_records/SlicerCachePivotTables.h"
#include "../Biff12_unions/SLICERCACHEOLAPIMPL.h"
#include "../Biff12_unions/SLICERCACHENATIVEITEMS.h"
#include "../Biff12_unions/FRTSLICERCACHE.h"
#include "../Biff12_records/EndSlicerCacheDef.h"
#include "../Biff12_records/EndSlicerCache.h"

using namespace XLS;

namespace XLSB
{

    SLICERCACHE::SLICERCACHE()
    {
    }

    SLICERCACHE::~SLICERCACHE()
    {
    }

    BaseObjectPtr SLICERCACHE::clone()
    {
        return BaseObjectPtr(new SLICERCACHE(*this));
    }

    //SLICERCACHE = BrtBeginSlicerCache [ACUID] BrtBeginSlicerCacheDef [BrtSlicerCachePivotTables]
    //  (SLICERCACHEOLAPIMPL / SLICERCACHENATIVEITEMS) FRTSLICERCACHE BrtEndSlicerCacheDef
    //  BrtEndSlicerCache
    const bool SLICERCACHE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSlicerCache>())
        {
            m_bBrtBeginSlicerCache = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginSlicerCache = false;

        if (proc.optional<ACUID>())
        {
            //m_ACUID = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginSlicerCacheDef>())
        {
            m_BrtBeginSlicerCacheDef = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SlicerCachePivotTables>())
        {
            m_BrtSlicerCachePivotTables = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SLICERCACHEOLAPIMPL>())
        {
            m_slicerCacheData = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SLICERCACHENATIVEITEMS>())
        {
            m_slicerCacheData = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<FRTSLICERCACHE>())
        {
            m_FRTSLICERCACHE = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndSlicerCacheDef>())
        {
            m_bBrtEndSlicerCacheDef = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSlicerCacheDef = false;

        if (proc.optional<EndSlicerCache>())
        {
            m_bBrtEndSlicerCache = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSlicerCache = false;

        return m_bBrtBeginSlicerCache && m_BrtBeginSlicerCacheDef && m_bBrtEndSlicerCacheDef && m_bBrtEndSlicerCache;
    }

	const bool SLICERCACHE::saveContent(BinProcessor& proc)
	{
		proc.mandatory<BeginSlicerCache>();

		if (m_BrtBeginSlicerCacheDef != nullptr)
			proc.mandatory(*m_BrtBeginSlicerCacheDef);

		if (m_BrtSlicerCachePivotTables != nullptr)
			proc.mandatory(*m_BrtSlicerCachePivotTables);

		if (m_slicerCacheData != nullptr)
			proc.mandatory(*m_slicerCacheData);

		if (m_FRTSLICERCACHE != nullptr)
			proc.mandatory(*m_FRTSLICERCACHE);

		proc.mandatory<EndSlicerCacheDef>();

		proc.mandatory<EndSlicerCache>();

		return true;
	}

} // namespace XLSB

