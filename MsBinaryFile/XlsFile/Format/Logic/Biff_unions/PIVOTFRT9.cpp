/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "PIVOTFRT9.h"
#include "DBQUERYEXT.h"
#include "PIVOTVIEWEX.h"

#include "../Biff_records/QsiSXTag.h"
#include "../Biff_records/SXViewEx9.h"

namespace XLS
{


PIVOTFRT9::PIVOTFRT9()
{
}


PIVOTFRT9::~PIVOTFRT9()
{
}


BaseObjectPtr PIVOTFRT9::clone()
{
	return BaseObjectPtr(new PIVOTFRT9(*this));
}

// PIVOTFRT9 = QsiSXTag [DBQUERYEXT] [PIVOTVIEWEX] SXViewEx9
const bool PIVOTFRT9::loadContent(BinProcessor& proc)
{
	GlobalWorkbookInfoPtr global_info = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<QsiSXTag>())
	{
		return false;
	}
	m_QsiSXTag = elements_.back();
	elements_.pop_back();

	if (proc.optional<DBQUERYEXT>())
	{
		m_DBQUERYEXT = elements_.back();
		elements_.pop_back();
		
		//QsiSXTag* qsi = dynamic_cast<QsiSXTag*>(m_QsiSXTag.get());
		//global_info->mapDBQueryExt.insert(std::make_pair(qsi->stName.value(), m_DBQUERYEXT));

	}
	if (proc.optional<PIVOTVIEWEX>())
	{
		m_PIVOTVIEWEX = elements_.back();
		elements_.pop_back();
	}
	if (proc.mandatory<SXViewEx9>())
	{
		m_SXViewEx9 = elements_.back();
		elements_.pop_back();
	}
	else 
		return false;

	return true;
}

const bool PIVOTFRT9::saveContent(BinProcessor& proc)
{
	if(m_QsiSXTag != nullptr)
		proc.mandatory(*m_QsiSXTag);
	else
		proc.mandatory<QsiSXTag>();
	if(m_DBQUERYEXT != nullptr)
		proc.mandatory(*m_DBQUERYEXT);
	if(m_PIVOTVIEWEX != nullptr)
		proc.mandatory(*m_PIVOTVIEWEX);
	if(m_SXViewEx9 != nullptr)
		proc.mandatory(*m_SXViewEx9);
	else
		proc.mandatory<SXViewEx9>();
	return true;
}

} // namespace XLS

