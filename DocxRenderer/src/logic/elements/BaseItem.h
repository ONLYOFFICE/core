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

#include <memory>
#include <vector>

#include "../../../../DesktopEditor/common/StringBuilder.h"
#include "../../../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

namespace NSDocxRenderer
{
	// Vertical position relative to the next object
	enum class eVerticalCrossingType
	{
		vctUnknown,
		vctCurrentInsideNext,
		vctCurrentOutsideNext,
		vctCurrentAboveNext,
		vctCurrentBelowNext,
		vctDublicate,
		vctTopAndBottomBordersMatch,
		vctTopBorderMatch,
		vctBottomBorderMatch,
		vctNoCrossingCurrentAboveNext,
		vctNoCrossingCurrentBelowNext
	};

	// Horizontal position relative to the next object
	enum class eHorizontalCrossingType
	{
		hctUnknown,
		hctCurrentInsideNext,
		hctCurrentOutsideNext,
		hctCurrentLeftOfNext,
		hctCurrentRightOfNext,
		hctDublicate,
		hctLeftAndRightBordersMatch,
		hctLeftBorderMatch,
		hctRightBorderMatch,
		hctNoCrossingCurrentLeftOfNext,
		hctNoCrossingCurrentRightOfNext
	};

	class CBaseItem
	{
	public:
		double m_dTop    {0.0};
		double m_dBot    {0.0};
		double m_dLeft   {0.0};
		double m_dRight  {0.0};
		double m_dHeight {0.0};
		double m_dWidth  {0.0};

		size_t m_nOrder = 0;

		CBaseItem();
		CBaseItem(const CBaseItem& other);
		CBaseItem(CBaseItem&& other);
		virtual ~CBaseItem();

		CBaseItem& operator=(const CBaseItem& other);
		CBaseItem& operator=(CBaseItem&& other);

		virtual eVerticalCrossingType GetVerticalCrossingType(const CBaseItem* pBaseItem) const;
		virtual eHorizontalCrossingType GetHorizontalCrossingType(const CBaseItem* pBaseItem) const;
		virtual void RecalcWithNewItem(const CBaseItem* pBaseItem);

		bool AreObjectsNoCrossingByVertically(const CBaseItem* pBaseItem) const noexcept;
		bool AreObjectsNoCrossingByHorizontally(const CBaseItem* pBaseItem) const noexcept;
		bool IsEqual(double dTop, double dBot, double dLeft, double dRight) const noexcept;

		bool operator==(const CBaseItem& other);
	};

	class IOoxmlItem
	{
	public:
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const = 0;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const = 0;
		virtual void ToBin(NSWasm::CData& oWriter) const = 0;

		static const BYTE kBin_g_nodeAttributeStart = 250;
		static const BYTE kBin_g_nodeAttributeEnd = 251;
	};

	// using template to avoid downcasting
	template <typename T>
	class CBaseItemGroup : public CBaseItem
	{
	public:
		std::vector<std::shared_ptr<T>> m_arItems;

		CBaseItemGroup()
		{
			static_assert(std::is_base_of<CBaseItem, T>::value, "T should has base of CBaseItem!");
		}
		CBaseItemGroup(const CBaseItemGroup<T>& other) : CBaseItemGroup()
		{
			for (const auto value : other.m_arItems)
				m_arItems.push_back(value);
		}
		CBaseItemGroup(CBaseItemGroup<T>&& other) : CBaseItemGroup()
		{
			m_arItems = std::move(other);
		}
		virtual ~CBaseItemGroup() {}

		CBaseItemGroup<T>& operator=(const CBaseItemGroup<T>& other)
		{
			if (this == &other)
				return *this;

			m_arItems.clear();
			for (const auto value : other.m_arItems)
				m_arItems.push_back(value);

			return *this;
		}
		CBaseItemGroup<T>& operator=(CBaseItemGroup<T>&& other)
		{
			if (this == &other)
				return *this;

			m_arItems = std::move(other);
			return *this;
		}

		void AddItem(const std::shared_ptr<T>& pItem)
		{
			CBaseItem::RecalcWithNewItem(pItem.get());
			m_arItems.push_back(pItem);
		}
		void AddItem(std::shared_ptr<T>&& pItem)
		{
			CBaseItem::RecalcWithNewItem(pItem.get());
			m_arItems.push_back(std::move(pItem));
		}
	};

	enum class eBaseItemCmpType
	{
		bictVertical,
		bictHorizontal
	};

	template <eBaseItemCmpType CmpType>
	struct CBaseItemCmp
	{
		bool operator() (const CBaseItem& item1, const CBaseItem& item2) const
		{
			if (CmpType == eBaseItemCmpType::bictVertical)
				return item1.m_dBot < item2.m_dBot;
			if (CmpType == eBaseItemCmpType::bictHorizontal)
				return item1.m_dLeft < item2.m_dLeft;
		}
	};
}
