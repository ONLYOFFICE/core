#ifndef FOOTNOTECONVERTER_H
#define FOOTNOTECONVERTER_H

#include "../../../DesktopEditor/common/StringBuilder.h"

#include "../Paragraph/CtrlNote.h"
#include "../Paragraph/CtrlHeadFoot.h"

namespace HWP
{
class CConverter2OOXML;
class CFootnoteConverter
{
	unsigned short m_ushCountFootnotes;
	unsigned short m_ushCountEndnotes;

	unsigned short m_ushHeaderCount;
	unsigned short m_ushFooterCount;

	NSStringUtils::CStringBuilder m_oFootnoteXml;
	NSStringUtils::CStringBuilder m_oEndnoteXml;
public:
	CFootnoteConverter();

	std::wstring CreateNote(const CCtrlNote* pNote, CConverter2OOXML& oConverter);
	std::wstring CreateHeadOrFoot(const CCtrlHeadFoot* pCtrlHeadFoot, CConverter2OOXML& oConverter);

	bool SaveToFile(const std::wstring& wsDirectory);

	unsigned short GetFootnoteCount() const;
	unsigned short GetEndnoteCount() const;
};
}

#endif // FOOTNOTECONVERTER_H
