#ifndef MARKDOWNPARAMETERS_H
#define MARKDOWNPARAMETERS_H

namespace HTML
{
struct TMarkdownParameters
{
	bool m_bUseAlternativeHTMLTags = false; //Использовать html теги там, где нет стандартной реализации в md(например для подчеркиваний)
	wchar_t m_wchUnorderedList     = L'-'; // Возможные варианты в md: -, +, *
	wchar_t m_wchOrderedList       = L'.'; // Возможные варианты в md: ., )
};
}

#endif // MARKDOWNPARAMETERS_H
