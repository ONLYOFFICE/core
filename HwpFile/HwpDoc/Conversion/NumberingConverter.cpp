/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "NumberingConverter.h"
#include "../../../DesktopEditor/common/File.h"

#include "Converter2OOXML.h"

namespace HWP
{
#define HWP_NUMBERING_LEVELS   7
#define OOXML_NUMBERING_LEVELS 9

#define DEFAULT_BULLET_CHAR 0xF0B7

#define IS_SYMBOL_FONT_CHAR(ch) (0xF000 <= (unsigned int)(ch) && 0xF0FF >= (unsigned int)(ch))

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
		case EHeadingType::OUTLINE:
		case EHeadingType::NUMBER: return std::wstring(L"decimal");
		case EHeadingType::BULLET: return std::wstring(L"bullet");
		case EHeadingType::NONE:
		default: return std::wstring();
	}
}

std::wstring NumberFormatToWSTR(ENumberFormat eNumberFormat)
{
	switch (eNumberFormat)
	{
		case ENumberFormat::CIRCLED_DIGIT:           return std::wstring(L"decimalEnclosedCircle");
		case ENumberFormat::ROMAN_CAPITAL:           return std::wstring(L"upperRoman");
		case ENumberFormat::ROMAN_SMALL:             return std::wstring(L"lowerRoman");
		case ENumberFormat::LATIN_CAPITAL:
		case ENumberFormat::CIRCLED_LATIN_CAPITAL:   return std::wstring(L"upperLetter");
		case ENumberFormat::LATIN_SMALL:
		case ENumberFormat::CIRCLED_LATIN_SMALL:     return std::wstring(L"lowerLetter");
		case ENumberFormat::HANGUL_SYLLABLE:
		case ENumberFormat::CIRCLED_HANGUL_SYLLABLE: return std::wstring(L"ganada");
		case ENumberFormat::HANGUL_JAMO:
		case ENumberFormat::CIRCLED_HANGUL_JAMO:     return std::wstring(L"chosung");
		case ENumberFormat::HANGUL_PHONETIC:         return std::wstring(L"koreanCounting");
		case ENumberFormat::IDEOGRAPH:               return std::wstring(L"ideographDigital");
		case ENumberFormat::CIRCLED_IDEOGRAPH:
		case ENumberFormat::DECAGON_CIRCLE_HANJA:    return std::wstring(L"ideographEnclosedCircle");
		case ENumberFormat::DECAGON_CIRCLE:          return std::wstring(L"decimalEnclosedCircle");
		case ENumberFormat::SYMBOL:
		case ENumberFormat::USER_CHAR:
		case ENumberFormat::DIGIT:
		default:                                     return std::wstring(L"decimal");
	}
}

int CNumberingConverter::FindUsed(EHeadingType eHeadingType, const CHWPRecord* pRecord) const
{
	std::vector<std::pair<EHeadingType, const CHWPRecord*>>::const_iterator itFound = std::find_if(m_arUsedNumbering.cbegin(), m_arUsedNumbering.cend(),
	                                                                                               [pRecord, eHeadingType](const std::pair<EHeadingType, const CHWPRecord*>& oValue)
	                                                                                               { return eHeadingType == oValue.first && pRecord == oValue.second;});

	if (m_arUsedNumbering.cend() == itFound)
		return 0;

	return (int)(itFound - m_arUsedNumbering.cbegin()) + 1;
}

int CNumberingConverter::AddUsed(EHeadingType eHeadingType, const CHWPRecord* pRecord)
{
	m_arUsedNumbering.push_back(std::make_pair(eHeadingType, pRecord));

	return (int)m_arUsedNumbering.size();
}

void CNumberingConverter::WriteAbstractNumHeader(int nAbstractNumId, const std::wstring& wsMultiLevelType)
{
	m_oNumberXml.WriteString(L"<w:abstractNum w:abstractNumId=\"" + std::to_wstring(nAbstractNumId) + L"\">");
	m_oNumberXml.WriteString(L"<w:multiLevelType w:val=\"" + wsMultiLevelType + L"\"/>");
}

void CNumberingConverter::WriteLvlJc(HWP_BYTE chAlign)
{
	m_oNumberXml.WriteString(L"<w:lvlJc w:val=\"");
	switch(chAlign)
	{
		default:
		case 0x0: m_oNumberXml.WriteString(L"start"); break;
		case 0x1: m_oNumberXml.WriteString(L"center"); break;
		case 0x2: m_oNumberXml.WriteString(L"right"); break;
	}
	m_oNumberXml.WriteString(L"\"/>");
}

void CNumberingConverter::WriteLvlCharProperties(int nCharShape, CConverter2OOXML& oConverter)
{
	if (nCharShape < 0 || nCharShape > 0xFFFF)
		nCharShape = 0;

	TConversionState oState;
	oConverter.WriteRunnerStyle((short)nCharShape, m_oNumberXml, oState);
}

void CNumberingConverter::WriteNumberingLevels(const CHWPRecordNumbering* pNumbering, CConverter2OOXML& oConverter)
{
	for (short shIndex = 0; shIndex < HWP_NUMBERING_LEVELS; ++shIndex)
	{
		m_oNumberXml.WriteString(L"<w:lvl w:ilvl=\"" + std::to_wstring(shIndex) + L"\">");

		int nStart = pNumbering->GetStartNumber(shIndex);

		if (nStart < 1)
			nStart = (0 < pNumbering->GetStart()) ? (int)pNumbering->GetStart() : 1;

		m_oNumberXml.WriteString(L"<w:start w:val=\"" + std::to_wstring(nStart) + L"\"/>");

		m_oNumberXml.WriteString(L"<w:numFmt w:val=\"" + NumberFormatToWSTR(pNumbering->GetNumFormatType(shIndex)) + L"\"/>");
		m_oNumberXml.WriteString(L"<w:suff w:val=\"space\"/>");

		std::wstring wsLvlText{pNumbering->GetNumFormat(shIndex)};

		if (wsLvlText.empty())
		{
			for (short shLvl = 0; shLvl <= shIndex; ++shLvl)
				wsLvlText += L'%' + std::to_wstring(shLvl + 1) + L'.';
		}
		else
		{
			for (size_t unPos = wsLvlText.find(L'^'); std::wstring::npos != unPos; unPos = wsLvlText.find(L'^', unPos + 1))
			{
				if (unPos + 1 < wsLvlText.length() && L'1' <= wsLvlText[unPos + 1] && L'9' >= wsLvlText[unPos + 1])
					wsLvlText[unPos] = L'%';
			}
		}

		m_oNumberXml.WriteString(L"<w:lvlText w:val=\"");
		m_oNumberXml.WriteEncodeXmlString(wsLvlText);
		m_oNumberXml.WriteString(L"\"/>");

		WriteLvlJc(pNumbering->GetAlign(shIndex));
		WriteLvlCharProperties(pNumbering->GetCharShape(shIndex), oConverter);

		m_oNumberXml.WriteString(L"</w:lvl>");
	}
}

void CNumberingConverter::WriteBulletLevels(const CHWPRecordBullet* pBullet, CConverter2OOXML& oConverter)
{
	const std::wstring wsNumFormat{HeadingTypeToWSTR(EHeadingType::BULLET)};

	const bool bOwnChar = (0 != pBullet->GetBulletChar() && !pBullet->UseImage());
	const HWP_CHAR chBullet = bOwnChar ? pBullet->GetBulletChar() : (HWP_CHAR)DEFAULT_BULLET_CHAR;

	for (short shIndex = 0; shIndex < OOXML_NUMBERING_LEVELS; ++shIndex)
	{
		m_oNumberXml.WriteString(L"<w:lvl w:ilvl=\"" + std::to_wstring(shIndex) + L"\">");

		m_oNumberXml.WriteString(L"<w:start w:val=\"1\"/>");
		m_oNumberXml.WriteString(L"<w:numFmt w:val=\"" + wsNumFormat + L"\"/>");
		m_oNumberXml.WriteString(L"<w:suff w:val=\"space\"/>");

		m_oNumberXml.WriteString(L"<w:lvlText w:val=\"");
		m_oNumberXml.AddCharSafe((wchar_t)chBullet);
		m_oNumberXml.WriteString(L"\"/>");

		WriteLvlJc(pBullet->GetAlign());

		if (IS_SYMBOL_FONT_CHAR(chBullet))
		{
			const std::wstring wsFont{(DEFAULT_BULLET_CHAR == chBullet) ? L"Symbol" : L"Wingdings"};

			m_oNumberXml.WriteString(L"<w:rPr><w:rFonts w:ascii=\"" + wsFont + L"\" w:hAnsi=\"" + wsFont + L"\" w:hint=\"default\"/></w:rPr>");
		}
		else
			WriteLvlCharProperties(pBullet->GetCharShape(), oConverter);

		m_oNumberXml.WriteString(L"</w:lvl>");
	}
}

int CNumberingConverter::CreateNumbering(const CHWPRecordNumbering* pNumbering, EHeadingType eHeadingType, CConverter2OOXML& oConverter)
{
	if (nullptr == pNumbering ||
	    (EHeadingType::NUMBER != eHeadingType && EHeadingType::OUTLINE != eHeadingType))
		return 0;

	const int nUsedNumbering = FindUsed(eHeadingType, pNumbering);

	if (0 != nUsedNumbering)
		return nUsedNumbering;

	const int nNumId = AddUsed(eHeadingType, pNumbering);

	WriteAbstractNumHeader(nNumId, L"multilevel");
	WriteNumberingLevels(pNumbering, oConverter);

	m_oNumberXml.WriteString(L"</w:abstractNum>");

	return nNumId;
}

int CNumberingConverter::CreateOutline(const CHWPRecordNumbering* pNumbering, CConverter2OOXML& oConverter)
{
	return CreateNumbering(pNumbering, EHeadingType::OUTLINE, oConverter);
}

int CNumberingConverter::CreateBullet(const CHWPRecordBullet* pBullet, CConverter2OOXML& oConverter)
{
	if (nullptr == pBullet)
		return 0;

	const int nUsedNumbering = FindUsed(EHeadingType::BULLET, pBullet);

	if (0 != nUsedNumbering)
		return nUsedNumbering;

	const int nNumId = AddUsed(EHeadingType::BULLET, pBullet);

	WriteAbstractNumHeader(nNumId, L"hybridMultilevel");
	WriteBulletLevels(pBullet, oConverter);

	m_oNumberXml.WriteString(L"</w:abstractNum>");

	return nNumId;
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
