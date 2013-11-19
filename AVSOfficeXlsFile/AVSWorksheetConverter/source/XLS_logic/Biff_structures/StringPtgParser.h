#pragma once

#include "Ptg.h"

namespace XLS
{;

class Rgce;
class RgbExtra;

class StringPtgParser
{
public:
	const bool parseToPtgs(const std::wstring& assembled_formula, Rgce& rgce, RgbExtra& rgb, MSXML2::IXMLDOMElementPtr parent);

private:
	PtgStack ptg_stack;

};


} // namespace XLS

