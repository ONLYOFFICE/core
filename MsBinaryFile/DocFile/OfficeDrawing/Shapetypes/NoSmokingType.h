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
	class NoSmokingType : public ShapeType
	{
	public:
		NoSmokingType () : ShapeType(msosptNoSmoking)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	(L"m,10800qy10800,,21600,10800,10800,21600,,10800xar@0@0@16@16@12@14@15@13xar@0@0@16@16@13@15@14@12xe");

            Formulas.push_back((L"val #0" ));
            Formulas.push_back((L"prod @0 2 1" ));
            Formulas.push_back((L"sum 21600 0 @1" ));
            Formulas.push_back((L"prod @2 @2 1" ));
            Formulas.push_back((L"prod @0 @0 1" ));
            Formulas.push_back((L"sum @3 0 @4" ));
            Formulas.push_back((L"prod @5 1 8" ));
            Formulas.push_back((L"sqrt @6" ));
            Formulas.push_back((L"prod @4 1 8" ));
            Formulas.push_back((L"sqrt @8" ));
            Formulas.push_back((L"sum @7 @9 0" ));
            Formulas.push_back((L"sum @7 0 @9" ));
            Formulas.push_back((L"sum @10 10800 0" ));
            Formulas.push_back((L"sum 10800 0 @10" ));
            Formulas.push_back((L"sum @11 10800 0" ));
            Formulas.push_back((L"sum 10800 0 @11" ));
            Formulas.push_back((L"sum 21600 0 @0" ));

            AdjustmentValues	=	(L"2700" );
            ConnectorLocations	=	(L"10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163" );
            TextBoxRectangle	=	(L"3163,3163,18437,18437" );

			Handle one;
            one.position		=	L"#0,center";
            one.xrange			=	L"0,7200";
			Handles.push_back (one);
		}
	};
}
