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
	class BlockArcType : public ShapeType
	{
	public:
		BlockArcType () : ShapeType(msosptBlockArc)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	(L"al10800,10800@0@0@2@14,10800,10800,10800,10800@3@15xe");

            Formulas.push_back((L"val #1"));
            Formulas.push_back((L"val #0"));
            Formulas.push_back((L"sum 0 0 #0"));
            Formulas.push_back((L"sumangle #0 0 180"));
            Formulas.push_back((L"sumangle #0 0 90"));
            Formulas.push_back((L"prod @4 2 1"));
            Formulas.push_back((L"sumangle #0 90 0"));
            Formulas.push_back((L"prod @6 2 1"));
            Formulas.push_back((L"abs #0"));
            Formulas.push_back((L"sumangle @8 0 90"));
            Formulas.push_back((L"if @9 @7 @5"));
            Formulas.push_back((L"sumangle @10 0 360"));
            Formulas.push_back((L"if @10 @11 @10"));
            Formulas.push_back((L"sumangle @12 0 360"));
            Formulas.push_back((L"if @12 @13 @12"));
            Formulas.push_back((L"sum 0 0 @14"));
            Formulas.push_back((L"val 10800"));
            Formulas.push_back((L"sum 10800 0 #1"));
            Formulas.push_back((L"prod #1 1 2"));
            Formulas.push_back((L"sum @18 5400 0"));
            Formulas.push_back((L"cos @19 #0"));
            Formulas.push_back((L"sin @19 #0"));
            Formulas.push_back((L"sum @20 10800 0"));
            Formulas.push_back((L"sum @21 10800 0"));
            Formulas.push_back((L"sum 10800 0 @20"));
            Formulas.push_back((L"sum #1 10800 0"));
            Formulas.push_back((L"if @9 @17 @25"));
            Formulas.push_back((L"if @9 0 21600"));
            Formulas.push_back((L"cos 10800 #0"));
            Formulas.push_back((L"sin 10800 #0"));
            Formulas.push_back((L"sin #1 #0"));
            Formulas.push_back((L"sum @28 10800 0"));
            Formulas.push_back((L"sum @29 10800 0"));
            Formulas.push_back((L"sum @30 10800 0"));
            Formulas.push_back((L"if @4 0 @31"));
            Formulas.push_back((L"if #0 @34 0"));
            Formulas.push_back((L"if @6 @35 @31"));
            Formulas.push_back((L"sum 21600 0 @36"));
            Formulas.push_back((L"if @4 0 @33"));
            Formulas.push_back((L"if #0 @38 @32"));
            Formulas.push_back((L"if @6 @39 0"));
            Formulas.push_back((L"if @4 @32 21600"));
            Formulas.push_back((L"if @6 @41 @33"));

			Adjustments.push_back(11796480);
			Adjustments.push_back(5400);

            ConnectorLocations	=	(L"10800,@27;@22,@23;10800,@26;@24,@23");
            TextBoxRectangle	=	(L"@36,@40,@37,@42");

			Handle one;
            one.position		=	(L"#1,#0");
            one.polar			=	(L"10800,10800");
            one.radiusrange		=	(L"0,10800");
			Handles.push_back (one);
		}
	};
}
