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

#include "SXFMLA_bu.h"
#include "../Biff_records/SxFmla.h"
#include "../Biff_records/SxName.h"
#include "../Biff_records/SXPair.h"

namespace XLS
{

//  (SxName *SXPair)
class Parenthesis_SXFMLA: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXFMLA)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXFMLA(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SxName>())
		{
			return false;
		}	
		
		int count = proc.repeated<SXPair>(0, 0);

		return true;
	};
};

SXFMLA::SXFMLA()
{
}


SXFMLA::~SXFMLA()
{
}


BaseObjectPtr SXFMLA::clone()
{
	return BaseObjectPtr(new SXFMLA(*this));
}

// SXFMLA = SxFmla *(SxName *SXPair)
const bool SXFMLA::loadContent(BinProcessor& proc)
{
	global_info = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<SxFmla>())
	{
		return false;
	}
	m_SxFmla = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<Parenthesis_SXFMLA>(0, 0);

	while(!elements_.empty())
	{
		if (dynamic_cast<SxName*>(elements_.front().get()))
		{
			_sx_name sx_name;
			sx_name.name = elements_.front(); elements_.pop_front();

			m_arPivotSxNames.push_back(sx_name);
		}
		else
		{
			while(!elements_.empty())
			{
				if (dynamic_cast<SxName*>(elements_.front().get()))
				{
					break;
				}
				else
				{
					m_arPivotSxNames.back().pair.push_back(elements_.front());
					elements_.pop_front();
				}
			}
		}
	}

	return true;
}

void SXFMLA::serialize_attr(CP_ATTR_NODE)
{
	if (!m_SxFmla) return;

	SxFmla* sx_fmla	= dynamic_cast<SxFmla*>(m_SxFmla.get()); 
	
	global_info->arPivotSxNames = m_arPivotSxNames;

	std::wstring fmla = sx_fmla->fmla.getAssembledFormula();
	CP_XML_ATTR(L"formula", fmla);
}


} // namespace XLS

