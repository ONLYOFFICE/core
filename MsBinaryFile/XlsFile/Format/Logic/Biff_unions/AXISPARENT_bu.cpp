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

#include "AXISPARENT.h"
#include "AXES.h"
#include "CRT.h"

#include "../Biff_records/AxisParent.h"
#include "../Biff_records/Begin.h"
#include "../Biff_records/Pos.h"
#include "../Biff_records/End.h"


namespace XLS
{


AXISPARENT::AXISPARENT()
{
}


AXISPARENT::~AXISPARENT()
{
}


BaseObjectPtr AXISPARENT::clone()
{
	return BaseObjectPtr(new AXISPARENT(*this));
}


// AXISPARENT = AxisParent Begin Pos [AXES] 1*4CRT End
const bool AXISPARENT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<AxisParent>())
	{
		return false;
	}
	m_AxisParent = elements_.back();
	elements_.pop_back();
	
	if (proc.optional<Begin>())		elements_.pop_back();

	if (proc.optional<Pos>())
	{
		m_Pos = elements_.back();
		elements_.pop_back(); 
	}

	int count = proc.repeated<AXES>(0, 0);
	if (count > 0)
	{
		m_AXES = elements_.back();
		elements_.pop_back(); 
	}
	
	count  = proc.repeated<CRT>(1, 4);

	while(count > 0 && !elements_.empty())
	{
		m_arCRT.insert(m_arCRT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	

	if (proc.optional<End>())	elements_.pop_back();

	return true;
}

void AXISPARENT::concatinate_second (BaseObjectPtr & addit)
{
	AXISPARENT * second = dynamic_cast<AXISPARENT *>(addit.get());

	if (second == NULL) return;

	for (size_t i = 0; i < second->m_arCRT.size(); i++)
	{
		CRT* crt = dynamic_cast<CRT*>(second->m_arCRT[i].get());
		crt->m_indAXISPARENT = 1;

		m_arCRT.push_back(second->m_arCRT[i]);
	}

}


} // namespace XLS

