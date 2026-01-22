#ifndef TEXTSETTINGS_H
#define TEXTSETTINGS_H

#include "../Common/3dParty/html/css/src/CCompiledStyle.h"
#include <stack>

namespace HTML
{
// Настройки текста
class CTextSettings
{
	std::stack<bool> m_arBidirectional;// Реверс текста
	std::stack<bool> m_arPreformatted; // Сохранение форматирования (Сохранение пробелов, табуляций, переносов строк)
	std::stack<bool> m_arQuotation;    // Цитата
	std::stack<bool> m_arAddSpaces;    // Добавлять пробелы перед текстом?
	std::stack<bool> m_arMergeText;    // Объединять подряд идущий текст в 1?

	std::wstring m_sPStyle;

	NSCSS::CCompiledStyle m_oAdditionalStyle;

public:
	CTextSettings()
	{
		SetNewBidirectional(false);
		SetNewPreformatted(false);
		SetNewQuotation(false);
		SetNewAddSpaces(true);
	}

	#define CREATE_METHODS_FROM_ELEMENT(element_name)\
	void SetNew##element_name(bool bValue)\
	{\
		m_ar##element_name.push(bValue);\
	}\
	\
	void RollBack##element_name()\
	{\
		if (m_ar##element_name.size() > 1)\
			m_ar##element_name.pop();\
	}\
	\
	bool Get##element_name() const\
	{\
		return m_ar##element_name.top();\
	}

	CREATE_METHODS_FROM_ELEMENT(Bidirectional)
	CREATE_METHODS_FROM_ELEMENT(Preformatted)
	CREATE_METHODS_FROM_ELEMENT(Quotation)
	CREATE_METHODS_FROM_ELEMENT(AddSpaces)
};
}

#endif // TEXTSETTINGS_H
