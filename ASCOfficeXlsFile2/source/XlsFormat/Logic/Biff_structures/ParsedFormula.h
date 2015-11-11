#pragma once

#include "BiffStructure.h"
#include "Rgce.h"
#include "RgbExtra.h"

namespace XLS
{

class CFRecord;

class ParsedFormula : public BiffStructure
{
public:
	ParsedFormula(const CellRef& cell_base_ref);

	void setCCE(const size_t cce_val); // mandatory to call before any use of "cce"
	const size_t getCCE() const;
	const bool HasPtgTbl() const;

	const std::wstring getAssembledFormula() const;
	
	static const ElementType	type = typeParsedFormula;

	Rgce		rgce;
	RgbExtra	rgcb;

	size_t	cce; // Size of rgce variable. Setting explicit is mandatory
	bool	cce_is_set;
};

} // namespace XLS

