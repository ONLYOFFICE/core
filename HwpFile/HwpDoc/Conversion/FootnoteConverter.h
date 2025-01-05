#ifndef FOOTNOTECONVERTER_H
#define FOOTNOTECONVERTER_H

#include "../../../DesktopEditor/common/StringBuilder.h"

#include "../Paragraph/CtrlNote.h"

namespace HWP
{
class CFootnoteConverter
{
	std::vector<std::pair<std::wstring, unsigned short>> m_arNotWrittenIds;

	unsigned short m_unCountFootnotes;
	unsigned short m_unCountEndnotes;

	NSStringUtils::CStringBuilder m_oFootnoteXml;
	NSStringUtils::CStringBuilder m_oEndnoteXml;
public:
	CFootnoteConverter();

	void Clear();

	bool NeedWritten() const;
	std::vector<std::pair<std::wstring, unsigned short>>GetNotWrittenIds(bool bNeedClear = false);

	void CreateFootnote(const CCtrlNote* pNote);
	bool SaveToFile(const std::wstring& wsDirectory);
};
}

#endif // FOOTNOTECONVERTER_H
