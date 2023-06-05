#include "parser_iterator.h"
#include "parser_helpers.h"
#include "unknown_symbol_exception.h"

namespace StarMathParser
{

ParserIterator::ParserIterator(
                               const std::shared_ptr<TVecParserPriorityPair>& vDividers,
                               const std::shared_ptr<TVecParserPriorityPair>& vOperators,
                               const TString& sExpression, int nIndex
                               )
                               : m_sInput{sExpression}
                               , m_nIndex{nIndex}
                               , m_vDividers{vDividers}
                               , m_vOperators{vOperators} {}

bool ParserIterator::HasNext() const
{
	while ((m_nIndex < m_sInput.length()) && isspace(m_sInput[m_nIndex]))
	{
		m_nIndex++;
	}
	return m_sInput.length() > m_nIndex;
}

TString ParserIterator::GetNext() const
{
	TString sOutputToken = "";
	bool bWasAllowedAlnum = false;

	while (m_nIndex < m_sInput.length())
	{
		int nLen = 0;
		if (StarMathParser::Parser::Helpers::IsAllowedAlnum(m_sInput, m_nIndex, nLen))
		{
			bWasAllowedAlnum = true;
			sOutputToken += m_sInput.substr(m_nIndex, nLen);
			m_nIndex += nLen;
		} else
		if ((bWasAllowedAlnum == true && Parser::Helpers::IsAllowedSequence(m_vDividers, m_sInput, m_nIndex, nLen)) ||
				(bWasAllowedAlnum == true && Parser::Helpers::IsAllowedSequence(m_vOperators, m_sInput, m_nIndex, nLen)) || isspace(m_sInput[m_nIndex]))
		{
			break;
		} else
		if ((bWasAllowedAlnum == false && Parser::Helpers::IsAllowedSequence(m_vDividers, m_sInput, m_nIndex, nLen)) ||
						(bWasAllowedAlnum == false && Parser::Helpers::IsAllowedSequence(m_vOperators, m_sInput, m_nIndex, nLen)))
		{
			sOutputToken += m_sInput.substr(m_nIndex, nLen);
			m_nIndex += nLen;
			break;
		}
		else
		{
			throw UnknownSymbolException("unknown symbol", m_nIndex);
		}
	}

	return sOutputToken;
}

}
