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

#include <vector>
#include <memory>

#include "BaseItem.h"
#include "Paragraph.h"

namespace NSDocxRenderer
{
class ITableBuilder
{
public:
	using shape_ptr_t		= std::shared_ptr<CShape>;
	using paragraph_ptr_t	= std::shared_ptr<CParagraph>;
	using ooxml_item_ptr_t	= std::shared_ptr<IOoxmlItem>;

	ITableBuilder() = default;
	~ITableBuilder() = default;

	virtual void SetShapes(std::vector<shape_ptr_t>&& shapes)
	{
		m_arShapes = std::move(shapes);
	}
	virtual void SetParagraphs(std::vector<paragraph_ptr_t>&& paragraphs)
	{
		m_arParagraphs = paragraphs;
	}

	virtual std::vector<shape_ptr_t>&& ReturnShapes()
	{
		return std::move(m_arShapes);
	}
	virtual std::vector<paragraph_ptr_t>&& ReturnParagraphs()
	{
		return std::move(m_arParagraphs);
	}
	virtual std::vector<ooxml_item_ptr_t>&& GetTables()
	{
		return std::move(m_arTables);
	}

public:
	virtual void BuildGraphicallCells() noexcept {};
	virtual void BuildTables() noexcept {};

private:
	std::vector<ooxml_item_ptr_t> m_arTables;
	std::vector<shape_ptr_t>	  m_arShapes;
	std::vector<paragraph_ptr_t>  m_arParagraphs;
};

namespace NSTables {
	ITableBuilder* Create();
}
} // namespace NSDocxRenderer
