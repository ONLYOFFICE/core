#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Rgce.h>
#include <Logic/Biff_structures/RgbExtra.h>

namespace XLS
{;

class CFRecord;

class ParsedFormula : public BiffStructure
{
public:
	ParsedFormula(const CellRef& cell_base_ref);

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);

	void setCCE(const size_t cce_val); // mandatory to call before any use of "cce"
	const size_t getCCE() const;
	const bool HasPtgTbl() const;

private:
	const std::wstring getAssembledFormula(BiffStructurePtr & parent) const;
	const bool parseStringFormula(const std::wstring formula, BiffStructurePtr & parent);

protected:
	Rgce rgce;
	RgbExtra rgcb;
private:
	size_t cce; // Size of rgce variable. Setting explicit is mandatory
	bool cce_is_set;
};

} // namespace XLS

