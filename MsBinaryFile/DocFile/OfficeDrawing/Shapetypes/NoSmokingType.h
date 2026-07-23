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

			Adjustments.push_back(2700);

            ConnectorLocations	=	(L"10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163" );
            TextBoxRectangle	=	(L"3163,3163,18437,18437" );

			Handle one;
            one.position		=	L"#0,center";
            one.xrange			=	L"0,7200";
			Handles.push_back (one);
		}
	};
}
