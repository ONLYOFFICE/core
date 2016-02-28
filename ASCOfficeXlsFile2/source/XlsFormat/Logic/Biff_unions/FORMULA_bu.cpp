
#include "FORMULA.h"
#include <Logic/Biff_records/Uncalced.h>
#include <Logic/Biff_records/Formula.h>
#include <Logic/Biff_records/Array.h>
#include <Logic/Biff_records/Table.h>
#include <Logic/Biff_records/ShrFmla.h>
#include <Logic/Biff_unions/SUB.h>
#include <Logic/Biff_records/String.h>
#include <Logic/Biff_records/Continue.h>

#include <utils.h>

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

	Formula *formula = NULL;
	if(!proc.mandatory<Formula>())
	{
		return false;
	}
	m_Formula = elements_.back();
	elements_.pop_back();

	formula = dynamic_cast<Formula *>(m_Formula.get());

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
		//proc.optional<SUB>(); // I haven't found any mention about SUB in the docs
	}

	if ((formula) && (formula->fShrFmla))
	{
		for (int i = 0; i < shared_formulas_locations_ref_.size(); i++)
		{
			if (shared_formulas_locations_ref_[i].inRange(location)) m_sharedIndex = i;
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
				if ((formula->cell.ixfe.value()) && (formula->cell.ixfe > m_global_info->cellStyleXfs_count))
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
					
					formula_str = array_->formula.getAssembledFormula();
					//aca="true" ca="true"
				}

				if (!formula_str.empty())
				{
					CP_XML_STREAM() << xml::utils::replace_text_to_xml(formula_str);
				}
			}

			if (formula_cash.empty())
			{
				if (m_Cash)
				{
					String * str = dynamic_cast<String*>(m_Cash.get());
					if (str)
						formula_cash =str->string.value();
				}
			}

			if (!formula_cash.empty())
			{
				CP_XML_NODE(L"v")
				{
					CP_XML_STREAM() << formula_cash;
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

