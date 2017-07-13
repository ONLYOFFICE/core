/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "SXTBL.h"
#include <Logic/Biff_records/SXTbl.h>
#include <Logic/Biff_unions/DREF.h>
#include <Logic/Biff_records/SxTbpg.h>
#include <Logic/Biff_records/SXTBRGIITM.h>
#include <Logic/Biff_records/SXString.h>

namespace XLS
{


SXTBL::SXTBL()
{
}


SXTBL::~SXTBL()
{
}

class Parenthesis_SXTBL_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXTBL_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXTBL_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXTBRGIITM>())
		{
			return false;
		}

		proc.repeated<SXString>(0, 0);
		return true;
	};
};


BaseObjectPtr SXTBL::clone()
{
	return BaseObjectPtr(new SXTBL(*this));
}


// SXTBL = SXTbl *DREF *SxTbpg *(SXTBRGIITM *SXString)
const bool SXTBL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXTbl>())
	{
		return false;
	}

	m_SXTbl = elements_.back();
	elements_.pop_back();

	int count =0;
	
	count  = proc.repeated<DREF>(0, 0);
	while(!elements_.empty())
	{
		m_arDREF.push_back(elements_.front()); 
		elements_.pop_front();
	}

	count  = proc.repeated<SxTbpg>(0, 0);
	while(!elements_.empty())
	{
		m_arSxTbpg.push_back(elements_.front()); 
		elements_.pop_front();
	}	

	count = proc.repeated<Parenthesis_SXTBL_1>(0, 0);
	while(!elements_.empty())
	{
		SXTBRGIITM* item = dynamic_cast<SXTBRGIITM*>(elements_.front().get());

		if (item)
		{
			_sxtbl_item it;
			it.item = elements_.front();
			m_arSXTBRGIITM.push_back(it); 
		}
		else
		{
			m_arSXTBRGIITM.back().strings.push_back(elements_.front());
		}
		elements_.pop_front();
	}	
	return true;
}
int SXTBL::serialize(std::wostream & stream)
{
	if (!m_SXTbl) return 0;


	return 0;
}
} // namespace XLS

