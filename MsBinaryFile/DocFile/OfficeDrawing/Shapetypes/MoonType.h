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
	class MoonType : public ShapeType
	{
	public:
		MoonType () : ShapeType(msosptMoon)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	(L"m21600,qx,10800,21600,21600wa@0@10@6@11,21600,21600,21600,xe");

            Formulas.push_back((L"val #0"));
            Formulas.push_back((L"sum 21600 0 #0"));
            Formulas.push_back((L"prod #0 #0 @1"));
            Formulas.push_back((L"prod 21600 21600 @1"));
            Formulas.push_back((L"prod @3 2 1"));
            Formulas.push_back((L"sum @4 0 @2"));
            Formulas.push_back((L"sum @5 0 #0"));
            Formulas.push_back((L"prod @5 1 2"));
            Formulas.push_back((L"sum @7 0 #0"));
            Formulas.push_back((L"prod @8 1 2"));
            Formulas.push_back((L"sum 10800 0 @9"));
            Formulas.push_back((L"sum @9 10800 0"));
            Formulas.push_back((L"prod #0 9598 32768"));
            Formulas.push_back((L"sum 21600 0 @12"));
            Formulas.push_back((L"ellipse @13 21600 10800"));
            Formulas.push_back((L"sum 10800 0 @14"));
            Formulas.push_back((L"sum @14 10800 0"));

			Adjustments.push_back(10800);

            ConnectorLocations	=	(L"21600,0;0,10800;21600,21600;@0,10800");

			ConnectorAngles.push_back(270);
			ConnectorAngles.push_back(180);
			ConnectorAngles.push_back(90);
			ConnectorAngles.push_back(0);

            TextBoxRectangle	=	(L"@12,@15,@0,@16");

			Handle one;
            one.position		=	(L"#0,center");
            one.xrange			=	(L"0,18900");
			Handles.push_back (one);
		}
	};
}
