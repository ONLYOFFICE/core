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

#include "NameParsedFormula.h"
#include "StringPtgParser.h"


namespace XLS
{

ParsedFormula::ParsedFormula(const CellRef& cell_base_ref)
:	cce(0),
	cce_is_set(false),
	rgce(cell_base_ref)
{
}

void ParsedFormula::set_base_ref(const CellRef& cell_base_ref)
{
	rgce.set_base_ref(cell_base_ref);
}
void ParsedFormula::setCCE(const size_t cce_val)
{
	rgce.setCCE(cce_val);
}

const size_t ParsedFormula::getCCE() const
{
	return rgce.getCCE();
}

const bool ParsedFormula::IsVolatile() const
{
	const PtgVector& ptgs = rgce.getPtgs();
	for (PtgVectorIterator it = ptgs.begin(), itEnd = ptgs.end(); it != itEnd; ++it)
	{
		if (0x0119/*PtgAttrSemi*/ == (*it)->getPtgId())
		{
			return true;
		}
	}
	return false;
}
const bool ParsedFormula::HasPtgTbl() const
{
	const PtgVector& ptgs = rgce.getPtgs();
	for(PtgVectorIterator it = ptgs.begin(), itEnd = ptgs.end(); it != itEnd; ++it)
	{
		if(0x0002/*PtgTbl*/ == (*it)->getPtgId())
		{
			return true;
		}
	}
	return false;
}


const std::wstring ParsedFormula::getAssembledFormula(bool full_ref) const
{
	const PtgVector& ptgs = rgce.getPtgs();
	if(ptgs.empty())
	{
		return L"";
	}

	try
	{
		AssemblerStack ptg_stack;
		PtgQueue extra_data = rgcb.getPtgs();
		for(PtgVectorIterator it = ptgs.begin(), itEnd = ptgs.end(); it != itEnd; ++it)
		{
			(*it)->assemble(ptg_stack, extra_data, full_ref);
		}
		if(1 != ptg_stack.size())
		{
			return L"";
			//EXCEPT::LE::WrongAPIUsage("Wrong formula assembling.", __FUNCTION__);
		}
		return ptg_stack.top();
	}
	catch(...)
	{
	}
	return L"";
}


//const bool ParsedFormula::parseStringFormula(const std::wstring formula, const std::wstring & tag_name)
//{
//	StringPtgParser parser;
//	if(parser.parseToPtgs(boost::algorithm::trim_copy(formula), rgce, rgcb, tag_name))
//	{
//		return true;
//	}
//	return false;
//}


} // namespace XLS

