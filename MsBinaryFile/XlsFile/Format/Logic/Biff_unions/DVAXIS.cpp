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

#include "DVAXIS.h"
#include "AXM.h"
#include "AXS.h"
#include "CRTMLFRT.h"

#include "../Biff_records/Axis.h"
#include "../Biff_records/Begin.h"
#include "../Biff_records/ValueRange.h"
#include "../Biff_records/End.h"
#include "../Biff_records/CatSerRange.h"
#include "../Biff_records/Tick.h"
#include "../Biff_records/FontX.h"
#include "../Biff_records/AxisLine.h"
#include "../Biff_records/LineFormat.h"

namespace XLS
{


DVAXIS::DVAXIS() :	id(0)
{
	m_bSecondary = false;
}


DVAXIS::~DVAXIS()
{
}


BaseObjectPtr DVAXIS::clone()
{
	return BaseObjectPtr(new DVAXIS(*this));
}


const bool DVAXIS::loadContentRead(BinReaderProcessor& proc)
{
	bool ok = loadContent(proc);
	if(ok)
	{
		id = proc.getGlobalWorkbookInfo()->last_Axes_id++;
	}
	return ok;
}

// DVAXIS = Axis Begin [ValueRange] [AXM] AXS [CRTMLFRT] End
const bool DVAXIS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Axis>())
	{
		return false;
	}
	m_Axis = elements_.back();
	elements_.pop_back();

	proc.mandatory<Begin>();				elements_.pop_back();
	
	if (proc.optional<ValueRange>())
	{
		m_ValueRange = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<AXM>())
	{
		m_AXM = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.mandatory<AXS>())
	{
		m_AXS = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<CRTMLFRT>())
	{
		m_CRTMLFRT = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.mandatory<End>())					elements_.pop_back();

	return true;
}

int DVAXIS::serialize(std::wostream & _stream)
{
	ValueRange	*value_range	= dynamic_cast<ValueRange*>	(m_ValueRange.get());
	Axis		*axis			= dynamic_cast<Axis*>		(m_Axis.get());
	CRTMLFRT	*crtMltFrt		= dynamic_cast<CRTMLFRT*>	(m_CRTMLFRT.get());
	
	int axes_type = axis->wType + 1;

	bool bLogarithScale = false;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:axId")
		{
			CP_XML_ATTR(L"val", id);
		}
		CP_XML_NODE(L"c:scaling")
		{
			CP_XML_NODE(L"c:orientation")
			{
				if ((value_range) && (value_range->fReversed)) CP_XML_ATTR(L"val", L"maxMin");
				else CP_XML_ATTR(L"val", L"minMax"); 
			}
			if (value_range)
			{
				if (value_range->fLog == true)
				{
					CP_XML_NODE(L"c:logBase") {  CP_XML_ATTR(L"val", 10); }
					bLogarithScale = true;
				}				
				if (value_range->fAutoMax == false)
				{
					CP_XML_NODE(L"c:max") 
					{ 
						if (bLogarithScale)	CP_XML_ATTR(L"val", pow(10, value_range->numMax.data.value));
						else				CP_XML_ATTR(L"val", value_range->numMax.data.value);
					}
				}			
				if (value_range->fAutoMin == false)
				{
					CP_XML_NODE(L"c:min") 
					{
						if (bLogarithScale)	CP_XML_ATTR(L"val", pow(10, value_range->numMin.data.value));
						else				CP_XML_ATTR(L"val", value_range->numMin.data.value); 
					}
				}
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
//----------------------------------------------------------------------------------------------
		m_AXS->serialize(_stream);

		if (value_range)
		{
			CP_XML_NODE(L"c:crosses")
			{
				if (value_range->fMaxCross == true)	CP_XML_ATTR(L"val", L"max");
				else								CP_XML_ATTR(L"val", L"autoZero");
			}
			CP_XML_NODE(L"c:auto")	{  CP_XML_ATTR(L"val", !value_range->fAutoCross); }
			
			if (value_range->fAutoMajor == false)
			{
				CP_XML_NODE(L"c:majorUnit")
				{
					if (bLogarithScale)	CP_XML_ATTR(L"val", pow(10, value_range->numMajor.data.value));
					else				CP_XML_ATTR(L"val", value_range->numMajor.data.value);
				}
			}
			if (value_range->fAutoMinor == false)
			{
				CP_XML_NODE(L"c:minorUnit")
				{
					if (bLogarithScale)	CP_XML_ATTR(L"val", pow(10, value_range->numMinor.data.value));
					else				CP_XML_ATTR(L"val", value_range->numMinor.data.value);
				}		
			}
		}
	}
	
	return axes_type;
}

} // namespace XLS

