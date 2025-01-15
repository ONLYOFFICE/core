#ifndef NUMBERINGCONVERTER_H
#define NUMBERINGCONVERTER_H

#include "../HWPElements/HWPRecordNumbering.h"
#include "../HWPElements/HWPRecordParaShape.h"

#include "../../../DesktopEditor/common/StringBuilder.h"

namespace HWP
{
class CNumberingConverter
{
	NSStringUtils::CStringBuilder m_oNumberXml;
	std::vector<const CHWPRecordNumbering*> m_arUsedNumbering;
public:
	CNumberingConverter();

	unsigned int GetCountNumbering() const;

	int CreateNumbering(const CHWPRecordNumbering* pNumbering, EHeadingType eHeadingType);
	bool SaveToFile(const std::wstring& wsDirectory);
};
}

#endif // NUMBERINGCONVERTER_H
