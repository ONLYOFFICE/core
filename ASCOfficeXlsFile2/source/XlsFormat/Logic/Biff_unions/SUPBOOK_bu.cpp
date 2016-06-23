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

#include "SUPBOOK.h"
#include <Logic/Biff_records/SupBook.h>
#include <Logic/Biff_records/ExternName.h>
#include <Logic/Biff_records/ExternSheet.h>
#include <Logic/Biff_records/Continue.h>
#include <Logic/Biff_records/XCT.h>
#include <Logic/Biff_records/CRN.h>

namespace XLS
{


SUPBOOK::SUPBOOK()
{
}


SUPBOOK::~SUPBOOK()
{
}

class Parenthesis_SUPBOOK_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SUPBOOK_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SUPBOOK_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<XCT>())
		{
			return false;
		}
		proc.repeated<CRN>(0, 0);
		return true;
	};
};



BaseObjectPtr SUPBOOK::clone()
{
	return BaseObjectPtr(new SUPBOOK(*this));
}


// SUPBOOK = SupBook [*ExternName *(XCT *CRN)] [ExternSheet] *Continue
const bool SUPBOOK::loadContent(BinProcessor& proc)
{
	SupBook supbook;
	if(!proc.mandatory(supbook))
	{
		return false;
	}
	m_SupBook = elements_.back();
	elements_.pop_back();

	if (supbook.cch != 0x0401  && supbook.cch != 0x3A01 )
	{
		proc.getGlobalWorkbookInfo()->arExternalNames.clear();
	}

	while(true)
	{
		ExternName extern_name(supbook.getSupportingLinkType());
		if(!proc.optional(extern_name))
		{
			break;
		}
	}

	int count = proc.repeated<Parenthesis_SUPBOOK_1>(0, 0);
	
	if (proc.optional<ExternSheet>())
	{
		m_ExternSheet = elements_.back();
		elements_.pop_back();
	}
	//proc.repeated<Continue>(0, 0);
	return true;
}

} // namespace XLS

