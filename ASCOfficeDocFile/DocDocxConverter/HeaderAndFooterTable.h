#pragma once

#include "CharacterRange.h"
#include "FileInformationBlock.h"

#define GET_CHARS_RANGE(NAME) inline CharacterRange* Get##NAME(int nIndex)	{\
	if (m_ar##NAME.empty())					return NULL;	\
	if (nIndex < (int)m_ar##NAME.size())	return m_ar##NAME[nIndex];\
	return NULL;	}	\

namespace DocFileFormat
{
	class HeaderAndFooterTable
	{
	public: 
		HeaderAndFooterTable (FileInformationBlock* fib, POLE::Stream* pTableStream);
		~HeaderAndFooterTable();

		GET_CHARS_RANGE (FirstHeaders);	
		GET_CHARS_RANGE (EvenHeaders);
		GET_CHARS_RANGE (OddHeaders);
		GET_CHARS_RANGE (FirstFooters);
		GET_CHARS_RANGE (EvenFooters);
		GET_CHARS_RANGE (OddFooters);

	private: 

		std::vector<CharacterRange*> m_arFirstHeaders;
		std::vector<CharacterRange*> m_arEvenHeaders;
		std::vector<CharacterRange*> m_arOddHeaders;
		std::vector<CharacterRange*> m_arFirstFooters;
		std::vector<CharacterRange*> m_arEvenFooters;
		std::vector<CharacterRange*> m_arOddFooters;
	};
}