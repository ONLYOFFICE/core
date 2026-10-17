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

#ifndef MDWRITER_H
#define MDWRITER_H

#include "IWriter.h"
#include "../MarkdownParameters.h"

#include <stack>

namespace HTML
{
class CMDWriter : public IWriter
{
	XmlString m_oDocument;

	struct TState
	{
		XmlString *m_pCurrentDocument{nullptr};

		bool m_bNeedBreakLine{false};
		bool m_bEmptyLine{true};

		UINT m_unLevelBlockquote{0};

		bool m_bInTable{false};
		bool m_bInPreformatted{false};
		bool m_bInCode{false};

		bool m_bInList{false};
		bool m_bIsOrederedList{false};
		UINT m_unLevelList{0};
		UINT m_unIndexListElement{1};

		struct TSpecialString
		{
			std::wstring m_wsLastSpecialString;
			bool m_bClosed{false};
		}m_oSpecialString;

		struct TTextRun
		{
			XmlString *m_pDocument{nullptr};
			size_t m_unTextEnd{0};
			size_t m_unEnd{0};
			std::vector<std::wstring> m_arDelimiters;
			std::vector<size_t> m_arOpeners;
			std::wstring m_wsTrailing;
			unsigned int m_unLastCode{0};
			size_t m_unLastLength{0};
			bool m_bCode{false};
			size_t m_unCodeBegin{0};
			std::wstring m_wsCodeContent;
		}m_oTextRun;

		struct TCodeSpan
		{
			XmlString *m_pDocument{nullptr};
			size_t m_unBegin{0};
			std::vector<std::wstring> m_arDelimiters;
			std::wstring m_wsOpenTags;
			std::wstring m_wsCloseTags;
			bool m_bOpened{false};
			bool m_bSpace{false};
		}m_oCodeSpan;

		bool m_bPendingSpace{false};
		bool m_bAfterSpace{true};
	};

	std::stack<TState> m_arStates;
	TMarkdownParameters m_oMDParametrs;

	XmlString m_oMetadata;
public:
	CMDWriter(const TMarkdownParameters& oMDParametrs);

	void Begin(const std::wstring& wsDst) override;
	void End(const std::wstring& wsDst)  override;

	bool WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors) override;

	void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) override;

	void WriteMetadata(const std::wstring& wsName, const std::wstring& wsValue) override;

	void PageBreak() override;

	void BeginBlock() override;
	void EndBlock(bool bAddBlock) override;

	void SetDataOutput(XmlString* pOutputData) override;
	void RevertDataOutput() override;

	TMarkdownParameters GetParametrs() const;

	void WriteString(const std::wstring& wsString, bool bSpecialString = false);
	void WriteOpenSpecialString(const std::wstring& wsString);
	void WriteCloseSpecialString(const std::wstring& wsString);

	XmlString* GetCurrentDocument() const override;

	void WriteBreakLine(bool bNeedChecked = true);

	void EnteredBlockquote();
	void OutBlockquote();
	UINT GetLevelBlockquote();

	void EnteredTable();
	void OutTable();
	bool InTable() const;

	void EnteredPreformatted();
	void OutPreformatted();
	bool InPreformatted() const;

	void EnteredCode();
	void OutCode();
	bool InCode() const;

	void EnteredList(bool bOrderedList);
	void OutList();
	void SetIndexOrderedList(UINT unIndex);
	void IncreaseIndexOrderedList();

	bool InOrederedList() const;
	UINT GetIndexOrderedList() const;
	UINT GetLevelList() const;

	void SetLinePrefix(const std::wstring& wsPrefix);
	void ClearLinePrefix();
private:
	void SaveState();
	void RollBackState();

	void WriteContent(const std::wstring& wsString, bool bSpecialString = false);
	void WriteSpaces(const std::wstring& wsSpaces, bool bPreserve);
	void FlushPendingSpace();

	bool IsTextRunEnd() const;
	bool AtBlockStart(bool bAfterNumber) const;

	void ShrinkTextRun(size_t unFirstClosed);
	void RemoveStrikethrough(size_t unFirstClosed);

	void WriteRun(const std::wstring& wsLeadingSpaces, std::wstring wsCore, const std::wstring& wsTrailingSpaces, const std::vector<std::wstring>& arDelimiters,
	              const std::wstring& wsOpenTags, const std::wstring& wsCloseTags, bool bPreformatted, bool bCode);

	void WriteCode(std::wstring wsText, bool bPreformatted, const NSCSS::CCompiledStyle* pCompiledStyle);
	void OpenCodeSpan(const std::vector<std::wstring>& arDelimiters, const std::wstring& wsOpenTags, const std::wstring& wsCloseTags);
	void CloseCodeSpan();

	std::wstring GetAlternativeTags(const NSCSS::CCompiledStyle* pCompiledStyle, bool bIsCloseTag) const;
};
}

#endif // MDWRITER_H
