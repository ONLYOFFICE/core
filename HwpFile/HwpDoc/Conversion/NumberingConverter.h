#ifndef NUMBERINGCONVERTER_H
#define NUMBERINGCONVERTER_H

#include "../HWPElements/HWPRecordNumbering.h"
#include "../HWPElements/HWPRecordParaShape.h"

#include "../../../DesktopEditor/common/StringBuilder.h"

namespace HWP
{
class CConverter2OOXML;
class CNumberingConverter
{
	NSStringUtils::CStringBuilder m_oNumberXml;
	std::vector<std::pair<EHeadingType, const CHWPRecordNumbering*>> m_arUsedNumbering;
public:
	CNumberingConverter();

	unsigned int GetCountNumbering() const;

	int CreateNumbering(const CHWPRecordNumbering* pNumbering, EHeadingType eHeadingType, CConverter2OOXML& oConverter);
	bool SaveToFile(const std::wstring& wsDirectory);
};
}

#endif // NUMBERINGCONVERTER_H
