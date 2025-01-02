#include "NumberingConverter.h"

namespace HWP
{
CNumberingConverter::CNumberingConverter()
{}

std::wstring HeadingTypeToWSTR(EHeadingType eHeadingType)
{
	switch(eHeadingType)
	{
		case EHeadingType::NONE: return std::wstring();
		case EHeadingType::OUTLINE: return std::wstring();
		case EHeadingType::NUMBER: return std::wstring(L"decimal");
		case EHeadingType::BULLET: return std::wstring(L"bullet");
	}
}

int CNumberingConverter::CreateNumbering(const CHWPRecordNumbering* pNumbering, EHeadingType eHeadingType, NSStringUtils::CStringBuilder& oStringBuilder)
{
	if (nullptr == pNumbering || eHeadingType == EHeadingType::NONE || EHeadingType::OUTLINE == eHeadingType)
		return 0;

	std::vector<const CHWPRecordNumbering*>::const_iterator itFound = std::find(m_arUsedNumbering.cbegin(), m_arUsedNumbering.cend(), pNumbering);

	if (m_arUsedNumbering.cend() != itFound)
		return itFound - m_arUsedNumbering.cbegin() + 1;

	m_arUsedNumbering.push_back(pNumbering);

	oStringBuilder.WriteString(L"<w:abstractNum w:abstractNumId=\"" + std::to_wstring(m_arUsedNumbering.size()) + L"\">");

	const std::wstring wsNumFormat{HeadingTypeToWSTR(eHeadingType)};

	std::wstring wsLvlText;

	if (EHeadingType::NUMBER == eHeadingType)
	{
		for (short shIndex = 0; shIndex < 7; ++shIndex)
		{
			oStringBuilder.WriteString(L"<w:lvl w:ilvl=\"" + std::to_wstring(shIndex) + L"\">");

			oStringBuilder.WriteString(L"<w:start w:val=\"" + std::to_wstring(pNumbering->GetStartNumber(shIndex)) + L"\"/>");
			oStringBuilder.WriteString(L"<w:numFmt w:val=\"" + wsNumFormat + L"\"/>");
			oStringBuilder.WriteString(L"<w:suff w:val=\"space\"/>");

			wsLvlText = pNumbering->GetNumFormat(shIndex);
			std::replace(wsLvlText.begin(), wsLvlText.end(), L'^', L'%');
			oStringBuilder.WriteString(L"<w:lvlText w:val=\"" + wsLvlText + L"\"/>");

			oStringBuilder.WriteString(L"<w:lvlJc w:val=\"");
			switch(pNumbering->GetAlign(shIndex))
			{
				default:
				case 0x0: oStringBuilder.WriteString(L"start"); break;
				case 0x1: oStringBuilder.WriteString(L"center"); break;
				case 0x2: oStringBuilder.WriteString(L"right"); break;
			}
			oStringBuilder.WriteString(L"\"/>");

			oStringBuilder.WriteString(L"</w:lvl>");
		}
	}
	else if (EHeadingType::BULLET == eHeadingType)
	{
		for (short shIndex = 0; shIndex < 9; ++shIndex)
		{
			oStringBuilder.WriteString(L"<w:lvl w:ilvl=\"" + std::to_wstring(shIndex) + L"\">");

			oStringBuilder.WriteString(L"<w:numFmt w:val=\"" + wsNumFormat + L"\"/>");
			oStringBuilder.WriteString(L"<w:suff w:val=\"space\"/>");
			oStringBuilder.WriteString(L"<w:isLgl w:val=\"false\"/>");

			oStringBuilder.WriteString(L"<w:lvlJc w:val=\"");
			switch(pNumbering->GetAlign(shIndex))
			{
				default:
				case 0x0: oStringBuilder.WriteString(L"start"); break;
				case 0x1: oStringBuilder.WriteString(L"center"); break;
				case 0x2: oStringBuilder.WriteString(L"right"); break;
			}
			oStringBuilder.WriteString(L"\"/>");

			switch (shIndex % 3)
			{
				case 0:
				{
					oStringBuilder.WriteString(L"<w:lvlText w:val=\"");
					oStringBuilder.AddCharSafe(0xF0B7);
					oStringBuilder.WriteString(L"\"/>");
					oStringBuilder.WriteString(L"<w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:hint=\"default\"/></w:rPr>");
					break;
				}
				case 1:
				{
					oStringBuilder.WriteString(L"<w:lvlText w:val=\"o\"/>");
					oStringBuilder.WriteString(L"<w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:hint=\"default\"/></w:rPr>");
					break;
				}
				case 2:
				{
					oStringBuilder.WriteString(L"<w:lvlText w:val=\"");
					oStringBuilder.AddCharSafe(0xF0A7);
					oStringBuilder.WriteString(L"\"/>");
					oStringBuilder.WriteString(L"<w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:hint=\"default\"/></w:rPr>");
					break;
				}
			}

			oStringBuilder.WriteString(L"</w:lvl>");
		}
	}

	oStringBuilder.WriteString(L"</w:abstractNum>");

	return m_arUsedNumbering.size();
}

void CNumberingConverter::WriteEnding(NSStringUtils::CStringBuilder& oStringBuilder)
{
	for (unsigned short ushIndex = 1; ushIndex <= m_arUsedNumbering.size(); ++ushIndex)
		oStringBuilder.WriteString(L"<w:num w:numId=\"" + std::to_wstring(ushIndex) + L"\"><w:abstractNumId w:val=\"" + std::to_wstring(ushIndex) + L"\"/></w:num>");
}
}
