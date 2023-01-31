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

#include "QUERYTABLE.h"
#include "DBQUERY.h"
#include "DBQUERYEXT.h"
#include "SXADDLQSI.h"
#include "QSIR.h"
#include "SORTDATA12.h"

#include "../Biff_records/Qsi.h"
#include "../Biff_records/QsiSXTag.h"
#include "../Biff_records/DbOrParamQry.h"
#include "../Biff_records/SXString.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

QUERYTABLE::QUERYTABLE()
{
}

QUERYTABLE::~QUERYTABLE()
{
}

BaseObjectPtr QUERYTABLE::clone()
{
	return BaseObjectPtr(new QUERYTABLE(*this));
}

// QUERYTABLE = Qsi DBQUERY QsiSXTag DBQUERYEXT [SXADDLQSI] [QSIR] [SORTDATA12]

const bool QUERYTABLE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Qsi>()) return false;

	global_info = proc.getGlobalWorkbookInfo();

	m_Qsi = elements_.back(); elements_.pop_back();

	if (proc.mandatory<DBQUERY>())
	{
		m_DBQUERY = elements_.back(); elements_.pop_back();
	}
	if (proc.mandatory<QsiSXTag>())
	{
		m_QsiSXTag = elements_.back(); elements_.pop_back();
	}
	if (proc.mandatory<DBQUERYEXT>())
	{
		m_DBQUERYEXT = elements_.back(); elements_.pop_back();

		//Qsi* qsi = dynamic_cast<Qsi*>(m_Qsi.get());
		//global_info->mapDBQueryExt.insert(std::make_pair(qsi->rgchName.value(), m_DBQUERYEXT));
	}
	if (proc.optional<SXADDLQSI>())
	{
		m_SXADDLQSI = elements_.back();	elements_.pop_back();
	}
	if (proc.optional<QSIR>())
	{
		m_QSIR = elements_.back();	elements_.pop_back();
	}
	if (proc.optional<SORTDATA12>())
	{
		m_SORTDATA12 = elements_.back(); elements_.pop_back();
	}

	return true;
}
int QUERYTABLE::serialize(std::wostream & strm)
{
	Qsi *info = dynamic_cast<Qsi*>(m_Qsi.get());
	if (!info) return 0;	

	DBQUERY *query = dynamic_cast<DBQUERY*>(m_DBQUERY.get());
	if (!query) return -1;

    std::wstring name = info->name;

	query->m_DBQUERYEXT = m_DBQUERYEXT;
	query->m_Qsi		= m_Qsi;

	int connectionId = query->serialize_connection(name);
	
	if (connectionId < 1) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"queryTable")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");	
			CP_XML_ATTR(L"connectionId", connectionId);	
			CP_XML_ATTR(L"name", name);

			if (!info->fAutoFormat)
			{
				CP_XML_ATTR(L"autoFormatId", info->itblAutoFmt); 
			}
			CP_XML_ATTR(L"applyNumberFormats",		info->fibitAtrNum);
			CP_XML_ATTR(L"applyBorderFormats",		info->fibitAtrBdr);
			CP_XML_ATTR(L"applyFontFormats",		info->fibitAtrFnt);
			CP_XML_ATTR(L"applyPatternFormats",		info->fibitAtrPat);
			CP_XML_ATTR(L"applyAlignmentFormats",	info->fibitAtrAlc);
			CP_XML_ATTR(L"applyWidthHeightFormats", 0);
		}
	}

	return 0;
}
} // namespace XLS

