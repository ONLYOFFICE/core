#ifndef SM_PARSER_TOKENWITHATTRIBUTES_H
#define SM_PARSER_TOKENWITHATTRIBUTES_H

#include "base_types.h"

namespace StarMathParser
{

struct TokenWithAttributes
{
	TString m_sToken = "";
	uint32_t m_nNestedLevel = 0;
	uint16_t m_nTypeOfBrackets = 0; // '()', '{}', '<>'
	std::vector<int> m_nAttributesSet;
};

}

#endif // SM_PARSER_TOKENWITHATTRIBUTES_H
