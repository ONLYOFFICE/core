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
	CloseCodeSpan();

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

static inline bool IsHtmlSpace(unsigned int unCode)
{
	return 0x20 == unCode || 0x09 == unCode || 0x0a == unCode || 0x0c == unCode || 0x0d == unCode;
}

static inline bool IsWhiteSpace(unsigned int unCode)
{
	return IsHtmlSpace(unCode) || 0x0b == unCode || 0xa0 == unCode || 0x1680 == unCode || (0x2000 <= unCode && 0x200a >= unCode) ||
	       0x202f == unCode || 0x205f == unCode || 0x3000 == unCode;
}

static inline bool IsAsciiPunctuation(unsigned int unCode)
{
	return (0x21 <= unCode && 0x2f >= unCode) || (0x3a <= unCode && 0x40 >= unCode) || (0x5b <= unCode && 0x60 >= unCode) ||
	       (0x7b <= unCode && 0x7e >= unCode);
}

static inline bool IsPunctuation(unsigned int unCode)
{
	return IsAsciiPunctuation(unCode) || (0xa1 <= unCode && 0xa9 >= unCode) || 0xab == unCode || 0xac == unCode ||
	       (0xae <= unCode && 0xb1 >= unCode) || 0xb4 == unCode || (0xb6 <= unCode && 0xb8 >= unCode) || 0xbb == unCode ||
	       0xbf == unCode || 0xd7 == unCode || 0xf7 == unCode || (0x2010 <= unCode && 0x2027 >= unCode) ||
	       (0x2030 <= unCode && 0x205e >= unCode) || (0x20a0 <= unCode && 0x20c0 >= unCode) || (0x2190 <= unCode && 0x2426 >= unCode) ||
	       (0x2500 <= unCode && 0x2775 >= unCode) || (0x2794 <= unCode && 0x2b73 >= unCode) || (0x2e00 <= unCode && 0x2e2e >= unCode) ||
	       (0x3001 <= unCode && 0x3004 >= unCode) || (0x3008 <= unCode && 0x3020 >= unCode) || (0xfe10 <= unCode && 0xfe19 >= unCode) ||
	       (0xfe30 <= unCode && 0xfe52 >= unCode) || (0xff01 <= unCode && 0xff0f >= unCode) || (0xff1a <= unCode && 0xff20 >= unCode) ||
	       (0xff3b <= unCode && 0xff40 >= unCode) || (0xff5b <= unCode && 0xff65 >= unCode) || (0x1f300 <= unCode && 0x1f6d7 >= unCode) ||
	       (0x1f900 <= unCode && 0x1fa53 >= unCode);
}

static inline bool IsLetterOrDigit(unsigned int unCode)
{
	return (0x30 <= unCode && 0x39 >= unCode) || (0x41 <= unCode && 0x5a >= unCode) || (0x61 <= unCode && 0x7a >= unCode) ||
	       (0xc0 <= unCode && 0x2c1 >= unCode && 0xd7 != unCode && 0xf7 != unCode) || 0x386 == unCode || (0x388 <= unCode && 0x3f5 >= unCode) ||
	       (0x400 <= unCode && 0x481 >= unCode) || (0x48a <= unCode && 0x52f >= unCode) || (0x5d0 <= unCode && 0x5ea >= unCode) ||
	       (0x620 <= unCode && 0x64a >= unCode) || (0x1e00 <= unCode && 0x1fbc >= unCode) || (0x3041 <= unCode && 0x3096 >= unCode) ||
	       (0x30a1 <= unCode && 0x30fa >= unCode) || (0x3400 <= unCode && 0x4dbf >= unCode) || (0x4e00 <= unCode && 0x9fff >= unCode) ||
	       (0xac00 <= unCode && 0xd7a3 >= unCode);
}

static inline bool IsWordCharacter(unsigned int unCode)
{
	return !IsWhiteSpace(unCode) && !IsPunctuation(unCode);
}

static inline size_t GetCodePointLength(const wchar_t* pText, size_t unSize, size_t unIndex)
{
	return (2 == sizeof(wchar_t) && unIndex + 1 < unSize &&
	        0xd800 <= static_cast<unsigned int>(pText[unIndex]) && 0xdbff >= static_cast<unsigned int>(pText[unIndex]) &&
	        0xdc00 <= static_cast<unsigned int>(pText[unIndex + 1]) && 0xdfff >= static_cast<unsigned int>(pText[unIndex + 1])) ? 2 : 1;
}

static inline unsigned int GetCodePoint(const wchar_t* pText, size_t unSize, size_t unIndex)
{
	if (2 == GetCodePointLength(pText, unSize, unIndex))
		return 0x10000 + ((static_cast<unsigned int>(pText[unIndex]) - 0xd800) << 10) + (static_cast<unsigned int>(pText[unIndex + 1]) - 0xdc00);

	return static_cast<unsigned int>(pText[unIndex]);
}

static inline size_t GetLastCodePointLength(const wchar_t* pText, size_t unSize)
{
	if (unSize > 1 && 2 == GetCodePointLength(pText, unSize, unSize - 2))
		return 2;

	return (0 == unSize) ? 0 : 1;
}

static inline unsigned int GetLastCodePoint(const wchar_t* pText, size_t unSize)
{
	return (0 == unSize) ? 0 : GetCodePoint(pText, unSize, unSize - GetLastCodePointLength(pText, unSize));
}

static inline std::wstring GetClosers(const std::vector<std::wstring>& arDelimiters, size_t unFirst)
{
	std::wstring wsClosers;

	for (size_t unIndex = arDelimiters.size(); unIndex > unFirst; --unIndex)
		wsClosers += arDelimiters[unIndex - 1];

	return wsClosers;
}

static inline bool IsLinkSyntax(const std::wstring& wsString)
{
	return !wsString.empty() && (L']' == wsString.front() || L'(' == wsString.front() || L')' == wsString.front() || 0 == wsString.compare(0, 2, L" \""));
}

static inline bool EndsWith(XmlString* pDocument, const std::wstring& wsValue)
{
	const size_t unSize{pDocument->GetCurSize()};

	return unSize >= wsValue.size() && 0 == wsValue.compare(0, wsValue.size(), pDocument->GetBuffer() + unSize - wsValue.size(), wsValue.size());
}

static inline bool EndsWithDelimiter(XmlString* pDocument, wchar_t wchDelimiter)
{
	const size_t unSize{pDocument->GetCurSize()};
	const wchar_t* pBuffer{pDocument->GetBuffer()};

	if (0 == unSize || wchDelimiter != pBuffer[unSize - 1])
		return false;

	size_t unBackslashes{0};

	while (unBackslashes + 1 < unSize && L'\\' == pBuffer[unSize - 2 - unBackslashes])
		++unBackslashes;

	return 0 == unBackslashes % 2;
}

static inline size_t GetCodeFenceLength(const std::wstring& wsContent)
{
	std::vector<bool> arRuns;
	size_t unRun{0};

	for (size_t unIndex = 0; unIndex <= wsContent.size(); ++unIndex)
	{
		if (unIndex < wsContent.size() && L'`' == wsContent[unIndex])
		{
			++unRun;
			continue;
		}

		if (0 == unRun)
			continue;

		if (arRuns.size() <= unRun)
			arRuns.resize(unRun + 1, false);

		arRuns[unRun] = true;
		unRun = 0;
	}

	size_t unLength{1};

	while (unLength < arRuns.size() && arRuns[unLength])
		++unLength;

	return unLength;
}

static inline std::wstring GetCodeSpan(const std::wstring& wsContent)
{
	const std::wstring wsFence(GetCodeFenceLength(wsContent), L'`');
	const std::wstring wsPadding{(L'`' == wsContent.front() || L'`' == wsContent.back() ||
	                              (L' ' == wsContent.front() && L' ' == wsContent.back() && std::wstring::npos != wsContent.find_first_not_of(L' '))) ? L" " : L""};

	return wsFence + wsPadding + wsContent + wsPadding + wsFence;
}

static inline std::vector<std::wstring> GetDelimiters(const NSCSS::CCompiledStyle* pCompiledStyle)
{
	std::vector<std::wstring> arDelimiters;

	if (nullptr == pCompiledStyle)
		return arDelimiters;

	if (pCompiledStyle->m_oFont.Bold())
		arDelimiters.push_back(L"**");

	if (pCompiledStyle->m_oFont.Italic())
		arDelimiters.push_back(L"*");

	if (pCompiledStyle->m_oText.LineThrough())
		arDelimiters.push_back(L"~~");

	return arDelimiters;
}

static inline void EscapeLastExclamationMark(XmlString* pDocument)
{
	const size_t unSize{pDocument->GetCurSize()};
	const wchar_t* pBuffer{pDocument->GetBuffer()};

	if (0 == unSize || L'!' != pBuffer[unSize - 1])
		return;

	size_t unBackslashes{0};

	while (unBackslashes + 1 < unSize && L'\\' == pBuffer[unSize - 2 - unBackslashes])
		++unBackslashes;

	if (0 != unBackslashes % 2)
		return;

	pDocument->SetCurSize(unSize - 1);
	pDocument->WriteString(L"\\!");
}

static inline bool IsCharacterReference(const std::wstring& wsText, size_t unIndex)
{
	size_t unPosition{unIndex + 1};

	const bool bNumeric{unPosition < wsText.size() && L'#' == wsText[unPosition]};
	bool bHexadecimal{false};

	if (bNumeric)
	{
		++unPosition;
		bHexadecimal = unPosition < wsText.size() && (L'x' == wsText[unPosition] || L'X' == wsText[unPosition]);

		if (bHexadecimal)
			++unPosition;
	}

	const size_t unNameBegin{unPosition};

	while (unPosition < wsText.size() && unPosition - unNameBegin < 32)
	{
		const unsigned int unCode{static_cast<unsigned int>(wsText[unPosition])};

		const bool bDigit{0x30 <= unCode && 0x39 >= unCode};
		const bool bHexadecimalLetter{(0x41 <= unCode && 0x46 >= unCode) || (0x61 <= unCode && 0x66 >= unCode)};
		const bool bLetter{(0x41 <= unCode && 0x5a >= unCode) || (0x61 <= unCode && 0x7a >= unCode)};

		if (!bDigit && !(bHexadecimal && bHexadecimalLetter) && !(!bNumeric && bLetter))
			break;

		++unPosition;
	}

	return unPosition > unNameBegin && unPosition < wsText.size() && L';' == wsText[unPosition];
}

static std::wstring EscapeSpecialCharacters(const std::wstring& wsText, bool bBlockStart, bool bAfterNumber, bool bInTable, size_t& unLastLength)
{
	std::wstring wsResult;
	wsResult.reserve(wsText.size() + 8);

	bool bLineStart{bBlockStart};
	bool bLastEscaped{false};
	size_t unListMarker{std::wstring::npos};

	if (bAfterNumber)
	{
		size_t unDigitsEnd{0};

		while (unDigitsEnd < wsText.size() && L'0' <= wsText[unDigitsEnd] && L'9' >= wsText[unDigitsEnd])
			++unDigitsEnd;

		if (unDigitsEnd < wsText.size() && (L'.' == wsText[unDigitsEnd] || L')' == wsText[unDigitsEnd]) &&
		    (unDigitsEnd + 1 == wsText.size() || IsWhiteSpace(wsText[unDigitsEnd + 1])))
			unListMarker = unDigitsEnd;
	}

	for (size_t unIndex = 0; unIndex < wsText.size(); ++unIndex)
	{
		const wchar_t wchValue{wsText[unIndex]};
		const wchar_t wchNext{(unIndex + 1 < wsText.size()) ? wsText[unIndex + 1] : L'\0'};

		bool bEscape{unListMarker == unIndex};

		if (bLineStart && L' ' != wchValue && L'\t' != wchValue)
		{
			bLineStart = false;

			if (L'>' == wchValue)
				bEscape = true;
			else if (L'#' == wchValue || L'-' == wchValue || L'+' == wchValue || L'=' == wchValue)
				bEscape = L'\0' == wchNext || wchValue == wchNext || IsWhiteSpace(wchNext);
			else if (L'0' <= wchValue && L'9' >= wchValue)
			{
				size_t unDigitsEnd{unIndex + 1};

				while (unDigitsEnd < wsText.size() && L'0' <= wsText[unDigitsEnd] && L'9' >= wsText[unDigitsEnd])
					++unDigitsEnd;

				if (unDigitsEnd - unIndex < 10 && unDigitsEnd < wsText.size() && (L'.' == wsText[unDigitsEnd] || L')' == wsText[unDigitsEnd]) &&
				    (unDigitsEnd + 1 == wsText.size() || IsWhiteSpace(wsText[unDigitsEnd + 1])))
					unListMarker = unDigitsEnd;
			}
		}

		switch (wchValue)
		{
			case L'*':
			case L'`':
			case L'~':
			case L'[':
			case L']':
			case L'$':
			{
				bEscape = true;
				break;
			}
			case L'\\':
			{
				bEscape = L'\0' == wchNext || IsAsciiPunctuation(wchNext);
				break;
			}
			case L'_':
			{
				bEscape = 0 == unIndex || !IsLetterOrDigit(wsText[unIndex - 1]) || !IsLetterOrDigit(wchNext);
				break;
			}
			case L'<':
			{
				bEscape = L'\0' == wchNext || L'/' == wchNext || L'!' == wchNext || L'?' == wchNext ||
				          (L'A' <= wchNext && L'Z' >= wchNext) || (L'a' <= wchNext && L'z' >= wchNext);
				break;
			}
			case L'&':
			{
				bEscape = L'\0' == wchNext || IsCharacterReference(wsText, unIndex);
				break;
			}
			case L'|':
			{
				bEscape = bInTable;
				break;
			}
			case L'\n':
			{
				bLineStart = true;
				break;
			}
			default:
				break;
		}

		if (bEscape)
			wsResult.push_back(L'\\');

		wsResult.push_back(wchValue);
		bLastEscaped = bEscape;
	}

	unLastLength = GetLastCodePointLength(wsText.c_str(), wsText.size()) + (bLastEscaped ? 1 : 0);

	return wsResult;
}

static inline void ReplaceSpaces(std::wstring& wsValue)
{
	// boost::wregex oRegex(L"\\s+");
	// wsValue = boost::regex_replace(wsValue, oRegex, L" ");

	std::wstring::const_iterator itBegin = std::find_if(wsValue.cbegin(), wsValue.cend(), IsHtmlSpace);
	std::wstring::const_iterator itEnd;

	while (wsValue.cend() != itBegin)
	{
		itEnd = std::find_if_not(itBegin, wsValue.cend(), IsHtmlSpace);

		wsValue.replace(itBegin, itEnd, L" ");

		itBegin = std::find_if(itBegin + 1, wsValue.cend(), IsHtmlSpace);
	}
}

bool CMDWriter::WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors)
{
	if (wsText.empty())
		return false;

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

	if (!bPreformatted && !InCode() && wsText.cend() == std::find_if_not(wsText.cbegin(), wsText.cend(), IsHtmlSpace))
	{
		if (!m_arStates.top().m_bAfterSpace)
			m_arStates.top().m_bPendingSpace = true;

		return false;
	}

	if (bPreformatted && !m_arStates.top().m_bEmptyLine && !m_arStates.top().m_bInTable && EndsWith(GetCurrentDocument(), L"```"))
	{
		for (std::vector<NSCSS::CNode>::const_reverse_iterator itElement{arSelectors.crbegin()}; itElement < arSelectors.crend(); ++itElement)
		{
			if (L"pre" == itElement->m_wsName || L"xmp" == itElement->m_wsName)
			{
				if (L"xmp" != itElement->m_wsName || (L'\n' != wsText.front() && L'\r' != wsText.front()))
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

	TState& oState{m_arStates.top()};
	TState::TTextRun& oTextRun{oState.m_oTextRun};
	XmlString* pDocument{GetCurrentDocument()};

	if ((InCode() && !InPreformatted()) || (InPreformatted() && InTable()))
	{
		WriteCode(wsText, bPreformatted, pCompiledStyle);
		return true;
	}

	if (InPreformatted())
	{
		FlushPendingSpace();
		WriteContent(wsText);

		oTextRun = TState::TTextRun{};
		oState.m_bAfterSpace = false;

		if (L'\n' == wsText.back())
			oState.m_bNeedBreakLine = false;

		return true;
	}

	if (!bPreformatted)
		ReplaceSpaces(wsText);

	const std::wstring::const_iterator itTextBegin{std::find_if_not(wsText.cbegin(), wsText.cend(), IsWhiteSpace)};
	const std::wstring::const_iterator itTextEnd{std::find_if_not(wsText.crbegin(), std::wstring::const_reverse_iterator(itTextBegin), IsWhiteSpace).base()};

	const std::wstring wsLeadingSpaces{wsText.cbegin(), itTextBegin}, wsTrailingSpaces{itTextEnd, wsText.cend()};
	std::wstring wsCore{itTextBegin, itTextEnd};

	const std::wstring wsOpenTags{GetAlternativeTags(pCompiledStyle, false)};

	if (wsCore.empty())
	{
		const bool bContinue{wsOpenTags.empty() && IsTextRunEnd()};
		const size_t unBegin{pDocument->GetCurSize()};

		if (!wsOpenTags.empty())
		{
			FlushPendingSpace();
			WriteContent(wsOpenTags + wsLeadingSpaces + GetAlternativeTags(pCompiledStyle, true));

			oTextRun = TState::TTextRun{};
			oState.m_bAfterSpace = false;
		}
		else
			WriteSpaces(wsLeadingSpaces, bPreformatted);

		if (bContinue)
		{
			oTextRun.m_wsTrailing += pDocument->GetSubData(unBegin);
			oTextRun.m_unEnd = pDocument->GetCurSize();
		}

		if (L'\n' == wsText.back())
			oState.m_bNeedBreakLine = false;

		return true;
	}

	WriteRun(wsLeadingSpaces, wsCore, wsTrailingSpaces, GetDelimiters(pCompiledStyle), wsOpenTags, GetAlternativeTags(pCompiledStyle, true), bPreformatted, false);

	if (L'\n' == wsText.back())
		oState.m_bNeedBreakLine = false;

	return true;
}

void CMDWriter::WriteRun(const std::wstring& wsLeadingSpaces, std::wstring wsCore, const std::wstring& wsTrailingSpaces, const std::vector<std::wstring>& arDelimiters,
                         const std::wstring& wsOpenTags, const std::wstring& wsCloseTags, bool bPreformatted, bool bCode)
{
	TState& oState{m_arStates.top()};
	TState::TTextRun& oTextRun{oState.m_oTextRun};
	XmlString* pDocument{GetCurrentDocument()};

	const bool bContinue{wsOpenTags.empty() && IsTextRunEnd()};

	size_t unCommon{0};

	while (bContinue && unCommon < arDelimiters.size() && unCommon < oTextRun.m_arDelimiters.size() &&
	       arDelimiters[unCommon] == oTextRun.m_arDelimiters[unCommon])
		++unCommon;

	std::wstring wsClosed{bContinue ? GetClosers(oTextRun.m_arDelimiters, unCommon) : std::wstring()};

	if (bCode && bContinue && oTextRun.m_bCode && unCommon == arDelimiters.size() && unCommon == oTextRun.m_arDelimiters.size() &&
	    oTextRun.m_wsTrailing.empty() && !oState.m_bPendingSpace && wsLeadingSpaces.empty())
	{
		pDocument->SetCurSize(oTextRun.m_unCodeBegin);
		wsCore = oTextRun.m_wsCodeContent + wsCore;
		wsClosed.clear();
	}
	else if (0 != unCommon)
	{
		pDocument->SetCurSize(oTextRun.m_unTextEnd);
		pDocument->WriteString(wsClosed + oTextRun.m_wsTrailing);
		oTextRun.m_unEnd = pDocument->GetCurSize();
	}

	const std::wstring wsCodeContent{bCode ? wsCore : std::wstring()};

	if (bCode)
		wsCore = GetCodeSpan(wsCore);

	bool bClosersAtEnd{!wsClosed.empty() && oTextRun.m_wsTrailing.empty()};

	const size_t unLeadBegin{pDocument->GetCurSize()};

	WriteSpaces(wsLeadingSpaces, bPreformatted);
	FlushPendingSpace();

	if (!wsOpenTags.empty())
		WriteContent(wsOpenTags, true);

	if (pDocument->GetCurSize() != unLeadBegin)
		bClosersAtEnd = false;

	std::vector<std::wstring> arOpened{arDelimiters.cbegin() + unCommon, arDelimiters.cend()};
	unsigned int unLastCode{0};
	size_t unLastLength{0};
	size_t unCodeBegin{0};

	while (!wsCore.empty())
	{
		const unsigned int unFirstCode{GetCodePoint(wsCore.c_str(), wsCore.size(), 0)};
		const bool bFirstPunctuation{IsPunctuation(unFirstCode)};
		const bool bOpenStrike{!arOpened.empty() && L"~~" == arOpened.back()};
		const bool bOpenEmphasis{!arOpened.empty() && L'*' == arOpened.front().front()};

		bool bShrink{false};
		bool bRemoveStrike{false};

		if (bClosersAtEnd)
		{
			const bool bClosedStrike{oTextRun.m_arDelimiters.size() > unCommon && L"~~" == oTextRun.m_arDelimiters.back()};
			const bool bLastPunctuation{IsPunctuation(oTextRun.m_unLastCode)};

			bool bFixPrevious{false};

			if (arOpened.empty())
				bFixPrevious = !bFirstPunctuation && (bClosedStrike || bLastPunctuation);
			else if (bOpenEmphasis && L'*' == wsClosed.back())
			{
				const bool bLeftPunctuation{bClosedStrike || bLastPunctuation};
				const bool bRightPunctuation{bOpenStrike || bFirstPunctuation};

				bFixPrevious = bLeftPunctuation && !bRightPunctuation;
				bShrink = !bLeftPunctuation && bRightPunctuation && bFirstPunctuation;
				bRemoveStrike = !bLeftPunctuation && bRightPunctuation && !bFirstPunctuation;
			}

			if (bFixPrevious)
			{
				if (bLastPunctuation)
					ShrinkTextRun(unCommon);
				else
					RemoveStrikethrough(unCommon);

				wsClosed = GetClosers(oTextRun.m_arDelimiters, unCommon);
				bClosersAtEnd = !wsClosed.empty() && oTextRun.m_wsTrailing.empty();
				continue;
			}
		}
		else if (!arOpened.empty())
		{
			const unsigned int unPreviousCode{GetLastCodePoint(pDocument->GetBuffer(), pDocument->GetCurSize())};

			if (0 != unPreviousCode && IsWordCharacter(unPreviousCode) && (!bOpenEmphasis || bOpenStrike || bFirstPunctuation))
			{
				bShrink = bFirstPunctuation;
				bRemoveStrike = !bFirstPunctuation;
			}
		}

		if (bRemoveStrike)
		{
			arOpened.pop_back();
			continue;
		}

		if (!bShrink)
			break;

		const size_t unFirstLength{bCode ? wsCore.size() : GetCodePointLength(wsCore.c_str(), wsCore.size(), 0)};

		if (bCode)
		{
			unCodeBegin = pDocument->GetCurSize();
			unLastLength = wsCore.size();
			WriteContent(wsCore);
		}
		else
			WriteContent(EscapeSpecialCharacters(wsCore.substr(0, unFirstLength), AtBlockStart(false), AtBlockStart(true), InTable(), unLastLength));

		unLastCode = unFirstCode;
		oState.m_bAfterSpace = false;
		bClosersAtEnd = false;

		const size_t unRestBegin{static_cast<size_t>(std::find_if_not(wsCore.cbegin() + unFirstLength, wsCore.cend(), IsWhiteSpace) - wsCore.cbegin())};

		WriteSpaces(wsCore.substr(unFirstLength, unRestBegin - unFirstLength), bPreformatted);
		wsCore.erase(0, unRestBegin);

		if (wsCore.empty())
			arOpened.clear();
	}

	std::vector<size_t> arOpeners{oTextRun.m_arOpeners.cbegin(), oTextRun.m_arOpeners.cbegin() + unCommon};

	if (!wsCore.empty())
	{
		FlushPendingSpace();

		if ((!bClosersAtEnd && !arOpened.empty() && EndsWithDelimiter(pDocument, arOpened.front().front())) ||
		    (bCode && arOpened.empty() && EndsWithDelimiter(pDocument, L'`')))
		{
			WriteContent(L" ");
			oState.m_bAfterSpace = true;
		}

		for (const std::wstring& wsDelimiter : arOpened)
		{
			arOpeners.push_back(pDocument->GetCurSize());
			WriteContent(wsDelimiter, true);
		}

		if (bCode)
		{
			unCodeBegin = pDocument->GetCurSize();
			unLastLength = wsCore.size();
			WriteContent(wsCore);
		}
		else
		{
			const bool bNumberContinuation{(L'0' <= wsCore.front() && L'9' >= wsCore.front()) || L'.' == wsCore.front() || L')' == wsCore.front()};

			WriteContent(EscapeSpecialCharacters(wsCore, AtBlockStart(false), bNumberContinuation && AtBlockStart(true), InTable(), unLastLength));
		}

		unLastCode = GetLastCodePoint(wsCore.c_str(), wsCore.size());
		oState.m_bAfterSpace = false;
	}

	const size_t unTextEnd{pDocument->GetCurSize()};

	std::vector<std::wstring> arCurrent{arDelimiters.cbegin(), arDelimiters.cbegin() + unCommon};
	arCurrent.insert(arCurrent.end(), arOpened.cbegin(), arOpened.cend());

	WriteContent(GetClosers(arCurrent, 0), true);

	if (!wsOpenTags.empty())
		WriteContent(wsCloseTags, true);

	const size_t unClosersEnd{pDocument->GetCurSize()};

	WriteSpaces(wsTrailingSpaces, bPreformatted);

	if (wsOpenTags.empty())
	{
		oTextRun.m_pDocument = pDocument;
		oTextRun.m_unTextEnd = unTextEnd;
		oTextRun.m_unEnd = pDocument->GetCurSize();
		oTextRun.m_arDelimiters = std::move(arCurrent);
		oTextRun.m_arOpeners = std::move(arOpeners);
		oTextRun.m_wsTrailing = pDocument->GetSubData(unClosersEnd);
		oTextRun.m_unLastCode = unLastCode;
		oTextRun.m_unLastLength = unLastLength;
		oTextRun.m_bCode = bCode;
		oTextRun.m_unCodeBegin = unCodeBegin;
		oTextRun.m_wsCodeContent = wsCodeContent;
	}
	else
		oTextRun = TState::TTextRun{};
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
	m_arStates.top().m_oTextRun = TState::TTextRun{};
	m_arStates.top().m_oCodeSpan = TState::TCodeSpan{};
	m_arStates.top().m_bPendingSpace = false;
	m_arStates.top().m_bAfterSpace = true;
}

void CMDWriter::RevertDataOutput()
{
	CloseCodeSpan();
	RollBackState();
}

TMarkdownParameters CMDWriter::GetParametrs() const
{
	return m_oMDParametrs;
}

void CMDWriter::WriteString(const std::wstring& wsString, bool bSpecialString)
{
	CloseCodeSpan();

	const bool bLinkSyntax{IsLinkSyntax(wsString)};

	if (!wsString.empty() && !bLinkSyntax)
	{
		if (IsWhiteSpace(wsString.front()))
			m_arStates.top().m_bPendingSpace = false;
		else
			FlushPendingSpace();
	}

	if (L"[" == wsString)
		EscapeLastExclamationMark(GetCurrentDocument());

	WriteContent(wsString, bSpecialString);

	if (!wsString.empty() && !bLinkSyntax && L"[" != wsString)
		m_arStates.top().m_bAfterSpace = IsWhiteSpace(wsString.back());
}

void CMDWriter::WriteOpenSpecialString(const std::wstring& wsString)
{
	CloseCodeSpan();

	if (!wsString.empty() && !IsWhiteSpace(wsString.front()))
		FlushPendingSpace();

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
	CloseCodeSpan();

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

	CloseCodeSpan();

	m_arStates.top().m_oSpecialString = TState::TSpecialString{};
	m_arStates.top().m_oTextRun = TState::TTextRun{};
	m_arStates.top().m_oCodeSpan = TState::TCodeSpan{};
	m_arStates.top().m_bPendingSpace = false;

	if (m_arStates.top().m_bInTable && bNeedChecked)
	{
		if (m_oMDParametrs.m_bUseAlternativeHTMLTags)
			WriteString(L"</br>");
		else
			WriteString(L" ");

		m_arStates.top().m_bNeedBreakLine = false;
		m_arStates.top().m_bAfterSpace = true;
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
	m_arStates.top().m_bAfterSpace = true;
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
	CloseCodeSpan();
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
	CloseCodeSpan();
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
	const TState oListState{m_arStates.top()};

	RollBackState();

	TState& oState{m_arStates.top()};

	if (oListState.m_pCurrentDocument != oState.m_pCurrentDocument)
		return;

	oState.m_bNeedBreakLine = oListState.m_bNeedBreakLine;
	oState.m_bEmptyLine     = oListState.m_bEmptyLine;
	oState.m_oSpecialString = oListState.m_oSpecialString;
	oState.m_oTextRun       = oListState.m_oTextRun;
	oState.m_oCodeSpan      = oListState.m_oCodeSpan;
	oState.m_bPendingSpace  = oListState.m_bPendingSpace;
	oState.m_bAfterSpace    = oListState.m_bAfterSpace;
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

void CMDWriter::WriteContent(const std::wstring& wsString, bool bSpecialString)
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

void CMDWriter::WriteSpaces(const std::wstring& wsSpaces, bool bPreserve)
{
	if (wsSpaces.empty())
		return;

	if (bPreserve)
	{
		FlushPendingSpace();
		WriteContent(wsSpaces);
		m_arStates.top().m_bAfterSpace = false;
		return;
	}

	for (const wchar_t wchSpace : wsSpaces)
	{
		if (L' ' == wchSpace)
		{
			if (!m_arStates.top().m_bAfterSpace)
				m_arStates.top().m_bPendingSpace = true;

			continue;
		}

		FlushPendingSpace();
		WriteContent(std::wstring(1, wchSpace));
		m_arStates.top().m_bAfterSpace = false;
	}
}

void CMDWriter::FlushPendingSpace()
{
	if (!m_arStates.top().m_bPendingSpace)
		return;

	m_arStates.top().m_bPendingSpace = false;
	WriteContent(L" ");
	m_arStates.top().m_bAfterSpace = true;
}

bool CMDWriter::IsTextRunEnd() const
{
	const TState::TTextRun& oTextRun{m_arStates.top().m_oTextRun};

	return nullptr != oTextRun.m_pDocument && GetCurrentDocument() == oTextRun.m_pDocument &&
	       oTextRun.m_unEnd == oTextRun.m_pDocument->GetCurSize();
}

bool CMDWriter::AtBlockStart(bool bAfterNumber) const
{
	XmlString* pDocument{GetCurrentDocument()};
	const wchar_t* pBuffer{pDocument->GetBuffer()};

	size_t unEnd{pDocument->GetCurSize()};

	if (bAfterNumber)
	{
		const size_t unNumberEnd{unEnd};

		while (unEnd > 0 && L'0' <= pBuffer[unEnd - 1] && L'9' >= pBuffer[unEnd - 1])
			--unEnd;

		if (unEnd == unNumberEnd || unNumberEnd - unEnd > 9)
			return false;
	}

	size_t unPosition{unEnd};

	while (unPosition > 0 && L'\n' != pBuffer[unPosition - 1])
	{
		const wchar_t wchValue{pBuffer[unPosition - 1]};

		if (L' ' != wchValue && L'>' != wchValue && L'-' != wchValue && L'+' != wchValue && L'*' != wchValue &&
		    L'.' != wchValue && L')' != wchValue && (L'0' > wchValue || L'9' < wchValue))
			return false;

		--unPosition;
	}

	while (unPosition < unEnd)
	{
		const wchar_t wchValue{pBuffer[unPosition]};

		if (L' ' == wchValue || L'>' == wchValue)
		{
			++unPosition;
			continue;
		}

		if ((L'-' == wchValue || L'+' == wchValue || L'*' == wchValue) && unPosition + 1 < unEnd && L' ' == pBuffer[unPosition + 1])
		{
			unPosition += 2;
			continue;
		}

		size_t unDigitsEnd{unPosition};

		while (unDigitsEnd < unEnd && L'0' <= pBuffer[unDigitsEnd] && L'9' >= pBuffer[unDigitsEnd])
			++unDigitsEnd;

		if (unDigitsEnd == unPosition || unDigitsEnd + 1 >= unEnd || (L'.' != pBuffer[unDigitsEnd] && L')' != pBuffer[unDigitsEnd]) ||
		    L' ' != pBuffer[unDigitsEnd + 1])
			return false;

		unPosition = unDigitsEnd + 2;
	}

	return true;
}

void CMDWriter::ShrinkTextRun(size_t unFirstClosed)
{
	TState::TTextRun& oTextRun{m_arStates.top().m_oTextRun};
	XmlString* pDocument{oTextRun.m_pDocument};
	const wchar_t* pBuffer{pDocument->GetBuffer()};

	size_t unTextEnd{oTextRun.m_unTextEnd - oTextRun.m_unLastLength};
	size_t unDelimiter{oTextRun.m_arDelimiters.size()};

	while (unDelimiter > unFirstClosed)
	{
		while (unTextEnd > 0 && IsWhiteSpace(pBuffer[unTextEnd - 1]))
			--unTextEnd;

		if (oTextRun.m_arOpeners[unDelimiter - 1] + oTextRun.m_arDelimiters[unDelimiter - 1].size() < unTextEnd)
			break;

		--unDelimiter;
		unTextEnd = oTextRun.m_arOpeners[unDelimiter];
	}

	std::wstring wsTail;
	size_t unPosition{unTextEnd};

	for (size_t unIndex = unDelimiter; unIndex < oTextRun.m_arDelimiters.size(); ++unIndex)
	{
		wsTail.append(pBuffer + unPosition, oTextRun.m_arOpeners[unIndex] - unPosition);
		unPosition = oTextRun.m_arOpeners[unIndex] + oTextRun.m_arDelimiters[unIndex].size();
	}

	wsTail.append(pBuffer + unPosition, oTextRun.m_unTextEnd - unPosition);

	oTextRun.m_arDelimiters.resize(unDelimiter);
	oTextRun.m_arOpeners.resize(unDelimiter);

	const std::wstring wsClosers{GetClosers(oTextRun.m_arDelimiters, unFirstClosed)};

	size_t unBackslashes{0};

	while (unBackslashes < unTextEnd && L'\\' == pBuffer[unTextEnd - 1 - unBackslashes])
		++unBackslashes;

	pDocument->SetCurSize(unTextEnd);

	if (!wsClosers.empty() && 1 == unBackslashes % 2)
		pDocument->AddCharSafe(L'\\');

	oTextRun.m_unTextEnd = pDocument->GetCurSize();

	pDocument->WriteString(wsClosers + wsTail);

	oTextRun.m_wsTrailing = wsTail;
	oTextRun.m_unEnd = pDocument->GetCurSize();
}

void CMDWriter::RemoveStrikethrough(size_t unFirstClosed)
{
	TState::TTextRun& oTextRun{m_arStates.top().m_oTextRun};
	XmlString* pDocument{oTextRun.m_pDocument};

	const size_t unOpener{oTextRun.m_arOpeners.back()};
	const size_t unTextBegin{unOpener + oTextRun.m_arDelimiters.back().size()};
	const std::wstring wsText{pDocument->GetSubData(unTextBegin, oTextRun.m_unTextEnd - unTextBegin)};

	oTextRun.m_arDelimiters.pop_back();
	oTextRun.m_arOpeners.pop_back();

	pDocument->SetCurSize(unOpener);
	pDocument->WriteString(wsText);

	oTextRun.m_unTextEnd = pDocument->GetCurSize();

	pDocument->WriteString(GetClosers(oTextRun.m_arDelimiters, unFirstClosed) + oTextRun.m_wsTrailing);

	oTextRun.m_unEnd = pDocument->GetCurSize();
}

void CMDWriter::WriteCode(std::wstring wsText, bool bPreformatted, const NSCSS::CCompiledStyle* pCompiledStyle)
{
	if (!bPreformatted)
		ReplaceSpaces(wsText);

	const std::vector<std::wstring> arDelimiters{GetDelimiters(pCompiledStyle)};
	const std::wstring wsOpenTags{GetAlternativeTags(pCompiledStyle, false)};
	const std::wstring wsCloseTags{GetAlternativeTags(pCompiledStyle, true)};

	size_t unLineBegin{0};

	while (true)
	{
		const size_t unLineEnd{wsText.find_first_of(L"\r\n", unLineBegin)};
		const std::wstring wsLine{wsText.substr(unLineBegin, (std::wstring::npos == unLineEnd) ? std::wstring::npos : unLineEnd - unLineBegin)};

		TState::TCodeSpan& oCodeSpan{m_arStates.top().m_oCodeSpan};

		if (bPreformatted)
		{
			if (!wsLine.empty())
			{
				OpenCodeSpan(arDelimiters, wsOpenTags, wsCloseTags);
				WriteContent(wsLine);
				m_arStates.top().m_bAfterSpace = false;
			}
		}
		else
		{
			const size_t unCoreBegin{wsLine.find_first_not_of(L' ')};
			const size_t unCoreEnd{(std::wstring::npos == unCoreBegin) ? std::wstring::npos : wsLine.find_last_not_of(L' ') + 1};

			if (0 != unCoreBegin && !wsLine.empty())
			{
				if (oCodeSpan.m_bOpened)
					oCodeSpan.m_bSpace = true;
				else
					WriteSpaces(L" ", false);
			}

			if (std::wstring::npos != unCoreBegin)
			{
				OpenCodeSpan(arDelimiters, wsOpenTags, wsCloseTags);

				if (oCodeSpan.m_bSpace)
				{
					WriteContent(L" ");
					oCodeSpan.m_bSpace = false;
				}

				WriteContent(wsLine.substr(unCoreBegin, unCoreEnd - unCoreBegin));
				m_arStates.top().m_bAfterSpace = false;

				if (unCoreEnd < wsLine.size())
					oCodeSpan.m_bSpace = true;
			}
		}

		if (std::wstring::npos == unLineEnd)
			break;

		CloseCodeSpan();
		WriteBreakLine();

		unLineBegin = unLineEnd + ((L'\r' == wsText[unLineEnd] && unLineEnd + 1 < wsText.size() && L'\n' == wsText[unLineEnd + 1]) ? 2 : 1);
	}
}

void CMDWriter::OpenCodeSpan(const std::vector<std::wstring>& arDelimiters, const std::wstring& wsOpenTags, const std::wstring& wsCloseTags)
{
	TState::TCodeSpan& oCodeSpan{m_arStates.top().m_oCodeSpan};

	if (oCodeSpan.m_bOpened)
	{
		if (oCodeSpan.m_arDelimiters == arDelimiters && oCodeSpan.m_wsOpenTags == wsOpenTags)
			return;

		CloseCodeSpan();
	}

	XmlString* pDocument{GetCurrentDocument()};

	oCodeSpan.m_pDocument = pDocument;
	oCodeSpan.m_unBegin = pDocument->GetCurSize();
	oCodeSpan.m_arDelimiters = arDelimiters;
	oCodeSpan.m_wsOpenTags = wsOpenTags;
	oCodeSpan.m_wsCloseTags = wsCloseTags;
	oCodeSpan.m_bOpened = true;

	WriteContent(L"`", true);
}

void CMDWriter::CloseCodeSpan()
{
	TState& oState{m_arStates.top()};
	TState::TCodeSpan& oCodeSpan{oState.m_oCodeSpan};

	if (oCodeSpan.m_bOpened)
	{
		XmlString* pDocument{oCodeSpan.m_pDocument};
		const std::wstring wsContent{pDocument->GetSubData(oCodeSpan.m_unBegin + 1)};

		pDocument->SetCurSize(oCodeSpan.m_unBegin);
		oCodeSpan.m_bOpened = false;

		if (!wsContent.empty())
			WriteRun(std::wstring(), wsContent, std::wstring(), oCodeSpan.m_arDelimiters, oCodeSpan.m_wsOpenTags, oCodeSpan.m_wsCloseTags, false, true);
	}

	if (oCodeSpan.m_bSpace)
	{
		oCodeSpan.m_bSpace = false;

		if (!oState.m_bAfterSpace)
			oState.m_bPendingSpace = true;
	}
}

std::wstring CMDWriter::GetAlternativeTags(const NSCSS::CCompiledStyle* pCompiledStyle, bool bIsCloseTag) const
{
	std::wstring wsTags;

	if (nullptr == pCompiledStyle || !m_oMDParametrs.m_bUseAlternativeHTMLTags)
		return wsTags;

	std::vector<std::wstring> arTags;

	if (pCompiledStyle->m_oText.Underline())
		arTags.push_back(L"u");

	if (L"top" == pCompiledStyle->m_oDisplay.GetVAlign().ToWString())
		arTags.push_back(L"sup");
	else if (L"bottom" == pCompiledStyle->m_oDisplay.GetVAlign().ToWString())
		arTags.push_back(L"sub");

	if (L"FFFF00" == pCompiledStyle->m_oBackground.GetColor().ToHEX())
		arTags.push_back(L"mark");

	for (const std::wstring& wsTag : arTags)
		wsTags = bIsCloseTag ? L"</" + wsTag + L'>' + wsTags : wsTags + L'<' + wsTag + L'>';

	return wsTags;
}
}
