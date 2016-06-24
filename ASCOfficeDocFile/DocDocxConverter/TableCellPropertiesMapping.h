/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
		_T( "top" ),
		_T( "center" ),
		_T( "bottom" )
	};

	class TableCellPropertiesMapping : public PropertiesMapping, public IMapping
	{
	public:	

		virtual ~TableCellPropertiesMapping();
		TableCellPropertiesMapping (XmlUtils::CXmlWriter* pWriter, const vector<short>* tableGrid, int gridIndex, int cellIndex);
		virtual void Apply( IVisitable* visited );
	
		inline int GetGridSpan() const
		{
			return _gridSpan;
		}

	private:

		void apppendCellShading (unsigned char* sprmArg, int size, int cellIndex);
		bool IsTableBordersDefined (const list<SinglePropertyModifier>* grpprl) const;
		bool IsTableCellWidthDefined (const list<SinglePropertyModifier>* grpprl) const;
	
	private: 
	
		int _gridIndex;
		int _cellIndex;
		XMLTools::XMLElement<wchar_t>* _tcPr;
		XMLTools::XMLElement<wchar_t>* _tcMar;
		XMLTools::XMLElement<wchar_t>* _tcBorders;
		const vector<short>* _grid;
		vector<short> _tGrid;

		short _width;
		Global::CellWidthType _ftsWidth;
		TC80 _tcDef;

		BorderCode* _brcTop;
		BorderCode* _brcLeft;
		BorderCode* _brcRight;
		BorderCode* _brcBottom;

		/// The grind span of this cell
		int _gridSpan;
	};
}