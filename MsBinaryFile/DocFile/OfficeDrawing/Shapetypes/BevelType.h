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
	class BevelType : public ShapeType
	{
	public:
		BevelType () : ShapeType(msosptBevel)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	(L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0e");

            Formulas.push_back((L"val #0"));
            Formulas.push_back((L"sum width 0 #0"));
            Formulas.push_back((L"sum height 0 #0"));
            Formulas.push_back((L"prod width 1 2"));
            Formulas.push_back((L"prod height 1 2"));
            Formulas.push_back((L"prod #0 1 2"));
            Formulas.push_back((L"prod #0 3 2"));
            Formulas.push_back((L"sum @1 @5 0"));
            Formulas.push_back((L"sum @2 @5 0"));

			Adjustments.push_back(2700);

            ConnectorLocations	=	(L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0");
            TextBoxRectangle	=	(L"@0,@0,@1,@2");

			Handle one;
            one.position		=	(L"#0,topLeft");
            one.xrange			=	(L"0,10800");
			Handles.push_back (one);
       
            Limo				=	(L"10800,10800");
		}
	};
}
