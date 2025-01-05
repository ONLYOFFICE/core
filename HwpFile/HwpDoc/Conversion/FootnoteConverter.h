#ifndef FOOTNOTECONVERTER_H
#define FOOTNOTECONVERTER_H

#include "../../../DesktopEditor/common/StringBuilder.h"

#include "../Paragraph/CtrlNote.h"

namespace HWP
{
class CFootnoteConverter
{
	std::vector<unsigned short> m_arNotWrittenIds;
	unsigned short m_unCountFootnotes;

	NSStringUtils::CStringBuilder m_oFootnoteXml;
public:
	CFootnoteConverter();

	void Clear();

	bool NeedWritten() const;
	std::vector<unsigned short> GetNotWrittenIds(bool bNeedClear = false);

	void CreateFootnote(const CCtrlNote* pNote);
	bool SaveToFile(const std::wstring& wsDirectory);
};
}

#endif // FOOTNOTECONVERTER_H
