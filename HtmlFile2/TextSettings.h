#ifndef TEXTSETTINGS_H
#define TEXTSETTINGS_H

#include "../Common/3dParty/html/css/src/CCompiledStyle.h"

namespace HTML
{
// Настройки текста
struct CTextSettings
{
	bool bBdo; // Реверс текста
	bool bPre; // Сохранение форматирования (Сохранение пробелов, табуляций, переносов строк)
	bool bQ;   // Цитата
	bool bAddSpaces; // Добавлять пробелы перед текстом?
	bool bMergeText; // Объединять подряд идущий текст в 1?
	int  nLi;  // Уровень списка
	bool bNumberingLi; // Является ли список нумерованным

	std::wstring sPStyle;

	enum ETextMode
	{
		Normal,
		Superscript,
		Subscript
	} eTextMode;

	NSCSS::CCompiledStyle oAdditionalStyle;

	CTextSettings()
		: bBdo(false), bPre(false), bQ(false), bAddSpaces(true), bMergeText(false), nLi(-1), bNumberingLi(false), eTextMode(Normal)
	{}

	CTextSettings(const CTextSettings& oTS) :
		bBdo(oTS.bBdo), bPre(oTS.bPre), bQ(oTS.bQ), bAddSpaces(oTS.bAddSpaces), bMergeText(oTS.bMergeText),
	    nLi(oTS.nLi), bNumberingLi(oTS.bNumberingLi), sPStyle(oTS.sPStyle), eTextMode(oTS.eTextMode),
	    oAdditionalStyle(oTS.oAdditionalStyle)
	{}

	void AddPStyle(const std::wstring& wsStyle)
	{
		if (std::wstring::npos == sPStyle.find(wsStyle))
			sPStyle += wsStyle;
	}
};
}

#endif // TEXTSETTINGS_H
