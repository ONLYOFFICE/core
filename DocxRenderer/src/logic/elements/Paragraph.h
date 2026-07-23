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

#pragma once
#include "BaseItem.h"
#include "TextLine.h"

namespace NSDocxRenderer
{
	class CParagraph : public CBaseItem, public IOoxmlItem
	{
	public:
		enum TextAlignmentType
		{
			tatUnknown,
			tatByLeft,
			tatByCenter,
			tatByRight,
			tatByWidth
		};

		// text frame properties
		bool                m_bIsNeedFirstLineIndent{false};
		bool                m_bIsShadingPresent     {false};
		LONG                m_lColorOfShadingFill   {c_iWhiteColor}; //BGR
		TextAlignmentType   m_eTextAlignmentType    {tatUnknown};

		// geometry paragraph
		double m_dLeftBorder {0.0}; // offset from left edge of page/shape/table
		double m_dRightBorder{0.0}; // offset from right edge of page/shape/table
		double m_dFirstLine  {0.0}; // offset relative to m_dLeftBorder

		double m_dSpaceBefore{0.0}; // default is 0 if w:before is absent
		double m_dSpaceAfter {0.0}; // in shape default is 8pt if w:after is absent
		double m_dLineHeight {0.0};

		std::vector<std::shared_ptr<CTextLine>> m_arTextLines;
		std::wstring m_wsStyleId;

	public:
		CParagraph() : CBaseItem() {}
		virtual ~CParagraph();
		virtual void Clear();
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToBin(NSWasm::CData& oWriter) const override final;

		void RemoveHighlightColor();
		void MergeLines();
	};
}
