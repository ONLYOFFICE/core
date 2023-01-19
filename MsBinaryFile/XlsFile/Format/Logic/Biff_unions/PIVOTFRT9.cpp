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

} // namespace XLS

