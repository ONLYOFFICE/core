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

#include "../Common/Base/XmlTools.h"
#include "BorderCode.h"
#include "PropertiesMapping.h"
#include "TablePropertyExceptions.h"
#include "SprmTDefTable.h"
#include "StyleSheet.h"
#include "StyleSheetMapping.h"
#include "IMapping.h"

namespace DocFileFormat
{
	static const wchar_t WidthType[4][5] =
	{
        L"nil",
        L"auto",
        L"pct",
        L"dxa"
	};

	struct FtsWWidth_Indent
	{
	public:
		FtsWWidth_Indent (unsigned char* buffer) : ftsWidth(0), wWidth(0)
		{
			if (buffer)
			{
				ftsWidth	=	buffer[0];

				if (3 == ftsWidth)			//	ftsDxa
					wWidth	=	FormatUtils::BytesToInt16 (++buffer, 0, 2);
			}
		}

		unsigned char	ftsWidth;
		short			wWidth;
	};

	class TablePropertiesMapping: public PropertiesMapping, public IMapping
	{
	public:
		TablePropertiesMapping( XMLTools::CStringXmlWriter* writer, StyleSheet* styles, std::vector<short>* grid, bool isTableStyleNeeded = true );
		virtual ~TablePropertiesMapping();
		void Apply( IVisitable* visited );

	private: 

        XMLTools::XMLElement* _tblPr;
        XMLTools::XMLElement* _tblGrid;
        XMLTools::XMLElement* _tblBorders;
		
		StyleSheet* _styles;
		std::vector<short>* _grid;
		
		std::shared_ptr<BorderCode> brcLeft;
		std::shared_ptr<BorderCode> brcTop;
		std::shared_ptr<BorderCode> brcBottom;
		std::shared_ptr<BorderCode> brcRight;
		std::shared_ptr<BorderCode> brcHorz;
		std::shared_ptr<BorderCode> brcVert;

		bool _isTableStyleNeeded;
	};
}
