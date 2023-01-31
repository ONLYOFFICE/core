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

#include "SERIESAXIS.h"
#include "AXS.h"
#include "CRTMLFRT.h"
#include "../Biff_records/Axis.h"
#include "../Biff_records/Begin.h"
#include "../Biff_records/CatSerRange.h"
#include "../Biff_records/End.h"

namespace XLS
{


SERIESAXIS::SERIESAXIS()
{
	m_bSecondary = false;
}


SERIESAXIS::~SERIESAXIS()
{
}


BaseObjectPtr SERIESAXIS::clone()
{
	return BaseObjectPtr(new SERIESAXIS(*this));
}


const bool SERIESAXIS::loadContentRead(BinReaderProcessor& proc)
{
	bool ok = loadContent(proc);
	if(ok)
	{
		id = proc.getGlobalWorkbookInfo()->last_Axes_id++;
	}
	return ok;
}

// SERIESAXIS = Axis Begin [CatSerRange] AXS [CRTMLFRT] End
const bool SERIESAXIS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Axis>())
	{
		return false;
	}
	m_Axis = elements_.back();
	elements_.pop_back();

	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE) break;
		if (type == rt_End) 
		{
			if (proc.mandatory<End>())
				elements_.pop_back();
			break;
		}

		switch(type)
		{
			case rt_Begin:
			{
				if (proc.optional<Begin>())
					elements_.pop_back();
			}break;
			case rt_CatSerRange:
			{
				if (proc.optional<CatSerRange>())
				{
					m_CatSerRange = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_IFmtRecord:
			case rt_Tick:
			case rt_FontX:
			case rt_AxisLine:
			{
				if (proc.mandatory<AXS>())
				{
					m_AXS = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_CrtMlFrt:		
			{
				proc.optional<CRTMLFRT>();		
			}break;
			default:
			{
				proc.SkipRecord();	
			}break;
		}
	}
	return true;
}
int SERIESAXIS::serialize(std::wostream & _stream)
{
	CatSerRange * cat_ser_range = dynamic_cast<CatSerRange*>(m_CatSerRange.get());
	Axis		* axis			= dynamic_cast<Axis*>		(m_Axis.get());

	int axes_type = axis->wType + 1;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:axId")
		{
			CP_XML_ATTR(L"val", id);
		}
		
		CP_XML_NODE(L"c:scaling")
		{
			if (cat_ser_range->fReversed)
			{
				CP_XML_NODE(L"c:orientation"){  CP_XML_ATTR(L"val", L"maxMin"); }
			}else
			{
				CP_XML_NODE(L"c:orientation"){  CP_XML_ATTR(L"val", L"minMax"); }
			}
		}
		CP_XML_NODE(L"c:delete"){  CP_XML_ATTR(L"val", 0); }
		
//-------------------------------------------------------------------------------
		CP_XML_NODE(L"c:axPos")
		{
			if (axes_type == 1)
			{
				if (m_bSecondary)	CP_XML_ATTR(L"val", L"t");
				else				CP_XML_ATTR(L"val", L"b");
			}
			else if (axes_type == 2)
			{
				if (m_bSecondary)	CP_XML_ATTR(L"val", L"r");
				else				CP_XML_ATTR(L"val", L"l");
			}
			else
			{
				if (m_bSecondary)	CP_XML_ATTR(L"val", L"t");
				else				CP_XML_ATTR(L"val", L"b");
			}
		}
//-----------------------------------------------------------------------------------
		m_AXS->serialize(_stream);

		if (cat_ser_range)
		{
			CP_XML_NODE(L"c:crosses")
			{
				if (cat_ser_range->fMaxCross == true)	CP_XML_ATTR(L"val", L"max");
				else									CP_XML_ATTR(L"val", L"autoZero");
			}	
		}
	}
	
	return 	axes_type;
}

} // namespace XLS

