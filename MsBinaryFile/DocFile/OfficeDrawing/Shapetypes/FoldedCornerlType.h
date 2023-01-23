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

#include "../ShapeType.h"

namespace DocFileFormat
{
	class FoldedCornerlType : public ShapeType
	{
	public:
		FoldedCornerlType () : ShapeType(msosptFoldedCorner)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m,l,21600@0,21600,21600@0,21600,xem@0,21600nfl@3@5c@7@9@11@13,21600@0e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 @0");
            Formulas.push_back(L"prod @1 8481 32768");
            Formulas.push_back(L"sum @2 @0 0");
            Formulas.push_back(L"prod @1 1117 32768");
            Formulas.push_back(L"sum @4 @0 0");
            Formulas.push_back(L"prod @1 11764 32768");
            Formulas.push_back(L"sum @6 @0 0");
            Formulas.push_back(L"prod @1 6144 32768");
            Formulas.push_back(L"sum @8 @0 0");
            Formulas.push_back(L"prod @1 20480 32768");
            Formulas.push_back(L"sum @10 @0 0");
            Formulas.push_back(L"prod @1 6144 32768");
            Formulas.push_back(L"sum @12 @0 0");

            AdjustmentValues	=	L"18900";
            TextBoxRectangle	=	L"0,0,21600,@13";

			Handle one;
            one.position		=	L"#0,bottomRight";
            one.xrange			=	L"10800,21600";
			Handles.push_back (one);
		}
	};
}
