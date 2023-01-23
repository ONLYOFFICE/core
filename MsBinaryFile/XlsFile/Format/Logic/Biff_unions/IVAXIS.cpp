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

#include "IVAXIS.h"
#include "AXS.h"
#include "CRTMLFRT.h"
#include "../Biff_records/Axis.h"
#include "../Biff_records/Begin.h"
#include "../Biff_records/CatSerRange.h"
#include "../Biff_records/AxcExt.h"
#include "../Biff_records/CatLab.h"
#include "../Biff_records/End.h"

namespace XLS
{


IVAXIS::IVAXIS()
:	id(0), bDataAxis(false)
{
	m_bSecondary = false;
}


IVAXIS::~IVAXIS()
{
}


BaseObjectPtr IVAXIS::clone()
{
	return BaseObjectPtr(new IVAXIS(*this));
}


const bool IVAXIS::loadContentRead(BinReaderProcessor& proc)
{
	bool ok = loadContent(proc);
	if(ok)
	{
		id = proc.getGlobalWorkbookInfo()->last_Axes_id++;
	}
	return ok;
}

// IVAXIS = Axis Begin [CatSerRange] AxcExt [CatLab] AXS [CRTMLFRT] End
const bool IVAXIS::loadContent(BinProcessor& proc)
{
	// This is the way to distinguish IVAXIS and DVAXIS
	if(!proc.checkNextRecord(rt_AxcExt, 4) && !proc.checkNextRecord(rt_CatSerRange, 4))
	{
		return false;
	}
	if(!proc.mandatory<Axis>())
	{
		return false;
	}
	m_Axis = elements_.back();
	elements_.pop_back();

	proc.mandatory<Begin>();					elements_.pop_back();
	
	if (proc.optional<CatSerRange>())
	{
		m_CatSerRange = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<AxcExt>()) // 4-th place
	{
		m_AxcExt = elements_.back();
		elements_.pop_back();
		
		AxcExt *ext = dynamic_cast<AxcExt *>(m_AxcExt.get());
		bDataAxis = ext->fDateAxis;
	}

	if (proc.optional<CatLab>())
	{
		m_CatLab = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<AXS>())
	{
		m_AXS = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<CRTMLFRT>())
	{
		m_CRTMLFRT = elements_.back();
		elements_.pop_back();
	}
	
	// fix
	proc.optional<CRTMLFRT>(); ///????

	if (proc.mandatory<End>())				elements_.pop_back();

	return true;
}

int IVAXIS::serialize(std::wostream & _stream)
{
	CatSerRange * cat_ser_range = dynamic_cast<CatSerRange*>(m_CatSerRange.get());
	Axis		* axis			= dynamic_cast<Axis*>		(m_Axis.get());
	AxcExt		* axcExt		= dynamic_cast<AxcExt*>		(m_AxcExt.get());

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

		if (m_AxcExt)
			m_AxcExt->serialize(_stream);
	
		if (cat_ser_range)
		{
			CP_XML_NODE(L"c:crosses")
			{
				if (cat_ser_range->fMaxCross == true)	CP_XML_ATTR(L"val", L"max");
				else									CP_XML_ATTR(L"val", L"autoZero");
			}
		}
		if (m_CatLab)
		{
			CatLab *label = dynamic_cast<CatLab*>(m_CatLab.get());

			CP_XML_NODE(L"c:lblAlgn")
			{
				switch(label->at)
				{
				case 0x0001:	CP_XML_ATTR(L"val", L"l");	break;
				case 0x0003:	CP_XML_ATTR(L"val", L"r");	break;
				case 0x0002:
				default:		CP_XML_ATTR(L"val", L"ctr"); break;
				}
			}
			CP_XML_NODE(L"c:lblOffset")
			{
				CP_XML_ATTR(L"val", label->wOffset);
			}
		}
	}
	
	return 	axes_type;
}
} // namespace XLS

