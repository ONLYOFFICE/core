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

#include "PropertiesMapping.h"
#include "TablePropertyExceptions.h"
#include "SprmTDefTable.h"
#include "BorderCode.h"
#include "IMapping.h"

namespace DocFileFormat
{
	typedef enum _VerticalCellAlignment
	{
		vca_top,
		vca_center,
		vca_bottom
	} VerticalCellAlignment;

	static const wchar_t VerticalCellAlignmentMap[3][7] =
	{
        L"top",
        L"center",
        L"bottom"
	};

	class TableCellPropertiesMapping : public PropertiesMapping, public IMapping
	{
	public:	

		virtual ~TableCellPropertiesMapping();
		TableCellPropertiesMapping (XMLTools::CStringXmlWriter* pWriter, const std::vector<short>* grid, int gridIndex, int cellIndex, unsigned int depth);
		virtual void Apply( IVisitable* visited );
	
		inline int GetGridSpan() const
		{
			return _gridSpan;
		}

		inline bool IsCoverCell() const
		{
			return _bCoverCell;
		}

	private:

		void apppendCellShading (unsigned char* sprmArg, int size, int cellIndex);
		bool IsTableBordersDefined (const std::vector<SinglePropertyModifier>* grpprl) const;
		bool IsTableCellWidthDefined (const std::vector<SinglePropertyModifier>* grpprl) const;
		
		int _gridIndex;
		int _cellIndex;
		
        XMLTools::XMLElement* _tcPr;
        XMLTools::XMLElement* _tcMar;
        XMLTools::XMLElement* _tcBorders;
		
		const std::vector<short>*	_grid;
		std::vector<short>			_tGrid;

		int						_width;
		Global::CellWidthType	_ftsWidth;
		TC80					_tcDef;
		unsigned int			_depth;

		std::shared_ptr<BorderCode> _brcLeft;
		std::shared_ptr<BorderCode> _brcTop;
		std::shared_ptr<BorderCode> _brcBottom;
		std::shared_ptr<BorderCode> _brcRight;

		int		_gridSpan;
		bool	_bCoverCell;
	};
}
