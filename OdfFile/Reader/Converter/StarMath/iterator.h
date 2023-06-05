#ifndef SM_PARSER_ITERATOR_H
#define SM_PARSER_ITERATOR_H

#include "base_types.h"

namespace StarMathParser::Interfaces
{

class Iterator
{
public:
	virtual bool HasNext() const = 0;
	virtual TString GetNext() const = 0;
};

}

#endif // SM_PARSER_ITERATOR_H
