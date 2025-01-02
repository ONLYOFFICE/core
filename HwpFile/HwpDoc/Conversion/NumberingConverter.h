#ifndef NUMBERINGCONVERTER_H
#define NUMBERINGCONVERTER_H

#include "../HWPElements/HWPRecordNumbering.h"
#include "../HWPElements/HWPRecordParaShape.h"

#include "../../../DesktopEditor/common/StringBuilder.h"

namespace HWP
{
class CNumberingConverter
{
	std::vector<const CHWPRecordNumbering*> m_arUsedNumbering;
public:
	CNumberingConverter();

	int CreateNumbering(const CHWPRecordNumbering* pNumbering, EHeadingType eHeadingType, NSStringUtils::CStringBuilder& oStringBuilder);
	void WriteEnding(NSStringUtils::CStringBuilder& oStringBuilder);
};
}

#endif // NUMBERINGCONVERTER_H
