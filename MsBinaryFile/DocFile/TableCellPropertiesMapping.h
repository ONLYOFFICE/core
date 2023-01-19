/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
		bool IsTableBordersDefined (const std::list<SinglePropertyModifier>* grpprl) const;
		bool IsTableCellWidthDefined (const std::list<SinglePropertyModifier>* grpprl) const;
		
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
