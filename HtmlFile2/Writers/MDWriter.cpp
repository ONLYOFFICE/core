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

#include "MDWriter.h"

#include <cwctype>

#include "../../DesktopEditor/common/File.h"
#include "../../Common/3dParty/html/css/src/CCompiledStyle.h"

namespace HTML
{
#define MD_BREAK_LINE L"  \n"

CMDWriter::CMDWriter(const TMarkdownParameters& oMDParametrs)
	: m_oMDParametrs(oMDParametrs)
{
	m_arStates.push(TState{});
	m_arStates.top().m_pCurrentDocument = &m_oDocument;
}

void CMDWriter::Begin(const std::wstring& wsDst)
{}

void CMDWriter::End(const std::wstring& wsDst)
{
	NSFile::CFileBinary oDocument;

	if (!oDocument.CreateFileW(wsDst))
		return;

	if (0 != m_oMetadata.GetCurSize())
	{
		oDocument.WriteStringUTF8(L"---");
		oDocument.WriteStringUTF8(MD_BREAK_LINE);
		oDocument.WriteStringUTF8(m_oMetadata.GetData());
		oDocument.WriteStringUTF8(L"---");
		oDocument.WriteStringUTF8(MD_BREAK_LINE);
	}
	oDocument.WriteStringUTF8(m_oDocument.GetData());
	oDocument.CloseFile();
}

inline void ReplaceSpaces(std::wstring& wsValue)
{
	// boost::wregex oRegex(L"\\s+");
	// wsValue = boost::regex_replace(wsValue, oRegex, L" ");

	std::wstring::const_iterator itBegin = std::find_if(wsValue.cbegin(), wsValue.cend(), [](wchar_t wchValue){ return std::iswspace(wchValue) && 0xa0 != wchValue; });
	std::wstring::const_iterator itEnd;

	while (wsValue.cend() != itBegin)
	{
		itEnd = std::find_if(itBegin, wsValue.cend(), [](wchar_t wchValue){ return !std::iswspace(wchValue) || 0xa0 == wchValue; });

		wsValue.replace(itBegin, itEnd, L" ");

		itBegin = std::find_if(itBegin + 1, wsValue.cend(), [](wchar_t wchValue){ return std::iswspace(wchValue) && 0xa0 != wchValue; });
	}
}

void ReplaceSpecialCharsInEdges(std::wstring& wsText)
{
	// Hexadecimal codes of the characters being replaced
	const wchar_t SP  {0x20};    // space
	const wchar_t NBSP{0xA0};    // non-breaking space
	const wchar_t ENSP{0x2002};  // en space
	const wchar_t EMSP{0x2003};  // em space

	// Corresponding HTML sequences
	const std::wstring NBSP_REP{L"&nbsp;"};
	const std::wstring ENSP_REP{L"&ensp;"};
	const std::wstring EMSP_REP{L"&emsp;"};

	auto is_special = [&](wchar_t c) -> bool {
		return c == SP || c == NBSP || c == ENSP || c == EMSP;
	};

	size_t unFirstNormal{0};
	while (unFirstNormal < wsText.size() && is_special(wsText[unFirstNormal]))
		++unFirstNormal;

	if (unFirstNormal == wsText.size())
	{
		std::wstring wsResult;
		wsResult.reserve(wsText.size() * 6);  // each character will be replaced by 6
		for (wchar_t c : wsText)
		{
			if (c == SP || c == NBSP) wsResult.append(NBSP_REP);
			else if (c == ENSP)       wsResult.append(ENSP_REP);
			else if (c == EMSP)       wsResult.append(EMSP_REP);
			else                      wsResult.push_back(c);
		}
		wsText = std::move(wsResult);
		return;
	}

	size_t unLastNormal{wsText.size() - 1};
	while (unLastNormal > unFirstNormal && is_special(wsText[unLastNormal]))
		--unLastNormal;

	const size_t unLeftCount {unFirstNormal};
	const size_t unRightCount{wsText.size() - 1 - unLastNormal};

	const size_t unNormalLen{unLastNormal - unFirstNormal + 1};
	const size_t unNewLen   {unNormalLen + unLeftCount * 6 + unRightCount * 6};

	std::wstring wsResult;
	wsResult.reserve(unNewLen);

	for (size_t i = 0; i < unLeftCount; ++i)
	{
		wchar_t c = wsText[i];
		if (c == SP || c == NBSP) wsResult.append(NBSP_REP);
		else if (c == ENSP)       wsResult.append(ENSP_REP);
		else if (c == EMSP)       wsResult.append(EMSP_REP);
	}

	for (size_t i = unFirstNormal; i <= unLastNormal; ++i)
		wsResult.push_back(wsText[i]);

	for (size_t i = unLastNormal + 1; i < wsText.size(); ++i)
	{
		wchar_t c = wsText[i];
		if (c == SP || c == NBSP) wsResult.append(NBSP_REP);
		else if (c == ENSP)       wsResult.append(ENSP_REP);
		else if (c == EMSP)       wsResult.append(EMSP_REP);
	}

	wsText = std::move(wsResult);
}

bool CMDWriter::WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors)
{
	bool bPreformatted{InPreformatted()};

	const NSCSS::CCompiledStyle* pCompiledStyle{arSelectors.back().m_pCompiledStyle};

	if (!bPreformatted && nullptr != pCompiledStyle)
	{
		// TODO:: behavior should be slightly different (implement)
		switch(pCompiledStyle->m_oDisplay.GetWhiteSpace().ToInt())
		{
			case NSCSS::NSProperties::EWhiteSpace::Pre:
			case NSCSS::NSProperties::EWhiteSpace::Pre_Wrap:
			case NSCSS::NSProperties::EWhiteSpace::Pre_Line:
				bPreformatted = true;
			default:
				break;
		}
	}

	if (!bPreformatted && wsText.end() == std::find_if_not(wsText.begin(), wsText.end(), [](wchar_t wchChar){ return iswspace(wchChar) && 0xa0 != wchChar;}))
		return false;

	if (bPreformatted && !m_arStates.top().m_bEmptyLine && !m_arStates.top().m_bInTable)
	{
		for (std::vector<NSCSS::CNode>::const_reverse_iterator itElement{arSelectors.crbegin()}; itElement < arSelectors.crend(); ++itElement)
		{
			if (L"pre" == itElement->m_wsName)
			{
				GetCurrentDocument()->AddCharSafe(L'\n');
				break;
			}
		}
	}

	// For now works correctly only for text (need to check with other nodes)
	if (m_arStates.top().m_bEmptyLine)
	{
		for (UINT unIndex = 0; unIndex < GetLevelBlockquote(); ++unIndex)
			WriteString(L"> ", true);
	}

	const bool bOnlySpaces{wsText.end() == std::find_if_not(wsText.begin(), wsText.end(), [](wchar_t wchChar){ return iswspace(wchChar);})};

	if (!bPreformatted && !InCode())
	{
		ReplaceSpaces(wsText);
		ReplaceSpecialCharsInEdges(wsText);
	}

	bool bNeedBold  {false},
		 bNeedItalic{false},
		 bNeedStrike{false};

	if (nullptr != pCompiledStyle && !bOnlySpaces)
	{
		if (pCompiledStyle->m_oFont.Bold())
			bNeedBold = true;

		if (pCompiledStyle->m_oFont.Italic())
			bNeedItalic = true;

		if (pCompiledStyle->m_oText.LineThrough())
			bNeedStrike = true;
	}

	if (bNeedBold)
		WriteOpenSpecialString(L"**");

	if (bNeedItalic)
		WriteOpenSpecialString(L"*");

	if (bNeedStrike)
		WriteOpenSpecialString(L"~~");

	ApplyAlternativeTags(pCompiledStyle);
	WriteString(wsText);
	ApplyAlternativeTags(pCompiledStyle, true);

	if (bNeedBold)
		WriteCloseSpecialString(L"**");

	if (bNeedItalic)
		WriteCloseSpecialString(L"*");

	if (bNeedStrike)
		WriteCloseSpecialString(L"~~");

	if (L'\n' == wsText.back())
		m_arStates.top().m_bNeedBreakLine = false;

	return true;
}

void CMDWriter::WriteEmptyParagraph(bool bVahish, bool bInP)
{}

void CMDWriter::WriteMetadata(const std::wstring& wsName, const std::wstring& wsValue)
{
	if (wsName.empty() || wsValue.empty())
		return;

	m_oMetadata.WriteString(wsName + L": " + wsValue + MD_BREAK_LINE);
}

void CMDWriter::PageBreak()
{}

void CMDWriter::BeginBlock()
{
	if (!m_arStates.top().m_bInList)
		WriteBreakLine();
}

void CMDWriter::EndBlock(bool bAddBlock)
{
	if (!m_arStates.top().m_bInTable)
		WriteBreakLine();
}

void CMDWriter::SetDataOutput(XmlString* pOutputData)
{
	SaveState();
	m_arStates.top().m_pCurrentDocument = pOutputData;
}

void CMDWriter::RevertDataOutput()
{
	RollBackState();
}

TMarkdownParameters CMDWriter::GetParametrs() const
{
	return m_oMDParametrs;
}

void CMDWriter::WriteString(const std::wstring& wsString, bool bSpecialString)
{
	GetCurrentDocument()->WriteString(wsString);

	if (m_arStates.top().m_bEmptyLine)
		m_arStates.top().m_bEmptyLine = wsString.empty();

	if (!bSpecialString)
	{
		if (!m_arStates.top().m_oSpecialString.m_wsLastSpecialString.empty() &&
		    m_arStates.top().m_oSpecialString.m_bClosed)
			m_arStates.top().m_oSpecialString.m_wsLastSpecialString.clear();

		m_arStates.top().m_bNeedBreakLine = true;
	}
}

void CMDWriter::WriteOpenSpecialString(const std::wstring& wsString)
{
	if (!m_arStates.top().m_oSpecialString.m_wsLastSpecialString.empty() &&
	    (m_arStates.top().m_oSpecialString.m_wsLastSpecialString == wsString ||
	     (L'*' == m_arStates.top().m_oSpecialString.m_wsLastSpecialString.front() && L'*' == wsString.front())))
		GetCurrentDocument()->WriteString(L" ");

	m_arStates.top().m_oSpecialString.m_wsLastSpecialString.clear();
	m_arStates.top().m_oSpecialString.m_bClosed = false;

	WriteString(wsString, true);
}

void CMDWriter::WriteCloseSpecialString(const std::wstring& wsString)
{
	m_arStates.top().m_oSpecialString.m_wsLastSpecialString = wsString;
		m_arStates.top().m_oSpecialString.m_bClosed = true;

	WriteString(wsString, true);
}

XmlString* CMDWriter::GetCurrentDocument() const
{
	return m_arStates.top().m_pCurrentDocument;
}

void CMDWriter::WriteBreakLine(bool bNeedChecked)
{
	if (bNeedChecked && !m_arStates.top().m_bNeedBreakLine)
		return;

	if (m_arStates.top().m_bInTable && bNeedChecked)
	{
		if (m_oMDParametrs.m_bUseAlternativeHTMLTags)
			WriteString(L"</br>");
		else
			WriteString(L" ");

		m_arStates.top().m_bNeedBreakLine = false;
		return;
	}

	if (m_arStates.top().m_bEmptyLine)
	{
		for (UINT unIndex = 0; unIndex < GetLevelBlockquote(); ++unIndex)
			WriteString(L"> ", true);
	}

	GetCurrentDocument()->WriteString(MD_BREAK_LINE);

	m_arStates.top().m_bEmptyLine = true;
	m_arStates.top().m_bNeedBreakLine = false;
}

void CMDWriter::EnteredBlockquote()
{
	m_arStates.top().m_unLevelBlockquote++;
}

void CMDWriter::OutBlockquote()
{
	if (m_arStates.top().m_unLevelBlockquote > 0)
		m_arStates.top().m_unLevelBlockquote--;
}

UINT CMDWriter::GetLevelBlockquote()
{
	return m_arStates.top().m_unLevelBlockquote;
}

void CMDWriter::EnteredTable()
{
	m_arStates.top().m_bInTable = true;
}

void CMDWriter::OutTable()
{
	m_arStates.top().m_bInTable = false;
}

bool CMDWriter::InTable() const
{
	return m_arStates.top().m_bInTable;
}

void CMDWriter::EnteredPreformatted()
{
	m_arStates.top().m_bInPreformatted = true;
}

void CMDWriter::OutPreformatted()
{
	m_arStates.top().m_bInPreformatted = false;
}

bool CMDWriter::InPreformatted() const
{
	return m_arStates.top().m_bInPreformatted;
}

void CMDWriter::EnteredCode()
{
	m_arStates.top().m_bInCode = true;
}

void CMDWriter::OutCode()
{
	m_arStates.top().m_bInCode = false;
}

bool CMDWriter::InCode() const
{
	return m_arStates.top().m_bInCode;
}

void CMDWriter::EnteredList(bool bOrderedList)
{
	SaveState();
	m_arStates.top().m_bInList = true;
	m_arStates.top().m_bIsOrederedList = bOrderedList;
	m_arStates.top().m_unLevelList++;
}

void CMDWriter::OutList()
{
	RollBackState();
}

void CMDWriter::SetIndexOrderedList(UINT unIndex)
{
	m_arStates.top().m_unIndexListElement = unIndex;
}

void CMDWriter::IncreaseIndexOrderedList()
{
	m_arStates.top().m_unIndexListElement++;
}

bool CMDWriter::InOrederedList() const
{
	return m_arStates.top().m_bIsOrederedList;
}

UINT CMDWriter::GetIndexOrderedList() const
{
	return m_arStates.top().m_unIndexListElement;
}

UINT CMDWriter::GetLevelList() const
{
	return m_arStates.top().m_unLevelList;
}

void CMDWriter::SaveState()
{
	m_arStates.push(m_arStates.top());
}

void CMDWriter::RollBackState()
{
	if (m_arStates.size() > 1)
		m_arStates.pop();
}

void CMDWriter::ApplyAlternativeTags(const NSCSS::CCompiledStyle* pCompiledStyle, bool bIsCloseTag)
{
	if (nullptr == pCompiledStyle || !m_oMDParametrs.m_bUseAlternativeHTMLTags)
		return;

	if (pCompiledStyle->m_oText.Underline())
		GetCurrentDocument()->WriteString(bIsCloseTag ? L"</u>" : L"<u>");

	if (L"top" == pCompiledStyle->m_oDisplay.GetVAlign().ToWString())
		GetCurrentDocument()->WriteString(bIsCloseTag ? L"</sup>" : L"<sup>");
	else if (L"bottom" == pCompiledStyle->m_oDisplay.GetVAlign().ToWString())
		GetCurrentDocument()->WriteString(bIsCloseTag ? L"</sub>" : L"<sub>");

	if (L"FFFF00" == pCompiledStyle->m_oBackground.GetColor().ToHEX())
		GetCurrentDocument()->WriteString(bIsCloseTag ? L"</mark>" : L"<mark>");
}
}
