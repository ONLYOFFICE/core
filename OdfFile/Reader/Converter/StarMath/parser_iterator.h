#ifndef SM_PARSER_PARSERITERATOR_H
#define SM_PARSER_PARSERITERATOR_H

#include <memory>
#include <vector>

#include "base_types.h"
#include "iterator.h"

namespace StarMathParser
{
class ParserIterator : public Interfaces::Iterator
{
	TString m_sInput;
	mutable int m_nIndex;
	std::shared_ptr<TVecParserPriorityPair> m_vDividers;
	std::shared_ptr<TVecParserPriorityPair> m_vOperators;
public:
	ParserIterator(const std::shared_ptr<TVecParserPriorityPair>& vDividers, const std::shared_ptr<TVecParserPriorityPair>& vOperators, const TString& sExpression, int nIndex = 0);
	bool HasNext() const override;
	TString GetNext() const override;
};

}

#endif // SM_PARSER_PARSERITERATOR_H
