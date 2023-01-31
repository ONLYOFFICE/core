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

#include "SXADDLQSI.h"
#include "../Biff_records/SXAddl.h"

namespace XLS
{

SXADDLQSI::SXADDLQSI() : current( &content)
{
	_sxAddl elm(NULL, 0);
	current->push_back(elm);
}

SXADDLQSI::~SXADDLQSI()
{
}


BaseObjectPtr SXADDLQSI::clone()
{
	return BaseObjectPtr(new SXADDLQSI(*this));
}

// SXADDLQSI = SXAddl_SXCQsi_SXDId SXADDLDBQUERY *UNKNOWNFRT SXAddl_SXCQsi_SXDEnd
const bool SXADDLQSI::loadContent(BinProcessor& proc)
{
	bool result = false;
	int level = 0;
	
	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();	

		if (type != rt_SXAddl) break;
	
		proc.optional<SXAddl>();

		SXAddl* addl = dynamic_cast<SXAddl*>(elements_.back().get());				
		if (!addl) continue;
		
		if (result && addl->bStartElement)
		{
			level++;
			_sxAddl elm(current, level); 

			current = &current->back().levels;
			
			current->push_back(elm);
			
			elements_.pop_back();
			continue;
		}
		
		result = true;
		
		if (addl->bEndElement)
		{ 
			elements_.pop_back();
			
			if (level == 0)
				break;
			else level--;

			current = current->back().prev;
			continue;
		}
		if (level == 0)
		{
			SXAddl_SXCQsi_SXDId* p0 = dynamic_cast<SXAddl_SXCQsi_SXDId*>(addl->content.get());
			if (p0)
			{
				m_SXAddl_SXCQsi_SXDId = addl->content;
			}
		}

		current->back().elements.push_back(elements_.back());
		elements_.pop_back();
	}

	current = NULL;
	return result;
}

} // namespace XLS

