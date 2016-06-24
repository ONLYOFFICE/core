/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "CHARTFOMATS.h"
#include <Logic/Biff_records/Chart.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Scl.h>
#include <Logic/Biff_records/PlotGrowth.h>
#include <Logic/Biff_records/ShtProps.h>
#include <Logic/Biff_records/AxesUsed.h>
#include <Logic/Biff_records/CrtLayout12A.h>
#include <Logic/Biff_records/DataLabExt.h>
#include <Logic/Biff_records/StartObject.h>
#include <Logic/Biff_records/EndObject.h>
#include <Logic/Biff_records/End.h>
#include <Logic/Biff_records/ObjectLink.h>

#include <Logic/Biff_unions/FONTLIST.h>
#include <Logic/Biff_unions/FRAME.h>
#include <Logic/Biff_unions/SERIESFORMAT.h>
#include <Logic/Biff_unions/SS.h>
#include <Logic/Biff_unions/DFTTEXT.h>
#include <Logic/Biff_unions/AXISPARENT.h>
#include <Logic/Biff_unions/DAT.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_unions/TEXTPROPS.h>

namespace XLS
{


CHARTFORMATS::CHARTFORMATS()
{
}


CHARTFORMATS::~CHARTFORMATS()
{
}


//  ([DataLabExt StartObject] ATTACHEDLABEL [EndObject])
class Parenthesis_CHARTFORMATS_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_CHARTFORMATS_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_CHARTFORMATS_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(proc.optional<DataLabExt>())
		{
			proc.mandatory<StartObject>();
		}
		if(!proc.mandatory<ATTACHEDLABEL>())
		{
			return false;
		}
		proc.optional<EndObject>();
		return true;
	};
};


BaseObjectPtr CHARTFORMATS::clone()
{
	return BaseObjectPtr(new CHARTFORMATS(*this));
}


/*
CHARTFOMATS = Chart Begin *2FONTLIST Scl PlotGrowth [FRAME] *SERIESFORMAT *SS ShtProps *2DFTTEXT 
				AxesUsed 1*2AXISPARENT [CrtLayout12A] [DAT] *ATTACHEDLABEL [CRTMLFRT] 
				*([DataLabExt StartObject] ATTACHEDLABEL [EndObject]) [TEXTPROPS] *2CRTMLFRT End
*/
const bool CHARTFORMATS::loadContent(BinProcessor& proc)
{
	int count = 0;
	if(!proc.mandatory<Chart>())
	{
		return false;
	}
	m_ChartRect = elements_.back();				elements_.pop_back();

	proc.mandatory<Begin>();					elements_.pop_back();
	
	count = proc.repeated<FONTLIST>(0, 2);
	while(count > 0)
	{
		m_arFONTLIST.insert(m_arFONTLIST.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	

	if (proc.optional<Scl>())
	{
		m_Scl = elements_.back();				elements_.pop_back();

		if (proc.optional<PlotGrowth>())
		{
			m_PlotGrowth = elements_.back();	elements_.pop_back();
		}
	}

	if (proc.optional<FRAME>())
	{
		m_FRAME = elements_.back();				elements_.pop_back();
	}

	count = proc.repeated<SERIESFORMAT>(0, 0);
	while(count > 0)
	{
		m_arSERIESFORMAT.insert(m_arSERIESFORMAT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	
	count = proc.repeated<SS>(0, 0);
	while(count > 0)
	{
		m_arSS.insert(m_arSS.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	if (proc.mandatory<ShtProps>())
	{
		m_ShtProps = elements_.back();			elements_.pop_back();
	}
	
	count = proc.repeated<DFTTEXT>(0, 2);
	while(count > 0)
	{
		m_arDFTTEXT.insert(m_arDFTTEXT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	
	if (proc.mandatory<AxesUsed>())
	{
		m_AxesUsed = elements_.back();			elements_.pop_back(); 
	}
	
	count = proc.repeated<AXISPARENT>(1, 2);
	if (elements_.size() > 0)
	{
		m_arAXISPARENT.push_back(elements_.front()); 
		elements_.pop_front(); 
	}
	count--;
	if (count > 0)
	{
		AXISPARENT * ax_first = dynamic_cast<AXISPARENT *>(m_arAXISPARENT[0].get());
		if (ax_first) ax_first->concatinate_second(elements_.front()); //for crt recalc

		m_arAXISPARENT.push_back(elements_.front()); count--;
		elements_.pop_front();
		
	}
	
	if (proc.optional<CrtLayout12A>())
	{
		m_CrtLayout12A = elements_.back();		elements_.pop_back();
	}
	if (proc.optional<DAT>())
	{
		m_DAT = elements_.back();				elements_.pop_back(); 
	}
	
	count = proc.repeated<ATTACHEDLABEL>(0, 0);
	while(count > 0)
	{
		m_arATTACHEDLABEL.insert(m_arATTACHEDLABEL.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	if (proc.optional<CRTMLFRT>())
	{
		elements_.pop_back();
	}
	
	count = proc.repeated<Parenthesis_CHARTFORMATS_1>(0, 0);

	int count1 = elements_.size();
	while(count1 > 0)
	{
		_chart_format cf;

		if ("DataLabExt" == elements_.front()->getClassName())
		{//необязат
			cf.dataLabExt = elements_.front();
			elements_.pop_front();
			count--;
		}
		if (cf.dataLabExt)
		{//start
			elements_.pop_front();
			count--;
		}
		if ("ATTACHEDLABEL" == elements_.front()->getClassName())
		{//обязат
			cf.attachedLABEL = elements_.front();
			elements_.pop_front();
			count--;
		
			m_arChartFormats.push_back(cf);
		}
		if (cf.dataLabExt)
		{//end
			elements_.pop_front();
			count--;
		}
	}
	if (proc.optional<TEXTPROPS>())
	{
		m_TEXTPROPS = elements_.back();		elements_.pop_back(); 
	}
	count = proc.repeated<CRTMLFRT>(0, 2);

	if(proc.mandatory<End>())				elements_.pop_back();

	return true;
}
BaseObjectPtr CHARTFORMATS::find_label( _UINT16 link_id, unsigned short ex)
{
	for (int i = 0 ; i < m_arATTACHEDLABEL.size(); i++)
	{
		ATTACHEDLABEL	* label		= dynamic_cast<ATTACHEDLABEL *>	(m_arATTACHEDLABEL[i].get());
		ObjectLink		* obj_link	= dynamic_cast<ObjectLink *>	(label->m_ObjectLink.get());
		if (obj_link == NULL) continue;
		
		if (obj_link->wLinkObj == link_id)
		{
			if (link_id !=4 ) 
				return m_arATTACHEDLABEL[i];
			else if (obj_link->wLinkVar1 == ex && obj_link->wLinkVar2 == (unsigned short)0xffff)
				return m_arATTACHEDLABEL[i];
		}
		
	}
	ATTACHEDLABEL * nul = NULL;
	return BaseObjectPtr(nul);
}

std::vector<std::pair<int, BaseObjectPtr>> CHARTFORMATS::find_labels( _UINT16 link_id, unsigned short ex)
{
	std::vector<std::pair<int, BaseObjectPtr>> result;

	for (int i = 0 ; i < m_arATTACHEDLABEL.size(); i++)
	{
		ATTACHEDLABEL	* label		= dynamic_cast<ATTACHEDLABEL *>	(m_arATTACHEDLABEL[i].get());
		ObjectLink		* obj_link	= dynamic_cast<ObjectLink *>	(label->m_ObjectLink.get());
		if (obj_link == NULL) continue;
		
		if (obj_link->wLinkObj == link_id && obj_link->wLinkVar1 == ex && obj_link->wLinkVar2 != (unsigned short)0xffff)
		{
			result.push_back(std::pair<int, BaseObjectPtr>(obj_link->wLinkVar2, m_arATTACHEDLABEL[i]));
		}
		
	}

	return result;
}

BaseObjectPtr CHARTFORMATS::find_default_text( _UINT16 link_id)
{
	ATTACHEDLABEL * nul = NULL;
	return BaseObjectPtr(nul);
}
} // namespace XLS

