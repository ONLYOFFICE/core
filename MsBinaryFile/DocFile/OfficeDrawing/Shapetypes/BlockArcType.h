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

            AdjustmentValues	=	(L"11796480,5400");
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
