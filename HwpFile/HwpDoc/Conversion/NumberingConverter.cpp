#include "NumberingConverter.h"
#include "../../../DesktopEditor/common/File.h"

#include "Converter2OOXML.h"

namespace HWP
{
CNumberingConverter::CNumberingConverter()
{}

unsigned int CNumberingConverter::GetCountNumbering() const
{
	return m_arUsedNumbering.size();
}

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

int CNumberingConverter::CreateNumbering(const CHWPRecordNumbering* pNumbering, EHeadingType eHeadingType, CConverter2OOXML& oConverter)
{
	if (nullptr == pNumbering || eHeadingType == EHeadingType::NONE || EHeadingType::OUTLINE == eHeadingType)
		return 0;

	std::vector<std::pair<EHeadingType, const CHWPRecordNumbering*>>::const_iterator itFound = std::find_if(m_arUsedNumbering.cbegin(), m_arUsedNumbering.cend(),
	                                                                                                        [pNumbering, eHeadingType](const std::pair<EHeadingType, const CHWPRecordNumbering*>& oValue)
	                                                                                                        { return eHeadingType == oValue.first && pNumbering == oValue.second;});

	if (m_arUsedNumbering.cend() != itFound)
		return itFound - m_arUsedNumbering.cbegin() + 1;

	m_arUsedNumbering.push_back(std::make_pair(eHeadingType, pNumbering));

	m_oNumberXml.WriteString(L"<w:abstractNum w:abstractNumId=\"" + std::to_wstring(m_arUsedNumbering.size()) + L"\">");

	const std::wstring wsNumFormat{HeadingTypeToWSTR(eHeadingType)};

	std::wstring wsLvlText;

	if (EHeadingType::NUMBER == eHeadingType)
	{
		for (short shIndex = 0; shIndex < 7; ++shIndex)
		{
			m_oNumberXml.WriteString(L"<w:lvl w:ilvl=\"" + std::to_wstring(shIndex) + L"\">");

			m_oNumberXml.WriteString(L"<w:start w:val=\"" + std::to_wstring(pNumbering->GetStartNumber(shIndex)) + L"\"/>");
			m_oNumberXml.WriteString(L"<w:numFmt w:val=\"" + wsNumFormat + L"\"/>");
			m_oNumberXml.WriteString(L"<w:suff w:val=\"space\"/>");

			wsLvlText = pNumbering->GetNumFormat(shIndex);

			if (wsLvlText.empty())
			{
				for (short shLvl = 0; shLvl <= shIndex; ++shLvl)
					wsLvlText += L'%' + std::to_wstring(shLvl + 1) + L'.';
			}
			else
				std::replace(wsLvlText.begin(), wsLvlText.end(), L'^', L'%');

			m_oNumberXml.WriteString(L"<w:lvlText w:val=\"" + wsLvlText + L"\"/>");

			m_oNumberXml.WriteString(L"<w:lvlJc w:val=\"");
			switch(pNumbering->GetAlign(shIndex))
			{
				default:
				case 0x0: m_oNumberXml.WriteString(L"start"); break;
				case 0x1: m_oNumberXml.WriteString(L"center"); break;
				case 0x2: m_oNumberXml.WriteString(L"right"); break;
			}
			m_oNumberXml.WriteString(L"\"/>");

			int nCharShape = pNumbering->GetCharShape(shIndex);

			if (INT_MAX == nCharShape)
				nCharShape = 0;

			TConversionState oState;
			oConverter.WriteRunnerStyle(nCharShape, m_oNumberXml, oState);

			m_oNumberXml.WriteString(L"</w:lvl>");
		}
	}
	else if (EHeadingType::BULLET == eHeadingType)
	{
		for (short shIndex = 0; shIndex < 9; ++shIndex)
		{
			m_oNumberXml.WriteString(L"<w:lvl w:ilvl=\"" + std::to_wstring(shIndex) + L"\">");

			m_oNumberXml.WriteString(L"<w:start w:val=\"1\"/>");
			m_oNumberXml.WriteString(L"<w:numFmt w:val=\"" + wsNumFormat + L"\"/>");

			m_oNumberXml.WriteString(L"<w:lvlJc w:val=\"");
			switch(pNumbering->GetAlign(shIndex))
			{
				default:
				case 0x0: m_oNumberXml.WriteString(L"start"); break;
				case 0x1: m_oNumberXml.WriteString(L"center"); break;
				case 0x2: m_oNumberXml.WriteString(L"right"); break;
			}
			m_oNumberXml.WriteString(L"\"/>");

			switch (shIndex % 3)
			{
				case 0:
				{
					m_oNumberXml.WriteString(L"<w:lvlText w:val=\"");
					m_oNumberXml.AddCharSafe(0xF0B7);
					m_oNumberXml.WriteString(L"\"/>");
					m_oNumberXml.WriteString(L"<w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:hint=\"default\"/></w:rPr>");
					break;
				}
				case 1:
				{
					m_oNumberXml.WriteString(L"<w:lvlText w:val=\"o\"/>");
					m_oNumberXml.WriteString(L"<w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:hint=\"default\"/></w:rPr>");
					break;
				}
				case 2:
				{
					m_oNumberXml.WriteString(L"<w:lvlText w:val=\"");
					m_oNumberXml.AddCharSafe(0xF0A7);
					m_oNumberXml.WriteString(L"\"/>");
					m_oNumberXml.WriteString(L"<w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:hint=\"default\"/></w:rPr>");
					break;
				}
			}

			m_oNumberXml.WriteString(L"</w:lvl>");
		}
	}

	m_oNumberXml.WriteString(L"</w:abstractNum>");

	return m_arUsedNumbering.size();
}

bool CNumberingConverter::SaveToFile(const std::wstring& wsDirectory)
{
	if (m_arUsedNumbering.empty())
		return false;

	NSStringUtils::CStringBuilder oNumberingData;

	oNumberingData.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
	oNumberingData.WriteString(L"<w:numbering xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" xmlns:cx2=\"http://schemas.microsoft.com/office/drawing/2015/10/21/chartex\" xmlns:cx3=\"http://schemas.microsoft.com/office/drawing/2016/5/9/chartex\" xmlns:cx4=\"http://schemas.microsoft.com/office/drawing/2016/5/10/chartex\" xmlns:cx5=\"http://schemas.microsoft.com/office/drawing/2016/5/11/chartex\" xmlns:cx6=\"http://schemas.microsoft.com/office/drawing/2016/5/12/chartex\" xmlns:cx7=\"http://schemas.microsoft.com/office/drawing/2016/5/13/chartex\" xmlns:cx8=\"http://schemas.microsoft.com/office/drawing/2016/5/14/chartex\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:aink=\"http://schemas.microsoft.com/office/drawing/2016/ink\" xmlns:am3d=\"http://schemas.microsoft.com/office/drawing/2017/model3d\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:oel=\"http://schemas.microsoft.com/office/2019/extlst\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" xmlns:w16du=\"http://schemas.microsoft.com/office/word/2023/wordml/word16du\" xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" xmlns:w16sdtfl=\"http://schemas.microsoft.com/office/word/2024/wordml/sdtformatlock\" xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex w16sdtdh w16sdtfl w16du wp14\">");
	oNumberingData.WriteString(m_oNumberXml.GetData());

	if (m_arUsedNumbering.empty())
	{
		oNumberingData.WriteString(L"<w:abstractNum w:abstractNumId=\"0\" w15:restartNumberingAfterBreak=\"0\">");
		oNumberingData.WriteString(L"<w:multiLevelType w:val=\"hybridMultilevel\"/>");
		oNumberingData.WriteString(L"<w:lvl w:ilvl=\"0\">");
		oNumberingData.WriteString(L"<w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:lvlText w:val=\"");
		oNumberingData.AddCharSafe(0xF0B7);
		oNumberingData.WriteString(L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"720\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:hint=\"default\"/></w:rPr></w:lvl>");
		oNumberingData.WriteString(L"<w:lvl w:ilvl=\"1\" w:tentative=\"1\">");
		oNumberingData.WriteString(L"<w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:lvlText w:val=\"o\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"1440\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:hint=\"default\"/></w:rPr></w:lvl>");
		oNumberingData.WriteString(L"<w:lvl w:ilvl=\"2\" w:tentative=\"1\">");
		oNumberingData.WriteString(L"<w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:lvlText w:val=\"");
		oNumberingData.AddCharSafe(0xF0A7);
		oNumberingData.WriteString(L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2160\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:hint=\"default\"/></w:rPr></w:lvl>");
		oNumberingData.WriteString(L"<w:lvl w:ilvl=\"3\" w:tentative=\"1\">");
		oNumberingData.WriteString(L"<w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:lvlText w:val=\"");
		oNumberingData.AddCharSafe(0xF0B7);
		oNumberingData.WriteString(L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2880\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:hint=\"default\"/></w:rPr></w:lvl>");
		oNumberingData.WriteString(L"<w:lvl w:ilvl=\"4\" w:tentative=\"1\">");
		oNumberingData.WriteString(L"<w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:lvlText w:val=\"o\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"3600\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:hint=\"default\"/></w:rPr></w:lvl>");
		oNumberingData.WriteString(L"<w:lvl w:ilvl=\"5\" w:tentative=\"1\">");
		oNumberingData.WriteString(L"<w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:lvlText w:val=\"");
		oNumberingData.AddCharSafe(0xF0A7);
		oNumberingData.WriteString(L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"4320\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:hint=\"default\"/></w:rPr></w:lvl>");
		oNumberingData.WriteString(L"<w:lvl w:ilvl=\"6\" w:tentative=\"1\">");
		oNumberingData.WriteString(L"<w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:lvlText w:val=\"");
		oNumberingData.AddCharSafe(0xF0B7);
		oNumberingData.WriteString(L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5040\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:hint=\"default\"/></w:rPr></w:lvl>");
		oNumberingData.WriteString(L"<w:lvl w:ilvl=\"7\" w:tentative=\"1\">");
		oNumberingData.WriteString(L"<w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:lvlText w:val=\"o\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5760\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:hint=\"default\"/></w:rPr></w:lvl>");
		oNumberingData.WriteString(L"<w:lvl w:ilvl=\"8\" w:tentative=\"1\">");
		oNumberingData.WriteString(L"<w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:lvlText w:val=\"");
		oNumberingData.AddCharSafe(0xF0A7);
		oNumberingData.WriteString(L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"6480\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:hint=\"default\"/></w:rPr></w:lvl>");
		oNumberingData.WriteString(L"</w:abstractNum>");

		oNumberingData.WriteString(L"<w:num w:numId=\"1\"><w:abstractNumId w:val=\"0\"/></w:num>");
	}
	else
	{
		for (unsigned short ushIndex = 1; ushIndex <= m_arUsedNumbering.size(); ++ushIndex)
			oNumberingData.WriteString(L"<w:num w:numId=\"" + std::to_wstring(ushIndex) + L"\"><w:abstractNumId w:val=\"" + std::to_wstring(ushIndex) + L"\"/></w:num>");
	}

	oNumberingData.WriteString(L"</w:numbering>");

	NSFile::CFileBinary oNumberingWriter;
	if (!oNumberingWriter.CreateFileW(wsDirectory + L"numbering.xml"))
		return false;

	oNumberingWriter.WriteStringUTF8(oNumberingData.GetData());
	oNumberingWriter.CloseFile();
	return true;
}
}
