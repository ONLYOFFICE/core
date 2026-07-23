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
#include "ContText.h"
#include "BaseItem.h"

namespace NSDocxRenderer
{
	class CTextLine : public CBaseItem, public IOoxmlItem
	{
	public:
		enum AssumedTextAlignmentType
		{
			atatUnknown,
			atatByLeftEdge,
			atatByCenter,
			atatByRightEdge,
			atatByWidth
		};

		std::vector<std::shared_ptr<CContText>> m_arConts;

		AssumedTextAlignmentType m_eAlignmentType{atatUnknown};
		eVertAlignType m_eVertAlignType          {eVertAlignType::vatUnknown};

		std::shared_ptr<CTextLine> m_pLine;
		std::shared_ptr<CShape>  m_pDominantShape {nullptr};

		UINT m_iNumDuplicates {0};

		double m_dTopWithMaxAscent{0};
		double m_dBotWithMaxDescent{0};

		double m_dFirstWordWidth{0.0};

		bool m_bIsPossibleVerSplit = false;

	public:
		CTextLine() = default;
		virtual ~CTextLine();
		virtual void Clear();
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToBin(NSWasm::CData& oWriter) const override final;
		virtual void RecalcWithNewItem(const CContText* pCont);
		virtual eVerticalCrossingType GetVerticalCrossingType(const CTextLine* pLine) const noexcept;

		void AddCont(const std::shared_ptr<CContText>& pCont);
		void AddConts(const std::vector<std::shared_ptr<CContText>>& arConts);
		void MergeConts();
		void CalcFirstWordWidth();
		void RecalcSizes();
		void SetVertAlignType(const eVertAlignType& oType);

		bool IsShadingPresent(const CTextLine* pLine) const noexcept;
		bool IsCanBeDeleted() const;

		double GetLeftNoEnum() const noexcept;

		size_t GetLength() const;
		void GetNextSym(size_t& nContPos, size_t& nSymPos) const noexcept;
	};
}
