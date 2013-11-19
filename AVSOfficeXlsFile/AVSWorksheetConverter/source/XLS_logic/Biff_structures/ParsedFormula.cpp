#include "stdafx.h"
#include "NameParsedFormula.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/WrongAPIUsage.h>
#include "StringPtgParser.h"

namespace XLS
{;

ParsedFormula::ParsedFormula(const CellRef& cell_base_ref)
:	cce(0),
	cce_is_set(false),
	rgce(cell_base_ref)
{
}


void ParsedFormula::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	if(!rgce.isEmpty())
	{
		rgce.toXML(xml_tag);
	}
// RgbExtra is not necessary for XSLT
// 	if(!rgcb.isEmpty())
// 	{
// 		rgcb.toXML(xml_tag);
// 	}

	static _bstr_t attr_name(L"assembled_formula");
	xml_tag->setAttribute(attr_name, getAssembledFormula(xml_tag->GetparentNode()).c_str());
}


void ParsedFormula::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	_bstr_t assembled_formula = getStructAttribute(xml_tag, L"assembled_formula");
	if(!assembled_formula || _bstr_t(L"") == assembled_formula) // for shares formulas, array formulas, or tables
	{
		rgce.fromXML(xml_tag);
	}
	else
	{
		parseStringFormula(static_cast<wchar_t*>(assembled_formula), xml_tag);
	}
}


void ParsedFormula::setCCE(const size_t cce_val)
{
	rgce.setCCE(cce_val);
}


const size_t ParsedFormula::getCCE() const
{
	return rgce.getCCE();
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


const std::wstring ParsedFormula::getAssembledFormula(MSXML2::IXMLDOMElementPtr parent) const
{
	const PtgVector& ptgs = rgce.getPtgs();
	if(ptgs.empty())
	{
		return L"";
	}
	AssemblerStack ptg_stack;
	PtgQueue extra_data = rgcb.getPtgs();
	for(PtgVectorIterator it = ptgs.begin(), itEnd = ptgs.end(); it != itEnd; ++it)
	{
		(*it)->assemble(ptg_stack, extra_data, parent);
	}
	if(1 != ptg_stack.size())
	{
		throw EXCEPT::LE::WrongAPIUsage("Wrong formula assembling.", __FUNCTION__);
	}
	return ptg_stack.top();
}


const bool ParsedFormula::parseStringFormula(const std::wstring formula, MSXML2::IXMLDOMElementPtr parent)
{
	StringPtgParser parser;
	if(parser.parseToPtgs(boost::algorithm::trim_copy(formula), rgce, rgcb, parent))
	{
		return true;
	}
	return false;
}


} // namespace XLS

