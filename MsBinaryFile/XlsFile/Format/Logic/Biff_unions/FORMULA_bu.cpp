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

#include "FORMULA.h"
#include "../Biff_records/Uncalced.h"
#include "../Biff_records/Formula.h"
#include "../Biff_records/Array.h"
#include "../Biff_records/Table.h"
#include "../Biff_records/ShrFmla.h"
#include "../Biff_unions/SUB.h"
#include "../Biff_records/String.h"
#include "../Biff_records/Continue.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

FORMULA::FORMULA(std::vector<CellRangeRef>& shared_formulas_locations_ref) : shared_formulas_locations_ref_(shared_formulas_locations_ref)
{
	m_sharedIndex = -1;
}


FORMULA::~FORMULA()
{
}


BaseObjectPtr FORMULA::clone()
{
	return BaseObjectPtr(new FORMULA(*this));
}


// FORMULA = [Uncalced] Formula [Array / Table / ShrFmla / SUB] [String *Continue]
const bool FORMULA::loadContent(BinProcessor& proc)
{
	m_global_info = proc.getGlobalWorkbookInfo();

	proc.optional<Uncalced>();

	if  (proc.optional<Formula>())
	{
		m_Formula = elements_.back();
		elements_.pop_back();
	}
	else if (proc.optional<Formula_BIFF3>())
	{
		m_Formula = elements_.back();
		elements_.pop_back();
	}
	else if (proc.optional<Formula_BIFF4>())
	{
		m_Formula = elements_.back();
		elements_.pop_back();
	}
//-----------------------------------------------------------------------------
	Formula *formula = dynamic_cast<Formula *>(m_Formula.get());
	if (!formula)
		return false;

	location = formula->getLocation();

	Array array_formula(location);
	if (proc.optional(array_formula))
	{
		m_ArrayFormula = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<Table>())
	{
		m_TableFormula = elements_.back();
		elements_.pop_back();
	}
	
	if(!m_ArrayFormula && !m_TableFormula)
	{
		ShrFmla shr_formula(location);
		if(proc.optional(shr_formula))
		{
			m_SharedFormula = elements_.back();
			elements_.pop_back();

			m_sharedIndex = shared_formulas_locations_ref_.size();
			shared_formulas_locations_ref_.push_back(shr_formula.ref_);
		}
		else
		{
			//proc.optional<SUB>(); 
		}
	}

	if ((formula) && (formula->fShrFmla))
	{
		for (size_t i = 0; i < shared_formulas_locations_ref_.size(); i++)
		{
			if (shared_formulas_locations_ref_[i].inRange(location)) 
				m_sharedIndex = i;
		}		
	}

	if(proc.optional<String>())
	{
		m_Cash = elements_.back();
		elements_.pop_back();
		
		int count  = proc.repeated<Continue>(0, 0);
		while(count > 0)
		{
			m_arContinue.insert(m_arContinue.begin(), elements_.back());
			elements_.pop_back();
			count--;
		}
	}

	return true;
}

const CellRef FORMULA::getLocation() const
{
	return location;
}

int FORMULA::serialize(std::wostream & stream)
{
	Formula *formula	= dynamic_cast<Formula *>	(m_Formula.get());
	ShrFmla *shared		= dynamic_cast<ShrFmla *>	(m_SharedFormula.get());
	Array	*array_		= dynamic_cast<Array *>		(m_ArrayFormula.get());
	Table	*table		= dynamic_cast<Table *>		(m_TableFormula.get());

	if (formula == NULL && shared == NULL && array_ == NULL && table == NULL) return 0;
	
	CP_XML_WRITER(stream)    
    {
		std::wstring ref = formula->cell.getLocation().toString();
		
		std::wstring formula_str;
		std::wstring formula_cash;
	
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			if (formula)
			{
				switch (formula->val.getType())
				{
					case 0: CP_XML_ATTR(L"t", L"str");	break;
					case 1: CP_XML_ATTR(L"t", L"b");	break;
					case 2: CP_XML_ATTR(L"t", L"e");	break;
				}	
				if (formula->cell.ixfe > m_global_info->cellStyleXfs_count)
				{
					CP_XML_ATTR(L"s", formula->cell.ixfe - m_global_info->cellStyleXfs_count);
				}
			}

			CP_XML_NODE(L"f")
			{			
				if (formula)
				{
					formula_str		= formula->formula.getAssembledFormula();
					formula_cash	= formula->val.getValue();
				}
				if ((shared) || ((formula) && (formula->fShrFmla && m_sharedIndex >= 0)))
				{
					if (shared)
					{
						formula_str = shared->formula.getAssembledFormula();

						CP_XML_ATTR(L"ref", shared->ref_.toString());	
					}
					CP_XML_ATTR(L"t", L"shared");	
					CP_XML_ATTR(L"si", m_sharedIndex);	
				}		
				if (array_)
				{
					CP_XML_ATTR(L"ref", array_->ref_.toString());

					CP_XML_ATTR(L"t", L"array");
					CP_XML_ATTR(L"ca", L"true");
					
					if (array_->fAlwaysCalc) CP_XML_ATTR(L"aca", L"true");
					
					formula_str = array_->formula.getAssembledFormula();
				}

				if (!formula_str.empty())
				{
					CP_XML_STREAM() << XmlUtils::EncodeXmlString(formula_str);
				}
			}

			if (formula_cash.empty() && m_Cash)
			{
				String * str = dynamic_cast<String*>(m_Cash.get());
				if (str)
				{
					formula_cash =str->string.value();
				}
			}

			if (!formula_cash.empty())
			{
				CP_XML_NODE(L"v")
				{
					CP_XML_STREAM() << XmlUtils::EncodeXmlString(formula_cash, true);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

