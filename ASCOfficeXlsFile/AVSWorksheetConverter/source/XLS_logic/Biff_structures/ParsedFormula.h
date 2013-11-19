#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/Rgce.h>
#include <XLS_logic/Biff_structures/RgbExtra.h>

namespace XLS
{;

class CFRecord;

class ParsedFormula : public BiffStructTagged
{
public:
	ParsedFormula(const CellRef& cell_base_ref);

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);

	void setCCE(const size_t cce_val); // mandatory to call before any use of "cce"
	const size_t getCCE() const;
	const bool HasPtgTbl() const;

private:
	const std::wstring getAssembledFormula(MSXML2::IXMLDOMElementPtr parent) const;
	const bool parseStringFormula(const std::wstring formula, MSXML2::IXMLDOMElementPtr parent);

protected:
	Rgce rgce;
	RgbExtra rgcb;
private:
	size_t cce; // Size of rgce variable. Setting explicit is mandatory
	bool cce_is_set;
};

} // namespace XLS

